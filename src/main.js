// c:\Users\ryanc\OneDrive\Desktop\VigiLERT\src\main.js

import { createApp } from 'vue';
import { createPinia } from 'pinia';
import App from './App.vue';
import router from './router';
import { useAuthStore } from './stores/auth';
import './index.css';

// 🔹 Leaflet Imports
import 'leaflet/dist/leaflet.css';
import * as L from 'leaflet';
window.L = L; 

// PWA Service Worker
import { registerSW } from 'virtual:pwa-register';

registerSW({
  scope: '/',
  onNeedRefresh() {
    console.log('A new version of the app is available. Please refresh.');
  },
  onOfflineReady() {
    console.log('The app is ready to work offline.');
  },
});

const app = createApp(App);
const pinia = createPinia();
app.use(pinia);
app.use(router);

// Initialize auth store
const authStore = useAuthStore();
authStore.init();

app.mount('#app');