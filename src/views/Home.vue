<template>
  <div class="min-h-screen flex items-center justify-center bg-[#EDE8F5]">
    <div class="text-center">
      <h1 class="text-4xl font-bold text-[#3D52A0] mb-4">Welcome to VIGILERT</h1>
      <p class="text-lg text-[#7091E6] mb-8">Redirecting to dashboard...</p>
    </div>
  </div>
</template>

<script setup>
import { onMounted } from 'vue';
import { useRouter } from 'vue-router';

const router = useRouter();

onMounted(() => {
  router.push('/dashboard');
});
</script> 