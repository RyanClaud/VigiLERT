<template>
  <div class="min-h-screen flex flex-col bg-gradient-to-br from-[#EDE8F5] via-[#f5f3f7] to-[#e8e4f0] pb-20">
    <!-- Enhanced Emergency Contact Header -->
    <div class="relative overflow-hidden bg-gradient-to-r from-[#3D52A0] via-[#5571c6] to-[#7091E6] py-8 px-4 md:px-8 shadow-2xl">
      <!-- Animated Background Elements -->
      <div class="absolute inset-0 overflow-hidden">
        <div class="absolute -top-24 -left-24 w-96 h-96 bg-white/10 rounded-full blur-3xl animate-pulse"></div>
        <div class="absolute -bottom-24 -right-24 w-96 h-96 bg-white/10 rounded-full blur-3xl animate-pulse" style="animation-delay: 1s;"></div>
      </div>
      
      <div class="relative max-w-7xl mx-auto">
        <div class="flex flex-col md:flex-row items-center justify-between gap-6">
          <!-- Emergency Contact Info Section -->
          <div class="flex items-center gap-6">
            <!-- Emergency Contact Avatar -->
            <div class="relative group">
              <div class="absolute inset-0 bg-gradient-to-r from-red-400 to-orange-400 rounded-full blur-lg opacity-75 group-hover:opacity-100 transition-opacity duration-300"></div>
              <div class="relative">
                <div class="relative bg-gradient-to-br from-white/20 to-white/10 backdrop-blur-sm p-4 rounded-full border-4 border-white shadow-2xl">
                  <span class="material-icons text-5xl md:text-6xl text-white">shield</span>
                  <div class="absolute -bottom-1 -right-1 flex items-center gap-1 bg-orange-500 px-2 py-1 rounded-full border-2 border-white shadow-lg">
                    <div class="w-2 h-2 bg-white rounded-full animate-pulse"></div>
                    <span class="text-xs font-bold text-white">Monitoring</span>
                  </div>
                </div>
              </div>
            </div>
            
            <!-- Emergency Contact Details -->
            <div class="text-center md:text-left">
              <div class="flex items-center gap-2 mb-2">
                <span class="material-icons text-red-300 text-sm animate-pulse">emergency</span>
                <p class="text-xs md:text-sm text-white/90 font-semibold uppercase tracking-widest">
                  Emergency Contact
                </p>
              </div>
              <h1 class="text-2xl md:text-4xl font-bold text-white mb-2 drop-shadow-lg">
                Safety Monitor,
                <span class="fcbg-gradient-to-r from-red-300 to-orange-300 bg-clip-text text-transparent">
                  Guardian
                </span>
              </h1>
              <div class="flex flex-wrap items-center gap-3 text-white/80 text-sm">
                <div class="flex items-center gap-1">
                  <span class="material-icons text-sm">visibility</span>
                  <span>Monitoring Rider Safety</span>
                </div>
                <div class="hidden md:flex items-center gap-1">
                  <span class="material-icons text-sm">schedule</span>
                  <span>{{ getCurrentTime() }}</span>
                </div>
              </div>
            </div>
          </div>
          
          <!-- Quick Actions & Stats -->
          <div class="flex flex-col gap-4">
            <!-- Emergency SOS Button -->
            <div class="bg-gradient-to-r from-red-500 to-red-600 rounded-2xl px-6 py-4 hover:from-red-600 hover:to-red-700 transition-all duration-300 cursor-pointer" @click="triggerSOS">
              <div class="flex items-center gap-4">
                <div class="text-5xl animate-pulse">🚨</div>
                <div>
                  <p class="text-2xl font-bold text-white">Emergency SOS</p>
                  <p class="text-sm text-white/80">Tap to alert rider</p>
                </div>
              </div>
            </div>
            
            <!-- Quick Stats -->
            <div class="flex gap-3">
              <div class="bg-white/10 backdrop-blur-md border border-white/20 rounded-2xl px-4 py-3 text-center hover:bg-white/20 transition-all duration-300 cursor-pointer group flex-1">
                <div class="flex items-center justify-center mb-1">
                  <span class="material-icons text-green-300 text-2xl group-hover:scale-110 transition-transform">verified_user</span>
                </div>
                <p class="text-xl font-bold text-white">{{ helmetPaired && motorcyclePaired ? 'Safe' : 'Alert' }}</p>
                <p class="text-xs text-white/70 font-medium">Status</p>
              </div>
              
              <div class="bg-white/10 backdrop-blur-md border border-white/20 rounded-2xl px-4 py-3 text-center hover:bg-white/20 transition-all duration-300 cursor-pointer group flex-1">
                <div class="flex items-center justify-center mb-1">
                  <span class="material-icons text-blue-300 text-2xl group-hover:scale-110 transition-transform">speed</span>
                </div>
                <p class="text-xl font-bold text-white">{{ currentSpeed.toFixed(0) }}</p>
                <p class="text-xs text-white/70 font-medium">km/h</p>
              </div>
            </div>
          </div>
        </div>
        
        <!-- Logout Button -->
        <div class="flex justify-end mt-6">
          <button @click="logout" class="bg-white/10 hover:bg-white/20 backdrop-blur-md border border-white/30 text-white px-6 py-3 rounded-xl font-semibold transition-all duration-300 flex items-center gap-2">
            <span class="material-icons">logout</span>
            Logout
          </button>
        </div>
      </div>
    </div>

    <!-- Main Dashboard -->
    <main class="flex-1 px-4 md:px-8 py-6">
      <!-- System Status Bar -->
      <div id="status" class="grid grid-cols-1 md:grid-cols-2 gap-4 mb-6">
        <!-- Helmet-Motorcycle Pairing Status -->
        <div class="relative overflow-hidden bg-white/80 backdrop-blur-lg rounded-2xl shadow-lg p-5 border border-white/50">
          <div class="flex items-center justify-between">
            <div class="flex items-center gap-3">
              <div :class="['p-3 rounded-xl', helmetPaired && motorcyclePaired ? 'bg-green-500' : 'bg-red-500']">
                <span class="material-icons text-white text-2xl">link</span>
              </div>
              <div>
                <p class="text-xs text-gray-500 font-medium uppercase">Device Pairing</p>
                <p :class="['text-lg font-bold', helmetPaired && motorcyclePaired ? 'text-green-600' : 'text-red-600']">
                  {{ helmetPaired && motorcyclePaired ? 'Connected' : 'Disconnected' }}
                </p>
              </div>
            </div>
            <div class="flex flex-col gap-1">
              <div class="flex items-center gap-2">
                <span class="material-icons text-sm" :class="helmetPaired ? 'text-green-500' : 'text-gray-400'">sports_motorsports</span>
                <span class="text-xs font-medium" :class="helmetPaired ? 'text-green-600' : 'text-gray-500'">Helmet</span>
              </div>
              <div class="flex items-center gap-2">
                <span class="material-icons text-sm" :class="motorcyclePaired ? 'text-green-500' : 'text-gray-400'">two_wheeler</span>
                <span class="text-xs font-medium" :class="motorcyclePaired ? 'text-green-600' : 'text-gray-500'">Motorcycle</span>
              </div>
            </div>
          </div>
        </div>

        <!-- Rider Tracking Control -->
        <div class="relative overflow-hidden bg-white/80 backdrop-blur-lg rounded-2xl shadow-lg p-5 border border-white/50">
          <div class="flex items-center justify-between">
            <div class="flex items-center gap-3">
              <div :class="['p-3 rounded-xl', isTrackingRider ? 'bg-blue-500' : 'bg-gray-500']">
                <span class="material-icons text-white text-2xl">my_location</span>
              </div>
              <div>
                <p class="text-xs text-gray-500 font-medium uppercase">Location Tracking</p>
                <p :class="['text-lg font-bold', isTrackingRider ? 'text-blue-600' : 'text-gray-600']">
                  {{ isTrackingRider ? 'Active' : 'Inactive' }}
                </p>
              </div>
            </div>
            <button @click="toggleRiderTracking" :class="['px-4 py-2 rounded-lg font-semibold text-sm transition-all duration-300', isTrackingRider ? 'bg-red-500 hover:bg-red-600 text-white' : 'bg-blue-500 hover:bg-blue-600 text-white']">
              {{ isTrackingRider ? 'Stop' : 'Start' }}
            </button>
          </div>
        </div>
      </div>

      <!-- Rider Status Cards -->
      <div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-4 gap-4 mb-6">
        <!-- Rider Status -->
        <div class="bg-white/80 backdrop-blur-lg rounded-2xl shadow-lg p-5 border border-white/50">
          <div class="flex items-center gap-3 mb-3">
            <div :class="['p-2 rounded-lg', riderStatus === 'Active' ? 'bg-green-500' : 'bg-red-500']">
              <span class="material-icons text-white">person</span>
            </div>
            <div>
              <p class="text-xs text-gray-500 font-medium uppercase">Rider Status</p>
              <p :class="['text-lg font-bold', riderStatus === 'Active' ? 'text-green-600' : 'text-red-600']">{{ riderStatus }}</p>
            </div>
          </div>
          <p class="text-sm text-gray-600">{{ riderSubtitle }}</p>
        </div>

        <!-- Speed Status -->
        <div class="bg-white/80 backdrop-blur-lg rounded-2xl shadow-lg p-5 border border-white/50">
          <div class="flex items-center gap-3 mb-3">
            <div :class="['p-2 rounded-lg', isOverSpeed ? 'bg-red-500' : 'bg-blue-500']">
              <span class="material-icons text-white">speed</span>
            </div>
            <div>
              <p class="text-xs text-gray-500 font-medium uppercase">Current Speed</p>
              <p :class="['text-lg font-bold', isOverSpeed ? 'text-red-600' : 'text-blue-600']">{{ currentSpeedText }}</p>
            </div>
          </div>
          <p class="text-sm text-gray-600">Limit: {{ speedLimit }} km/h</p>
        </div>

        <!-- Alcohol Status -->
        <div class="bg-white/80 backdrop-blur-lg rounded-2xl shadow-lg p-5 border border-white/50">
          <div class="flex items-center gap-3 mb-3">
            <div :class="['p-2 rounded-lg', alcoholStatus === 'Safe' ? 'bg-green-500' : 'bg-red-500']">
              <span class="material-icons text-white">local_drink</span>
            </div>
            <div>
              <p class="text-xs text-gray-500 font-medium uppercase">Alcohol Status</p>
              <p :class="['text-lg font-bold', alcoholStatus === 'Safe' ? 'text-green-600' : 'text-red-600']">{{ alcoholStatus }}</p>
            </div>
          </div>
          <p class="text-sm text-gray-600">{{ alcoholSubtitle }}</p>
        </div>

        <!-- Crash Status -->
        <div class="bg-white/80 backdrop-blur-lg rounded-2xl shadow-lg p-5 border border-white/50">
          <div class="flex items-center gap-3 mb-3">
            <div :class="['p-2 rounded-lg', crashDisplayStatus === 'Stable' ? 'bg-green-500' : 'bg-red-500']">
              <span class="material-icons text-white">warning</span>
            </div>
            <div>
              <p class="text-xs text-gray-500 font-medium uppercase">Crash Status</p>
              <p :class="['text-lg font-bold', crashDisplayStatus === 'Stable' ? 'text-green-600' : 'text-red-600']">{{ crashDisplayStatus }}</p>
            </div>
          </div>
          <p class="text-sm text-gray-600">{{ crashDisplayMessage }}</p>
        </div>
      </div>

      <!-- Location & Speed Tabs -->
      <div class="mb-6 relative z-10">
        <div class="bg-white/80 backdrop-blur-md rounded-2xl shadow-xl p-4 border border-white/50">
          <div class="flex gap-4 mb-4">
            <button @click="activeTab = 'My Location'" :class="['px-4 py-2 rounded-lg font-semibold transition-all duration-300', activeTab === 'My Location' ? 'bg-blue-500 text-white' : 'bg-gray-200 text-gray-700 hover:bg-gray-300']">
              My Location
            </button>
            <button @click="activeTab = 'Speed Data'" :class="['px-4 py-2 rounded-lg font-semibold transition-all duration-300', activeTab === 'Speed Data' ? 'bg-blue-500 text-white' : 'bg-gray-200 text-gray-700 hover:bg-gray-300']">
              Speed Data
            </button>
          </div>
        </div>
      </div>

      <!-- Tab Content -->
      <div id="map" class="relative z-0">
        <div v-show="activeTab === 'My Location'" class="bg-white/90 backdrop-blur-lg rounded-3xl shadow-2xl p-6 md:p-8 mb-6 border-2 border-white/50 transition-all duration-300">
          <div class="text-center mb-6">
            <h3 class="text-2xl font-bold text-gray-800 mb-2">📍 Rider Location Monitoring</h3>
            <p class="text-gray-600">Real-time location tracking for emergency response</p>
          </div>
          
          <div class="grid grid-cols-1 md:grid-cols-2 gap-6">
            <!-- Current Location -->
            <div class="bg-gradient-to-br from-blue-50 to-blue-100 rounded-2xl p-6 border-2 border-blue-200">
              <h4 class="font-bold text-blue-800 mb-4 flex items-center gap-2">
                <span class="material-icons">location_on</span>
                Current Location
              </h4>
              <div class="space-y-3">
                <div class="flex justify-between">
                  <span class="text-sm text-blue-700">Latitude:</span>
                  <span class="font-bold text-blue-900 text-xs">{{ location.lat?.toFixed(6) || 'N/A' }}</span>
                </div>
                <div class="flex justify-between">
                  <span class="text-sm text-blue-700">Longitude:</span>
                  <span class="font-bold text-blue-900 text-xs">{{ location.lng?.toFixed(6) || 'N/A' }}</span>
                </div>
                <div class="flex justify-between">
                  <span class="text-sm text-blue-700">Speed:</span>
                  <span class="font-bold text-blue-900">{{ currentSpeed.toFixed(1) }} km/h</span>
                </div>
              </div>
              <div class="mt-4">
                <a v-if="location.lat && location.lng" :href="`https://www.google.com/maps?q=${location.lat},${location.lng}`" target="_blank" class="inline-flex items-center gap-2 bg-blue-500 hover:bg-blue-600 text-white px-4 py-2 rounded-lg font-semibold text-sm transition-all duration-300">
                  <span class="material-icons text-sm">map</span>
                  View on Maps
                </a>
              </div>
            </div>

            <!-- Emergency Contact Location -->
            <div class="bg-gradient-to-br from-orange-50 to-orange-100 rounded-2xl p-6 border-2 border-orange-200">
              <h4 class="font-bold text-orange-800 mb-4 flex items-center gap-2">
                <span class="material-icons">shield</span>
                Your Location
              </h4>
              <div class="space-y-3">
                <div class="flex justify-between">
                  <span class="text-sm text-orange-700">Latitude:</span>
                  <span class="font-bold text-orange-900 text-xs">{{ emergencyContactLocation.lat?.toFixed(6) || 'N/A' }}</span>
                </div>
                <div class="flex justify-between">
                  <span class="text-sm text-orange-700">Longitude:</span>
                  <span class="font-bold text-orange-900 text-xs">{{ emergencyContactLocation.lng?.toFixed(6) || 'N/A' }}</span>
                </div>
                <div class="flex justify-between">
                  <span class="text-sm text-orange-700">Tracking:</span>
                  <span :class="['font-bold', isTrackingRider ? 'text-green-600' : 'text-gray-600']">{{ isTrackingRider ? 'Active' : 'Inactive' }}</span>
                </div>
              </div>
              <div class="mt-4">
                <a v-if="emergencyContactLocation.lat && emergencyContactLocation.lng" :href="`https://www.google.com/maps?q=${emergencyContactLocation.lat},${emergencyContactLocation.lng}`" target="_blank" class="inline-flex items-center gap-2 bg-orange-500 hover:bg-orange-600 text-white px-4 py-2 rounded-lg font-semibold text-sm transition-all duration-300">
                  <span class="material-icons text-sm">map</span>
                  View on Maps
                </a>
              </div>
            </div>
          </div>
        </div>

        <div v-show="activeTab === 'Speed Data'" class="bg-white/90 backdrop-blur-lg rounded-3xl shadow-2xl p-6 md:p-8 mb-6 border-2 border-white/50 transition-all duration-300">
          <div class="text-center mb-6">
            <h3 class="text-2xl font-bold text-gray-800 mb-2">📊 Speed Monitoring</h3>
            <p class="text-gray-600">Track rider speed and set safety limits</p>
          </div>
          
          <!-- Speed Limit Control -->
          <div class="bg-gradient-to-br from-purple-50 to-purple-100 rounded-2xl p-6 border-2 border-purple-200 mb-6">
            <h4 class="font-bold text-purple-800 mb-4 flex items-center gap-2">
              <span class="material-icons">tune</span>
              Speed Limit Control
            </h4>
            <div class="flex items-center gap-4">
              <label class="text-sm font-medium text-purple-700">Set Speed Limit:</label>
              <input v-model.number="speedLimit" @change="updateSpeedLimitInFirebase" type="number" min="20" max="120" class="px-3 py-2 border border-purple-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-purple-500 focus:border-transparent">
              <span class="text-sm text-purple-600 font-medium">km/h</span>
            </div>
          </div>

          <!-- Recent Speed History -->
          <div class="bg-gradient-to-br from-green-50 to-green-100 rounded-2xl p-6 border-2 border-green-200">
            <h4 class="font-bold text-green-800 mb-4 flex items-center gap-2">
              <span class="material-icons">timeline</span>
              Recent Speed History
            </h4>
            <div class="grid grid-cols-5 gap-2">
              <div v-for="(speed, index) in speedHistory.slice(-10)" :key="index" class="text-center">
                <div :class="['px-2 py-1 rounded text-xs font-bold', speed > speedLimit ? 'bg-red-500 text-white' : 'bg-green-500 text-white']">
                  {{ speed.toFixed(0) }}
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- Recent Alerts -->
      <div v-if="showAlerts && alerts.length > 0" class="bg-white/90 backdrop-blur-lg rounded-3xl shadow-2xl p-6 md:p-8 mb-6 border-2 border-white/50">
        <div class="flex items-center justify-between mb-6">
          <h3 class="text-2xl font-bold text-gray-800 flex items-center gap-3">
            <div class="bg-red-500 p-2 rounded-xl">
              <span class="material-icons text-white text-2xl">warning</span>
            </div>
            Recent Alerts
          </h3>
          <button @click="toggleAlerts" class="text-gray-500 hover:text-gray-700 transition-colors">
            <span class="material-icons">close</span>
          </button>
        </div>
        <div class="space-y-3">
          <div v-for="(alert, index) in alerts.slice(0, 5)" :key="index" :class="['p-4 rounded-xl border-l-4', alert.type === 'danger' ? 'bg-red-50 border-red-500' : alert.type === 'success' ? 'bg-green-50 border-green-500' : 'bg-blue-50 border-blue-500']">
            <div class="flex items-start justify-between">
              <div>
                <p :class="['font-bold', alert.type === 'danger' ? 'text-red-800' : alert.type === 'success' ? 'text-green-800' : 'text-blue-800']">{{ alert.message }}</p>
                <p :class="['text-sm', alert.type === 'danger' ? 'text-red-600' : alert.type === 'success' ? 'text-green-600' : 'text-blue-600']">{{ alert.details }}</p>
              </div>
              <span :class="['text-xs font-medium', alert.type === 'danger' ? 'text-red-500' : alert.type === 'success' ? 'text-green-500' : 'text-blue-500']">{{ alert.time }}</span>
            </div>
          </div>
        </div>
      </div>
    </main>

    <!-- Footer -->
    <footer class="relative overflow-hidden bg-gradient-to-r from-[#3D52A0] via-[#4a5fb8] to-[#3D52A0] text-center py-6 text-white border-t-4 border-white/20 mt-auto backdrop-blur-sm">
      <div class="absolute inset-0 bg-gradient-to-t from-black/10 to-transparent"></div>
      <div class="relative">
        <p class="font-semibold text-sm tracking-wide">© 2025 VIGILERT. All rights reserved.</p>
        <p class="text-xs text-white/70 mt-1">Emergency Contact Monitoring System</p>
      </div>
    </footer>
  </div>
</template>

<script setup>
import { ref, computed, onMounted, onUnmounted } from 'vue';
import { useRouter } from 'vue-router';
import { database } from '../firebase/config';
import { ref as dbRef, set, onValue, onChildAdded } from 'firebase/database';

const router = useRouter();

// Reactive data
const showRiderCards = ref(true);
const isMonitoring = ref(true);
const helmetPaired = ref(false);
const motorcyclePaired = ref(false);
const currentSpeed = ref(0);
const speedLimit = ref(60);
const isOverSpeed = ref(false);
const riderStatus = ref('Active');
const alertnessStatus = ref('Normal');
const alcoholStatus = ref('Safe');
const crashDisplayStatus = ref('Stable');
const crashDisplayMessage = ref('Normal');
const deviceBattery = ref(85);
const gpsConnected = ref(true);
const gsmConnected = ref(true);
const isTrackingRider = ref(false);
const emergencyContactLocation = ref({ lat: null, lng: null });
const activeTab = ref('My Location');
const showAlerts = ref(false);
const alerts = ref([]);
const crashEvents = ref([]);
const recentTrips = ref([]);
const speedHistory = ref([]);
const diagnostics = ref({});
const location = ref({ lat: null, lng: null });
const user = ref(null);

// Computed properties
const currentSpeedText = computed(() => {
  return currentSpeed.value ? `${currentSpeed.value.toFixed(1)} km/h` : '0 km/h';
});

const riderSubtitle = computed(() => {
  return helmetPaired.value && motorcyclePaired.value ? 'All systems connected' : 'Check connections';
});

const alcoholSubtitle = computed(() => {
  return alcoholStatus.value === 'Safe' ? 'No alcohol detected' : 'Alcohol detected!';
});

// Methods
const getCurrentTime = () => {
  return new Date().toLocaleTimeString();
};

const logout = () => {
  router.push('/emergency-login');
};

const triggerSOS = () => {
  console.log('Emergency SOS triggered');
  alert('Emergency SOS Alert sent to rider!');
};

const toggleRiderTracking = () => {
  isTrackingRider.value = !isTrackingRider.value;
  if (isTrackingRider.value) {
    console.log('Started tracking rider');
  } else {
    console.log('Stopped tracking rider');
  }
};

const toggleAlerts = () => {
  showAlerts.value = !showAlerts.value;
};

const updateSpeedLimitInFirebase = () => {
  const speedLimitRef = dbRef(database, 'MnzBjTBslZNijOkq732PE91hHa23/speedLimit');
  set(speedLimitRef, speedLimit.value)
    .then(() => {
      console.log('Speed limit updated:', speedLimit.value);
    })
    .catch((error) => {
      console.error('Error updating speed limit:', error);
    });
};

// Firebase listeners
const setupFirebaseListeners = () => {
  const userUID = 'MnzBjTBslZNijOkq732PE91hHa23';
  
  // Listen to helmet pairing status
  const helmetRef = dbRef(database, `helmet_public/${userUID}/devices/helmet`);
  onValue(helmetRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      helmetPaired.value = data.status === 'On';
    }
  });

  // Listen to motorcycle pairing status
  const motorcycleRef = dbRef(database, `helmet_public/${userUID}/devices/motorcycle`);
  onValue(motorcycleRef, (snapshot) => {
    const data = snapshot.val();
    motorcyclePaired.value = data && data.status === 'On';
  });

  // Listen to live data
  const liveRef = dbRef(database, `helmet_public/${userUID}/live`);
  onValue(liveRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      currentSpeed.value = data.speed || 0;
      isOverSpeed.value = currentSpeed.value > speedLimit.value;
      
      // Update location
      location.value = {
        lat: data.locationLat || 0,
        lng: data.locationLng || 0
      };
      
      // Update rider status based on engine running
      riderStatus.value = data.engineRunning ? 'Active' : 'Inactive';
      
      // Update alcohol status
      alcoholStatus.value = data.alcoholDetected ? 'Danger' : 'Safe';
      
      // Update crash status
      crashDisplayStatus.value = data.crashDetected ? 'Alert' : 'Stable';
      crashDisplayMessage.value = data.crashDetected ? 'Crash Detected' : 'Normal';
    }
  });

  // Listen to speed limit changes
  const speedLimitRef = dbRef(database, `${userUID}/speedLimit`);
  onValue(speedLimitRef, (snapshot) => {
    const data = snapshot.val();
    if (data !== null) {
      speedLimit.value = data;
    }
  });

  // Listen to alerts
  const alertsRef = dbRef(database, `helmet_public/${userUID}/alerts`);
  onValue(alertsRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      alerts.value = Object.values(data).reverse().slice(0, 10);
      showAlerts.value = alerts.value.length > 0;
    }
  });

  // Listen to crash events
  const crashRef = dbRef(database, `helmet_public/${userUID}/crashes`);
  onValue(crashRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      crashEvents.value = Object.values(data).reverse().slice(0, 5);
    }
  });
};

// Lifecycle hooks
onMounted(() => {
  setupFirebaseListeners();
  
  // Get user location for emergency contact tracking
  if (navigator.geolocation) {
    navigator.geolocation.getCurrentPosition(
      (position) => {
        emergencyContactLocation.value = {
          lat: position.coords.latitude,
          lng: position.coords.longitude
        };
      },
      (error) => {
        console.error('Error getting location:', error);
      }
    );
  }
});

onUnmounted(() => {
  // Clean up listeners if needed
});
</script>

<style scoped>
/* Custom Scrollbar */
.custom-scrollbar::-webkit-scrollbar {
  width: 8px;
  height: 8px;
}

.custom-scrollbar::-webkit-scrollbar-track {
  background: #f1f1f1;
  border-radius: 10px;
}

.custom-scrollbar::-webkit-scrollbar-thumb {
  background: linear-gradient(180deg, #7091E6, #3D52A0);
  border-radius: 10px;
}

.custom-scrollbar::-webkit-scrollbar-thumb:hover {
  background: linear-gradient(180deg, #5571c6, #2a3a70);
}
</style>