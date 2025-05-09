<template>
  <div class="min-h-screen flex flex-col bg-[#EDE8F5]">
    <div class="flex flex-col md:flex-row items-center justify-between py-4 px-4">
      <div class="flex items-center gap-2 bg-[#7091E6] px-6 py-2 rounded-xl shadow text-white">
        <span class="font-semibold text-lg">Emergency Contact View (For Viewing-Access Only)</span>
      </div>
      <button
        @click="logout"
        class="mt-2 md:mt-0 bg-red-500 hover:bg-red-600 text-white px-4 py-2 rounded-lg font-semibold transition"
      >
        Logout
      </button>
    </div>
    <main class="flex-1 px-4 md:px-8 py-6">
      <div v-if="loading" class="flex justify-center items-center h-64">
        <span class="text-[#7091E6] text-lg font-semibold">Loading dashboard...</span>
      </div>
      <div v-else>
        <div class="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 gap-6 mb-6">
          <div class="bg-[#3D52A0] text-white rounded-xl shadow-lg p-6 flex items-center gap-4">
            <span class="material-icons text-3xl">RIDER</span>
            <div>
              <div class="text-sm font-medium">Status</div>
              <div class="text-2xl font-bold">{{ helmetData.helmetConnected ? 'Active' : 'Inactive' }}</div>
              <div class="text-xs">{{ helmetData.helmetConnected ? 'Helmet connected' : 'Helmet not connected' }}</div>
            </div>
          </div>
          <div class="bg-[#7091E6] text-white rounded-xl shadow-lg p-6 flex items-center gap-4">
            <span class="material-icons text-3xl">speed</span>
            <div>
              <div class="text-sm font-medium">Current Speed</div>
              <div class="text-2xl font-bold">{{ helmetData.speed || 0 }} kph</div>
              <div class="text-xs">{{ helmetData.speed > (helmetData.speedLimit || 90) ? 'Over speed limit!' : 'Within speed limit' }}</div>
            </div>
          </div>
          <div :class="['rounded-xl shadow-lg p-6 flex items-center gap-4', helmetData.alertnessStatus === 'Normal' ? 'bg-[#8697C4] text-white' : 'bg-yellow-400 text-[#3D52A0]']">
            <span class="material-icons text-3xl">AWAKE</span>
            <div>
              <div class="text-sm font-medium">Alertness</div>
              <div class="text-2xl font-bold">{{ helmetData.alertnessStatus || 'Normal' }}</div>
              <div class="text-xs">{{ helmetData.alertnessStatus === 'Normal' ? 'No drowsiness detected' : 'Drowsiness detected!' }}</div>
            </div>
          </div>
        </div>
        <div class="mb-6">
          <div class="bg-[#ADBBD4] rounded-lg p-2 flex flex-wrap gap-2 text-[#3D52A0] font-semibold">My Location, Speed Data, Diagnostics (View Only)</div>
        </div>
        <div class="bg-white rounded-lg shadow p-4 md:p-6 mb-4">
          <h4 class="font-semibold text-lg mb-2 text-[#3D52A0]">Current Location</h4>
          <div class="text-sm text-[#7091E6] mb-2">Lat: {{ helmetData.locationLat }}, Lng: {{ helmetData.locationLng }}</div>
          <div class="text-xs text-[#8697C4] mb-2">Address: {{ helmetData.locationAddress }}</div>
        </div>
        <div class="bg-white rounded-lg shadow p-4 md:p-6 mb-4">
          <h4 class="font-semibold text-lg mb-2 text-[#3D52A0]">Speed Data</h4>
          <div class="text-sm text-[#7091E6] mb-2">Current Speed: {{ helmetData.speed || 0 }} kph</div>
          <div class="text-xs text-[#8697C4] mb-2">Speed Limit: {{ helmetData.speedLimit || 90 }} kph</div>
        </div>
        <div class="bg-white rounded-lg shadow p-4 md:p-6 mb-4">
          <DiagnosticsSection :diagnostics="helmetData.diagnostics || []" />
        </div>
        <section class="mt-8">
          <h3 class="font-semibold text-lg mb-2 text-[#3D52A0] flex items-center gap-2">
            <span class="material-icons text-xl">Notifications</span>
            Recent Alerts
          </h3>
          <div class="bg-[rgb(251,251,252)] rounded-lg shadow p-4">
            <RecentAlerts :alerts="helmetData.alerts || []" />
          </div>
        </section>
        <section class="mt-8">
          <h3 class="font-semibold text-lg mb-2 text-[#3D52A0] flex items-center gap-2">
            <span class="material-icons text-xl">Emergency</span>
            Contacts Information
          </h3>
          <div class="bg-[#EDE8F5] rounded-lg shadow p-4">
            <p><strong>Name:</strong> {{ emergencyContact.name || 'N/A' }}</p>
            <p><strong>Phone:</strong> {{ emergencyContact.phone || 'N/A' }}</p>
            <p><strong>Relationship:</strong> {{ emergencyContact.relationship || 'N/A' }}</p>
          </div>
        </section>
      </div>
    </main>
    <footer class="bg-[#ADBBD4] text-center py-4 text-[#3D52A0] text-sm border-t mt-auto">
      © 2025 VIGILERT. All rights reserved.
    </footer>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue';
import { useRoute, useRouter } from 'vue-router';
import { getDatabase, ref as dbRef, onValue } from 'firebase/database';
import DiagnosticsSection from '../components/DiagnosticsSection.vue';
import RecentAlerts from '../components/RecentAlerts.vue';

const route = useRoute();
const router = useRouter();
const userId = route.params.userId;
const helmetData = ref({});
const emergencyContact = ref({});
const loading = ref(true);

function logout() {
  router.push('/emergency-login');
}

onMounted(() => {
  const db = getDatabase();

  // Fetch helmet data
  const helmetRef = dbRef(db, `helmet/${userId}`);
  onValue(helmetRef, (snapshot) => {
    helmetData.value = snapshot.val() || {};
  });

  // Fetch emergency contact data
  const emergencyContactRef = dbRef(db, `users/${userId}/emergencyContacts`);
  onValue(emergencyContactRef, (snapshot) => {
    const contacts = snapshot.val();
    if (contacts) {
      // Assuming the first contact is the one to display
      emergencyContact.value = Object.values(contacts)[0] || {};
    }
    loading.value = false;
  });
});
</script>