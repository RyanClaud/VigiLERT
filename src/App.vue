<template>
  <div class="min-h-screen flex flex-col bg-[#061B2B] text-white selection:bg-[#00B85C] selection:text-white font-['Inter',sans-serif] relative overflow-x-hidden">
    
    <!-- Global Futuristic Ambient Glow & Faint Cyber Grid -->
    <div class="fixed inset-0 pointer-events-none z-0 overflow-hidden">
      <!-- Dark Blue Center Radial Gradient -->
      <div class="absolute inset-0 bg-[radial-gradient(circle_at_50%_20%,#0B2540_0%,#061B2B_80%)]"></div>
      
      <!-- Faint Cyber Grid Pattern -->
      <div class="absolute inset-0 opacity-[0.03]"
        style="background-image: linear-gradient(rgba(112,227,74,0.3) 1px, transparent 1px), linear-gradient(90deg, rgba(112,227,74,0.3) 1px, transparent 1px); background-size: 48px 48px;">
      </div>

      <!-- Subtle Ambient Emerald Glows -->
      <div class="absolute -top-32 -left-32 w-96 h-96 bg-[#00B85C]/10 rounded-full blur-3xl pointer-events-none"></div>
      <div class="absolute top-1/2 -right-32 w-96 h-96 bg-[#00B85C]/5 rounded-full blur-3xl pointer-events-none"></div>
    </div>

    <!-- Top Navigation Header (Authenticated) -->
    <header v-if="authStore.isAuthenticated" class="sticky top-0 z-50 bg-[#0B2540]/85 backdrop-blur-md border-b border-white/10 shadow-xl transition-all duration-300">
      <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
        <div class="flex justify-between h-16 sm:h-20 items-center">
          
          <!-- Left: Branding & Tagline -->
          <div class="flex items-center gap-3 sm:gap-6">
            <router-link to="/dashboard" class="flex items-center gap-2.5 sm:gap-3 group focus:outline-none">
              <!-- Shield Logo -->
              <div class="w-10 h-10 sm:w-11 sm:h-11 relative flex items-center justify-center">
                <img 
                  src="/images/shield-logo.png" 
                  alt="VigiLERT Logo" 
                  class="w-full h-full object-contain filter drop-shadow-[0_0_8px_rgba(0,184,92,0.5)] group-hover:scale-105 transition-transform" 
                />
              </div>
              
              <!-- Brand Title + ECG Wave -->
              <div class="flex flex-col">
                <div class="flex items-center gap-1.5">
                  <span class="text-xl sm:text-2xl font-black tracking-tight text-white">
                    Vigi<span class="text-[#00B85C]">LERT</span>
                  </span>
                  <svg class="h-4 sm:h-5 w-10 sm:w-14 text-[#00B85C] inline-block" viewBox="0 0 80 28" fill="none">
                    <path d="M0 14 H22 L28 3 L34 25 L40 8 L46 19 L52 14 H80" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/>
                  </svg>
                </div>
                <span class="hidden sm:inline-block text-[11px] font-medium italic text-slate-300 tracking-wider">
                  Ride Smart. Stay Safe.
                </span>
              </div>
            </router-link>

            <!-- Desktop Navigation Pills -->
            <nav class="hidden md:flex items-center gap-1.5 lg:gap-2 ml-4 lg:ml-8">
              <router-link to="/dashboard" class="app-nav-pill" prefetch>
                <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <rect width="7" height="9" x="3" y="3" rx="1"/>
                  <rect width="7" height="5" x="14" y="3" rx="1"/>
                  <rect width="7" height="9" x="14" y="12" rx="1"/>
                  <rect width="7" height="5" x="3" y="16" rx="1"/>
                </svg>
                <span>Dashboard</span>
              </router-link>

              <router-link to="/emergency-contacts" class="app-nav-pill" prefetch>
                <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M16 21v-2a4 4 0 0 0-4-4H6a4 4 0 0 0-4 4v2"/>
                  <circle cx="9" cy="7" r="4"/>
                  <path d="M22 21v-2a4 4 0 0 0-3-3.87"/>
                  <path d="M16 3.13a4 4 0 0 1 0 7.75"/>
                </svg>
                <span>Emergency Contacts</span>
              </router-link>

              <router-link to="/trip-history" class="app-nav-pill" prefetch>
                <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <circle cx="12" cy="12" r="10"/>
                  <polyline points="12 6 12 12 16 14"/>
                </svg>
                <span>Trip History</span>
              </router-link>

              <router-link to="/device-setup" class="app-nav-pill" prefetch>
                <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <rect width="14" height="20" x="5" y="2" rx="2" ry="2"/>
                  <path d="M12 18h.01"/>
                </svg>
                <span>Device Setup</span>
              </router-link>

              <router-link to="/profile" class="app-nav-pill" prefetch>
                <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M19 21v-2a4 4 0 0 0-4-4H9a4 4 0 0 0-4 4v2"/>
                  <circle cx="12" cy="7" r="4"/>
                </svg>
                <span>Profile</span>
              </router-link>
            </nav>
          </div>

          <!-- Right: User Profile & Actions -->
          <div class="flex items-center gap-3">
            
            <!-- User Profile Chip (Desktop) -->
            <router-link 
              to="/profile" 
              class="hidden sm:flex items-center gap-3 bg-[#061B2B]/60 hover:bg-[#061B2B]/90 border border-white/10 hover:border-[#00B85C]/50 rounded-2xl py-1.5 px-3.5 backdrop-blur-sm transition-all group cursor-pointer"
              title="View & Edit Profile Settings"
            >
              <!-- Avatar -->
              <div class="relative w-8 h-8 rounded-full overflow-hidden border border-emerald-400/40 group-hover:border-[#00B85C] bg-gradient-to-tr from-[#00B85C] to-[#0B2540] flex items-center justify-center flex-shrink-0 transition-all">
                <img 
                  v-if="authStore.photoURL" 
                  :src="authStore.photoURL" 
                  alt="User Avatar"
                  class="w-full h-full object-cover" 
                />
                <span v-else class="text-xs font-bold text-white">
                  {{ (authStore.user?.displayName || authStore.user?.email || 'R')[0].toUpperCase() }}
                </span>
                <span class="absolute bottom-0 right-0 w-2 h-2 rounded-full bg-[#00B85C] ring-1 ring-[#061B2B]"></span>
              </div>

              <!-- Name & Role -->
              <div class="flex flex-col text-left">
                <span class="text-xs font-bold text-white max-w-[120px] truncate leading-tight group-hover:text-[#00B85C] transition-colors">
                  {{ authStore.user?.displayName || authStore.user?.email?.split('@')[0] || 'Rider' }}
                </span>
                <div class="flex items-center gap-1 mt-0.5">
                  <span class="w-1.5 h-1.5 rounded-full bg-[#00B85C] animate-pulse"></span>
                  <span class="text-[10px] text-slate-300 font-medium uppercase tracking-wider">Rider Settings</span>
                </div>
              </div>
            </router-link>

            <!-- Logout Button (Desktop) -->
            <button 
              @click="handleLogout" 
              class="hidden sm:inline-flex items-center gap-2 bg-red-500/10 hover:bg-red-500/20 active:bg-red-500/30 text-red-300 hover:text-red-200 border border-red-500/20 hover:border-red-400/40 font-semibold px-4 py-2 rounded-xl transition-all duration-200 text-xs shadow-sm"
              title="Sign out of VigiLERT"
            >
              <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                <path d="M9 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h4"/>
                <polyline points="16 17 21 12 16 7"/>
                <line x1="21" x2="9" y1="12" y2="12"/>
              </svg>
              <span>Logout</span>
            </button>

            <!-- Mobile Menu Toggle Button -->
            <button 
              @click="toggleMobileMenu" 
              class="sm:hidden text-white p-2 rounded-xl bg-white/5 border border-white/10 hover:bg-white/10 transition-colors focus:outline-none"
              aria-label="Toggle Navigation Menu"
            >
              <svg v-if="!showMobileMenu" class="w-6 h-6" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6h16M4 12h16m-7 6h7" />
              </svg>
              <svg v-else class="w-6 h-6" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12" />
              </svg>
            </button>
          </div>

        </div>

        <!-- Mobile Navigation Drawer -->
        <transition name="drawer">
          <div v-if="showMobileMenu" class="sm:hidden border-t border-white/10 bg-[#0B2540] rounded-b-2xl shadow-2xl py-4 px-2 space-y-2">
            <!-- Mobile User Profile (Clickable to /profile) -->
            <router-link to="/profile" @click="closeMobileMenu" class="flex items-center gap-3 px-3 py-2.5 bg-white/5 hover:bg-white/10 rounded-xl border border-white/5 mb-3 transition-all group">
              <div class="w-10 h-10 rounded-full overflow-hidden border border-[#00B85C] bg-[#00B85C]/20 flex items-center justify-center font-bold text-white text-sm flex-shrink-0">
                <img 
                  v-if="authStore.photoURL" 
                  :src="authStore.photoURL" 
                  alt="Avatar"
                  class="w-full h-full object-cover" 
                />
                <span v-else>{{ (authStore.user?.displayName || authStore.user?.email || 'R')[0].toUpperCase() }}</span>
              </div>
              <div class="flex flex-col text-left min-w-0 flex-1">
                <span class="text-sm font-bold text-white truncate group-hover:text-[#00B85C] transition-colors">
                  {{ authStore.user?.displayName || authStore.user?.email || 'Rider' }}
                </span>
                <span class="text-[11px] text-[#00B85C] font-semibold flex items-center gap-1">
                  <span class="material-icons text-xs">manage_accounts</span>
                  Edit Profile & Security
                </span>
              </div>
            </router-link>

            <!-- Links -->
            <router-link to="/dashboard" class="app-nav-pill-mobile" @click="closeMobileMenu" prefetch>
              <span class="material-icons text-lg">dashboard</span>
              <span>Dashboard</span>
            </router-link>

            <router-link to="/emergency-contacts" class="app-nav-pill-mobile" @click="closeMobileMenu" prefetch>
              <span class="material-icons text-lg">contacts</span>
              <span>Emergency Contacts</span>
            </router-link>

            <router-link to="/trip-history" class="app-nav-pill-mobile" @click="closeMobileMenu" prefetch>
              <span class="material-icons text-lg">route</span>
              <span>Trip History</span>
            </router-link>

            <router-link to="/device-setup" class="app-nav-pill-mobile" @click="closeMobileMenu" prefetch>
              <span class="material-icons text-lg">settings_suggest</span>
              <span>Device Setup</span>
            </router-link>

            <router-link to="/profile" class="app-nav-pill-mobile" @click="closeMobileMenu" prefetch>
              <span class="material-icons text-lg">account_circle</span>
              <span>Profile Settings</span>
            </router-link>

            <!-- Mobile Logout -->
            <button 
              @click="handleLogout" 
              class="w-full flex items-center justify-center gap-2 bg-red-500/15 hover:bg-red-500/25 text-red-300 font-bold py-2.5 px-4 rounded-xl transition-all border border-red-500/25 mt-2 text-sm"
            >
              <span class="material-icons text-base">logout</span>
              <span>Sign Out</span>
            </button>
          </div>
        </transition>

      </div>
    </header>

    <!-- Main Application Views Outlet -->
    <main class="relative z-10 flex-1 w-full max-w-7xl mx-auto py-5 sm:py-7 px-3 sm:px-6 lg:px-8">
      <router-view v-if="!authStore.loading" v-slot="{ Component }">
        <transition name="page-fade" mode="out-in">
          <component :is="Component" />
        </transition>
      </router-view>
      
      <!-- Global Loading State -->
      <div v-else class="flex flex-col justify-center items-center h-[70vh] gap-4">
        <div class="relative w-16 h-16 flex items-center justify-center">
          <div class="absolute inset-0 rounded-full border-2 border-[#00B85C]/20 border-t-[#00B85C] animate-spin"></div>
          <img src="/images/shield-logo.png" alt="VigiLERT" class="w-8 h-8 object-contain" />
        </div>
        <p class="text-sm font-semibold text-slate-400 animate-pulse tracking-wide">
          Connecting to VigiLERT System...
        </p>
      </div>
    </main>

    <!-- Fixed Mobile Bottom Navigation Bar (Matching Mockup media_1794925789095_c2d335e7.jpg) -->
    <nav v-if="authStore.isAuthenticated" class="sm:hidden fixed bottom-0 left-0 right-0 z-50 bg-[#0B2540]/95 backdrop-blur-lg border-t border-white/10 shadow-[0_-4px_20px_rgba(0,0,0,0.5)] px-2 py-1.5 flex items-center justify-around">
      <router-link to="/dashboard" class="mobile-tab-btn" prefetch>
        <svg class="w-5 h-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
          <path d="m3 9 9-7 9 7v11a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2z"/>
          <polyline points="9 22 9 12 15 12 15 22"/>
        </svg>
        <span>Dashboard</span>
      </router-link>

      <router-link to="/emergency-contacts" class="mobile-tab-btn" prefetch>
        <svg class="w-5 h-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
          <path d="M16 21v-2a4 4 0 0 0-4-4H6a4 4 0 0 0-4 4v2"/>
          <circle cx="9" cy="7" r="4"/>
          <path d="M22 21v-2a4 4 0 0 0-3-3.87"/>
          <path d="M16 3.13a4 4 0 0 1 0 7.75"/>
        </svg>
        <span>Contacts</span>
      </router-link>

      <router-link to="/trip-history" class="mobile-tab-btn" prefetch>
        <svg class="w-5 h-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
          <circle cx="12" cy="12" r="10"/>
          <polyline points="12 6 12 12 16 14"/>
        </svg>
        <span>Trips</span>
      </router-link>

      <router-link to="/device-setup" class="mobile-tab-btn" prefetch>
        <svg class="w-5 h-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
          <circle cx="12" cy="12" r="3"/>
          <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"/>
        </svg>
        <span>Setup</span>
      </router-link>

      <router-link to="/profile" class="mobile-tab-btn" prefetch>
        <svg class="w-5 h-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
          <path d="M19 21v-2a4 4 0 0 0-4-4H9a4 4 0 0 0-4 4v2"/>
          <circle cx="12" cy="7" r="4"/>
        </svg>
        <span>Profile</span>
      </router-link>
    </nav>

    <!-- Bottom Footer Status Bar -->
    <footer class="relative z-10 w-full border-t border-white/5 bg-[#061B2B]/90 backdrop-blur-md py-3 px-4 sm:px-6 lg:px-8 text-xs text-slate-400 mt-auto pb-16 sm:pb-3">
      <div class="max-w-7xl mx-auto flex flex-col sm:flex-row items-center justify-between gap-2">
        <div class="flex items-center flex-wrap justify-center sm:justify-start gap-x-2 gap-y-1 font-medium">
          <span class="text-white font-semibold">VigiLERT</span>
          <span>•</span>
          <span>Motorcycle Safety</span>
          <span>•</span>
          <span>Emergency Response</span>
          <span>•</span>
          <span>Anti-Theft Protection</span>
        </div>
        <div class="flex items-center gap-1.5 text-[#00B85C] font-semibold">
          <svg class="w-4 h-4" viewBox="0 0 24 24" fill="currentColor">
            <path fill-rule="evenodd" d="M12.516 2.17a.75.75 0 0 0-1.032 0 11.209 11.209 0 0 1-7.877 3.08.75.75 0 0 0-.722.515A12.74 12.74 0 0 0 2.25 9.75c0 5.942 4.064 10.933 9.563 12.348a.75.75 0 0 0 .374 0c5.499-1.415 9.563-6.406 9.563-12.348 0-1.39-.223-2.73-.635-3.985a.75.75 0 0 0-.722-.516l-.143.001c-2.996 0-5.717-1.17-7.734-3.08Zm3.094 8.016a.75.75 0 1 0-1.22-.872l-3.236 4.53L9.53 12.22a.75.75 0 0 0-1.06 1.06l2.25 2.25a.75.75 0 0 0 1.14-.094l3.75-5.25Z" clip-rule="evenodd" />
          </svg>
          <span>Secure & Trusted</span>
        </div>
      </div>
    </footer>

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
/* Global Nav Pill styles */
.app-nav-pill {
  @apply inline-flex items-center gap-2 px-3.5 py-2 text-xs lg:text-sm font-semibold text-slate-300 hover:text-white hover:bg-white/5 rounded-xl transition-all duration-200;
}

.app-nav-pill.router-link-active {
  @apply bg-[#00B85C] text-white shadow-lg shadow-[#00B85C]/25 font-bold;
}

.app-nav-pill-mobile {
  @apply flex items-center gap-3 px-4 py-3 rounded-xl text-sm font-semibold text-slate-300 hover:text-white hover:bg-white/5 transition-all;
}

.app-nav-pill-mobile.router-link-active {
  @apply bg-[#00B85C] text-white font-bold;
}

/* Mobile Bottom Tab styles */
.mobile-tab-btn {
  @apply flex flex-col items-center justify-center gap-1 py-1 px-3 text-[11px] font-semibold text-slate-400 hover:text-white transition-colors;
}

.mobile-tab-btn.router-link-active {
  @apply text-[#00B85C] font-bold;
}

/* Page transitions */
.page-fade-enter-active {
  transition: opacity 0.15s ease-out;
}

.page-fade-leave-active {
  transition: opacity 0.1s ease-in;
}

.page-fade-enter-from,
.page-fade-leave-to {
  opacity: 0;
}

/* Mobile drawer animation */
.drawer-enter-active {
  transition: all 0.2s ease-out;
}

.drawer-leave-active {
  transition: all 0.15s ease-in;
}

.drawer-enter-from,
.drawer-leave-to {
  opacity: 0;
  transform: translateY(-8px);
}
</style>
