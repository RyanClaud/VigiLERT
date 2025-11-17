<template>
  <div class="flex gap-2 flex-wrap">
    <button
      v-for="tab in tabs"
      :key="tab"
      @click="$emit('update:modelValue', tab)"
      :class="[
        'relative px-6 py-3 text-sm font-semibold rounded-xl transition-all duration-300 focus:outline-none overflow-hidden group',
        modelValue === tab 
          ? 'bg-gradient-to-r from-[#7091E6] to-[#5571c6] text-white shadow-lg transform scale-105' 
          : 'bg-white/60 text-[#3D52A0] hover:bg-white hover:shadow-md hover:scale-102'
      ]"
    >
      <span class="relative z-10 flex items-center gap-2">
        <span class="material-icons text-lg" v-if="getIcon(tab)">{{ getIcon(tab) }}</span>
        {{ tab }}
      </span>
      <div v-if="modelValue === tab" class="absolute inset-0 bg-gradient-to-r from-[#5571c6] to-[#3D52A0] opacity-0 group-hover:opacity-100 transition-opacity duration-300"></div>
    </button>
  </div>
</template>

<script setup>
defineProps({
  tabs: Array,
  modelValue: String
});

const getIcon = (tab) => {
  const icons = {
    'My Location': 'location_on',
    'Speed Data': 'speed',
    'Diagnostics': 'analytics'
  };
  return icons[tab] || '';
};
</script> 