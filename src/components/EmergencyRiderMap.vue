<template>
  <div class="space-y-4">

    <!-- Info Bar -->
    <div class="bg-gradient-to-r from-[#3D52A0]/10 to-[#7091E6]/10 rounded-2xl p-4 border border-[#7091E6]/20">
      <div class="flex flex-col md:flex-row items-start md:items-center justify-between gap-4">
        <!-- Location Info -->
        <div class="flex items-start gap-3 flex-1">
          <div class="bg-gradient-to-br from-[#7091E6] to-[#3D52A0] p-3 rounded-xl flex-shrink-0">
            <span class="material-icons text-white text-2xl">two_wheeler</span>
          </div>
          <div class="flex-1 min-w-0">
            <h3 class="text-base font-bold text-[#3D52A0] mb-1">Rider Location (GPS Module)</h3>
            <div class="flex items-start gap-2 text-sm text-gray-700 mb-1">
              <span class="material-icons text-sm flex-shrink-0 mt-0.5">place</span>
              <span class="font-medium break-words">{{ currentAddress }}</span>
            </div>
            <div class="flex items-center gap-2 text-xs text-gray-500">
              <span class="material-icons text-xs">my_location</span>
              <span class="font-mono">{{ formatLatLng(location.lat, location.lng) }}</span>
            </div>
            <div class="flex items-center gap-2 mt-1">
              <span :class="['w-2 h-2 rounded-full flex-shrink-0', location.lat ? 'bg-green-500 animate-pulse' : 'bg-red-400']"></span>
              <span class="text-xs text-gray-500">
                {{ location.lat ? 'GPS Fix Active — Live from motorcycle module' : 'Waiting for GPS fix...' }}
              </span>
            </div>
          </div>
        </div>

        <!-- Action Buttons -->
        <div class="flex gap-2 flex-shrink-0">
          <button @click="centerOnRider" :disabled="!location.lat"
            class="inline-flex items-center gap-2 px-4 py-2 bg-gradient-to-r from-[#3D52A0] to-[#7091E6] text-white text-sm font-semibold rounded-xl hover:opacity-90 transition-all shadow-lg disabled:opacity-40 disabled:cursor-not-allowed">
            <span class="material-icons text-sm">gps_fixed</span>
            Center
          </button>
          <a v-if="location.lat && location.lng"
            :href="`https://www.google.com/maps?q=${location.lat},${location.lng}`"
            target="_blank"
            class="inline-flex items-center gap-2 px-4 py-2 bg-gradient-to-r from-green-500 to-green-600 text-white text-sm font-semibold rounded-xl hover:opacity-90 transition-all shadow-lg">
            <span class="material-icons text-sm">open_in_new</span>
            Google Maps
          </a>
        </div>
      </div>
    </div>

    <!-- Speed + Satellite Info Strip -->
    <div class="grid grid-cols-3 gap-3">
      <div class="bg-white/80 backdrop-blur-sm rounded-xl p-3 border border-white/50 text-center shadow">
        <p class="text-xs text-gray-500 uppercase font-medium mb-1">Speed</p>
        <p :class="['text-xl font-black', isOverSpeed ? 'text-red-600' : 'text-[#3D52A0]']">{{ speed.toFixed(0) }}</p>
        <p class="text-xs text-gray-400">km/h</p>
      </div>
      <div class="bg-white/80 backdrop-blur-sm rounded-xl p-3 border border-white/50 text-center shadow">
        <p class="text-xs text-gray-500 uppercase font-medium mb-1">Satellites</p>
        <p class="text-xl font-black text-[#3D52A0]">{{ satellites }}</p>
        <p class="text-xs text-gray-400">connected</p>
      </div>
      <div class="bg-white/80 backdrop-blur-sm rounded-xl p-3 border border-white/50 text-center shadow">
        <p class="text-xs text-gray-500 uppercase font-medium mb-1">GPS Status</p>
        <p :class="['text-sm font-bold mt-1', location.lat ? 'text-green-600' : 'text-red-500']">
          {{ location.lat ? 'Active' : 'No Fix' }}
        </p>
        <p class="text-xs text-gray-400">{{ location.lat ? 'Live tracking' : 'Searching...' }}</p>
      </div>
    </div>

    <!-- Map Container -->
    <div class="relative rounded-2xl overflow-hidden shadow-2xl border-4 border-white/50">
      <!-- Loading overlay -->
      <div v-if="!isMapReady" class="absolute inset-0 flex items-center justify-center bg-gray-100 z-10">
        <div class="text-center">
          <div class="animate-spin rounded-full h-12 w-12 border-b-2 border-[#7091E6] mx-auto mb-3"></div>
          <p class="text-sm text-gray-600 font-medium">Loading map...</p>
        </div>
      </div>

      <!-- No GPS overlay -->
      <div v-if="isMapReady && !location.lat"
        class="absolute inset-0 flex items-center justify-center bg-black/30 z-10 backdrop-blur-sm">
        <div class="bg-white rounded-2xl p-6 text-center shadow-2xl max-w-xs mx-4">
          <span class="material-icons text-5xl text-gray-400 mb-3 block">gps_off</span>
          <p class="font-bold text-gray-700 mb-1">Waiting for GPS Fix</p>
          <p class="text-sm text-gray-500">The motorcycle GPS module is searching for satellites. Map will update automatically.</p>
        </div>
      </div>

      <div id="emergency-rider-map" class="h-[480px] w-full"></div>

      <!-- Live badge -->
      <div v-if="isMapReady && location.lat"
        class="absolute top-4 left-4 bg-white/90 backdrop-blur-sm px-3 py-2 rounded-xl shadow-lg flex items-center gap-2">
        <span class="w-2 h-2 bg-red-500 rounded-full animate-pulse"></span>
        <span class="text-xs font-bold text-gray-700">LIVE — GPS Module</span>
      </div>

      <!-- Last updated -->
      <div v-if="isMapReady && lastUpdated"
        class="absolute bottom-4 right-4 bg-white/90 backdrop-blur-sm px-3 py-2 rounded-xl shadow-lg">
        <p class="text-xs text-gray-500">Updated: {{ lastUpdated }}</p>
      </div>
    </div>

  </div>
</template>

<script setup>
import { ref, watch, onMounted, onBeforeUnmount, nextTick } from 'vue';
import L from 'leaflet';
import 'leaflet/dist/leaflet.css';

const props = defineProps({
  location:    { type: Object, default: () => ({ lat: null, lng: null }) },
  speed:       { type: Number, default: 0 },
  speedLimit:  { type: Number, default: 60 },
  satellites:  { type: Number, default: 0 },
  isOverSpeed: { type: Boolean, default: false }
});

// ── State ──────────────────────────────────────────────────────────────────
const isMapReady     = ref(false);
const currentAddress = ref('Fetching address...');
const lastUpdated    = ref('');
const addressCache   = new Map();

let mapInstance  = null;
let riderMarker  = null;
let pathLine     = null;
const pathCoords = [];

// ── Helpers ────────────────────────────────────────────────────────────────
function formatLatLng(lat, lng) {
  return lat && lng ? `${Number(lat).toFixed(6)}, ${Number(lng).toFixed(6)}` : 'No GPS Fix';
}

async function reverseGeocode(lat, lng) {
  if (!lat || !lng) return null;
  const key = `${lat.toFixed(4)},${lng.toFixed(4)}`;
  if (addressCache.has(key)) return addressCache.get(key);
  try {
    const res = await fetch(
      `https://nominatim.openstreetmap.org/reverse?lat=${lat}&lon=${lng}&format=json`,
      { headers: { 'User-Agent': 'VigiLERT-Emergency-Dashboard' } }
    );
    if (!res.ok) throw new Error('geocode failed');
    const data = await res.json();
    const a = data.address;
    let addr = [a.road, a.suburb, a.city || a.town || a.municipality]
      .filter(Boolean).join(', ') || data.display_name;
    addressCache.set(key, addr);
    return addr;
  } catch {
    return `${lat.toFixed(6)}, ${lng.toFixed(6)}`;
  }
}

function createRiderIcon() {
  return L.divIcon({
    html: `<div style="
      background: linear-gradient(135deg,#3D52A0,#7091E6);
      width:44px;height:44px;border-radius:50% 50% 50% 0;
      transform:rotate(-45deg);
      display:flex;align-items:center;justify-content:center;
      border:3px solid white;
      box-shadow:0 4px 16px rgba(61,82,160,0.5);">
      <span style="transform:rotate(45deg);font-size:22px;">🏍️</span>
    </div>`,
    className: '',
    iconSize: [44, 44],
    iconAnchor: [22, 44],
    popupAnchor: [0, -48]
  });
}

function buildPopup(lat, lng, address, speed) {
  return `
    <div style="min-width:220px;font-family:sans-serif;">
      <div style="display:flex;align-items:center;gap:8px;margin-bottom:10px;">
        <span style="font-size:24px;">🏍️</span>
        <strong style="color:#3D52A0;font-size:15px;">Rider Location</strong>
      </div>
      <div style="background:#EDE8F5;padding:8px;border-radius:8px;margin-bottom:8px;">
        <p style="margin:0;font-size:12px;color:#3D52A0;"><strong>📍 Address:</strong></p>
        <p style="margin:4px 0 0;font-size:12px;color:#444;">${address || 'Loading...'}</p>
      </div>
      <p style="margin:4px 0;font-size:12px;"><strong>🛰️ Coordinates:</strong> ${lat.toFixed(6)}, ${lng.toFixed(6)}</p>
      <p style="margin:4px 0;font-size:12px;"><strong>⚡ Speed:</strong> ${speed.toFixed(1)} km/h</p>
      <a href="https://www.google.com/maps?q=${lat},${lng}" target="_blank"
        style="display:inline-block;margin-top:8px;padding:6px 12px;background:#3D52A0;color:white;
               text-decoration:none;border-radius:8px;font-size:12px;font-weight:bold;">
        Open in Google Maps →
      </a>
    </div>`;
}

// ── Map Init ───────────────────────────────────────────────────────────────
const initMap = async () => {
  await nextTick();
  const el = document.getElementById('emergency-rider-map');
  if (!el) return;

  // Default to Philippines center if no GPS yet
  const lat = props.location.lat || 14.5995;
  const lng = props.location.lng || 120.9842;
  const zoom = props.location.lat ? 15 : 6;

  if (mapInstance) { mapInstance.remove(); mapInstance = null; }

  setTimeout(() => {
    mapInstance = L.map(el, { center: [lat, lng], zoom, zoomControl: true });

    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
      attribution: '&copy; OpenStreetMap contributors',
      maxZoom: 19
    }).addTo(mapInstance);

    // Rider marker
    riderMarker = L.marker([lat, lng], { icon: createRiderIcon() }).addTo(mapInstance);
    riderMarker.bindPopup(buildPopup(lat, lng, 'Loading...', props.speed));

    // Path polyline (trail)
    pathLine = L.polyline([], {
      color: '#7091E6', weight: 3, opacity: 0.7, dashArray: '6,4'
    }).addTo(mapInstance);

    isMapReady.value = true;

    // Fetch initial address
    if (props.location.lat) {
      reverseGeocode(lat, lng).then(addr => {
        currentAddress.value = addr || formatLatLng(lat, lng);
        riderMarker.setPopupContent(buildPopup(lat, lng, addr, props.speed));
      });
      lastUpdated.value = new Date().toLocaleTimeString();
    } else {
      currentAddress.value = 'Waiting for GPS fix...';
    }
  }, 100);
};

// ── Center on rider ────────────────────────────────────────────────────────
const centerOnRider = () => {
  if (mapInstance && props.location.lat) {
    mapInstance.setView([props.location.lat, props.location.lng], 16);
    riderMarker?.openPopup();
  }
};

// ── Watch location changes (real-time GPS updates) ─────────────────────────
watch(
  () => props.location,
  async (newLoc) => {
    if (!mapInstance || !riderMarker || !newLoc.lat || !newLoc.lng || !isMapReady.value) return;

    const { lat, lng } = newLoc;

    // Move marker
    riderMarker.setLatLng([lat, lng]);

    // Pan map smoothly
    mapInstance.panTo([lat, lng], { animate: true, duration: 0.8 });

    // Add to path trail (keep last 100 points)
    pathCoords.push([lat, lng]);
    if (pathCoords.length > 100) pathCoords.shift();
    pathLine.setLatLngs(pathCoords);

    // Update timestamp
    lastUpdated.value = new Date().toLocaleTimeString();

    // Reverse geocode (throttled via cache)
    const addr = await reverseGeocode(lat, lng);
    currentAddress.value = addr || formatLatLng(lat, lng);
    riderMarker.setPopupContent(buildPopup(lat, lng, addr, props.speed));
  },
  { deep: true }
);

// ── Lifecycle ──────────────────────────────────────────────────────────────
onMounted(() => initMap());

onBeforeUnmount(() => {
  if (mapInstance) { mapInstance.remove(); mapInstance = null; }
  riderMarker = null; pathLine = null;
});
</script>

<style scoped>
#emergency-rider-map { border-radius: 1rem; }

:deep(.leaflet-control-zoom) {
  border: none !important;
  box-shadow: 0 4px 12px rgba(0,0,0,.15) !important;
  border-radius: 12px !important;
  overflow: hidden;
}
:deep(.leaflet-control-zoom a) {
  width: 36px !important; height: 36px !important;
  line-height: 36px !important; font-size: 20px !important;
  color: #3D52A0 !important; background: white !important;
  border: none !important; transition: all .3s !important;
}
:deep(.leaflet-control-zoom a:hover) { background: #7091E6 !important; color: white !important; }
:deep(.leaflet-popup-content-wrapper) { border-radius: 12px !important; box-shadow: 0 4px 20px rgba(0,0,0,.15) !important; }
</style>
