<template>
  <div class="min-h-screen bg-[#EDE8F5]">
    <nav v-if="authStore.isAuthenticated" class="bg-gradient-to-r from-[#3D52A0] to-[#2a3a70] shadow-xl sticky top-0 z-50 backdrop-blur-sm">
      <div class="max-w-7xl mx-auto px-4">
        <div class="flex justify-between h-16 items-center">
          <div class="flex items-center">
            <div class="flex-shrink-0 flex items-center gap-2">
              <span class="material-icons text-white text-3xl">security</span>
              <h1 class="text-2xl font-bold text-white tracking-wide">VIGILERT</h1>
            </div>
            <div class="hidden sm:ml-8 sm:flex sm:space-x-4">
              <router-link to="/dashboard" class="nav-link" prefetch>
                <span class="material-icons text-sm">dashboard</span>
                Dashboard
              </router-link>
              <router-link to="/emergency-contacts" class="nav-link" prefetch>
                <span class="material-icons text-sm">contacts</span>
                Emergency Contacts
              </router-link>
            </div>
          </div>
          <div class="flex items-center sm:hidden">
            <button @click="toggleMobileMenu" class="text-white focus:outline-none p-2 rounded-lg hover:bg-white/10 transition-all">
              <span v-if="!showMobileMenu" class="material-icons text-3xl">menu</span>
              <span v-else class="material-icons text-3xl">close</span>
            </button>
          </div>
          <div class="hidden sm:flex items-center">
            <button @click="handleLogout" class="flex items-center gap-2 bg-[#7091E6] text-white font-semibold px-5 py-2.5 rounded-xl shadow-lg transition-all duration-300 hover:bg-[#8697C4] hover:scale-105 focus:outline-none focus:ring-2 focus:ring-white/50">
              <span class="material-icons text-sm">logout</span>
              Logout
            </button>
          </div>
        </div>
        <transition name="mobile-menu">
          <div v-if="showMobileMenu" class="sm:hidden bg-[#2a3a70] rounded-b-2xl shadow-2xl mt-2 overflow-hidden">
            <div class="flex flex-col space-y-1 p-4">
              <router-link to="/dashboard" class="nav-link-mobile" @click="closeMobileMenu" prefetch>
                <span class="material-icons">dashboard</span>
                Dashboard
              </router-link>
              <router-link to="/emergency-contacts" class="nav-link-mobile" @click="closeMobileMenu" prefetch>
                <span class="material-icons">contacts</span>
                Emergency Contacts
              </router-link>
              <button @click="handleLogout" class="flex items-center gap-2 justify-center bg-[#7091E6] text-white font-semibold px-4 py-3 rounded-xl shadow-lg transition-all duration-200 hover:bg-[#8697C4] focus:outline-none focus:ring-2 focus:ring-white/50 mt-3">
                <span class="material-icons">logout</span>
                Logout
              </button>
            </div>
          </div>
        </transition>
      </div>
    </nav>

    <main class="max-w-7xl mx-auto py-6 sm:px-6 lg:px-8">
      <router-view v-if="!authStore.loading" v-slot="{ Component }">
        <transition name="fade" mode="out-in">
          <component :is="Component" />
        </transition>
      </router-view>
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

const toggleMobileMenu = () => {
  showMobileMenu.value = !showMobileMenu.value;
};

const closeMobileMenu = () => {
  showMobileMenu.value = false;
};

const handleLogout = async () => {
  closeMobileMenu();
  try {
    await authStore.logout();
    router.push('/login');
  } catch (error) {
    console.error('Error logging out:', error);
  }
};

// Close mobile menu on route change
router.afterEach(() => {
  closeMobileMenu();
});
</script>

<style>
.nav-link {
  @apply inline-flex items-center gap-2 px-4 py-2 text-sm font-medium text-white/80 hover:text-white hover:bg-white/10 rounded-lg transition-all duration-300;
}

.nav-link-mobile {
  @apply flex items-center gap-3 px-4 py-3 rounded-xl text-base font-medium text-white hover:bg-[#7091E6] transition-all duration-300;
}

.router-link-active {
  @apply text-white bg-white/20 font-bold shadow-lg;
}

/* Fast page transitions */
.fade-enter-active {
  transition: opacity 0.12s ease-in;
}

.fade-leave-active {
  transition: opacity 0.08s ease-out;
}

.fade-enter-from,
.fade-leave-to {
  opacity: 0;
}

/* Ultra-fast mobile menu transitions */
.mobile-menu-enter-active {
  transition: all 0.15s ease-out;
  transform-origin: top;
}

.mobile-menu-leave-active {
  transition: all 0.1s ease-in;
  transform-origin: top;
}

.mobile-menu-enter-from {
  transform: scaleY(0.95);
  opacity: 0;
}

.mobile-menu-leave-to {
  transform: scaleY(0.95);
  opacity: 0;
}

/* Optimize mobile interactions */
@media (max-width: 640px) {
  * {
    -webkit-tap-highlight-color: transparent;
    touch-action: manipulation;
  }
  
  .nav-link-mobile {
    transition: background-color 0.15s ease;
  }
}
</style> 