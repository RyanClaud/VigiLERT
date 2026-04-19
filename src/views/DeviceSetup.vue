<template>
  <div class="min-h-screen flex flex-col bg-gradient-to-br from-[#EDE8F5] via-[#f5f3f7] to-[#e8e4f0] pb-20">

    <!-- ===== ONBOARDING WELCOME BANNER (only shown for new users) ===== -->
    <div v-if="isOnboarding"
      class="relative overflow-hidden bg-gradient-to-r from-[#3D52A0] via-[#5571c6] to-[#7091E6] px-4 md:px-8 py-10 shadow-2xl">
      <div class="absolute inset-0 overflow-hidden">
        <div class="absolute -top-24 -left-24 w-96 h-96 bg-white/10 rounded-full blur-3xl animate-pulse"></div>
        <div class="absolute -bottom-24 -right-24 w-96 h-96 bg-white/10 rounded-full blur-3xl animate-pulse" style="animation-delay:1s"></div>
      </div>
      <div class="relative max-w-3xl mx-auto text-center">
        <div class="inline-flex items-center justify-center w-20 h-20 bg-white/20 rounded-full mb-5 shadow-xl">
          <span class="material-icons text-5xl text-white">shield</span>
        </div>
        <h1 class="text-3xl md:text-4xl font-bold text-white mb-3 drop-shadow-lg">
          Welcome to VIGILERT! 🎉
        </h1>
        <p class="text-white/85 text-base md:text-lg max-w-xl mx-auto leading-relaxed">
          Your account is ready. Now let's connect your motorcycle and helmet hardware so the safety system can protect you.
        </p>
        <div class="flex flex-wrap justify-center gap-3 mt-6">
          <div class="bg-white/15 backdrop-blur-sm border border-white/25 rounded-xl px-4 py-2 flex items-center gap-2">
            <span class="material-icons text-green-300 text-lg">check_circle</span>
            <span class="text-white text-sm font-medium">Account Created</span>
          </div>
          <div class="bg-white/15 backdrop-blur-sm border border-white/25 rounded-xl px-4 py-2 flex items-center gap-2">
            <span class="material-icons text-yellow-300 text-lg">radio_button_unchecked</span>
            <span class="text-white text-sm font-medium">Connect Devices</span>
          </div>
          <div class="bg-white/15 backdrop-blur-sm border border-white/25 rounded-xl px-4 py-2 flex items-center gap-2">
            <span class="material-icons text-white/40 text-lg">radio_button_unchecked</span>
            <span class="text-white/60 text-sm font-medium">Start Riding Safely</span>
          </div>
        </div>
      </div>
    </div>

    <!-- ===== REGULAR HEADER (settings mode) ===== -->
    <div v-else
      class="relative overflow-hidden bg-gradient-to-r from-[#3D52A0] via-[#5571c6] to-[#7091E6] py-8 px-4 md:px-8 shadow-2xl">
      <div class="absolute inset-0 overflow-hidden">
        <div class="absolute -top-24 -left-24 w-96 h-96 bg-white/10 rounded-full blur-3xl animate-pulse"></div>
      </div>
      <div class="relative max-w-3xl mx-auto flex items-center gap-4">
        <button @click="$router.back()" class="text-white/80 hover:text-white transition-colors">
          <span class="material-icons text-3xl">arrow_back</span>
        </button>
        <div>
          <h1 class="text-2xl md:text-3xl font-bold text-white drop-shadow-lg">Device Setup</h1>
          <p class="text-white/80 text-sm mt-1">Connect your VIGILERT hardware to your account</p>
        </div>
      </div>
    </div>

    <div class="max-w-3xl mx-auto w-full px-4 py-6 space-y-5">

      <!-- ===== WHAT YOU NEED (onboarding only) ===== -->
      <div v-if="isOnboarding" class="bg-white rounded-2xl shadow-lg p-6 border-l-4 border-[#7091E6]">
        <h2 class="text-base font-bold text-[#3D52A0] mb-3 flex items-center gap-2">
          <span class="material-icons text-lg">inventory_2</span>
          What you need
        </h2>
        <div class="grid grid-cols-1 sm:grid-cols-2 gap-3">
          <div class="flex items-center gap-3 bg-blue-50 rounded-xl p-3">
            <span class="material-icons text-[#3D52A0] text-2xl">two_wheeler</span>
            <div>
              <p class="font-semibold text-gray-800 text-sm">Motorcycle Module</p>
              <p class="text-gray-500 text-xs">ESP32 installed on your motorcycle</p>
            </div>
          </div>
          <div class="flex items-center gap-3 bg-blue-50 rounded-xl p-3">
            <span class="material-icons text-[#3D52A0] text-2xl">sports_motorsports</span>
            <div>
              <p class="font-semibold text-gray-800 text-sm">Helmet Module</p>
              <p class="text-gray-500 text-xs">ESP32 installed in your helmet</p>
            </div>
          </div>
          <div class="flex items-center gap-3 bg-green-50 rounded-xl p-3">
            <span class="material-icons text-green-600 text-2xl">wifi</span>
            <div>
              <p class="font-semibold text-gray-800 text-sm">Your WiFi Network</p>
              <p class="text-gray-500 text-xs">SSID and password ready</p>
            </div>
          </div>
          <div class="flex items-center gap-3 bg-purple-50 rounded-xl p-3">
            <span class="material-icons text-purple-600 text-2xl">smartphone</span>
            <div>
              <p class="font-semibold text-gray-800 text-sm">This Phone</p>
              <p class="text-gray-500 text-xs">To connect to the device hotspot</p>
            </div>
          </div>
        </div>
        <div class="mt-4 bg-amber-50 border border-amber-200 rounded-xl p-3 flex gap-2">
          <span class="material-icons text-amber-500 flex-shrink-0 text-lg">info</span>
          <p class="text-amber-800 text-xs leading-relaxed">
            Don't have the hardware yet? You can <button @click="skipToApp" class="underline font-semibold">skip this step</button> and come back later from the Devices menu.
          </p>
        </div>
      </div>

      <!-- ===== YOUR USER ID ===== -->
      <div class="bg-white rounded-2xl shadow-lg p-6">
        <h2 class="text-base font-bold text-[#3D52A0] mb-1 flex items-center gap-2">
          <span class="material-icons text-lg">badge</span>
          Your User ID
        </h2>
        <p class="text-sm text-gray-500 mb-4">
          You'll paste this into the device setup page. Tap to copy it now.
        </p>

        <div
          @click="copyUID"
          class="flex items-center gap-3 bg-[#f0f4ff] border-2 border-[#c7d2fe] rounded-xl px-4 py-3 cursor-pointer hover:bg-[#e0e8ff] active:scale-98 transition-all select-all group"
          title="Tap to copy"
        >
          <span class="material-icons text-[#3D52A0] text-xl">fingerprint</span>
          <span class="font-mono text-sm text-[#1e3a8a] break-all flex-1">{{ authStore.user?.uid ?? 'Not logged in' }}</span>
          <span :class="['material-icons text-xl transition-all', copied ? 'text-green-500' : 'text-[#3D52A0] group-hover:scale-110']">
            {{ copied ? 'check_circle' : 'content_copy' }}
          </span>
        </div>

        <div v-if="copied" class="mt-2 flex items-center gap-1 text-green-600 text-xs font-medium">
          <span class="material-icons text-sm">check</span> Copied! Now paste it into the device setup page.
        </div>
      </div>

      <!-- ===== STEP-BY-STEP GUIDE ===== -->
      <div class="bg-white rounded-2xl shadow-lg p-6">
        <h2 class="text-base font-bold text-[#3D52A0] mb-5 flex items-center gap-2">
          <span class="material-icons text-lg">list_alt</span>
          How to connect a device
        </h2>

        <ol class="relative border-l-2 border-[#c7d2fe] ml-3 space-y-6">
          <li v-for="(step, i) in setupSteps" :key="i" class="ml-6">
            <div class="absolute -left-3.5 flex items-center justify-center w-7 h-7 rounded-full bg-gradient-to-br from-[#3D52A0] to-[#7091E6] text-white font-bold text-xs shadow">
              {{ i + 1 }}
            </div>
            <p class="font-semibold text-gray-800 text-sm">{{ step.title }}</p>
            <p class="text-gray-500 text-xs mt-1 leading-relaxed">{{ step.detail }}</p>
            <div v-if="step.code" class="mt-2 bg-gray-900 rounded-lg px-3 py-2 inline-flex items-center gap-2">
              <span class="font-mono text-green-400 text-xs">{{ step.code }}</span>
            </div>
            <div v-if="step.image" class="mt-2 bg-blue-50 border border-blue-200 rounded-xl p-3 flex items-center gap-2">
              <span class="material-icons text-blue-500 text-lg">{{ step.image }}</span>
              <span class="text-blue-700 text-xs">{{ step.imageLabel }}</span>
            </div>
          </li>
        </ol>

        <div class="mt-6 bg-amber-50 border border-amber-200 rounded-xl p-4 flex gap-3">
          <span class="material-icons text-amber-500 flex-shrink-0">wifi_off</span>
          <p class="text-amber-800 text-xs leading-relaxed">
            <strong>Important:</strong> When connecting to <strong>VIGILERT-SETUP-XXXX</strong>, your phone temporarily loses internet. That's normal — the device creates its own local network just for setup.
          </p>
        </div>
      </div>

      <!-- ===== REGISTERED DEVICES ===== -->
      <div class="bg-white rounded-2xl shadow-lg p-6">
        <div class="flex items-center justify-between mb-4">
          <h2 class="text-base font-bold text-[#3D52A0] flex items-center gap-2">
            <span class="material-icons text-lg">devices</span>
            Connected Devices
            <span v-if="devices.length > 0"
              class="bg-green-500 text-white text-xs px-2 py-0.5 rounded-full font-bold">
              {{ devices.length }}
            </span>
          </h2>
          <button @click="loadDevices" class="text-[#3D52A0] hover:text-[#5571c6] transition-colors" title="Refresh">
            <span class="material-icons text-xl" :class="{ 'animate-spin': loadingDevices }">refresh</span>
          </button>
        </div>

        <!-- Loading -->
        <div v-if="loadingDevices" class="flex items-center justify-center py-8 text-gray-400">
          <span class="material-icons animate-spin mr-2">sync</span>
          <span class="text-sm">Checking for devices...</span>
        </div>

        <!-- No devices yet -->
        <div v-else-if="devices.length === 0" class="text-center py-8">
          <div class="w-16 h-16 bg-gray-100 rounded-full flex items-center justify-center mx-auto mb-3">
            <span class="material-icons text-3xl text-gray-400">devices_other</span>
          </div>
          <p class="font-semibold text-gray-600 text-sm">No devices connected yet</p>
          <p class="text-gray-400 text-xs mt-1 max-w-xs mx-auto">
            Follow the steps above to connect your motorcycle and helmet modules. They'll appear here automatically once connected.
          </p>
          <div class="mt-4 flex items-center justify-center gap-2 text-xs text-gray-400">
            <span class="material-icons text-sm animate-pulse text-blue-400">wifi_tethering</span>
            <span>Waiting for devices to connect...</span>
          </div>
        </div>

        <!-- Device list -->
        <ul v-else class="space-y-3">
          <li v-for="device in devices" :key="device.key"
            class="flex items-center gap-4 p-4 rounded-xl border border-gray-100 hover:bg-gray-50 transition-colors">
            <div :class="['w-12 h-12 rounded-full flex items-center justify-center flex-shrink-0 shadow-sm',
                          device.online ? 'bg-green-100' : 'bg-gray-100']">
              <span :class="['material-icons text-2xl', device.online ? 'text-green-600' : 'text-gray-400']">
                {{ device.key === 'motorcycle' ? 'two_wheeler' : 'sports_motorsports' }}
              </span>
            </div>
            <div class="flex-1 min-w-0">
              <p class="font-semibold text-gray-800 text-sm capitalize">{{ device.key }} Module</p>
              <div class="flex items-center gap-2 mt-1">
                <span :class="['inline-flex items-center gap-1 text-xs font-medium px-2 py-0.5 rounded-full',
                               device.online ? 'bg-green-100 text-green-700' : 'bg-gray-100 text-gray-500']">
                  <span class="w-1.5 h-1.5 rounded-full" :class="device.online ? 'bg-green-500 animate-pulse' : 'bg-gray-400'"></span>
                  {{ device.online ? 'Online' : 'Offline' }}
                </span>
                <span class="text-xs text-gray-400">{{ device.lastSeen }}</span>
              </div>
            </div>
            <button @click="removeDevice(device.key)"
              class="text-red-400 hover:text-red-600 transition-colors flex-shrink-0 p-1 rounded-lg hover:bg-red-50"
              title="Remove device">
              <span class="material-icons text-xl">delete_outline</span>
            </button>
          </li>
        </ul>

        <!-- Both devices connected — success state -->
        <div v-if="devices.length >= 2 && devices.every(d => d.online)"
          class="mt-4 bg-green-50 border border-green-200 rounded-xl p-4 flex items-center gap-3">
          <span class="material-icons text-green-500 text-2xl">verified</span>
          <div>
            <p class="font-bold text-green-800 text-sm">All devices connected!</p>
            <p class="text-green-600 text-xs mt-0.5">Your VIGILERT system is fully operational.</p>
          </div>
        </div>
      </div>

      <!-- ===== FACTORY RESET ===== -->
      <div class="bg-white rounded-2xl shadow-lg p-6">
        <h2 class="text-sm font-bold text-red-500 mb-3 flex items-center gap-2">
          <span class="material-icons text-lg">restart_alt</span>
          Reset a Device
        </h2>
        <p class="text-xs text-gray-500 mb-3 leading-relaxed">
          To re-configure a device (e.g. changed WiFi password), connect it via USB, open Arduino Serial Monitor at 115200 baud, and type:
        </p>
        <div class="bg-gray-900 rounded-xl px-4 py-3 flex items-center gap-3">
          <span class="font-mono text-green-400 text-sm flex-1">RESET DEVICE</span>
          <button @click="copyResetCmd" class="text-gray-400 hover:text-white transition-colors p-1" title="Copy">
            <span class="material-icons text-lg">{{ resetCopied ? 'check' : 'content_copy' }}</span>
          </button>
        </div>
        <p class="text-xs text-gray-400 mt-2">The device clears its saved settings and restarts in setup mode.</p>
      </div>

      <!-- ===== ONBOARDING: Go to Dashboard button ===== -->
      <div v-if="isOnboarding" class="pb-4">
        <button @click="goToDashboard"
          :class="['w-full py-4 rounded-2xl font-bold text-base transition-all duration-300 shadow-lg flex items-center justify-center gap-3',
                   devices.length > 0
                     ? 'bg-gradient-to-r from-[#3D52A0] to-[#7091E6] text-white hover:opacity-90 hover:scale-[1.01]'
                     : 'bg-white border-2 border-[#3D52A0] text-[#3D52A0] hover:bg-[#f0f4ff]']">
          <span class="material-icons text-xl">{{ devices.length > 0 ? 'dashboard' : 'skip_next' }}</span>
          {{ devices.length > 0 ? 'Go to Dashboard' : 'Skip for now — Go to Dashboard' }}
        </button>
        <p v-if="devices.length === 0" class="text-center text-xs text-gray-400 mt-2">
          You can always connect devices later from the Devices menu
        </p>
      </div>

    </div>
  </div>
</template>

<script setup>
import { ref, computed, onMounted, onUnmounted } from 'vue'
import { useRouter, useRoute } from 'vue-router'
import { useAuthStore } from '../stores/auth'
import { database } from '../firebase/config'
import { ref as dbRef, onValue, remove } from 'firebase/database'

const authStore = useAuthStore()
const router = useRouter()
const route = useRoute()

// Onboarding mode — triggered when ?onboarding=true is in the URL
const isOnboarding = computed(() => route.query.onboarding === 'true')

// -- Navigation -----------------------------------------------------------
function skipToApp() { router.push('/dashboard') }
function goToDashboard() { router.push('/dashboard') }

// -- Copy UID -------------------------------------------------------------
const copied = ref(false)
async function copyUID() {
  const uid = authStore.user?.uid
  if (!uid) return
  try {
    await navigator.clipboard.writeText(uid)
  } catch {
    const el = document.createElement('textarea')
    el.value = uid
    document.body.appendChild(el)
    el.select()
    document.execCommand('copy')
    document.body.removeChild(el)
  }
  copied.value = true
  setTimeout(() => { copied.value = false }, 3000)
}

// -- Copy reset command ---------------------------------------------------
const resetCopied = ref(false)
async function copyResetCmd() {
  try { await navigator.clipboard.writeText('RESET DEVICE') } catch { /* ignore */ }
  resetCopied.value = true
  setTimeout(() => { resetCopied.value = false }, 2000)
}

// -- Setup steps ----------------------------------------------------------
const setupSteps = [
  {
    title: 'Power on your VIGILERT device',
    detail: 'Connect the ESP32 module to power. The onboard LED will blink rapidly while it starts up.'
  },
  {
    title: 'Connect your phone to the VIGILERT WiFi hotspot',
    detail: 'Go to your phone\'s WiFi settings. You\'ll see a network named "VIGILERT-SETUP-XXXX" (last 4 characters are unique to your device). Connect to it.',
    code: 'Password: vigilert123',
    image: 'wifi',
    imageLabel: 'The device creates its own temporary WiFi network for setup'
  },
  {
    title: 'Open the setup page',
    detail: 'A setup page should pop up automatically (captive portal). If it doesn\'t appear, open your browser and go to:',
    code: '192.168.4.1'
  },
  {
    title: 'Fill in the setup form',
    detail: 'Enter your home WiFi name and password. Then paste your User ID (copied from the box above). Select the device type (Motorcycle or Helmet). Tap Save & Connect.'
  },
  {
    title: 'Device reboots and connects — done!',
    detail: 'The device saves your settings and reboots. Reconnect your phone to your normal WiFi. The device will appear in the Connected Devices list below within 60 seconds.'
  }
]

// -- Registered devices ---------------------------------------------------
const devices = ref([])
const loadingDevices = ref(true)
let unsubscribe = null

function formatLastSeen(ts) {
  if (!ts) return 'Never'
  const d = Date.now() - ts
  if (d < 60000)   return 'Just now'
  if (d < 3600000) return `${Math.floor(d / 60000)}m ago`
  if (d < 86400000) return `${Math.floor(d / 3600000)}h ago`
  return `${Math.floor(d / 86400000)}d ago`
}

function loadDevices() {
  const uid = authStore.user?.uid
  if (!uid) { loadingDevices.value = false; return }
  loadingDevices.value = true
  if (unsubscribe) unsubscribe()
  unsubscribe = onValue(dbRef(database, `helmet_public/${uid}/devices`), (snap) => {
    loadingDevices.value = false
    if (!snap.exists()) { devices.value = []; return }
    const data = snap.val()
    devices.value = Object.entries(data).map(([key, val]) => {
      const hb = val.lastHeartbeat || val.timestamp || 0
      const online = val.status === 'On' && (Date.now() - hb < 10000)
      return { key, online, lastSeen: formatLastSeen(hb) }
    })
  }, () => { loadingDevices.value = false })
}

async function removeDevice(key) {
  const uid = authStore.user?.uid
  if (!uid || !confirm(`Remove the ${key} module from your account?`)) return
  try {
    await remove(dbRef(database, `helmet_public/${uid}/devices/${key}`))
  } catch {
    alert('Could not remove device. Please try again.')
  }
}

onMounted(() => loadDevices())
onUnmounted(() => { if (unsubscribe) unsubscribe() })
</script>
