<template>
  <div class="bg-[#EDE8F5] shadow rounded-xl p-6">
    <ul class="divide-y divide-[#ADBBD4]">
      <!-- Existing alerts -->
      <li 
        v-for="(alert, i) in combinedAlerts" 
        :key="i" 
        class="py-4 flex items-start space-x-4 justify-between group transition hover:bg-[#ADBBD4]/40 rounded-lg"
      >
        <div class="flex items-start space-x-4">
          <span>
            <!-- Icons -->
            <svg v-if="alert.type === 'speed'" class="h-6 w-6 text-yellow-500" viewBox="0 0 24 24" fill="none" stroke="currentColor">
              <circle cx="12" cy="12" r="10" stroke-width="2" />
              <path d="M12 8v4m0 4h.01" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
            </svg>
            <svg v-else-if="alert.type === 'drowsiness'" class="h-6 w-6 text-red-500" viewBox="0 0 24 24" fill="none" stroke="currentColor">
              <circle cx="12" cy="12" r="10" stroke-width="2" />
              <path d="M15 9l-6 6M9 9l6 6" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
            </svg>
            <svg v-else-if="alert.type === 'alcohol'" class="h-6 w-6 text-red-600" viewBox="0 0 24 24" fill="none" stroke="currentColor">
              <circle cx="12" cy="12" r="10" stroke-width="2" />
              <path d="M12 8v4m0 0v4m0-4h4m-4 0H8" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
            </svg>
            <svg v-else class="h-6 w-6 text-[#7091E6]" viewBox="0 0 24 24" fill="none" stroke="currentColor">
              <circle cx="12" cy="12" r="10" stroke-width="2" />
            </svg>
          </span>
          <div class="flex-1">
            <div class="font-semibold text-[#3D52A0]">{{ alert.message }}</div>
            <div class="text-xs text-[#7091E6]">{{ alert.details }}</div>
            <div v-if="alert.extra" class="text-xs text-red-600 font-medium mt-1">{{ alert.extra }}</div>
          </div>
        </div>

        <div class="flex flex-col items-end">
          <div class="text-xs text-[#ADBBD4] whitespace-nowrap">
            {{ formatDate(alert.timestamp) }}
          </div>
          <!-- Delete Button -->
          <button 
            @click="$emit('delete', i)"
            class="mt-1 opacity-0 group-hover:opacity-100 text-red-500 hover:text-red-700 transition-opacity"
            title="Delete Alert"
          >
            <span class="material-icons">delete</span>
          </button>
        </div>
      </li>
    </ul>
  </div>
</template>