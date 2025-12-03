# Phase 2: Route Lines Implementation Guide

## 🎯 Overview

This guide shows how to add route polylines to the map, displaying the actual path taken during trips.

## ✅ What Phase 1 Delivered (Already Done!)

- ✅ Reverse geocoding (addresses instead of coordinates)
- ✅ Crash markers with detailed popups
- ✅ Dynamic icons (🏍️ for GPS module, 👤 for phone GPS)

## 🚀 Phase 2: Route Lines

### What You'll Get:
- Blue lines showing the path taken during trips
- Multiple trip routes on the map
- Trip selection to show/hide routes
- Start/end markers for each trip

---

## 📋 Implementation Steps

### Step 1: Update Arduino Code (2 hours)

#### Add Waypoint Tracking Variables

Add these variables to `MotorcycleToWebApplication_FIXED_COMPLETE.ino`:

```cpp
// ✅ NEW: Trip tracking with waypoints
bool isTripActive = false;
unsigned long tripStartTime = 0;
unsigned long lastWaypointTime = 0;
const unsigned long WAYPOINT_INTERVAL = 10000; // Save waypoint every 10 seconds
int waypointCount = 0;
const int MAX_WAYPOINTS = 100; // Limit waypoints per trip
String currentTripId = "";

// Waypoint buffer (store temporarily before sending)
struct Waypoint {
  float lat;
  float lng;
  unsigned long timestamp;
};
Waypoint waypointBuffer[10]; // Buffer for batch sending
int bufferIndex = 0;
```

#### Add Trip Start Function

```cpp
void startTrip() {
  if (isTripActive) return;
  
  isTripActive = true;
  tripStartTime = millis();
  waypointCount = 0;
  bufferIndex = 0;
  currentTripId = String(tripStartTime);
  
  Serial.println("[TRIP] Started - ID: " + currentTripId);
  
  // Save trip start to Firebase
  if (gps.location.isValid()) {
    StaticJsonDocument<256> doc;
    doc["tripId"] = currentTripId;
    doc["startLat"] = gps.location.lat();
    doc["startLng"] = gps.location.lng();
    doc["startTime"] = tripStartTime;
    doc["gpsSource"] = "module";
    
    String payload;
    serializeJson(doc, payload);
    
    HTTPClient http;
    String path = "/helmet_public/" + userUID + "/trips/" + currentTripId + ".json?auth=" + firebaseAuth;
    http.begin(firebaseHost + path);
    http.addHeader("Content-Type", "application/json");
    http.PUT(payload);
    http.end();
  }
}
```

#### Add Waypoint Recording Function

```cpp
void recordWaypoint() {
  if (!isTripActive || !gps.location.isValid()) return;
  
  unsigned long now = millis();
  
  // Check if it's time to record a waypoint
  if (now - lastWaypointTime < WAYPOINT_INTERVAL) return;
  if (waypointCount >= MAX_WAYPOINTS) return;
  
  lastWaypointTime = now;
  
  // Add to buffer
  waypointBuffer[bufferIndex].lat = gps.location.lat();
  waypointBuffer[bufferIndex].lng = gps.location.lng();
  waypointBuffer[bufferIndex].timestamp = now;
  bufferIndex++;
  waypointCount++;
  
  Serial.printf("[TRIP] Waypoint %d recorded: %.6f, %.6f\n", 
    waypointCount, gps.location.lat(), gps.location.lng());
  
  // Send batch when buffer is full
  if (bufferIndex >= 10) {
    sendWaypointBatch();
  }
}

void sendWaypointBatch() {
  if (bufferIndex == 0) return;
  
  StaticJsonDocument<1024> doc;
  JsonArray waypoints = doc.createNestedArray("waypoints");
  
  for (int i = 0; i < bufferIndex; i++) {
    JsonObject wp = waypoints.createNestedObject();
    wp["lat"] = waypointBuffer[i].lat;
    wp["lng"] = waypointBuffer[i].lng;
    wp["timestamp"] = waypointBuffer[i].timestamp;
  }
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/trips/" + currentTripId + "/waypoints.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  http.POST(payload);
  http.end();
  
  Serial.printf("[TRIP] Sent batch of %d waypoints\n", bufferIndex);
  bufferIndex = 0;
}
```

#### Add Trip End Function

```cpp
void endTrip() {
  if (!isTripActive) return;
  
  // Send remaining waypoints
  if (bufferIndex > 0) {
    sendWaypointBatch();
  }
  
  // Update trip end info
  if (gps.location.isValid()) {
    StaticJsonDocument<256> doc;
    doc["endLat"] = gps.location.lat();
    doc["endLng"] = gps.location.lng();
    doc["endTime"] = millis();
    doc["waypointCount"] = waypointCount;
    
    String payload;
    serializeJson(doc, payload);
    
    HTTPClient http;
    String path = "/helmet_public/" + userUID + "/trips/" + currentTripId + ".json?auth=" + firebaseAuth;
    http.begin(firebaseHost + path);
    http.addHeader("Content-Type", "application/json");
    http.PATCH(payload);
    http.end();
  }
  
  Serial.println("[TRIP] Ended - Total waypoints: " + String(waypointCount));
  isTripActive = false;
}
```

#### Update Main Loop

Add to `loop()`:

```cpp
void loop() {
  // ... existing code ...
  
  // ✅ NEW: Trip tracking
  if (engineRunning && !isTripActive) {
    startTrip();
  }
  
  if (isTripActive) {
    recordWaypoint();
  }
  
  if (!engineRunning && isTripActive) {
    endTrip();
  }
  
  // ... rest of code ...
}
```

---

### Step 2: Update Dashboard (1 hour)

#### Update LocationSection.vue

Add these functions to `src/components/LocationSection.vue`:

```javascript
// ✅ NEW: Trip route management
const tripRoutes = ref([]);
const selectedTrips = ref(new Set());
let routePolylines = [];

// Fetch trip data with waypoints
async function loadTripRoutes() {
  if (!props.user?.uid) return;
  
  try {
    const tripsRef = dbRef(database, `helmet_public/${props.user.uid}/trips`);
    const snapshot = await get(tripsRef);
    
    if (snapshot.exists()) {
      const trips = [];
      snapshot.forEach((childSnapshot) => {
        const trip = childSnapshot.val();
        if (trip.waypoints && trip.waypoints.length > 0) {
          trips.push({
            id: childSnapshot.key,
            ...trip
          });
        }
      });
      
      tripRoutes.value = trips;
      console.log('[Routes] Loaded trips:', trips.length);
    }
  } catch (error) {
    console.error('[Routes] Error loading trips:', error);
  }
}

// Draw route polyline on map
function drawTripRoute(trip) {
  if (!mapInstance || !trip.waypoints) return;
  
  const coordinates = trip.waypoints.map(wp => [wp.lat, wp.lng]);
  
  // Create polyline
  const polyline = L.polyline(coordinates, {
    color: '#3D52A0',
    weight: 4,
    opacity: 0.7,
    smoothFactor: 1
  }).addTo(mapInstance);
  
  // Add start marker
  const startMarker = L.marker([trip.startLat, trip.startLng], {
    icon: L.divIcon({
      html: '<div style="background: #10b981; color: white; width: 30px; height: 30px; border-radius: 50%; display: flex; align-items: center; justify-content: center; font-weight: bold; border: 2px solid white;">S</div>',
      className: 'start-marker',
      iconSize: [30, 30]
    })
  }).addTo(mapInstance);
  
  // Add end marker
  const endMarker = L.marker([trip.endLat, trip.endLng], {
    icon: L.divIcon({
      html: '<div style="background: #ef4444; color: white; width: 30px; height: 30px; border-radius: 50%; display: flex; align-items: center; justify-content: center; font-weight: bold; border: 2px solid white;">E</div>',
      className: 'end-marker',
      iconSize: [30, 30]
    })
  }).addTo(mapInstance);
  
  // Bind popup
  polyline.bindPopup(`
    <div style="min-width: 200px;">
      <h3 style="font-weight: bold; margin-bottom: 8px;">🏍️ Trip Route</h3>
      <p style="margin: 4px 0;"><strong>Start:</strong> ${trip.startAddress || 'Loading...'}</p>
      <p style="margin: 4px 0;"><strong>End:</strong> ${trip.endAddress || 'Loading...'}</p>
      <p style="margin: 4px 0;"><strong>Waypoints:</strong> ${trip.waypoints.length}</p>
    </div>
  `);
  
  routePolylines.push({ id: trip.id, polyline, startMarker, endMarker });
  
  // Geocode addresses if not cached
  if (!trip.startAddress) {
    reverseGeocode(trip.startLat, trip.startLng).then(address => {
      trip.startAddress = address;
    });
  }
  if (!trip.endAddress) {
    reverseGeocode(trip.endLat, trip.endLng).then(address => {
      trip.endAddress = address;
    });
  }
}

// Toggle trip visibility
function toggleTripRoute(tripId) {
  if (selectedTrips.value.has(tripId)) {
    // Remove route
    selectedTrips.value.delete(tripId);
    const route = routePolylines.find(r => r.id === tripId);
    if (route) {
      route.polyline.remove();
      route.startMarker.remove();
      route.endMarker.remove();
      routePolylines = routePolylines.filter(r => r.id !== tripId);
    }
  } else {
    // Add route
    selectedTrips.value.add(tripId);
    const trip = tripRoutes.value.find(t => t.id === tripId);
    if (trip) {
      drawTripRoute(trip);
    }
  }
}

// Load trips on mount
onMounted(() => {
  initMap();
  loadTripRoutes();
});
```

#### Add Trip Selection UI

Add this to the template in `LocationSection.vue`:

```vue
<!-- Trip Route Selector -->
<div v-if="tripRoutes.length > 0" class="bg-gradient-to-r from-purple-500/10 to-blue-500/10 rounded-2xl p-4 border border-purple-500/20">
  <div class="flex items-center gap-3 mb-3">
    <div class="bg-purple-500 p-2 rounded-xl">
      <span class="material-icons text-white text-2xl">route</span>
    </div>
    <div>
      <h4 class="text-sm font-bold text-gray-800">Trip Routes</h4>
      <p class="text-xs text-gray-500">Select trips to show on map</p>
    </div>
  </div>
  
  <div class="space-y-2 max-h-48 overflow-y-auto">
    <div v-for="trip in tripRoutes.slice(0, 5)" :key="trip.id"
         @click="toggleTripRoute(trip.id)"
         :class="[
           'p-3 rounded-xl cursor-pointer transition-all duration-300',
           selectedTrips.has(trip.id) 
             ? 'bg-purple-500 text-white' 
             : 'bg-white hover:bg-gray-50'
         ]">
      <div class="flex items-center justify-between">
        <div class="flex-1">
          <p class="text-sm font-semibold">
            {{ new Date(trip.startTime).toLocaleDateString() }}
          </p>
          <p class="text-xs opacity-80">
            {{ trip.waypointCount || trip.waypoints?.length || 0 }} waypoints
          </p>
        </div>
        <span class="material-icons">
          {{ selectedTrips.has(trip.id) ? 'visibility' : 'visibility_off' }}
        </span>
      </div>
    </div>
  </div>
</div>
```

---

### Step 3: Firebase Structure

The data will be stored like this:

```json
{
  "helmet_public": {
    "USER_ID": {
      "trips": {
        "1234567890": {
          "tripId": "1234567890",
          "startLat": 14.5995,
          "startLng": 120.9842,
          "startAddress": "Quezon Ave, Manila",
          "startTime": 1234567890,
          "endLat": 14.6020,
          "endLng": 120.9870,
          "endAddress": "EDSA, Quezon City",
          "endTime": 1234569890,
          "gpsSource": "module",
          "waypointCount": 45,
          "waypoints": [
            {"lat": 14.5995, "lng": 120.9842, "timestamp": 1234567890},
            {"lat": 14.6000, "lng": 120.9850, "timestamp": 1234567900},
            {"lat": 14.6010, "lng": 120.9860, "timestamp": 1234567910}
          ]
        }
      }
    }
  }
}
```

---

## 🎨 Visual Result

```
Map with:
- 🏍️ Current location (blue motorcycle icon)
- ⚠️ Crash markers (red warning icons)
- ━━━ Blue route lines showing trip paths
- 🟢 S - Start marker (green)
- 🔴 E - End marker (red)
```

---

## ⚠️ Important Considerations

### 1. **Data Storage**
- Each waypoint = ~50 bytes
- 100 waypoints = ~5KB per trip
- 100 trips = ~500KB
- Firebase free tier: 1GB storage ✅

### 2. **Performance**
- Limit to 100 waypoints per trip
- Show max 5 trips on map at once
- Use batch sending (10 waypoints at a time)
- Sample every 10 seconds (not every second)

### 3. **Battery Impact**
- GPS already running ✅
- Minimal extra processing
- Batch sending reduces WiFi usage
- Negligible battery impact

### 4. **Network Usage**
- ~500 bytes per waypoint batch
- ~5KB per trip total
- Acceptable for WiFi ✅

---

## 🧪 Testing Plan

### Test 1: Short Trip
1. Start engine
2. Ride for 2 minutes
3. Stop engine
4. Check Firebase for waypoints
5. Verify route appears on map

### Test 2: Long Trip
1. Start engine
2. Ride for 30 minutes
3. Check waypoint count (should be ~180)
4. Verify route line is smooth

### Test 3: Multiple Trips
1. Take 3 different trips
2. Open dashboard
3. Toggle each trip on/off
4. Verify routes don't overlap confusingly

---

## 📊 Estimated Effort

| Task | Time | Difficulty |
|------|------|-----------|
| Arduino waypoint tracking | 1.5h | Medium |
| Dashboard route display | 1h | Easy |
| Testing & debugging | 1h | Medium |
| **Total** | **3.5h** | **Medium** |

---

## 🎯 Summary

**Phase 1 (Done)**: Addresses, crash markers, dynamic icons ✅

**Phase 2 (This Guide)**: Route polylines showing trip paths

**Benefits**:
- See actual routes taken
- Visualize trip history
- Better trip analysis
- Professional appearance

**When to Implement**:
- When you have time for Arduino updates
- After testing Phase 1 features
- When you want trip visualization

**Phase 1 already provides huge value!** Phase 2 is optional enhancement for trip route visualization.
