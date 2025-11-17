<template>
  <div class="fixed inset-0 bg-gradient-to-br from-[#EDE8F5] to-[#e8e4f0] flex items-center justify-center z-50">
    <div class="text-center">
      <div class="relative">
        <div class="w-20 h-20 border-4 border-[#7091E6]/30 border-t-[#7091E6] rounded-full animate-spin"></div>
        <div class="absolute inset-0 flex items-center justify-center">
          <span class="material-icons text-4xl text-[#7091E6] animate-pulse">security</span>
        </div>
      </div>
      <p class="mt-4 text-[#3D52A0] font-semibold">Loading...</p>
    </div>
  </div>
</template>

<script setup>
// Simple loading component
</script>

<style scoped>
@keyframes spin {
  to {
    transform: rotate(360deg);
  }
}
</style>
