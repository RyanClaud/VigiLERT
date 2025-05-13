<template>
  <div class="min-h-screen flex flex-col bg-[#EDE8F5]">
    <!-- User Greeting/Header -->
    <div class="flex flex-col items-center justify-center py-4">
      <div class="flex items-center gap-2 bg-[#7091E6] px-6 py-2 rounded-xl shadow text-white">
    
        <span class="font-semibold text-lg">
          You are now viewing,
          <span v-if="authStore.user && authStore.user.displayName">{{ authStore.user.displayName }}</span>
          <span v-else-if="authStore.user && authStore.user.email">{{ authStore.user.email }}</span>
          <span v-else>Rider's Dashboard</span>
        </span>
      </div>
    </div>

    <!-- Main Dashboard -->
    <main class="flex-1 px-4 md:px-8 py-6">
      <!-- Top Cards -->
      <div class="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 gap-6 mb-6">
        <div class="bg-[#3D52A0] text-white rounded-xl shadow-lg p-6 flex items-center gap-4">
          <span class="material-icons text-3xl">Rider</span>
          <DashboardCard title="Rider Status" :value="riderStatus" :subtitle="riderSubtitle" icon="status" status="success" />
        </div>
        <div class="bg-[#7091E6] text-white rounded-xl shadow-lg p-6 flex items-center gap-4">
          <span class="material-icons text-3xl">Speed</span>
          <DashboardCard title="Current Speed" :value="currentSpeedText" :subtitle="speedSubtitle" icon="speed" status="info" />
        </div>
        <div :class="['rounded-xl shadow-lg p-6 flex items-center gap-4', alertnessStatus === 'Normal' ? 'bg-[#8697C4] text-white' : 'bg-yellow-400 text-[#3D52A0]' ]">
          <span class="material-icons text-3xl">Warning</span>
          <DashboardCard title="Alertness" :value="alertnessStatus" :subtitle="alertnessSubtitle" icon="alert" :status="alertnessStatus === 'Normal' ? 'success' : 'warning'" />
        </div>
      </div>

      <!-- Tabs -->
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
import { ref, computed, onMounted } from 'vue';
import { useAuthStore } from '../stores/auth';
import { database } from '../firebase/config';
import { ref as dbRef, onValue } from 'firebase/database';
import TabGroup from '../components/TabGroup.vue';
import DashboardCard from '../components/DashboardCard.vue';
import LocationSection from '../components/LocationSection.vue';
import SpeedDataSection from '../components/SpeedDataSection.vue';
import DiagnosticsSection from '../components/DiagnosticsSection.vue';
import RecentAlerts from '../components/RecentAlerts.vue';

const authStore = useAuthStore();
const activeTab = ref('Speed Data');

// Dashboard state
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

onMounted(() => {
  const userId = authStore.user?.uid || 'MnzBjTBslZNijOkq732PE91hHa23';
  const helmetPublicRef = dbRef(database, `helmet_public/${userId}`);

  // Listen to live helmet data
  onValue(helmetPublicRef, (snapshot) => {
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

  // Load recent trips
  const tripsRef = dbRef(database, `helmet_public/${userId}/trips`);
  onValue(tripsRef, async (snapshot) => {
    const data = snapshot.val();
    if (data) {
      const tripList = Object.entries(data).map(([id, trip]) => ({
        id,
        ...trip
      }));
      tripList.sort((a, b) => {
        const aTime = typeof a.timestamp === 'string' ? parseInt(a.timestamp) : a.timestamp;
        const bTime = typeof b.timestamp === 'string' ? parseInt(b.timestamp) : b.timestamp;
        return bTime - aTime;
      });
      recentTrips.value = tripList.slice(0, 5);
      await Promise.all(recentTrips.value.map(loadLocationNames));
    }
  });

  const helmetRef = dbRef(database, `helmet/${userId}`);
  onValue(helmetRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      riderStatus.value = data.helmetConnected ? 'Active' : 'Inactive';
      riderSubtitle.value = data.helmetConnected ? 'Helmet connected' : 'Helmet not connected';
      alertnessStatus.value = data.alertnessStatus || 'Normal';
      alertnessSubtitle.value = alertnessStatus.value === 'Normal' ? 'No drowsiness detected' : 'Drowsiness detected!';
    }
  });
});

async function loadLocationNames(trip) {
  async function reverseGeocode(lat, lng) {
    try {
      const response = await fetch(`https://photon.komoot.io/reverse?lat= ${lat}&lon=${lng}`);
      const data = await response.json();
      return data?.features[0]?.properties?.name || "Unknown";
    } catch (err) {
      return "Unknown";
    }
  }

  if (trip.startLat && trip.startLng && !trip.startLocationName) {
    trip.startLocationName = await reverseGeocode(trip.startLat, trip.startLng);
  }
  if (trip.endLat && trip.endLng && !trip.endLocationName) {
    trip.endLocationName = await reverseGeocode(trip.endLat, trip.endLng);
  }
  recentTrips.value = [...recentTrips.value];
}

async function reverseGeocode(lat, lng) {
  try {
    const response = await fetch(`https://photon.komoot.io/reverse?lat= ${lat}&lon=${lng}`);
    const data = await response.json();
    currentLocationName.value = data?.features[0]?.properties?.name || "Unknown Location";
  } catch (err) {
    currentLocationName.value = "Unknown Location";
  }
}
</script>