<template>
  <div class="space-y-6">
    <div class="bg-white shadow px-4 py-5 sm:rounded-lg sm:p-6">
      <div class="md:grid md:grid-cols-3 md:gap-6">
        <div class="md:col-span-1">
          <h3 class="text-lg font-medium leading-6 text-gray-900">Trip History</h3>
          <p class="mt-1 text-sm text-gray-500">
            View your past trips and their details.
          </p>
        </div>
        <div class="mt-5 md:mt-0 md:col-span-2">
          <!-- Filters -->
          <div class="flex space-x-4 mb-4">
            <div class="flex-1">
              <label for="date-range" class="block text-sm font-medium text-gray-700">Date Range</label>
              <select
                id="date-range"
                v-model="selectedDateRange"
                class="mt-1 block w-full pl-3 pr-10 py-2 text-base border-gray-300 focus:outline-none focus:ring-blue-500 focus:border-blue-500 sm:text-sm rounded-md"
              >
                <option value="7">Last 7 days</option>
                <option value="30">Last 30 days</option>
                <option value="90">Last 90 days</option>
                <option value="all">All time</option>
              </select>
            </div>
          </div>

          <!-- Trip List -->
          <div class="bg-[#EDE8F5] shadow overflow-hidden sm:rounded-md">
            <ul role="list" class="divide-y divide-[#ADBBD4]">
              <li v-for="trip in filteredTrips" :key="trip.id">
                <div class="px-4 py-4 sm:px-6">
                  <div class="flex items-center justify-between">
                    <div class="flex items-center">
                      <div class="flex-shrink-0">
                        <svg class="h-6 w-6 text-[#7091E6]" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                          <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z" />
                        </svg>
                      </div>
                      <div class="ml-4">
                        <h4 class="text-lg font-medium text-[#3D52A0]">
                          Trip on {{ formatDate(trip.startTime) }}
                        </h4>
                        <p class="text-sm text-[#7091E6]">
                          Duration: {{ formatDuration(trip.duration) }}
                        </p>
                      </div>
                    </div>
                    <div class="ml-2 flex-shrink-0 flex">
                      <span
                        :class="[
                          trip.status === 'completed' ? 'bg-green-100 text-green-800' : 'bg-yellow-100 text-yellow-800',
                          'px-2 inline-flex text-xs leading-5 font-semibold rounded-full'
                        ]"
                      >
                        {{ trip.status }}
                      </span>
                    </div>
                  </div>
                  <div class="mt-2 sm:flex sm:justify-between">
                    <div class="sm:flex">
                      <p class="flex items-center text-sm text-gray-500">
                        <svg class="flex-shrink-0 mr-1.5 h-5 w-5 text-gray-400" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                          <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M17.657 16.657L13.414 20.9a1.998 1.998 0 01-2.827 0l-4.244-4.243a8 8 0 1111.314 0z" />
                          <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 11a3 3 0 11-6 0 3 3 0 016 0z" />
                        </svg>
                        {{ trip.startLocation }} → {{ trip.endLocation }}
                      </p>
                    </div>
                    <div class="mt-2 flex items-center text-sm text-gray-500 sm:mt-0">
                      <svg class="flex-shrink-0 mr-1.5 h-5 w-5 text-gray-400" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M13 10V3L4 14h7v7l9-11h-7z" />
                      </svg>
                      <p>
                        Max Speed: {{ trip.maxSpeed }} km/h
                      </p>
                    </div>
                  </div>
                </div>
              </li>
            </ul>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue';
import { database } from '../firebase/config';
import { ref as dbRef, onValue } from 'firebase/database';

const trips = ref([]);
const selectedDateRange = ref('7');

const filteredTrips = computed(() => {
  const now = new Date();
  const days = parseInt(selectedDateRange.value);
  
  if (selectedDateRange.value === 'all') {
    return trips.value;
  }

  return trips.value.filter(trip => {
    const tripDate = new Date(trip.startTime);
    const diffTime = Math.abs(now - tripDate);
    const diffDays = Math.ceil(diffTime / (1000 * 60 * 60 * 24));
    return diffDays <= days;
  });
});

const formatDate = (timestamp) => {
  return new Date(timestamp).toLocaleDateString('en-US', {
    year: 'numeric',
    month: 'short',
    day: 'numeric',
    hour: '2-digit',
    minute: '2-digit'
  });
};

const formatDuration = (minutes) => {
  const hours = Math.floor(minutes / 60);
  const mins = minutes % 60;
  return `${hours}h ${mins}m`;
};

onMounted(() => {
  const tripsRef = dbRef(database, 'trips');
  onValue(tripsRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      trips.value = Object.entries(data).map(([id, trip]) => ({
        id,
        ...trip
      })).sort((a, b) => b.startTime - a.startTime);
    }
  });
});
</script> 