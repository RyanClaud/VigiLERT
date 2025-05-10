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
      <div class="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 gap-6 mb-6">
        <div class="bg-[#3D52A0] text-white rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl">
          <span class="material-icons text-3xl">person</span>
          <DashboardCard title="Rider Status" :value="riderStatus" :subtitle="riderSubtitle" icon="status" status="success" />
        </div>
        <div class="bg-[#7091E6] text-white rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl">
          <span class="material-icons text-3xl">speed</span>
          <DashboardCard title="Current Speed" :value="currentSpeed + ' kph'" :subtitle="speedSubtitle" icon="speed" status="info" />
        </div>
        <div :class="['rounded-xl shadow-lg p-6 flex items-center gap-4 transition-transform hover:scale-105 hover:shadow-2xl', alertnessStatus === 'Normal' ? 'bg-[#8697C4] text-white' : 'bg-yellow-400 text-[#3D52A0]']">
          <span class="material-icons text-3xl">warning</span>
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

      <!-- Recent Alerts -->
      <section class="mt-8">
        <h3 class="font-semibold text-lg mb-2 text-[#6e7eb9] flex items-center gap-2">
          <span class="material-icons text-xl">notifications</span>
          Recent Alerts
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
import { ref, onMounted } from 'vue';
import { useAuthStore } from '../stores/auth';
import { database } from '../firebase/config';
import { ref as dbRef, onValue } from 'firebase/database';
import DashboardCard from '../components/DashboardCard.vue';
import TabGroup from '../components/TabGroup.vue';
import LocationSection from '../components/LocationSection.vue';
import SpeedDataSection from '../components/SpeedDataSection.vue';
import DiagnosticsSection from '../components/DiagnosticsSection.vue';
import RecentAlerts from '../components/RecentAlerts.vue';

const authStore = useAuthStore();
const activeTab = ref('My Location');

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
const speedSubtitle = ref('Within speed limit');

function onAvatarError(event) {
  event.target.src = 'https://ui-avatars.com/api/?name=User&background=ADBBD4&color=3D52A0 ';
}

onMounted(() => {
  const helmetPublicRef = dbRef(database, `helmet_public/${authStore.user.uid}`);

  // Listen to public helmet data (sent by ESP32)
  onValue(helmetPublicRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      // Update location
      if (typeof data.locationLat === 'number' && typeof data.locationLng === 'number') {
        location.value = {
          lat: data.locationLat,
          lng: data.locationLng
        };
      }

      // Update speed
      const rawSpeed = parseFloat(data.speed) || 0;
      currentSpeed.value = rawSpeed < 0.1 ? 0.00 : rawSpeed.toFixed(2);
      speedHistory.value.push(currentSpeed.value);
      if (speedHistory.value.length > 10) speedHistory.value.shift();
      speedSubtitle.value = currentSpeed.value > speedLimit.value ? 'Over speed limit!' : 'Within speed limit';

      // Update diagnostics
      diagnostics.value = [
        { label: 'Headlight', value: data.headlight ? 'On' : 'Off' },
        { label: 'Taillight', value: data.taillight ? 'On' : 'Off' },
        { label: 'Left Signal', value: data.leftSignal ? 'Blinking' : 'Off' },
        { label: 'Right Signal', value: data.rightSignal ? 'Blinking' : 'Off' },
        { label: 'Battery Voltage', value: `${parseFloat(data.batteryVoltage || 0).toFixed(2)} V` }
      ];

      // Show alert if over speed limit
      if (currentSpeed.value > speedLimit.value) {
        alerts.value.unshift({
          type: 'danger',
          message: 'Speed Limit Exceeded',
          details: `Speed: ${currentSpeed.value} kph | Limit: ${speedLimit.value} kph`,
          time: new Date().toLocaleTimeString(),
          extra: ''
        });
        if (alerts.value.length > 5) alerts.value.pop();
      }
    }
  });

  // Optional: Also listen to private helmet data if available
  const helmetRef = dbRef(database, `helmet/${authStore.user.uid}`);
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
</script>