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
      <!-- Rider Status Cards -->
      <div class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-5 gap-5 mb-8">
        <!-- Rider Status -->
        <div :class="[
          'group relative overflow-hidden text-white rounded-3xl shadow-xl p-6 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer',
          riderStatus === 'Inactive' ? 'bg-gradient-to-br from-red-600 via-red-500 to-red-700 animate-pulse' : 'bg-gradient-to-br from-[#3D52A0] via-[#4a5fb8] to-[#2a3a70]'
        ]">
          <div class="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-4">
              <div :class="['p-3 rounded-xl', riderStatus === 'Inactive' ? 'bg-white/30 animate-bounce' : 'bg-white/20']">
                <svg class="w-10 h-10 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M16 7a4 4 0 11-8 0 4 4 0 018 0zM12 14a7 7 0 00-7 7h14a7 7 0 00-7-7z"/>
                </svg>
              </div>
              <div :class="['w-3 h-3 rounded-full', riderStatus === 'Inactive' ? 'bg-yellow-300 animate-ping' : 'bg-green-400 animate-pulse']"></div>
            </div>
            <div class="mt-auto">
              <p class="text-sm opacity-80 mb-1">Rider Status</p>
              <p class="text-2xl font-bold">{{ riderStatus }}</p>
              <p class="text-xs opacity-70 mt-1">{{ riderSubtitle }}</p>
            </div>
          </div>
        </div>
        
        <!-- Current Speed -->
        <div :class="[
          'group relative overflow-hidden text-white rounded-3xl shadow-xl p-6 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer',
          isOverSpeed ? 'bg-gradient-to-br from-red-500 via-red-600 to-red-700 animate-pulse' : 'bg-gradient-to-br from-[#7091E6] via-[#6081d6] to-[#5571c6]'
        ]">
          <div class="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-4">
              <div class="bg-white/20 p-3 rounded-xl">
                <svg class="w-10 h-10 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M13 10V3L4 14h7v7l9-11h-7z"/>
                </svg>
              </div>
              <div :class="['w-3 h-3 rounded-full', isOverSpeed ? 'bg-yellow-300 animate-pulse' : 'bg-blue-300']"></div>
            </div>
            <div class="mt-auto">
              <p class="text-sm opacity-80 mb-1">Current Speed</p>
              <p class="text-2xl font-bold">{{ currentSpeedText }}</p>
              <p class="text-xs opacity-70 mt-1">{{ isOverSpeed ? 'Over speed limit!' : speedSubtitle }}</p>
            </div>
          </div>
        </div>
        
        <!-- Alertness -->
        <div :class="[
          'group relative overflow-hidden rounded-3xl shadow-xl p-6 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer',
          alertnessStatus === 'Normal' ? 'bg-gradient-to-br from-[#8697C4] via-[#7687b4] to-[#6677a4] text-white' : 'bg-gradient-to-br from-yellow-400 via-yellow-500 to-yellow-600 text-gray-800'
        ]">
          <div class="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-4">
              <div class="bg-white/20 p-3 rounded-xl">
                <svg class="w-10 h-10" :class="alertnessStatus === 'Normal' ? 'text-white' : 'text-gray-800'" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 12a3 3 0 11-6 0 3 3 0 016 0z"/>
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M2.458 12C3.732 7.943 7.523 5 12 5c4.478 0 8.268 2.943 9.542 7-1.274 4.057-5.064 7-9.542 7-4.477 0-8.268-2.943-9.542-7z"/>
                </svg>
              </div>
              <div :class="['w-3 h-3 rounded-full', alertnessStatus === 'Normal' ? 'bg-green-400' : 'bg-orange-400 animate-pulse']"></div>
            </div>
            <div class="mt-auto">
              <p class="text-sm opacity-80 mb-1">Alertness</p>
              <p class="text-2xl font-bold">{{ alertnessStatus }}</p>
              <p class="text-xs opacity-70 mt-1">{{ alertnessSubtitle }}</p>
            </div>
          </div>
        </div>
        
        <!-- Alcohol Detection -->
        <div :class="[
          'group relative overflow-hidden rounded-3xl shadow-xl p-6 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer',
          alcoholStatus === 'Safe' ? 'bg-gradient-to-br from-[#3D52A0] via-[#4a5fb8] to-[#2a3a70] text-white' : 'bg-gradient-to-br from-red-500 via-red-600 to-red-700 text-white'
        ]">
          <div class="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-4">
              <div class="bg-white/20 p-3 rounded-xl">
                <svg class="w-10 h-10 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12l2 2 4-4m5.618-4.016A11.955 11.955 0 0112 2.944a11.955 11.955 0 01-8.618 3.04A12.02 12.02 0 003 9c0 5.591 3.824 10.29 9 11.622 5.176-1.332 9-6.03 9-11.622 0-1.042-.133-2.052-.382-3.016z"/>
                </svg>
              </div>
              <div :class="['w-3 h-3 rounded-full', alcoholStatus === 'Safe' ? 'bg-green-400' : 'bg-red-300 animate-pulse']"></div>
            </div>
            <div class="mt-auto">
              <p class="text-sm opacity-80 mb-1">Alcohol Detection</p>
              <p class="text-2xl font-bold">{{ alcoholStatus }}</p>
              <p class="text-xs opacity-70 mt-1">{{ alcoholSubtitle }}</p>
            </div>
          </div>
        </div>
        
        <!-- Crash Detection Card -->
        <div :class="[
          'group relative overflow-hidden rounded-3xl shadow-xl p-6 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer',
          crashDisplayStatus === 'Stable' ? 'bg-gradient-to-br from-green-600 via-green-700 to-green-800 text-white' : 'bg-gradient-to-br from-red-600 via-red-700 to-red-800 text-white animate-pulse'
        ]">
          <div class="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-4">
              <div class="bg-white/20 p-3 rounded-xl">
                <svg class="w-10 h-10 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 9v2m0 4h.01m-6.938 4h13.856c1.54 0 2.502-1.667 1.732-3L13.732 4c-.77-1.333-2.694-1.333-3.464 0L3.34 16c-.77 1.333.192 3 1.732 3z"/>
                </svg>
              </div>
              <div :class="['w-3 h-3 rounded-full', crashDisplayStatus === 'Stable' ? 'bg-emerald-300' : 'bg-yellow-300 animate-pulse']"></div>
            </div>
            <div class="mt-auto">
              <p class="text-sm opacity-80 mb-1">Vehicle Status</p>
              <p class="text-2xl font-bold">{{ crashDisplayMessage }}</p>
              <p class="text-xs opacity-70 mt-1">{{ crashDisplayStatus === 'Stable' ? 'No incidents' : 'Alert active' }}</p>
            </div>
          </div>
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
      <div id="alerts" class="relative overflow-hidden bg-white/80 backdrop-blur-lg rounded-3xl shadow-2xl p-8 mb-6 border border-white/50 transition-all duration-300 hover:shadow-3xl">
        <div class="absolute top-0 right-0 w-64 h-64 bg-gradient-to-br from-[#7091E6]/10 to-[#3D52A0]/10 rounded-full -mr-32 -mt-32"></div>
        <h3 class="relative font-bold text-2xl mb-6 text-[#3D52A0] flex items-center justify-between">
          <span class="flex items-center gap-3">
            <div class="bg-gradient-to-br from-[#7091E6] to-[#3D52A0] p-3 rounded-2xl shadow-lg">
              <span class="material-icons text-3xl text-white">notifications_active</span>
            </div>
            <span>Recent Alerts</span>
            <span v-if="alerts.length > 0" class="bg-red-500 text-white text-sm px-3 py-1 rounded-full">
              {{ alerts.length }}
            </span>
          </span>
          <button @click="toggleAlerts"
            class="text-sm font-semibold px-6 py-3 rounded-xl bg-gradient-to-r from-[#7091E6] to-[#5571c6] text-white hover:from-[#3D52A0] hover:to-[#2a3a70] transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105">
            {{ showAlerts ? 'Hide' : 'Show' }}
          </button>
        </h3>
        <div v-if="showAlerts" class="relative max-h-64 overflow-y-auto custom-scrollbar">
          <RecentAlerts :alerts="alerts" :crash-events="crashEvents" />
        </div>
      </div>

      <!-- Recent Trips Preview -->
      <section id="trips" class="mb-6">
        <div class="relative overflow-hidden bg-white/80 backdrop-blur-lg rounded-3xl shadow-2xl p-8 border border-white/50 transition-all duration-300 hover:shadow-3xl">
          <div class="absolute top-0 left-0 w-64 h-64 bg-gradient-to-br from-[#3D52A0]/10 to-[#7091E6]/10 rounded-full -ml-32 -mt-32"></div>
          <h3 class="relative text-2xl font-bold text-[#3D52A0] mb-6 flex items-center gap-3">
            <div class="bg-gradient-to-br from-[#3D52A0] to-[#2a3a70] p-3 rounded-2xl shadow-lg">
              <span class="material-icons text-3xl text-white">route</span>
            </div>
            <span>Recent Trips</span>
          </h3>
          <div v-if="recentTrips.length > 0"
            class="relative space-y-4 max-h-64 overflow-y-auto custom-scrollbar">
            <div v-for="trip in recentTrips" :key="trip.id" 
              class="group relative overflow-hidden p-6 border-2 border-white/50 rounded-2xl hover:border-[#7091E6] hover:shadow-xl transition-all duration-300 bg-gradient-to-br from-white to-[#EDE8F5]/30 backdrop-blur-sm">
              <div class="absolute top-0 right-0 w-32 h-32 bg-gradient-to-br from-[#7091E6]/10 to-transparent rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
              <div class="relative">
                <div class="flex items-center gap-2 mb-3">
                  <div class="bg-gradient-to-r from-[#7091E6] to-[#5571c6] px-3 py-1 rounded-lg">
                    <p class="text-xs text-white font-bold uppercase tracking-wider">Route Details</p>
                  </div>
                </div>
                <div class="space-y-2 mb-4">
                  <p class="text-sm flex items-start gap-2">
                    <span class="material-icons text-[#3D52A0] text-lg">trip_origin</span>
                    <span><strong class="text-[#3D52A0]">From:</strong> <span class="text-gray-700">{{ trip.startLocationName || formatLatLng(trip.startLat, trip.startLng) }}</span></span>
                  </p>
                  <p class="text-sm flex items-start gap-2">
                    <span class="material-icons text-[#3D52A0] text-lg">location_on</span>
                    <span><strong class="text-[#3D52A0]">To:</strong> <span class="text-gray-700">{{ trip.endLocationName || formatLatLng(trip.endLat, trip.endLng) }}</span></span>
                  </p>
                  <p class="text-sm flex items-center gap-2">
                    <span class="material-icons text-[#3D52A0] text-lg">speed</span>
                    <span><strong class="text-[#3D52A0]">Max Speed:</strong> <span class="text-gray-700 font-semibold">{{ trip.maxSpeed || 'N/A' }} km/h</span></span>
                  </p>
                </div>
                <div class="flex gap-3">
                  <a :href="getGoogleMapsLink(trip)" target="_blank" rel="noopener noreferrer"
                    class="flex-1 inline-flex items-center justify-center gap-2 px-4 py-3 bg-gradient-to-r from-[#7091E6] to-[#5571c6] text-white text-sm font-semibold rounded-xl hover:from-[#3D52A0] hover:to-[#2a3a70] transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105">
                    <span class="material-icons text-lg">navigation</span>
                    Navigate
                  </a>
                </div>
              </div>
            </div>
          </div>
          <div v-else class="relative p-12 text-center">
            <div class="inline-block p-6 bg-gradient-to-br from-gray-100 to-gray-200 rounded-full mb-4">
              <span class="material-icons text-7xl text-gray-400">explore_off</span>
            </div>
            <p class="text-gray-600 font-semibold text-lg">No trips found yet</p>
            <p class="text-sm text-gray-400 mt-2">Trip history will appear here</p>
          </div>
        </div>
      </section>

      <!-- Crash Events -->
      <section class="mt-8" v-if="crashEvents.length > 0">
        <div class="relative overflow-hidden bg-gradient-to-br from-red-50 to-red-100/50 backdrop-blur-lg rounded-3xl shadow-2xl p-8 border-l-8 border-red-500">
          <div class="absolute top-0 right-0 w-96 h-96 bg-red-500/10 rounded-full -mr-48 -mt-48"></div>
          <h3 class="relative font-bold text-2xl mb-6 text-red-600 flex items-center gap-3">
            <div class="bg-gradient-to-br from-red-500 to-red-600 p-3 rounded-2xl shadow-lg animate-pulse">
              <span class="material-icons text-3xl text-white">warning</span>
            </div>
            <span>Crash Site Locations</span>
          </h3>
          <div class="relative max-h-64 overflow-y-auto space-y-4 custom-scrollbar">
            <div v-for="(event, index) in crashEvents" :key="index" 
              class="group relative overflow-hidden p-6 border-2 border-red-300 rounded-2xl bg-white hover:shadow-xl transition-all duration-300">
              <div class="absolute top-0 right-0 w-32 h-32 bg-red-500/10 rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
              <div class="relative">
                <div class="space-y-2 mb-4">
                  <p class="text-sm flex items-center gap-2">
                    <span class="material-icons text-red-600 text-lg">flash_on</span>
                    <span><strong class="text-red-700">Impact:</strong> <span class="text-gray-700 font-semibold">{{ event.impactStrength }} g</span></span>
                  </p>
                  <p class="text-sm flex items-start gap-2">
                    <span class="material-icons text-red-600 text-lg">location_on</span>
                    <span><strong class="text-red-700">Location:</strong> <span class="text-gray-700">{{ event.location }}</span></span>
                  </p>
                </div>
                <div class="flex flex-wrap gap-3">
                  <a :href="getGoogleMapsLink(event.lat, event.lng)" target="_blank" rel="noopener noreferrer"
                    class="inline-flex items-center gap-2 px-5 py-3 bg-gradient-to-r from-green-600 to-green-700 text-white text-sm font-semibold rounded-xl hover:from-green-700 hover:to-green-800 transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105">
                    <span class="material-icons text-lg">location_on</span>
                    See Location
                  </a>
                </div>
              </div>
            </div>
          </div>
        </div>
      </section>
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