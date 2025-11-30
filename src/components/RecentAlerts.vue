<template>
  <div class="bg-[#EDE8F5] shadow rounded-xl p-6">
    <!-- Scrollable Alert List -->
    <ul class="divide-y divide-[#ADBBD4] space-y-2">
      <!-- Loop Through Alerts -->
      <li 
        v-for="(alert, index) in alerts" 
        :key="index" 
        class="py-3 flex items-start justify-between group bg-white p-3 rounded-lg shadow-sm hover:shadow-md transition"
      >
        <!-- Left Side - Icon + Message -->
        <div class="flex items-start space-x-4">
          <!-- Alert Type Icon -->
          <span class="flex-shrink-0">
            <!-- Speed Alert -->
            <svg v-if="alert.type === 'speed'" class="h-6 w-6 text-yellow-500" viewBox="0 0 24 24" fill="none" stroke="currentColor">
              <path d="M13 10V3L4 14h7v7l9-11h-7z" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
            </svg>
            <!-- Drowsiness Alert -->
            <svg v-else-if="alert.type === 'drowsiness'" class="h-6 w-6 text-orange-500" viewBox="0 0 24 24" fill="none" stroke="currentColor">
              <path d="M15 12a3 3 0 11-6 0 3 3 0 016 0z" stroke-width="2"/>
              <path d="M2.458 12C3.732 7.943 7.523 5 12 5c4.478 0 8.268 2.943 9.542 7-1.274 4.057-5.064 7-9.542 7-4.477 0-8.268-2.943-9.542-7z" stroke-width="2"/>
              <path d="M12 9v6" stroke-width="2" stroke-linecap="round"/>
            </svg>
            <!-- Alcohol Alert -->
            <svg v-else-if="alert.type === 'alcohol'" class="h-6 w-6 text-red-600" viewBox="0 0 24 24" fill="none" stroke="currentColor">
              <path d="M9 2l1 6 1-6m2 0l1 6 1-6M6 8h12l-1.5 13.5a2 2 0 01-2 1.5h-5a2 2 0 01-2-1.5L6 8z" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
            </svg>
            <!-- Crash Alert -->
            <svg v-else-if="alert.type === 'crash'" class="h-6 w-6 text-red-700" viewBox="0 0 24 24" fill="none" stroke="currentColor">
              <path d="M12 9v2m0 4h.01m-6.938 4h13.856c1.54 0 2.502-1.667 1.732-3L13.732 4c-.77-1.333-2.694-1.333-3.464 0L3.34 16c-.77 1.333.192 3 1.732 3z" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
            </svg>
            <!-- Theft Alert -->
            <svg v-else-if="alert.type === 'theft'" class="h-6 w-6 text-orange-600" viewBox="0 0 24 24" fill="none" stroke="currentColor">
              <path d="M12 15v2m-6 4h12a2 2 0 002-2v-6a2 2 0 00-2-2H6a2 2 0 00-2 2v6a2 2 0 002 2zm10-10V7a4 4 0 00-8 0v4h8z" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
            </svg>
            <!-- Warning Alert -->
            <svg v-else-if="alert.type === 'warning'" class="h-6 w-6 text-yellow-600" viewBox="0 0 24 24" fill="none" stroke="currentColor">
              <path d="M12 9v2m0 4h.01m-6.938 4h13.856c1.54 0 2.502-1.667 1.732-3L13.732 4c-.77-1.333-2.694-1.333-3.464 0L3.34 16c-.77 1.333.192 3 1.732 3z" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
            </svg>
            <!-- Success Alert -->
            <svg v-else-if="alert.type === 'success'" class="h-6 w-6 text-green-500" viewBox="0 0 24 24" fill="none" stroke="currentColor">
              <path d="M9 12l2 2 4-4m6 2a9 9 0 11-18 0 9 9 0 0118 0z" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
            </svg>
            <!-- Info Alert -->
            <svg v-else-if="alert.type === 'info'" class="h-6 w-6 text-blue-500" viewBox="0 0 24 24" fill="none" stroke="currentColor">
              <path d="M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
            </svg>
            <!-- Default/Danger Alert -->
            <svg v-else class="h-6 w-6 text-red-500" viewBox="0 0 24 24" fill="none" stroke="currentColor">
              <circle cx="12" cy="12" r="10" stroke-width="2" />
              <path d="M12 8v4m0 4h.01" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
            </svg>
          </span>

          <!-- Alert Text -->
          <div class="flex-1">
            <p class="font-semibold text-[#3D52A0]">{{ alert.message }}</p>
            <p class="text-xs text-[#7091E6]">{{ alert.details }}</p>
            <p v-if="alert.time" class="text-xs text-gray-400 mt-1">{{ alert.time }}</p>
          </div>
        </div>

        <!-- Right Side - Delete Button -->
        <button 
          @click="$emit('delete', index)" 
          class="text-red-500 hover:text-red-700 opacity-0 group-hover:opacity-100 transition-opacity"
          title="Delete Alert"
        >
          <span class="material-icons">delete</span>
        </button>
      </li>
    </ul>
  </div>
</template>

<script setup>
// Define Props
const props = defineProps({
  alerts: {
    type: Array,
    required: true,
    default: () => []
  }
});

// Emit delete event
defineEmits(['delete']);
</script>

<style scoped>
/* Optional custom styles for better spacing */
.material-icons {
  font-size: 18px;
}
</style>