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
              Review recorded GPS journeys, max speeds, and historical breadcrumbs
            </p>
          </div>
        </div>

        <div class="flex items-center gap-2">
          <span class="text-xs font-bold px-3 py-1 rounded-full bg-[#00B85C]/15 text-[#00B85C] border border-[#00B85C]/30">
            {{ recentTrips.length }} Recent Routes
          </span>
        </div>
      </div>

      <!-- Trip List -->
      <div v-if="recentTrips.length > 0" class="space-y-3.5">
        <div
          v-for="trip in recentTrips"
          :key="trip.id"
          class="bg-[#061B2B]/75 border border-white/10 hover:border-[#00B85C]/40 rounded-2xl p-5 shadow-lg transition-all duration-300 hover:scale-[1.01] group"
        >
          <div class="flex flex-col sm:flex-row justify-between items-start sm:items-center gap-4">
            
            <!-- Route Details -->
            <div class="space-y-2 flex-1 min-w-0">
              <div class="flex items-center gap-2">
                <span class="w-2 h-2 rounded-full bg-[#00B85C] animate-pulse"></span>
                <p class="font-bold text-white text-sm uppercase tracking-wide">Recorded Journey</p>
              </div>

              <!-- From / To -->
              <div class="space-y-1 text-xs text-slate-300 pl-4 border-l-2 border-[#00B85C]/40">
                <div class="flex items-start gap-2">
                  <span class="material-icons text-xs text-[#00B85C] mt-0.5">trip_origin</span>
                  <span><strong class="text-slate-400">From:</strong> {{ trip.startLocationName || formatLatLng(trip.startLat, trip.startLng) }}</span>
                </div>
                <div class="flex items-start gap-2">
                  <span class="material-icons text-xs text-red-400 mt-0.5">location_on</span>
                  <span><strong class="text-slate-400">To:</strong> {{ trip.endLocationName || formatLatLng(trip.endLat, trip.endLng) }}</span>
                </div>
              </div>

              <!-- Metrics -->
              <div class="flex flex-wrap items-center gap-3 pt-1">
                <div class="inline-flex items-center gap-1.5 px-2.5 py-1 rounded-lg bg-white/5 border border-white/10 text-xs text-slate-300">
                  <span class="material-icons text-xs text-[#00B85C]">speed</span>
                  <span>Max Speed: <strong class="text-white">{{ trip.maxSpeed || '0' }} km/h</strong></span>
                </div>
              </div>
            </div>

            <!-- Action Buttons -->
            <div class="flex sm:flex-col gap-2 w-full sm:w-auto flex-shrink-0">
              <!-- Navigate Button -->
              <a
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

      <!-- Empty State -->
      <div v-else class="bg-[#061B2B]/40 border border-white/5 rounded-2xl p-10 text-center">
        <span class="material-icons text-5xl text-white/20 mb-3 block">explore_off</span>
        <p class="text-white font-bold text-base">No Trip Records Found</p>
        <p class="text-xs text-slate-400 mt-1 max-w-sm mx-auto">
          Start riding with your paired VigiLERT motorcycle module. Journeys will be recorded with GPS coordinates and logged here automatically.
        </p>
      </div>

    </div>

  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue';
import { useAuthStore } from '../stores/auth';
import { database } from '../firebase/config';
import { ref as dbRef, onValue, remove } from 'firebase/database';

const authStore = useAuthStore();
const recentTrips = ref([]);

// Format lat/lng
const formatLatLng = (lat, lng) => {
  return lat && lng ? `${Number(lat).toFixed(6)}, ${Number(lng).toFixed(6)}` : 'N/A';
};

// Load recent trips from Firebase
onMounted(() => {
  const userId = authStore.user?.uid || 'MnzBjTBslZNijOkq732PE91hHa23';
  const tripsRef = dbRef(database, `helmet_public/${userId}/trips`);

  onValue(tripsRef, async (snapshot) => {
    const data = snapshot.val();
    if (data) {
      const tripList = Object.entries(data).map(([id, trip]) => ({
        id,
        ...trip
      }));

      // Sort by timestamp descending (latest first)
      tripList.sort((a, b) => {
        const aTime = typeof a.timestamp === 'string' ? parseInt(a.timestamp) : a.timestamp;
        const bTime = typeof b.timestamp === 'string' ? parseInt(b.timestamp) : b.timestamp;
        return bTime - aTime;
      });

      recentTrips.value = tripList.slice(0, 10);
    } else {
      recentTrips.value = [];
    }
  });
});

// Helper: Generate Google Maps Link
const getGoogleMapsLink = (trip) => {
  const start = `${trip.startLat},${trip.startLng}`;
  const end = `${trip.endLat},${trip.endLng}`;
  return `https://www.google.com/maps/dir/?api=1&origin=${start}&destination=${end}`;
};

// Helper: Delete Trip
const deleteTrip = async (tripId) => {
  const userId = authStore.user?.uid || 'MnzBjTBslZNijOkq732PE91hHa23';
  const tripRef = dbRef(database, `helmet_public/${userId}/trips/${tripId}`);
  await remove(tripRef);
};
</script>
