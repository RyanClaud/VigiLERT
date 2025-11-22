# How to Add Crash Markers to Your Map

## Problem
The crash markers aren't showing on the map because the `crashEvents` prop isn't being passed to the LocationSection component.

## Solution

### Step 1: Find Where the Map is Rendered

Based on your screenshot, you have a map showing. We need to find where it's rendered. Check these locations:

1. **Dashboard.vue** - Search for "My Location" tab content
2. **Look for** `<LocationSection` or any component with `:location`
3. **Or look for** inline Leaflet map code

### Step 2: Pass Crash Events to the Map Component

Once you find where the map is rendered, update it to pass crash events:

**If using LocationSection:**
```vue
<LocationSection 
  :location="location" 
  :crash-events="crashEvents"
  @update-location="handleLocationUpdate" 
/>
```

**If using inline map:**
You'll need to add the crash marker code directly.

### Step 3: Verify Crash Events Exist

In your Dashboard.vue script section, make sure you have:

```javascript
const crashEvents = ref([]);

// In onMounted or wherever you load data:
const crashRef = dbRef(database, `helmet_public/${userId}/crashes`);
onValue(crashRef, (snapshot) => {
  const data = snapshot.val();
  if (data) {
    const events = Object.entries(data).map(([id, crash]) => ({
      id,
      ...crash
    }));
    crashEvents.value = events;
    console.log('[DEBUG] Loaded crash events:', events);
  }
});
```

### Step 4: Test

1. Open browser console (F12)
2. Look for: `[DEBUG] Loaded crash events: [...]`
3. Should see array of crash objects with lat/lng
4. Look for: `[MAP] Added X crash markers`

## Quick Debug Steps

### Debug 1: Check if Crash Events are Loaded

Open browser console and type:
```javascript
// This will show if crash events exist in your Dashboard
console.log('Crash events:', crashEvents.value);
```

### Debug 2: Check Firebase Data

1. Open Firebase Console
2. Go to Realtime Database
3. Navigate to: `helmet_public/{yourUserId}/crashes`
4. Verify crash data exists with `lat` and `lng` fields

### Debug 3: Check if LocationSection Receives Props

Add this to LocationSection.vue script:
```javascript
watch(() => props.crashEvents, (newVal) => {
  console.log('[LocationSection] Crash events received:', newVal);
}, { immediate: true });
```

## Manual Integration (If Needed)

If you can't find where the map is rendered, here's how to add crash markers manually:

### Option 1: Update Existing Map Code

Find where your map is initialized (look for `L.map(...)`) and add this after the map is created:

```javascript
// After map initialization
const crashMarkers = [];

function addCrashMarkers(crashes) {
  // Remove old markers
  crashMarkers.forEach(m => m.remove());
  crashMarkers.length = 0;
  
  // Add new markers
  crashes.forEach(crash => {
    if (crash.lat && crash.lng) {
      const marker = L.marker([crash.lat, crash.lng], {
        icon: L.divIcon({
          className: 'crash-marker',
          html: '<div style="background: red; width: 30px; height: 30px; border-radius: 50%; border: 3px solid white; display: flex; align-items: center; justify-content: center; color: white; font-weight: bold;">⚠</div>',
          iconSize: [30, 30],
          iconAnchor: [15, 15]
        })
      }).addTo(map);
      
      marker.bindPopup(`
        <b>⚠️ CRASH DETECTED</b><br>
        Location: ${crash.lat.toFixed(6)}, ${crash.lng.toFixed(6)}<br>
        Impact: ${crash.impactStrength || 'N/A'} m/s²<br>
        Time: ${new Date(crash.timestamp).toLocaleString()}
      `);
      
      crashMarkers.push(marker);
    }
  });
}

// Call this when crash events update
watch(() => crashEvents.value, (newCrashes) => {
  addCrashMarkers(newCrashes);
});
```

### Option 2: Use the Updated LocationSection

1. Make sure you're using the updated `LocationSection.vue` (with crash markers code)
2. Pass crash events as prop
3. Markers will appear automatically

## Expected Result

When working correctly, you should see:

1. **Red warning markers** (⚠️) at crash locations
2. **Your current location** as a blue/red pin
3. **Click crash marker** to see popup with details
4. **Console logs**:
   ```
   [DEBUG] Loaded crash events: [{lat: 12.747, lng: 121.482, ...}]
   [MAP] Added 1 crash markers
   ```

## Still Not Working?

### Check 1: Is the map element the right one?

The LocationSection uses `id="location-section-map"`. Make sure this matches your HTML.

### Check 2: Are crash events in the right format?

Each crash should have:
```javascript
{
  lat: 12.747488,      // Required
  lng: 121.482131,     // Required
  timestamp: 1732248246000,
  impactStrength: 15.23,
  roll: 52.4
}
```

### Check 3: Is Leaflet loaded?

Check console for Leaflet errors. Should see no errors related to `L is not defined`.

## Contact Points

If still not working, check:
1. Browser console for errors
2. Firebase Console for crash data
3. Network tab to see if data is being fetched
4. Vue DevTools to see component props

The crash markers feature is fully implemented in LocationSection.vue - you just need to make sure it's being used and receiving the crash events data!
