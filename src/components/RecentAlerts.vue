<template>
  <div class="bg-[#EDE8F5] shadow rounded-xl p-6">
    <ul class="divide-y divide-[#ADBBD4]">
      <li v-for="(alert, i) in alerts" :key="i" class="py-4 flex items-start space-x-4 transition hover:bg-[#ADBBD4]/40 rounded-lg">
        <span>
          <svg v-if="alert.type === 'speed'" class="h-6 w-6 text-yellow-500" fill="none" stroke="currentColor" viewBox="0 0 24 24"><circle cx="12" cy="12" r="10" stroke="currentColor" stroke-width="2" fill="none" /><path d="M12 8v4m0 4h.01" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/></svg>
          <svg v-else-if="alert.type === 'drowsiness'" class="h-6 w-6 text-red-500" fill="none" stroke="currentColor" viewBox="0 0 24 24"><circle cx="12" cy="12" r="10" stroke="currentColor" stroke-width="2" fill="none" /><path d="M15 9l-6 6M9 9l6 6" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" /></svg>
          <svg v-else class="h-6 w-6 text-[#7091E6]" fill="none" stroke="currentColor" viewBox="0 0 24 24"><circle cx="12" cy="12" r="10" stroke="currentColor" stroke-width="2" fill="none" /></svg>
        </span>
        <div class="flex-1">
          <div class="font-semibold text-[#3D52A0]">{{ alert.message }}</div>
          <div class="text-xs text-[#7091E6]">{{ alert.details }}</div>
          <div v-if="alert.extra" class="text-xs text-red-600 font-medium mt-1">{{ alert.extra }}</div>
        </div>
        <div class="text-xs text-[#ADBBD4] whitespace-nowrap">{{ alert.time }}</div>
      </li>
    </ul>
  </div>
</template>

<script setup>
defineProps({
  alerts: Array
});
</script> 