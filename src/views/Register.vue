<template>
  <div class="min-h-screen flex items-center justify-center bg-gradient-to-br from-[#EDE8F5] to-[#ADBBD4] py-12 px-4 sm:px-6 lg:px-8">
    <div class="max-w-md w-full space-y-6 bg-white shadow-2xl rounded-3xl p-8 md:p-10 backdrop-blur-sm">
      <div class="flex flex-col items-center">
        <div class="bg-gradient-to-br from-[#7091E6] to-[#3D52A0] rounded-full p-4 mb-4">
          <span class="material-icons text-white text-4xl">person_add</span>
        </div>
        <h2 class="text-3xl font-extrabold text-[#3D52A0] mb-2">Create your account</h2>
        <p class="text-[#8697C4] text-sm">Join the VIGILERT safety network</p>
      </div>
      <form class="mt-6 space-y-4" @submit.prevent="handleRegister">
        <div class="space-y-4">
          <div class="relative">
            <label for="name" class="block text-sm font-medium text-[#3D52A0] mb-2">Full Name</label>
            <span class="absolute inset-y-0 left-0 pl-4 flex items-center pointer-events-none mt-8">
              <span class="material-icons text-[#7091E6] text-xl">person</span>
            </span>
            <input
              id="name"
              v-model="name"
              type="text"
              required
              class="appearance-none rounded-xl relative block w-full pl-12 pr-4 py-3 border-2 border-gray-200 placeholder-gray-400 text-gray-900 focus:outline-none focus:ring-2 focus:ring-[#7091E6] focus:border-[#7091E6] transition-all duration-300"
              placeholder="Enter your full name"
            />
          </div>
          <div class="relative">
            <label for="email" class="block text-sm font-medium text-[#3D52A0] mb-2">Email Address</label>
            <span class="absolute inset-y-0 left-0 pl-4 flex items-center pointer-events-none mt-8">
              <span class="material-icons text-[#7091E6] text-xl">email</span>
            </span>
            <input
              id="email"
              v-model="email"
              type="email"
              required
              class="appearance-none rounded-xl relative block w-full pl-12 pr-4 py-3 border-2 border-gray-200 placeholder-gray-400 text-gray-900 focus:outline-none focus:ring-2 focus:ring-[#7091E6] focus:border-[#7091E6] transition-all duration-300"
              placeholder="your.email@example.com"
            />
          </div>
          <div class="relative">
            <label for="password" class="block text-sm font-medium text-[#3D52A0] mb-2">Password</label>
            <span class="absolute inset-y-0 left-0 pl-4 flex items-center pointer-events-none mt-8">
              <span class="material-icons text-[#7091E6] text-xl">lock</span>
            </span>
            <input
              id="password"
              v-model="password"
              type="password"
              required
              class="appearance-none rounded-xl relative block w-full pl-12 pr-4 py-3 border-2 border-gray-200 placeholder-gray-400 text-gray-900 focus:outline-none focus:ring-2 focus:ring-[#7091E6] focus:border-[#7091E6] transition-all duration-300"
              placeholder="Create a strong password"
            />
          </div>
          <div class="relative">
            <label for="confirmPassword" class="block text-sm font-medium text-[#3D52A0] mb-2">Confirm Password</label>
            <span class="absolute inset-y-0 left-0 pl-4 flex items-center pointer-events-none mt-8">
              <span class="material-icons text-[#7091E6] text-xl">lock_outline</span>
            </span>
            <input
              id="confirmPassword"
              v-model="confirmPassword"
              type="password"
              required
              class="appearance-none rounded-xl relative block w-full pl-12 pr-4 py-3 border-2 border-gray-200 placeholder-gray-400 text-gray-900 focus:outline-none focus:ring-2 focus:ring-[#7091E6] focus:border-[#7091E6] transition-all duration-300"
              placeholder="Re-enter your password"
            />
          </div>
        </div>
        <div class="pt-2">
          <button
            type="submit"
            class="group relative w-full flex items-center justify-center gap-2 py-3 px-4 border border-transparent text-base font-semibold rounded-xl text-white bg-gradient-to-r from-[#7091E6] to-[#3D52A0] hover:from-[#3D52A0] hover:to-[#7091E6] focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-[#7091E6] transition-all duration-300 transform hover:scale-105 shadow-lg"
          >
            <span class="material-icons text-xl">check_circle</span>
            Create Account
          </button>
        </div>
        <div class="text-sm text-center pt-2">
          <router-link to="/login" class="font-medium text-[#3D52A0] hover:text-[#7091E6] transition-colors duration-300">
            Already have an account? <span class="underline">Sign in</span>
          </router-link>
        </div>
      </form>
      <div class="flex items-center my-5">
        <div class="flex-grow border-t-2 border-gray-200"></div>
        <span class="mx-4 text-gray-400 text-sm font-medium">or</span>
        <div class="flex-grow border-t-2 border-gray-200"></div>
      </div>
      <div class="flex justify-center">
        <button
          type="button"
          class="flex items-center gap-2 px-6 py-3 bg-gray-100 border-2 border-gray-200 rounded-xl shadow-sm text-sm font-semibold text-gray-400 opacity-60 cursor-not-allowed"
          disabled
        >
          <svg class="h-5 w-5" viewBox="0 0 48 48"><g><path fill="#4285F4" d="M24 9.5c3.54 0 6.7 1.22 9.19 3.23l6.87-6.87C35.64 2.36 30.13 0 24 0 14.82 0 6.73 5.8 2.69 14.09l7.99 6.21C12.13 13.13 17.57 9.5 24 9.5z"/><path fill="#34A853" d="M46.1 24.5c0-1.64-.15-3.22-.42-4.74H24v9.01h12.42c-.54 2.9-2.18 5.36-4.65 7.01l7.19 5.6C43.98 37.13 46.1 31.3 46.1 24.5z"/><path fill="#FBBC05" d="M10.68 28.3a14.5 14.5 0 010-8.6l-7.99-6.21A24.01 24.01 0 000 24c0 3.77.9 7.34 2.69 10.91l7.99-6.21z"/><path fill="#EA4335" d="M24 48c6.13 0 11.64-2.03 15.53-5.53l-7.19-5.6c-2.01 1.35-4.59 2.13-8.34 2.13-6.43 0-11.87-3.63-13.32-8.8l-7.99 6.21C6.73 42.2 14.82 48 24 48z"/><path fill="none" d="M0 0h48v48H0z"/></g></svg>
          Sign up with Google (Email only)
        </button>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue';
import { useRouter } from 'vue-router';
import { auth, database } from '../firebase/config';
import { createUserWithEmailAndPassword } from 'firebase/auth';
import { ref as dbRef, set } from 'firebase/database';

const router = useRouter();
const name = ref('');
const email = ref('');
const password = ref('');
const confirmPassword = ref('');

const handleRegister = async () => {
  if (password.value !== confirmPassword.value) {
    alert('Passwords do not match');
    return;
  }

  try {
    const userCredential = await createUserWithEmailAndPassword(auth, email.value, password.value);
    const user = userCredential.user;

    // Create user profile in database
    await set(dbRef(database, `users/${user.uid}`), {
      name: name.value,
      email: email.value,
      createdAt: Date.now()
    });

    router.push('/dashboard');
  } catch (error) {
    console.error('Error registering:', error);
    alert(error.message || 'Failed to register. Please try again.');
  }
};
</script> 