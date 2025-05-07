<template>
  <section class="bg-[#EDE8F5] shadow rounded-xl p-6 mb-6">
    <h4 class="font-semibold text-lg mb-2 text-[#3D52A0]">Electrical Diagnostics</h4>
    <p class="text-sm text-[#7091E6] mb-4">Motorcycle lighting systems</p>
    <div class="grid grid-cols-1 md:grid-cols-3 gap-4">
      <div v-for="item in displayDiagnostics" :key="item.name" class="flex items-center space-x-3 p-3 rounded-lg border transition hover:bg-[#ADBBD4]/40"
        :class="{
          'border-green-200 bg-green-50': item.status === 'ok',
          'border-yellow-200 bg-yellow-50': item.status === 'warning',
          'border-red-200 bg-red-50': item.status === 'error',
          'border-gray-200 bg-gray-50': item.status === 'unknown'
        }">
        <!-- Light Icon -->
        <span>
          <component :is="getLightIcon(item.name)" class="h-6 w-6 mr-1" />
        </span>
        <!-- Status Icon -->
        <span>
          <svg v-if="item.status === 'ok'" class="h-6 w-6 text-green-500" fill="none" stroke="currentColor" viewBox="0 0 24 24"><circle cx="12" cy="12" r="10" stroke="currentColor" stroke-width="2" fill="none" /><path d="M9 12l2 2 4-4" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" /></svg>
          <svg v-else-if="item.status === 'warning'" class="h-6 w-6 text-yellow-500" fill="none" stroke="currentColor" viewBox="0 0 24 24"><circle cx="12" cy="12" r="10" stroke="currentColor" stroke-width="2" fill="none" /><path d="M12 8v4m0 4h.01" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/></svg>
          <svg v-else-if="item.status === 'error'" class="h-6 w-6 text-red-500" fill="none" stroke="currentColor" viewBox="0 0 24 24"><circle cx="12" cy="12" r="10" stroke="currentColor" stroke-width="2" fill="none" /><path d="M15 9l-6 6M9 9l6 6" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" /></svg>
          <svg v-else class="h-6 w-6 text-gray-400" fill="none" stroke="currentColor" viewBox="0 0 24 24"><circle cx="12" cy="12" r="10" stroke="currentColor" stroke-width="2" fill="none" /></svg>
        </span>
        <div>
          <div class="font-medium text-[#3D52A0] flex items-center gap-1">
            {{ item.name }}
          </div>
          <div class="text-xs text-[#7091E6]">
            {{ item.statusText }}
          </div>
        </div>
        <div class="ml-auto text-xs text-[#ADBBD4]">{{ item.time || '' }}</div>
      </div>
    </div>
  </section>
</template>

<script setup>
import { computed, h } from 'vue';
const props = defineProps({ diagnostics: Array });

const PARTS = [
  'Headlight',
  'Taillight',
  'Left Signal',
  'Right Signal',
  'Brake Light'
];

const statusTextMap = {
  ok: 'Operating normally',
  warning: 'Needs attention',
  error: 'Requires immediate service',
  unknown: 'Unknown status'
};

// SVG icon components for each part
const HeadlightIcon = {
  render() {
    return h('svg', { class: 'h-6 w-6 text-[#3D52A0]', fill: 'none', stroke: 'currentColor', viewBox: '0 0 24 24' }, [
      h('circle', { cx: '12', cy: '12', r: '6', stroke: 'currentColor', 'stroke-width': '2', fill: 'none' }),
      h('path', { d: 'M18 12h2M4 12h2M12 6V4M12 20v-2', stroke: 'currentColor', 'stroke-width': '2', 'stroke-linecap': 'round' })
    ]);
  }
};
const TaillightIcon = {
  render() {
    return h('svg', { class: 'h-6 w-6 text-[#7091E6]', fill: 'none', stroke: 'currentColor', viewBox: '0 0 24 24' }, [
      h('rect', { x: '7', y: '7', width: '10', height: '10', rx: '3', stroke: 'currentColor', 'stroke-width': '2', fill: 'none' }),
      h('path', { d: 'M7 12h10', stroke: 'currentColor', 'stroke-width': '2', 'stroke-linecap': 'round' })
    ]);
  }
};
const LeftSignalIcon = {
  render() {
    return h('svg', { class: 'h-6 w-6 text-yellow-500', fill: 'none', stroke: 'currentColor', viewBox: '0 0 24 24' }, [
      h('path', { d: 'M15 19l-7-7 7-7', stroke: 'currentColor', 'stroke-width': '2', 'stroke-linecap': 'round', 'stroke-linejoin': 'round' })
    ]);
  }
};
const RightSignalIcon = {
  render() {
    return h('svg', { class: 'h-6 w-6 text-yellow-500', fill: 'none', stroke: 'currentColor', viewBox: '0 0 24 24' }, [
      h('path', { d: 'M9 5l7 7-7 7', stroke: 'currentColor', 'stroke-width': '2', 'stroke-linecap': 'round', 'stroke-linejoin': 'round' })
    ]);
  }
};
const BrakeLightIcon = {
  render() {
    return h('svg', { class: 'h-6 w-6 text-red-500', fill: 'none', stroke: 'currentColor', viewBox: '0 0 24 24' }, [
      h('rect', { x: '6', y: '6', width: '12', height: '12', rx: '6', stroke: 'currentColor', 'stroke-width': '2', fill: 'none' }),
      h('path', { d: 'M6 12h12', stroke: 'currentColor', 'stroke-width': '2', 'stroke-linecap': 'round' })
    ]);
  }
};

function getLightIcon(name) {
  switch (name) {
    case 'Headlight': return HeadlightIcon;
    case 'Taillight': return TaillightIcon;
    case 'Left Signal': return LeftSignalIcon;
    case 'Right Signal': return RightSignalIcon;
    case 'Brake Light': return BrakeLightIcon;
    default: return HeadlightIcon;
  }
}

const displayDiagnostics = computed(() =>
  PARTS.map(name => {
    const found = (props.diagnostics || []).find(d => d.name === name);
    if (found) {
      return {
        ...found,
        statusText: statusTextMap[found.status] || statusTextMap.unknown,
        status: found.status || 'unknown'
      };
    } else {
      return {
        name,
        status: 'unknown',
        statusText: statusTextMap.unknown,
        time: ''
      };
    }
  })
);
</script> 