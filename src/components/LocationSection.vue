<template>
  <div class="space-y-4">
    <!-- Location Info Card -->
    <div class="bg-gradient-to-r from-[#7091E6]/10 to-[#3D52A0]/10 rounded-2xl p-6 border border-[#7091E6]/20">
      <div class="flex items-start gap-4">
        <div class="bg-gradient-to-br from-[#7091E6] to-[#3D52A0] p-3 rounded-xl">
          <span class="material-icons text-white text-3xl">my_location</span>
        </div>
        <div class="flex-1">
          <h3 class="text-lg font-bold text-[#3D52A0] mb-2">Current Location</h3>
          <div class="flex items-center gap-2 text-sm text-gray-600">
            <span class="material-icons text-sm">place</span>
            <span class="font-mono">{{ formatLatLng(location.lat, location.lng) }}</span>
          </div>
          <div class="flex items-center gap-2 mt-2">
            <span class="w-2 h-2 bg-green-500 rounded-full animate-pulse"></span>
            <span class="text-xs text-gray-500">Live tracking active</span>
          </div>
        </div>
        <a 
          v-if="location.lat && location.lng"
          :href="`https://www.google.com/maps?q=${location.lat},${location.lng}`" 
          target="_blank"
          class="inline-flex items-center gap-2 px-4 py-2 bg-gradient-to-r from-[#7091E6] to-[#5571c6] text-white text-sm font-semibold rounded-xl hover:from-[#3D52A0] hover:to-[#2a3a70] transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105"
        >
          <span class="material-icons text-lg">open_in_new</span>
          Open in Maps
        </a>
      </div>
    </div>

    <!-- Map Container -->
    <div class="relative rounded-2xl overflow-hidden shadow-2xl border-4 border-white/50">
      <!-- Loading State -->
      <div v-if="!isMapReady" class="absolute inset-0 flex items-center justify-center bg-gray-100 z-10">
        <div class="text-center">
          <div class="animate-spin rounded-full h-12 w-12 border-b-2 border-[#7091E6] mx-auto mb-3"></div>
          <p class="text-sm text-gray-600 font-medium">Loading map...</p>
        </div>
      </div>
      
      <div id="location-section-map" class="h-[450px] w-full"></div>
      
      <div v-if="isMapReady" class="absolute top-4 right-4 bg-white/90 backdrop-blur-sm px-4 py-2 rounded-xl shadow-lg">
        <div class="flex items-center gap-2">
          <span class="w-3 h-3 bg-red-500 rounded-full animate-pulse"></span>
          <span class="text-sm font-semibold text-gray-700">Your Position</span>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, watch, onMounted, onBeforeUnmount, nextTick } from 'vue';
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
let marker = null;
const isMapReady = ref(false);

// Function to create a red marker icon
function createRedMarkerIcon() {
  return L.icon({
    iconUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon-2x.png',
    shadowUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-shadow.png',
    iconSize: [25, 41],
    shadowSize: [41, 41],
    iconAnchor: [12, 41],
    popupAnchor: [1, -34],
    tooltipAnchor: [16, -28]
  });
}

// Initialize map asynchronously
const initMap = async () => {
  await nextTick();
  
  const mapElement = document.getElementById('location-section-map');
  if (!mapElement) {
    console.warn("Map container not found");
    return;
  }

  // Check if location is valid
  const lat = props.location.lat || 14.5995;
  const lng = props.location.lng || 120.9842;

  try {
    // Remove existing map if any
    if (mapInstance) {
      mapInstance.remove();
      mapInstance = null;
    }

    // Create map with timeout to prevent blocking
    setTimeout(() => {
      mapInstance = L.map(mapElement, {
        center: [lat, lng],
        zoom: 13,
        zoomControl: true,
        attributionControl: true
      });

      // Add tile layer
      L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a>',
        maxZoom: 19
      }).addTo(mapInstance);

      // Add marker
      marker = L.marker([lat, lng], { 
        icon: createRedMarkerIcon() 
      }).addTo(mapInstance);
      
      marker.bindPopup(`<b>Current Location</b><br>Lat: ${lat.toFixed(6)}<br>Lng: ${lng.toFixed(6)}`);

      isMapReady.value = true;
    }, 100);
  } catch (error) {
    console.error("Error initializing map:", error);
  }
};

onMounted(() => {
  initMap();
});

// Watch for location changes
watch(
  () => props.location,
  (newLoc) => {
    if (mapInstance && marker && newLoc.lat && newLoc.lng && isMapReady.value) {
      try {
        mapInstance.setView([newLoc.lat, newLoc.lng], 13);
        marker.setLatLng([newLoc.lat, newLoc.lng]);
        marker.bindPopup(`<b>Current Location</b><br>Lat: ${newLoc.lat.toFixed(6)}<br>Lng: ${newLoc.lng.toFixed(6)}`);
      } catch (error) {
        console.error("Error updating map:", error);
      }
    }
  },
  { deep: true }
);

onBeforeUnmount(() => {
  if (mapInstance) {
    try {
      mapInstance.remove();
    } catch (error) {
      console.error("Error removing map:", error);
    }
    mapInstance = null;
    marker = null;
  }
});
</script>

<style scoped>
/* Map styling */
#location-section-map {
  border-radius: 1rem;
}

/* Custom marker styling */
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
  filter: invert(1) hue-rotate(180deg);
}

/* Leaflet controls styling */
:deep(.leaflet-control-zoom) {
  border: none !important;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.15) !important;
  border-radius: 12px !important;
  overflow: hidden;
}

:deep(.leaflet-control-zoom a) {
  width: 36px !important;
  height: 36px !important;
  line-height: 36px !important;
  font-size: 20px !important;
  font-weight: bold !important;
  color: #3D52A0 !important;
  background: white !important;
  border: none !important;
  transition: all 0.3s ease !important;
}

:deep(.leaflet-control-zoom a:hover) {
  background: #7091E6 !important;
  color: white !important;
}

:deep(.leaflet-popup-content-wrapper) {
  border-radius: 12px !important;
  box-shadow: 0 4px 20px rgba(0, 0, 0, 0.15) !important;
}

:deep(.leaflet-popup-tip) {
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1) !important;
}
</style>