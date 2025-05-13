import { defineConfig } from 'vite';
import vue from '@vitejs/plugin-vue';
import { VitePWA } from 'vite-plugin-pwa';

export default defineConfig({
  plugins: [
    vue(),
    VitePWA({
      registerType: 'autoUpdate',
      includeAssets: ['favicon.ico', 'robots.txt', 'apple-touch-icon.png'],
      manifest: {
        name: 'VigiLERT',
        short_name: 'VigiLERT',
        description: 'Smart Helmet Safety System',
        theme_color: '#3D52A0',
        background_color: '#ffffff',
        display: 'standalone',
        icons: [
          {
            src: '/pwa-192x192.png',
            sizes: '192x192',
            type: 'image/png',
          },
          {
            src: '/pwa-512x512.png',
            sizes: '512x512',
            type: 'image/png',
            purpose: 'any maskable',
          },
        ],
      },
    }),
  ],
  build: {
    rollupOptions: {
      output: {
        manualChunks: {
          // Separate Vue runtime and core libraries
          vue: ['vue', 'vue-router'],
          // Leaflet maps library
          leaflet: ['leaflet'],
          // Vendor libraries (add more as needed)
          vendor: ['axios', 'lodash'],
        },
      },
    },
  },
});