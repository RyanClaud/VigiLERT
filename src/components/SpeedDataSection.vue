<template>
  <section class="bg-[#EDE8F5] shadow rounded-xl p-6 mb-6">
    <!-- Section Title -->
    <h4 class="font-semibold text-lg mb-2 text-[#3D52A0]">Speed Monitoring</h4>
    <p class="text-sm text-[#7091E6] mb-4">Current and historical speed data</p>

    <!-- Current Speed Display -->
    <div class="flex items-center mb-4">
      <span class="text-3xl font-bold text-[#7091E6] mr-2">{{ currentSpeed }} kph</span>
      <span class="text-[#8697C4]">Current speed</span>
    </div>

    <!-- Over-Speed Alert -->
    <div v-if="isOverSpeed" class="mb-4 p-3 bg-red-100 border-l-4 border-red-500 text-red-700 rounded-md animate-pulse">
      <p class="font-medium">⚠️ You're exceeding the speed limit!</p>
      <p class="text-sm mt-1">Current: {{ currentSpeed }} kph | Limit: {{ speedLimit }} kph</p>
    </div>

    <!-- Bar Chart -->
    <Bar :data="chartData" :options="chartOptions" style="height: 130px;" />

    <!-- Time Indicators -->
    <div class="flex items-center justify-between mt-2">
      <span class="text-xs text-[#ADBBD4]">-24h</span>
      <div class="flex items-center space-x-2">
        <span class="text-xs text-[#ADBBD4]">Speed Limit:</span>
        <span class="font-semibold text-[#3D52A0]">{{ speedLimit }} kph</span>
      </div>
      <span class="text-xs text-[#ADBBD4]">Now</span>
    </div>
  </section>
</template>

<script setup>
import { Bar } from 'vue-chartjs';
import {
  Chart,
  BarElement,
  CategoryScale,
  LinearScale,
  Tooltip
} from 'chart.js';

Chart.register(BarElement, CategoryScale, LinearScale, Tooltip);

import { ref, computed, watch, defineProps, defineEmits } from 'vue';

const emit = defineEmits(['overspeed']);

const props = defineProps({
  speedData: {
    type: Array,
    required: true
  },
  speedLimit: {
    type: Number,
    required: true
  }
});

// Computed property for current speed
const currentSpeed = computed(() => {
  return props.speedData[props.speedData.length - 1] || 0;
});

// Check if current speed exceeds the limit
const isOverSpeed = computed(() => {
  return currentSpeed.value > props.speedLimit;
});

// Watch for changes and emit overspeed event
watch(isOverSpeed, (newVal) => {
  if (newVal) {
    emit('overspeed', {
      speed: currentSpeed.value,
      limit: props.speedLimit
    });
  }
});

// Chart Data
const chartData = ref({
  labels: [],
  datasets: [
    {
      label: 'Speed (kph)',
      data: [],
      backgroundColor: []
    }
  ]
});

// Chart Options
const chartOptions = {
  responsive: true,
  plugins: {
    legend: { display: false }
  },
  scales: {
    y: {
      beginAtZero: true,
      max: Math.max(...props.speedData, props.speedLimit) + 10
    }
  }
};

// Watch for prop changes and update chart
watch(
  () => props,
  () => {
    // Update chart labels
    chartData.value.labels = props.speedData.map((_, i) => i - props.speedData.length + 1);

    // Update chart data
    chartData.value.datasets[0].data = [...props.speedData];

    // Set bar background color based on speed vs limit
    chartData.value.datasets[0].backgroundColor = props.speedData.map(s =>
      s > props.speedLimit ? '#f87171' : '#3b82f6'
    );

    // Dynamically adjust Y-axis max
    chartOptions.scales.y.max = Math.max(...props.speedData, props.speedLimit) + 10;
  },
  { deep: true, immediate: true }
);
</script>