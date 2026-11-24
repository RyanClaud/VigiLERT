<template>
  <div class="min-h-screen flex flex-col bg-[#061B2B] text-white pb-20 font-['Inter',sans-serif]">

    <!-- ===== ONBOARDING WELCOME BANNER (only shown for new users) ===== -->
    <div v-if="isOnboarding"
      class="relative overflow-hidden bg-[#0B2540]/90 border-b border-[#00B85C]/20 px-4 md:px-8 py-10 shadow-2xl">
      <div class="absolute inset-0 overflow-hidden pointer-events-none">
        <div class="absolute -top-24 -left-24 w-96 h-96 bg-[#00B85C]/10 rounded-full blur-3xl animate-pulse"></div>
        <div class="absolute -bottom-24 -right-24 w-96 h-96 bg-[#00B85C]/10 rounded-full blur-3xl animate-pulse" style="animation-delay:1s"></div>
      </div>
      <div class="relative max-w-3xl mx-auto text-center">
        <div class="inline-flex items-center justify-center w-20 h-20 bg-[#00B85C]/20 border border-[#00B85C]/40 rounded-full mb-5 shadow-xl">
          <img src="/vigilert-logo.png" alt="VigiLERT" class="w-12 h-12 object-contain filter drop-shadow" />
        </div>
        <h1 class="text-3xl md:text-4xl font-black text-white mb-3 tracking-tight">
          Welcome to Vigi<span class="text-[#00B85C]">LERT</span>! 🎉
        </h1>
        <p class="text-slate-300 text-sm md:text-base max-w-xl mx-auto leading-relaxed">
          Your account is ready. Connect your smart motorcycle and helmet modules to enable real-time accident detection and anti-theft protection.
        </p>
        <div class="flex flex-wrap justify-center gap-3 mt-6">
          <div class="bg-[#061B2B]/70 backdrop-blur-sm border border-[#00B85C]/30 rounded-xl px-4 py-2 flex items-center gap-2">
            <span class="material-icons text-[#00B85C] text-lg">check_circle</span>
            <span class="text-white text-xs font-semibold">Account Created</span>
          </div>
          <div class="bg-[#061B2B]/70 backdrop-blur-sm border border-amber-400/30 rounded-xl px-4 py-2 flex items-center gap-2">
            <span class="material-icons text-amber-400 text-lg">radio_button_unchecked</span>
            <span class="text-white text-xs font-semibold">Connect Devices</span>
          </div>
          <div class="bg-[#061B2B]/70 backdrop-blur-sm border border-white/10 rounded-xl px-4 py-2 flex items-center gap-2">
            <span class="material-icons text-white/40 text-lg">radio_button_unchecked</span>
            <span class="text-slate-400 text-xs font-semibold">Start Riding Safely</span>
          </div>
        </div>
      </div>
    </div>

    <!-- ===== REGULAR HEADER (settings mode) ===== -->
    <div v-else
      class="relative overflow-hidden bg-[#0B2540]/80 border-b border-[#00B85C]/20 py-7 px-4 md:px-8 shadow-2xl">
      <div class="relative max-w-3xl mx-auto flex items-center gap-4">
        <button @click="$router.back()" class="text-slate-300 hover:text-white transition-colors p-2 rounded-xl bg-white/5 border border-white/10">
          <span class="material-icons text-2xl">arrow_back</span>
        </button>
        <div>
          <h1 class="text-2xl md:text-3xl font-extrabold text-white tracking-tight">Hardware Device Pairing</h1>
          <p class="text-slate-300 text-xs mt-1">Connect your ESP32 motorcycle and helmet modules to your account</p>
        </div>
      </div>
    </div>

    <div class="max-w-3xl mx-auto w-full px-4 py-6 space-y-5">

      <!-- ===== WHAT YOU NEED (onboarding only) ===== -->
      <div v-if="isOnboarding" class="bg-[#0B2540]/80 rounded-2xl shadow-xl p-6 border border-white/10 backdrop-blur-md">
        <h2 class="text-base font-bold text-white mb-3 flex items-center gap-2">
          <span class="material-icons text-lg text-[#00B85C]">inventory_2</span>
          What you need
        </h2>
        <div class="grid grid-cols-1 sm:grid-cols-2 gap-3">
          <div class="flex items-center gap-3 bg-[#061B2B]/70 rounded-xl p-3 border border-white/5">
            <span class="material-icons text-[#00B85C] text-2xl">two_wheeler</span>
            <div>
              <p class="font-semibold text-white text-sm">Motorcycle Module</p>
              <p class="text-slate-400 text-xs">ESP32 installed on your motorcycle</p>
            </div>
          </div>
          <div class="flex items-center gap-3 bg-[#061B2B]/70 rounded-xl p-3 border border-white/5">
            <span class="material-icons text-[#00B85C] text-2xl">sports_motorsports</span>
            <div>
              <p class="font-semibold text-white text-sm">Helmet Module</p>
              <p class="text-slate-400 text-xs">ESP32 installed in your helmet</p>
            </div>
          </div>
          <div class="flex items-center gap-3 bg-[#061B2B]/70 rounded-xl p-3 border border-white/5">
            <span class="material-icons text-green-400 text-2xl">wifi</span>
            <div>
              <p class="font-semibold text-white text-sm">Your WiFi Network</p>
              <p class="text-slate-400 text-xs">SSID and password ready</p>
            </div>
          </div>
          <div class="flex items-center gap-3 bg-[#061B2B]/70 rounded-xl p-3 border border-white/5">
            <span class="material-icons text-purple-400 text-2xl">smartphone</span>
            <div>
              <p class="font-semibold text-white text-sm">This Device</p>
              <p class="text-slate-400 text-xs">To connect to the module hotspot</p>
            </div>
          </div>
        </div>
        <div class="mt-4 bg-amber-500/10 border border-amber-400/20 rounded-xl p-3 flex gap-2">
          <span class="material-icons text-amber-400 flex-shrink-0 text-lg">info</span>
          <p class="text-amber-200 text-xs leading-relaxed">
            Don't have the hardware ready right now? You can <button @click="skipToApp" class="underline font-bold text-white hover:text-amber-100">skip this step</button> and return anytime from the Setup menu.
          </p>
        </div>
      </div>

      <!-- ===== YOUR USER ID ===== -->
      <div class="bg-[#0B2540]/80 rounded-2xl shadow-xl p-6 border border-white/10 backdrop-blur-md">
        <h2 class="text-base font-bold text-white mb-1 flex items-center gap-2">
          <span class="material-icons text-lg text-[#00B85C]">badge</span>
          Your Unique Firebase User ID (UID)
        </h2>
        <p class="text-xs text-slate-400 mb-4">
          This ID links your hardware to your account in Firebase. Tap below to copy it.
        </p>

        <div
          @click="copyUID"
          class="flex items-center gap-3 bg-[#061B2B] border-2 border-emerald-500/30 rounded-xl px-4 py-3 cursor-pointer hover:border-[#00B85C] active:scale-98 transition-all select-all group"
          title="Tap to copy"
        >
          <span class="material-icons text-[#00B85C] text-xl">fingerprint</span>
          <span class="font-mono text-sm text-emerald-300 break-all flex-1">{{ authStore.user?.uid ?? 'Not logged in' }}</span>
          <span :class="['material-icons text-xl transition-all', copied ? 'text-green-400' : 'text-slate-400 group-hover:text-white']">
            {{ copied ? 'check_circle' : 'content_copy' }}
          </span>
        </div>

        <div v-if="copied" class="mt-2 flex items-center gap-1 text-[#00B85C] text-xs font-semibold">
          <span class="material-icons text-sm">check</span> Copied to clipboard! Ready to paste into sketch or hotspot setup.
        </div>
      </div>

      <!-- ===== STEP-BY-STEP GUIDE ===== -->
      <div class="bg-[#0B2540]/80 rounded-2xl shadow-xl p-6 border border-white/10 backdrop-blur-md">
        <h2 class="text-base font-bold text-white mb-5 flex items-center gap-2">
          <span class="material-icons text-lg text-[#00B85C]">list_alt</span>
          How to connect a device
        </h2>

        <ol class="relative border-l-2 border-[#00B85C]/30 ml-3 space-y-6">
          <li v-for="(step, i) in setupSteps" :key="i" class="ml-6">
            <div class="absolute -left-3.5 flex items-center justify-center w-7 h-7 rounded-full bg-gradient-to-br from-[#00B85C] to-[#087A45] text-white font-bold text-xs shadow-md shadow-[#00B85C]/20">
              {{ i + 1 }}
            </div>
            <p class="font-bold text-white text-sm">{{ step.title }}</p>
            <p class="text-slate-400 text-xs mt-1 leading-relaxed">{{ step.detail }}</p>
            <div v-if="step.code" class="mt-2 bg-[#061B2B] border border-white/10 rounded-lg px-3 py-2 inline-flex items-center gap-2">
              <span class="font-mono text-[#00B85C] text-xs">{{ step.code }}</span>
            </div>
            <div v-if="step.image" class="mt-2 bg-blue-500/10 border border-blue-400/20 rounded-xl p-3 flex items-center gap-2">
              <span class="material-icons text-blue-400 text-lg">{{ step.image }}</span>
              <span class="text-blue-300 text-xs">{{ step.imageLabel }}</span>
            </div>
          </li>
        </ol>

        <div class="mt-6 bg-amber-500/10 border border-amber-400/20 rounded-xl p-4 flex gap-3">
          <span class="material-icons text-amber-400 flex-shrink-0">wifi_off</span>
          <p class="text-amber-200 text-xs leading-relaxed">
            <strong>Important:</strong> When connecting to <strong>VIGILERT-SETUP-XXXX</strong>, your phone temporarily loses external internet. That's normal — the device creates its own local hotspot just for configuration.
          </p>
        </div>
      </div>

      <!-- ===== REGISTERED DEVICES ===== -->
      <div class="bg-[#0B2540]/80 rounded-2xl shadow-xl p-6 border border-white/10 backdrop-blur-md">
        <div class="flex items-center justify-between mb-4">
          <h2 class="text-base font-bold text-white flex items-center gap-2">
            <span class="material-icons text-lg text-[#00B85C]">devices</span>
            Connected Devices
            <span v-if="devices.length > 0"
              class="bg-[#00B85C] text-white text-xs px-2 py-0.5 rounded-full font-bold shadow-sm">
              {{ devices.length }}
            </span>
          </h2>
          <button @click="loadDevices" class="text-slate-300 hover:text-white transition-colors p-1" title="Refresh">
            <span class="material-icons text-xl" :class="{ 'animate-spin': loadingDevices }">refresh</span>
          </button>
        </div>

        <!-- Loading -->
        <div v-if="loadingDevices" class="flex items-center justify-center py-8 text-slate-400">
          <span class="material-icons animate-spin mr-2 text-[#00B85C]">sync</span>
          <span class="text-sm">Checking device heartbeat...</span>
        </div>

        <!-- No devices yet -->
        <div v-else-if="devices.length === 0" class="text-center py-8 bg-[#061B2B]/40 rounded-xl border border-white/5">
          <div class="w-16 h-16 bg-white/5 rounded-full flex items-center justify-center mx-auto mb-3">
            <span class="material-icons text-3xl text-slate-500">devices_other</span>
          </div>
          <p class="font-bold text-white text-sm">No devices connected yet</p>
          <p class="text-slate-400 text-xs mt-1 max-w-xs mx-auto">
            Follow the steps above to provision your motorcycle and helmet modules. They will show up here automatically.
          </p>
          <div class="mt-4 flex items-center justify-center gap-2 text-xs text-slate-400">
            <span class="material-icons text-sm animate-pulse text-[#00B85C]">wifi_tethering</span>
            <span>Listening for module heartbeats...</span>
          </div>
        </div>

        <!-- Device list -->
        <ul v-else class="space-y-3">
          <li v-for="device in devices" :key="device.key"
            class="flex items-center gap-4 p-4 rounded-xl bg-[#061B2B]/75 border border-white/10 hover:border-[#00B85C]/30 transition-colors">
            <div :class="['w-12 h-12 rounded-2xl flex items-center justify-center flex-shrink-0 shadow-sm border',
                          device.online ? 'bg-[#00B85C]/15 border-[#00B85C]/40 text-[#00B85C]' : 'bg-white/5 border-white/10 text-slate-500']">
              <span class="material-icons text-2xl">
                {{ device.key === 'motorcycle' ? 'two_wheeler' : 'sports_motorsports' }}
              </span>
            </div>
            <div class="flex-1 min-w-0">
              <p class="font-bold text-white text-sm capitalize">{{ device.key }} Module</p>
              <div class="flex items-center gap-2 mt-1">
                <span :class="['inline-flex items-center gap-1 text-xs font-semibold px-2.5 py-0.5 rounded-full border',
                               device.online ? 'bg-[#00B85C]/15 text-[#00B85C] border-[#00B85C]/30' : 'bg-white/5 text-slate-400 border-white/10']">
                  <span class="w-1.5 h-1.5 rounded-full" :class="device.online ? 'bg-[#00B85C] animate-pulse' : 'bg-slate-500'"></span>
                  {{ device.online ? 'Online' : 'Offline' }}
                </span>
                <span class="text-xs text-slate-400">{{ device.lastSeen }}</span>
              </div>
            </div>
            <button @click="removeDevice(device.key)"
              class="text-red-400 hover:text-white hover:bg-red-500/20 transition-colors flex-shrink-0 p-2 rounded-xl border border-red-500/20"
              title="Remove device">
              <span class="material-icons text-xl">delete_outline</span>
            </button>
          </li>
        </ul>

        <!-- Both devices connected — success state -->
        <div v-if="devices.length >= 2 && devices.every(d => d.online)"
          class="mt-4 bg-[#00B85C]/15 border border-[#00B85C]/30 rounded-xl p-4 flex items-center gap-3">
          <span class="material-icons text-[#00B85C] text-2xl">verified</span>
          <div>
            <p class="font-bold text-white text-sm">All Modules Paired & Active!</p>
            <p class="text-emerald-300 text-xs mt-0.5">Your VigiLERT safety and anti-theft system is fully armed.</p>
          </div>
        </div>
      </div>

      <!-- ===== FACTORY RESET ===== -->
      <div class="bg-[#0B2540]/80 rounded-2xl shadow-xl p-6 border border-white/10 backdrop-blur-md">
        <h2 class="text-sm font-bold text-red-400 mb-2 flex items-center gap-2">
          <span class="material-icons text-lg">restart_alt</span>
          Hardware Reset
        </h2>
        <p class="text-xs text-slate-400 mb-3 leading-relaxed">
          To re-configure a module (e.g. updating Wi-Fi SSID or password), connect it via USB, open Arduino Serial Monitor at 115200 baud, and type:
        </p>
        <div class="bg-[#061B2B] border border-white/10 rounded-xl px-4 py-3 flex items-center gap-3">
          <span class="font-mono text-[#00B85C] text-sm flex-1 font-bold">RESET DEVICE</span>
          <button @click="copyResetCmd" class="text-slate-400 hover:text-white transition-colors p-1" title="Copy">
            <span class="material-icons text-lg">{{ resetCopied ? 'check' : 'content_copy' }}</span>
          </button>
        </div>
        <p class="text-[11px] text-slate-400 mt-2">The ESP32 clears non-volatile storage (NVS) and restarts in provisioning mode.</p>
      </div>

      <!-- ===== ONBOARDING: Go to Dashboard button ===== -->
      <div v-if="isOnboarding" class="pb-4">
        <button @click="goToDashboard"
          class="w-full py-4 rounded-2xl font-bold text-base transition-all duration-300 shadow-lg shadow-[#00B85C]/25 flex items-center justify-center gap-3 bg-gradient-to-r from-[#00B85C] to-[#087A45] hover:from-[#00c965] hover:to-[#0a8c4f] text-white">
          <span class="material-icons text-xl">{{ devices.length > 0 ? 'dashboard' : 'skip_next' }}</span>
          {{ devices.length > 0 ? 'Go to Dashboard' : 'Skip for now — Go to Dashboard' }}
        </button>
        <p v-if="devices.length === 0" class="text-center text-xs text-slate-400 mt-2">
          You can return to device setup anytime from the navigation bar.
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

const router = useRouter()
const route = useRoute()
const authStore = useAuthStore()

// State
const copied = ref(false)
const resetCopied = ref(false)
const devices = ref([])
const loadingDevices = ref(true)

// Are we in onboarding mode? (came from register)
const isOnboarding = computed(() => route.query.onboarding === 'true')

// Setup steps
const setupSteps = [
  {
    title: 'Power on your device',
    detail: 'Plug in the Motorcycle Module (or turn on your motorcycle) or turn on the Helmet Module. The status LED will start blinking slowly.',
    code: null,
    image: null,
  },
  {
    title: 'Connect to the device hotspot',
    detail: 'On this phone, open WiFi Settings. Look for a network named VIGILERT-SETUP-XXXX (where XXXX is a unique code). Connect to it.',
    code: 'SSID: VIGILERT-SETUP-XXXX',
    image: 'wifi_tethering',
    imageLabel: 'No internet warning is normal',
  },
  {
    title: 'Open the setup page',
    detail: 'Once connected, open your browser and go to 192.168.4.1. If the captive portal opens automatically, use that.',
    code: 'http://192.168.4.1',
    image: null,
  },
  {
    title: 'Enter your WiFi details and User ID',
    detail: 'Select your home WiFi network (or phone mobile hotspot), enter the WiFi password, and paste your User ID (copied above). Tap Save & Connect.',
    code: null,
    image: 'vpn_key',
    imageLabel: 'Requires 2.4 GHz WiFi network',
  },
  {
    title: 'Device connects and appears below',
    detail: 'The device will reboot, connect to your WiFi and Firebase, and show up in the Connected Devices list below within 10-20 seconds.',
    code: null,
    image: 'check_circle',
    imageLabel: 'LED turns solid when connected',
  },
]

// Copy user UID
function copyUID() {
  if (!authStore.user?.uid) return
  navigator.clipboard.writeText(authStore.user.uid).then(() => {
    copied.value = true
    setTimeout(() => { copied.value = false }, 3000)
  })
}

// Copy reset command
function copyResetCmd() {
  navigator.clipboard.writeText('RESET DEVICE').then(() => {
    resetCopied.value = true
    setTimeout(() => { resetCopied.value = false }, 2500)
  })
}

// Format timestamp to "X ago" or time string
function formatLastSeen(ts) {
  if (!ts) return 'Never'
  const diffSec = Math.floor((Date.now() - ts) / 1000)
  if (diffSec < 10) return 'Just now'
  if (diffSec < 60) return `${diffSec}s ago`
  if (diffSec < 3600) return `${Math.floor(diffSec / 60)}m ago`
  return `${Math.floor(diffSec / 3600)}h ago`
}

// Is the timestamp considered "online" (within 15 seconds)?
function isOnline(ts) {
  if (!ts) return false
  return (Date.now() - ts) < 15000
}

let devicesUnsub = null

function loadDevices() {
  const uid = authStore.user?.uid
  if (!uid) {
    loadingDevices.value = false
    return
  }
  loadingDevices.value = true

  const devicesRef = dbRef(database, `helmet_public/${uid}/devices`)
  devicesUnsub = onValue(devicesRef, (snapshot) => {
    loadingDevices.value = false
    const val = snapshot.val()
    if (!val) {
      devices.value = []
      return
    }

    const list = []
    for (const [key, dev] of Object.entries(val)) {
      list.push({
        key, // 'motorcycle' | 'helmet'
        online: isOnline(dev.lastHeartbeat),
        lastSeen: formatLastSeen(dev.lastHeartbeat),
        timestamp: dev.lastHeartbeat || 0,
      })
    }
    devices.value = list
  }, (err) => {
    console.error('Error loading devices:', err)
    loadingDevices.value = false
  })
}

function removeDevice(key) {
  const uid = authStore.user?.uid
  if (!uid) return
  if (!confirm(`Are you sure you want to remove the ${key} module? You will need to reconfigure it.`)) return

  const devRef = dbRef(database, `helmet_public/${uid}/devices/${key}`)
  remove(devRef).catch((err) => console.error('Error removing device:', err))
}

function goToDashboard() {
  router.push('/dashboard')
}

function skipToApp() {
  router.push('/dashboard')
}

onMounted(() => {
  loadDevices()
})

onUnmounted(() => {
  if (devicesUnsub) devicesUnsub()
})
</script>
