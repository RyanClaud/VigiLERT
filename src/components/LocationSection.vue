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

// Function to create a red marker icon
function createRedMarkerIcon() {
  return L.icon({
    iconUrl: 'https://unpkg.com/leaflet/dist/images/marker-icon-2x.png ',
    shadowUrl: 'https://unpkg.com/leaflet/dist/images/marker-shadow.png ',
    iconSize: [25, 41],
    shadowSize: [41, 41],
    iconAnchor: [12, 41],
    popupAnchor: [-3, -76],
    tooltipAnchor: [16, -44],
    className: 'marker-red'
  });
}

onMounted(() => {
  const mapElement = document.getElementById('location-section-map');
  if (!mapElement) {
    console.warn("Map container not found");
    return;
  }

  if (mapInstance) {
    mapInstance.setView([props.location.lat, props.location.lng], 13);
    return;
  }

  mapInstance = L.map(mapElement).setView([props.location.lat, props.location.lng], 13);

  L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; OpenStreetMap contributors'
  }).addTo(mapInstance);

  let marker = L.marker([props.location.lat, props.location.lng], { icon: createRedMarkerIcon() }).addTo(mapInstance);
  marker.bindPopup("Current Location").openPopup();

  watch(
    () => props.location,
    (newLoc) => {
      if (newLoc.lat && newLoc.lng) {
        mapInstance.setView([newLoc.lat, newLoc.lng], 13);
        marker.setLatLng([newLoc.lat, newLoc.lng]);
        marker.bindPopup(`Lat: ${newLoc.lat}, Lng: ${newLoc.lng}`).openPopup();
      }
    },
    { deep: true }
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

.marker-red {
  width: 25px;
  height: 41px;
  background-color: red;
  border-radius: 50%;
  display: flex;
  justify-content: center;
  align-items: center;
}

.marker-red .leaflet-marker-icon {
  filter: invert(1) hue-rotate(180deg); /* Invert colors and rotate hue to red */
}
</style>