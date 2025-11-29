<template>
  <div class="min-h-screen flex flex-col bg-gradient-to-br from-[#EDE8F5] via-[#f5f3f7] to-[#e8e4f0]">
    <!-- Header -->
    <div class="bg-gradient-to-r from-[#7091E6] via-[#5571c6] to-[#3D52A0] shadow-xl">
      <div class="px-4 md:px-8 py-4">
        <div class="flex items-center justify-between">
          <div class="flex items-center gap-3">
            <div class="bg-white/20 p-2 rounded-xl backdrop-blur-sm">
              <span class="material-icons text-white text-3xl">shield</span>
            </div>
            <div>
              <h1 class="font-bold text-xl text-white">Emergency Contact Dashboard</h1>
              <p class="text-sm text-white/80">Monitoring Rider Safety</p>
            </div>
          </div>
          <!-- Log Out Button -->
          <button
            @click="logout"
            class="inline-flex items-center gap-2 bg-red-500 hover:bg-red-600 text-white text-sm font-semibold px-4 py-2 rounded-xl transition-all duration-300 shadow-lg hover:shadow-xl"
          >
            <span class="material-icons text-lg">logout</span>
            <span class="hidden sm:inline">Log Out</span>
          </button>
        </div>
      </div>
    </div>

    <!-- Main Dashboard -->
    <main class="flex-1 px-4 md:px-8 py-6">
      <!-- Top Cards -->
      <div class="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-5 gap-6 mb-6">
        <!-- Rider Status -->
        <div class="bg-[#3D52A0] text-white rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl">
          <span class="material-icons text-3xl">Rider</span>
          <DashboardCard title="Rider Status" :value="riderStatus" :subtitle="riderSubtitle" icon="status" status="success" />
        </div>
        <!-- Current Speed -->
        <div :class="[
          'bg-[#7091E6] text-white rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl',
          isOverSpeed ? 'bg-red-500' : ''
        ]">
          <span class="material-icons text-3xl">Speed</span>
          <DashboardCard title="Current Speed" :value="currentSpeedText" :subtitle="speedSubtitle" icon="speed" status="info" />
        </div>
        <!-- Alertness -->
        <div :class="[
          'rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl',
          alertnessStatus === 'Normal' ? 'bg-[#8697C4] text-white' : 'bg-yellow-400 text-[#3D52A0]'
        ]">
          <span class="material-icons text-3xl">Warning</span>
          <DashboardCard title="Alertness" :value="alertnessStatus" :subtitle="alertnessSubtitle"
            :status="alertnessStatus === 'Normal' ? 'success' : 'warning'" />
        </div>
        <!-- Alcohol Detection -->
        <div :class="[
          'rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl',
          alcoholStatus === 'Safe' ? 'bg-[#3D52A0] text-white' : 'bg-red-500 text-white'
        ]">
          <span class="material-icons text-3xl">Liquor</span>
          <DashboardCard title="Alcohol Detection" :value="alcoholStatus" :subtitle="alcoholSubtitle"
            :status="alcoholStatus === 'Safe' ? 'success' : 'danger'" />
        </div>
        <!-- Crash Detection Card -->
        <div :class="[
          'rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl',
          crashDisplayStatus === 'Stable' ? 'bg-green-600 text-white' : 'bg-red-600 text-white animate-pulse'
        ]">
          <span class="material-icons text-3xl">Vehicle</span>
          <DashboardCard title="Crash Status" :value="crashDisplayMessage"
            :status="crashDisplayStatus === 'Stable' ? 'success' : 'danger'" />
        </div>
      </div>

      <!-- Speed Limit Control -->
      <div class="relative overflow-hidden bg-gradient-to-r from-amber-400 via-yellow-400 to-amber-500 rounded-3xl shadow-2xl p-6 mb-8 transition-all duration-500 hover:shadow-3xl group">
        <div class="absolute inset-0 bg-gradient-to-r from-yellow-300/20 to-amber-400/20 opacity-0 group-hover:opacity-100 transition-opacity duration-500"></div>
        <div class="relative">
          <div class="flex justify-between items-center mb-4">
            <div class="flex items-center gap-3">
              <div class="bg-white/30 backdrop-blur-sm p-2 rounded-xl">
                <span class="material-icons text-3xl text-gray-800">tune</span>
              </div>
              <label for="speed-limit" class="font-bold text-gray-900 text-lg">Set Rider Speed Limit</label>
            </div>
            <div class="relative">
              <div class="absolute inset-0 bg-white/40 blur-xl rounded-2xl"></div>
              <span class="relative text-3xl font-black text-gray-900 bg-white/60 backdrop-blur-md px-6 py-3 rounded-2xl shadow-lg border border-white/50">
                {{ speedLimit }} <span class="text-xl font-semibold">km/h</span>
              </span>
            </div>
          </div>
          <div class="relative">
            <input type="range" min="0" max="120" step="5" v-model.number="speedLimit"
              @change="updateSpeedLimitInFirebase"
              class="w-full h-4 bg-white/40 backdrop-blur-sm rounded-full cursor-pointer focus:outline-none focus:ring-4 focus:ring-white/50 accent-gray-800 transition-all appearance-none slider-thumb" />
            <div class="flex justify-between mt-3 text-sm font-bold text-gray-800">
              <span class="bg-white/30 px-3 py-1 rounded-lg">0 km/h</span>
              <span class="bg-white/30 px-3 py-1 rounded-lg">120 km/h</span>
            </div>
          </div>
          <p class="text-sm text-gray-700 mt-3 text-center font-medium">
            ⚠️ Rider will be alerted when exceeding this speed
          </p>
        </div>
      </div>

      <!-- Emergency Contact Location Tracking -->
      <div class="relative overflow-hidden bg-gradient-to-r from-green-500 via-green-600 to-green-700 rounded-3xl shadow-2xl p-6 mb-8 text-white border-2 border-white/30 hover:shadow-3xl transition-all duration-300">
        <div class="flex flex-col md:flex-row items-start md:items-center justify-between gap-4">
          <div class="flex items-center gap-4">
            <div class="p-4 bg-white/20 rounded-2xl backdrop-blur-sm">
              <span class="material-icons text-4xl">my_location</span>
            </div>
            <div>
              <h3 class="text-2xl font-bold mb-1">Track Rider Location</h3>
              <p class="text-green-100 text-sm">
                {{ isTrackingRider ? 'Tracking active - Your location shared with rider' : 'Enable to see rider\'s real-time location' }}
              </p>
              <p v-if="emergencyContactLocation.lat" class="text-green-200 text-xs mt-1">
                Your Location: {{ formatLatLng(emergencyContactLocation.lat, emergencyContactLocation.lng) }}
              </p>
            </div>
          </div>
          <button
            @click="toggleRiderTracking"
            :class="[
              'inline-flex items-center gap-3 px-6 py-4 rounded-xl font-bold text-lg transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105 whitespace-nowrap',
              isTrackingRider 
                ? 'bg-red-500 hover:bg-red-600 text-white' 
                : 'bg-white hover:bg-gray-100 text-green-600'
            ]"
          >
            <span class="material-icons text-2xl">
              {{ isTrackingRider ? 'location_off' : 'gps_fixed' }}
            </span>
            <span>{{ isTrackingRider ? 'Stop Tracking' : 'Start Tracking' }}</span>
          </button>
        </div>
      </div>

      <!-- Tabs -->
      <div class="mb-6">
        <TabGroup :tabs="['My Location', 'Speed Data', 'Diagnostics']" v-model="activeTab"
          class="bg-[#ADBBD4] rounded-lg p-2 flex flex-wrap gap-2" />
      </div>

      <!-- Tab Content -->
      <div v-if="activeTab === 'My Location'" class="bg-white rounded-lg shadow p-4 md:p-6 mb-4">
        <LocationSection :location="location" :user="user" />
      </div>
      <div v-else-if="activeTab === 'Speed Data'" class="bg-white rounded-lg shadow p-4 md:p-6 mb-4">
        <SpeedDataSection
          :speedData="speedHistory"
          :speedLimit="speedLimit"
          @overspeed="handleOverspeed"
        />
      </div>
      <div v-else-if="activeTab === 'Diagnostics'" class="bg-white rounded-lg shadow p-4 md:p-6 mb-4">
        <DiagnosticsSection :diagnostics="diagnostics" />
      </div>

      <!-- Recent Alerts Section -->
      <h3 class="font-semibold text-lg mb-2 text-[rgb(8,8,8)] flex items-center justify-between">
        <span class="flex items-center gap-2">
          <span class="material-icons text-xl">Notifications</span> Recent Alerts
        </span>
        <button @click="toggleAlerts"
          class="text-sm font-medium text-blue-600 hover:text-blue-800 transition">
          {{ showAlerts ? 'Hide Alerts' : 'Show Alerts' }}
        </button>
      </h3>
      <div v-if="showAlerts" class="bg-white rounded-lg shadow p-4 mb-6 max-h-64 overflow-y-auto">
        <RecentAlerts :alerts="alerts" :crash-events="crashEvents" />
      </div>

      <!-- Recent Trips Preview -->
      <section class="mb-6">
        <h3 class="text-lg font-medium text-gray-800 mb-2">Recent Trips</h3>
        <div v-if="recentTrips.length > 0"
          class="space-y-4 max-h-64 overflow-y-auto bg-white shadow-sm p-4 rounded-md">
          <div v-for="trip in recentTrips" :key="trip.id" class="py-2 border-b last:border-b-0">
            <p class="text-sm text-gray-500">Routes</p>
            <p><strong>From:</strong> {{ trip.startLocationName || formatLatLng(trip.startLat, trip.startLng) }}</p>
            <p><strong>To:</strong> {{ trip.endLocationName || formatLatLng(trip.endLat, trip.endLng) }}</p>
            <p><strong>Max Speed:</strong> {{ trip.maxSpeed || 'N/A' }} km/h</p>
            <a :href="getGoogleMapsLink(trip)" target="_blank" rel="noopener noreferrer"
              class="mt-2 inline-block px-3 py-1 bg-blue-600 text-white text-sm font-medium rounded hover:bg-blue-700 transition mr-2">
              Navigate
            </a>
          </div>
        </div>
        <div v-else class="bg-white shadow-sm p-6 rounded-md text-center text-gray-500">
          No trips found yet.
        </div>
      </section>

      <!-- Crash Events -->
      <section class="mt-8">
        <div v-if="crashEvents.length > 0" class="mt-6">
          <h3 class="font-semibold text-lg mb-2 text-black-600 flex items-center gap-2">
            <span class="material-icons text-xl">Warning!</span> Navigate to Crash Site
          </h3>
          <div class="max-h-64 overflow-y-auto space-y-4 bg-white shadow rounded-lg p-4">
            <div v-for="(event, index) in crashEvents" :key="index" class="border-b pb-2 last:border-b-0">
              <p><strong>Impact:</strong> {{ event.impactStrength }} g</p>
              <p><strong>Location:</strong> {{ event.location }}</p>
              <a :href="getGoogleMapsLink(event.lat, event.lng)" target="_blank" rel="noopener noreferrer"
                class="mt-2 inline-block px-3 py-1 bg-green-600 text-white text-sm font-medium rounded hover:bg-green-700 transition">
                See Location
              </a>
            </div>
          </div>
        </div>
      </section>
    </main>

    <!-- Footer -->
    <footer class="bg-[#ADBBD4] text-center py-4 text-[#3D52A0] text-sm border-t mt-auto">
      © 2025 VIGILERT. All rights reserved.
    </footer>
  </div>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue';
import { useRouter } from 'vue-router';
import { database } from '../firebase/config';
import { ref as dbRef, set, onValue, onChildAdded, remove } from 'firebase/database';

// Components
import TabGroup from '../components/TabGroup.vue';
import DashboardCard from '../components/DashboardCard.vue';
import LocationSection from '../components/LocationSection.vue';
import SpeedDataSection from '../components/SpeedDataSection.vue';
import DiagnosticsSection from '../components/DiagnosticsSection.vue';
import RecentAlerts from '../components/RecentAlerts.vue';

const router = useRouter();

// States
const riderStatus = ref('Inactive');
const riderSubtitle = ref('Helmet not connected');
const alertnessStatus = ref('Normal');
const alertnessSubtitle = ref('No drowsiness detected');
const alcoholStatus = ref('Safe');
const alcoholSubtitle = ref('No alcohol detected');
const currentSpeed = ref(0);
const speedHistory = ref([]);
const speedLimit = ref(80); // Default value
const speedSubtitle = ref('GPS Module')
const diagnostics = ref([]);
const alerts = ref([]);
const activeTab = ref('Speed Data');
const location = ref({ lat: null, lng: null });
const user = ref({ name: 'Loading...' });
const recentTrips = ref([]);
const crashEvents = ref([]);
const isOverSpeed = ref(false);
const showAlerts = ref(true);

// Crash UI
const crashDisplayStatus = ref('Stable'); // Stable | Alerting
const crashDisplayMessage = ref('Vehicle Stable');
let crashInterval = null;
let flashCount = 0;
let lastCrashTimestamp = null;

const userId = 'MnzBjTBslZNijOkq732PE91hHa23'; // Firebase UID

// Emergency Contact Tracking States
const isTrackingRider = ref(false);
const emergencyContactLocation = ref({ lat: null, lng: null });
let gpsWatchId = null;

// Helpers
const formatLatLng = (lat, lng) => {
  return lat && lng ? `${Number(lat).toFixed(6)}, ${Number(lng).toFixed(6)}` : 'N/A';
};

const currentSpeedText = computed(() => currentSpeed.value.toFixed(2) + ' kph');

const getGoogleMapsLink = (tripOrLat, lng = undefined) => {
  let startLat, startLng, endLat, endLng;
  if (lng === undefined) {
    startLat = parseFloat(tripOrLat.startLat);
    startLng = parseFloat(tripOrLat.startLng);
    endLat = parseFloat(tripOrLat.endLat);
    endLng = parseFloat(tripOrLat.endLng);
  } else {
    startLat = parseFloat(tripOrLat);
    startLng = parseFloat(lng);
    endLat = startLat;
    endLng = startLng;
  }
  const isValidCoord = (lat, lng) =>
    lat !== undefined && lng !== undefined && !isNaN(lat) && !isNaN(lng);
  if (!isValidCoord(startLat, startLng)) return 'https://www.google.com/maps'; 
  const zoomLevel = 14;
  return `https://www.google.com/maps/dir/?api=1&origin=${startLat},${startLng}&destination=${endLat},${endLng}&zoom=${zoomLevel}`;
};

// Play alert sound
const playSound = () => {
  try {
    const audio = new Audio('/sounds/alert.mp3');
    audio.play().catch(err => {
      console.warn("Audio playback failed:", err);
    });
  } catch (err) {
    console.error("Error creating audio object:", err);
  }
};

// Flash Crash Message
const flashCrashMessage = () => {
  clearInterval(crashInterval);
  flashCount = 0;
  crashDisplayStatus.value = 'Alerting';
  crashDisplayMessage.value = 'Crash Detected';
  playSound();
  alerts.value.unshift({
    type: 'danger',
    message: 'Crash Detected',
    details: 'Impact Strength: High',
    time: new Date().toLocaleTimeString()
  });
  if (alerts.value.length > 5) alerts.value.pop();
  crashInterval = setInterval(() => {
    if (flashCount >= 3) {
      clearInterval(crashInterval);
      crashDisplayStatus.value = 'Stable';
      crashDisplayMessage.value = 'Vehicle Stable';
    }
    crashDisplayMessage.value = crashDisplayMessage.value === 'Crash Detected' ? 'Vehicle Stable' : 'Crash Detected';
    flashCount++;
  }, 2000);
};

// Handle Overspeed Event
const handleOverspeed = (payload) => {
  playSound(); // Triggers alert sound
  alerts.value.unshift({
    type: 'danger',
    message: 'Speed Limit Exceeded!',
    details: `Current: ${payload.speed} kph | Limit: ${payload.limit} kph`,
    time: new Date().toLocaleTimeString()
  });
  if (alerts.value.length > 5) alerts.value.pop();
};

// Firebase References
const helmetPublicRef = dbRef(database, `helmet_public/${userId}`);
const helmetRef = dbRef(database, `helmet_public/${userId}/helmetStatus/status`);
const tripsRef = dbRef(database, `helmet_public/${userId}/trips`);
const crashRef = dbRef(database, `helmet_public/${userId}/crashes`);
const alcoholRef = dbRef(database, `helmet_public/${userId}/alcohol/status`);
const speedLimitRef = dbRef(database, `helmet_public/${userId}/settings/speedLimit`);

onMounted(() => {
  const storedLastCrashTime = localStorage.getItem(`lastCrashTimestamp_${userId}`);
  if (storedLastCrashTime) lastCrashTimestamp = parseInt(storedLastCrashTime);

  onValue(speedLimitRef, (snapshot) => {
    const data = snapshot.val();
    if (data !== null) speedLimit.value = data;
  });

  initializeCrashListener();

  onValue(alcoholRef, (snapshot) => {
    const data = snapshot.val();
    if (data && data.status === "Danger") {
      alcoholStatus.value = 'Danger';
      alcoholSubtitle.value = `Alcohol Detected! Value: ${data.sensorValue}`;
      alerts.value.unshift({
        type: 'danger',
        message: 'Alcohol Detected!',
        details: `Sensor Value: ${data.sensorValue}`,
        time: new Date().toLocaleTimeString()
      });
      playSound();
      if (alerts.value.length > 5) alerts.value.pop();
    } else {
      alcoholStatus.value = 'Safe';
      alcoholSubtitle.value = 'No alcohol detected';
    }
  });

  onValue(helmetPublicRef, (snapshot) => {
    const data = snapshot.val();
    if (data?.live) {
      const liveData = data.live;
      location.value = {
        lat: Number(liveData.locationLat),
        lng: Number(liveData.locationLng)
      };
      const rawSpeed = parseFloat(liveData.speed) || 0;
      currentSpeed.value = rawSpeed < 0.1 ? 0 : rawSpeed;
      speedHistory.value.push(currentSpeed.value);
      if (speedHistory.value.length > 10) speedHistory.value.shift();

      const isOverspeed = currentSpeed.value > speedLimit.value;
      isOverSpeed.value = isOverspeed;

      if (isOverspeed) {
        handleOverspeed({
          speed: currentSpeed.value,
          limit: speedLimit.value
        });
      }
    }
  });

  onValue(helmetRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      riderStatus.value = data.helmetConnected ? 'Active' : 'Inactive';
      riderSubtitle.value = data.helmetConnected ? 'Helmet connected' : 'Helmet not connected';
      alertnessStatus.value = data.alertnessStatus || 'Normal';
      alertnessSubtitle.value = alertnessStatus.value === 'Normal' ? 'No drowsiness detected' : 'Drowsiness detected!';
      alcoholStatus.value = data.alcoholLevel > 0.05 ? 'Danger' : 'Safe';
      alcoholSubtitle.value = alcoholStatus.value === 'Danger'
        ? `Alcohol detected: ${data.alcoholLevel.toFixed(2)}%`
        : 'No alcohol detected';
      if (alertnessStatus.value !== 'Normal' || alcoholStatus.value === 'Danger') {
        alerts.value.unshift({
          type: 'danger',
          message: alertnessStatus.value !== 'Normal' ? 'Drowsiness Detected' : 'Alcohol Detected!',
          details: alertnessStatus.value !== 'Normal' ? alertnessSubtitle.value : alcoholSubtitle.value,
          time: new Date().toLocaleTimeString()
        });
        playSound();
        if (alerts.value.length > 5) alerts.value.pop();
      }
    }
  });

  onValue(tripsRef, async (snapshot) => {
    const data = snapshot.val();
    if (data) {
      const tripList = Object.entries(data).map(([id, trip]) => ({ id, ...trip }));
      tripList.sort((a, b) => {
        const aTime = typeof a.startTime === 'string' ? parseInt(a.startTime) : a.startTime;
        const bTime = typeof b.startTime === 'string' ? parseInt(b.startTime) : b.startTime;
        return bTime - aTime;
      });
      recentTrips.value = tripList.slice(0, 5);
    }
  });
});

// Show/Hide Alerts Panel
const toggleAlerts = () => {
  showAlerts.value = !showAlerts.value;
};

// Update speed limit in Firebase
const updateSpeedLimitInFirebase = () => {
  set(dbRef(database, `helmet_public/${userId}/settings/speedLimit`), speedLimit.value).catch(error => {
    console.error("Failed to update speed limit:", error);
  });
};

// Initialize crash event listener
const initializeCrashListener = () => {
  onChildAdded(crashRef, (snapshot) => {
    const event = snapshot.val();
    if (!event || !event.timestamp || typeof event.roll !== 'number') {
      console.warn("Invalid crash event received", event);
      return;
    }
    const eventTime = event.timestamp;
    const rollTriggered = event.roll < -40 || event.roll > 40;
    if (
      rollTriggered &&
      (!lastCrashTimestamp || eventTime > lastCrashTimestamp)
    ) {
      lastCrashTimestamp = eventTime;
      localStorage.setItem(`lastCrashTimestamp_${userId}`, eventTime.toString());
      crashEvents.value.push({
        timestamp: eventTime,
        impactStrength: "N/A",
        location: event.hasGPS ? `${event.lat}, ${event.lng}` : 'No GPS',
        lat: event.lat,
        lng: event.lng
      });
      flashCrashMessage();
    }
  }, (error) => {
    console.error("Firebase crash listener error:", error);
  });
};

// Toggle Rider Tracking
const toggleRiderTracking = () => {
  if (isTrackingRider.value) {
    stopRiderTracking();
  } else {
    startRiderTracking();
  }
};

// Start tracking rider's location
const startRiderTracking = () => {
  if (!navigator.geolocation) {
    alert('Geolocation is not supported by your browser');
    return;
  }

  isTrackingRider.value = true;

  // Get emergency contact's location and share it
  gpsWatchId = navigator.geolocation.watchPosition(
    (position) => {
      const lat = position.coords.latitude;
      const lng = position.coords.longitude;
      
      emergencyContactLocation.value = { lat, lng };
      
      // Save emergency contact location to Firebase so rider can see it
      set(dbRef(database, `helmet_public/${userId}/emergencyContact/location`), {
        lat,
        lng,
        timestamp: Date.now(),
        tracking: true
      }).catch(error => {
        console.error('Failed to update emergency contact location:', error);
      });
      
      console.log('[Emergency Contact] Location shared:', lat, lng);
    },
    (error) => {
      console.error('GPS tracking error:', error);
      let errorMessage = 'Unable to track location. ';
      
      switch(error.code) {
        case error.PERMISSION_DENIED:
          errorMessage += 'Please allow location access in your browser settings.';
          break;
        case error.POSITION_UNAVAILABLE:
          errorMessage += 'Location information is unavailable.';
          break;
        case error.TIMEOUT:
          errorMessage += 'Location request timed out.';
          break;
        default:
          errorMessage += 'An unknown error occurred.';
      }
      
      alert(errorMessage);
      stopRiderTracking();
    },
    {
      enableHighAccuracy: true,
      timeout: 10000,
      maximumAge: 0
    }
  );
  
  alerts.value.unshift({
    type: 'success',
    message: 'Rider Tracking Active',
    details: 'You can now see rider\'s real-time location',
    time: new Date().toLocaleTimeString()
  });
  if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
};

// Stop tracking rider's location
const stopRiderTracking = () => {
  if (gpsWatchId !== null) {
    navigator.geolocation.clearWatch(gpsWatchId);
    gpsWatchId = null;
    isTrackingRider.value = false;
    
    // Update Firebase to indicate tracking stopped
    set(dbRef(database, `helmet_public/${userId}/emergencyContact/location`), {
      lat: emergencyContactLocation.value.lat,
      lng: emergencyContactLocation.value.lng,
      timestamp: Date.now(),
      tracking: false
    }).catch(error => {
      console.error('Failed to update tracking status:', error);
    });
    
    alerts.value.unshift({
      type: 'info',
      message: 'Rider Tracking Stopped',
      details: 'Location sharing disabled',
      time: new Date().toLocaleTimeString()
    });
    if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
  }
};

// Logout Function for Emergency Contact
const logout = () => {
  // Stop tracking before logout
  if (isTrackingRider.value) {
    stopRiderTracking();
  }
  router.push({ name: 'EmergencyContactLogin' }); // Make sure this route exists
};
</script>