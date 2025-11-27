<template>
  <section class="space-y-6">
    <!-- GPS Status Banner -->
    <div class="p-4 bg-gradient-to-r from-blue-500 to-blue-600 border-l-4 border-blue-700 text-white rounded-2xl shadow-xl">
      <div class="flex items-center justify-between">
        <div class="flex items-center gap-3">
          <span class="material-icons text-3xl animate-pulse">gps_fixed</span>
          <div>
            <p class="font-bold text-lg">GPS Speed Tracking Active</p>
            <p class="text-sm mt-1 opacity-90">Real-time data from GPS module</p>
          </div>
        </div>
        <div class="flex items-center gap-2">
          <div class="w-3 h-3 bg-green-400 rounded-full animate-pulse"></div>
          <span class="text-sm font-semibold">LIVE</span>
        </div>
      </div>
    </div>

    <!-- Over-Speed Alert -->
    <div v-if="isOverSpeed" class="p-4 bg-gradient-to-r from-red-500 to-red-600 border-l-4 border-red-700 text-white rounded-2xl animate-pulse shadow-xl">
      <div class="flex items-center gap-3">
        <span class="material-icons text-4xl">warning</span>
        <div>
          <p class="font-bold text-lg">⚠️ Speed Limit Exceeded!</p>
          <p class="text-sm mt-1">Current: {{ displaySpeed }} km/h | Limit: {{ speedLimit }} km/h</p>
        </div>
      </div>
    </div>

    <!-- Modern Digital Speedometer -->
    <div class="relative flex flex-col items-center">
      <!-- Main Speed Display Card -->
      <div class="relative w-full max-w-2xl">
        <!-- Large Speed Display -->
        <div class="relative overflow-hidden bg-gradient-to-br from-white to-gray-50 rounded-3xl shadow-2xl p-8 md:p-12 border-4"
             :class="isOverSpeed ? 'border-red-500' : 'border-blue-500'">
          
          <!-- Background Pattern -->
          <div class="absolute inset-0 opacity-5">
            <div class="absolute inset-0" style="background-image: radial-gradient(circle, #3D52A0 1px, transparent 1px); background-size: 20px 20px;"></div>
          </div>
          
          <!-- Content -->
          <div class="relative">
            <!-- GPS Status -->
            <div class="flex items-center justify-center gap-2 mb-6">
              <span class="material-icons text-3xl text-blue-500 animate-pulse">satellite_alt</span>
              <div class="flex items-center gap-2">
                <div class="w-2 h-2 bg-green-500 rounded-full animate-pulse"></div>
                <span class="text-sm font-bold text-gray-600">GPS TRACKING</span>
              </div>
            </div>
            
            <!-- Speed Number -->
            <div class="text-center mb-6">
              <div class="inline-block">
                <p class="text-9xl md:text-[12rem] font-black leading-none tracking-tight"
                   :class="isOverSpeed ? 'text-red-600' : 'text-[#3D52A0]'">
                  {{ displaySpeed }}
                </p>
                <div class="flex items-center justify-center gap-2 mt-2">
                  <div class="h-1 w-16 rounded-full" :class="isOverSpeed ? 'bg-red-500' : 'bg-blue-500'"></div>
                  <p class="text-3xl md:text-4xl font-bold text-gray-600">km/h</p>
                  <div class="h-1 w-16 rounded-full" :class="isOverSpeed ? 'bg-red-500' : 'bg-blue-500'"></div>
                </div>
              </div>
            </div>
            
            <!-- Status Badge -->
            <div class="flex justify-center mb-6">
              <div class="inline-flex items-center gap-3 px-8 py-4 rounded-2xl shadow-lg"
                   :class="isOverSpeed ? 'bg-red-500' : 'bg-green-500'">
                <span class="material-icons text-3xl text-white">
                  {{ isOverSpeed ? 'warning' : 'check_circle' }}
                </span>
                <div class="text-white">
                  <p class="text-xl font-black">{{ isOverSpeed ? 'OVER LIMIT' : 'SAFE SPEED' }}</p>
                  <p class="text-sm opacity-90">{{ isOverSpeed ? 'Slow down!' : 'Within speed limit' }}</p>
                </div>
              </div>
            </div>
            
            <!-- Speed Progress Bar -->
            <div class="relative">
              <div class="flex items-center justify-between text-xs font-bold text-gray-500 mb-2">
                <span>0 km/h</span>
                <span class="text-orange-500">LIMIT: {{ speedLimit }} km/h</span>
                <span>120 km/h</span>
              </div>
              <div class="relative h-6 bg-gray-200 rounded-full overflow-hidden shadow-inner">
                <!-- Progress Fill -->
                <div class="absolute inset-y-0 left-0 rounded-full transition-all duration-500 ease-out"
                     :style="{ width: progressWidth }"
                     :class="isOverSpeed ? 'bg-gradient-to-r from-red-500 to-red-600' : 'bg-gradient-to-r from-green-500 to-blue-500'">
                  <div class="absolute inset-0 bg-white/20 animate-pulse"></div>
                </div>
                
                <!-- Speed Limit Marker -->
                <div class="absolute inset-y-0 w-1 bg-orange-500 shadow-lg"
                     :style="{ left: limitMarkerPosition }">
                  <div class="absolute -top-2 left-1/2 transform -translate-x-1/2 w-0 h-0 border-l-4 border-r-4 border-b-4 border-transparent border-b-orange-500"></div>
                </div>
              </div>
              
              <!-- Speed Scale -->
              <div class="flex justify-between mt-2">
                <div v-for="mark in [0, 20, 40, 60, 80, 100, 120]" :key="mark"
                     class="text-xs font-semibold"
                     :class="mark === speedLimit ? 'text-orange-500' : 'text-gray-400'">
                  {{ mark }}
                </div>
              </div>
            </div>
            
            <!-- GPS Source -->
            <div class="flex items-center justify-center gap-2 mt-6 text-sm text-gray-500">
              <span class="material-icons text-lg">router</span>
              <span class="font-semibold">Real-time GPS Module Data</span>
            </div>
          </div>
        </div>
      </div>

      <!-- Speed Info Cards -->
      <div class="grid grid-cols-3 gap-4 mt-8 w-full max-w-md">
        <!-- Max Speed -->
        <div class="bg-gradient-to-br from-blue-500 to-blue-600 rounded-2xl p-5 text-white text-center shadow-xl hover:shadow-2xl transition-all duration-300 transform hover:scale-105">
          <div class="bg-white/20 backdrop-blur-sm rounded-full w-12 h-12 flex items-center justify-center mx-auto mb-2">
            <span class="material-icons text-2xl">trending_up</span>
          </div>
          <p class="text-3xl font-black">{{ maxSpeed }}</p>
          <p class="text-xs opacity-90 font-semibold mt-1">Max Speed</p>
          <p class="text-xs opacity-70 mt-1">km/h</p>
        </div>

        <!-- Avg Speed -->
        <div class="bg-gradient-to-br from-green-500 to-green-600 rounded-2xl p-5 text-white text-center shadow-xl hover:shadow-2xl transition-all duration-300 transform hover:scale-105">
          <div class="bg-white/20 backdrop-blur-sm rounded-full w-12 h-12 flex items-center justify-center mx-auto mb-2">
            <span class="material-icons text-2xl">speed</span>
          </div>
          <p class="text-3xl font-black">{{ avgSpeed }}</p>
          <p class="text-xs opacity-90 font-semibold mt-1">Avg Speed</p>
          <p class="text-xs opacity-70 mt-1">km/h</p>
        </div>

        <!-- Speed Limit -->
        <div class="bg-gradient-to-br from-orange-500 to-orange-600 rounded-2xl p-5 text-white text-center shadow-xl hover:shadow-2xl transition-all duration-300 transform hover:scale-105">
          <div class="bg-white/20 backdrop-blur-sm rounded-full w-12 h-12 flex items-center justify-center mx-auto mb-2">
            <span class="material-icons text-2xl">warning</span>
          </div>
          <p class="text-3xl font-black">{{ speedLimit }}</p>
          <p class="text-xs opacity-90 font-semibold mt-1">Speed Limit</p>
          <p class="text-xs opacity-70 mt-1">km/h</p>
        </div>
      </div>

      <!-- GPS Data Info -->
      <div class="mt-6 p-4 bg-gradient-to-r from-gray-50 to-gray-100 rounded-2xl border border-gray-200">
        <div class="flex items-center justify-center gap-2 text-sm text-gray-600">
          <span class="material-icons text-lg text-blue-500">info</span>
          <p class="font-medium">Speed data received from GPS module in real-time</p>
        </div>
        <div class="flex items-center justify-center gap-4 mt-2 text-xs text-gray-500">
          <div class="flex items-center gap-1">
            <span class="w-2 h-2 bg-green-500 rounded-full animate-pulse"></span>
            <span>Live Updates</span>
          </div>
          <div class="flex items-center gap-1">
            <span class="material-icons text-sm">update</span>
            <span>Auto Refresh</span>
          </div>
          <div class="flex items-center gap-1">
            <span class="material-icons text-sm">gps_fixed</span>
            <span>GPS Tracking</span>
          </div>
        </div>
      </div>
    </div>
  </section>
</template>

<script setup>
import { computed, watch } from 'vue';

const emit = defineEmits(['overspeed']);

const props = defineProps({
  currentSpeed: {
    type: Number,
    required: true,
    default: 0
  },
  speedLimit: {
    type: Number,
    required: true,
    default: 80
  },
  speedHistory: {
    type: Array,
    default: () => []
  }
});

// Display speed (rounded)
const displaySpeed = computed(() => {
  return Math.round(props.currentSpeed);
});

// Check if current speed exceeds the limit
const isOverSpeed = computed(() => {
  return props.currentSpeed > props.speedLimit;
});

// Speed color based on current speed
const speedColor = computed(() => {
  const speed = props.currentSpeed;
  const limit = props.speedLimit;
  
  if (speed > limit) return '#EF4444'; // Red
  if (speed > limit * 0.8) return '#F59E0B'; // Orange
  return '#10B981'; // Green
});

// Calculate max speed from history
const maxSpeed = computed(() => {
  if (props.speedHistory.length === 0) return 0;
  return Math.round(Math.max(...props.speedHistory));
});

// Calculate average speed
const avgSpeed = computed(() => {
  if (props.speedHistory.length === 0) return 0;
  const sum = props.speedHistory.reduce((a, b) => a + b, 0);
  return Math.round(sum / props.speedHistory.length);
});

// Progress bar calculations
const maxSpeedDisplay = 120; // Max speed on gauge

// Progress bar width (0-100%)
const progressWidth = computed(() => {
  const percentage = Math.min((props.currentSpeed / maxSpeedDisplay) * 100, 100);
  return `${percentage}%`;
});

// Speed limit marker position on progress bar
const limitMarkerPosition = computed(() => {
  const percentage = Math.min((props.speedLimit / maxSpeedDisplay) * 100, 100);
  return `${percentage}%`;
});

// Watch for overspeed and emit event
watch(isOverSpeed, (newVal) => {
  if (newVal) {
    emit('overspeed', {
      speed: props.currentSpeed,
      limit: props.speedLimit
    });
  }
});
</script>