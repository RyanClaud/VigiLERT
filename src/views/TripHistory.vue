<template>
  <div class="space-y-6 pb-12 font-['Inter',sans-serif]">
    
    <!-- Trip History Main Card -->
    <div class="bg-[#0B2540]/80 border border-[#00B85C]/20 shadow-2xl rounded-3xl p-6 sm:p-8 backdrop-blur-md relative overflow-hidden">
      <!-- Ambient Glow -->
      <div class="absolute -top-24 -right-24 w-96 h-96 bg-[#00B85C]/10 rounded-full blur-3xl pointer-events-none"></div>

      <!-- Header Section -->
      <div class="flex flex-col sm:flex-row items-start sm:items-center justify-between gap-4 mb-6 pb-6 border-b border-white/10">
        <div class="flex items-center gap-3.5">
          <div class="w-12 h-12 rounded-2xl bg-[#00B85C]/15 border border-[#00B85C]/30 flex items-center justify-center text-[#00B85C] shadow-md shadow-[#00B85C]/10">
            <span class="material-icons text-3xl">route</span>
          </div>
          <div>
            <h2 class="text-2xl font-black text-white tracking-tight">Trip History & Telemetry</h2>
            <p class="text-xs font-semibold text-slate-300 mt-0.5">
              Review recorded journeys, GPS telemetry, duration, and speed records
            </p>
          </div>
        </div>

        <div class="flex items-center gap-2">
          <span class="text-xs font-bold px-3 py-1 rounded-full bg-[#00B85C]/15 text-[#00B85C] border border-[#00B85C]/30">
            {{ filteredTrips.length }} of {{ allTrips.length }} {{ allTrips.length === 1 ? 'Route' : 'Routes' }}
          </span>
        </div>
      </div>

      <!-- ✅ DATE FILTER TOOLBAR -->
      <div class="mb-6 bg-[#061B2B]/75 border border-white/10 rounded-2xl p-4 shadow-lg space-y-3">
        <div class="flex items-center justify-between gap-2 flex-wrap">
          <div class="flex items-center gap-2 text-xs font-bold text-slate-300 uppercase tracking-wider">
            <span class="material-icons text-base text-[#00B85C]">filter_alt</span>
            <span>Filter Trips by Date:</span>
          </div>

          <!-- Active Filter Description Tag -->
          <span class="text-xs font-semibold text-[#00B85C] bg-[#00B85C]/10 px-2.5 py-0.5 rounded-full border border-[#00B85C]/20">
            {{ activeFilterLabel }}
          </span>
        </div>

        <!-- Filter Buttons List -->
        <div class="flex flex-wrap items-center gap-2">
          <button
            v-for="btn in filterButtons"
            :key="btn.key"
            @click="setFilter(btn.key)"
            :class="[
              'px-3.5 py-1.5 text-xs font-bold rounded-xl transition-all duration-200 flex items-center gap-1.5',
              selectedFilter === btn.key
                ? 'bg-gradient-to-r from-[#00B85C] to-[#087A45] text-white shadow-md shadow-[#00B85C]/25 border border-[#00B85C]'
                : 'bg-white/5 hover:bg-white/10 text-slate-300 border border-white/10 hover:border-white/20'
            ]"
          >
            <span class="material-icons text-xs">{{ btn.icon }}</span>
            <span>{{ btn.label }}</span>
          </button>
        </div>

        <!-- Custom Date Picker Field (shown when 'custom' is selected) -->
        <div v-if="selectedFilter === 'custom'" class="pt-2 flex flex-wrap items-center gap-3 border-t border-white/10">
          <div class="flex items-center gap-2">
            <label for="trip-custom-date" class="text-xs font-bold text-slate-300">Choose Specific Date:</label>
            <input
              id="trip-custom-date"
              type="date"
              v-model="customDate"
              class="bg-[#061B2B] text-white border border-[#00B85C]/50 rounded-xl px-3 py-1.5 text-xs focus:ring-2 focus:ring-[#00B85C] outline-none shadow-inner"
            />
          </div>

          <button
            v-if="customDate"
            @click="customDate = ''"
            class="text-[11px] font-semibold text-slate-400 hover:text-white underline"
          >
            Clear Date
          </button>
        </div>
      </div>

      <!-- ✅ TELEMETRY SUMMARY METRICS BAR -->
      <div v-if="filteredTrips.length > 0" class="grid grid-cols-2 lg:grid-cols-4 gap-3.5 mb-6">
        <!-- Routes Count -->
        <div class="bg-[#061B2B]/60 border border-white/10 rounded-2xl p-3.5 flex items-center gap-3">
          <div class="w-10 h-10 rounded-xl bg-blue-500/15 border border-blue-500/30 flex items-center justify-center text-blue-400">
            <span class="material-icons text-xl">route</span>
          </div>
          <div>
            <p class="text-[10px] font-bold text-slate-400 uppercase tracking-wider">Filtered Rides</p>
            <p class="text-lg font-black text-white">{{ filterStats.count }}</p>
          </div>
        </div>

        <!-- Total Distance -->
        <div class="bg-[#061B2B]/60 border border-white/10 rounded-2xl p-3.5 flex items-center gap-3">
          <div class="w-10 h-10 rounded-xl bg-[#00B85C]/15 border border-[#00B85C]/30 flex items-center justify-center text-[#00B85C]">
            <span class="material-icons text-xl">straighten</span>
          </div>
          <div>
            <p class="text-[10px] font-bold text-slate-400 uppercase tracking-wider">Total Distance</p>
            <p class="text-lg font-black text-white">{{ filterStats.distanceKm }}</p>
          </div>
        </div>

        <!-- Total Riding Time -->
        <div class="bg-[#061B2B]/60 border border-white/10 rounded-2xl p-3.5 flex items-center gap-3">
          <div class="w-10 h-10 rounded-xl bg-amber-500/15 border border-amber-500/30 flex items-center justify-center text-amber-400">
            <span class="material-icons text-xl">timer</span>
          </div>
          <div>
            <p class="text-[10px] font-bold text-slate-400 uppercase tracking-wider">Ride Duration</p>
            <p class="text-lg font-black text-white">{{ filterStats.durationFormatted }}</p>
          </div>
        </div>

        <!-- Top Speed -->
        <div class="bg-[#061B2B]/60 border border-white/10 rounded-2xl p-3.5 flex items-center gap-3">
          <div class="w-10 h-10 rounded-xl bg-purple-500/15 border border-purple-500/30 flex items-center justify-center text-purple-400">
            <span class="material-icons text-xl">speed</span>
          </div>
          <div>
            <p class="text-[10px] font-bold text-slate-400 uppercase tracking-wider">Peak Speed</p>
            <p class="text-lg font-black text-white">{{ filterStats.maxSpeedKmh }}</p>
          </div>
        </div>
      </div>

      <!-- Trip List -->
      <div v-if="filteredTrips.length > 0" class="space-y-3.5">
        <div
          v-for="trip in filteredTrips"
          :key="trip.id"
          class="bg-[#061B2B]/75 border border-white/10 hover:border-[#00B85C]/40 rounded-2xl p-5 shadow-lg transition-all duration-300 hover:scale-[1.01] group"
        >
          <div class="flex flex-col sm:flex-row justify-between items-start sm:items-center gap-4">
            
            <!-- Route Details -->
            <div class="space-y-2 flex-1 min-w-0">
              <div class="flex items-center gap-2 flex-wrap">
                <span class="w-2 h-2 rounded-full bg-[#00B85C] animate-pulse"></span>
                <p class="font-bold text-white text-sm uppercase tracking-wide">
                  {{ formatTripDate(trip) }}
                </p>
                <span v-if="trip.hasGPS === false || (!trip.startLat && !trip.from?.lat)" class="text-[10px] font-bold px-2 py-0.5 rounded bg-amber-500/15 text-amber-400 border border-amber-500/30">
                  Stationary / Bench
                </span>
                <span v-else class="text-[10px] font-bold px-2 py-0.5 rounded bg-[#00B85C]/15 text-[#00B85C] border border-[#00B85C]/30 flex items-center gap-1">
                  <span class="material-icons text-[11px]">gps_fixed</span>
                  GPS Tracked
                </span>
                <span v-if="trip.source === 'phone_gps' || trip.source === 'phone_auto_fallback'" class="text-[10px] font-semibold px-2 py-0.5 rounded bg-blue-500/15 text-blue-400 border border-blue-500/30">
                  Phone GPS
                </span>
              </div>

              <!-- From / To -->
              <div class="space-y-1 text-xs text-slate-300 pl-4 border-l-2 border-[#00B85C]/40">
                <div class="flex items-start gap-2">
                  <span class="material-icons text-xs text-[#00B85C] mt-0.5">trip_origin</span>
                  <span>
                    <strong class="text-slate-400">From:</strong> 
                    {{ trip.startLocationName || formatCoords(trip.startLat || trip.from?.lat, trip.startLng || trip.from?.lng) }}
                  </span>
                </div>
                <div class="flex items-start gap-2">
                  <span class="material-icons text-xs text-red-400 mt-0.5">location_on</span>
                  <span>
                    <strong class="text-slate-400">To:</strong> 
                    {{ trip.endLocationName || formatCoords(trip.endLat || trip.to?.lat, trip.endLng || trip.to?.lng) }}
                  </span>
                </div>
              </div>

              <!-- Metrics -->
              <div class="flex flex-wrap items-center gap-3 pt-1">
                <div class="inline-flex items-center gap-1.5 px-2.5 py-1 rounded-lg bg-white/5 border border-white/10 text-xs text-slate-300">
                  <span class="material-icons text-xs text-[#00B85C]">straighten</span>
                  <span>Distance: <strong class="text-white">{{ formatDistance(trip) }}</strong></span>
                </div>
                <div class="inline-flex items-center gap-1.5 px-2.5 py-1 rounded-lg bg-white/5 border border-white/10 text-xs text-slate-300">
                  <span class="material-icons text-xs text-blue-400">timer</span>
                  <span>Duration: <strong class="text-white">{{ formatDuration(trip.duration) }}</strong></span>
                </div>
                <div class="inline-flex items-center gap-1.5 px-2.5 py-1 rounded-lg bg-white/5 border border-white/10 text-xs text-slate-300">
                  <span class="material-icons text-xs text-amber-400">speed</span>
                  <span>Max Speed: <strong class="text-white">{{ Number(trip.maxSpeed || 0).toFixed(1) }} km/h</strong></span>
                </div>
                <div v-if="trip.avgSpeed" class="inline-flex items-center gap-1.5 px-2.5 py-1 rounded-lg bg-white/5 border border-white/10 text-xs text-slate-300">
                  <span class="material-icons text-xs text-teal-400">avg_pace</span>
                  <span>Avg: <strong class="text-white">{{ Number(trip.avgSpeed || 0).toFixed(1) }} km/h</strong></span>
                </div>
              </div>
            </div>

            <!-- Action Buttons -->
            <div class="flex sm:flex-col gap-2 w-full sm:w-auto flex-shrink-0">
              <!-- View FROM & TO Map Button -->
              <button
                @click="toggleTripMap(trip)"
                class="flex-1 sm:flex-initial inline-flex items-center justify-center gap-1.5 px-4 py-2 bg-gradient-to-r from-[#00B85C] to-[#087A45] hover:from-[#00c965] hover:to-[#0a8c4f] text-white text-xs font-bold rounded-xl shadow-md shadow-[#00B85C]/20 transition-all hover:scale-105"
              >
                <span class="material-icons text-sm">{{ selectedTripId === trip.id ? 'expand_less' : 'map' }}</span>
                <span>{{ selectedTripId === trip.id ? 'Hide Pins & Map' : 'View FROM & TO Pins' }}</span>
              </button>

              <!-- Open In Maps Button -->
              <a
                v-if="hasValidCoords(trip)"
                :href="getGoogleMapsLink(trip)"
                target="_blank"
                rel="noopener noreferrer"
                class="flex-1 sm:flex-initial inline-flex items-center justify-center gap-1.5 px-3 py-2 bg-[#061B2B] hover:bg-[#0B2540] text-slate-300 hover:text-white border border-white/10 rounded-xl text-xs font-semibold transition-all"
              >
                <span class="material-icons text-xs text-[#00B85C]">open_in_new</span>
                <span>Open in Maps</span>
              </a>

              <!-- Delete Button -->
              <button
                @click="deleteTrip(trip.id)"
                class="flex-1 sm:flex-initial inline-flex items-center justify-center gap-1.5 px-3 py-2 bg-red-500/10 hover:bg-red-500 text-red-400 hover:text-white border border-red-500/25 rounded-xl text-xs font-bold transition-all"
              >
                <span class="material-icons text-xs">delete</span>
                <span>Delete</span>
              </button>
            </div>

          </div>

          <!-- ============================================================== -->
          <!-- EXPANDED INTERACTIVE MAP WITH FROM AND TO PIN MARKERS           -->
          <!-- ============================================================== -->
          <div v-if="selectedTripId === trip.id" class="mt-4 pt-4 border-t border-white/10 space-y-3">
            
            <!-- Header Status Boxes: FROM & TO Breakdown -->
            <div class="grid grid-cols-1 md:grid-cols-2 gap-3">
              <!-- FROM Origin Box -->
              <div class="bg-[#061B2B]/95 border border-[#00B85C]/40 rounded-xl p-3.5 flex items-start gap-3 shadow-md">
                <div class="w-8 h-8 rounded-lg bg-[#00B85C]/20 border border-[#00B85C]/50 flex items-center justify-center text-[#00B85C] font-black text-xs flex-shrink-0">
                  FROM
                </div>
                <div class="min-w-0 flex-1">
                  <div class="flex items-center justify-between gap-2">
                    <p class="text-[10px] font-bold text-[#00B85C] uppercase tracking-wider">Start Origin Location</p>
                    <span class="text-[10px] font-mono text-slate-400">{{ formatTimeOnly(trip.startTime) }}</span>
                  </div>
                  <p class="text-xs text-white font-semibold mt-0.5 break-words">
                    {{ trip.startLocationName || formatCoords(trip.startLat || trip.from?.lat, trip.startLng || trip.from?.lng) }}
                  </p>
                  <p class="text-[10px] font-mono text-slate-400 mt-1">
                    📍 Coordinates: {{ formatLatLngExact(trip.startLat || trip.from?.lat, trip.startLng || trip.from?.lng) }}
                  </p>
                </div>
              </div>

              <!-- TO Destination Box -->
              <div class="bg-[#061B2B]/95 border border-red-500/40 rounded-xl p-3.5 flex items-start gap-3 shadow-md">
                <div class="w-8 h-8 rounded-lg bg-red-500/20 border border-red-500/50 flex items-center justify-center text-red-400 font-black text-xs flex-shrink-0">
                  TO
                </div>
                <div class="min-w-0 flex-1">
                  <div class="flex items-center justify-between gap-2">
                    <p class="text-[10px] font-bold text-red-400 uppercase tracking-wider">Last Recorded / Destination</p>
                    <span class="text-[10px] font-mono text-slate-400">{{ formatTimeOnly(trip.endTime || trip.timestamp) }}</span>
                  </div>
                  <p class="text-xs text-white font-semibold mt-0.5 break-words">
                    {{ trip.endLocationName || formatCoords(trip.endLat || trip.to?.lat, trip.endLng || trip.to?.lng) }}
                  </p>
                  <p class="text-[10px] font-mono text-slate-400 mt-1">
                    🏁 Coordinates: {{ formatLatLngExact(trip.endLat || trip.to?.lat, trip.endLng || trip.to?.lng) }}
                  </p>
                </div>
              </div>
            </div>

            <!-- Leaflet Interactive Map Container -->
            <div class="relative w-full h-80 sm:h-96 rounded-2xl overflow-hidden border border-[#00B85C]/30 shadow-2xl bg-[#061B2B]">
              <div :id="'map-trip-' + trip.id" class="w-full h-full"></div>

              <!-- Top Left Overlay Badge -->
              <div class="absolute top-3 left-3 z-[1000] bg-[#061B2B]/90 backdrop-blur-md px-3 py-1.5 rounded-xl border border-white/10 shadow-lg flex items-center gap-2">
                <span class="w-2.5 h-2.5 rounded-full bg-[#00B85C] animate-pulse"></span>
                <span class="text-xs font-bold text-white">Interactive Trip Route & Markers</span>
              </div>
            </div>

          </div>
        </div>
      </div>

      <!-- Empty State: Filter Returned 0 Results -->
      <div v-else-if="allTrips.length > 0" class="bg-[#061B2B]/40 border border-white/5 rounded-2xl p-10 text-center">
        <span class="material-icons text-5xl text-amber-400/40 mb-3 block">event_busy</span>
        <p class="text-white font-bold text-base">No Trips Found for Selected Date</p>
        <p class="text-xs text-slate-400 mt-1 max-w-sm mx-auto">
          No journeys recorded matching "{{ activeFilterLabel }}". You can switch dates or view your complete history.
        </p>
        <button
          @click="setFilter('all')"
          class="mt-4 px-4 py-2 bg-gradient-to-r from-[#00B85C] to-[#087A45] text-white text-xs font-bold rounded-xl shadow-md hover:scale-105 transition-all inline-flex items-center gap-1.5"
        >
          <span class="material-icons text-sm">all_inclusive</span>
          <span>View All Time Trips</span>
        </button>
      </div>

      <!-- Empty State: No Trips at All in Database -->
      <div v-else class="bg-[#061B2B]/40 border border-white/5 rounded-2xl p-10 text-center">
        <span class="material-icons text-5xl text-white/20 mb-3 block">explore_off</span>
        <p class="text-white font-bold text-base">No Trip Records Found</p>
        <p class="text-xs text-slate-400 mt-1 max-w-sm mx-auto">
          Start riding with your paired VigiLERT motorcycle module or phone GPS. Journeys will be recorded with GPS coordinates and logged here automatically.
        </p>
      </div>

    </div>

  </div>
</template>

<script setup>
import { ref, computed, watch, onBeforeUnmount, nextTick } from 'vue';
import { useAuthStore } from '../stores/auth';
import { database } from '../firebase/config';
import { ref as dbRef, onValue, remove } from 'firebase/database';
import L from 'leaflet';
import 'leaflet/dist/leaflet.css';

const authStore = useAuthStore();
const allTrips = ref([]);
let tripsUnsubscribe = null;
let altTripsUnsubscribe = null;

// Selected Trip for Map Inspection (FROM & TO Pins)
const selectedTripId = ref(null);
let activeMapInstance = null;

// Date filtering states
const selectedFilter = ref('all'); // 'all', 'today', 'yesterday', 'week', 'month', 'custom'
const customDate = ref(''); // YYYY-MM-DD

const filterButtons = [
  { key: 'all', label: 'All Time', icon: 'all_inclusive' },
  { key: 'today', label: 'Today', icon: 'today' },
  { key: 'yesterday', label: 'Yesterday', icon: 'history' },
  { key: 'week', label: 'Past 7 Days', icon: 'date_range' },
  { key: 'month', label: 'Past 30 Days', icon: 'calendar_month' },
  { key: 'custom', label: 'Custom Date', icon: 'edit_calendar' }
];

const setFilter = (key) => {
  selectedFilter.value = key;
  if (key === 'custom' && !customDate.value) {
    // Default to today's date formatted as YYYY-MM-DD
    const today = new Date();
    const y = today.getFullYear();
    const m = String(today.getMonth() + 1).padStart(2, '0');
    const d = String(today.getDate()).padStart(2, '0');
    customDate.value = `${y}-${m}-${d}`;
  }
};

const activeFilterLabel = computed(() => {
  switch (selectedFilter.value) {
    case 'today':
      return 'Today';
    case 'yesterday':
      return 'Yesterday';
    case 'week':
      return 'Past 7 Days';
    case 'month':
      return 'Past 30 Days';
    case 'custom':
      return customDate.value ? `Date: ${customDate.value}` : 'Custom Date (Select)';
    case 'all':
    default:
      return 'All Recorded Trips';
  }
});

// Resolve user UID with robust fallbacks
const currentUid = computed(() => {
  return authStore.user?.uid || localStorage.getItem('vigilert_user_uid') || 'DMIARybyNHVhzwAmrx2QQjdpSb13';
});

// Filtered trips computation
const filteredTrips = computed(() => {
  const now = new Date();
  const startOfToday = new Date(now.getFullYear(), now.getMonth(), now.getDate()).getTime();
  const startOfYesterday = startOfToday - 86400000;
  const startOfWeek = now.getTime() - (7 * 86400000);
  const startOfMonth = now.getTime() - (30 * 86400000);

  return allTrips.value.filter(trip => {
    const rawTime = Number(trip.timestamp || trip.endTime || trip.startTime || trip.id || 0);
    if (!rawTime) return selectedFilter.value === 'all';

    const tripDate = new Date(rawTime);

    switch (selectedFilter.value) {
      case 'today':
        return rawTime >= startOfToday;
      case 'yesterday':
        return rawTime >= startOfYesterday && rawTime < startOfToday;
      case 'week':
        return rawTime >= startOfWeek;
      case 'month':
        return rawTime >= startOfMonth;
      case 'custom': {
        if (!customDate.value) return true;
        const [yr, mo, dy] = customDate.value.split('-').map(Number);
        return (
          tripDate.getFullYear() === yr &&
          tripDate.getMonth() + 1 === mo &&
          tripDate.getDate() === dy
        );
      }
      case 'all':
      default:
        return true;
    }
  });
});

// Aggregated statistics for the filtered trips
const filterStats = computed(() => {
  const list = filteredTrips.value;
  let totalDist = 0;
  let totalDur = 0;
  let maxSpd = 0;

  for (const t of list) {
    totalDist += parseFloat(t.distance || t.totalDistance || 0) || 0;
    totalDur += Number(t.duration || 0) || 0;
    const spd = parseFloat(t.maxSpeed || 0) || 0;
    if (spd > maxSpd) maxSpd = spd;
  }

  return {
    count: list.length,
    distanceKm: totalDist >= 1 ? `${totalDist.toFixed(2)} km` : `${(totalDist * 1000).toFixed(0)} m`,
    durationFormatted: formatDuration(totalDur),
    maxSpeedKmh: `${maxSpd.toFixed(1)} km/h`
  };
});

// Format coordinates
const formatCoords = (lat, lng) => {
  const numLat = parseFloat(lat);
  const numLng = parseFloat(lng);
  if (!isNaN(numLat) && !isNaN(numLng) && (numLat !== 0 || numLng !== 0)) {
    return `${numLat.toFixed(5)}, ${numLng.toFixed(5)}`;
  }
  return 'Base Station / No GPS Fix';
};

const formatTimeOnly = (rawTime) => {
  if (!rawTime) return 'N/A';
  const d = new Date(Number(rawTime));
  return isNaN(d.getTime()) ? 'N/A' : d.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit', second: '2-digit' });
};

const formatLatLngExact = (lat, lng) => {
  const numLat = parseFloat(lat);
  const numLng = parseFloat(lng);
  if (!isNaN(numLat) && !isNaN(numLng) && (numLat !== 0 || numLng !== 0)) {
    return `${numLat.toFixed(6)}, ${numLng.toFixed(6)}`;
  }
  return 'No GPS Coordinates';
};

// Custom Leaflet Icons for FROM (Start/Origin) and TO (Destination/Last Location)
const createFromIcon = () => {
  return L.divIcon({
    html: `
      <div style="position:relative; display:flex; flex-direction:column; align-items:center; filter:drop-shadow(0 4px 8px rgba(0,0,0,0.5)); cursor:pointer;">
        <div style="background:#00B85C; color:#ffffff; font-size:10px; font-weight:900; padding:2px 8px; border-radius:999px; text-transform:uppercase; letter-spacing:0.5px; border:2px solid #ffffff; white-space:nowrap; margin-bottom:-4px; z-index:3; box-shadow:0 2px 6px rgba(0,0,0,0.35);">
          FROM
        </div>
        <div style="width:34px; height:34px; border-radius:50% 50% 50% 0; transform:rotate(-45deg); background:#00B85C; border:2.5px solid #ffffff; display:flex; align-items:center; justify-content:center; z-index:2;">
          <span style="transform:rotate(45deg); color:#ffffff; font-size:16px; font-weight:bold; line-height:1;">A</span>
        </div>
        <div style="width:12px; height:5px; background:rgba(0,0,0,0.3); border-radius:50%; margin-top:2px;"></div>
      </div>
    `,
    className: 'trip-from-marker',
    iconSize: [46, 52],
    iconAnchor: [23, 50],
    popupAnchor: [0, -50]
  });
};

const createToIcon = () => {
  return L.divIcon({
    html: `
      <div style="position:relative; display:flex; flex-direction:column; align-items:center; filter:drop-shadow(0 4px 8px rgba(0,0,0,0.5)); cursor:pointer;">
        <div style="background:#DC2626; color:#ffffff; font-size:10px; font-weight:900; padding:2px 8px; border-radius:999px; text-transform:uppercase; letter-spacing:0.5px; border:2px solid #ffffff; white-space:nowrap; margin-bottom:-4px; z-index:3; box-shadow:0 2px 6px rgba(0,0,0,0.35);">
          TO
        </div>
        <div style="width:34px; height:34px; border-radius:50% 50% 50% 0; transform:rotate(-45deg); background:#DC2626; border:2.5px solid #ffffff; display:flex; align-items:center; justify-content:center; z-index:2;">
          <span style="transform:rotate(45deg); color:#ffffff; font-size:16px; font-weight:bold; line-height:1;">B</span>
        </div>
        <div style="width:12px; height:5px; background:rgba(0,0,0,0.3); border-radius:50%; margin-top:2px;"></div>
      </div>
    `,
    className: 'trip-to-marker',
    iconSize: [46, 52],
    iconAnchor: [23, 50],
    popupAnchor: [0, -50]
  });
};

// Toggle Interactive Leaflet Map for a specific selected trip
const toggleTripMap = async (trip) => {
  if (selectedTripId.value === trip.id) {
    if (activeMapInstance) {
      activeMapInstance.remove();
      activeMapInstance = null;
    }
    selectedTripId.value = null;
    return;
  }

  if (activeMapInstance) {
    activeMapInstance.remove();
    activeMapInstance = null;
  }

  selectedTripId.value = trip.id;
  await nextTick();

  setTimeout(() => {
    initTripMap(trip);
  }, 100);
};

// Initialize Leaflet Map with FROM and TO Pin Markers
const initTripMap = (trip) => {
  const containerId = 'map-trip-' + trip.id;
  const container = document.getElementById(containerId);
  if (!container) return;

  const startLat = parseFloat(trip.startLat || trip.from?.lat || 0);
  const startLng = parseFloat(trip.startLng || trip.from?.lng || 0);
  const endLat = parseFloat(trip.endLat || trip.to?.lat || startLat);
  const endLng = parseFloat(trip.endLng || trip.to?.lng || startLng);

  const hasStart = !isNaN(startLat) && !isNaN(startLng) && (startLat !== 0 || startLng !== 0);
  const hasEnd = !isNaN(endLat) && !isNaN(endLng) && (endLat !== 0 || endLng !== 0);

  const defaultCenter = [12.774763, 121.471025];
  const centerLat = hasStart ? startLat : (hasEnd ? endLat : defaultCenter[0]);
  const centerLng = hasStart ? startLng : (hasEnd ? endLng : defaultCenter[1]);

  const map = L.map(containerId, {
    center: [centerLat, centerLng],
    zoom: 14,
    zoomControl: true,
    attributionControl: true
  });
  activeMapInstance = map;

  L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a>',
    maxZoom: 19
  }).addTo(map);

  const bounds = [];

  // 🟢 FROM Pin Marker
  if (hasStart) {
    const fromMarker = L.marker([startLat, startLng], { icon: createFromIcon() }).addTo(map);
    fromMarker.bindPopup(`
      <div style="font-family:Inter,sans-serif; min-width:210px;">
        <h4 style="margin:0 0 6px 0; color:#00B85C; font-weight:800; font-size:13px; display:flex; align-items:center; gap:6px;">
          <span>🟢</span> <span>FROM (Trip Origin)</span>
        </h4>
        <p style="margin:3px 0; font-size:12px; color:#1e293b;"><strong>Address:</strong><br>${trip.startLocationName || 'Start Position'}</p>
        <p style="margin:3px 0; font-size:11px; color:#64748b;"><strong>Coordinates:</strong> ${startLat.toFixed(6)}, ${startLng.toFixed(6)}</p>
        <p style="margin:3px 0; font-size:11px; color:#64748b;"><strong>Start Time:</strong> ${formatTimeOnly(trip.startTime)}</p>
      </div>
    `);
    bounds.push([startLat, startLng]);
  }

  // 🔴 TO Pin Marker
  if (hasEnd) {
    const toMarker = L.marker([endLat, endLng], { icon: createToIcon() }).addTo(map);
    toMarker.bindPopup(`
      <div style="font-family:Inter,sans-serif; min-width:210px;">
        <h4 style="margin:0 0 6px 0; color:#DC2626; font-weight:800; font-size:13px; display:flex; align-items:center; gap:6px;">
          <span>🔴</span> <span>TO (Last Location)</span>
        </h4>
        <p style="margin:3px 0; font-size:12px; color:#1e293b;"><strong>Address:</strong><br>${trip.endLocationName || 'Last Location'}</p>
        <p style="margin:3px 0; font-size:11px; color:#64748b;"><strong>Coordinates:</strong> ${endLat.toFixed(6)}, ${endLng.toFixed(6)}</p>
        <p style="margin:3px 0; font-size:11px; color:#64748b;"><strong>Recorded At:</strong> ${formatTimeOnly(trip.endTime || trip.timestamp)}</p>
      </div>
    `);
    bounds.push([endLat, endLng]);
  }

  // Draw Route Polyline
  if (trip.routeCoords && Array.isArray(trip.routeCoords) && trip.routeCoords.length > 1) {
    const polylineCoords = trip.routeCoords.map(pt => [pt.lat || pt[0], pt.lng || pt[1]]);
    L.polyline(polylineCoords, {
      color: '#00B85C',
      weight: 5,
      opacity: 0.85
    }).addTo(map);
    polylineCoords.forEach(c => bounds.push(c));
  } else if (hasStart && hasEnd && (startLat !== endLat || startLng !== endLng)) {
    // Draw direct styled connection line between FROM and TO
    L.polyline([[startLat, startLng], [endLat, endLng]], {
      color: '#00B85C',
      weight: 4,
      dashArray: '6, 8',
      opacity: 0.85
    }).addTo(map);
  }

  // Adjust camera to fit markers
  if (bounds.length > 1 && (startLat !== endLat || startLng !== endLng)) {
    map.fitBounds(L.latLngBounds(bounds), { padding: [50, 50], maxZoom: 16 });
  } else {
    map.setView([centerLat, centerLng], 15);
  }

  // Invalidate map size to prevent tile clipping
  setTimeout(() => {
    map.invalidateSize();
  }, 250);
};

// Reverse Geocode Helper for dynamic address resolution
const addressMap = ref({});
const resolveAddressForTrip = async (lat, lng, callback) => {
  const numLat = parseFloat(lat);
  const numLng = parseFloat(lng);
  if (isNaN(numLat) || isNaN(numLng) || (numLat === 0 && numLng === 0)) return;
  const key = `${numLat.toFixed(4)},${numLng.toFixed(4)}`;
  if (addressMap.value[key]) {
    callback(addressMap.value[key]);
    return;
  }
  try {
    const res = await fetch(`https://nominatim.openstreetmap.org/reverse?lat=${numLat}&lon=${numLng}&format=json&addressdetails=1`, {
      headers: { 'User-Agent': 'VigiLERT Motorcycle Safety System' }
    });
    if (!res.ok) return;
    const data = await res.json();
    const addr = data.address || {};
    let parts = [];
    if (addr.road) parts.push(addr.road);
    if (addr.suburb) parts.push(addr.suburb);
    if (addr.city) parts.push(addr.city);
    else if (addr.town) parts.push(addr.town);
    else if (addr.municipality) parts.push(addr.municipality);
    const formatted = parts.join(', ') || data.display_name?.split(',').slice(0, 3).join(',') || `${numLat.toFixed(5)}, ${numLng.toFixed(5)}`;
    addressMap.value[key] = formatted;
    callback(formatted);
  } catch (e) {
    // fallback
  }
};

const hasValidCoords = (trip) => {
  const lat = parseFloat(trip.startLat || trip.from?.lat || 0);
  const lng = parseFloat(trip.startLng || trip.from?.lng || 0);
  return lat !== 0 && lng !== 0;
};

const formatDistance = (trip) => {
  const dist = parseFloat(trip.distance || trip.totalDistance || 0);
  if (dist >= 1) return `${dist.toFixed(2)} km`;
  return `${(dist * 1000).toFixed(0)} m`;
};

const formatDuration = (durationMs) => {
  if (!durationMs) return '0 min';
  const totalSeconds = Math.round(Number(durationMs) / 1000);
  const mins = Math.floor(totalSeconds / 60);
  const secs = totalSeconds % 60;
  if (mins === 0) return `${secs}s`;
  if (mins < 60) return `${mins}m ${secs}s`;
  const hours = Math.floor(mins / 60);
  return `${hours}h ${mins % 60}m`;
};

const formatTripDate = (trip) => {
  const rawTime = trip.timestamp || trip.endTime || trip.startTime || (trip.id ? parseInt(trip.id) : null);
  if (rawTime) {
    const d = new Date(Number(rawTime));
    if (!isNaN(d.getTime())) {
      return d.toLocaleDateString(undefined, {
        month: 'short',
        day: 'numeric',
        year: 'numeric',
        hour: '2-digit',
        minute: '2-digit'
      });
    }
  }
  return 'Recorded Journey';
};

// Helper: Generate Google Maps Link
const getGoogleMapsLink = (trip) => {
  const startLat = trip.startLat || trip.from?.lat;
  const startLng = trip.startLng || trip.from?.lng;
  const endLat = trip.endLat || trip.to?.lat || startLat;
  const endLng = trip.endLng || trip.to?.lng || startLng;
  return `https://www.google.com/maps/dir/?api=1&origin=${startLat},${startLng}&destination=${endLat},${endLng}`;
};

// Map store to hold merged trips from both paths
const tripsMap = new Map();

const mergeAndSortTrips = () => {
  const list = Array.from(tripsMap.values());
  // Sort descending by timestamp or start time
  list.sort((a, b) => {
    const aTime = Number(a.timestamp || a.endTime || a.startTime || a.id || 0);
    const bTime = Number(b.timestamp || b.endTime || b.startTime || b.id || 0);
    return bTime - aTime;
  });
  allTrips.value = list;

  // Background address resolution for trips with missing/raw coordinate labels
  list.forEach(t => {
    const sLat = t.startLat || t.from?.lat;
    const sLng = t.startLng || t.from?.lng;
    if (sLat && sLng && (!t.startLocationName || t.startLocationName.includes(',') || t.startLocationName === 'Stationary Base')) {
      resolveAddressForTrip(sLat, sLng, (name) => {
        t.startLocationName = name;
      });
    }
    const eLat = t.endLat || t.to?.lat;
    const eLng = t.endLng || t.to?.lng;
    if (eLat && eLng && (!t.endLocationName || t.endLocationName.includes(',') || t.endLocationName === 'Stationary Base')) {
      resolveAddressForTrip(eLat, eLng, (name) => {
        t.endLocationName = name;
      });
    }
  });
};

// Setup trips listeners (listens to both `helmet_public/${uid}/trips` and `${uid}/trips`)
const setupTripsListener = (uid) => {
  if (tripsUnsubscribe) {
    tripsUnsubscribe();
    tripsUnsubscribe = null;
  }
  if (altTripsUnsubscribe) {
    altTripsUnsubscribe();
    altTripsUnsubscribe = null;
  }
  tripsMap.clear();
  allTrips.value = [];

  if (!uid) return;

  console.log('[TRIP HISTORY] Listening for trips under UID:', uid);
  const primaryTripsRef = dbRef(database, `helmet_public/${uid}/trips`);
  const altTripsRef = dbRef(database, `${uid}/trips`);

  // Primary path listener
  tripsUnsubscribe = onValue(primaryTripsRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      Object.entries(data).forEach(([id, trip]) => {
        tripsMap.set(id, { id, ...trip });
      });
    }
    mergeAndSortTrips();
  }, (err) => {
    console.error('[TRIP HISTORY] Error loading primary trips:', err);
  });

  // Alternative path listener (legacy or hardware direct)
  altTripsUnsubscribe = onValue(altTripsRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      Object.entries(data).forEach(([id, trip]) => {
        if (!tripsMap.has(id)) {
          tripsMap.set(id, { id, ...trip });
        }
      });
      mergeAndSortTrips();
    }
  }, (err) => {
    // Non-fatal, just debug log
    console.debug('[TRIP HISTORY] Alt path not available:', err);
  });
};

// Reactive watcher on user authentication
watch(currentUid, (newUid) => {
  if (newUid) setupTripsListener(newUid);
}, { immediate: true });

onBeforeUnmount(() => {
  if (activeMapInstance) {
    activeMapInstance.remove();
    activeMapInstance = null;
  }
  if (tripsUnsubscribe) tripsUnsubscribe();
  if (altTripsUnsubscribe) altTripsUnsubscribe();
});

// Helper: Delete Trip
const deleteTrip = async (tripId) => {
  if (!confirm('Are you sure you want to delete this trip record?')) return;
  const uid = currentUid.value;
  try {
    const primaryTripRef = dbRef(database, `helmet_public/${uid}/trips/${tripId}`);
    await remove(primaryTripRef);
    const altTripRef = dbRef(database, `${uid}/trips/${tripId}`);
    await remove(altTripRef).catch(() => {});
    tripsMap.delete(tripId);
    mergeAndSortTrips();
  } catch (err) {
    console.error('Failed to delete trip:', err);
  }
};
</script>
