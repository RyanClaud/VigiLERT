<template>
  <div class="leaflet-container">
    <div id="location-section-map" style="height: 400px; width: 100%;"></div>

    <div class="mt-4 text-sm">
      <p><strong>Current Location:</strong> {{ formatLatLng(location.lat, location.lng) }}</p>
    </div>
  </div>
</template>

<script setup>
import { ref, watch, onMounted, onBeforeUnmount } from 'vue';
import L from 'leaflet';
import 'leaflet/dist/leaflet.css';

const props = defineProps({
  location: {
    type: Object,
    required: true,
    default: () => ({ lat: null, lng: null })
  }
});

function formatLatLng(lat, lng) {
  return lat && lng ? `${Number(lat).toFixed(6)}, ${Number(lng).toFixed(6)}` : 'N/A';
}

let mapInstance = null;

onMounted(() => {
  if (!props.location.lat || !props.location.lng) {
    console.warn("No valid coordinates to display map");
    return;
  }

  mapInstance = L.map('location-section-map').setView([props.location.lat, props.location.lng], 13);

  L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; OpenStreetMap contributors'
  }).addTo(mapInstance);

  let marker = L.marker([props.location.lat, props.location.lng]).addTo(mapInstance);
  marker.bindPopup("Current Location").openPopup();

  // Watch for changes and update map
  watch(
    () => props.location,
    (newLoc) => {
      if (newLoc.lat && newLoc.lng) {
        mapInstance.setView([newLoc.lat, newLoc.lng], 13);
        marker.setLatLng([newLoc.lat, newLoc.lng]);
        marker.bindPopup(`Lat: ${newLoc.lat}, Lng: ${newLoc.lng}`).openPopup();
      }
    }
  );
});

onBeforeUnmount(() => {
  if (mapInstance) {
    mapInstance.remove();
    mapInstance = null;
  }
});
</script>

<style scoped>
.leaflet-container {
  height: 400px;
  width: 100%;
}
</style>