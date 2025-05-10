<template>
  <section class="bg-[#EDE8F5] shadow rounded-2xl p-6 mb-6">
    <h4 class="font-semibold text-lg mb-2 text-[#3D52A0]">Current Location</h4>
    <p class="text-sm text-[#7091E6] mb-4">Real-time GPS tracking</p>

    <!-- Show map only when lat/lng are valid -->
    <div v-if="hasValidLocation" class="map-container">
      <l-map
        ref="mapRef"
        :zoom="15"
        :center="[location.lat, location.lng]"
        style="height: 350px; width: 100%; border-radius: 1rem;"
        class="shadow-lg"
      >
        <l-tile-layer url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png" />
        <l-marker :lat-lng="[location.lat, location.lng]" />
      </l-map>
    </div>

    <!-- Fallback message if no location is available -->
    <div v-else class="text-center py-8 bg-white rounded-xl shadow text-[#8697C4]">
      Waiting for valid GPS signal...
    </div>

    <!-- Rider Info Footer -->
    <div class="flex flex-col md:flex-row items-start md:items-center justify-between mt-4 gap-2">
      <div>
        <div class="font-medium text-[#3D52A0] text-xl">{{ user.name || 'Rider' }}</div>
        <div class="text-lg text-[#8697C4]">
          <span class="font-bold">Lat:</span> {{ location.lat ? location.lat.toFixed(6) : 'N/A' }},
          <span class="font-bold">Lng:</span> {{ location.lng ? location.lng.toFixed(6) : 'N/A' }}
        </div>
      </div>
      <button
        class="px-4 py-2 bg-[#7091E6] text-white rounded-lg shadow hover:bg-[#3D52A0] transition text-sm font-semibold"
        @click="navigateToMaps"
      >
        Navigate
      </button>
    </div>
  </section>
</template>

<script setup>
import { defineProps, computed, ref } from 'vue';
import 'leaflet/dist/leaflet.css';
import * as L from 'leaflet';
import { LMap, LTileLayer, LMarker } from '@vue-leaflet/vue-leaflet';

const props = defineProps({
  location: {
    type: Object,
    required: true,
    default: () => ({ lat: null, lng: null })
  },
  user: {
    type: Object,
    required: false,
    default: () => ({ name: 'Rider' })
  }
});

// Reference to the map component
const mapRef = ref(null);

// Check if location has valid coordinates
const hasValidLocation = computed(() => {
  return typeof props.location.lat === 'number' && typeof props.location.lng === 'number';
});

// Function to navigate within the Leaflet map
const navigateToMaps = () => {
  if (!hasValidLocation.value) return;

  // Get the map instance
  const map = mapRef.value.mapObject;

  // Center the map on the rider's location
  map.setView([props.location.lat, props.location.lng], 18); // Zoom level 18 for detailed view

  // Optionally, add a marker at the rider's location
  const marker = L.marker([props.location.lat, props.location.lng]).addTo(map);
  marker.bindPopup('You are here!').openPopup();

  // Optionally, you can add a tile layer for better visualization
  L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; <a href="https://www.openstreetmap.org/copyright ">OpenStreetMap</a> contributors'
  }).addTo(map);
};
</script>

<style scoped>
.map-container {
  overflow: hidden;
  border-radius: 1rem;
}
</style>git push -u origin main