<template>
  <div class="min-h-screen bg-[#EDE8F5]">
    <nav v-if="authStore.isAuthenticated" class="bg-[#3D52A0] shadow-lg">
      <div class="max-w-7xl mx-auto px-4">
        <div class="flex justify-between h-16 items-center">
          <div class="flex items-center">
            <div class="flex-shrink-0 flex items-center">
              <h1 class="text-xl font-bold text-white">VIGILERT</h1>
            </div>
            <div class="hidden sm:ml-6 sm:flex sm:space-x-8">
              <router-link to="/dashboard" class="nav-link">Dashboard</router-link>
              <router-link to="/emergency-contacts" class="nav-link">Emergency Contacts</router-link>
            </div>
          </div>
          <div class="flex items-center sm:hidden">
            <button @click="showMobileMenu = !showMobileMenu" class="text-white focus:outline-none">
              <svg v-if="!showMobileMenu" class="h-7 w-7" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6h16M4 12h16M4 18h16" /></svg>
              <svg v-else class="h-7 w-7" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12" /></svg>
            </button>
          </div>
          <div class="hidden sm:flex items-center">
            <button @click="handleLogout" class="bg-[#7091E6] text-white font-semibold px-4 py-2 rounded-lg shadow transition hover:bg-[#8697C4] hover:scale-105 focus:outline-none focus:ring-2 focus:ring-[#ADBBD4]">Logout</button>
          </div>
        </div>
        <div v-if="showMobileMenu" class="sm:hidden bg-[#3D52A0] rounded-b-lg shadow-lg mt-2">
          <div class="flex flex-col space-y-2 p-4">
            <router-link to="/dashboard" class="nav-link-mobile" @click="showMobileMenu = false">Dashboard</router-link>
            <router-link to="/trip-history" class="nav-link-mobile" @click="showMobileMenu = false">Trip History</router-link>
            <router-link to="/emergency-contacts" class="nav-link-mobile" @click="showMobileMenu = false">Emergency Contacts</router-link>
            <button @click="handleLogout" class="bg-[#7091E6] text-white font-semibold px-4 py-2 rounded-lg shadow transition hover:bg-[#8697C4] hover:scale-105 focus:outline-none focus:ring-2 focus:ring-[#ADBBD4] mt-2">Logout</button>
          </div>
        </div>
      </div>
    </nav>

    <main class="max-w-7xl mx-auto py-6 sm:px-6 lg:px-8">
      <router-view v-if="!authStore.loading"></router-view>
      <div v-else class="flex justify-center items-center h-screen">
        <div class="animate-spin rounded-full h-12 w-12 border-b-2 border-[#7091E6]"></div>
      </div>
    </main>
  </div>
</template>

<script setup>
import { ref } from 'vue';
import { useRouter } from 'vue-router';
import { useAuthStore } from './stores/auth';

const router = useRouter();
const authStore = useAuthStore();
const showMobileMenu = ref(false);

const handleLogout = async () => {
  try {
    await authStore.logout();
    router.push('/login');
  } catch (error) {
    console.error('Error logging out:', error);
  }
};
</script>

<style>
.nav-link {
  @apply inline-flex items-center px-1 pt-1 text-sm font-medium text-gray-200 hover:text-white transition;
}

.nav-link-mobile {
  @apply block px-3 py-2 rounded-md text-base font-medium text-white hover:bg-[#7091E6] transition;
}

.router-link-active {
  @apply text-[#ADBBD4] font-bold;
}
</style> 