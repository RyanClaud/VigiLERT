<template>
  <div class="min-h-screen flex items-center justify-center bg-[#EDE8F5] py-12 px-4 sm:px-6 lg:px-8">
    <div class="max-w-md w-full space-y-8 bg-white/90 shadow-xl rounded-2xl p-8">
      <div class="flex flex-col items-center">
       <!--     <img src="https://img.icons8.com/color/96/000000/motorcycle-helmet.png" alt="VIGILERT Logo" class="h-16 w-16 mb-2" /> -->
        <h2 class="text-3xl font-extrabold text-[#3D52A0] mb-1">Sign in to <span class="text-[#7091E6]">VIGILERT</span></h2>
        <p class="text-[#8697C4] text-sm">Your safety, our priority</p>
      </div>
          <div class="flex justify-center">
        <button
          @click="handleGoogleSignIn"
          type="button"
          class="flex items-center px-4 py-2 bg-white border border-[#ADBBD4] rounded-md shadow-sm text-sm font-semibold text-[#3D52A0] hover:bg-[#EDE8F5] transition"
        >
          <svg class="h-5 w-5 mr-2" viewBox="0 0 48 48"><g><path fill="#4285F4" d="M24 9.5c3.54 0 6.7 1.22 9.19 3.23l6.87-6.87C35.64 2.36 30.13 0 24 0 14.82 0 6.73 5.8 2.69 14.09l7.99 6.21C12.13 13.13 17.57 9.5 24 9.5z"/><path fill="#34A853" d="M46.1 24.5c0-1.64-.15-3.22-.42-4.74H24v9.01h12.42c-.54 2.9-2.18 5.36-4.65 7.01l7.19 5.6C43.98 37.13 46.1 31.3 46.1 24.5z"/><path fill="#FBBC05" d="M10.68 28.3a14.5 14.5 0 010-8.6l-7.99-6.21A24.01 24.01 0 000 24c0 3.77.9 7.34 2.69 10.91l7.99-6.21z"/><path fill="#EA4335" d="M24 48c6.13 0 11.64-2.03 15.53-5.53l-7.19-5.6c-2.01 1.35-4.59 2.13-8.34 2.13-6.43 0-11.87-3.63-13.32-8.8l-7.99 6.21C6.73 42.2 14.82 48 24 48z"/><path fill="none" d="M0 0h48v48H0z"/></g></svg>
          Sign in with Google
        </button>
      </div>

      <form class="mt-8 space-y-6" @submit.prevent="handleLogin">
        <div class="rounded-md shadow-sm -space-y-px">
          <div class="relative">
            <span class="absolute inset-y-0 left-0 pl-3 flex items-center pointer-events-none">
              <!-- Heroicon: Envelope -->
              <svg class="h-5 w-5 text-blue-400" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M16 12l-4-4-4 4m8 0l-4 4-4-4" /></svg>
            </span>
            <input
              id="email"
              v-model="email"
              type="email"
              autocomplete="email"
              required
              class="appearance-none rounded-t-md relative block w-full px-10 py-2 border border-gray-300 placeholder-gray-400 text-gray-900 focus:outline-none focus:ring-blue-500 focus:border-blue-500 focus:z-10 sm:text-sm"
              placeholder="Email address"
            />
          </div>
          <div class="relative">
            <span class="absolute inset-y-0 left-0 pl-3 flex items-center pointer-events-none">
              <!-- Heroicon: Lock Closed -->
              <svg class="h-5 w-5 text-blue-400" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 11c1.104 0 2-.896 2-2V7a2 2 0 10-4 0v2c0 1.104.896 2 2 2zm6 2v6a2 2 0 01-2 2H8a2 2 0 01-2-2v-6a2 2 0 012-2h8a2 2 0 012 2z" /></svg>
            </span>
            <input
              id="password"
              v-model="password"
              type="password"
              required
              class="appearance-none rounded-b-md relative block w-full px-10 py-2 border border-gray-300 placeholder-gray-400 text-gray-900 focus:outline-none focus:ring-blue-500 focus:border-blue-500 focus:z-10 sm:text-sm"
              placeholder="Password"
            />
          </div>
        </div>
        <div>
          <button
          type="submit"
               :disabled="loading"
               class="group relative w-full flex justify-center py-2 px-4 border border-transparent text-sm font-semibold rounded-md text-white bg-gradient-to-r from-[#7091E6] to-[#3D52A0] hover:from-[#3D52A0] hover:to-[#7091E6] focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-[#ADBBD4] transition disabled:opacity-60"
          >
            <svg class="h-5 w-5 text-white mr-2" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M5 13l4 4L19 7" /></svg>
            Sign in
          </button>
        </div>
        <div class="text-sm text-center">
          <router-link to="/register" class="font-medium text-[#3D52A0] hover:text-[#7091E6]">
            Don't have an account? Register
          </router-link>
        </div>
      </form>
      <div class="flex items-center my-4">
        <div class="flex-grow border-t border-[#ADBBD4]"></div>
        <span class="mx-2 text-[#ADBBD4] text-xs">or</span>
        <div class="flex-grow border-t border-[#ADBBD4]"></div>
      </div>
      <div class="flex flex-col gap-2">
        <button
          @click="goToEmergencyLogin"
          type="button"
          class="w-full py-2 bg-[#3D52A0] text-white rounded-lg font-semibold hover:bg-[#7091E6] transition"
        >
          Emergency Contact Login
        </button>
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

const handleLogin = async () => {
  loading.value = true;
  try {
    await signInWithEmailAndPassword(auth, email.value, password.value);
    router.push('/dashboard');
  } catch (error) {
    console.error('Error logging in:', error);
    alert('Failed to login. Please check your credentials.');
  } finally {
    loading.value = false;
  }
};

const handleGoogleSignIn = async () => {
  try {
    const provider = new GoogleAuthProvider();
    await signInWithPopup(auth, provider);
    router.push('/dashboard');
  } catch (error) {
    alert(error.message || 'Google sign-in failed.');
  }
};

const goToEmergencyLogin = () => {
  router.push('/emergency-login');
};
</script> 