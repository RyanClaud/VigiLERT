<template>
  <div class="min-h-screen flex flex-col items-center justify-center bg-[#EDE8F5] py-8">
    <div class="bg-white rounded-xl shadow-lg p-8 w-full max-w-md">
      <h2 class="text-2xl font-bold text-[#3D52A0] mb-4 text-center">Emergency Contact Login</h2>
      <form @submit.prevent="checkPhone">
        <label class="block text-[#3D52A0] mb-2">Mobile Number</label>
        <input
          v-model="phone"
          type="tel"
          autocomplete="tel"
          placeholder="e.g. 09876543210"
          class="w-full px-4 py-2 rounded-lg border border-[#ADBBD4] focus:ring-[#7091E6] focus:border-[#7091E6] mb-4"
          required
        />
        <button
          type="submit"
          :disabled="loading || success"
          class="w-full py-2 bg-[#7091E6] text-white rounded-lg font-semibold hover:bg-[#3D52A0] transition disabled:opacity-60"
        >
          Access Dashboard
        </button>
      </form>
      <div v-if="error" class="text-red-600 mt-2 text-center">{{ error }}</div>
      <div v-if="success" class="text-green-600 mt-2 text-center">Success! Redirecting to dashboard...</div>
      <button
        @click="goBack"
        class="w-full mt-4 py-2 bg-[#f82929] text-[rgb(249,250,252)] rounded-lg font-semibold hover:bg-gray-400 transition"
      >
        Back to User Login
      </button>
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

const normalizePhoneNumber = (phoneNumber) => {
  const cleaned = phoneNumber.replace(/\D/g, ''); 
  if (cleaned.startsWith('0')) {
    return '+63' + cleaned.slice(1); // Mobile number starts with '+63'
  }
  if (!cleaned.startsWith('+')) {
    return '+' + cleaned; // Add '+' if not present
  }
  return cleaned;
};

const checkPhone = async () => {
  error.value = '';
  success.value = false;
  loading.value = true;

  const db = getDatabase();
  const usersRef = dbRef(db, 'users');
  const normalizedInputPhone = normalizePhoneNumber(phone.value);

  console.log('Normalized Input Phone:', normalizedInputPhone);

  try {
    const snapshot = await get(usersRef);
    if (!snapshot.exists()) {
      console.error('No users found in the database.');
      error.value = 'No users found in the database.';
      loading.value = false;
      return;
    }

    let foundUserId = null;

    snapshot.forEach((userSnap) => {
      const userId = userSnap.key;
      const userData = userSnap.val();

      console.log('Checking user:', userId, userData);

      if (userData.emergencyContacts) {
        Object.values(userData.emergencyContacts).forEach((contact) => {
          const normalizedContactPhone = normalizePhoneNumber(contact.phone);
          console.log('Comparing:', normalizedContactPhone, normalizedInputPhone);
          if (normalizedContactPhone === normalizedInputPhone) {
            foundUserId = userId;
          }
        });
      }
    });

    if (foundUserId) {
      console.log('Found user ID:', foundUserId);
      success.value = true;
      setTimeout(() => {
        router.push(`/emergency-dashboard/${foundUserId}`);
      }, 1200);
    } else {
      console.error('No user found for this contact number.');
      error.value = 'No user found for this contact number.';
    }
  } catch (e) {
    console.error('Error while checking phone number:', e);
    error.value = 'An error occurred while checking the phone number. Please try again.';
  } finally {
    loading.value = false;
  }
};

const goBack = () => {
  router.push('/login'); // Navigate back to the Login.vue page
};
</script>