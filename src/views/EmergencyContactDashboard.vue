<template>
  <div class="min-h-screen flex flex-col bg-gradient-to-br from-[#0a0f1e] via-[#0f1729] to-[#0a0f1e] pb-20">

    <!-- ===== HEADER ===== -->
    <div class="relative overflow-hidden bg-gradient-to-r from-[#0a0f1e] via-[#1a2a5e] to-[#0a0f1e] py-8 px-4 md:px-8 shadow-2xl border-b border-white/5">
      <div class="absolute inset-0 overflow-hidden">
        <div class="absolute -top-24 -left-24 w-96 h-96 bg-red-900/15 rounded-full blur-3xl animate-pulse"></div>
        <div class="absolute -bottom-24 -right-24 w-96 h-96 bg-[#3D52A0]/15 rounded-full blur-3xl animate-pulse" style="animation-delay:1s"></div>
      </div>
      <div class="absolute inset-0 opacity-3"
        style="background-image: linear-gradient(rgba(255,255,255,.03) 1px, transparent 1px), linear-gradient(90deg, rgba(255,255,255,.03) 1px, transparent 1px); background-size: 40px 40px;"></div>
      <div class="relative max-w-7xl mx-auto">
        <div class="flex flex-col md:flex-row items-center justify-between gap-6">

          <!-- Left: Identity -->
          <div class="flex items-center gap-5">
            <div class="relative">
              <div class="absolute inset-0 bg-gradient-to-r from-red-400 to-orange-400 rounded-full blur-lg opacity-70"></div>
              <div class="relative bg-gradient-to-br from-white/20 to-white/10 backdrop-blur-sm p-4 rounded-full border-4 border-white shadow-2xl">
                <span class="material-icons text-5xl text-white">shield</span>
                <div class="absolute -bottom-1 -right-1 flex items-center gap-1 bg-orange-500 px-2 py-1 rounded-full border-2 border-white shadow-lg">
                  <div class="w-2 h-2 bg-white rounded-full animate-pulse"></div>
                  <span class="text-xs font-bold text-white">Monitoring</span>
                </div>
              </div>
            </div>
            <div>
              <div class="flex items-center gap-2 mb-1">
                <span class="material-icons text-red-300 text-sm animate-pulse">emergency</span>
                <p class="text-xs text-white/80 font-semibold uppercase tracking-widest">Emergency Contact</p>
              </div>
              <h1 class="text-2xl md:text-3xl font-bold text-white drop-shadow-lg">
                Safety Monitor,
                <span class="bg-gradient-to-r from-red-300 to-orange-300 bg-clip-text text-transparent">Guardian</span>
              </h1>
              <div class="flex items-center gap-3 text-white/70 text-sm mt-1">
                <span class="material-icons text-sm">visibility</span>
                <span>Monitoring Rider Safety</span>
                <span class="material-icons text-sm ml-2">schedule</span>
                <span>{{ currentTime }}</span>
              </div>
            </div>
          </div>

          <!-- Right: SOS + Quick Stats -->
          <div class="flex flex-col gap-3 items-end">
            <!-- SOS Button -->
            <button @click="triggerSOS"
              class="relative overflow-hidden bg-gradient-to-r from-red-500 to-red-600 hover:from-red-600 hover:to-red-700 text-white rounded-2xl px-6 py-4 shadow-2xl transition-all duration-300 hover:scale-105 active:scale-95 border-2 border-red-300/50">
              <div class="absolute inset-0 bg-white/10 animate-pulse rounded-2xl"></div>
              <div class="relative flex items-center gap-3">
                <span class="material-icons text-3xl animate-pulse">emergency</span>
                <div class="text-left">
                  <p class="font-bold text-lg leading-tight">Emergency SOS</p>
                  <p class="text-red-200 text-xs">Tap to alert rider</p>
                </div>
              </div>
            </button>

            <!-- Quick Stats Row -->
            <div class="flex gap-3">
              <div class="bg-white/10 backdrop-blur-md border border-white/20 rounded-2xl px-4 py-3 text-center min-w-[80px]">
                <span :class="['material-icons text-2xl', engineRunning ? 'text-green-300' : 'text-gray-400']">power_settings_new</span>
                <p class="text-xs font-bold text-white mt-1">{{ engineRunning ? 'ON' : 'OFF' }}</p>
                <p class="text-xs text-white/60">Engine</p>
              </div>
              <div class="bg-white/10 backdrop-blur-md border border-white/20 rounded-2xl px-4 py-3 text-center min-w-[80px]">
                <span class="material-icons text-2xl text-blue-300">speed</span>
                <p class="text-xs font-bold text-white mt-1">{{ currentSpeed.toFixed(0) }}</p>
                <p class="text-xs text-white/60">km/h</p>
              </div>
              <div class="bg-white/10 backdrop-blur-md border border-white/20 rounded-2xl px-4 py-3 text-center min-w-[80px]">
                <span :class="['material-icons text-2xl', alcoholStatus === 'Safe' ? 'text-green-300' : 'text-red-300 animate-pulse']">local_bar</span>
                <p class="text-xs font-bold text-white mt-1">{{ alcoholStatus }}</p>
                <p class="text-xs text-white/60">Alcohol</p>
              </div>
              <div class="bg-white/10 backdrop-blur-md border border-white/20 rounded-2xl px-4 py-3 text-center min-w-[80px]">
                <span :class="['material-icons text-2xl', alerts.length > 0 ? 'text-yellow-300 animate-pulse' : 'text-white/50']">notifications_active</span>
                <p class="text-xs font-bold text-white mt-1">{{ alerts.length }}</p>
                <p class="text-xs text-white/60">Alerts</p>
              </div>
            </div>

            <!-- Logout -->
            <button @click="logout" class="bg-white/10 hover:bg-white/20 border border-white/30 text-white px-4 py-2 rounded-xl text-sm font-semibold transition-all flex items-center gap-2">
              <span class="material-icons text-sm">logout</span>Logout
            </button>
          </div>
        </div>
      </div>
    </div>

    <!-- ===== CRASH ALERT BANNER ===== -->
    <div v-if="crashDisplayStatus !== 'Stable'"
      class="mx-4 md:mx-8 mt-4 relative overflow-hidden bg-gradient-to-r from-red-600 via-red-500 to-red-600 rounded-2xl shadow-2xl border-4 border-red-300 animate-pulse">
      <div class="relative p-5 flex items-center gap-4">
        <div class="bg-white/20 p-3 rounded-full animate-bounce">
          <span class="material-icons text-4xl text-white">warning</span>
        </div>
        <div class="flex-1">
          <h3 class="text-xl font-bold text-white flex items-center gap-2">
            <span>🚨</span> CRASH DETECTED
          </h3>
          <p class="text-white/90 text-sm mt-1">{{ crashDisplayMessage }} — Check on rider immediately!</p>
          <a v-if="location.lat && location.lng"
            :href="`https://www.google.com/maps?q=${location.lat},${location.lng}`" target="_blank"
            class="inline-flex items-center gap-1 mt-2 bg-white/20 hover:bg-white/30 text-white text-xs px-3 py-1 rounded-lg transition-all">
            <span class="material-icons text-sm">map</span> View Location
          </a>
        </div>
      </div>
    </div>

    <!-- ===== CRASH HISTORY (shown when crashes exist) ===== -->
    <div v-if="crashEvents.length > 0" class="mx-4 md:mx-8 mt-4">
      <div class="relative overflow-hidden bg-red-500/5 backdrop-blur-lg rounded-2xl shadow-xl p-5 border border-red-500/20">
        <h3 class="font-bold text-base text-red-400 mb-4 flex items-center gap-2">
          <span class="material-icons text-lg">place</span>
          Crash Site Locations
          <span class="bg-red-500/20 border border-red-400/30 text-red-400 text-xs px-2 py-0.5 rounded-full ml-1">{{ crashEvents.length }}</span>
        </h3>
        <div class="space-y-3">
          <div v-for="(event, index) in crashEvents" :key="index"
            class="p-4 border border-red-500/20 rounded-xl bg-red-500/5">
            <!-- Header row -->
            <div class="flex items-center justify-between mb-3">
              <span :class="['text-xs font-bold px-2 py-0.5 rounded-full border',
                event.severity === 'High'
                  ? 'bg-red-500/20 border-red-400/40 text-red-400'
                  : 'bg-orange-500/20 border-orange-400/40 text-orange-400']">
                {{ event.severity || 'Medium' }} Severity
              </span>
              <span class="text-xs text-white/30">{{ event.time }}</span>
            </div>
            <!-- Stats -->
            <div class="grid grid-cols-2 gap-2 mb-3">
              <div class="bg-white/5 rounded-lg p-2">
                <p class="text-xs text-white/40">Impact</p>
                <p class="text-sm font-bold text-white">{{ event.impactStrength }} g</p>
              </div>
              <div class="bg-white/5 rounded-lg p-2">
                <p class="text-xs text-white/40">Speed</p>
                <p class="text-sm font-bold text-white">{{ event.speed ? event.speed.toFixed(1) + ' km/h' : 'N/A' }}</p>
              </div>
              <div class="bg-white/5 rounded-lg p-2 col-span-2">
                <p class="text-xs text-white/40 mb-0.5">GPS Coordinates</p>
                <p class="text-xs font-mono text-white/60">
                  {{ event.hasGPS ? `${Number(event.lat).toFixed(6)}, ${Number(event.lng).toFixed(6)}` : 'No GPS fix recorded' }}
                </p>
              </div>
            </div>
            <!-- Navigation buttons -->
            <div class="flex gap-2">
              <a v-if="event.hasGPS && event.lat && event.lng"
                :href="`https://www.google.com/maps?q=${event.lat},${event.lng}`"
                target="_blank" rel="noopener noreferrer"
                class="flex-1 inline-flex items-center justify-center gap-1.5 px-3 py-2.5 bg-blue-500/20 border border-blue-400/30 text-blue-400 text-xs font-bold rounded-lg hover:bg-blue-500/30 transition-all">
                <span class="material-icons text-sm">navigation</span>Navigate to Crash Site
              </a>
              <div v-else
                class="flex-1 inline-flex items-center justify-center gap-1.5 px-3 py-2 bg-white/5 border border-white/10 text-white/30 text-xs rounded-lg">
                <span class="material-icons text-sm">gps_off</span>No GPS location
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>

    <!-- ===== MAIN ===== -->
    <main class="flex-1 px-4 md:px-8 py-6 max-w-7xl mx-auto w-full">

      <!-- Row 1: Pairing + Engine Status -->
      <div class="grid grid-cols-1 md:grid-cols-2 gap-4 mb-6">
        <!-- Device Pairing -->
        <div class="relative overflow-hidden bg-white/5 backdrop-blur-lg rounded-2xl shadow-lg p-5 border border-white/10 hover:border-white/20 transition-all duration-300">
          <div class="flex items-center justify-between">
            <div class="flex items-center gap-3">
              <div :class="['p-3 rounded-xl border', helmetPaired && motorcyclePaired ? 'bg-green-500/20 border-green-400/30' : 'bg-red-500/20 border-red-400/30']">
                <span :class="['material-icons text-2xl', helmetPaired && motorcyclePaired ? 'text-green-400' : 'text-red-400']">link</span>
              </div>
              <div>
                <p class="text-xs text-white/40 font-medium uppercase tracking-wide">Device Pairing</p>
                <p :class="['text-lg font-bold', helmetPaired && motorcyclePaired ? 'text-green-400' : 'text-red-400']">
                  {{ helmetPaired && motorcyclePaired ? 'Connected' : 'Disconnected' }}
                </p>
              </div>
            </div>
            <div class="flex flex-col gap-1.5">
              <div class="flex items-center gap-2">
                <span :class="['material-icons text-sm', helmetPaired ? 'text-green-400' : 'text-white/20']">sports_motorsports</span>
                <span :class="['text-xs font-medium', helmetPaired ? 'text-green-400' : 'text-white/30']">Helmet</span>
                <span :class="['w-1.5 h-1.5 rounded-full', helmetPaired ? 'bg-green-400 animate-pulse' : 'bg-white/10']"></span>
              </div>
              <div class="flex items-center gap-2">
                <span :class="['material-icons text-sm', motorcyclePaired ? 'text-green-400' : 'text-white/20']">two_wheeler</span>
                <span :class="['text-xs font-medium', motorcyclePaired ? 'text-green-400' : 'text-white/30']">Motorcycle</span>
                <span :class="['w-1.5 h-1.5 rounded-full', motorcyclePaired ? 'bg-green-400 animate-pulse' : 'bg-white/10']"></span>
              </div>
            </div>
          </div>
        </div>

        <!-- Engine Status -->
        <div :class="['relative overflow-hidden rounded-2xl shadow-lg p-5 border transition-all duration-500',
          engineRunning ? 'bg-green-500/10 border-green-400/20' : 'bg-white/5 border-white/10']">
          <div class="flex items-center justify-between">
            <div class="flex items-center gap-3">
              <div :class="['p-3 rounded-xl border', engineRunning ? 'bg-green-500/20 border-green-400/30' : 'bg-white/5 border-white/10']">
                <span :class="['material-icons text-2xl', engineRunning ? 'text-green-400' : 'text-white/30']">power_settings_new</span>
              </div>
              <div>
                <p :class="['text-xs font-medium uppercase tracking-wide', engineRunning ? 'text-green-400/60' : 'text-white/40']">Engine Status</p>
                <p :class="['text-lg font-bold', engineRunning ? 'text-green-400' : 'text-white/60']">
                  {{ engineRunning ? 'Running' : 'Stopped' }}
                </p>
              </div>
            </div>
            <div :class="['w-3 h-3 rounded-full', engineRunning ? 'bg-green-400 animate-pulse' : 'bg-white/10']"></div>
          </div>
          <p :class="['text-xs mt-2', engineRunning ? 'text-green-400/50' : 'text-white/25']">
            {{ engineRunning ? 'Rider is currently on the road' : 'Motorcycle engine is off' }}
          </p>
        </div>
      </div>

      <!-- Row 2: Status Cards (mirrors user dashboard) -->
      <div class="grid grid-cols-2 lg:grid-cols-5 gap-4 mb-6">
        <!-- Rider Status -->
        <div :class="['group relative overflow-hidden text-white rounded-3xl shadow-xl p-5 transition-all duration-500 hover:scale-105 col-span-1',
          engineRunning ? 'bg-gradient-to-br from-[#3D52A0] via-[#4a5fb8] to-[#2a3a70]' : 'bg-gradient-to-br from-red-600 via-red-500 to-red-700 animate-pulse']">
          <div class="absolute top-0 right-0 w-24 h-24 bg-white/10 rounded-full -mr-12 -mt-12"></div>
          <div class="relative">
            <div class="flex items-center justify-between mb-3">
              <div :class="['p-2 rounded-xl', engineRunning ? 'bg-white/20' : 'bg-white/30 animate-bounce']">
                <span class="material-icons text-2xl">person</span>
              </div>
              <div :class="['w-3 h-3 rounded-full', engineRunning ? 'bg-green-400 animate-pulse' : 'bg-yellow-300 animate-ping']"></div>
            </div>
            <p class="text-xs opacity-70 mb-1">Rider Status</p>
            <p class="text-xl font-bold">{{ engineRunning ? 'Active' : 'Inactive' }}</p>
            <p class="text-xs opacity-60 mt-1">{{ engineRunning ? 'On the road' : 'Engine off' }}</p>
          </div>
        </div>

        <!-- Speed -->
        <div :class="['group relative overflow-hidden text-white rounded-3xl shadow-xl p-5 transition-all duration-500 hover:scale-105 col-span-1',
          isOverSpeed ? 'bg-gradient-to-br from-red-500 via-red-600 to-red-700 animate-pulse' : 'bg-gradient-to-br from-[#7091E6] via-[#6081d6] to-[#5571c6]']">
          <div class="absolute top-0 right-0 w-24 h-24 bg-white/10 rounded-full -mr-12 -mt-12"></div>
          <div class="relative">
            <div class="flex items-center justify-between mb-3">
              <div class="bg-white/20 p-2 rounded-xl">
                <span class="material-icons text-2xl">speed</span>
              </div>
              <div :class="['w-3 h-3 rounded-full', isOverSpeed ? 'bg-yellow-300 animate-pulse' : 'bg-blue-300']"></div>
            </div>
            <p class="text-xs opacity-70 mb-1">Current Speed</p>
            <p class="text-xl font-bold">{{ currentSpeedText }}</p>
            <p class="text-xs opacity-60 mt-1">{{ isOverSpeed ? 'Over speed limit!' : `Limit: ${speedLimit} km/h` }}</p>
          </div>
        </div>

        <!-- Alertness -->
        <div class="group relative overflow-hidden bg-gradient-to-br from-[#8697C4] via-[#7687b4] to-[#6677a4] text-white rounded-3xl shadow-xl p-5 transition-all duration-500 hover:scale-105 col-span-1">
          <div class="absolute top-0 right-0 w-24 h-24 bg-white/10 rounded-full -mr-12 -mt-12"></div>
          <div class="relative">
            <div class="flex items-center justify-between mb-3">
              <div class="bg-white/20 p-2 rounded-xl">
                <span class="material-icons text-2xl">visibility</span>
              </div>
              <div class="w-3 h-3 rounded-full bg-green-400"></div>
            </div>
            <p class="text-xs opacity-70 mb-1">Alertness</p>
            <p class="text-xl font-bold">{{ alcoholStatus === 'Safe' ? 'Normal' : 'Impaired' }}</p>
            <p class="text-xs opacity-60 mt-1">{{ alcoholStatus === 'Safe' ? 'No impairment' : 'Alcohol detected' }}</p>
          </div>
        </div>

        <!-- Alcohol -->
        <div :class="['group relative overflow-hidden text-white rounded-3xl shadow-xl p-5 transition-all duration-500 hover:scale-105 col-span-1',
          alcoholStatus === 'Safe' ? 'bg-gradient-to-br from-[#3D52A0] via-[#4a5fb8] to-[#2a3a70]' : 'bg-gradient-to-br from-red-500 via-red-600 to-red-700 animate-pulse']">
          <div class="absolute top-0 right-0 w-24 h-24 bg-white/10 rounded-full -mr-12 -mt-12"></div>
          <div class="relative">
            <div class="flex items-center justify-between mb-3">
              <div class="bg-white/20 p-2 rounded-xl">
                <span class="material-icons text-2xl">local_bar</span>
              </div>
              <div :class="['w-3 h-3 rounded-full', alcoholStatus === 'Safe' ? 'bg-green-400' : 'bg-red-300 animate-pulse']"></div>
            </div>
            <p class="text-xs opacity-70 mb-1">Alcohol</p>
            <p class="text-xl font-bold">{{ alcoholStatus }}</p>
            <p class="text-xs opacity-60 mt-1">{{ alcoholStatus === 'Safe' ? 'No alcohol detected' : 'Danger level!' }}</p>
          </div>
        </div>

        <!-- Crash -->
        <div :class="['group relative overflow-hidden text-white rounded-3xl shadow-xl p-5 transition-all duration-500 hover:scale-105 col-span-1',
          crashDisplayStatus === 'Stable' ? 'bg-gradient-to-br from-green-600 via-green-700 to-green-800' : 'bg-gradient-to-br from-red-600 via-red-700 to-red-800 animate-pulse']">
          <div class="absolute top-0 right-0 w-24 h-24 bg-white/10 rounded-full -mr-12 -mt-12"></div>
          <div class="relative">
            <div class="flex items-center justify-between mb-3">
              <div class="bg-white/20 p-2 rounded-xl">
                <span class="material-icons text-2xl">warning</span>
              </div>
              <div :class="['w-3 h-3 rounded-full', crashDisplayStatus === 'Stable' ? 'bg-emerald-300' : 'bg-yellow-300 animate-pulse']"></div>
            </div>
            <p class="text-xs opacity-70 mb-1">Vehicle Status</p>
            <p class="text-xl font-bold">{{ crashDisplayStatus }}</p>
            <p class="text-xs opacity-60 mt-1">{{ crashDisplayStatus === 'Stable' ? 'No incidents' : 'Alert active' }}</p>
          </div>
        </div>
      </div>

      <!-- Row 3: Speed Limit Control (Emergency Contact only — interactive) -->
      <div class="relative overflow-hidden rounded-2xl shadow-2xl p-6 mb-6 transition-all duration-500 border border-white/10"
        :style="{ background: speedLimitGradient }">
        <div class="relative">
          <!-- Header -->
          <div class="flex items-center justify-between mb-5">
            <div class="flex items-center gap-3">
              <div class="bg-white/20 backdrop-blur-sm p-2.5 rounded-xl">
                <span class="material-icons text-2xl text-white">tune</span>
              </div>
              <div>
                <h3 class="text-base font-bold text-white">Speed Limit Control</h3>
                <p class="text-white/60 text-xs flex items-center gap-1">
                  <span class="material-icons text-xs">edit</span>
                  Only you (emergency contact) can adjust this
                </p>
              </div>
            </div>
            <div class="text-right">
              <span class="text-3xl font-black text-white">{{ speedLimit }}</span>
              <span class="text-sm font-semibold text-white/70 ml-1">km/h</span>
              <p class="text-white/50 text-xs mt-0.5">{{ speedZoneLabel }}</p>
            </div>
          </div>

          <!-- Progress bar with threshold marker and current speed indicator -->
          <div class="relative mb-2">
            <!-- Track background -->
            <div class="w-full h-5 rounded-full bg-white/15 overflow-visible relative">
              <!-- Fill -->
              <div class="h-full rounded-full transition-all duration-300 absolute top-0 left-0"
                :style="{ width: `${(speedLimit / 120) * 100}%`, background: speedBarColor }"></div>

              <!-- Current speed indicator (white vertical line) -->
              <div v-if="currentSpeed > 0"
                class="absolute top-1/2 -translate-y-1/2 flex flex-col items-center"
                :style="{ left: `${Math.min((currentSpeed / 120) * 100, 100)}%` }">
                <div class="w-0.5 h-7 bg-white shadow-lg rounded-full -mt-1"></div>
                <span class="text-white text-xs font-bold mt-1 bg-black/30 px-1 rounded whitespace-nowrap">
                  {{ currentSpeed.toFixed(0) }}
                </span>
              </div>

              <!-- Threshold marker (diamond shape at the set limit) -->
              <div class="absolute top-1/2 -translate-y-1/2 -translate-x-1/2 flex flex-col items-center"
                :style="{ left: `${(speedLimit / 120) * 100}%` }">
                <div class="w-4 h-4 bg-white rotate-45 shadow-xl border-2 border-white/50"
                  :style="{ boxShadow: `0 0 8px ${speedBarColor}` }"></div>
              </div>
            </div>

            <!-- Invisible range input on top for interaction -->
            <input type="range" min="0" max="120" step="5" v-model.number="speedLimit"
              @input="updateSpeedLimitInFirebase"
              class="absolute inset-0 w-full h-5 opacity-0 cursor-pointer" />
          </div>

          <!-- Labels -->
          <div class="flex justify-between text-xs font-bold text-white/60 mt-3">
            <span>0</span><span>30</span><span>60</span><span>90</span><span>120 km/h</span>
          </div>

          <!-- Legend -->
          <div class="mt-4 flex flex-wrap items-center gap-4 text-xs text-white/70">
            <div class="flex items-center gap-1.5">
              <div class="w-3 h-3 bg-white rotate-45"></div>
              <span>Limit threshold</span>
            </div>
            <div class="flex items-center gap-1.5">
              <div class="w-0.5 h-4 bg-white rounded-full"></div>
              <span>Current speed</span>
            </div>
            <div class="flex items-center gap-1.5">
              <div :class="['w-2.5 h-2.5 rounded-full', speedZoneColor]"></div>
              <span>{{ speedZoneLabel }}</span>
            </div>
          </div>

          <!-- Over-speed alert -->
          <div v-if="isOverSpeed"
            class="mt-4 flex items-center gap-2 bg-red-500/20 border border-red-400/30 rounded-xl px-4 py-2.5">
            <span class="material-icons text-red-300 text-sm animate-pulse">warning</span>
            <p class="text-red-200 text-xs font-semibold">
              Rider is exceeding your set limit! ({{ currentSpeed.toFixed(0) }} km/h > {{ speedLimit }} km/h)
            </p>
          </div>
        </div>
      </div>

      <!-- Row 4: Location + Speed Tabs -->
      <div class="mb-4">
        <div class="bg-white/5 backdrop-blur-md rounded-2xl p-1.5 border border-white/10 flex gap-1.5">
          <button v-for="tab in ['Rider Location', 'Speed Data', 'Recent Alerts']" :key="tab"
            @click="activeTab = tab"
            :class="['flex-1 px-4 py-2 rounded-xl font-semibold text-sm transition-all duration-300',
              activeTab === tab ? 'bg-gradient-to-r from-[#3D52A0] to-[#7091E6] text-white shadow-lg' : 'text-white/40 hover:text-white/70 hover:bg-white/5']">
            {{ tab }}
          </button>
        </div>
      </div>

      <!-- Tab: Rider Location -->
      <div v-show="activeTab === 'Rider Location'" class="bg-white/5 backdrop-blur-lg rounded-2xl shadow-2xl p-6 md:p-8 mb-6 border border-white/10">
        <h3 class="text-lg font-bold text-white mb-5 flex items-center gap-2">
          <span class="material-icons text-[#7091E6]">location_on</span>
          Rider Location Monitoring
          <span class="ml-auto flex items-center gap-1 text-xs font-normal text-green-400 bg-green-500/10 px-3 py-1 rounded-full border border-green-400/20">
            <span class="w-1.5 h-1.5 bg-green-400 rounded-full animate-pulse"></span>
            Live GPS Module
          </span>
        </h3>
        <EmergencyRiderMap
          :location="location"
          :speed="currentSpeed"
          :speed-limit="speedLimit"
          :satellites="satelliteCount"
          :is-over-speed="isOverSpeed"
        />
      </div>

      <!-- Tab: Speed Data -->
      <div v-show="activeTab === 'Speed Data'" class="bg-white/5 backdrop-blur-lg rounded-2xl shadow-2xl p-6 md:p-8 mb-6 border border-white/10">
        <h3 class="text-lg font-bold text-white mb-5 flex items-center gap-2">
          <span class="material-icons text-[#7091E6]">speed</span>
          Speed Monitoring
        </h3>

        <!-- Live Speed Gauge -->
        <div class="bg-white/5 border border-white/10 rounded-2xl p-6 mb-4">
          <div class="flex items-center justify-between mb-4">
            <h4 class="font-bold text-white/70">Live Speed</h4>
            <span :class="['px-3 py-1 rounded-full text-xs font-bold border',
              isOverSpeed
                ? 'bg-red-500/20 border-red-400/30 text-red-400 animate-pulse'
                : 'bg-green-500/20 border-green-400/30 text-green-400']">
              {{ isOverSpeed ? 'OVER LIMIT' : 'SAFE' }}
            </span>
          </div>
          <div class="flex items-end gap-4">
            <p :class="['text-6xl font-black', isOverSpeed ? 'text-red-400' : 'text-white']">{{ currentSpeed.toFixed(0) }}</p>
            <div class="pb-2">
              <p class="text-white/40 text-sm">km/h</p>
              <p class="text-white/30 text-xs">Limit: {{ speedLimit }} km/h</p>
            </div>
          </div>
          <div class="mt-4 w-full h-2 bg-white/10 rounded-full overflow-hidden">
            <div class="h-full rounded-full transition-all duration-500"
              :style="{ width: `${Math.min((currentSpeed / Math.max(speedLimit * 1.5, 1)) * 100, 100)}%`,
                        background: isOverSpeed ? '#ef4444' : '#3b82f6' }">
            </div>
          </div>
        </div>

        <!-- Speed History -->
        <div class="bg-white/5 border border-white/10 rounded-2xl p-5">
          <h4 class="font-bold text-white/60 mb-4 flex items-center gap-2 text-sm">
            <span class="material-icons text-base text-[#7091E6]">timeline</span> Recent Speed History
          </h4>
          <div v-if="speedHistory.length > 0" class="flex items-end gap-1 h-16">
            <div v-for="(speed, i) in speedHistory.slice(-20)" :key="i"
              class="flex-1 rounded-t transition-all duration-300"
              :style="{ height: `${Math.min((speed / Math.max(speedLimit * 1.5, 1)) * 100, 100)}%`,
                        background: speed > speedLimit ? '#ef4444' : '#22c55e' }"
              :title="`${speed.toFixed(0)} km/h`">
            </div>
          </div>
          <p v-else class="text-white/30 text-sm text-center py-4">No speed history yet</p>
        </div>
      </div>

      <!-- Tab: Recent Alerts -->
      <div v-show="activeTab === 'Recent Alerts'" class="bg-white/5 backdrop-blur-lg rounded-2xl shadow-2xl p-6 md:p-8 mb-6 border border-white/10">
        <div class="flex items-center justify-between mb-5">
          <div class="flex items-center gap-2">
            <span class="material-icons text-[#7091E6]">notifications_active</span>
            <h3 class="text-lg font-bold text-white">Recent Alerts</h3>
            <span v-if="alerts.length > 0" class="bg-red-500/80 text-white text-xs px-2 py-0.5 rounded-full font-bold">{{ alerts.length }}</span>
          </div>
          <button v-if="alerts.length > 0" @click="alerts = []"
            class="text-xs font-semibold px-3 py-1.5 rounded-lg bg-red-500/20 border border-red-400/30 text-red-400 hover:bg-red-500/30 transition-all">
            Clear All
          </button>
        </div>

        <!-- Empty state -->
        <div v-if="alerts.length === 0" class="text-center py-12">
          <span class="material-icons text-5xl text-white/10 mb-3 block">notifications_none</span>
          <p class="text-white/30 font-medium">No alerts since you opened this page</p>
          <p class="text-white/20 text-xs mt-1">Alerts from the rider's motorcycle and helmet will appear here</p>
        </div>

        <!-- Alert items -->
        <div v-else class="space-y-2 max-h-80 overflow-y-auto custom-scrollbar pr-1">
          <div v-for="(alert, index) in alerts" :key="index"
            :class="['group flex items-start gap-3 p-3 rounded-xl border transition-all duration-200',
              alert.type === 'danger' || alert.type === 'crash'
                ? 'bg-red-500/10 border-red-500/20'
                : alert.type === 'alcohol'
                  ? 'bg-orange-500/10 border-orange-500/20'
                : alert.type === 'warning' || alert.type === 'theft'
                  ? 'bg-yellow-500/10 border-yellow-500/20'
                : alert.type === 'success'
                  ? 'bg-green-500/10 border-green-500/20'
                : alert.type === 'speed'
                  ? 'bg-amber-500/10 border-amber-500/20'
                : 'bg-blue-500/10 border-blue-500/20']">

            <!-- Icon -->
            <div :class="['flex-shrink-0 w-8 h-8 rounded-lg flex items-center justify-center mt-0.5',
              alert.type === 'danger' || alert.type === 'crash' ? 'bg-red-500/20' :
              alert.type === 'alcohol' ? 'bg-orange-500/20' :
              alert.type === 'warning' || alert.type === 'theft' ? 'bg-yellow-500/20' :
              alert.type === 'success' ? 'bg-green-500/20' :
              alert.type === 'speed' ? 'bg-amber-500/20' : 'bg-blue-500/20']">
              <span :class="['material-icons text-base',
                alert.type === 'danger' || alert.type === 'crash' ? 'text-red-400' :
                alert.type === 'alcohol' ? 'text-orange-400' :
                alert.type === 'warning' || alert.type === 'theft' ? 'text-yellow-400' :
                alert.type === 'success' ? 'text-green-400' :
                alert.type === 'speed' ? 'text-amber-400' : 'text-blue-400']">
                {{ alert.type === 'crash' ? 'warning' :
                   alert.type === 'alcohol' ? 'local_bar' :
                   alert.type === 'speed' ? 'speed' :
                   alert.type === 'theft' ? 'security' :
                   alert.type === 'success' ? 'check_circle' :
                   alert.type === 'warning' ? 'warning' :
                   alert.type === 'info' ? 'info' : 'notifications' }}
              </span>
            </div>

            <!-- Content -->
            <div class="flex-1 min-w-0">
              <p :class="['font-semibold text-sm leading-tight',
                alert.type === 'danger' || alert.type === 'crash' ? 'text-red-300' :
                alert.type === 'alcohol' ? 'text-orange-300' :
                alert.type === 'warning' || alert.type === 'theft' ? 'text-yellow-300' :
                alert.type === 'success' ? 'text-green-300' :
                alert.type === 'speed' ? 'text-amber-300' : 'text-blue-300']">
                {{ alert.message }}
              </p>
              <p v-if="alert.details" class="text-xs text-white/40 mt-0.5 truncate">{{ alert.details }}</p>
              <p class="text-xs text-white/25 mt-0.5">{{ alert.time }}</p>
            </div>

            <!-- Delete -->
            <button @click="alerts.splice(index, 1)"
              class="flex-shrink-0 opacity-0 group-hover:opacity-100 transition-opacity text-white/30 hover:text-red-400 p-1 rounded-lg hover:bg-red-500/10">
              <span class="material-icons text-sm">close</span>
            </button>
          </div>
        </div>
      </div>

    </main>

    <!-- Footer -->
    <footer class="bg-[#0a0f1e] text-center py-5 text-white border-t border-white/5">
      <p class="font-semibold text-sm text-white/40">© 2025 VIGILERT. All rights reserved.</p>
      <p class="text-xs text-white/20 mt-1">Emergency Contact Monitoring System</p>
    </footer>
  </div>
</template>

<script setup>
import { ref, computed, onMounted, onUnmounted } from 'vue';
import { useRouter } from 'vue-router';
import { database } from '../firebase/config';
import { ref as dbRef, set, onValue, push, onChildAdded } from 'firebase/database';
import EmergencyRiderMap from '../components/EmergencyRiderMap.vue';

const router = useRouter();
const appStartTime = Date.now();

// ── State ──────────────────────────────────────────────────────────────────
const helmetPaired      = ref(false);
const motorcyclePaired  = ref(false);
const engineRunning     = ref(false);
const currentSpeed      = ref(0);
const speedLimit        = ref(60);
const isOverSpeed       = ref(false);
const alcoholStatus     = ref('Safe');
const crashDisplayStatus  = ref('Stable');
const crashDisplayMessage = ref('Normal');
const crashEvents         = ref([]);  // crash history with GPS locations
const isTrackingRider   = ref(false);
const emergencyContactLocation = ref({ lat: null, lng: null });
const activeTab         = ref('Rider Location');
const alerts            = ref([]);
const speedHistory      = ref([]);
const location          = ref({ lat: null, lng: null });
const currentTime       = ref(new Date().toLocaleTimeString());
const satelliteCount    = ref(0);

// ── Heartbeat tracking (mirrors user dashboard logic) ──────────────────────
const DISCONNECT_TIMEOUT = 10000; // 10 seconds — same as user dashboard
const lastHelmetUpdate     = ref(0);
const lastMotorcycleUpdate = ref(0);
let helmetDisconnectTimer     = null;
let motorcycleDisconnectTimer = null;
let heartbeatCheckInterval    = null;
let clockInterval             = null;

// ── Computed ───────────────────────────────────────────────────────────────
const currentSpeedText = computed(() =>
  `${currentSpeed.value.toFixed(1)} km/h`
);

// Speed limit gradient background (green → yellow → orange → red)
const speedLimitGradient = computed(() => {
  const pct = speedLimit.value / 120;
  if (pct <= 0.25)      return 'linear-gradient(135deg, #16a34a, #22c55e)';
  if (pct <= 0.5)       return 'linear-gradient(135deg, #ca8a04, #eab308)';
  if (pct <= 0.75)      return 'linear-gradient(135deg, #ea580c, #f97316)';
  return                       'linear-gradient(135deg, #dc2626, #ef4444)';
});

// Progress bar fill color
const speedBarColor = computed(() => {
  const pct = speedLimit.value / 120;
  if (pct <= 0.25) return '#22c55e';
  if (pct <= 0.5)  return '#eab308';
  if (pct <= 0.75) return '#f97316';
  return '#ef4444';
});

const speedZoneColor = computed(() => {
  const pct = speedLimit.value / 120;
  if (pct <= 0.25) return 'bg-green-400';
  if (pct <= 0.5)  return 'bg-yellow-400';
  if (pct <= 0.75) return 'bg-orange-400';
  return 'bg-red-500';
});

const speedZoneLabel = computed(() => {
  if (speedLimit.value <= 30)  return 'School / Residential Zone';
  if (speedLimit.value <= 60)  return 'City / Urban Zone';
  if (speedLimit.value <= 90)  return 'Highway Zone';
  return 'Expressway Zone';
});

// ── Methods ────────────────────────────────────────────────────────────────
const logout = () => router.push('/emergency-login');

const toggleRiderTracking = () => {
  isTrackingRider.value = !isTrackingRider.value;
};

// ── Alert sound ────────────────────────────────────────────────────────────
let currentAudio = null;

const playAlertSound = () => {
  try {
    if (currentAudio) {
      currentAudio.pause();
      currentAudio.currentTime = 0;
    }
    currentAudio = new Audio('/sounds/alert.mp3');
    currentAudio.play().catch(err => console.warn('[SOUND] Playback failed:', err));
    currentAudio.addEventListener('ended', () => { currentAudio = null; });
  } catch (err) {
    console.error('[SOUND] Error:', err);
  }
};

const updateSpeedLimitInFirebase = () => {
  const userUID = 'MnzBjTBslZNijOkq732PE91hHa23';
  set(dbRef(database, `${userUID}/speedLimit`), speedLimit.value)
    .then(() => console.log('[SPEED LIMIT] Updated to', speedLimit.value))
    .catch(err => console.error('[SPEED LIMIT] Error:', err));
};

// ── SOS: writes to Firebase path that Dashboard.vue listens on ─────────────
const triggerSOS = async () => {
  const confirmed = confirm(
    '🚨 EMERGENCY SOS\n\nThis will send an emergency alert to the rider.\n\nAre you sure?'
  );
  if (!confirmed) return;

  const userUID = 'MnzBjTBslZNijOkq732PE91hHa23';
  try {
    const sosRef = dbRef(database, `helmet_public/${userUID}/sos`);
    await push(sosRef, {
      timestamp: Date.now(),
      triggeredBy: 'emergency_contact',
      emergencyContactLocation: {
        lat: emergencyContactLocation.value.lat,
        lng: emergencyContactLocation.value.lng
      },
      status: 'active'
    });

    // Add local alert
    alerts.value.unshift({
      type: 'danger',
      message: '🆘 SOS Alert Sent to Rider!',
      details: 'Emergency contact triggered SOS',
      time: new Date().toLocaleTimeString()
    });

    playAlertSound();
    alert('✅ SOS Alert sent! The rider has been notified.');
  } catch (err) {
    console.error('[SOS] Error:', err);
    alert('❌ Failed to send SOS. Please try again.');
  }
};

// ── Firebase Listeners ─────────────────────────────────────────────────────
const setupFirebaseListeners = () => {
  const userUID = 'MnzBjTBslZNijOkq732PE91hHa23';

  // ── Helmet: heartbeat-aware connection detection ──────────────────────────
  onValue(dbRef(database, `helmet_public/${userUID}/devices/helmet`), snap => {
    const d = snap.val();

    if (d && d.status === 'On') {
      // Fresh heartbeat received — device is online
      lastHelmetUpdate.value = Date.now();

      if (!helmetPaired.value) {
        helmetPaired.value = true;
        console.log('[HELMET] ✓ Connected');
      }

      // Cancel any pending disconnect timer
      if (helmetDisconnectTimer) {
        clearTimeout(helmetDisconnectTimer);
        helmetDisconnectTimer = null;
      }
    } else {
      // Firebase fired but status is not 'On' — start disconnect countdown
      if (helmetPaired.value) {
        console.log('[HELMET] No heartbeat — waiting 10s to confirm disconnect...');
        if (helmetDisconnectTimer) clearTimeout(helmetDisconnectTimer);
        helmetDisconnectTimer = setTimeout(() => {
          helmetPaired.value = false;
          console.log('[HELMET] ✗ Disconnected (confirmed after 10s)');
        }, DISCONNECT_TIMEOUT);
      }
    }
  });

  // ── Motorcycle: heartbeat-aware connection detection ──────────────────────
  onValue(dbRef(database, `helmet_public/${userUID}/devices/motorcycle`), snap => {
    const d = snap.val();

    if (d && d.status === 'On') {
      lastMotorcycleUpdate.value = Date.now();

      if (!motorcyclePaired.value) {
        motorcyclePaired.value = true;
        console.log('[MOTORCYCLE] ✓ Connected');
      }

      if (motorcycleDisconnectTimer) {
        clearTimeout(motorcycleDisconnectTimer);
        motorcycleDisconnectTimer = null;
      }

      // GPS satellite count from motorcycle heartbeat
      if (d.gps && d.gps.satellites !== undefined) {
        satelliteCount.value = d.gps.satellites;
      }
    } else {
      if (motorcyclePaired.value) {
        console.log('[MOTORCYCLE] No heartbeat — waiting 10s to confirm disconnect...');
        if (motorcycleDisconnectTimer) clearTimeout(motorcycleDisconnectTimer);
        motorcycleDisconnectTimer = setTimeout(() => {
          motorcyclePaired.value = false;
          console.log('[MOTORCYCLE] ✗ Disconnected (confirmed after 10s)');
        }, DISCONNECT_TIMEOUT);
      }
    }
  });

  // ── Heartbeat staleness check (runs every 5s, same logic as user dashboard) ─
  heartbeatCheckInterval = setInterval(() => {
    const now = Date.now();

    // Helmet stale check
    if (lastHelmetUpdate.value > 0 && now - lastHelmetUpdate.value > DISCONNECT_TIMEOUT && helmetPaired.value) {
      helmetPaired.value = false;
      console.log('[HELMET] ✗ Stale heartbeat — marked disconnected');
    }

    // Motorcycle stale check
    if (lastMotorcycleUpdate.value > 0 && now - lastMotorcycleUpdate.value > DISCONNECT_TIMEOUT && motorcyclePaired.value) {
      motorcyclePaired.value = false;
      console.log('[MOTORCYCLE] ✗ Stale heartbeat — marked disconnected');
    }
  }, 5000);

  // Live data
  onValue(dbRef(database, `helmet_public/${userUID}/live`), snap => {
    const d = snap.val();
    if (!d) return;

    // Engine — this is the source of truth for rider status
    engineRunning.value = !!d.engineRunning;

    // Speed
    const spd = d.speed || 0;
    currentSpeed.value = spd;
    isOverSpeed.value = spd > speedLimit.value;
    if (spd > 0) {
      speedHistory.value.push(spd);
      if (speedHistory.value.length > 50) speedHistory.value.shift();
    }

    // Location
    if (d.locationLat && d.locationLng) {
      location.value = { lat: d.locationLat, lng: d.locationLng };
    }

    // Satellite count from GPS module
    if (d.gps && d.gps.satellites !== undefined) {
      satelliteCount.value = d.gps.satellites;
    } else if (d.satellites !== undefined) {
      satelliteCount.value = d.satellites;
    }

    // Alcohol
    alcoholStatus.value = d.alcoholDetected ? 'Danger' : 'Safe';

    // Crash
    crashDisplayStatus.value  = d.crashDetected ? 'Alert'         : 'Stable';
    crashDisplayMessage.value = d.crashDetected ? 'Crash Detected' : 'Normal';
  });

  // ── Crash history listener ────────────────────────────────────────────
  onChildAdded(dbRef(database, `helmet_public/${userUID}/crashes`), (snap) => {
    const event = snap.val();
    if (!event || !event.timestamp) return;

    // Only show crashes after app opened
    if (event.timestamp <= appStartTime) return;

    const crashEvent = {
      timestamp: event.timestamp,
      impactStrength: event.impactStrength || (event.acceleration ? Number(event.acceleration).toFixed(2) : 'N/A'),
      roll: event.roll || event.leanAngle || 0,
      lat: event.lat || (event.location && event.location.lat) || null,
      lng: event.lng || (event.location && event.location.lng) || null,
      hasGPS: event.hasGPS || event.gpsValid || (event.lat && event.lat !== 0),
      severity: event.severity || 'Medium',
      time: event.time || new Date(event.timestamp).toLocaleTimeString(),
      speed: event.speed || 0
    };

    // Keep only the latest 5 crashes
    crashEvents.value.unshift(crashEvent);
    if (crashEvents.value.length > 5) crashEvents.value = crashEvents.value.slice(0, 5);

    // Update status
    crashDisplayStatus.value  = 'Alert';
    crashDisplayMessage.value = 'Crash Detected';

    // Play alert sound for crash
    playAlertSound();

    console.log('[CRASH] New crash event received:', crashEvent);
  });

  // Speed limit (sync from rider dashboard changes)
  onValue(dbRef(database, `${userUID}/speedLimit`), snap => {
    const d = snap.val();
    if (d !== null && d !== undefined) speedLimit.value = d;
  });

  // Alerts — only show ones created after this page opened, play sound for danger/warning
  onValue(dbRef(database, `helmet_public/${userUID}/alerts`), snap => {
    const d = snap.val();
    if (!d) return;
    const fresh = Object.values(d)
      .filter(a => (a.timestamp || 0) > appStartTime)
      .sort((a, b) => (b.timestamp || 0) - (a.timestamp || 0))
      .slice(0, 20);

    // Play sound if there are new alerts since last check
    const prevCount = alerts.value.length;
    alerts.value = fresh;
    if (fresh.length > prevCount) {
      const newest = fresh[0];
      const type = newest?.type || '';
      if (['danger', 'crash', 'alcohol', 'warning', 'theft', 'speed'].includes(type)) {
        playAlertSound();
      }
    }
  });
};

// ── Lifecycle ──────────────────────────────────────────────────────────────
onMounted(() => {
  setupFirebaseListeners();

  clockInterval = setInterval(() => {
    currentTime.value = new Date().toLocaleTimeString();
  }, 1000);

  if (navigator.geolocation) {
    navigator.geolocation.getCurrentPosition(
      pos => {
        emergencyContactLocation.value = {
          lat: pos.coords.latitude,
          lng: pos.coords.longitude
        };
      },
      err => console.warn('[GEO] Error:', err)
    );
  }
});

onUnmounted(() => {
  if (clockInterval) clearInterval(clockInterval);
  if (heartbeatCheckInterval) clearInterval(heartbeatCheckInterval);
  if (helmetDisconnectTimer) clearTimeout(helmetDisconnectTimer);
  if (motorcycleDisconnectTimer) clearTimeout(motorcycleDisconnectTimer);
});
</script>

<style scoped>
.custom-scrollbar::-webkit-scrollbar { width: 6px; }
.custom-scrollbar::-webkit-scrollbar-track { background: #f1f1f1; border-radius: 10px; }
.custom-scrollbar::-webkit-scrollbar-thumb { background: linear-gradient(180deg, #7091E6, #3D52A0); border-radius: 10px; }
.custom-scrollbar::-webkit-scrollbar-thumb:hover { background: #3D52A0; }
</style>
