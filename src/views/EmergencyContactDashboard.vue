<template>
  <div class="min-h-screen flex flex-col bg-[#EDE8F5]">
    <!-- Header -->
    <div class="flex flex-col items-center justify-between py-4">
      <div class="flex items-center gap-2 bg-[#7091E6] px-6 py-2 rounded-xl shadow text-white w-full justify-between">
        <span class="font-semibold text-lg">
          Emergency Contact Dashboard
        </span>
        <!-- Log Out Button -->
        <button
          @click="logout"
          class="bg-red-500 hover:bg-red-600 text-white text-sm font-medium px-4 py-1 rounded transition"
        >
          Log Out
        </button>
      </div>
    </div>

    <!-- Main Dashboard -->
    <main class="flex-1 px-4 md:px-8 py-6">
      <!-- Top Cards -->
      <div class="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-5 gap-6 mb-6">
        <!-- Rider Status -->
        <div class="bg-[#3D52A0] text-white rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl">
          <span class="material-icons text-3xl">Person</span>
          <DashboardCard title="Rider Status" :value="riderStatus" :subtitle="riderSubtitle" icon="status" status="success" />
        </div>
        <!-- Current Speed -->
        <div class="bg-[#7091E6] text-white rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl">
          <span class="material-icons text-3xl">Speed</span>
          <DashboardCard title="Current Speed" :value="currentSpeedText" :subtitle="speedSubtitle" icon="speed" status="info" />
        </div>
        <!-- Alertness -->
        <div :class="[
          'rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl',
          alertnessStatus === 'Normal' ? 'bg-[#8697C4] text-white' : 'bg-yellow-400 text-[#3D52A0]'
        ]">
          <span class="material-icons text-3xl">Warning</span>
          <DashboardCard title="Alertness" :value="alertnessStatus" :subtitle="alertnessSubtitle" icon="alert" :status="alertnessStatus === 'Normal' ? 'success' : 'warning'" />
        </div>
        <!-- Alcohol Detection -->
        <div :class="[
          'rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl',
          alcoholStatus === 'Safe' ? 'bg-[#3D52A0] text-white' : 'bg-red-500 text-white'
        ]">
          <span class="material-icons text-3xl">Liquor</span>
          <DashboardCard title="Alcohol Detection" :value="alcoholStatus" :subtitle="alcoholSubtitle" icon="alcohol" :status="alcoholStatus === 'Safe' ? 'success' : 'danger'" />
        </div>
        <!-- Crash Detection Card -->
        <div :class="[
          'rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl',
          crashDisplayStatus === 'Stable' ? 'bg-green-600 text-white' : 'bg-red-600 text-white'
        ]">
          <span class="material-icons text-3xl">Crash</span>
          <DashboardCard 
            title="Crash Status" 
            :value="crashDisplayMessage" 
            icon="crash" 
            :status="crashDisplayStatus === 'Stable' ? 'success' : 'danger'" 
          />
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

      <!-- Recent Alerts -->
      <h3 class="font-semibold text-lg mb-2 text-[rgb(8,8,8)] flex items-center gap-2">
        <span class="material-icons text-xl">Notifications</span> Recent Alerts
      </h3>
      <div class="bg-[#ffffff] rounded-lg shadow p-4">
        <RecentAlerts 
          :alerts="alerts" 
          :user-id="userId"
          @delete="handleDeleteAlert"
        />
      </div>

      <!-- Recent Trips Preview -->
      <section class="mb-6">
        <h3 class="text-lg font-medium text-gray-800 mb-2">Recent Trips</h3>
        <div v-if="recentTrips.length > 0" class="space-y-4 max-h-64 overflow-y-auto bg-white shadow-sm p-4 rounded-md">
          <div v-for="trip in recentTrips" :key="trip.id" class="py-2 border-b last:border-b-0">
            <p class="text-sm text-gray-500">Routes</p>
            <p><strong>From:</strong> {{ trip.startLocationName || formatLatLng(trip.startLat, trip.startLng) }}</p>
            <p><strong>To:</strong> {{ trip.endLocationName || formatLatLng(trip.endLat, trip.endLng) }}</p>
            <p><strong>Max Speed:</strong> {{ trip.maxSpeed || 'N/A' }} km/h</p>
            <p v-if="trip.distance"><strong>Distance:</strong> {{ trip.distance }}</p>
            <a
              :href="getGoogleMapsLink(trip)"
              target="_blank"
              rel="noopener noreferrer"
              class="mt-2 inline-block px-3 py-1 bg-blue-600 text-white text-sm font-medium rounded hover:bg-blue-700 transition"
            >
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
            <div v-for="(event, index) in crashEvents" :key="index" class="border-b pb-2 last:border-0">
              <p><strong>Impact:</strong> {{ event.impactStrength }} g</p>
              <p><strong>Location:</strong> {{ event.location }}</p>
              <a
                :href="getGoogleMapsLink(event.lat, event.lng)"
                target="_blank"
                rel="noopener noreferrer"
                class="inline-block mt-2 px-3 py-1 bg-green-600 text-white text-sm font-medium rounded hover:bg-green-700 transition"
              >
                See Location
              </a>
              <button
                @click="deleteCrashEvent(index)"
                class="inline-block mt-2 ml-2 px-3 py-1 bg-red-600 text-white text-sm font-medium rounded hover:bg-red-700 transition"
              >
                Delete
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
import { useRouter } from 'vue-router';
import { database } from '../firebase/config';
import { ref as dbRef, onValue, remove } from 'firebase/database';
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
const speedLimit = ref(90);
const diagnostics = ref([]);
const alerts = ref([]);
const activeTab = ref('Speed Data');
const location = ref({ lat: null, lng: null });
const user = ref({ name: 'Loading...' });
const recentTrips = ref([]);
const crashEvents = ref([]);

// Crash Detection States
const crashDisplayStatus = ref('Stable'); // Stable | Alerting
const crashDisplayMessage = ref('Vehicle Stable');
let crashInterval = null;
let flashCount = 0;

// Helpers
const formatLatLng = (lat, lng) => {
  return lat && lng ? `${Number(lat).toFixed(6)}, ${Number(lng).toFixed(6)}` : 'N/A';
};

const currentSpeedText = computed(() => currentSpeed.value.toFixed(2) + ' kph');

const getGoogleMapsLink = (lat, lng) => {
  const coords = `${lat},${lng}`;
  return `https://www.google.com/maps/dir/?api=1&destination= ${encodeURIComponent(coords)}`;
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
  crashInterval = setInterval(() => {
    if (flashCount >= 3) {
      clearInterval(crashInterval);
      crashDisplayStatus.value = 'Stable';
      crashDisplayMessage.value = 'Vehicle Stable';
      return;
    }
    crashDisplayMessage.value = crashDisplayMessage.value === 'Crash Detected' ? 'Vehicle Stable' : 'Crash Detected';
    flashCount++;
  }, 2000); // Toggle every 2 seconds
};

// Firebase References
const userId = 'MnzBjTBslZNijOkq732PE91hHa23'; // Replace with dynamic UID if needed
const helmetPublicRef = dbRef(database, `helmet_public/${userId}`);
const helmetRef = dbRef(database, `helmet/${userId}`);
const tripsRef = dbRef(database, `helmet_public/${userId}/trips`);
const crashRef = dbRef(database, `helmet_public/${userId}/crashes`);
const alcoholRef = dbRef(database, `helmet_public/${userId}/alcohol`);
const alertRef = dbRef(database, `alerts/${userId}`);

// Firebase Listeners
onMounted(() => {
  // Crash listener
  onValue(crashRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      const crashList = Object.values(data);
      if (crashList.some(e => e.impactStrength >= 1.5)) {
        flashCrashMessage(); // Trigger flash alert
      }
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
      if (currentSpeed.value > speedLimit.value) {
        alerts.value.unshift({
          type: 'danger',
          message: 'Speed Limit Exceeded',
          details: `Speed: ${currentSpeed.value} kph | Limit: ${speedLimit.value} kph`,
          time: new Date().toLocaleTimeString()
        });
        playSound();
        if (alerts.value.length > 5) alerts.value.pop();
      }
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
        playSound();
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

  // Alerts Listener
  onValue(alertRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      alerts.value = Object.entries(data).map(([key, value]) => ({
        id: key,
        ...value
      }));
    } else {
      alerts.value = [];
    }
  });
});

// Handle alert deletion
const handleDeleteAlert = (alertId) => {
  const alertPath = `alerts/${userId}/${alertId}`;
  const alertDbRef = dbRef(database, alertPath);
  remove(alertDbRef).catch((err) => {
    console.error("Failed to delete alert", err);
  });
};

// Logout function
const logout = () => {
  router.push({ name: 'EmergencyContactLogin' });
};
</script>