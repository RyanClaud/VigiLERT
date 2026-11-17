<template>
  <div class="min-h-screen relative flex flex-col justify-between bg-[#061B2B] text-white overflow-x-hidden select-none font-['Inter',sans-serif]">
    
    <!-- Background Futuristic Effects -->
    <div class="fixed inset-0 pointer-events-none z-0 overflow-hidden">
      <!-- Dark Blue Radial Center Gradient -->
      <div class="absolute inset-0 bg-[radial-gradient(circle_at_30%_35%,#0B2540_0%,#061B2B_75%)]"></div>
      
      <!-- Faint Cyber Grid Pattern -->
      <div class="absolute inset-0 opacity-[0.04]"
        style="background-image: linear-gradient(rgba(112,227,74,0.3) 1px, transparent 1px), linear-gradient(90deg, rgba(112,227,74,0.3) 1px, transparent 1px); background-size: 48px 48px;">
      </div>

      <!-- Top-Left Neon Cyber Stripes Accent -->
      <svg class="absolute -top-6 -left-6 w-56 h-56 sm:w-72 sm:h-72 opacity-90" viewBox="0 0 200 200" fill="none">
        <path d="M-20 80 L80 -20 L110 -20 L-20 110 Z" fill="url(#reg-cyber-grad-1)"/>
        <path d="M-20 130 L130 -20 L150 -20 L-20 150 Z" fill="url(#reg-cyber-grad-2)"/>
        <path d="M-20 170 L170 -20 L185 -20 L-20 185 Z" fill="#00B85C" fill-opacity="0.6"/>
        <defs>
          <linearGradient id="reg-cyber-grad-1" x1="0" y1="0" x2="1" y2="1">
            <stop offset="0%" stop-color="#70E34A"/>
            <stop offset="100%" stop-color="#00B85C"/>
          </linearGradient>
          <linearGradient id="reg-cyber-grad-2" x1="0" y1="0" x2="1" y2="1">
            <stop offset="0%" stop-color="#00B85C"/>
            <stop offset="100%" stop-color="#061B2B"/>
          </linearGradient>
        </defs>
      </svg>

      <!-- Bottom-Left Neon Cyber Stripes Accent -->
      <svg class="absolute -bottom-10 -left-10 w-64 h-64 sm:w-80 sm:h-80 opacity-90" viewBox="0 0 220 220" fill="none">
        <path d="M-20 100 L100 220 L125 220 L-20 75 Z" fill="url(#reg-cyber-grad-3)"/>
        <path d="M-20 50 L150 220 L170 220 L-20 30 Z" fill="#00B85C" fill-opacity="0.5"/>
        <defs>
          <linearGradient id="reg-cyber-grad-3" x1="0" y1="1" x2="1" y2="0">
            <stop offset="0%" stop-color="#70E34A"/>
            <stop offset="100%" stop-color="#00B85C"/>
          </linearGradient>
        </defs>
      </svg>

      <!-- Top-Right Ambient Glow -->
      <div class="absolute -top-24 -right-24 w-96 h-96 bg-[#00B85C]/10 rounded-full blur-3xl"></div>
      
      <!-- Bottom-Right Cyber Corner Accent -->
      <svg class="absolute -bottom-8 -right-8 w-48 h-48 opacity-75" viewBox="0 0 160 160" fill="none">
        <path d="M180 60 L60 180 L80 180 L180 80 Z" fill="#00B85C" fill-opacity="0.4"/>
        <path d="M180 100 L100 180 L120 180 L180 120 Z" fill="#70E34A" fill-opacity="0.3"/>
      </svg>
    </div>

    <!-- Main Content Container -->
    <div class="relative z-10 w-full max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-6 sm:py-8 lg:py-10 flex-1 flex items-center">
      <div class="w-full grid grid-cols-1 lg:grid-cols-12 gap-8 lg:gap-8 xl:gap-12 items-center">

        <!-- ========================================== -->
        <!-- LEFT PANEL: BRANDING & SYSTEM OVERVIEW     -->
        <!-- ========================================== -->
        <div class="lg:col-span-7 xl:col-span-7 flex flex-col justify-center space-y-6 lg:space-y-7">
          
          <!-- Branding Header -->
          <div class="flex items-center gap-4 sm:gap-5">
            <div class="relative flex-shrink-0 w-16 h-16 sm:w-20 sm:h-20 flex items-center justify-center">
              <img 
                src="/images/shield-logo.png" 
                alt="VigiLERT Shield Logo" 
                class="w-full h-full object-contain drop-shadow-[0_0_15px_rgba(0,184,92,0.4)]"
              />
            </div>

            <div class="flex flex-col">
              <div class="flex items-center flex-wrap gap-1 sm:gap-2">
                <h1 class="text-3xl sm:text-4xl lg:text-5xl font-extrabold tracking-tight text-white">
                  Vigi<span class="text-[#00B85C]">LERT</span>
                </h1>
                <svg class="h-6 sm:h-8 w-16 sm:w-24 text-[#00B85C] inline-block filter drop-shadow-[0_0_8px_rgba(0,184,92,0.6)]" viewBox="0 0 100 36" fill="none">
                  <path d="M0 18 H28 L36 4 L44 32 L52 10 L60 24 L68 18 H100" stroke="currentColor" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"/>
                </svg>
              </div>
              <p class="text-base sm:text-lg font-medium italic text-white/95 tracking-wide mt-0.5">
                Ride Smart. Stay Safe.
              </p>
            </div>
          </div>

          <!-- Description -->
          <p class="text-sm sm:text-base text-slate-300 leading-relaxed max-w-xl font-normal">
            Join the intelligent motorcycle safety revolution. Connect your smart helmet and motorcycle modules to experience real-time protection, accident response, and anti-theft security on every ride.
          </p>

          <!-- 3 Steps Visual Card -->
          <div class="bg-[#0B2540]/60 border border-[#00B85C]/25 backdrop-blur-md rounded-2xl p-5 sm:p-6 space-y-4 max-w-xl shadow-[0_0_20px_rgba(0,184,92,0.1)]">
            <div v-for="step in steps" :key="step.title" class="flex items-start gap-3.5 group">
              <div class="w-8 h-8 rounded-full bg-gradient-to-br from-[#00B85C] to-[#087A45] flex items-center justify-center flex-shrink-0 text-white text-xs font-extrabold shadow-md shadow-[#00B85C]/20 group-hover:scale-110 transition-transform">
                {{ step.num }}
              </div>
              <div>
                <p class="text-white font-semibold text-sm group-hover:text-[#70E34A] transition-colors">{{ step.title }}</p>
                <p class="text-slate-300 text-xs mt-0.5 leading-relaxed">{{ step.desc }}</p>
              </div>
            </div>
          </div>

          <!-- Bottom Branding Statement with Green Brush Underline -->
          <div class="pt-1">
            <p class="text-xl sm:text-2xl font-normal italic text-white tracking-wide">
              Your Safety. Our Priority.
            </p>
            <svg class="w-40 sm:w-52 h-3.5 text-[#00B85C] mt-1" viewBox="0 0 200 14" fill="none">
              <path d="M2 10 C 60 14, 140 12, 198 2" stroke="currentColor" stroke-width="3.5" stroke-linecap="round"/>
            </svg>
          </div>

        </div>

        <!-- ========================================== -->
        <!-- RIGHT PANEL: REGISTRATION CARD             -->
        <!-- ========================================== -->
        <div class="lg:col-span-5 xl:col-span-5 flex justify-center lg:justify-end">
          <div class="w-full max-w-[440px] bg-white rounded-[32px] p-7 sm:p-9 shadow-[0_25px_60px_-15px_rgba(0,0,0,0.55)] border border-gray-100 text-[#061B2B] relative z-20">
            
            <!-- Card Header -->
            <div class="flex flex-col items-center text-center">
              <div class="w-14 h-14 sm:w-16 sm:h-16 flex items-center justify-center mb-1">
                <img 
                  src="/images/shield-logo-card.png" 
                  alt="VigiLERT Shield Logo" 
                  class="w-full h-full object-contain"
                />
              </div>

              <h2 class="text-2xl sm:text-3xl font-extrabold text-[#061B2B] tracking-tight">
                Create Account
              </h2>
              <p class="text-sm text-[#64748B] mt-1 font-medium">
                Start protecting your ride today
              </p>

              <!-- Heartbeat Pulse Divider -->
              <div class="flex items-center justify-center w-full my-3.5">
                <svg class="w-48 h-3.5 text-[#00B85C]" viewBox="0 0 160 16" fill="none">
                  <path d="M0 8 H60 L66 1 L72 15 L78 3 L84 12 L90 8 H160" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
                </svg>
              </div>
            </div>

            <!-- Google Sign Up Button -->
            <button 
              type="button" 
              @click="handleGoogleSignUp" 
              :disabled="loading"
              class="w-full flex items-center justify-center gap-3 bg-white hover:bg-gray-50/80 active:bg-gray-100 border border-gray-200/90 text-[#1E293B] font-semibold py-3 px-5 rounded-full shadow-sm hover:shadow transition-all duration-200 disabled:opacity-60 disabled:cursor-not-allowed group text-sm"
            >
              <svg class="w-4 h-4 flex-shrink-0" viewBox="0 0 48 48">
                <path fill="#4285F4" d="M24 9.5c3.54 0 6.7 1.22 9.19 3.23l6.87-6.87C35.64 2.36 30.13 0 24 0 14.82 0 6.73 5.8 2.69 14.09l7.99 6.21C12.13 13.13 17.57 9.5 24 9.5z"/>
                <path fill="#34A853" d="M46.1 24.5c0-1.64-.15-3.22-.42-4.74H24v9.01h12.42c-.54 2.9-2.18 5.36-4.65 7.01l7.19 5.6C43.98 37.13 46.1 31.3 46.1 24.5z"/>
                <path fill="#FBBC05" d="M10.68 28.3a14.5 14.5 0 010-8.6l-7.99-6.21A24.01 24.01 0 000 24c0 3.77.9 7.34 2.69 10.91l7.99-6.21z"/>
                <path fill="#EA4335" d="M24 48c6.13 0 11.64-2.03 15.53-5.53l-7.19-5.6c-2.01 1.35-4.59 2.13-8.34 2.13-6.43 0-11.87-3.63-13.32-8.8l-7.99 6.21C6.73 42.2 14.82 48 24 48z"/>
              </svg>
              <span>Sign up with Google</span>
            </button>

            <!-- Divider -->
            <div class="flex items-center gap-3 my-3.5">
              <div class="flex-1 h-px bg-gray-200"></div>
              <span class="text-xs text-[#64748B] font-normal whitespace-nowrap">or register with email</span>
              <div class="flex-1 h-px bg-gray-200"></div>
            </div>

            <!-- Form -->
            <form @submit.prevent="handleRegister" class="space-y-3">
              
              <!-- Full Name -->
              <div class="relative">
                <span class="absolute left-3.5 top-1/2 -translate-y-1/2 text-gray-400 pointer-events-none">
                  <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M19 21v-2a4 4 0 0 0-4-4H9a4 4 0 0 0-4 4v2"/>
                    <circle cx="12" cy="7" r="4"/>
                  </svg>
                </span>
                <input 
                  v-model="name" 
                  type="text" 
                  required 
                  placeholder="Full Name"
                  class="w-full bg-white border border-gray-200 text-[#061B2B] placeholder:text-gray-400 rounded-xl pl-10 pr-4 py-2.5 text-sm focus:outline-none focus:border-[#00B85C] focus:ring-2 focus:ring-[#00B85C]/20 transition-all duration-200" 
                />
              </div>

              <!-- Email -->
              <div class="relative">
                <span class="absolute left-3.5 top-1/2 -translate-y-1/2 text-gray-400 pointer-events-none">
                  <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <rect width="20" height="16" x="2" y="4" rx="2"/>
                    <path d="m22 7-8.97 5.7a1.94 1.94 0 0 1-2.06 0L2 7"/>
                  </svg>
                </span>
                <input 
                  v-model="email" 
                  type="email" 
                  required 
                  autocomplete="email"
                  placeholder="Email address"
                  class="w-full bg-white border border-gray-200 text-[#061B2B] placeholder:text-gray-400 rounded-xl pl-10 pr-4 py-2.5 text-sm focus:outline-none focus:border-[#00B85C] focus:ring-2 focus:ring-[#00B85C]/20 transition-all duration-200" 
                />
              </div>

              <!-- Password -->
              <div class="relative">
                <span class="absolute left-3.5 top-1/2 -translate-y-1/2 text-gray-400 pointer-events-none">
                  <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <rect width="18" height="11" x="3" y="11" rx="2" ry="2"/>
                    <path d="M7 11V7a5 5 0 0 1 10 0v4"/>
                  </svg>
                </span>
                <input 
                  v-model="password" 
                  :type="showPassword ? 'text' : 'password'" 
                  required
                  placeholder="Password (min. 8 characters)"
                  class="w-full bg-white border border-gray-200 text-[#061B2B] placeholder:text-gray-400 rounded-xl pl-10 pr-11 py-2.5 text-sm focus:outline-none focus:border-[#00B85C] focus:ring-2 focus:ring-[#00B85C]/20 transition-all duration-200" 
                />
                <button 
                  type="button" 
                  @click="showPassword = !showPassword"
                  class="absolute right-3.5 top-1/2 -translate-y-1/2 text-gray-400 hover:text-gray-600 transition-colors p-1"
                >
                  <svg v-if="!showPassword" class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M2 12s3-7 10-7 10 7 10 7-3 7-10 7-10-7-10-7Z"/>
                    <circle cx="12" cy="12" r="3"/>
                  </svg>
                  <svg v-else class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M9.88 9.88a3 3 0 1 0 4.24 4.24"/>
                    <path d="M10.73 5.08A10.43 10.43 0 0 1 12 5c7 0 10 7 10 7a13.16 13.16 0 0 1-1.67 2.68"/>
                    <path d="M6.61 6.61A13.526 13.526 0 0 0 2 12s3 7 10 7a9.74 9.74 0 0 0 5.39-1.61"/>
                    <line x1="2" x2="22" y1="2" y2="22"/>
                  </svg>
                </button>
              </div>

              <!-- Confirm Password -->
              <div class="relative">
                <span class="absolute left-3.5 top-1/2 -translate-y-1/2 text-gray-400 pointer-events-none">
                  <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <rect width="18" height="11" x="3" y="11" rx="2" ry="2"/>
                    <path d="M7 11V7a5 5 0 0 1 10 0v4"/>
                  </svg>
                </span>
                <input 
                  v-model="confirmPassword" 
                  :type="showPassword ? 'text' : 'password'" 
                  required
                  placeholder="Confirm password"
                  class="w-full bg-white border border-gray-200 text-[#061B2B] placeholder:text-gray-400 rounded-xl pl-10 pr-4 py-2.5 text-sm focus:outline-none focus:border-[#00B85C] focus:ring-2 focus:ring-[#00B85C]/20 transition-all duration-200" 
                />
              </div>

              <!-- Error Alert -->
              <div v-if="errorMsg" class="flex items-center gap-2 bg-red-50 border border-red-200 rounded-xl p-2.5 text-red-600 text-xs animate-shake">
                <svg class="w-4 h-4 flex-shrink-0 text-red-500" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <circle cx="12" cy="12" r="10"/>
                  <line x1="12" x2="12" y1="8" y2="12"/>
                  <line x1="12" x2="12.01" y1="16" y2="16"/>
                </svg>
                <p class="font-medium">{{ errorMsg }}</p>
              </div>

              <!-- Submit -->
              <button 
                type="submit" 
                :disabled="loading"
                class="w-full bg-gradient-to-r from-[#00B85C] to-[#087A45] hover:from-[#00c965] hover:to-[#0a8c4f] active:scale-[0.99] text-white font-bold py-3 px-6 rounded-xl shadow-lg shadow-[#00B85C]/25 hover:shadow-xl hover:shadow-[#00B85C]/35 transition-all duration-200 disabled:opacity-60 disabled:cursor-not-allowed flex items-center justify-center gap-2 text-sm sm:text-base group mt-1"
              >
                <svg v-if="loading" class="animate-spin -ml-1 mr-2 h-4 w-4 text-white" fill="none" viewBox="0 0 24 24">
                  <circle class="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" stroke-width="4"></circle>
                  <path class="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.291A7.962 7.962 0 014 12H0c0 3.042 1.135 5.824 3 7.938l3-2.647z"></path>
                </svg>
                <span>{{ loading ? 'Creating account...' : 'Create Account' }}</span>
                <svg v-if="!loading" class="w-4 h-4 transform group-hover:translate-x-1 transition-transform" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M5 12h14"/>
                  <path d="m12 5 7 7-7 7"/>
                </svg>
              </button>
            </form>

            <!-- Sign in link -->
            <p class="text-center text-xs sm:text-sm text-[#64748B] mt-4">
              Already have an account?
              <router-link to="/login" class="text-[#00B85C] hover:text-[#087A45] font-bold transition-colors ml-1">
                Sign in
              </router-link>
            </p>

          </div>
        </div>

      </div>
    </div>

    <!-- Bottom Footer Bar -->
    <footer class="relative z-10 w-full border-t border-white/5 bg-[#061B2B]/90 backdrop-blur-md py-3 px-4 sm:px-6 lg:px-8 text-xs text-slate-400">
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
        <div class="flex items-center gap-1.5 text-[#00B85C] font-medium">
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
import { auth, database } from '../firebase/config';
import { createUserWithEmailAndPassword, GoogleAuthProvider, signInWithPopup } from 'firebase/auth';
import { ref as dbRef, set } from 'firebase/database';

const router = useRouter();
const name = ref('');
const email = ref('');
const password = ref('');
const confirmPassword = ref('');
const loading = ref(false);
const errorMsg = ref('');
const showPassword = ref(false);

const steps = [
  { num: 1, title: 'Create your account', desc: 'Sign up with email or Google in seconds' },
  { num: 2, title: 'Connect your devices', desc: 'Link your motorcycle and helmet modules' },
  { num: 3, title: 'Ride with confidence', desc: 'Real-time safety monitoring on every trip' },
];

const handleRegister = async () => {
  errorMsg.value = '';
  if (password.value !== confirmPassword.value) {
    errorMsg.value = 'Passwords do not match.';
    return;
  }
  if (password.value.length < 8) {
    errorMsg.value = 'Password must be at least 8 characters.';
    return;
  }
  loading.value = true;
  try {
    const { user } = await createUserWithEmailAndPassword(auth, email.value, password.value);
    await set(dbRef(database, `users/${user.uid}`), {
      name: name.value,
      email: email.value,
      createdAt: Date.now()
    });
    router.push('/device-setup?onboarding=true');
  } catch (error) {
    errorMsg.value = error.message || 'Registration failed. Please try again.';
  } finally {
    loading.value = false;
  }
};

const handleGoogleSignUp = async () => {
  loading.value = true;
  errorMsg.value = '';
  try {
    const provider = new GoogleAuthProvider();
    const result = await signInWithPopup(auth, provider);
    const isNewUser = result._tokenResponse?.isNewUser ?? false;
    router.push(isNewUser ? '/device-setup?onboarding=true' : '/dashboard');
  } catch (error) {
    if (error.code !== 'auth/popup-closed-by-user') {
      errorMsg.value = error.message || 'Google sign-up failed.';
    }
  } finally {
    loading.value = false;
  }
};
</script>

<style scoped>
@keyframes shake {
  0%, 100% { transform: translateX(0); }
  20%, 60% { transform: translateX(-4px); }
  40%, 80% { transform: translateX(4px); }
}

.animate-shake {
  animation: shake 0.4s ease-in-out;
}
</style>
