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