<template>
  <section class="bg-[#EDE8F5] shadow rounded-2xl p-6 mb-6">
    <h4 class="font-semibold text-lg mb-2 text-[#3D52A0]">Current Location</h4>
    <p class="text-sm text-[#7091E6] mb-4">Real-time GPS tracking</p>
    <l-map
      v-if="location.lat && location.lng"
      :zoom="15"
      :center="[location.lat, location.lng]"
      style="height: 350px; width: 100%; max-width: 100%;"
      class="rounded-xl mb-4 shadow-lg"
    >
      <l-tile-layer
        url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
      />
      <l-marker :lat-lng="[location.lat, location.lng]" />
    </l-map>
    <div class="flex flex-col md:flex-row items-start md:items-center justify-between mt-2 gap-2">
      <div>
        <div class="font-medium text-[#3D52A0]">{{ user.name }}</div>
        <div class="text-xs text-[#8697C4]">Lat: {{ location.lat }}, Lng: {{ location.lng }}</div>
      </div>
      <button class="px-4 py-2 bg-[#7091E6] text-white rounded-lg shadow hover:bg-[#3D52A0] transition text-sm font-semibold">Navigate</button>
    </div>
  </section>
</template>

<script setup>
import 'leaflet/dist/leaflet.css';
import { LMap, LTileLayer, LMarker } from '@vue-leaflet/vue-leaflet';
defineProps({
  location: Object,
  user: Object
});
</script> 