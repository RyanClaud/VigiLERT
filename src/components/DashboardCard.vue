<template>
  <div class="bg-[#EDE8F5] shadow rounded-xl p-5 flex flex-col justify-between h-full transition hover:shadow-2xl hover:scale-105">
    <div class="flex items-center space-x-3">
      <span v-if="icon === 'status'">
        <svg class="h-7 w-7 text-green-500" fill="none" stroke="currentColor" viewBox="0 0 24 24"><circle cx="12" cy="12" r="10" stroke="currentColor" stroke-width="2" fill="none" /><path d="M9 12l2 2 4-4" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" /></svg>
      </span>
      <span v-else-if="icon === 'speed'">
        <svg class="h-7 w-7 text-blue-500" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path d="M12 3v2m6.364 1.636l-1.414 1.414M21 12h-2M19.364 18.364l-1.414-1.414M12 21v-2M4.636 19.364l1.414-1.414M3 12h2M4.636 4.636l1.414 1.414" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/><circle cx="12" cy="12" r="4" stroke="currentColor" stroke-width="2" fill="none"/></svg>
      </span>
      <span v-else-if="icon === 'alert'">
        <svg class="h-7 w-7 text-yellow-500" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path d="M12 9v2m0 4h.01" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/><circle cx="12" cy="12" r="10" stroke="currentColor" stroke-width="2" fill="none"/></svg>
      </span>
      <span v-else>
        <svg class="h-7 w-7 text-gray-400" fill="none" stroke="currentColor" viewBox="0 0 24 24"><circle cx="12" cy="12" r="10" stroke="currentColor" stroke-width="2" fill="none" /></svg>
      </span>
      <div>
        <div class="text-sm font-medium text-[#3D52A0]">{{ title }}</div>
        <div class="text-2xl font-bold text-[#7091E6]">{{ value }}</div>
      </div>
    </div>
    <div class="mt-2 text-xs text-[#8697C4]">{{ subtitle }}</div>
    <div class="mt-2 text-xs text-gray-400">{{ subtitle }}</div>
  </div>
</template>

<script setup>
defineProps({
  title: String,
  value: String,
  subtitle: String,
  icon: String,
  status: String
});
</script> 