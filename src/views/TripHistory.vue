<template>
  <div class="bg-[#EDE8F5] p-4 rounded-lg shadow-md">
    <h3 class="text-lg font-semibold text-gray-800 mb-4">Recent Trips</h3>

    <!-- Trip List -->
    <div v-if="recentTrips.length > 0" class="space-y-4">
      <div
        v-for="trip in recentTrips"
        :key="trip.id"
        class="bg-white p-4 rounded-md shadow-sm border-l-4 border-[#7091E6] hover:shadow transition-shadow"
      >
        <div class="flex justify-between items-start">
          <div>
            <p class="font-medium text-gray-800">Your past Routes</p>
            <p class="text-sm text-gray-500 mt-1">
              From: {{ trip.startLocationName || formatLatLng(trip.startLat, trip.startLng) }}
            </p>
            <p class="text-sm text-gray-500">
              To: {{ trip.endLocationName || formatLatLng(trip.endLat, trip.endLng) }}
            </p>
            <p class="text-sm mt-2">
              Max Speed:
              <span class="font-semibold text-[#3D52A0]">
                {{ trip.maxSpeed || 'N/A' }} km/h
              </span>
            </p>
          </div>

          <!-- Action Buttons -->
          <div class="flex flex-col space-y-2">
            <!-- Navigate Button -->
            <a
              :href="getGoogleMapsLink(trip)"
              target="_blank"
              rel="noopener noreferrer"
              class="px-3 py-1 bg-blue-600 text-white text-xs sm:text-sm font-medium rounded hover:bg-blue-700 transition duration-200"
            >
              Navigate
            </a>

            <!-- Delete Button -->
            <button
              @click="deleteTrip(trip.id)"
              class="px-3 py-1 bg-red-600 text-white text-xs sm:text-sm font-medium rounded hover:bg-red-700 transition duration-200"
            >
              Delete
            </button>
          </div>
        </div>
      </div>
    </div>

    <!-- Empty State -->
    <div v-else class="bg-white p-6 rounded-md shadow-sm text-center text-gray-500">
      No trips found yet.
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

      recentTrips.value = tripList.slice(0, 5);
    }
  });
});

// Helper: Generate Google Maps Link
const getGoogleMapsLink = (trip) => {
  const start = `${trip.startLat},${trip.startLng}`;
  const end = `${trip.endLat},${trip.endLng}`;
  return `https://www.google.com/maps/dir/?api=1&origin= ${start}&destination=${end}`;
};

// Helper: Delete Trip
const deleteTrip = async (tripId) => {
  const userId = authStore.user?.uid || 'MnzBjTBslZNijOkq732PE91hHa23';
  const tripRef = dbRef(database, `helmet_public/${userId}/trips/${tripId}`);
  await remove(tripRef);
};
</script>