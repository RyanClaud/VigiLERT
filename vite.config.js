import { defineConfig } from 'vite';
import vue from '@vitejs/plugin-vue';
import { VitePWA } from 'vite-plugin-pwa';

export default defineConfig({
  plugins: [
    vue({
      template: {
        compilerOptions: {
          // Optimize component rendering
          hoistStatic: true,
          cacheHandlers: true,
        },
      },
    }),
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
    // Optimize chunk size
    chunkSizeWarningLimit: 1000,
    rollupOptions: {
      output: {
        manualChunks: {
          // Separate Vue runtime and core libraries
          'vue-core': ['vue', 'vue-router', 'pinia'],
          // Firebase
          'firebase': ['firebase/app', 'firebase/auth', 'firebase/database'],
          // Leaflet maps library
          'leaflet': ['leaflet'],
        },
      },
    },
    // Enable minification
    minify: 'terser',
    terserOptions: {
      compress: {
        drop_console: true,
        drop_debugger: true,
      },
    },
  },
  // Optimize dependencies
  optimizeDeps: {
    include: ['vue', 'vue-router', 'pinia', 'firebase/app', 'firebase/auth', 'firebase/database'],
  },
  // Server configuration for faster HMR
  server: {
    hmr: {
      overlay: false,
    },
  },
});