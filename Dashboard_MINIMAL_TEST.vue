<template>
  <div class="min-h-screen flex flex-col bg-gradient-to-br from-[#EDE8F5] via-[#f5f3f7] to-[#e8e4f0] pb-20">
    <!-- User Greeting/Header -->
    <div class="flex flex-col items-center justify-center py-6 px-4">
      <div class="relative overflow-hidden">
        <div class="absolute inset-0 bg-gradient-to-r from-[#7091E6] via-[#5571c6] to-[#3D52A0] opacity-90 blur-xl"></div>
        <div class="relative flex items-center gap-4 bg-white/20 backdrop-blur-md border border-white/30 px-8 py-4 rounded-2xl shadow-2xl">
          <div class="relative">
            <span class="material-icons text-4xl text-white drop-shadow-lg">account_circle</span>
            <div class="absolute -bottom-1 -right-1 w-4 h-4 bg-green-400 rounded-full border-2 border-white"></div>
          </div>
          <div>
            <p class="text-xs text-white/80 font-medium uppercase tracking-wider">Welcome Back</p>
            <p class="font-bold text-xl text-white drop-shadow-md">User</p>
          </div>
        </div>
      </div>
    </div>

    <!-- Main Dashboard -->
    <main class="flex-1 px-4 md:px-8 py-6">
      <div class="text-center">
        <h1 class="text-2xl font-bold">Dashboard Test</h1>
        <p>This is a minimal test version</p>
      </div>
    </main>

    <!-- Footer -->
    <footer class="relative overflow-hidden bg-gradient-to-r from-[#3D52A0] via-[#4a5fb8] to-[#3D52A0] text-center py-6 text-white border-t-4 border-white/20 mt-auto backdrop-blur-sm">
      <div class="absolute inset-0 bg-gradient-to-t from-black/10 to-transparent"></div>
      <div class="relative">
        <p class="font-semibold text-sm tracking-wide">© 2025 VIGILERT. All rights reserved.</p>
        <p class="text-xs text-white/70 mt-1">Smart Helmet Safety System</p>
      </div>
    </footer>
  </div>
</template>

<script setup>
import { ref } from 'vue';

// Minimal setup for testing
const test = ref('Dashboard Test');
</script>

<style scoped>
/* Minimal styles */
</style>