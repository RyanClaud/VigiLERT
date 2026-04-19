<template>
  <div class="min-h-screen flex bg-gradient-to-br from-[#0f1729] via-[#1a2a5e] to-[#0f1729] overflow-hidden">

    <!-- Left panel — branding -->
    <div class="hidden lg:flex lg:w-1/2 relative flex-col items-center justify-center p-12 overflow-hidden">
      <!-- Animated orbs — red/orange theme for emergency -->
      <div class="absolute inset-0">
        <div class="absolute top-1/4 left-1/4 w-96 h-96 bg-red-900/25 rounded-full blur-3xl animate-pulse"></div>
        <div class="absolute bottom-1/4 right-1/4 w-80 h-80 bg-orange-800/20 rounded-full blur-3xl animate-pulse" style="animation-delay:1.5s"></div>
        <div class="absolute top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 w-64 h-64 bg-red-700/10 rounded-full blur-2xl animate-pulse" style="animation-delay:0.75s"></div>
      </div>

      <!-- Grid overlay -->
      <div class="absolute inset-0 opacity-5"
        style="background-image: linear-gradient(rgba(255,255,255,.1) 1px, transparent 1px), linear-gradient(90deg, rgba(255,255,255,.1) 1px, transparent 1px); background-size: 40px 40px;"></div>

      <div class="relative z-10 text-center max-w-md">
        <!-- Logo with emergency accent -->
        <div class="inline-flex items-center justify-center w-24 h-24 rounded-3xl bg-gradient-to-br from-red-600 to-red-800 shadow-2xl mb-8 ring-4 ring-red-500/20">
          <span class="material-icons text-5xl text-white">emergency</span>
        </div>

        <h1 class="text-5xl font-black text-white mb-4 tracking-tight">
          VIGIL<span class="text-red-400">ERT</span>
        </h1>
        <p class="text-white/60 text-lg leading-relaxed mb-10">
          Emergency Contact Portal — Monitor your rider's safety in real time and respond instantly when it matters most.
        </p>

        <!-- Info cards -->
        <div class="space-y-3 text-left">
          <div v-for="item in infoItems" :key="item.label"
            class="flex items-center gap-3 bg-white/5 border border-white/10 rounded-2xl px-4 py-3 hover:bg-white/8 transition-all duration-300">
            <div class="w-9 h-9 rounded-xl bg-red-500/20 flex items-center justify-center flex-shrink-0">
              <span class="material-icons text-red-400 text-lg">{{ item.icon }}</span>
            </div>
            <div>
              <p class="text-white text-sm font-semibold">{{ item.label }}</p>
              <p class="text-white/40 text-xs">{{ item.desc }}</p>
            </div>
          </div>
        </div>
      </div>
    </div>

    <!-- Right panel — form -->
    <div class="w-full lg:w-1/2 flex items-center justify-center p-6 lg:p-12">
      <div class="w-full max-w-md">

        <!-- Mobile logo -->
        <div class="flex items-center gap-3 mb-8 lg:hidden">
          <div class="w-10 h-10 rounded-xl bg-gradient-to-br from-red-600 to-red-800 flex items-center justify-center shadow-lg">
            <span class="material-icons text-white text-xl">emergency</span>
          </div>
          <span class="text-2xl font-black text-white tracking-tight">VIGIL<span class="text-red-400">ERT</span></span>
        </div>

        <!-- Header -->
        <div class="mb-8">
          <div class="inline-flex items-center gap-2 bg-red-500/10 border border-red-500/20 rounded-full px-4 py-1.5 mb-4">
            <span class="w-2 h-2 bg-red-400 rounded-full animate-pulse"></span>
            <span class="text-red-400 text-xs font-semibold uppercase tracking-wider">Emergency Contact Access</span>
          </div>
          <h2 class="text-3xl font-bold text-white mb-2">Guardian Login</h2>
          <p class="text-white/50">Enter the mobile number registered as an emergency contact to access the rider's safety dashboard.</p>
        </div>

        <!-- Form -->
        <form @submit.prevent="checkPhone" class="space-y-5">
          <div class="group">
            <label class="block text-sm font-medium text-white/60 mb-1.5">
              Registered Mobile Number
            </label>
            <div class="relative">
              <span class="absolute left-4 top-1/2 -translate-y-1/2 material-icons text-white/30 text-lg group-focus-within:text-red-400 transition-colors">phone</span>
              <input
                v-model="phone"
                type="tel"
                autocomplete="tel"
                required
                :disabled="loading || success"
                placeholder="e.g. 09876543210 or +639876543210"
                class="w-full bg-white/5 border border-white/10 text-white placeholder-white/25 rounded-2xl pl-11 pr-4 py-3.5 focus:outline-none focus:border-red-500 focus:ring-2 focus:ring-red-500/20 transition-all duration-300 disabled:opacity-50"
              />
            </div>
            <p class="text-white/30 text-xs mt-1.5 ml-1">
              Philippine numbers: 09XXXXXXXXX or +639XXXXXXXXX
            </p>
          </div>

          <!-- Error message -->
          <div v-if="error"
            class="flex items-start gap-2 bg-red-500/10 border border-red-500/20 rounded-xl px-4 py-3 animate-slide-down">
            <span class="material-icons text-red-400 text-sm mt-0.5 flex-shrink-0">error_outline</span>
            <p class="text-red-400 text-sm">{{ error }}</p>
          </div>

          <!-- Success message -->
          <div v-if="success"
            class="flex items-center gap-2 bg-green-500/10 border border-green-500/20 rounded-xl px-4 py-3 animate-slide-down">
            <span class="material-icons text-green-400 text-sm">check_circle</span>
            <p class="text-green-400 text-sm font-medium">Verified! Redirecting to dashboard...</p>
          </div>

          <!-- Submit -->
          <button type="submit" :disabled="loading || success"
            class="w-full bg-gradient-to-r from-red-600 to-red-700 hover:from-red-500 hover:to-red-600 text-white font-bold py-3.5 px-6 rounded-2xl shadow-lg hover:shadow-red-900/40 hover:shadow-xl transition-all duration-300 hover:-translate-y-0.5 active:translate-y-0 disabled:opacity-60 disabled:cursor-not-allowed flex items-center justify-center gap-2">
            <span v-if="loading" class="material-icons text-lg animate-spin">refresh</span>
            <span v-else-if="success" class="material-icons text-lg">check_circle</span>
            <span v-else class="material-icons text-lg">shield</span>
            <span>{{ loading ? 'Verifying...' : success ? 'Redirecting...' : 'Access Dashboard' }}</span>
          </button>
        </form>

        <!-- Back to login -->
        <div class="mt-6 pt-6 border-t border-white/10">
          <button @click="goBack"
            class="w-full flex items-center justify-center gap-2 bg-white/5 hover:bg-white/10 border border-white/10 hover:border-white/20 text-white/60 hover:text-white font-medium py-3 px-6 rounded-2xl transition-all duration-300">
            <span class="material-icons text-lg">arrow_back</span>
            Back to Rider Login
          </button>
        </div>

        <!-- Help text -->
        <p class="text-center text-white/25 text-xs mt-5 leading-relaxed">
          Your number must be registered by the rider as an emergency contact. Contact the rider if you're having trouble accessing the dashboard.
        </p>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue';
import { getDatabase, ref as dbRef, get } from 'firebase/database';
import { useRouter } from 'vue-router';

const phone   = ref('');
const error   = ref('');
const success = ref(false);
const loading = ref(false);
const router  = useRouter();

const infoItems = [
  { icon: 'location_on',        label: 'Live Location Tracking',  desc: 'See the rider\'s real-time GPS position' },
  { icon: 'warning',            label: 'Instant Crash Alerts',    desc: 'Get notified immediately if a crash is detected' },
  { icon: 'local_bar',          label: 'Alcohol Detection',       desc: 'Monitor alcohol sensor status in real time' },
  { icon: 'emergency',          label: 'SOS Alert',               desc: 'Send an emergency alert directly to the rider' },
];

const normalizePhoneNumber = (num) => {
  const cleaned = num.replace(/\D/g, '');
  if (cleaned.startsWith('0')) return '+63' + cleaned.slice(1);
  if (!cleaned.startsWith('+')) return '+' + cleaned;
  return cleaned;
};

const checkPhone = async () => {
  error.value   = '';
  success.value = false;
  loading.value = true;

  const db = getDatabase();
  const normalizedInput = normalizePhoneNumber(phone.value);

  try {
    const snapshot = await get(dbRef(db, 'users'));

    if (!snapshot.exists()) {
      error.value = 'No registered users found.';
      return;
    }

    let foundUserId = null;

    snapshot.forEach((userSnap) => {
      const userData = userSnap.val();
      if (userData.emergencyContacts) {
        Object.values(userData.emergencyContacts).forEach((contact) => {
          if (normalizePhoneNumber(contact.phone) === normalizedInput) {
            foundUserId = userSnap.key;
          }
        });
      }
    });

    if (foundUserId) {
      success.value = true;
      setTimeout(() => router.push(`/emergency-dashboard/${foundUserId}`), 1200);
    } else {
      error.value = 'This number is not registered as an emergency contact. Please check the number and try again.';
    }
  } catch (e) {
    console.error('[EMERGENCY LOGIN]', e);
    error.value = 'Something went wrong. Please try again.';
  } finally {
    loading.value = false;
  }
};

const goBack = () => router.push('/login');
</script>
