<template>
  <div class="min-h-screen flex flex-col bg-[#EDE8F5]">
    <!-- User Greeting/Header -->
    <div class="flex flex-col items-center justify-center py-4">
      <div class="flex items-center gap-2 bg-[#7091E6] px-6 py-2 rounded-xl shadow text-white">
        <span v-if="authStore.user && authStore.user.photoURL">
          <img :src="authStore.user.photoURL" alt="User Avatar"
            class="w-8 h-8 rounded-full border-2 border-white shadow"
            @error="onAvatarError" />
        </span>
        <span v-else class="material-icons text-2xl">account_circle</span>
        <span class="font-semibold text-lg">
          Welcome,
          <span v-if="authStore.user?.displayName">{{ authStore.user.displayName }}</span>
          <span v-else-if="authStore.user?.email">{{ authStore.user.email }}</span>
          <span v-else>User</span>
        </span>
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
      <div class="bg-yellow-400 rounded-lg shadow p-4 mb-8 mx-2 md:mx-8">
        <div class="flex justify-between items-center">
          <label for="speed-limit" class="font-medium text-gray-700">Set Speed Limit</label>
          <span class="text-xl font-bold text-[#0c0c0c]">{{ speedLimit }} km/h</span>
        </div>
        <input type="range" min="1" max="120" step="5" v-model.number="speedLimit"
          @change="updateSpeedLimitInFirebase"
          class="w-full h-2 bg-blue-00 rounded-lg cursor-pointer focus:outline-none focus:ring-2 accent-[#2701fd]" />
        <div class="flex justify-between mt-1 text-xs text-gray-900">
          <span>1 km/h</span>
          <span>120 km/h</span>
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
        <SpeedDataSection :speedData="speedHistory" :speedLimit="speedLimit" :isOverSpeed="isOverSpeed" />
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
            <button @click="deleteTrip(trip.id)"
              class="inline-block mt-2 px-3 py-1 bg-red-600 text-white text-sm font-medium rounded hover:bg-red-700 transition">
              Delete
            </button>
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
                class="inline-block mt-2 px-3 py-1 bg-green-600 text-white text-sm font-medium rounded hover:bg-green-700 transition">
                See Location
              </a>
              <button @click="deleteCrashEvent(index)"
                class="inline-block mt-2 ml-2 px-3 py-1 bg-red-600 text-white text-sm font-medium rounded hover:bg-red-700 transition">
                Delete
              </button>
              <button v-if="isCrashActive(index)" @click="clearCrashAlert(index)"
                class="inline-block mt-2 ml-2 px-3 py-1 bg-gray-600 text-white text-sm font-medium rounded hover:bg-gray-700 transition">
                Clear Crash
              </button>
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
import { useAuthStore } from '../stores/auth';
import { database } from '../firebase/config';
import { ref as dbRef, set, onValue, remove } from 'firebase/database';

// Components
import TabGroup from '../components/TabGroup.vue';
import DashboardCard from '../components/DashboardCard.vue';
import LocationSection from '../components/LocationSection.vue';
import SpeedDataSection from '../components/SpeedDataSection.vue';
import DiagnosticsSection from '../components/DiagnosticsSection.vue';
import RecentAlerts from '../components/RecentAlerts.vue';

const authStore = useAuthStore();

// States
const riderStatus = ref('Inactive');
const riderSubtitle = ref('Helmet not connected');
const alertnessStatus = ref('Normal');
const alertnessSubtitle = ref('No drowsiness detected');
const alcoholStatus = ref('Safe');
const alcoholSubtitle = ref('No alcohol detected');
const currentSpeed = ref(0);
const speedHistory = ref([]);
const speedLimit = ref(90); // Default value
const diagnostics = ref([]);
const alerts = ref([]);
const activeTab = ref('Speed Data');
const location = ref({ lat: null, lng: null });
const user = ref({ name: 'Loading...' });
const recentTrips = ref([]);
const crashEvents = ref([]);

// Extra state
const isOverSpeed = ref(false);
const showAlerts = ref(true);

// Crash UI
const crashDisplayStatus = ref('Stable'); // Stable | Alerting
const crashDisplayMessage = ref('Vehicle Stable');
let crashInterval = null;
let flashCount = 0;

// Track last known crash timestamp
let lastCrashTimestamp = null;
const userId = 'MnzBjTBslZNijOkq732PE91hHa23'; // Firebase UID

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
    lat !== undefined &&
    lng !== undefined &&
    !isNaN(lat) &&
    !isNaN(lng);
  if (!isValidCoord(startLat, startLng)) return 'https://www.google.com/maps  ';
  const zoomLevel = 14;
  return `https://www.google.com/maps/dir/?api=1&origin= ${startLat},${startLng}&destination=${endLat},${endLng}&zoom=${zoomLevel}`;
};

// Play alert sound
const playSound = () => {
  const audio = new Audio('/sounds/alert.mp3');
  audio.play().catch(err => console.warn("Audio playback failed:", err));
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
      return;
    }
    crashDisplayMessage.value = crashDisplayMessage.value === 'Crash Detected' ? 'Vehicle Stable' : 'Crash Detected';
    flashCount++;
  }, 2000);
};

// Firebase References
const helmetPublicRef = dbRef(database, `helmet_public/${userId}`);
const helmetRef = dbRef(database, `helmet/${userId}`);
const tripsRef = dbRef(database, `helmet_public/${userId}/trips`);
const crashRef = dbRef(database, `helmet_public/${userId}/crashes`);
const alcoholRef = dbRef(database, `helmet_public/${userId}/alcohol`);
const speedLimitRef = dbRef(database, `helmet_public/${userId}/settings/speedLimit`);

onMounted(() => {
  // Load last crash timestamp
  const storedLastCrashTime = localStorage.getItem(`lastCrashTimestamp_${userId}`);
  if (storedLastCrashTime) lastCrashTimestamp = parseInt(storedLastCrashTime);

  // Read initial speed limit
  onValue(speedLimitRef, (snapshot) => {
    const data = snapshot.val();
    if (data !== null) speedLimit.value = data;
  });

  // Crash listener - Only trigger on new crash events
  onValue(crashRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      Object.values(data).forEach(event => {
        const eventTime = event.timestamp;
        const impactInG = event.impactStrength / 9.81; // Convert m/s² → g-force
        const rollTriggered = event.roll < -47 || event.roll > 40;
        if (
          (impactInG >= 1.5 || rollTriggered) &&
          eventTime > (lastCrashTimestamp || 0)
        ) {
          lastCrashTimestamp = eventTime;
          localStorage.setItem(`lastCrashTimestamp_${userId}`, eventTime.toString());
          flashCrashMessage(); // triggers alert card + sound
        }
      });
    }
  });

  // Alcohol listener
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

  // Helmet Public Listener
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
      isOverSpeed.value = currentSpeed.value > speedLimit.value;
    }
  });

  // Helmet-specific data
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

  // Trip data
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

// Delete a trip by ID
const deleteTrip = async (tripId) => {
  const confirmDelete = confirm("Are you sure you want to delete this trip?");
  if (!confirmDelete) return;
  const tripRef = dbRef(database, `helmet_public/${userId}/trips/${tripId}`);
  try {
    await remove(tripRef);
    recentTrips.value = recentTrips.value.filter(trip => trip.id !== tripId);
  } catch (error) {
    console.error("Failed to delete trip:", error);
    alert("Failed to delete trip.");
  }
};

// Determine if crash alert should be shown
const isCrashActive = (index) => {
  const event = crashEvents.value[index];
  return event && event.timestamp > lastCrashTimestamp;
};

// Clear crash alert manually
const clearCrashAlert = (index) => {
  const event = crashEvents.value[index];
  if (event && event.timestamp > lastCrashTimestamp) {
    lastCrashTimestamp = event.timestamp;
    localStorage.setItem(`lastCrashTimestamp_${userId}`, event.timestamp.toString());
    crashDisplayStatus.value = 'Stable';
    crashDisplayMessage.value = 'Vehicle Stable';
  }
};

// Delete Crash Event
const deleteCrashEvent = (index) => {
  crashEvents.value.splice(index, 1);
};
</script>