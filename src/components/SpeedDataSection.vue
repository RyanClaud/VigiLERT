<template>
  <section class="bg-[#EDE8F5] shadow rounded-xl p-6 mb-6">
    <h4 class="font-semibold text-lg mb-2 text-[#3D52A0]">Speed Monitoring</h4>
    <p class="text-sm text-[#7091E6] mb-4">Current and historical speed data</p>
    <div class="flex items-center mb-4">
      <span class="text-3xl font-bold text-[#7091E6] mr-2">{{ speedData[speedData.length - 1] }} kph</span>
      <span class="text-[#8697C4]">Current speed</span>
    </div>
    <Bar :data="chartData" :options="chartOptions" style="height: 130px;" />
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
import { Chart, BarElement, CategoryScale, LinearScale, Tooltip } from 'chart.js';
Chart.register(BarElement, CategoryScale, LinearScale, Tooltip);

const props = defineProps({
  speedData: Array,
  speedLimit: Number
});

const chartData = {
  labels: Array.from({ length: props.speedData.length }, (_, i) => i - props.speedData.length + 1),
  datasets: [
    {
      label: 'Speed (kph)',
      data: props.speedData,
      backgroundColor: props.speedData.map(s => s > props.speedLimit ? '#f87171' : '#3b82f6')
    }
  ]
};

const chartOptions = {
  responsive: true,
  plugins: { legend: { display: false } },
  scales: {
    y: { beginAtZero: true, max: Math.max(...props.speedData, props.speedLimit) + 10 }
  }
};
</script> 