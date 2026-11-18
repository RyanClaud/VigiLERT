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
              <!-- Navigate Button -->
              <a
                v-if="hasValidCoords(trip)"
                :href="getGoogleMapsLink(trip)"
                target="_blank"
                rel="noopener noreferrer"
                class="flex-1 sm:flex-initial inline-flex items-center justify-center gap-1.5 px-4 py-2 bg-gradient-to-r from-[#00B85C] to-[#087A45] hover:from-[#00c965] hover:to-[#0a8c4f] text-white text-xs font-bold rounded-xl shadow-md shadow-[#00B85C]/20 transition-all hover:scale-105"
              >
                <span class="material-icons text-sm">open_in_new</span>
                <span>View Route</span>
              </a>

              <!-- Delete Button -->
              <button
                @click="deleteTrip(trip.id)"
                class="flex-1 sm:flex-initial inline-flex items-center justify-center gap-1.5 px-4 py-2 bg-red-500/10 hover:bg-red-500 text-red-400 hover:text-white border border-red-500/25 rounded-xl text-xs font-bold transition-all"
              >
                <span class="material-icons text-sm">delete</span>
                <span>Delete</span>
              </button>
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
import { ref, computed, watch, onBeforeUnmount } from 'vue';
import { useAuthStore } from '../stores/auth';
import { database } from '../firebase/config';
import { ref as dbRef, onValue, remove } from 'firebase/database';

const authStore = useAuthStore();
const allTrips = ref([]);
let tripsUnsubscribe = null;
let altTripsUnsubscribe = null;

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
  return authStore.user?.uid || localStorage.getItem('vigilert_user_uid') || 'DMIARybyNHVHzwAmrx2QQjdpSb13';
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
