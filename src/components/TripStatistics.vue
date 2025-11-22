<template>
  <div class="relative overflow-hidden bg-gradient-to-br from-[#7091E6]/10 to-[#3D52A0]/10 rounded-3xl shadow-2xl p-8 border border-[#7091E6]/30">
    <div class="absolute top-0 right-0 w-96 h-96 bg-gradient-to-br from-[#7091E6]/20 to-transparent rounded-full -mr-48 -mt-48"></div>
    
    <h3 class="relative text-2xl font-bold text-[#3D52A0] mb-6 flex items-center gap-3">
      <div class="bg-gradient-to-br from-[#7091E6] to-[#5571c6] p-3 rounded-2xl shadow-lg">
        <span class="material-icons text-3xl text-white">analytics</span>
      </div>
      <span>Current Trip Statistics</span>
    </h3>

    <div class="relative grid grid-cols-2 md:grid-cols-4 gap-4">
      <!-- Total Distance -->
      <div class="bg-white/80 backdrop-blur-sm p-5 rounded-2xl border-2 border-[#7091E6]/20 hover:border-[#7091E6] transition-all duration-300 hover:scale-105">
        <div class="flex items-center gap-3 mb-2">
          <div class="p-2 bg-blue-500 rounded-xl">
            <span class="material-icons text-white text-xl">straighten</span>
          </div>
          <span class="text-xs font-semibold text-gray-600 uppercase">Distance</span>
        </div>
        <p class="text-3xl font-bold text-[#3D52A0]">{{ stats.distance }}</p>
        <p class="text-xs text-gray-500 mt-1">kilometers</p>
      </div>

      <!-- Average Speed -->
      <div class="bg-white/80 backdrop-blur-sm p-5 rounded-2xl border-2 border-[#7091E6]/20 hover:border-[#7091E6] transition-all duration-300 hover:scale-105">
        <div class="flex items-center gap-3 mb-2">
          <div class="p-2 bg-green-500 rounded-xl">
            <span class="material-icons text-white text-xl">speed</span>
          </div>
          <span class="text-xs font-semibold text-gray-600 uppercase">Avg Speed</span>
        </div>
        <p class="text-3xl font-bold text-green-600">{{ stats.avgSpeed }}</p>
        <p class="text-xs text-gray-500 mt-1">km/h</p>
      </div>

      <!-- Max Speed -->
      <div class="bg-white/80 backdrop-blur-sm p-5 rounded-2xl border-2 border-[#7091E6]/20 hover:border-[#7091E6] transition-all duration-300 hover:scale-105">
        <div class="flex items-center gap-3 mb-2">
          <div class="p-2 bg-orange-500 rounded-xl">
            <span class="material-icons text-white text-xl">flash_on</span>
          </div>
          <span class="text-xs font-semibold text-gray-600 uppercase">Max Speed</span>
        </div>
        <p class="text-3xl font-bold text-orange-600">{{ stats.maxSpeed }}</p>
        <p class="text-xs text-gray-500 mt-1">km/h</p>
      </div>

      <!-- Duration -->
      <div class="bg-white/80 backdrop-blur-sm p-5 rounded-2xl border-2 border-[#7091E6]/20 hover:border-[#7091E6] transition-all duration-300 hover:scale-105">
        <div class="flex items-center gap-3 mb-2">
          <div class="p-2 bg-purple-500 rounded-xl">
            <span class="material-icons text-white text-xl">schedule</span>
          </div>
          <span class="text-xs font-semibold text-gray-600 uppercase">Duration</span>
        </div>
        <p class="text-3xl font-bold text-purple-600">{{ stats.duration }}</p>
        <p class="text-xs text-gray-500 mt-1">minutes</p>
      </div>

      <!-- Safety Score -->
      <div class="bg-white/80 backdrop-blur-sm p-5 rounded-2xl border-2 border-[#7091E6]/20 hover:border-[#7091E6] transition-all duration-300 hover:scale-105">
        <div class="flex items-center gap-3 mb-2">
          <div :class="['p-2 rounded-xl', getSafetyScoreColor()]">
            <span class="material-icons text-white text-xl">shield</span>
          </div>
          <span class="text-xs font-semibold text-gray-600 uppercase">Safety Score</span>
        </div>
        <p :class="['text-3xl font-bold', getSafetyScoreTextColor()]">{{ stats.safetyScore }}/100</p>
        <p class="text-xs text-gray-500 mt-1">{{ getSafetyLabel() }}</p>
      </div>

      <!-- Harsh Braking -->
      <div class="bg-white/80 backdrop-blur-sm p-5 rounded-2xl border-2 border-[#7091E6]/20 hover:border-[#7091E6] transition-all duration-300 hover:scale-105">
        <div class="flex items-center gap-3 mb-2">
          <div class="p-2 bg-red-500 rounded-xl">
            <span class="material-icons text-white text-xl">warning</span>
          </div>
          <span class="text-xs font-semibold text-gray-600 uppercase">Harsh Braking</span>
        </div>
        <p class="text-3xl font-bold text-red-600">{{ stats.harshBraking }}</p>
        <p class="text-xs text-gray-500 mt-1">incidents</p>
      </div>

      <!-- Rapid Acceleration -->
      <div class="bg-white/80 backdrop-blur-sm p-5 rounded-2xl border-2 border-[#7091E6]/20 hover:border-[#7091E6] transition-all duration-300 hover:scale-105">
        <div class="flex items-center gap-3 mb-2">
          <div class="p-2 bg-yellow-500 rounded-xl">
            <span class="material-icons text-white text-xl">trending_up</span>
          </div>
          <span class="text-xs font-semibold text-gray-600 uppercase">Rapid Accel</span>
        </div>
        <p class="text-3xl font-bold text-yellow-600">{{ stats.rapidAccel }}</p>
        <p class="text-xs text-gray-500 mt-1">incidents</p>
      </div>

      <!-- Sharp Turns -->
      <div class="bg-white/80 backdrop-blur-sm p-5 rounded-2xl border-2 border-[#7091E6]/20 hover:border-[#7091E6] transition-all duration-300 hover:scale-105">
        <div class="flex items-center gap-3 mb-2">
          <div class="p-2 bg-indigo-500 rounded-xl">
            <span class="material-icons text-white text-xl">turn_right</span>
          </div>
          <span class="text-xs font-semibold text-gray-600 uppercase">Sharp Turns</span>
        </div>
        <p class="text-3xl font-bold text-indigo-600">{{ stats.sharpTurns }}</p>
        <p class="text-xs text-gray-500 mt-1">incidents</p>
      </div>
    </div>

    <!-- Progress Bar for Safety Score -->
    <div class="relative mt-6 p-4 bg-white/60 backdrop-blur-sm rounded-2xl">
      <div class="flex justify-between items-center mb-2">
        <span class="text-sm font-semibold text-gray-700">Overall Safety Performance</span>
        <span :class="['text-sm font-bold', getSafetyScoreTextColor()]">{{ getSafetyLabel() }}</span>
      </div>
      <div class="w-full bg-gray-200 rounded-full h-4 overflow-hidden">
        <div 
          :class="['h-full rounded-full transition-all duration-500', getSafetyScoreBarColor()]"
          :style="{ width: `${stats.safetyScore}%` }"
        ></div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { computed } from 'vue';

const props = defineProps({
  stats: {
    type: Object,
    default: () => ({
      distance: '0.0',
      avgSpeed: '0',
      maxSpeed: '0',
      duration: '0',
      safetyScore: 0,
      harshBraking: 0,
      rapidAccel: 0,
      sharpTurns: 0
    })
  }
});

const getSafetyScoreColor = () => {
  const score = props.stats.safetyScore;
  if (score >= 80) return 'bg-green-500';
  if (score >= 60) return 'bg-yellow-500';
  return 'bg-red-500';
};

const getSafetyScoreTextColor = () => {
  const score = props.stats.safetyScore;
  if (score >= 80) return 'text-green-600';
  if (score >= 60) return 'text-yellow-600';
  return 'text-red-600';
};

const getSafetyScoreBarColor = () => {
  const score = props.stats.safetyScore;
  if (score >= 80) return 'bg-gradient-to-r from-green-500 to-green-600';
  if (score >= 60) return 'bg-gradient-to-r from-yellow-500 to-yellow-600';
  return 'bg-gradient-to-r from-red-500 to-red-600';
};

const getSafetyLabel = () => {
  const score = props.stats.safetyScore;
  if (score >= 90) return 'Excellent';
  if (score >= 80) return 'Good';
  if (score >= 60) return 'Fair';
  return 'Needs Improvement';
};
</script>
