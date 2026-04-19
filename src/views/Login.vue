<template>
  <div class="min-h-screen flex bg-gradient-to-br from-[#0f1729] via-[#1a2a5e] to-[#0f1729] overflow-hidden">

    <!-- Left panel — branding (hidden on mobile) -->
    <div class="hidden lg:flex lg:w-1/2 relative flex-col items-center justify-center p-12 overflow-hidden">
      <!-- Animated background orbs -->
      <div class="absolute inset-0">
        <div class="absolute top-1/4 left-1/4 w-96 h-96 bg-[#3D52A0]/30 rounded-full blur-3xl animate-pulse"></div>
        <div class="absolute bottom-1/4 right-1/4 w-80 h-80 bg-[#7091E6]/20 rounded-full blur-3xl animate-pulse" style="animation-delay:1.5s"></div>
        <div class="absolute top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 w-64 h-64 bg-[#5571c6]/15 rounded-full blur-2xl animate-pulse" style="animation-delay:0.75s"></div>
      </div>

      <!-- Grid overlay -->
      <div class="absolute inset-0 opacity-5"
        style="background-image: linear-gradient(rgba(255,255,255,.1) 1px, transparent 1px), linear-gradient(90deg, rgba(255,255,255,.1) 1px, transparent 1px); background-size: 40px 40px;"></div>

      <div class="relative z-10 text-center max-w-md">
        <!-- Logo -->
        <div class="inline-flex items-center justify-center w-24 h-24 rounded-3xl bg-gradient-to-br from-[#3D52A0] to-[#7091E6] shadow-2xl mb-8 ring-4 ring-white/10">
          <span class="material-icons text-5xl text-white">shield</span>
        </div>

        <h1 class="text-5xl font-black text-white mb-4 tracking-tight">
          VIGIL<span class="text-[#7091E6]">ERT</span>
        </h1>
        <p class="text-white/60 text-lg leading-relaxed mb-10">
          Smart motorcycle safety system. Real-time monitoring, crash detection, and engine protection — all in one.
        </p>

        <!-- Feature pills -->
        <div class="flex flex-wrap justify-center gap-3">
          <div v-for="f in features" :key="f.label"
            class="flex items-center gap-2 bg-white/5 border border-white/10 backdrop-blur-sm rounded-full px-4 py-2 text-sm text-white/70 hover:bg-white/10 transition-all duration-300">
            <span class="material-icons text-[#7091E6] text-base">{{ f.icon }}</span>
            {{ f.label }}
          </div>
        </div>
      </div>
    </div>

    <!-- Right panel — form -->
    <div class="w-full lg:w-1/2 flex items-center justify-center p-6 lg:p-12">
      <div class="w-full max-w-md">

        <!-- Mobile logo -->
        <div class="flex items-center gap-3 mb-8 lg:hidden">
          <div class="w-10 h-10 rounded-xl bg-gradient-to-br from-[#3D52A0] to-[#7091E6] flex items-center justify-center shadow-lg">
            <span class="material-icons text-white text-xl">shield</span>
          </div>
          <span class="text-2xl font-black text-white tracking-tight">VIGIL<span class="text-[#7091E6]">ERT</span></span>
        </div>

        <h2 class="text-3xl font-bold text-white mb-2">Welcome back</h2>
        <p class="text-white/50 mb-8">Sign in to your safety dashboard</p>

        <!-- Google Sign In -->
        <button @click="handleGoogleSignIn" :disabled="loading"
          class="w-full flex items-center justify-center gap-3 bg-white hover:bg-gray-50 text-gray-800 font-semibold py-3.5 px-6 rounded-2xl shadow-lg hover:shadow-xl transition-all duration-300 hover:-translate-y-0.5 active:translate-y-0 mb-6 disabled:opacity-60 disabled:cursor-not-allowed group">
          <svg class="w-5 h-5 flex-shrink-0" viewBox="0 0 48 48">
            <path fill="#4285F4" d="M24 9.5c3.54 0 6.7 1.22 9.19 3.23l6.87-6.87C35.64 2.36 30.13 0 24 0 14.82 0 6.73 5.8 2.69 14.09l7.99 6.21C12.13 13.13 17.57 9.5 24 9.5z"/>
            <path fill="#34A853" d="M46.1 24.5c0-1.64-.15-3.22-.42-4.74H24v9.01h12.42c-.54 2.9-2.18 5.36-4.65 7.01l7.19 5.6C43.98 37.13 46.1 31.3 46.1 24.5z"/>
            <path fill="#FBBC05" d="M10.68 28.3a14.5 14.5 0 010-8.6l-7.99-6.21A24.01 24.01 0 000 24c0 3.77.9 7.34 2.69 10.91l7.99-6.21z"/>
            <path fill="#EA4335" d="M24 48c6.13 0 11.64-2.03 15.53-5.53l-7.19-5.6c-2.01 1.35-4.59 2.13-8.34 2.13-6.43 0-11.87-3.63-13.32-8.8l-7.99 6.21C6.73 42.2 14.82 48 24 48z"/>
          </svg>
          <span>Continue with Google</span>
        </button>

        <!-- Divider -->
        <div class="flex items-center gap-4 mb-6">
          <div class="flex-1 h-px bg-white/10"></div>
          <span class="text-white/30 text-sm">or sign in with email</span>
          <div class="flex-1 h-px bg-white/10"></div>
        </div>

        <!-- Email/Password form -->
        <form @submit.prevent="handleLogin" class="space-y-4">
          <div class="group">
            <label class="block text-sm font-medium text-white/60 mb-1.5">Email address</label>
            <div class="relative">
              <span class="absolute left-4 top-1/2 -translate-y-1/2 material-icons text-white/30 text-lg group-focus-within:text-[#7091E6] transition-colors">email</span>
              <input v-model="email" type="email" required autocomplete="email"
                placeholder="you@example.com"
                class="w-full bg-white/5 border border-white/10 text-white placeholder-white/25 rounded-2xl pl-11 pr-4 py-3.5 focus:outline-none focus:border-[#7091E6] focus:bg-white/8 focus:ring-2 focus:ring-[#7091E6]/20 transition-all duration-300" />
            </div>
          </div>

          <div class="group">
            <label class="block text-sm font-medium text-white/60 mb-1.5">Password</label>
            <div class="relative">
              <span class="absolute left-4 top-1/2 -translate-y-1/2 material-icons text-white/30 text-lg group-focus-within:text-[#7091E6] transition-colors">lock</span>
              <input v-model="password" :type="showPassword ? 'text' : 'password'" required
                placeholder="••••••••"
                class="w-full bg-white/5 border border-white/10 text-white placeholder-white/25 rounded-2xl pl-11 pr-12 py-3.5 focus:outline-none focus:border-[#7091E6] focus:bg-white/8 focus:ring-2 focus:ring-[#7091E6]/20 transition-all duration-300" />
              <button type="button" @click="showPassword = !showPassword"
                class="absolute right-4 top-1/2 -translate-y-1/2 text-white/30 hover:text-white/60 transition-colors">
                <span class="material-icons text-lg">{{ showPassword ? 'visibility_off' : 'visibility' }}</span>
              </button>
            </div>
          </div>

          <!-- Error message -->
          <div v-if="errorMsg" class="flex items-center gap-2 bg-red-500/10 border border-red-500/20 rounded-xl px-4 py-3">
            <span class="material-icons text-red-400 text-sm">error_outline</span>
            <p class="text-red-400 text-sm">{{ errorMsg }}</p>
          </div>

          <button type="submit" :disabled="loading"
            class="w-full bg-gradient-to-r from-[#3D52A0] to-[#7091E6] hover:from-[#4a5fb8] hover:to-[#8aa3f0] text-white font-bold py-3.5 px-6 rounded-2xl shadow-lg hover:shadow-[#3D52A0]/40 hover:shadow-xl transition-all duration-300 hover:-translate-y-0.5 active:translate-y-0 disabled:opacity-60 disabled:cursor-not-allowed flex items-center justify-center gap-2">
            <span v-if="loading" class="material-icons text-lg animate-spin">refresh</span>
            <span>{{ loading ? 'Signing in...' : 'Sign In' }}</span>
          </button>
        </form>

        <!-- Footer links -->
        <div class="mt-6 flex flex-col gap-3">
          <p class="text-center text-white/40 text-sm">
            Don't have an account?
            <router-link to="/register" class="text-[#7091E6] hover:text-[#8aa3f0] font-semibold transition-colors ml-1">Create one</router-link>
          </p>
          <div class="h-px bg-white/10"></div>
          <button @click="goToEmergencyLogin"
            class="w-full flex items-center justify-center gap-2 bg-white/5 hover:bg-white/10 border border-white/10 hover:border-white/20 text-white/70 hover:text-white font-medium py-3 px-6 rounded-2xl transition-all duration-300">
            <span class="material-icons text-lg text-red-400">emergency</span>
            Emergency Contact Login
          </button>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue';
import { useRouter } from 'vue-router';
import { auth } from '../firebase/config';
import { signInWithEmailAndPassword, GoogleAuthProvider, signInWithPopup } from 'firebase/auth';

const router = useRouter();
const email = ref('');
const password = ref('');
const loading = ref(false);
const errorMsg = ref('');
const showPassword = ref(false);

const features = [
  { icon: 'shield', label: 'Crash Detection' },
  { icon: 'local_bar', label: 'Alcohol Monitoring' },
  { icon: 'gps_fixed', label: 'Live GPS Tracking' },
  { icon: 'power_settings_new', label: 'Engine Control' },
  { icon: 'notifications_active', label: 'Instant Alerts' },
  { icon: 'security', label: 'Anti-Theft' },
];

const handleLogin = async () => {
  loading.value = true;
  errorMsg.value = '';
  try {
    await signInWithEmailAndPassword(auth, email.value, password.value);
    router.push('/dashboard');
  } catch (error) {
    errorMsg.value = 'Invalid email or password. Please try again.';
  } finally {
    loading.value = false;
  }
};

const handleGoogleSignIn = async () => {
  loading.value = true;
  errorMsg.value = '';
  try {
    const provider = new GoogleAuthProvider();
    const result = await signInWithPopup(auth, provider);
    const isNewUser = result._tokenResponse?.isNewUser ?? false;
    router.push(isNewUser ? '/device-setup?onboarding=true' : '/dashboard');
  } catch (error) {
    errorMsg.value = error.message || 'Google sign-in failed.';
  } finally {
    loading.value = false;
  }
};

const goToEmergencyLogin = () => router.push('/emergency-login');
</script>
