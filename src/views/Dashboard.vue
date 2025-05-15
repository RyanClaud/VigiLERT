<template>
  <div class="min-h-screen flex flex-col bg-[#EDE8F5]">
    <!-- User Greeting/Header -->
    <div class="flex flex-col items-center justify-center py-4">
      <div class="flex items-center gap-2 bg-[#7091E6] px-6 py-2 rounded-xl shadow text-white">
        <span>
          <img
            v-if="authStore.user && authStore.user.photoURL"
            :src="authStore.user.photoURL"
            alt="User Avatar"
            class="w-8 h-8 rounded-full border-2 border-white shadow"
            @error="onAvatarError"
          />
          <span v-else class="material-icons text-2xl">account_circle</span>
        </span>
        <span class="font-semibold text-lg">
          Welcome,
          <span v-if="authStore.user && authStore.user.displayName">{{ authStore.user.displayName }}</span>
          <span v-else-if="authStore.user && authStore.user.email">{{ authStore.user.email }}</span>
          <span v-else>User</span>
        </span>
      </div>
    </div>
    <!-- Main Dashboard -->
    <main class="flex-1 px-4 md:px-8 py-6">
      <!-- Top Cards -->
      <div class="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-4 gap-6 mb-6">
        <!-- Existing Cards -->
        <div class="bg-[#3D52A0] text-white rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl">
          <span class="material-icons text-3xl">Rider</span>
          <DashboardCard title="Rider Status" :value="riderStatus" :subtitle="riderSubtitle" icon="status" status="success" />
        </div>
        <div class="bg-[#7091E6] text-white rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl">
          <span class="material-icons text-3xl">Speed</span>
          <DashboardCard title="Current Speed" :value="currentSpeedText" :subtitle="speedSubtitle" icon="speed" status="info" />
        </div>
        <div :class="[
          'rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl',
          alertnessStatus === 'Normal' ? 'bg-[#8697C4] text-white' : 'bg-yellow-400 text-[#3D52A0]'
        ]">
          <span class="material-icons text-3xl">Warning</span>
          <DashboardCard title="Alertness" :value="alertnessStatus" :subtitle="alertnessSubtitle" icon="alert" :status="alertnessStatus === 'Normal' ? 'success' : 'warning'" />
        </div>

        <!-- New Alcohol Detection Card -->
        <div :class="[
          'rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl',
          alcoholStatus === 'Safe' ? 'bg-[#3D52A0] text-white' : 'bg-red-500 text-white'
        ]">
          <span class="material-icons text-3xl">Alcohol</span>
          <DashboardCard title="Alcohol Detection" :value="alcoholStatus" :subtitle="alcoholSubtitle" icon="alcohol" :status="alcoholStatus === 'Safe' ? 'success' : 'danger'" />
        </div>
      </div>

      <!-- Tabs and other sections remain unchanged -->
      <div class="mb-6">
        <TabGroup :tabs="['My Location', 'Speed Data', 'Diagnostics']" v-model="activeTab" class="bg-[#ADBBD4] rounded-lg p-2 flex flex-wrap gap-2" />
      </div>
      <!-- Tab Content -->
      <div v-if="activeTab === 'My Location'" class="bg-white rounded-lg shadow p-4 md:p-6 mb-4">
        <LocationSection :location="location" :user="user" />
      </div>
      <div v-else-if="activeTab === 'Speed Data'" class="bg-white rounded-lg shadow p-4 md:p-6 mb-4">
        <SpeedDataSection :speedData="speedHistory" :speedLimit="speedLimit" />
      </div>
      <div v-else-if="activeTab === 'Diagnostics'" class="bg-white rounded-lg shadow p-4 md:p-6 mb-4">
        <DiagnosticsSection :diagnostics="diagnostics" />
      </div>
      <!-- Recent Trips Preview -->
      <section class="mb-6">
        <h3 class="text-lg font-medium text-gray-800 mb-2">Recent Trips</h3>
        <div v-if="recentTrips.length > 0" class="space-y-4">
          <div v-for="trip in recentTrips" :key="trip.id" class="bg-white shadow-sm p-4 rounded-md">
            <p class="text-sm text-gray-500">Routes</p>
            <p><strong>From:</strong> {{ trip.startLocationName || formatLatLng(trip.startLat, trip.startLng) }}</p>
            <p><strong>To:</strong> {{ trip.endLocationName || formatLatLng(trip.endLat, trip.endLng) }}</p>
            <p><strong>Max Speed:</strong> {{ trip.maxSpeed || 'N/A' }} km/h</p>
            <!-- Navigate Button -->
            <a
              :href="getGoogleMapsLink(trip)"
              target="_blank"
              rel="noopener noreferrer"
              class="mt-2 inline-block px-3 py-1 bg-blue-600 text-white text-sm font-medium rounded hover:bg-blue-700 transition"
            >
              Navigate
            </a>
            <!-- Delete Button -->
            <button
              @click="deleteTrip(trip.id)"
              class="mt-2 ml-2 px-3 py-1 bg-red-600 text-white text-sm font-medium rounded hover:bg-red-700 transition"
            >
              Delete
            </button>
          </div>
        </div>
        <div v-else class="bg-white shadow-sm p-6 rounded-md text-center text-gray-500">
          No trips found yet.
        </div>
      </section>
      <!-- Recent Alerts -->
      <section class="mt-8">
        <h3 class="font-semibold text-lg mb-2 text-[#6e7eb9] flex items-center gap-2">
          <span class="material-icons text-xl">Notifications</span> Recent Alerts
        </h3>
        <div class="bg-[#ffffff] rounded-lg shadow p-4">
          <RecentAlerts :alerts="alerts" />
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
import { ref, computed, onMounted, watch, onBeforeUnmount } from 'vue';
import { useAuthStore } from '../stores/auth';
import { database } from '../firebase/config';
import { ref as dbRef, onValue, remove } from 'firebase/database';
import TabGroup from '../components/TabGroup.vue';
import DashboardCard from '../components/DashboardCard.vue';
import LocationSection from '../components/LocationSection.vue';
import SpeedDataSection from '../components/SpeedDataSection.vue';
import DiagnosticsSection from '../components/DiagnosticsSection.vue';
import RecentAlerts from '../components/RecentAlerts.vue';
import L from 'leaflet';
import { nextTick } from 'vue';

const authStore = useAuthStore();
const activeTab = ref('Speed Data');

// Existing states
const user = ref({ name: 'Loading...' });
const location = ref({ lat: null, lng: null });
const currentSpeed = ref(0);
const speedHistory = ref([]);
const speedLimit = ref(90);
const diagnostics = ref([]);
const alerts = ref([]);
const riderStatus = ref('Inactive');
const riderSubtitle = ref('Helmet not connected');
const alertnessStatus = ref('Normal');
const alertnessSubtitle = ref('No drowsiness detected');
const currentLocationName = ref(null);
const recentTrips = ref([]);
const speedSubtitle = ref('Waiting for data...'); 
const alcoholAlertVisible = ref(false);

// New state for Alcohol Detection
const alcoholStatus = ref('Safe'); // Possible values: 'Safe', 'Caution', 'Danger'
const alcoholSubtitle = ref('No alcohol detected');

// Format lat/lng
const formatLatLng = (lat, lng) => {
  return lat && lng ? `${Number(lat).toFixed(6)}, ${Number(lng).toFixed(6)}` : 'N/A';
};

// Unified Date Formatter
const formatDate = (timestamp) => {
  if (!timestamp) return 'Unknown Date';
  let numTimestamp = typeof timestamp === 'string' ? parseInt(timestamp) : timestamp;
  if (isNaN(numTimestamp)) return 'Invalid Date';
  const date = new Date(numTimestamp);
  if (isNaN(date.getTime())) return 'Invalid Date';
  return date.toLocaleString(undefined, {
    year: 'numeric',
    month: 'short',
    day: 'numeric',
    hour: '2-digit',
    minute: '2-digit'
  });
};

// Computed — formatted speed for display only
const currentSpeedText = computed(() => currentSpeed.value.toFixed(2) + ' kph');

let helmetPublicListener = null;

onMounted(() => {
  const userId = authStore.user?.uid || 'MnzBjTBslZNijOkq732PE91hHa23';
  const helmetPublicRef = dbRef(database, `helmet_public/${userId}`);
  const helmetRef = dbRef(database, `helmet/${userId}`);
  const tripsRef = dbRef(database, `helmet_public/${userId}/trips`);
  const alcoholRef = dbRef(database, `helmet_public/${userId}/alcohol`);

  helmetPublicListener = onValue(helmetPublicRef, (snapshot) => {
    const data = snapshot.val();
    if (data && data.live) {
      const liveData = data.live;
      if (typeof liveData.locationLat === 'number' && typeof liveData.locationLng === 'number') {
        location.value = {
          lat: Number(liveData.locationLat),
          lng: Number(liveData.locationLng)
        };
        reverseGeocode(location.value.lat, location.value.lng);
      }
      const rawSpeed = parseFloat(liveData.speed) || 0;
      currentSpeed.value = rawSpeed < 0.1 ? 0 : rawSpeed;
      speedHistory.value.push(currentSpeed.value);
      if (speedHistory.value.length > 10) speedHistory.value.shift();
      speedSubtitle.value = currentSpeed.value > speedLimit.value ? 'Over speed limit!' : 'Within speed limit';
      diagnostics.value = [
        { label: 'Headlight', value: liveData.headlight ? 'On' : 'Off' },
        { label: 'Taillight', value: liveData.taillight ? 'On' : 'Off' },
        { label: 'Left Signal', value: liveData.leftSignal ? 'Blinking' : 'Off' },
        { label: 'Right Signal', value: liveData.rightSignal ? 'Blinking' : 'Off' },
        { label: 'Battery Voltage', value: `${parseFloat(liveData.batteryVoltage || 0).toFixed(2)} V` }
      ];
      if (currentSpeed.value > speedLimit.value) {
        alerts.value.unshift({
          type: 'danger',
          message: 'Speed Limit Exceeded',
          details: `Speed: ${currentSpeed.value} kph | Limit: ${speedLimit.value} kph`,
          time: new Date().toLocaleTimeString()
        });
        if (alerts.value.length > 5) alerts.value.pop();
      }
    }
  });

  onValue(tripsRef, async (snapshot) => {
    const data = snapshot.val();
    if (data) {
      const tripList = Object.entries(data).map(([id, trip]) => ({
        id,
        ...trip
      }));
      // Sort trips by timestamp in descending order (latest first)
      tripList.sort((a, b) => {
        const aTime = typeof a.timestamp === 'string' ? parseInt(a.timestamp) : a.timestamp;
        const bTime = typeof b.timestamp === 'string' ? parseInt(b.timestamp) : b.timestamp;
        // Handle invalid timestamps gracefully
        if (isNaN(aTime) || isNaN(bTime)) {
          console.warn("Invalid timestamp detected:", a, b);
          return 0; // Keep original order for invalid timestamps
        }
        return bTime - aTime; // Descending order (latest first)
      });
      // Limit to the most recent 5 trips
      recentTrips.value = tripList.slice(0, 5);
      // Load location names for start and end locations
      await Promise.all(recentTrips.value.map(loadLocationNames));
    }
  });

  onValue(helmetRef, (snapshot) => {
  const data = snapshot.val();
  if (data) {
    riderStatus.value = data.helmetConnected ? 'Active' : 'Inactive';
    riderSubtitle.value = data.helmetConnected ? 'Helmet connected' : 'Helmet not connected';
    alertnessStatus.value = data.alertnessStatus || 'Normal';
    alertnessSubtitle.value = alertnessStatus.value === 'Normal' ? 'No drowsiness detected' : 'Drowsiness detected!';

    // Alcohol detection logic
    alcoholStatus.value = data.alcoholLevel > 0.05 ? 'Danger' : 'Safe';
    alcoholSubtitle.value = data.alcoholLevel > 0.05 
      ? `Alcohol detected: ${data.alcoholLevel.toFixed(2)}%` 
      : 'No alcohol detected';

    // Show alert if alcohol is above threshold
    alcoholAlertVisible.value = data.alcoholLevel > 0.05;
  }
});

  // Listen for alcohol data
  onValue(alcoholRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      alcoholStatus.value = data.status || 'Safe';
      alcoholSubtitle.value = data.sensorValue > 1500 ? `Alcohol detected: ${data.sensorValue}` : 'No alcohol detected';
    }
  });
});

function initLiveMap(lat, lng) {
  const mapElement = document.getElementById('location-section-map');
  if (!mapElement) {
    console.warn("Map container not found");
    return;
  }
  if (window.map) {
    window.map.setView([lat, lng], 13);
    window.map.invalidateSize();
    return;
  }
  window.map = L.map(mapElement).setView([lat, lng], 13);
  L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; OpenStreetMap contributors'
  }).addTo(window.map);
  L.marker([lat, lng]).addTo(window.map).bindPopup("Current Location").openPopup();
}

watch(
  () => location.value,
  async (newLoc) => {
    if (newLoc.lat && newLoc.lng) {
      await nextTick(); // Ensure DOM is updated
      initLiveMap(newLoc.lat, newLoc.lng);
    }
  },
  { deep: true }
);

onBeforeUnmount(() => {
  if (helmetPublicListener) helmetPublicListener();
  if (window.map) {
    window.map.remove();
    window.map = null;
  }
});

async function loadLocationNames(trip) {
  if (trip.startLat && trip.startLng && !trip.startLocationName) {
    trip.startLocationName = await reverseGeocode(trip.startLat, trip.startLng);
  }
  if (trip.endLat && trip.endLng && !trip.endLocationName) {
    trip.endLocationName = await reverseGeocode(trip.endLat, trip.endLng);
  }
  recentTrips.value = [...recentTrips.value];
}

async function reverseGeocode(lat, lng) {
  const controller = new AbortController();
  const timeoutId = setTimeout(() => controller.abort(), 5000); // 5 sec timeout
  try {
    const response = await fetch(
      `https://nominatim.openstreetmap.org/reverse?format=json&lat= ${lat}&lon=${lng}`,
      {
        signal: controller.signal,
        headers: {
          'User-Agent': 'VigilERTApp/1.0'
        }
      }
    );
    clearTimeout(timeoutId);
    if (!response.ok) throw new Error(`HTTP error! Status: ${response.status}`);
    const data = await response.json();
    return data.display_name || "Unknown Location";
  } catch (err) {
    console.error("Reverse geocode error:", err.message);
    return "Unknown Location";
  }
}

// Helper: Generate Google Maps Link
const getGoogleMapsLink = (trip) => {
  const start = `${trip.startLat},${trip.startLng}`;
  const end = `${trip.endLat},${trip.endLng}`;
  return `https://www.google.com/maps/dir/?api=1&origin= ${start}&destination=${end}`;
};

// Helper: Delete Trip
const deleteTrip = async (tripId) => {
  const userId = authStore.user?.uid || 'MnzBjTBslZNijOkq732PE91hHa23';
  const tripRef = dbRef(database, `helmet_public/${userId}/trips/${tripId}`);
  await remove(tripRef);
};
</script>