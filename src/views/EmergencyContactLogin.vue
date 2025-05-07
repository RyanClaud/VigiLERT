<template>
  <div class="min-h-screen flex flex-col items-center justify-center bg-[#EDE8F5] py-8">
    <div class="bg-white rounded-xl shadow-lg p-8 w-full max-w-md">
      <h2 class="text-2xl font-bold text-[#3D52A0] mb-4 text-center">Emergency Contact Login</h2>
      <form @submit.prevent="checkPhone">
        <label class="block text-[#3D52A0] mb-2">Mobile Number</label>
        <input v-model="phone" type="tel" placeholder="e.g. +1234567890" class="w-full px-4 py-2 rounded-lg border border-[#ADBBD4] focus:ring-[#7091E6] focus:border-[#7091E6] mb-4" required />
        <button type="submit" :disabled="loading || success" class="w-full py-2 bg-[#7091E6] text-white rounded-lg font-semibold hover:bg-[#3D52A0] transition disabled:opacity-60">Access Dashboard</button>
      </form>
      <div v-if="error" class="text-red-600 mt-2 text-center">{{ error }}</div>
      <div v-if="success" class="text-green-600 mt-2 text-center">Success! Redirecting to dashboard...</div>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue';
import { getDatabase, ref as dbRef, get } from 'firebase/database';
import { useRouter } from 'vue-router';

const phone = ref('');
const error = ref('');
const success = ref(false);
const loading = ref(false);
const router = useRouter();

const checkPhone = async () => {
  error.value = '';
  success.value = false;
  loading.value = true;
  const db = getDatabase();
  const usersRef = dbRef(db, 'users');
  try {
    const snapshot = await get(usersRef);
    let foundUserId = null;
    snapshot.forEach((userSnap) => {
      const userId = userSnap.key;
      const userData = userSnap.val();
      if (userData.emergencyContacts) {
        Object.values(userData.emergencyContacts).forEach((contact) => {
          if (
            contact.phone === phone.value ||
            contact.phone === '+63' + phone.value.replace(/^0/, '') ||
            contact.phone === '0' + phone.value.replace(/^\+63/, '')
          ) {
            foundUserId = userId;
          }
        });
      }
    });
    if (foundUserId) {
      success.value = true;
      setTimeout(() => {
        router.push(`/emergency-dashboard/${foundUserId}`);
      }, 1200);
    } else {
      error.value = 'No user found for this contact number.';
    }
  } catch (e) {
    error.value = e.message;
  } finally {
    loading.value = false;
  }
};
</script> 