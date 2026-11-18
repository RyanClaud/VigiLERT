<template>
  <div class="space-y-4">
    <!-- GPS Source Selector -->
    <div class="bg-gradient-to-r from-blue-500/10 to-purple-500/10 rounded-2xl p-4 border border-blue-500/20">
      <div class="flex flex-col gap-4">
        <!-- Header -->
        <div class="flex items-center gap-3">
          <div class="bg-blue-500 p-2 rounded-xl">
            <span class="material-icons text-white text-2xl">settings_input_antenna</span>
          </div>
          <div>
            <h4 class="text-sm font-bold text-gray-800">GPS Source</h4>
            <p class="text-xs text-gray-500">Select location data source</p>
          </div>
        </div>
        
        <!-- Buttons - Responsive Grid (2 Sources: Module vs Phone) -->
        <div class="grid grid-cols-1 sm:grid-cols-2 gap-3">
          <button
            @click="setGPSSource('module')"
            :class="[
              'px-4 py-3 rounded-xl text-sm font-semibold transition-all duration-300 flex items-center justify-center gap-2',
              gpsSource === 'module' 
                ? 'bg-blue-600 text-white shadow-lg shadow-blue-500/20' 
                : 'bg-white text-gray-600 hover:bg-gray-100'
            ]"
          >
            <span class="material-icons text-lg">router</span>
            <span>🏍️ GPS Module</span>
          </button>
          <button
            @click="setGPSSource('phone')"
            :class="[
              'px-4 py-3 rounded-xl text-sm font-semibold transition-all duration-300 flex items-center justify-center gap-2',
              gpsSource === 'phone' 
                ? 'bg-emerald-600 text-white shadow-lg shadow-emerald-500/20' 
                : 'bg-white text-gray-600 hover:bg-gray-100'
            ]"
          >
            <span class="material-icons text-lg">smartphone</span>
            <span>📱 Phone GPS</span>
          </button>
        </div>

        <!-- Hardware GPS Fix Status Banner (when GPS Module is active) -->
        <div v-if="gpsSource === 'module'" class="text-xs rounded-xl p-3.5 border transition-all shadow-md"
          :class="hasModuleFix ? 'bg-emerald-500/15 border-emerald-500/40 text-emerald-300' : 'bg-amber-500/15 border-amber-500/40 text-amber-300'"
        >
          <div class="flex items-center gap-2 font-bold">
            <span class="material-icons text-base">{{ hasModuleFix ? 'satellite_alt' : 'sync' }}</span>
            <span>{{ hasModuleFix ? 'Hardware GPS: 3D Satellite Fix Active' : 'Auto-Fallback: Tracking via Phone GPS (Hardware GPS Searching)' }}</span>
          </div>
          <p class="mt-1 text-[11px] text-slate-200">
            {{ hasModuleFix 
                ? 'Receiving high-accuracy satellite coordinates directly from the motorcycle module.' 
                : 'Motorcycle GPS has no satellite lock (indoors or bench test). The system automatically uses Phone GPS so your real-time position and trips are never lost.' }}
          </p>
        </div>
      </div>
    </div>

    <!-- Location Info Card -->
    <div class="bg-gradient-to-r from-[#7091E6]/10 to-[#3D52A0]/10 rounded-2xl p-4 md:p-6 border border-[#7091E6]/20">
      <div class="flex flex-col md:flex-row items-start gap-4">
        <!-- Icon and Info -->
        <div class="flex items-start gap-3 flex-1 w-full">
          <div class="bg-gradient-to-br from-[#7091E6] to-[#3D52A0] p-3 rounded-xl flex-shrink-0">
            <span class="material-icons text-white text-3xl">my_location</span>
          </div>
          <div class="flex-1 min-w-0">
            <h3 class="text-lg font-bold text-[#3D52A0] mb-2">Current Location</h3>
            <!-- ✅ Show address instead of coordinates -->
            <div class="flex items-start gap-2 text-sm text-gray-700 mb-2">
              <span class="material-icons text-sm flex-shrink-0 mt-0.5">place</span>
              <span class="font-medium break-words">{{ currentAddress }}</span>
            </div>
            <div class="flex items-center gap-2 text-xs text-gray-500">
              <span class="material-icons text-xs">my_location</span>
              <span class="font-mono">{{ formatLatLng(location.lat, location.lng) }}</span>
            </div>
            <div class="flex items-center gap-2 mt-2">
              <span class="w-2 h-2 rounded-full animate-pulse flex-shrink-0"
                :class="activeLocationSource === 'module' ? 'bg-blue-500' : 'bg-emerald-500'"></span>
              <span class="text-xs text-gray-600 font-medium">
                Source: {{ activeLocationLabel }}
              </span>
            </div>
          </div>
        </div>
        
        <!-- Action Buttons -->
        <div class="flex flex-col sm:flex-row md:flex-col gap-2 w-full md:w-auto">
          <button
            @click="getUserLocation"
            :disabled="isGettingLocation"
            class="inline-flex items-center justify-center gap-2 px-4 py-3 bg-gradient-to-r from-green-500 to-green-600 text-white text-sm font-semibold rounded-xl hover:from-green-600 hover:to-green-700 transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105 disabled:opacity-50 disabled:cursor-not-allowed whitespace-nowrap"
          >
            <span class="material-icons text-lg" :class="{ 'animate-spin': isGettingLocation }">
              {{ isGettingLocation ? 'refresh' : 'gps_fixed' }}
            </span>
            <span>{{ isGettingLocation ? 'Getting...' : 'Get My Location' }}</span>
          </button>
          <a 
            v-if="location.lat && location.lng"
            :href="`https://www.google.com/maps?q=${location.lat},${location.lng}`" 
            target="_blank"
            class="inline-flex items-center justify-center gap-2 px-4 py-3 bg-gradient-to-r from-[#7091E6] to-[#5571c6] text-white text-sm font-semibold rounded-xl hover:from-[#3D52A0] hover:to-[#2a3a70] transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105 whitespace-nowrap"
          >
            <span class="material-icons text-lg">open_in_new</span>
            <span>Open in Maps</span>
          </a>
        </div>
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
import { ref, computed, watch, onMounted, onBeforeUnmount, nextTick } from 'vue';
import L from 'leaflet';
import 'leaflet/dist/leaflet.css';
import { useAuthStore } from '../stores/auth';

const authStore = useAuthStore();

const props = defineProps({
  location: {
    type: Object,
    required: true,
    default: () => ({ lat: null, lng: null })
  },
  user: {
    type: Object,
    default: () => ({})
  },
  crashEvents: {
    type: Array,
    default: () => []
  }
});

const emit = defineEmits(['update-location']);

// ✅ NEW: Address cache to avoid repeated API calls
const addressCache = ref(new Map());
const currentAddress = ref('Loading address...');

function formatLatLng(lat, lng) {
  return lat && lng ? `${Number(lat).toFixed(6)}, ${Number(lng).toFixed(6)}` : 'N/A';
}

// ✅ NEW: Reverse Geocoding Function
async function reverseGeocode(lat, lng) {
  if (!lat || !lng) return null;
  
  // Check cache first
  const cacheKey = `${lat.toFixed(4)},${lng.toFixed(4)}`;
  if (addressCache.value.has(cacheKey)) {
    return addressCache.value.get(cacheKey);
  }
  
  try {
    // Use OpenStreetMap Nominatim API (free, no key needed)
    const url = `https://nominatim.openstreetmap.org/reverse?lat=${lat}&lon=${lng}&format=json&addressdetails=1`;
    const response = await fetch(url, {
      headers: {
        'User-Agent': 'VigiLERT Motorcycle Safety System'
      }
    });
    
    if (!response.ok) throw new Error('Geocoding failed');
    
    const data = await response.json();
    
    // Format address nicely
    const address = data.address;
    let formattedAddress = '';
    
    if (address.road) formattedAddress += address.road;
    if (address.suburb) formattedAddress += (formattedAddress ? ', ' : '') + address.suburb;
    if (address.city) formattedAddress += (formattedAddress ? ', ' : '') + address.city;
    else if (address.town) formattedAddress += (formattedAddress ? ', ' : '') + address.town;
    else if (address.municipality) formattedAddress += (formattedAddress ? ', ' : '') + address.municipality;
    
    if (!formattedAddress) formattedAddress = data.display_name;
    
    // Cache the result
    addressCache.value.set(cacheKey, formattedAddress);
    
    console.log('[Geocoding] Address found:', formattedAddress);
    return formattedAddress;
  } catch (error) {
    console.error('[Geocoding] Error:', error);
    return `${lat.toFixed(6)}, ${lng.toFixed(6)}`;
  }
}

let mapInstance = null;
let marker = null;
let crashMarkers = [];
const isMapReady = ref(false);
const isGettingLocation = ref(false);
const gpsSource = ref('module'); // 'module' or 'phone'
let phoneLocation = ref({ lat: null, lng: null });
let moduleLocation = ref({ lat: null, lng: null });
let phoneWatchId = null;

// Check if hardware GPS module currently has valid non-zero coordinates
const hasModuleFix = computed(() => {
  const lat = Number(props.location?.lat);
  const lng = Number(props.location?.lng);
  return !isNaN(lat) && !isNaN(lng) && lat !== 0 && lng !== 0;
});

// Currently active tracking source
const activeLocationSource = computed(() => {
  if (gpsSource.value === 'phone') return 'phone';
  return hasModuleFix.value ? 'module' : 'phone_auto_fallback';
});

const activeLocationLabel = computed(() => {
  if (gpsSource.value === 'phone') return '📱 Phone GPS (Manual)';
  if (hasModuleFix.value) return '🏍️ GPS Module (3D Lock)';
  return '🔄 Phone GPS (Auto-Fallback Active)';
});

// Start continuous phone location watch (powers automatic fallback)
const startPhoneLocationWatch = () => {
  if (!navigator.geolocation || phoneWatchId !== null) return;
  
  phoneWatchId = navigator.geolocation.watchPosition(
    (position) => {
      const lat = position.coords.latitude;
      const lng = position.coords.longitude;
      phoneLocation.value = { lat, lng };
      
      // Auto-fallback: if motorcycle GPS module has no fix, use phone location!
      if (!hasModuleFix.value || gpsSource.value === 'phone') {
        emit('update-location', { 
          lat, 
          lng, 
          source: (gpsSource.value === 'phone' ? 'phone' : 'phone_auto_fallback') 
        });
        updateLocationBySource();
      }
    },
    (err) => {
      console.warn('[LocationSection] Geolocation watch error:', err);
    },
    { enableHighAccuracy: true, maximumAge: 3000, timeout: 10000 }
  );
};

// Set GPS source
const setGPSSource = (source) => {
  gpsSource.value = source;
  console.log('[LocationSection] GPS source changed to:', source);
  
  if (source === 'phone' && (!phoneLocation.value.lat || !phoneLocation.value.lng)) {
    getUserLocation();
  } else {
    updateLocationBySource();
  }
};

// ✅ ENHANCED: Update location with address and dynamic icon
const updateLocationBySource = async () => {
  let newLocation = { lat: null, lng: null };
  
  if (gpsSource.value === 'module') {
    if (hasModuleFix.value) {
      newLocation = { lat: Number(props.location.lat), lng: Number(props.location.lng) };
    } else if (phoneLocation.value.lat && phoneLocation.value.lng) {
      // Auto-switch: if hardware has no fix, seamlessly track via phone GPS
      newLocation = { ...phoneLocation.value };
    }
  } else if (gpsSource.value === 'phone') {
    newLocation = { ...phoneLocation.value };
  }
  
  // Update map if location changed
  if (mapInstance && marker && newLocation.lat && newLocation.lng) {
    mapInstance.setView([newLocation.lat, newLocation.lng], 15);
    marker.setLatLng([newLocation.lat, newLocation.lng]);
    
    // ✅ Update marker icon based on GPS source
    marker.setIcon(createLocationMarkerIcon());
    
    // ✅ Get address and update popup
    currentAddress.value = 'Loading address...';
    const address = await reverseGeocode(newLocation.lat, newLocation.lng);
    if (address) {
      currentAddress.value = address;
    }
    
    const sourceLabel = activeLocationLabel.value;
    
    marker.bindPopup(`
      <div style="min-width: 200px;">
        <h3 style="font-weight: bold; color: #3D52A0; margin-bottom: 8px;">📍 Current Location</h3>
        <p style="margin: 4px 0; font-size: 13px;"><strong>Address:</strong><br>${address || 'Loading...'}</p>
        <p style="margin: 4px 0; font-size: 11px; color: #666;"><strong>Coordinates:</strong> ${newLocation.lat.toFixed(6)}, ${newLocation.lng.toFixed(6)}</p>
        <p style="margin: 4px 0; font-size: 12px;"><strong>Source:</strong> ${sourceLabel}</p>
      </div>
    `);
  }
};

// Function to get user's current location
const getUserLocation = () => {
  if (!navigator.geolocation) {
    alert('Geolocation is not supported by your browser');
    return;
  }

  isGettingLocation.value = true;

  navigator.geolocation.getCurrentPosition(
    (position) => {
      const lat = position.coords.latitude;
      const lng = position.coords.longitude;
      
      // Store phone location
      phoneLocation.value = { lat, lng };
      
      // Emit the new location to parent component
      emit('update-location', { lat, lng, source: 'phone' });
      
      // Update location based on current source
      updateLocationBySource();
      
      // Update map immediately
      if (mapInstance && marker) {
        const displayLat = gpsSource.value === 'module' ? props.location.lat : lat;
        const displayLng = gpsSource.value === 'module' ? props.location.lng : lng;
        mapInstance.setView([displayLat, displayLng], 15);
        marker.setLatLng([displayLat, displayLng]);
        marker.bindPopup(`<b>Your Current Location</b><br>Lat: ${displayLat.toFixed(6)}<br>Lng: ${displayLng.toFixed(6)}<br>Source: ${gpsSource.value}`).openPopup();
      }
      
      isGettingLocation.value = false;
    },
    (error) => {
      console.error('Error getting location:', error);
      let errorMessage = 'Unable to get your location. ';
      
      switch(error.code) {
        case error.PERMISSION_DENIED:
          errorMessage += 'Please allow location access in your browser settings.';
          break;
        case error.POSITION_UNAVAILABLE:
          errorMessage += 'Location information is unavailable.';
          break;
        case error.TIMEOUT:
          errorMessage += 'Location request timed out.';
          break;
        default:
          errorMessage += 'An unknown error occurred.';
      }
      
      alert(errorMessage);
      isGettingLocation.value = false;
    },
    {
      enableHighAccuracy: true,
      timeout: 10000,
      maximumAge: 0
    }
  );
};

// ✅ NEW: Dynamic marker icon based on GPS source and User Profile Picture
function createLocationMarkerIcon() {
  const userPhoto = authStore.photoURL || authStore.user?.photoURL || props.user?.photoURL;
  const isModule = (gpsSource.value === 'module');
  const themeColor = isModule ? '#2563eb' : '#00B85C';
  const badgeEmoji = isModule ? '🏍️' : '📱';

  if (userPhoto) {
    return L.divIcon({
      html: `
        <div style="position:relative; width:48px; height:48px; display:flex; align-items:center; justify-content:center;">
          <!-- Ambient pulse ring -->
          <div style="position:absolute; width:46px; height:46px; border-radius:50%; background:${themeColor}; opacity:0.35; animation:ping 2s cubic-bezier(0,0,0.2,1) infinite;"></div>
          <!-- Pointed Pin Head -->
          <div style="position:relative; width:40px; height:40px; border-radius:50% 50% 50% 0; transform:rotate(-45deg); background:${themeColor}; border:2.5px solid #ffffff; box-shadow:0 4px 14px rgba(0,0,0,0.45); display:flex; align-items:center; justify-content:center; z-index:2;">
            <!-- Circular photo rotated back upright -->
            <div style="transform:rotate(45deg); width:30px; height:30px; border-radius:50%; overflow:hidden; background:#061B2B; display:flex; align-items:center; justify-content:center; border:1px solid rgba(255,255,255,0.4);">
              <img src="${userPhoto}" style="width:100%; height:100%; object-fit:cover; display:block;" alt="Rider" />
            </div>
          </div>
          <!-- Source mini badge -->
          <div style="position:absolute; bottom:0px; right:0px; width:18px; height:18px; border-radius:50%; background:#061B2B; border:1.5px solid white; display:flex; align-items:center; justify-content:center; font-size:10px; z-index:3; box-shadow:0 2px 5px rgba(0,0,0,0.4);">
            ${badgeEmoji}
          </div>
        </div>
      `,
      className: 'user-avatar-marker',
      iconSize: [48, 48],
      iconAnchor: [24, 46],
      popupAnchor: [0, -46]
    });
  }

  // Fallback if no photo is uploaded
  const iconHtml = isModule ? '🏍️' : '📱';
  return L.divIcon({
    html: `<div style="background-color: ${themeColor}; width: 40px; height: 40px; border-radius: 50% 50% 50% 0; transform: rotate(-45deg); display: flex; align-items: center; justify-content: center; border: 3px solid white; box-shadow: 0 4px 12px rgba(0,0,0,0.3);"><span style="transform: rotate(45deg); font-size: 24px;">${iconHtml}</span></div>`,
    className: 'location-marker',
    iconSize: [40, 40],
    iconAnchor: [20, 40],
    popupAnchor: [0, -40]
  });
}

// Watch for profile photo updates to refresh marker icon dynamically
watch(
  () => authStore.photoURL || authStore.user?.photoURL || props.user?.photoURL,
  () => {
    if (marker && isMapReady.value) {
      marker.setIcon(createLocationMarkerIcon());
    }
  }
);

// Function to create crash marker icon
function createCrashMarkerIcon() {
  return L.divIcon({
    html: '<div style="background-color: #dc2626; width: 32px; height: 32px; border-radius: 50%; display: flex; align-items: center; justify-content: center; border: 3px solid white; box-shadow: 0 2px 8px rgba(0,0,0,0.3);"><span style="color: white; font-size: 20px; font-weight: bold;">⚠</span></div>',
    className: 'crash-marker',
    iconSize: [32, 32],
    iconAnchor: [16, 32],
    popupAnchor: [0, -32]
  });
}

// ✅ ENHANCED: Add crash markers with addresses
async function addCrashMarkers() {
  if (!mapInstance || !isMapReady.value) return;
  
  // Clear existing crash markers
  crashMarkers.forEach(m => m.remove());
  crashMarkers = [];
  
  console.log('[LocationSection] Adding crash markers:', props.crashEvents.length);
  
  // Add new crash markers with geocoding
  for (const crash of props.crashEvents) {
    if (crash.lat && crash.lng && crash.hasGPS) {
      const crashMarker = L.marker([crash.lat, crash.lng], {
        icon: createCrashMarkerIcon()
      }).addTo(mapInstance);
      
      // Initial popup with loading address
      let popupContent = `
        <div style="min-width: 250px;">
          <h3 style="font-weight: bold; color: #dc2626; margin-bottom: 8px; display: flex; align-items: center; gap: 8px;">
            <span style="font-size: 24px;">⚠️</span>
            <span>Crash Detected</span>
          </h3>
          <div style="background: #fee2e2; padding: 8px; border-radius: 8px; margin-bottom: 8px;">
            <p style="margin: 0; font-size: 13px; color: #991b1b;">
              <strong>📍 Location:</strong> <span id="crash-address-${crash.timestamp}">Loading address...</span>
            </p>
          </div>
          <p style="margin: 4px 0; font-size: 13px;"><strong>💥 Impact:</strong> ${crash.impactStrength || 'N/A'} g</p>
          <p style="margin: 4px 0; font-size: 13px;"><strong>🕐 Time:</strong> ${new Date(crash.timestamp).toLocaleString()}</p>
          <p style="margin: 4px 0; font-size: 11px; color: #666;"><strong>Coordinates:</strong> ${crash.lat.toFixed(6)}, ${crash.lng.toFixed(6)}</p>
          <a href="https://www.google.com/maps?q=${crash.lat},${crash.lng}" target="_blank" 
             style="display: inline-block; margin-top: 8px; padding: 6px 12px; background: #3b82f6; color: white; text-decoration: none; border-radius: 6px; font-size: 12px; font-weight: bold;">
            Open in Google Maps →
          </a>
        </div>
      `;
      
      crashMarker.bindPopup(popupContent);
      crashMarkers.push(crashMarker);
      
      // Geocode address asynchronously (with delay to respect rate limits)
      setTimeout(async () => {
        const address = await reverseGeocode(crash.lat, crash.lng);
        if (address) {
          // Update popup content with address
          const updatedPopup = popupContent.replace(
            `<span id="crash-address-${crash.timestamp}">Loading address...</span>`,
            `<span id="crash-address-${crash.timestamp}">${address}</span>`
          );
          crashMarker.setPopupContent(updatedPopup);
        }
      }, crashMarkers.length * 1100); // Stagger requests by 1.1 seconds
    }
  }
  
  console.log('[LocationSection] Added crash markers:', crashMarkers.length);
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

      // ✅ Add marker with dynamic icon
      marker = L.marker([lat, lng], { 
        icon: createLocationMarkerIcon() 
      }).addTo(mapInstance);
      
      marker.bindPopup(`<b>📍 Current Location</b><br>Loading address...`);

      isMapReady.value = true;
      
      // ✅ Fetch address for current location
      reverseGeocode(lat, lng).then(address => {
        if (address) {
          currentAddress.value = address;
          const sourceLabel = gpsSource.value === 'module' 
            ? (hasModuleFix.value ? '🏍️ GPS Module' : '🏍️ GPS Module (Waiting for Fix)')
            : '📱 Phone GPS';
          marker.setPopupContent(`
            <div style="min-width: 200px;">
              <h3 style="font-weight: bold; color: #3D52A0; margin-bottom: 8px;">📍 Current Location</h3>
              <p style="margin: 4px 0; font-size: 13px;"><strong>Address:</strong><br>${address}</p>
              <p style="margin: 4px 0; font-size: 11px; color: #666;"><strong>Coordinates:</strong> ${lat.toFixed(6)}, ${lng.toFixed(6)}</p>
              <p style="margin: 4px 0; font-size: 12px;"><strong>Source:</strong> ${sourceLabel}</p>
            </div>
          `);
        }
      });
      
      // Add crash markers after map is ready
      setTimeout(() => {
        addCrashMarkers();
      }, 200);
    }, 100);
  } catch (error) {
    console.error("Error initializing map:", error);
  }
};

onMounted(() => {
  initMap();
});

// Watch for location changes from module
watch(
  () => props.location,
  (newLoc) => {
    if (mapInstance && marker && newLoc.lat && newLoc.lng && isMapReady.value) {
      try {
        // Store module location
        moduleLocation.value = { ...newLoc };
        
        // Update based on current source
        if (gpsSource.value === 'module') {
          updateLocationBySource();
        }
      } catch (error) {
        console.error("Error updating map:", error);
      }
    }
  },
  { deep: true }
);

// Watch for crash events changes
watch(
  () => props.crashEvents,
  () => {
    console.log('[LocationSection] Crash events updated:', props.crashEvents.length);
    addCrashMarkers();
  },
  { deep: true }
);

onBeforeUnmount(() => {
  // Clear crash markers
  crashMarkers.forEach(m => {
    try {
      m.remove();
    } catch (error) {
      console.error("Error removing crash marker:", error);
    }
  });
  crashMarkers = [];
  
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