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

    <!-- Motorcycle Speedometer -->
    <div class="relative flex flex-col items-center">
      <!-- Speedometer Gauge -->
      <div class="relative w-80 h-80 md:w-96 md:h-96">
        <!-- Outer Ring -->
        <svg class="w-full h-full transform -rotate-180" viewBox="0 0 200 200">
          <!-- Background Circle -->
          <circle
            cx="100"
            cy="100"
            r="85"
            fill="none"
            stroke="#E5E7EB"
            stroke-width="12"
          />
          
          <!-- Speed Arc (0-180 degrees) -->
          <circle
            cx="100"
            cy="100"
            r="85"
            fill="none"
            :stroke="speedColor"
            stroke-width="12"
            stroke-linecap="round"
            :stroke-dasharray="circumference"
            :stroke-dashoffset="dashOffset"
            class="transition-all duration-300"
          />
          
          <!-- Speed Limit Marker -->
          <line
            :x1="limitMarkerX1"
            :y1="limitMarkerY1"
            :x2="limitMarkerX2"
            :y2="limitMarkerY2"
            stroke="#EF4444"
            stroke-width="3"
            stroke-linecap="round"
          />
        </svg>

        <!-- Center Display -->
        <div class="absolute inset-0 flex flex-col items-center justify-center">
          <div class="text-center">
            <!-- GPS Icon -->
            <div class="mb-2">
              <span class="material-icons text-2xl text-blue-500 animate-pulse">satellite_alt</span>
            </div>
            
            <!-- Speed Display -->
            <p class="text-7xl md:text-8xl font-black leading-none" 
               :class="isOverSpeed ? 'text-red-600' : 'text-[#3D52A0]'">
              {{ displaySpeed }}
            </p>
            <p class="text-2xl md:text-3xl font-bold text-[#7091E6] mt-2">km/h</p>
            
            <!-- Status Indicator -->
            <div class="mt-4 flex items-center justify-center gap-2 px-4 py-2 rounded-full bg-white/80 backdrop-blur-sm shadow-lg">
              <span class="w-3 h-3 rounded-full animate-pulse" :class="isOverSpeed ? 'bg-red-500' : 'bg-green-500'"></span>
              <p class="text-sm font-bold" :class="isOverSpeed ? 'text-red-600' : 'text-green-600'">
                {{ isOverSpeed ? 'OVER LIMIT' : 'SAFE SPEED' }}
              </p>
            </div>
            
            <!-- GPS Source Label -->
            <div class="mt-3 flex items-center justify-center gap-1 text-xs text-gray-500">
              <span class="material-icons text-sm">router</span>
              <span class="font-medium">GPS Module</span>
            </div>
          </div>
        </div>

        <!-- Speed Markers -->
        <div class="absolute inset-0">
          <div v-for="marker in speedMarkers" :key="marker.speed" 
            class="absolute text-xs font-bold text-gray-600"
            :style="marker.style">
            {{ marker.speed }}
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

// Speedometer calculations (0-180 degrees for 0-120 km/h)
const maxSpeedDisplay = 120; // Max speed on gauge
const circumference = 2 * Math.PI * 85; // Circle circumference
const halfCircumference = circumference / 2; // Half circle for 180 degrees

const dashOffset = computed(() => {
  const percentage = Math.min(props.currentSpeed / maxSpeedDisplay, 1);
  return halfCircumference - (percentage * halfCircumference);
});

// Speed limit marker position (red line)
// With -rotate-180, the arc now starts at the correct position
// 0 km/h should be at 0° (right side), 120 km/h at 180° (left side)
const limitAngle = computed(() => {
  const percentage = Math.min(props.speedLimit / maxSpeedDisplay, 1);
  // Simple linear mapping: 0° at 0 km/h, 180° at 120 km/h
  const angle = percentage * 180; // 0° at 0 km/h, 180° at 120 km/h
  return angle;
});

const limitMarkerX1 = computed(() => {
  // Convert angle to radians
  const angleRad = limitAngle.value * (Math.PI / 180);
  return 100 + 75 * Math.cos(angleRad);
});

const limitMarkerY1 = computed(() => {
  const angleRad = limitAngle.value * (Math.PI / 180);
  return 100 + 75 * Math.sin(angleRad);
});

const limitMarkerX2 = computed(() => {
  const angleRad = limitAngle.value * (Math.PI / 180);
  return 100 + 95 * Math.cos(angleRad);
});

const limitMarkerY2 = computed(() => {
  const angleRad = limitAngle.value * (Math.PI / 180);
  return 100 + 95 * Math.sin(angleRad);
});

// Speed markers around the gauge
const speedMarkers = computed(() => {
  const markers = [0, 20, 40, 60, 80, 100, 120];
  return markers.map(speed => {
    const percentage = speed / maxSpeedDisplay;
    // Place numbers on top arc: 180° (left) to 360° (right)
    // This makes them go from middle-left, up and around to middle-right
    const angle = 180 + (percentage * 180); // 180° at 0, 360° at 120
    const angleRad = angle * (Math.PI / 180);
    const radius = 110;
    const x = 100 + radius * Math.cos(angleRad);
    const y = 100 + radius * Math.sin(angleRad);
    
    return {
      speed,
      style: {
        left: `${(x / 200) * 100}%`,
        top: `${(y / 200) * 100}%`,
        transform: 'translate(-50%, -50%)'
      }
    };
  });
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