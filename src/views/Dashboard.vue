<template>
  <div class="min-h-screen flex flex-col bg-[#061B2B] text-white pb-20">
    
    <!-- ============================================================== -->
    <!-- PANORAMIC HERO WELCOME BANNER (Matches Reference Mockup)      -->
    <!-- ============================================================== -->
    <div class="relative overflow-hidden rounded-3xl shadow-2xl mb-6 border border-[#00B85C]/20 bg-[#0B2540] group">
      <!-- Panoramic Mountain Rider Background -->
      <div class="absolute inset-0 z-0">
        <img 
          src="/images/dashboard-banner.png" 
          alt="VigiLERT Rider Banner" 
          class="w-full h-full object-cover object-left md:object-center opacity-85 group-hover:scale-[1.01] transition-transform duration-700" 
        />
        <!-- Dark Cyber Navy Gradient Overlay -->
        <div class="absolute inset-0 bg-gradient-to-r from-[#061B2B]/95 via-[#0B2540]/75 to-[#061B2B]/90"></div>
      </div>

      <!-- Faint Tech Overlay Pattern -->
      <div class="absolute inset-0 opacity-[0.05] pointer-events-none"
        style="background-image: linear-gradient(rgba(112,227,74,0.3) 1px, transparent 1px), linear-gradient(90deg, rgba(112,227,74,0.3) 1px, transparent 1px); background-size: 32px 32px;">
      </div>

      <div class="relative z-10 p-6 sm:p-8 flex flex-col md:flex-row items-start md:items-center justify-between gap-6">
        <!-- Rider Welcome Header -->
        <div class="max-w-xl">
          <div class="inline-flex items-center gap-2 px-3 py-1 rounded-full bg-[#00B85C]/15 border border-[#00B85C]/30 text-[#00B85C] text-xs font-bold mb-3 shadow-sm">
            <span class="w-2 h-2 rounded-full bg-[#00B85C] animate-pulse"></span>
            <span>CONNECTED SAFETY SYSTEM ACTIVE</span>
          </div>
          <h1 class="text-2xl sm:text-3xl md:text-4xl font-black text-white tracking-tight drop-shadow-md">
            Welcome Back, <span class="text-white">{{ authStore.user?.displayName || authStore.user?.email?.split('@')[0] || 'Rider' }}</span>!
          </h1>
          <p class="text-white/90 text-sm sm:text-base font-medium italic mt-1 drop-shadow">
            Ride Smart. Stay Safe.
          </p>
          <div class="flex flex-wrap items-center gap-3 text-slate-300 text-xs mt-3">
            <div class="flex items-center gap-1.5">
              <span class="material-icons text-xs text-[#00B85C]">account_circle</span>
              <span>{{ authStore.user?.email || 'Logged in' }}</span>
            </div>
            <span>•</span>
            <div class="flex items-center gap-1.5">
              <span class="material-icons text-xs text-[#00B85C]">schedule</span>
              <span>{{ getCurrentTime() }}</span>
            </div>
          </div>
        </div>

        <!-- Integrated Floating Weather Pill (Compact) -->
        <div v-if="!weather.loading && !weather.error" class="flex items-center gap-4 bg-[#061B2B]/85 backdrop-blur-md border border-white/10 rounded-2xl p-4 shadow-xl">
          <div class="text-4xl filter drop-shadow">{{ getWeatherEmoji(weather.condition) }}</div>
          <div class="text-left">
            <p class="text-2xl font-bold text-white leading-none">{{ weather.temperature }}°C</p>
            <p class="text-xs text-slate-300 font-medium mt-1">{{ weather.description }}</p>
            <div class="flex items-center gap-3 mt-1.5 text-[11px] text-slate-400">
              <span class="flex items-center gap-1">
                <span class="material-icons text-[12px] text-slate-400">location_on</span>
                {{ weather.location }}
              </span>
              <span>•</span>
              <span :class="['font-semibold', getRidingSafetyTextColor()]">
                {{ getRidingSafetyLevel() }}
              </span>
            </div>
          </div>
        </div>
      </div>
    </div>

    <!-- ============================================================== -->
    <!-- 4 TOP KPI STAT CARDS (Matches Reference Mockup)               -->
    <!-- ============================================================== -->
    <div class="grid grid-cols-2 lg:grid-cols-4 gap-3 sm:gap-4 mb-6">
      
      <!-- KPI 1: Active Rides / Engine State (Green Theme) -->
      <div class="relative overflow-hidden bg-[#0B2540]/75 hover:bg-[#0B2540]/95 border border-white/10 hover:border-[#00B85C]/40 backdrop-blur-md rounded-2xl p-4 sm:p-5 shadow-lg transition-all duration-300 hover:scale-[1.02] group">
        <div class="flex items-center gap-3.5 mb-2">
          <div class="w-10 h-10 sm:w-11 sm:h-11 rounded-full bg-[#00B85C]/20 border border-[#00B85C]/40 flex items-center justify-center text-[#00B85C] flex-shrink-0 group-hover:scale-110 transition-transform">
            <span class="material-icons text-xl sm:text-2xl">two_wheeler</span>
          </div>
          <div>
            <p class="text-[11px] text-slate-400 font-semibold uppercase tracking-wider">Active Rides</p>
            <p class="text-xl sm:text-2xl font-black text-white leading-tight">
              {{ recentTrips.length }}
            </p>
          </div>
        </div>
        <div class="flex items-center gap-1.5 text-xs font-semibold text-[#00B85C] mt-2 pt-2 border-t border-white/5">
          <span class="w-2 h-2 rounded-full" :class="engineRunning ? 'bg-[#00B85C] animate-pulse' : 'bg-slate-500'"></span>
          <span>{{ engineRunning ? ('Running • ' + currentSpeedText) : 'Engine Idle' }}</span>
        </div>
      </div>

      <!-- KPI 2: Active Alerts (Red Theme) -->
      <div class="relative overflow-hidden bg-[#0B2540]/75 hover:bg-[#0B2540]/95 border border-white/10 hover:border-red-500/40 backdrop-blur-md rounded-2xl p-4 sm:p-5 shadow-lg transition-all duration-300 hover:scale-[1.02] group">
        <div class="flex items-center gap-3.5 mb-2">
          <div class="w-10 h-10 sm:w-11 sm:h-11 rounded-full bg-red-500/20 border border-red-500/40 flex items-center justify-center text-red-400 flex-shrink-0 group-hover:scale-110 transition-transform">
            <span class="material-icons text-xl sm:text-2xl" :class="alerts.length ? 'animate-pulse' : ''">warning</span>
          </div>
          <div>
            <p class="text-[11px] text-slate-400 font-semibold uppercase tracking-wider">Active Alerts</p>
            <p class="text-xl sm:text-2xl font-black text-white leading-tight">
              {{ alerts.length }}
            </p>
          </div>
        </div>
        <div class="flex items-center gap-1.5 text-xs font-semibold mt-2 pt-2 border-t border-white/5" :class="alerts.length ? 'text-red-400' : 'text-emerald-400'">
          <span class="material-icons text-xs">{{ alerts.length ? 'notifications_active' : 'check_circle' }}</span>
          <span>{{ alerts.length ? (alerts.length + ' requiring attention') : 'All Clear • Safe' }}</span>
        </div>
      </div>

      <!-- KPI 3: Safety Status (Blue Theme) -->
      <div class="relative overflow-hidden bg-[#0B2540]/75 hover:bg-[#0B2540]/95 border border-white/10 hover:border-blue-400/40 backdrop-blur-md rounded-2xl p-4 sm:p-5 shadow-lg transition-all duration-300 hover:scale-[1.02] group">
        <div class="flex items-center gap-3.5 mb-2">
          <div class="w-10 h-10 sm:w-11 sm:h-11 rounded-full bg-blue-500/20 border border-blue-400/40 flex items-center justify-center text-blue-400 flex-shrink-0 group-hover:scale-110 transition-transform">
            <span class="material-icons text-xl sm:text-2xl">verified_user</span>
          </div>
          <div>
            <p class="text-[11px] text-slate-400 font-semibold uppercase tracking-wider">Safety Status</p>
            <p class="text-xl sm:text-2xl font-black text-white leading-tight">
              {{ tripStats.safetyScore || 100 }}%
            </p>
          </div>
        </div>
        <div class="flex items-center gap-1.5 text-xs font-semibold text-blue-300 mt-2 pt-2 border-t border-white/5">
          <span class="material-icons text-xs">shield</span>
          <span>{{ alcoholStatus === 'Safe' ? 'Rider Protected' : 'Alcohol Violation' }}</span>
        </div>
      </div>

      <!-- KPI 4: Tracked Devices (Purple Theme) -->
      <div class="relative overflow-hidden bg-[#0B2540]/75 hover:bg-[#0B2540]/95 border border-white/10 hover:border-purple-400/40 backdrop-blur-md rounded-2xl p-4 sm:p-5 shadow-lg transition-all duration-300 hover:scale-[1.02] group">
        <div class="flex items-center gap-3.5 mb-2">
          <div class="w-10 h-10 sm:w-11 sm:h-11 rounded-full bg-purple-500/20 border border-purple-400/40 flex items-center justify-center text-purple-400 flex-shrink-0 group-hover:scale-110 transition-transform">
            <span class="material-icons text-xl sm:text-2xl">devices</span>
          </div>
          <div>
            <p class="text-[11px] text-slate-400 font-semibold uppercase tracking-wider">Tracked Devices</p>
            <p class="text-xl sm:text-2xl font-black text-white leading-tight">
              {{ (helmetPaired ? 1 : 0) + (motorcyclePaired ? 1 : 0) }} / 2
            </p>
          </div>
        </div>
        <div class="flex items-center gap-1.5 text-xs font-semibold text-purple-300 mt-2 pt-2 border-t border-white/5">
          <span class="w-2 h-2 rounded-full" :class="helmetPaired && motorcyclePaired ? 'bg-emerald-400 animate-pulse' : (helmetPaired || motorcyclePaired ? 'bg-amber-400' : 'bg-slate-500')"></span>
          <span>{{ helmetPaired && motorcyclePaired ? 'Helmet & Bike Online' : (helmetPaired ? 'Helmet Online • Bike Offline' : (motorcyclePaired ? 'Bike Online • Helmet Offline' : 'Devices Offline')) }}</span>
        </div>
      </div>

    </div>

    <!-- ⚠️ HELMET REMOVED WARNING BANNER -->
    <div v-if="!helmetPaired && previousHelmetState === true" 
         class="mx-4 md:mx-8 mb-6 relative overflow-hidden bg-gradient-to-r from-red-600 via-red-500 to-red-600 rounded-2xl shadow-2xl border-4 border-red-300 animate-pulse">
      <div class="absolute inset-0 bg-gradient-to-r from-red-400/20 to-red-600/20 animate-pulse"></div>
      <div class="relative p-6 flex items-center gap-4">
        <div class="bg-white/20 p-4 rounded-full animate-bounce">
          <span class="material-icons text-5xl text-white">warning</span>
        </div>
        <div class="flex-1">
          <h3 class="text-2xl font-bold text-white mb-2 flex items-center gap-2">
            <span class="animate-pulse">⚠️</span>
            HELMET REMOVED WARNING
            <span class="animate-pulse">⚠️</span>
          </h3>
          <p class="text-white/90 text-lg font-semibold">
            Rider has removed helmet during trip. Engine has been stopped for safety.
          </p>
          <p class="text-white/70 text-sm mt-2">
            Please ensure helmet is worn before starting engine again.
          </p>
        </div>
        <div class="flex flex-col gap-2">
          <div class="bg-white/20 px-4 py-2 rounded-xl">
            <p class="text-white text-sm font-bold">Status: UNSAFE</p>
          </div>
          <div class="bg-white/20 px-4 py-2 rounded-xl">
            <p class="text-white text-sm font-bold">Engine: STOPPED</p>
          </div>
        </div>
      </div>
    </div>

    <!-- 🚨 ANTI-THEFT ALERT BANNER -->
    <div v-if="antiTheftAlert.active" 
         class="mx-3 sm:mx-4 md:mx-8 mb-6 relative overflow-hidden bg-gradient-to-r from-orange-600 via-orange-500 to-orange-600 rounded-2xl shadow-2xl border-4 border-orange-300 animate-pulse">
      <div class="absolute inset-0 bg-gradient-to-r from-orange-400/20 to-orange-600/20 animate-pulse"></div>
      <div class="relative p-4 sm:p-6 flex flex-col sm:flex-row items-center sm:items-start md:items-center gap-3 sm:gap-4 text-center sm:text-left">
        <div class="bg-white/20 p-3 sm:p-4 rounded-full flex-shrink-0 animate-bounce">
          <span class="material-icons text-4xl sm:text-5xl text-white">security</span>
        </div>
        <div class="flex-1 min-w-0">
          <h3 class="text-xl sm:text-2xl font-bold text-white mb-1 flex items-center justify-center sm:justify-start gap-2">
            <span class="animate-pulse">🚨</span>
            ANTI-THEFT ALERT
            <span class="animate-pulse">🚨</span>
          </h3>
          <p class="text-white/95 text-base sm:text-lg font-semibold break-words">
            {{ antiTheftAlert.message }}
          </p>
          <p class="text-white/80 text-xs sm:text-sm mt-1 font-medium">
            Time: {{ antiTheftAlert.time }} • Alert Level: {{ antiTheftAlert.level }}
          </p>
        </div>
        <div class="flex sm:flex-col flex-wrap justify-center gap-2 w-full sm:w-auto mt-2 sm:mt-0 flex-shrink-0">
          <div :class="['px-3 py-1.5 rounded-xl text-center', antiTheftAlert.severity === 'high' ? 'bg-red-500/30' : antiTheftAlert.severity === 'medium' ? 'bg-orange-500/30' : 'bg-yellow-500/30']">
            <p class="text-white text-xs sm:text-sm font-bold">Severity: {{ antiTheftAlert.severity.toUpperCase() }}</p>
          </div>
          <button v-if="showStopSoundButton" @click="stopSound" class="bg-red-600 hover:bg-red-700 px-3.5 py-2 rounded-xl text-white text-xs font-bold transition-all flex items-center justify-center gap-1 shadow-lg">
            <span class="material-icons text-base">volume_off</span>
            Mute Alarm
          </button>
          <button @click="dismissAntiTheftAlert" class="bg-white/20 hover:bg-white/30 px-4 py-2 rounded-xl text-white text-xs sm:text-sm font-bold transition-all">
            Dismiss
          </button>
        </div>
      </div>
    </div>

    <!-- Main Dashboard -->
    <main class="flex-1 px-4 md:px-8 py-6">
      <!-- System Status Bar -->
      <div id="status" class="grid grid-cols-1 md:grid-cols-2 gap-4 mb-6">
        <!-- Helmet-Motorcycle Pairing Status -->
        <div class="relative overflow-hidden bg-white/5 backdrop-blur-lg rounded-2xl shadow-lg p-5 border border-white/10 hover:border-white/20 transition-all duration-300">
          <div class="flex items-center justify-between">
            <div class="flex items-center gap-3">
              <div :class="['p-3 rounded-xl', helmetPaired && motorcyclePaired ? 'bg-green-500/20 border border-green-400/30' : 'bg-red-500/20 border border-red-400/30']">
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

        <!-- Emergency SOS Button -->
        <div class="relative overflow-hidden bg-red-500/10 border border-red-500/20 hover:bg-red-500/20 hover:border-red-400/40 rounded-2xl shadow-lg p-5 cursor-pointer transition-all duration-300 hover:-translate-y-0.5 active:translate-y-0 group" @click="triggerSOS">
          <div class="absolute inset-0 bg-gradient-to-br from-red-500/5 to-transparent rounded-2xl"></div>
          <div class="relative flex items-center justify-between">
            <div class="flex items-center gap-3">
              <div class="p-3 rounded-xl bg-red-500/20 border border-red-400/30 group-hover:bg-red-500/30 transition-colors">
                <span class="material-icons text-red-400 text-3xl animate-pulse">emergency</span>
              </div>
              <div>
                <p class="text-xs text-red-400/70 font-medium uppercase tracking-wide">Emergency</p>
                <p class="text-xl font-bold text-white">SOS Alert</p>
              </div>
            </div>
            <span class="material-icons text-red-400/50 text-4xl group-hover:text-red-400 transition-colors">touch_app</span>
          </div>
        </div>
      </div>

      <!-- ⚡ ENGINE CONTROL DECK -->
      <div id="engine" class="relative overflow-hidden rounded-3xl shadow-2xl mb-6 transition-all duration-500 backdrop-blur-md"
        :class="engineRunning ? 'bg-[#0B2540]/85 border-2 border-[#00B85C]/50 shadow-[0_0_30px_rgba(0,184,92,0.25)]' : 'bg-[#0B2540]/75 border border-white/10'">

        <!-- Animated background glow when running -->
        <div v-if="engineRunning" class="absolute inset-0 bg-[#00B85C]/5 animate-pulse rounded-3xl pointer-events-none"></div>
        <div class="absolute inset-0 overflow-hidden rounded-3xl pointer-events-none">
          <div class="absolute -top-20 -right-20 w-64 h-64 rounded-full blur-3xl opacity-20"
            :class="engineRunning ? 'bg-[#00B85C]' : 'bg-[#0B2540]'"></div>
        </div>

        <div class="relative p-6 md:p-8">
          <!-- Header row -->
          <div class="flex items-center justify-between mb-6">
            <div class="flex items-center gap-4">
              <!-- Power icon with ring animation when running -->
              <div class="relative">
                <div v-if="engineRunning" class="absolute inset-0 rounded-full bg-green-400/20 animate-ping"></div>
                <div :class="['relative w-14 h-14 rounded-2xl flex items-center justify-center shadow-lg transition-all duration-500',
                  engineRunning ? 'bg-green-500/20 border border-green-400/40' : 'bg-white/5 border border-white/10']">
                  <span :class="['material-icons text-3xl transition-colors duration-500',
                    engineRunning ? 'text-green-400' : 'text-white/40']">power_settings_new</span>
                </div>
              </div>
              <div>
                <p class="text-white/50 text-xs font-semibold uppercase tracking-widest mb-0.5">Engine Control</p>
                <div class="flex items-center gap-2">
                  <span :class="['w-2 h-2 rounded-full transition-all duration-500',
                    enginePending ? 'bg-yellow-400 animate-pulse' :
                    engineRunning ? 'bg-green-400 animate-pulse' : 'bg-white/20']"></span>
                  <p class="text-white font-bold text-lg">
                    {{ enginePending ? (engineRunning ? 'Stopping...' : 'Starting...') : (engineRunning ? 'Engine Running' : 'Engine Stopped') }}
                  </p>
                </div>
                <p class="text-white/30 text-xs mt-0.5">
                  {{ autoEngineControl ? '⚡ Auto mode active' : '🔧 Manual control' }}
                </p>
              </div>
            </div>

            <!-- Mode badge -->
            <button @click="toggleAutoControl"
              :class="['flex items-center gap-1.5 px-3 py-1.5 rounded-xl text-xs font-semibold transition-all duration-300 border',
                autoEngineControl
                  ? 'bg-blue-500/20 border-blue-400/30 text-blue-300 hover:bg-blue-500/30'
                  : 'bg-white/5 border-white/10 text-white/50 hover:bg-white/10']">
              <span class="material-icons text-sm">{{ autoEngineControl ? 'smart_toy' : 'person' }}</span>
              {{ autoEngineControl ? 'Auto' : 'Manual' }}
            </button>
          </div>

          <!-- Main engine toggle button -->
          <button
            @click="handleEngineToggle"
            :disabled="enginePending || (alcoholDetected && !engineRunning) || !helmetPaired || !motorcyclePaired"
            :class="['w-full relative overflow-hidden rounded-2xl py-5 font-bold text-lg transition-all duration-300 flex items-center justify-center gap-3 shadow-xl',
              enginePending
                ? 'bg-yellow-500/20 border border-yellow-400/30 text-yellow-300 cursor-wait'
                : engineRunning
                  ? 'bg-red-500/20 border border-red-400/30 text-red-300 hover:bg-red-500/30 hover:border-red-400/50 active:scale-98'
                  : (alcoholDetected || !helmetPaired || !motorcyclePaired)
                    ? 'bg-white/5 border border-white/10 text-white/25 cursor-not-allowed'
                    : 'bg-gradient-to-r from-[#00B85C] to-[#087A45] hover:from-[#00c965] hover:to-[#0a8c4f] text-white shadow-lg shadow-[#00B85C]/30 active:scale-98 hover:-translate-y-0.5']">

            <!-- Shimmer effect on hover when enabled -->
            <div v-if="!enginePending && !engineRunning && helmetPaired && motorcyclePaired && !alcoholDetected"
              class="absolute inset-0 bg-gradient-to-r from-transparent via-white/5 to-transparent -translate-x-full hover:translate-x-full transition-transform duration-700 pointer-events-none"></div>

            <span v-if="enginePending" class="material-icons text-2xl animate-spin">refresh</span>
            <span v-else class="material-icons text-2xl">{{ engineRunning ? 'power_settings_new' : 'power_settings_new' }}</span>

            <span>
              {{ enginePending
                  ? (engineRunning ? 'Stopping Engine...' : 'Starting Engine...')
                  : engineRunning
                    ? 'Stop Engine'
                    : (alcoholDetected ? 'Blocked — Alcohol Detected'
                      : (!helmetPaired || !motorcyclePaired) ? 'Devices Not Connected'
                      : 'Start Engine') }}
            </span>
          </button>

          <!-- Blocking reason -->
          <div v-if="!engineRunning && (alcoholDetected || !helmetPaired || !motorcyclePaired) && !enginePending"
            class="mt-3 flex items-center gap-2 bg-red-500/10 border border-red-500/20 rounded-xl px-4 py-2.5">
            <span class="material-icons text-red-400 text-sm">block</span>
            <p class="text-red-400 text-xs font-medium">
              {{ alcoholDetected ? 'Alcohol detected — engine start blocked for safety'
                : !helmetPaired ? 'Helmet not connected — wear your helmet first'
                : 'Motorcycle module not connected' }}
            </p>
          </div>

          <!-- Pending feedback -->
          <div v-if="enginePending"
            class="mt-3 flex items-center gap-2 bg-yellow-500/10 border border-yellow-500/20 rounded-xl px-4 py-2.5">
            <span class="material-icons text-yellow-400 text-sm animate-pulse">hourglass_top</span>
            <p class="text-yellow-400 text-xs font-medium">
              Command sent — waiting for motorcycle to respond...
            </p>
          </div>
        </div>
      </div>

      <!-- 📱 MOBILE: Collapsible Rider Status Cards -->
      <div class="mb-6">
        <button @click="showRiderCards = !showRiderCards" 
                class="w-full md:hidden flex items-center justify-between p-4 bg-white/5 backdrop-blur-lg rounded-2xl shadow-lg mb-4 border border-white/10">
          <div class="flex items-center gap-3">
            <span class="material-icons text-[#7091E6]">person</span>
            <span class="font-bold text-white">Rider Status</span>
          </div>
          <span class="material-icons text-white/40 transition-transform duration-300" 
                :class="showRiderCards ? 'rotate-180' : ''">expand_more</span>
        </button>
        
        <div :class="['grid grid-cols-2 sm:grid-cols-3 lg:grid-cols-5 gap-4 transition-all duration-300', 
                      showRiderCards ? 'grid' : 'hidden md:grid']">
        <!-- Rider Status -->
        <div :class="[
          'group relative overflow-hidden text-white rounded-2xl shadow-xl p-5 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer border',
          riderStatus === 'Inactive'
            ? 'bg-red-500/10 border-red-500/20 hover:bg-red-500/15'
            : 'bg-white/5 border-white/10 hover:bg-white/8'
        ]">
          <div class="absolute top-0 right-0 w-24 h-24 rounded-full -mr-12 -mt-12 blur-2xl opacity-30"
            :class="riderStatus === 'Inactive' ? 'bg-red-500' : 'bg-[#3D52A0]'"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-3">
              <div :class="['p-2 rounded-xl', riderStatus === 'Inactive' ? 'bg-red-500/20' : 'bg-white/10']">
                <span :class="['material-icons text-xl', riderStatus === 'Inactive' ? 'text-red-400' : 'text-[#7091E6]']">person</span>
              </div>
              <div :class="['w-2 h-2 rounded-full', riderStatus === 'Inactive' ? 'bg-red-400 animate-ping' : 'bg-green-400 animate-pulse']"></div>
            </div>
            <p class="text-xs text-white/40 mb-1">Rider Status</p>
            <p :class="['text-lg font-bold', riderStatus === 'Inactive' ? 'text-red-400' : 'text-white']">{{ riderStatus }}</p>
            <p class="text-xs text-white/30 mt-1">{{ riderSubtitle }}</p>
          </div>
        </div>
        
        <!-- Current Speed -->
        <div :class="[
          'group relative overflow-hidden text-white rounded-2xl shadow-xl p-5 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer border',
          isOverSpeed
            ? 'bg-red-500/10 border-red-500/20 hover:bg-red-500/15'
            : 'bg-white/5 border-white/10 hover:bg-white/8'
        ]">
          <div class="absolute top-0 right-0 w-24 h-24 rounded-full -mr-12 -mt-12 blur-2xl opacity-30"
            :class="isOverSpeed ? 'bg-red-500' : 'bg-[#7091E6]'"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-3">
              <div :class="['p-2 rounded-xl', isOverSpeed ? 'bg-red-500/20' : 'bg-white/10']">
                <span :class="['material-icons text-xl', isOverSpeed ? 'text-red-400' : 'text-[#7091E6]']">speed</span>
              </div>
              <div :class="['w-2 h-2 rounded-full', isOverSpeed ? 'bg-red-400 animate-pulse' : 'bg-blue-400']"></div>
            </div>
            <p class="text-xs text-white/40 mb-1">Current Speed</p>
            <p :class="['text-lg font-bold', isOverSpeed ? 'text-red-400' : 'text-white']">{{ currentSpeedText }}</p>
            <p class="text-xs text-white/30 mt-1">{{ isOverSpeed ? 'Over limit!' : `Limit: ${speedLimit} km/h` }}</p>
          </div>
        </div>
        
        <!-- Alertness -->
        <div :class="[
          'group relative overflow-hidden text-white rounded-2xl shadow-xl p-5 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer border',
          alertnessStatus !== 'Normal'
            ? 'bg-yellow-500/10 border-yellow-500/20'
            : 'bg-white/5 border-white/10 hover:bg-white/8'
        ]">
          <div class="absolute top-0 right-0 w-24 h-24 rounded-full -mr-12 -mt-12 blur-2xl opacity-30 bg-[#8697C4]"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-3">
              <div class="p-2 rounded-xl bg-white/10">
                <span class="material-icons text-xl text-[#8697C4]">visibility</span>
              </div>
              <div :class="['w-2 h-2 rounded-full', alertnessStatus === 'Normal' ? 'bg-green-400' : 'bg-yellow-400 animate-pulse']"></div>
            </div>
            <p class="text-xs text-white/40 mb-1">Alertness</p>
            <p class="text-lg font-bold text-white">{{ alertnessStatus }}</p>
            <p class="text-xs text-white/30 mt-1">{{ alertnessSubtitle }}</p>
          </div>
        </div>
        
        <!-- Alcohol Detection -->
        <div :class="[
          'group relative overflow-hidden text-white rounded-2xl shadow-xl p-5 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer border',
          alcoholStatus !== 'Safe'
            ? 'bg-red-500/10 border-red-500/20 animate-pulse'
            : 'bg-white/5 border-white/10 hover:bg-white/8'
        ]">
          <div class="absolute top-0 right-0 w-24 h-24 rounded-full -mr-12 -mt-12 blur-2xl opacity-30"
            :class="alcoholStatus !== 'Safe' ? 'bg-red-500' : 'bg-[#3D52A0]'"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-3">
              <div :class="['p-2 rounded-xl', alcoholStatus !== 'Safe' ? 'bg-red-500/20' : 'bg-white/10']">
                <span :class="['material-icons text-xl', alcoholStatus !== 'Safe' ? 'text-red-400' : 'text-[#7091E6]']">local_bar</span>
              </div>
              <div :class="['w-2 h-2 rounded-full', alcoholStatus === 'Safe' ? 'bg-green-400' : 'bg-red-400 animate-pulse']"></div>
            </div>
            <p class="text-xs text-white/40 mb-1">Alcohol</p>
            <p :class="['text-lg font-bold', alcoholStatus !== 'Safe' ? 'text-red-400' : 'text-white']">{{ alcoholStatus }}</p>
            <p class="text-xs text-white/30 mt-1">{{ alcoholSubtitle }}</p>
          </div>
        </div>
        
        <!-- Crash Detection Card -->
        <div :class="[
          'group relative overflow-hidden text-white rounded-2xl shadow-xl p-5 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer border',
          crashDisplayStatus !== 'Stable'
            ? 'bg-red-500/10 border-red-500/20 animate-pulse'
            : 'bg-white/5 border-white/10 hover:bg-white/8'
        ]">
          <div class="absolute top-0 right-0 w-24 h-24 rounded-full -mr-12 -mt-12 blur-2xl opacity-30"
            :class="crashDisplayStatus !== 'Stable' ? 'bg-red-500' : 'bg-green-600'"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-3">
              <div :class="['p-2 rounded-xl', crashDisplayStatus !== 'Stable' ? 'bg-red-500/20' : 'bg-white/10']">
                <span :class="['material-icons text-xl', crashDisplayStatus !== 'Stable' ? 'text-red-400' : 'text-green-400']">warning</span>
              </div>
              <div :class="['w-2 h-2 rounded-full', crashDisplayStatus === 'Stable' ? 'bg-green-400' : 'bg-yellow-400 animate-pulse']"></div>
            </div>
            <p class="text-xs text-white/40 mb-1">Vehicle Status</p>
            <p :class="['text-lg font-bold', crashDisplayStatus !== 'Stable' ? 'text-red-400' : 'text-white']">{{ crashDisplayMessage }}</p>
            <p class="text-xs text-white/30 mt-1">{{ crashDisplayStatus === 'Stable' ? 'No incidents' : 'Alert active' }}</p>
          </div>
        </div>
        </div>
      </div>

      <!-- ✅ DEBUG PANEL REMOVED - Cleaner dashboard interface -->



      <!-- Speed Limit Display (read-only — set by Emergency Contact) -->
      <div class="relative overflow-hidden bg-white/5 border border-white/10 rounded-2xl shadow-xl p-6 mb-6">
        <div class="flex justify-between items-center mb-4">
          <div class="flex items-center gap-3">
            <div class="bg-white/10 p-2 rounded-xl">
              <span class="material-icons text-2xl text-[#7091E6]">speed</span>
            </div>
            <div>
              <p class="text-white font-bold">Speed Limit</p>
              <p class="text-white/40 text-xs flex items-center gap-1">
                <span class="material-icons text-xs">lock</span>
                Set by your emergency contact
              </p>
            </div>
          </div>
          <span class="text-2xl font-black text-white bg-white/10 px-4 py-2 rounded-xl border border-white/10">
            {{ speedLimit }} <span class="text-sm font-semibold text-white/60">km/h</span>
          </span>
        </div>

        <!-- Read-only progress bar -->
        <div class="w-full h-3 bg-white/10 rounded-full overflow-hidden relative">
          <!-- Fill -->
          <div class="h-full rounded-full transition-all duration-500"
            :style="{ width: `${(speedLimit / 120) * 100}%`, background: speedBarColor }"></div>
          <!-- Current speed indicator -->
          <div v-if="currentSpeed > 0"
            class="absolute top-0 h-full w-0.5 bg-white/60 transition-all duration-300"
            :style="{ left: `${Math.min((currentSpeed / 120) * 100, 100)}%` }"></div>
        </div>

        <div class="flex justify-between mt-2 text-xs text-white/25">
          <span>0</span><span>30</span><span>60</span><span>90</span><span>120 km/h</span>
        </div>

        <!-- Over-speed warning -->
        <div v-if="isOverSpeed"
          class="mt-3 flex items-center gap-2 bg-red-500/10 border border-red-400/20 rounded-xl px-3 py-2">
          <span class="material-icons text-red-400 text-sm animate-pulse">warning</span>
          <p class="text-red-400 text-xs font-semibold">
            Current speed ({{ currentSpeed.toFixed(0) }} km/h) exceeds the limit set by your emergency contact!
          </p>
        </div>

        <div class="mt-3 flex items-center gap-2">
          <div :class="['w-2 h-2 rounded-full', speedZoneColor]"></div>
          <span class="text-white/40 text-xs">{{ speedZoneLabel }}</span>
        </div>
      </div>

      <!-- Tabs (Location vs Speed) -->
      <div class="mb-4 relative z-10">
        <div class="bg-[#0B2540]/80 backdrop-blur-md rounded-2xl p-1.5 border border-white/10 flex gap-1.5">
          <button v-for="tab in ['My Location', 'Speed Data']" :key="tab"
            @click="activeTab = tab"
            :class="['flex-1 px-4 py-2.5 rounded-xl font-bold text-sm transition-all duration-300',
              activeTab === tab
                ? 'bg-[#00B85C] text-white shadow-lg shadow-[#00B85C]/30'
                : 'text-slate-400 hover:text-white hover:bg-white/5']">
            {{ tab }}
          </button>
        </div>
      </div>

      <!-- Tab Content -->
      <div id="map" class="relative z-0">
        <div v-show="activeTab === 'My Location'" class="bg-[#0B2540]/75 backdrop-blur-md rounded-2xl shadow-2xl p-4 sm:p-6 md:p-8 mb-6 border border-white/10 hover:border-[#00B85C]/30 transition-all duration-300">
          <LocationSection :location="location" :user="user" :crash-events="crashEvents" :crash-detected="crashDisplayStatus !== 'Stable' || !!sensorData.crashDetected" @update-location="handleLocationUpdate" />
        </div>
        <div v-show="activeTab === 'Speed Data'" class="bg-[#0B2540]/75 backdrop-blur-md rounded-2xl shadow-2xl p-4 sm:p-6 md:p-8 mb-6 border border-white/10 hover:border-[#00B85C]/30 transition-all duration-300">
          <SpeedDataSection
            :currentSpeed="currentSpeed"
            :speedLimit="speedLimit"
            :speedHistory="speedHistory"
            @overspeed="handleOverspeed"
          />
        </div>
      </div>

      <!-- Recent Alerts Section -->
      <div id="alerts" class="relative overflow-hidden bg-white/5 backdrop-blur-lg rounded-2xl shadow-xl p-6 mb-6 border border-white/10 hover:border-white/20 transition-all duration-300">
        <div class="flex items-center justify-between mb-4">
          <div class="flex items-center gap-2">
            <span class="material-icons text-[#7091E6]">notifications_active</span>
            <h3 class="font-bold text-lg text-white">Recent Alerts</h3>
            <span v-if="alerts.length > 0"
              class="bg-red-500/80 text-white text-xs px-2 py-0.5 rounded-full font-bold">
              {{ alerts.length }}
            </span>
          </div>
          <div class="flex gap-2">
            <button v-if="showStopSoundButton" @click="stopSound"
              class="text-xs font-semibold px-3 py-1.5 rounded-lg bg-orange-500/20 border border-orange-400/30 text-orange-400 hover:bg-orange-500/30 transition-all animate-pulse flex items-center gap-1">
              <span class="material-icons text-sm">volume_off</span>Stop Sound
            </button>
            <button v-if="alerts.length > 0" @click="clearAllAlerts"
              class="text-xs font-semibold px-3 py-1.5 rounded-lg bg-red-500/20 border border-red-400/30 text-red-400 hover:bg-red-500/30 transition-all">
              Clear All
            </button>
            <button @click="toggleAlerts"
              class="text-xs font-semibold px-3 py-1.5 rounded-lg bg-white/5 border border-white/10 text-white/50 hover:bg-white/10 transition-all">
              {{ showAlerts ? 'Hide' : 'Show' }}
            </button>
          </div>
        </div>

        <!-- Alert list -->
        <div v-if="showAlerts">
          <!-- Empty state -->
          <div v-if="alerts.length === 0" class="text-center py-10">
            <span class="material-icons text-4xl text-white/10 mb-2 block">notifications_none</span>
            <p class="text-white/30 text-sm">No alerts yet</p>
            <p class="text-white/20 text-xs mt-1">Alerts from your motorcycle and helmet will appear here</p>
          </div>

          <!-- Alert items -->
          <div v-else class="space-y-2 max-h-80 overflow-y-auto custom-scrollbar pr-1">
            <div v-for="(alert, index) in alerts" :key="index"
              :class="['group flex items-start gap-3 p-3 rounded-xl border transition-all duration-200 hover:scale-[1.01]',
                alert.type === 'danger' || alert.type === 'crash'
                  ? 'bg-red-500/10 border-red-500/20 hover:bg-red-500/15'
                  : alert.type === 'alcohol'
                    ? 'bg-orange-500/10 border-orange-500/20 hover:bg-orange-500/15'
                  : alert.type === 'warning' || alert.type === 'theft'
                    ? 'bg-yellow-500/10 border-yellow-500/20 hover:bg-yellow-500/15'
                  : alert.type === 'success'
                    ? 'bg-green-500/10 border-green-500/20 hover:bg-green-500/15'
                  : alert.type === 'speed'
                    ? 'bg-amber-500/10 border-amber-500/20 hover:bg-amber-500/15'
                  : 'bg-blue-500/10 border-blue-500/20 hover:bg-blue-500/15']">

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
                  alert.type === 'warning' ? 'text-yellow-400' :
                  alert.type === 'theft' ? 'text-yellow-400' :
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

              <!-- Severity badge & Dismiss button -->
              <div class="flex flex-col items-end gap-1 flex-shrink-0">
                <span :class="['px-2 py-0.5 rounded-full text-[10px] font-bold uppercase tracking-wider',
                  alert.type === 'danger' || alert.type === 'crash' ? 'bg-red-500/20 text-red-400 border border-red-500/30' :
                  alert.type === 'alcohol' || alert.type === 'theft' || alert.type === 'warning' ? 'bg-amber-500/20 text-amber-300 border border-amber-500/30' :
                  'bg-blue-500/20 text-blue-300 border border-blue-500/30']">
                  {{ alert.type === 'danger' || alert.type === 'crash' ? 'High' : (alert.type === 'alcohol' || alert.type === 'theft') ? 'Medium' : 'Info' }}
                </span>
                <button @click="alerts.splice(index, 1)"
                  class="opacity-0 group-hover:opacity-100 transition-opacity text-white/30 hover:text-red-400 p-0.5 rounded-lg hover:bg-red-500/10"
                  title="Dismiss alert">
                  <span class="material-icons text-sm">close</span>
                </button>
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- Recent Trips Preview -->
      <section id="trips" class="mb-6">
        <div class="relative overflow-hidden bg-[#0B2540]/75 backdrop-blur-md rounded-2xl shadow-xl p-6 border border-white/10 hover:border-[#00B85C]/30 transition-all duration-300">
          <h3 class="text-lg font-bold text-white mb-5 flex items-center gap-2">
            <span class="material-icons text-[#00B85C]">route</span>
            <span>Recent Trips</span>
          </h3>
          <div v-if="recentTrips.length > 0" class="relative space-y-3 max-h-64 overflow-y-auto custom-scrollbar">
            <div v-for="trip in recentTrips" :key="trip.id" 
              class="group relative overflow-hidden p-4 border border-white/10 rounded-xl hover:border-[#00B85C]/40 hover:bg-white/5 transition-all duration-300">
              <div class="space-y-1.5 mb-3">
                <p class="text-sm flex items-start gap-2 text-white/70">
                  <span class="material-icons text-[#00B85C] text-base">trip_origin</span>
                  <span><strong class="text-white/50">From:</strong> {{ trip.startLocationName || formatLatLng(trip.startLat, trip.startLng) }}</span>
                </p>
                <p class="text-sm flex items-start gap-2 text-white/70">
                  <span class="material-icons text-[#00B85C] text-base">location_on</span>
                  <span><strong class="text-white/50">To:</strong> {{ trip.endLocationName || formatLatLng(trip.endLat, trip.endLng) }}</span>
                </p>
                <p class="text-sm flex items-center gap-2 text-white/70">
                  <span class="material-icons text-[#7091E6] text-base">speed</span>
                  <span><strong class="text-white/50">Max:</strong> {{ trip.maxSpeed || 'N/A' }} km/h</span>
                </p>
              </div>
              <div class="flex gap-2">
                <a :href="getGoogleMapsLink(trip)" target="_blank" rel="noopener noreferrer"
                  class="flex-1 inline-flex items-center justify-center gap-1.5 px-3 py-2 bg-[#3D52A0]/30 border border-[#7091E6]/30 text-[#7091E6] text-xs font-semibold rounded-lg hover:bg-[#3D52A0]/50 transition-all">
                  <span class="material-icons text-sm">navigation</span>Navigate
                </a>
                <button @click="deleteTrip(trip.id)"
                  class="inline-flex items-center justify-center gap-1.5 px-3 py-2 bg-red-500/10 border border-red-400/20 text-red-400 text-xs font-semibold rounded-lg hover:bg-red-500/20 transition-all">
                  <span class="material-icons text-sm">delete</span>
                </button>
              </div>
            </div>
          </div>
          <div v-else class="text-center py-10">
            <span class="material-icons text-5xl text-white/10 mb-3 block">explore_off</span>
            <p class="text-white/30 font-medium">No trips recorded yet</p>
          </div>
        </div>
      </section>

      <!-- Crash Events -->
      <section class="mb-6" v-if="crashEvents.length > 0">
        <div class="relative overflow-hidden bg-red-500/5 backdrop-blur-lg rounded-2xl shadow-xl p-6 border border-red-500/20">
          <!-- Header -->
          <div class="flex items-center justify-between mb-4">
            <h3 class="font-bold text-lg text-red-400 flex items-center gap-2">
              <span class="material-icons text-red-400 animate-pulse">warning</span>
              <span>Crash Site Locations</span>
              <span class="bg-red-500/20 border border-red-400/30 text-red-400 text-xs px-2 py-0.5 rounded-full">{{ crashEvents.length }}</span>
            </h3>
            <div class="flex items-center gap-2">
              <button @click="clearAllCrashLocations"
                class="inline-flex items-center gap-1.5 px-3 py-1.5 bg-red-500/20 hover:bg-red-500/30 border border-red-500/30 text-red-300 hover:text-white text-xs font-bold rounded-xl transition-all"
                title="Dismiss all crash locations from view">
                <span class="material-icons text-xs">clear_all</span>
                <span>Dismiss All</span>
              </button>
              <button @click="clearAllCrashLocations"
                class="p-1.5 rounded-xl bg-white/10 hover:bg-white/20 text-white/60 hover:text-white transition-all border border-white/10 flex items-center justify-center"
                title="Close Crash Site Locations">
                <span class="material-icons text-base">close</span>
              </button>
            </div>
          </div>

          <div class="space-y-4">
            <div v-for="(event, index) in crashEvents" :key="index"
              class="relative overflow-hidden p-5 border border-red-500/20 rounded-xl bg-red-500/5 hover:bg-red-500/10 transition-all duration-300">

              <!-- Severity badge -->
              <div class="flex items-center justify-between mb-3">
                <span :class="['text-xs font-bold px-2 py-1 rounded-full border',
                  event.severity === 'High'
                    ? 'bg-red-500/20 border-red-400/40 text-red-400'
                    : 'bg-orange-500/20 border-orange-400/40 text-orange-400']">
                  {{ event.severity || 'Medium' }} Severity
                </span>
                <span class="text-xs text-white/30">{{ event.time || new Date(event.timestamp).toLocaleTimeString() }}</span>
              </div>

              <!-- Data rows -->
              <div class="grid grid-cols-2 gap-2 mb-4">
                <div class="bg-white/5 rounded-lg p-2">
                  <p class="text-xs text-white/40 mb-0.5">Impact Force</p>
                  <p class="text-sm font-bold text-white">{{ event.impactStrength || 'N/A' }} g</p>
                </div>
                <div class="bg-white/5 rounded-lg p-2">
                  <p class="text-xs text-white/40 mb-0.5">Lean Angle</p>
                  <p class="text-sm font-bold text-white">{{ event.roll ? event.roll.toFixed(1) + '°' : 'N/A' }}</p>
                </div>
                <div class="bg-white/5 rounded-lg p-2 col-span-2">
                  <p class="text-xs text-white/40 mb-0.5">GPS Location</p>
                  <p class="text-sm font-mono text-white/70">
                    {{ event.hasGPS ? `${Number(event.lat).toFixed(6)}, ${Number(event.lng).toFixed(6)}` : 'No GPS fix at time of crash' }}
                  </p>
                </div>
              </div>

              <!-- Action buttons -->
              <div class="flex flex-wrap gap-2">
                <!-- Navigate button — only if GPS available -->
                <a v-if="event.hasGPS && event.lat && event.lng"
                  :href="`https://www.google.com/maps?q=${event.lat},${event.lng}`"
                  target="_blank" rel="noopener noreferrer"
                  class="flex-1 inline-flex items-center justify-center gap-1.5 px-3 py-2.5 bg-blue-500/20 border border-blue-400/30 text-blue-400 text-xs font-bold rounded-lg hover:bg-blue-500/30 transition-all">
                  <span class="material-icons text-sm">navigation</span>Navigate to Site
                </a>
                <a v-if="event.hasGPS && event.lat && event.lng"
                  :href="`https://www.google.com/maps?q=${event.lat},${event.lng}`"
                  target="_blank" rel="noopener noreferrer"
                  class="inline-flex items-center justify-center gap-1.5 px-3 py-2.5 bg-green-500/10 border border-green-400/20 text-green-400 text-xs font-semibold rounded-lg hover:bg-green-500/20 transition-all">
                  <span class="material-icons text-sm">map</span>Maps
                </a>
                <div v-if="!event.hasGPS"
                  class="flex-1 inline-flex items-center justify-center gap-1.5 px-3 py-2.5 bg-white/5 border border-white/10 text-white/30 text-xs rounded-lg">
                  <span class="material-icons text-sm">gps_off</span>No GPS location recorded
                </div>
                <button @click="deleteCrashEvent(index)"
                  class="inline-flex items-center justify-center gap-1.5 px-3 py-2.5 bg-red-500/10 border border-red-400/20 text-red-400 text-xs font-semibold rounded-lg hover:bg-red-500/20 transition-all">
                  <span class="material-icons text-sm">delete</span>
                </button>
              </div>
            </div>
          </div>
        </div>
      </section>
    </main>

    <!-- 🚀 FLOATING ENGINE ACTION BUTTON (Mobile Only — positioned comfortably above bottom nav) -->
    <div class="fixed bottom-20 right-4 z-40 md:hidden">
      <button @click="handleEngineToggle" 
              :disabled="enginePending || (alcoholDetected && !engineRunning) || !helmetPaired || !motorcyclePaired"
              :class="[
                'w-14 h-14 rounded-full shadow-2xl flex items-center justify-center transition-all duration-300 transform hover:scale-105 active:scale-95 border-2',
                enginePending
                  ? 'bg-yellow-500/20 border-yellow-400/40 text-yellow-400 cursor-wait'
                  : engineRunning 
                    ? 'bg-red-500/20 border-red-400/50 text-red-400 animate-pulse' 
                    : (alcoholDetected || !helmetPaired || !motorcyclePaired)
                      ? 'bg-white/5 border-white/10 text-white/20 cursor-not-allowed'
                      : 'bg-green-500/20 border-green-400/50 text-green-400'
              ]"
              title="Quick Toggle Engine Power">
        <span :class="['material-icons text-2xl', enginePending ? 'animate-spin' : '']">
          {{ enginePending ? 'refresh' : 'power_settings_new' }}
        </span>
      </button>
    </div>

    <!-- ✅ Floating Stop Sound Button (Bottom Right — stacks above mobile quick button) -->
    <div v-if="showStopSoundButton" 
         class="fixed bottom-36 right-4 sm:bottom-8 sm:right-8 z-50">
      <button @click="stopSound"
              class="bg-gradient-to-r from-orange-500 to-red-500 text-white px-4 py-2.5 sm:px-5 sm:py-3 rounded-full shadow-2xl hover:from-orange-600 hover:to-red-600 transition-all duration-300 transform hover:scale-105 active:scale-95 animate-pulse border-2 border-white/80 backdrop-blur-md">
        <div class="flex items-center gap-1.5 sm:gap-2">
          <span class="material-icons text-lg sm:text-xl">volume_off</span>
          <span class="font-bold text-xs sm:text-sm">Stop Alert Sound</span>
        </div>
      </button>
    </div>
  
      <!-- ===== SOS CONFIRMATION MODAL ===== -->
      <div v-if="showSosConfirmModal" class="fixed inset-0 z-[9999] flex items-center justify-center p-4 bg-black/80 backdrop-blur-md animate-fadeIn">
        <div class="relative w-full max-w-md bg-[#0A1A2F] border-2 border-red-500/50 rounded-3xl p-6 shadow-2xl text-center overflow-hidden">
          <div class="absolute -top-20 -left-20 w-44 h-44 bg-red-600/30 rounded-full blur-3xl pointer-events-none"></div>
          <div class="absolute -bottom-20 -right-20 w-44 h-44 bg-rose-600/20 rounded-full blur-3xl pointer-events-none"></div>

          <div class="relative z-10">
            <div class="w-16 h-16 mx-auto mb-4 rounded-2xl bg-red-500/20 border border-red-400/40 flex items-center justify-center animate-pulse">
              <span class="material-icons text-red-400 text-4xl">emergency</span>
            </div>

            <h3 class="text-xl font-black text-white uppercase tracking-wider mb-2">Emergency SOS Alert</h3>
            <p class="text-white/70 text-sm mb-4">
              Are you in immediate danger or need emergency roadside assistance?
            </p>

            <div class="bg-white/5 border border-white/10 rounded-2xl p-3.5 mb-5 text-left">
              <p class="text-xs text-white/40 uppercase tracking-wide font-semibold mb-1 flex items-center gap-1.5">
                <span class="material-icons text-xs text-red-400">place</span> Broadcast Location
              </p>
              <p class="text-sm font-mono text-white/90">
                {{ location.lat ? `${location.lat.toFixed(6)}, ${location.lng.toFixed(6)}` : 'Resolving GPS location...' }}
              </p>
              <p class="text-[11px] text-white/50 mt-1">
                This alert will instantly notify all your registered emergency safety contacts with your live location.
              </p>
            </div>

            <div class="flex flex-col gap-2.5">
              <button @click="confirmAndSendSOS" :disabled="isSendingSos"
                class="w-full py-3.5 px-4 rounded-xl bg-gradient-to-r from-red-600 to-rose-700 hover:from-red-700 hover:to-rose-800 text-white font-bold text-sm uppercase tracking-wider shadow-lg shadow-red-500/30 transition-all flex items-center justify-center gap-2 disabled:opacity-50">
                <span class="material-icons text-lg">crisis_alert</span>
                <span>{{ isSendingSos ? 'Broadcasting SOS...' : 'Send SOS Alert Now' }}</span>
              </button>
              <button @click="showSosConfirmModal = false" :disabled="isSendingSos"
                class="w-full py-3 px-4 rounded-xl bg-white/10 hover:bg-white/15 text-white/70 hover:text-white font-semibold text-sm transition-all border border-white/10">
                Cancel
              </button>
            </div>
          </div>
        </div>
      </div>

      <!-- ===== SOS SENT SUCCESS MODAL ===== -->
      <div v-if="showSosSentModal" class="fixed inset-0 z-[9999] flex items-center justify-center p-4 bg-black/80 backdrop-blur-md animate-fadeIn">
        <div class="relative w-full max-w-md bg-[#0A1A2F] border-2 border-emerald-500/50 rounded-3xl p-6 shadow-2xl text-center overflow-hidden">
          <div class="absolute -top-20 -left-20 w-44 h-44 bg-emerald-600/30 rounded-full blur-3xl pointer-events-none"></div>

          <div class="relative z-10">
            <div class="w-16 h-16 mx-auto mb-4 rounded-2xl bg-emerald-500/20 border border-emerald-400/40 flex items-center justify-center">
              <span class="material-icons text-emerald-400 text-4xl">check_circle</span>
            </div>

            <h3 class="text-xl font-black text-white uppercase tracking-wider mb-2">SOS Broadcast Active</h3>
            <p class="text-white/80 text-sm mb-4">
              Your emergency contacts have been notified with your live coordinates.
            </p>

            <div class="flex flex-col gap-2.5 mb-2">
              <a href="tel:911"
                class="w-full py-3.5 px-4 rounded-xl bg-gradient-to-r from-blue-600 to-indigo-700 hover:from-blue-700 hover:to-indigo-800 text-white font-bold text-sm uppercase tracking-wider shadow-lg transition-all flex items-center justify-center gap-2">
                <span class="material-icons text-lg">phone</span>
                <span>Call 911 Hotline</span>
              </a>
              <button @click="resolveSos"
                class="w-full py-3 px-4 rounded-xl bg-emerald-500/20 hover:bg-emerald-500/30 text-emerald-300 border border-emerald-500/30 font-bold text-sm transition-all">
                I Am Safe (Cancel SOS)
              </button>
              <button @click="showSosSentModal = false"
                class="w-full py-2.5 px-4 rounded-xl bg-white/10 hover:bg-white/15 text-white/60 hover:text-white font-medium text-xs transition-all">
                Close Window
              </button>
            </div>
          </div>
        </div>
      </div>

      <!-- ===== SOS ERROR MODAL ===== -->
      <div v-if="showSosErrorModal" class="fixed inset-0 z-[9999] flex items-center justify-center p-4 bg-black/80 backdrop-blur-md animate-fadeIn">
        <div class="relative w-full max-w-md bg-[#0A1A2F] border-2 border-red-500/50 rounded-3xl p-6 shadow-2xl text-center overflow-hidden">
          <div class="relative z-10">
            <div class="w-16 h-16 mx-auto mb-4 rounded-2xl bg-red-500/20 border border-red-400/40 flex items-center justify-center">
              <span class="material-icons text-red-400 text-4xl">error</span>
            </div>
            <h3 class="text-xl font-black text-white uppercase tracking-wider mb-2">SOS Notice</h3>
            <p class="text-white/80 text-sm mb-5">{{ sosErrorMessage }}</p>
            <div class="flex flex-col gap-2.5">
              <a href="tel:911" class="w-full py-3.5 px-4 rounded-xl bg-red-600 hover:bg-red-700 text-white font-bold text-sm uppercase tracking-wider shadow-lg transition-all flex items-center justify-center gap-2">
                <span class="material-icons text-lg">phone</span>
                <span>Call 911 Emergency</span>
              </a>
              <button @click="showSosErrorModal = false" class="w-full py-2.5 px-4 rounded-xl bg-white/10 text-white/70 hover:text-white font-medium text-xs">
                Dismiss
              </button>
            </div>
          </div>
        </div>
      </div>

      <!-- ===== INCOMING SOS MODAL (For Rider when Emergency Contact triggers SOS) ===== -->
      <div v-if="showIncomingSosModal" class="fixed inset-0 z-[9999] flex items-center justify-center p-4 bg-black/85 backdrop-blur-md animate-fadeIn">
        <div class="relative w-full max-w-md bg-[#0A1A2F] border-2 border-red-500 rounded-3xl p-6 shadow-2xl text-center overflow-hidden animate-pulse">
          <div class="relative z-10">
            <div class="w-16 h-16 mx-auto mb-4 rounded-2xl bg-red-600 flex items-center justify-center text-white shadow-lg">
              <span class="material-icons text-4xl">notification_important</span>
            </div>

            <h3 class="text-xl font-black text-red-400 uppercase tracking-wider mb-2">Incoming Emergency SOS!</h3>
            <p class="text-white/90 text-sm mb-4 font-medium">
              Your emergency contact has triggered an urgent safety check on you!
            </p>

            <div v-if="incomingSosData?.emergencyContactLocation?.lat" class="bg-white/5 border border-white/10 rounded-2xl p-3 mb-5 text-left text-xs">
              <p class="text-white/40 mb-1 font-semibold">Contact's Location:</p>
              <p class="font-mono text-white/90">
                {{ incomingSosData.emergencyContactLocation.lat.toFixed(6) }}, {{ incomingSosData.emergencyContactLocation.lng.toFixed(6) }}
              </p>
              <a :href="`https://www.google.com/maps?q=${incomingSosData.emergencyContactLocation.lat},${incomingSosData.emergencyContactLocation.lng}`" target="_blank"
                class="mt-2 inline-flex items-center gap-1 text-blue-400 hover:underline">
                <span class="material-icons text-xs">open_in_new</span> View Contact on Google Maps
              </a>
            </div>

            <div class="flex flex-col gap-2.5">
              <button @click="acknowledgeIncomingSos"
                class="w-full py-3.5 px-4 rounded-xl bg-emerald-600 hover:bg-emerald-700 text-white font-bold text-sm uppercase tracking-wider shadow-lg transition-all flex items-center justify-center gap-2">
                <span class="material-icons text-base">verified</span>
                <span>I Am Safe / Acknowledge</span>
              </button>
            </div>
          </div>
        </div>
      </div>
  </div>
</template>

<script setup>
import { ref, computed, onMounted, onBeforeUnmount, watch, nextTick } from 'vue';
import { useAuthStore } from '../stores/auth';
import { database } from '../firebase/config';
import { ref as dbRef, set, update, onValue, onChildAdded, remove, get } from 'firebase/database';

// Components
import TabGroup from '../components/TabGroup.vue';
import DashboardCard from '../components/DashboardCard.vue';
import LocationSection from '../components/LocationSection.vue';
import SpeedDataSection from '../components/SpeedDataSection.vue';
import RecentAlerts from '../components/RecentAlerts.vue';

const authStore = useAuthStore();

// States
const riderStatus = ref('Inactive');
const riderSubtitle = ref('Helmet not connected');
const alertnessStatus = ref('Normal');
const alertnessSubtitle = ref('No drowsiness detected');
const alcoholStatus = ref('Safe');
const alcoholSubtitle = ref('No alcohol detected');
const currentSpeed = ref(0);
const speedHistory = ref([]);
const speedLimit = ref(80); // Default value

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
  if (speedLimit.value <= 30) return 'School / Residential Zone';
  if (speedLimit.value <= 60) return 'City / Urban Zone';
  if (speedLimit.value <= 90) return 'Highway Zone';
  return 'Expressway Zone';
});

const diagnostics = ref([]);
const alerts = ref([]);
const activeTab = ref('My Location');
const location = ref({ lat: null, lng: null });
const user = computed(() => ({
  name: authStore.user?.displayName || authStore.user?.email?.split('@')[0] || 'Rider',
  email: authStore.user?.email || '',
  photoURL: authStore.photoURL || authStore.user?.photoURL || ''
}));
const recentTrips = ref([]);
const crashEvents = ref([]);
const isOverSpeed = ref(false);
const showAlerts = ref(true);
const showStopSoundButton = ref(false);  // ✅ NEW: Control stop sound button visibility

// New states for enhanced features
// ✅ STABLE states (used for display - debounced)
const helmetPaired = ref(false);
const motorcyclePaired = ref(false);

// ✅ FIX: Track when we last received Firebase updates (not Arduino timestamps!)
const lastHelmetUpdate = ref(0);
const lastMotorcycleUpdate = ref(0);

// ✅ RAW states (immediate sensor readings - not displayed)
const helmetRawConnected = ref(false);
const motorcycleRawConnected = ref(false);

const deviceBattery = ref(85);
const gsmConnected = ref(true);
const gpsConnected = ref(true);
const hasHardwareGpsFix = ref(false);

// ✅ Debouncing timers

// ✅ NEW: Anti-theft alert state
const antiTheftAlert = ref({
  active: false,
  message: '',
  time: '',
  level: 0,
  severity: 'low',
  timestamp: 0
});
const helmetDisconnectTimer = ref(null);
const motorcycleDisconnectTimer = ref(null);
const helmetReconnectTimer = ref(null);
const motorcycleReconnectTimer = ref(null);
const DISCONNECT_DELAY = 10000; // 10 seconds before showing disconnected
const RECONNECT_DELAY = 2000; // 2 seconds before showing reconnected
let helmetAlertShown = false;
let motorcycleAlertShown = false;

// ✅ NEW: Weather data for riding conditions
const weather = ref({
  loading: true,
  error: null,
  location: 'Loading...',
  temperature: 0,
  feelsLike: 0,
  description: '',
  condition: '',
  windSpeed: 0,
  humidity: 0,
  visibility: 0,
  lastUpdate: ''
});

// Crash UI
const crashDisplayStatus = ref('Stable'); // Stable | Alerting
const crashDisplayMessage = ref('Vehicle Stable');

// GPS Speed Monitoring
const isGPSSpeedActive = ref(false);
const gpsSpeedSource = ref('module'); // 'module' or 'phone'
let gpsWatchId = null;

let crashInterval = null;
let flashCount = 0;
let lastCrashTimestamp = null;

// ✅ FIXED: Use authenticated user's UID dynamically
const userId = computed(() => authStore.user?.uid || null);

// Sensor Data Tracking
const sensorData = ref({
  mpu6050: {
    accelX: 0,
    accelY: 0,
    accelZ: 0,
    gyro: 0,
    lastUpdate: Date.now()
  },
  gps: {
    accuracy: '5m',
    satellites: 8
  },
  gsm: {
    signal: '85%',
    network: '4G'
  },
  engineRunning: false, // ✅ Engine status from relay
  crashDetected: false, // ✅ Crash detection status
  antiTheftArmed: false, // ✅ Anti-theft armed status
  alcohol: {
    value: 0,
    lastUpdate: Date.now()
  },
  battery: {
    voltage: '12.6V',
    health: '95%'
  }
});

// Trip Statistics
const tripStats = ref({
  distance: '0.0',
  avgSpeed: '0',
  maxSpeed: '0',
  duration: '0',
  safetyScore: 100,
  harshBraking: 0,
  rapidAccel: 0,
  sharpTurns: 0
});

let tripStartTime = null;
let tripDistance = 0;
let speedSum = 0;
let speedCount = 0;

// ✅ FIX: previousHelmetState must be a ref to be accessible in template
const previousHelmetState = ref(null);

// ✅ NEW: Engine Control States
const engineRunning = ref(false);
const enginePending = ref(false);  // optimistic pending state — shows spinner during Firebase round-trip
const autoEngineControl = ref(false);
const alcoholDetected = ref(false);

// ✅ NEW: Mobile Navigation
const activeSection = ref('status');
const showRiderCards = ref(false);

// Helpers
const formatLatLng = (lat, lng) => {
  return lat && lng ? `${Number(lat).toFixed(6)}, ${Number(lng).toFixed(6)}` : 'N/A';
};

const currentSpeedText = computed(() => currentSpeed.value.toFixed(2) + ' kph');

const getGoogleMapsLink = (tripOrLat, lng = undefined) => {
  // ✅ FIX: Add safety check for undefined trip data
  if (!tripOrLat) return 'https://www.google.com/maps';
  
  let startLat, startLng, endLat, endLng;
  if (lng === undefined) {
    // ✅ FIX: Check if trip object has required properties
    if (!tripOrLat.startLat || !tripOrLat.startLng) return 'https://www.google.com/maps';
    
    startLat = parseFloat(tripOrLat.startLat);
    startLng = parseFloat(tripOrLat.startLng);
    endLat = parseFloat(tripOrLat.endLat);
    endLng = parseFloat(tripOrLat.endLng);
  } else {
    startLat = parseFloat(tripOrLat);
    startLng = parseFloat(lng);
    endLat = startLat;
    endLng = startLng;
  }
  const isValidCoord = (lat, lng) =>
    lat !== undefined && lng !== undefined && !isNaN(lat) && !isNaN(lng);
  if (!isValidCoord(startLat, startLng)) return 'https://www.google.com/maps'; 
  const zoomLevel = 14;
  return `https://www.google.com/maps/dir/?api=1&origin=${startLat},${startLng}&destination=${endLat},${endLng}&zoom=${zoomLevel}`;
};

// ✅ ENHANCED: Audio control with persistent stop & mute functionality
let currentAudio = null;
const isAudioMuted = ref(false);
const lastUserMuteTime = ref(0);
const lastPlayedAlertType = ref('');
const lastPlayedAlertTime = ref(0);

const playSound = (alertType = 'general') => {
  try {
    const now = Date.now();
    // If user clicked Stop Sound or Dismissed alert within the last 30s, suppress repeated alarms
    if (isAudioMuted.value && (now - lastUserMuteTime.value < 30000)) {
      console.log(`[AUDIO] Audio muted by user, suppressing ${alertType} sound`);
      return;
    }

    // Debounce repeated sound triggers within 3 seconds
    if (lastPlayedAlertType.value === alertType && (now - lastPlayedAlertTime.value < 3000)) {
      return;
    }

    // Stop any currently playing audio
    if (currentAudio) {
      try {
        currentAudio.pause();
        currentAudio.currentTime = 0;
      } catch (e) { /* ignore */ }
      currentAudio = null;
    }
    
    lastPlayedAlertType.value = alertType;
    lastPlayedAlertTime.value = now;

    // Create and play new audio
    currentAudio = new Audio('/sounds/alert.mp3');
    currentAudio.play().catch(err => {
      console.warn("Audio playback failed (browser policy or file missing):", err);
    });
    
    // Show stop button when audio starts
    showStopSoundButton.value = true;
    
    // Auto-hide stop button when audio ends
    currentAudio.addEventListener('ended', () => {
      showStopSoundButton.value = false;
      currentAudio = null;
    });
    
  } catch (err) {
    console.error("Error creating audio object:", err);
  }
};

// ✅ Stop sound function with mute persistence
const stopSound = () => {
  if (currentAudio) {
    try {
      currentAudio.pause();
      currentAudio.currentTime = 0;
    } catch (e) { /* ignore */ }
    currentAudio = null;
  }
  showStopSoundButton.value = false;
  isAudioMuted.value = true;
  lastUserMuteTime.value = Date.now();

  // Also halt flashing crash interval if running
  if (crashInterval) {
    clearInterval(crashInterval);
    crashInterval = null;
  }
  flashCount = 0;
  console.log('[AUDIO] Sound stopped & muted by user');
};

// Flash Crash Message (visual flash only — sound is triggered once by the state change)
const flashCrashMessage = () => {
  console.log('[FLASH CRASH] 🚨🚨🚨 Starting crash alert animation...');
  if (crashInterval) {
    clearInterval(crashInterval);
    crashInterval = null;
  }
  flashCount = 0;
  
  // Set crash status immediately
  crashDisplayStatus.value = 'Alerting';
  crashDisplayMessage.value = 'Crash Detected';
  
  // Add alert to list (only if not dismissed)
  const alertTime = Date.now();
  if (shouldShowAlert(alertTime)) {
    alerts.value.unshift({
      type: 'danger',
      message: '🚨 CRASH DETECTED!',
      details: 'Impact detected - Check vehicle status',
      time: new Date().toLocaleTimeString(),
      timestamp: alertTime
    });
    if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
  } else {
    console.log('[ALERTS] Crash alert dismissed (older than dismissal time)');
  }
  
  // Flash animation - stays visible longer (10 flashes = 20 seconds)
  crashInterval = setInterval(() => {
    if (flashCount >= 10) {  // Flash 10 times (20 seconds total)
      clearInterval(crashInterval);
      crashInterval = null;
      crashDisplayStatus.value = 'Stable';
      crashDisplayMessage.value = 'Vehicle Stable';
      stopSound();
      console.log('[FLASH CRASH] ✓ Animation complete, returning to Stable');
      return;
    }
    
    // Toggle message
    crashDisplayMessage.value = crashDisplayMessage.value === 'Crash Detected' ? '⚠️ CHECK VEHICLE' : 'Crash Detected';
    flashCount++;
  }, 2000);  // Flash every 2 seconds
};

// Handle Overspeed Event
const handleOverspeed = (payload) => {
  const alertTime = Date.now();
  if (!shouldShowAlert(alertTime)) {
    console.log('[ALERTS] Overspeed alert dismissed');
    return;
  }
  
  playSound(); // Triggers alert sound
  alerts.value.unshift({
    type: 'speed',
    message: '⚡ Speed Limit Exceeded!',
    details: `Current: ${payload.speed} kph | Limit: ${payload.limit} kph`,
    time: new Date().toLocaleTimeString(),
    timestamp: alertTime
  });
  if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
};

// ✅ DEBUG: Watch for alcohol status changes
watch(alcoholStatus, (newVal, oldVal) => {
  console.log(`🔍 [WATCHER] alcoholStatus changed from "${oldVal}" to "${newVal}"`);
  console.trace('Stack trace for alcoholStatus change:');
});

watch(alcoholSubtitle, (newVal, oldVal) => {
  console.log(`🔍 [WATCHER] alcoholSubtitle changed from "${oldVal}" to "${newVal}"`);
});

watch(riderStatus, (newVal, oldVal) => {
  console.log(`🔍 [WATCHER] riderStatus changed from "${oldVal}" to "${newVal}"`);
});

watch(alertnessStatus, (newVal, oldVal) => {
  console.log(`🔍 [WATCHER] alertnessStatus changed from "${oldVal}" to "${newVal}"`);
});

// ✅ NEW: Watch for pairing status changes and update rider status accordingly
watch([helmetPaired, motorcyclePaired], ([newHelmet, newMotorcycle], [oldHelmet, oldMotorcycle]) => {
  console.log(`🔍 [WATCHER] Pairing changed - Helmet: ${oldHelmet} → ${newHelmet}, Motorcycle: ${oldMotorcycle} → ${newMotorcycle}`);
  
  // Update rider status based on pairing
  const bothDevicesOn = newHelmet && newMotorcycle;
  riderStatus.value = bothDevicesOn ? 'Active' : 'Inactive';
  
  if (bothDevicesOn) {
    riderSubtitle.value = 'Helmet connected';
  } else if (!newHelmet && !newMotorcycle) {
    riderSubtitle.value = 'Both devices disconnected';
  } else if (!newHelmet) {
    riderSubtitle.value = 'Helmet not connected';
  } else {
    riderSubtitle.value = 'Motorcycle not connected';
  }
  
  console.log(`🔍 [WATCHER] Rider Status updated to: ${riderStatus.value} - ${riderSubtitle.value}`);
});

// ✅ FIXED: Firebase References - now reactive based on authenticated user
const helmetPublicRef = computed(() => userId.value ? dbRef(database, `helmet_public/${userId.value}`) : null);
const helmetRef = computed(() => userId.value ? dbRef(database, `helmet_public/${userId.value}/helmetStatus/status`) : null);
const tripsRef = computed(() => userId.value ? dbRef(database, `helmet_public/${userId.value}/trips`) : null);
const crashRef = computed(() => userId.value ? dbRef(database, `helmet_public/${userId.value}/crashes`) : null);
const alcoholRef = computed(() => userId.value ? dbRef(database, `helmet_public/${userId.value}/alcohol/status`) : null);
const speedLimitRef = computed(() => userId.value ? dbRef(database, `helmet_public/${userId.value}/settings/speedLimit`) : null);

// ✅ Track app start time to ignore old crashes
const appStartTime = ref(Date.now());

// ✅ FIXED: Setup Firebase listeners only when user is authenticated
const setupFirebaseListeners = () => {
  if (!userId.value) {
    console.warn('[INIT] No user ID available, skipping Firebase setup');
    return;
  }
  
  console.log('[INIT] Setting up Firebase listeners for user:', userId.value);
  
  const storedLastCrashTime = localStorage.getItem(`lastCrashTimestamp_${userId.value}`);
  if (storedLastCrashTime) lastCrashTimestamp = parseInt(storedLastCrashTime);

  onValue(speedLimitRef.value, (snapshot) => {
    const data = snapshot.val();
    if (data !== null) speedLimit.value = data;
  });

  // ✅ Note: Device connection monitoring (helmet & motorcycle) is unified in setupDeviceListeners()

  // Listen for device health
  const deviceHealthRef = dbRef(database, `helmet_public/${userId.value}/deviceHealth`);
  onValue(deviceHealthRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      deviceBattery.value = data.battery || 85;
      gsmConnected.value = data.gsm !== false;
      gpsConnected.value = data.gps !== false;
    }
  });

  // ✅ NEW: Fetch weather data for riding conditions
  fetchWeather();

  initializeCrashListener();

  // ✅ Listen for SOS alerts from Emergency Contact
  const sosRef = dbRef(database, `helmet_public/${userId.value}/sos`);
  const seenSosKeys = new Set();
  let initialSosLoadDone = false;
  setTimeout(() => { initialSosLoadDone = true; }, 3000);

  onChildAdded(sosRef, (snapshot) => {
    const key = snapshot.key;
    const sosData = snapshot.val();
    if (!sosData) return;

    // Skip pre-existing SOS entries during initial load
    if (!initialSosLoadDone) {
      seenSosKeys.add(key);
      return;
    }
    if (seenSosKeys.has(key)) return;
    seenSosKeys.add(key);

    // Only handle SOS triggered by emergency contact
    if (sosData.triggeredBy !== 'emergency_contact') return;

    console.log('[SOS] 🆘 Emergency Contact triggered SOS:', sosData);

    incomingSosData.value = {
      key,
      ...sosData,
      time: new Date(sosData.timestamp || Date.now()).toLocaleTimeString()
    };
    showIncomingSosModal.value = true;
    playSound('danger');

    addAlert('danger', '🆘 EMERGENCY CONTACT SOS!',
      `Emergency contact has triggered SOS alert. ${
        sosData.emergencyContactLocation?.lat
          ? `Their location: ${sosData.emergencyContactLocation.lat.toFixed(6)}, ${sosData.emergencyContactLocation.lng.toFixed(6)}`
          : ''
      }`
    );

    // Browser notification
    if ('Notification' in window && Notification.permission === 'granted') {
      new Notification('🆘 Emergency Contact SOS', {
        body: 'Your emergency contact has triggered an SOS alert!',
        icon: '/favicon.ico',
        tag: 'sos-alert'
      });
    }
  });

  // ✅ ENHANCED: Real-time Alcohol Detection Listener with Force Update
  if (!alcoholRef.value) {
    console.error('[ALCOHOL] alcoholRef is null, cannot set up listener');
    return;
  }
  onValue(alcoholRef.value, (snapshot) => {
    const data = snapshot.val();
    const timestamp = new Date().toLocaleTimeString();
    
    console.log('━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━');
    console.log(`[${timestamp}] 🍺 ALCOHOL LISTENER TRIGGERED`);
    console.log('[DEBUG] Raw Firebase Data:', JSON.stringify(data, null, 2));
    console.log(`[DEBUG] Current alcoholStatus BEFORE update: ${alcoholStatus.value}`);
    console.log(`[DEBUG] Current alcoholSubtitle BEFORE update: ${alcoholSubtitle.value}`);
    
    if (data) {
      const sensorValue = data.sensorValue || 0;
      const status = data.status || 'Safe';
      const threshold = 2000;
      
      console.log(`[ALCOHOL] Sensor Value: ${sensorValue}`);
      console.log(`[ALCOHOL] Threshold: ${threshold}`);
      console.log(`[ALCOHOL] Status from Firebase: "${status}"`);
      console.log(`[ALCOHOL] Is Danger: ${status === "Danger"}`);
      console.log(`[ALCOHOL] Exact comparison: "${status}" === "Danger" = ${status === "Danger"}`);
      
      // ✅ Force update sensor data
      sensorData.value.alcohol = {
        value: sensorValue,
        lastUpdate: Date.now()
      };
      
      // ✅ Determine alcohol danger status
      const isAlcoholDanger = status === "Danger" || status === "danger" || status === "DANGER" || data.detected === true || (sensorValue >= 1500 && sensorValue > 0);
      
      // ✅ Update alcohol status with force reactivity using nextTick
      if (isAlcoholDanger) {
        console.log('🚨🚨🚨 ALCOHOL DANGER DETECTED! 🚨🚨🚨');
        console.log(`   Value ${sensorValue} indicates alcohol danger!`);
        
        // Force update refs with immediate assignment
        alcoholStatus.value = 'Danger';
        alcoholSubtitle.value = `Alcohol Detected! Value: ${sensorValue}`;
        alcoholDetected.value = true;  // ✅ FIX: Update alcoholDetected for engine control
        
        console.log(`[UPDATE] alcoholStatus AFTER update: ${alcoholStatus.value}`);
        console.log(`[UPDATE] alcoholSubtitle AFTER update: ${alcoholSubtitle.value}`);
        console.log(`[UPDATE] alcoholDetected AFTER update: ${alcoholDetected.value}`);
        
        // 🛑 AUTOMATIC SAFETY ENGINE SHUTDOWN: If engine is running, cut ignition immediately!
        if (engineRunning.value) {
          console.log('[SAFETY] 🚨 Alcohol threshold exceeded while engine is running! Shutting down engine automatically...');
          engineRunning.value = false;
          enginePending.value = false;
          
          stopSound();
          playSound();
          
          endTripSessionAndSave();
          
          const uid = userId.value || authStore.user?.uid || 'DMIARybyNHVhzwAmrx2QQjdpSb13';
          set(dbRef(database, `${uid}/engineControl/command`), 'STOP');
          set(dbRef(database, `${uid}/engineControl/stopButton`), true);
          set(dbRef(database, `${uid}/engineControl/startButton`), false);
          set(dbRef(database, `${uid}/engineControl/engineRunning`), false);
          set(dbRef(database, `helmet_public/${uid}/live/engineRunning`), false);
          set(dbRef(database, `helmet_public/${uid}/devices/motorcycle/engineRunning`), false);
          
          alerts.value.unshift({
            type: 'alcohol',
            message: '🛑 Engine Shut Down — Alcohol Detected!',
            details: `Engine was automatically stopped for rider safety (Alcohol level: ${sensorValue}). DO NOT RIDE!`,
            time: new Date().toLocaleTimeString()
          });
          if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
        }
        
        // Add alcohol alert - only if this is a NEW detection (after app start)
        const alcoholAlertTime = data.timestamp || Date.now();
        const isNewAlcoholAlert = alcoholAlertTime > appStartTime.value && shouldShowAlert(alcoholAlertTime);
        
        if (isNewAlcoholAlert) {
          // Add severe intoxication alert
          if (sensorValue >= severeThreshold) {
            alerts.value.unshift({
              type: 'alcohol',
              message: '⚠️ SEVERE INTOXICATION - DROWSINESS RISK!',
              details: `Alcohol level ${sensorValue} indicates high drowsiness risk. DO NOT RIDE!`,
              time: timestamp
            });
            playSound();
          }
          
          const newAlert = {
            type: 'alcohol',
            message: '🚨 Alcohol Detected!',
            details: `Sensor Value: ${sensorValue} (Threshold: ${threshold})`,
            time: new Date().toLocaleTimeString()
          };
          alerts.value.unshift(newAlert);
          console.log('[ALERT] ✅ Alcohol alert added:', newAlert);
          playSound();
          if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
        } else {
          console.log('[ALERT] Skipping old/dismissed alcohol alert (status persisted from before app start)');
          // Still update severe intoxication alertness state without adding alert
          if (sensorValue >= severeThreshold) {
            alertnessStatus.value = 'Drowsy';
            alertnessSubtitle.value = `Severe intoxication detected! Value: ${sensorValue}`;
          }
        }
      } else {
        console.log('✓ Alcohol status: SAFE');
        alcoholStatus.value = 'Safe';
        alcoholSubtitle.value = `No alcohol detected (Value: ${sensorValue})`;
        alcoholDetected.value = false;  // ✅ FIX: Update alcoholDetected for engine control
        
        // ✅ Reset alertness when alcohol is safe
        alertnessStatus.value = 'Normal';
        alertnessSubtitle.value = 'No drowsiness detected';
        
        console.log(`[UPDATE] alcoholStatus AFTER update: ${alcoholStatus.value}`);
        console.log(`[UPDATE] alcoholSubtitle AFTER update: ${alcoholSubtitle.value}`);
        console.log(`[UPDATE] alcoholDetected AFTER update: ${alcoholDetected.value}`);
        console.log(`[UPDATE] alertnessStatus AFTER update: ${alertnessStatus.value}`);
      }
      
      console.log('[SUCCESS] Alcohol card should now update on dashboard');
      console.log(`[FINAL CHECK] alcoholStatus.value = "${alcoholStatus.value}"`);
      console.log(`[FINAL CHECK] alcoholSubtitle.value = "${alcoholSubtitle.value}"`);
    } else {
      console.warn('⚠️ [WARNING] No alcohol data received from Firebase');
      console.warn('   Check if helmet module is sending data to /alcohol/status');
    }
    console.log('━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n');
  });

  if (!helmetPublicRef.value) {
    console.error('[HELMET_PUBLIC] helmetPublicRef is null, cannot set up listener');
    return;
  }
  let isInitialLiveCrashCheck = true;
  onValue(helmetPublicRef.value, (snapshot) => {
    const data = snapshot.val();
    if (data?.live) {
      const liveData = data.live;
      const rawLat = Number(liveData.locationLat);
      const rawLng = Number(liveData.locationLng);
      const isGpsValid = Boolean(liveData.gpsValid);
      const hasHardwareFix = Boolean(isGpsValid && !isNaN(rawLat) && !isNaN(rawLng) && rawLat !== 0 && rawLng !== 0);
      hasHardwareGpsFix.value = hasHardwareFix;

      // Only take location & speed from motorcycle module if it has a locked 3D satellite fix!
      if (hasHardwareFix) {
        location.value = { lat: rawLat, lng: rawLng };
        const rawSpeed = parseFloat(liveData.speed) || 0;
        currentSpeed.value = rawSpeed < 0.2 ? 0 : rawSpeed;
        speedHistory.value.push(currentSpeed.value);
        if (speedHistory.value.length > 10) speedHistory.value.shift();
        isOverSpeed.value = currentSpeed.value > speedLimit.value;
        recordTripPoint(rawLat, rawLng, currentSpeed.value, 'module');
        updateTripStatistics(currentSpeed.value);
        gpsConnected.value = true;
      } else {
        // Hardware module has NO lock (indoors / searching).
        // The system automatically falls back to Phone GPS for location and speed accuracy!
        gpsConnected.value = false;
      }
      
      // ✅ Update MPU6050 sensor data
      if (liveData.mpu6050) {
        const accelX = liveData.mpu6050.accelX || 0;
        const accelY = liveData.mpu6050.accelY || 0;
        const accelZ = liveData.mpu6050.accelZ || 0;
        const gyro = liveData.mpu6050.gyro || 0;
        
        sensorData.value.mpu6050 = {
          accelX: accelX.toFixed(2),
          accelY: accelY.toFixed(2),
          accelZ: accelZ.toFixed(2),
          gyro: gyro.toFixed(1),
          lastUpdate: Date.now()
        };
        
        // Detect harsh braking (high negative acceleration)
        if (accelX < -2.5) {
          tripStats.value.harshBraking++;
        }
        
        // Detect rapid acceleration (high positive acceleration)
        if (accelX > 2.5) {
          tripStats.value.rapidAccel++;
        }
        
        // Detect sharp turns (high gyro values)
        if (Math.abs(gyro) > 45) {
          tripStats.value.sharpTurns++;
        }
      }
      
      // ✅ Update GPS quality data
      if (liveData.gps) {
        sensorData.value.gps = {
          accuracy: liveData.gps.accuracy || 'N/A',
          satellites: liveData.gps.satellites || 0
        };
      }
      
      // ✅ Update GSM data
      if (liveData.gsm) {
        sensorData.value.gsm = {
          signal: liveData.gsm.signal || '0%',
          network: liveData.gsm.network || 'N/A'
        };
        
        // Update GSM connection status
        const signalValue = parseInt(liveData.gsm.signal) || 0;
        gsmConnected.value = signalValue > 0;
      }
      
      // ✅ Update engine status (relay state)
      if (typeof liveData.engineRunning !== 'undefined') {
        sensorData.value.engineRunning = liveData.engineRunning;
        console.log('[ENGINE] Status updated:', liveData.engineRunning ? 'RUNNING' : 'STOPPED');
      }
      
      // ✅ Update crash detection status
      if (typeof liveData.crashDetected !== 'undefined') {
        const wasCrash = sensorData.value.crashDetected;
        const incomingCrash = Boolean(liveData.crashDetected);

        // Sanity check: is vehicle upright and is this reading stale from before app opened?
        const mpu = liveData.mpu6050;
        const isUpright = mpu ? (Math.abs(mpu.roll || 0) < 30 && Math.abs(mpu.pitch || 0) < 30 && (mpu.totalAccel || 9.8) < 15) : true;
        const liveTimestamp = Number(liveData.timestamp || 0);
        const isOldSession = liveTimestamp > 0 && liveTimestamp < appStartTime.value - 5000;

        if (isInitialLiveCrashCheck) {
          isInitialLiveCrashCheck = false;
          // If dashboard opened/refreshed and crash is either from past session or vehicle is upright:
          if (incomingCrash && (isUpright || isOldSession)) {
            console.log('[CRASH] Initial load detected stale or upright crash flag. Auto-clearing...');
            sensorData.value.crashDetected = false;
            crashDisplayStatus.value = 'Stable';
            crashDisplayMessage.value = 'Vehicle Stable';
            if (userId.value) {
              set(dbRef(database, `helmet_public/${userId.value}/live/crashDetected`), false).catch(() => {});
            }
            return;
          }
        }

        sensorData.value.crashDetected = incomingCrash;
        console.log('[CRASH] Status updated:', incomingCrash ? 'DETECTED' : 'CLEAR');

        if (incomingCrash && !wasCrash && !isOldSession) {
          // Double check that it's not a zero-accel sensor glitch (0.00g and 0 roll)
          if (mpu && mpu.totalAccel < 3.0 && Math.abs(mpu.roll || 0) < 5.0 && Math.abs(mpu.pitch || 0) < 5.0) {
            console.warn('[CRASH] Ignored false crash triggered by zero-accel sensor read');
            sensorData.value.crashDetected = false;
            return;
          }
          crashDisplayStatus.value = 'Alerting';
          crashDisplayMessage.value = 'Crash Detected!';
          const impactVal = liveData.mpu6050?.totalAccel ? (liveData.mpu6050.totalAccel / 9.81).toFixed(1) + ' g' : 'Severe';
          const leanVal = liveData.mpu6050?.roll ? Math.abs(liveData.mpu6050.roll).toFixed(1) + '°' : '';

          // Auto-fallback: If hardware GPS has no stable 3D lock, use Device GPS
          const hasModuleFix = Boolean(liveData.gpsValid && liveData.locationLat && Number(liveData.locationLat) !== 0);
          const crashLat = hasModuleFix ? Number(liveData.locationLat) : (location.value?.lat || lastPhoneSpeedPos?.lat || null);
          const crashLng = hasModuleFix ? Number(liveData.locationLng) : (location.value?.lng || lastPhoneSpeedPos?.lng || null);
          const usedDevice = !hasModuleFix && Boolean(crashLat && crashLng);

          const liveCrashEvent = {
            timestamp: Date.now(),
            impactStrength: impactVal,
            roll: liveData.mpu6050?.roll || 0,
            location: (crashLat && crashLng) 
              ? `${Number(crashLat).toFixed(6)}, ${Number(crashLng).toFixed(6)}${usedDevice ? ' (Device GPS)' : ''}` 
              : 'Location acquiring...',
            lat: crashLat ? Number(crashLat) : null,
            lng: crashLng ? Number(crashLng) : null,
            hasGPS: Boolean(crashLat && crashLng),
            gpsSource: usedDevice ? 'device_fallback' : (hasModuleFix ? 'module' : 'none'),
            severity: 'High',
            time: new Date().toLocaleTimeString(),
            speed: liveData.speed || 0
          };

          crashEvents.value = [liveCrashEvent];
          activeTab.value = 'My Location';

          alerts.value.unshift({
            type: 'crash',
            message: '🚨 CRASH DETECTED!',
            details: `Impact: ${impactVal} | Lean: ${leanVal} | Location: ${liveCrashEvent.location}`,
            time: new Date().toLocaleTimeString(),
            timestamp: Date.now()
          });
          if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
          flashCrashMessage();
          playSound('crash');
        } else if (!incomingCrash && (wasCrash || crashDisplayStatus.value === 'Alerting')) {
          if (crashInterval) {
            clearInterval(crashInterval);
            crashInterval = null;
          }
          crashDisplayStatus.value = 'Stable';
          crashDisplayMessage.value = 'Vehicle Stable';
          stopSound();
          isAudioMuted.value = false;
        }
      }
      
      // ✅ Update anti-theft armed status
      if (typeof liveData.antiTheftArmed !== 'undefined') {
        sensorData.value.antiTheftArmed = liveData.antiTheftArmed;
        console.log('[ANTI-THEFT] Status updated:', liveData.antiTheftArmed ? 'ARMED' : 'DISARMED');
      }
      
      // ✅ Update battery data
      if (liveData.batteryVoltage) {
        sensorData.value.battery = {
          voltage: liveData.batteryVoltage.toFixed(1) + 'V',
          health: '95%' // Calculate based on voltage if needed
        };
      }
    }
  });

  // ✅ FIX: Use .value for ref access
  if (!helmetRef.value) {
    console.error('[HELMET_STATUS] helmetRef is null, cannot set up listener');
    return;
  }
  onValue(helmetRef.value, (snapshot) => {
    const data = snapshot.val();
    console.log('[DEBUG] Helmet status data from Firebase:', data);
    
    if (data) {
      const isConnected = data.helmetConnected === true || data.helmetConnected === 'true';
      
      // ✅ Detect helmet removal and trigger warning
      if (previousHelmetState.value === true && isConnected === false) {
        // Helmet was just removed!
        console.log('[WARNING] Helmet REMOVED - Triggering alert!');
        
        alerts.value.unshift({
          type: 'warning',
          message: '⚠️ HELMET REMOVED!',
          details: 'Rider removed helmet during trip. Engine stopped for safety.',
          time: new Date().toLocaleTimeString()
        });
        
        playSound();
        
        // Flash warning on screen
        if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
      }
      
      // ✅ Detect helmet put on
      if (previousHelmetState.value === false && isConnected === true) {
        console.log('[INFO] Helmet PUT ON');
        
        alerts.value.unshift({
          type: 'success',
          message: '✅ Helmet Connected',
          details: 'Rider put on helmet. Ready to ride safely.',
          time: new Date().toLocaleTimeString()
        });
        
        if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
      }
      
      // Update previous state
      previousHelmetState.value = isConnected;
      
      // ✅ FIX: Rider status should be Active ONLY when BOTH devices are on
      const bothDevicesOn = helmetPaired.value && motorcyclePaired.value;
      riderStatus.value = bothDevicesOn ? 'Active' : 'Inactive';
      
      if (bothDevicesOn) {
        riderSubtitle.value = 'Helmet connected';
      } else if (!helmetPaired.value && !motorcyclePaired.value) {
        riderSubtitle.value = 'Both devices disconnected';
      } else if (!helmetPaired.value) {
        riderSubtitle.value = 'Helmet not connected';
      } else {
        riderSubtitle.value = 'Motorcycle not connected';
      }
      
      console.log(`[DEBUG] Helmet: ${helmetPaired.value}, Motorcycle: ${motorcyclePaired.value}, Rider Status: ${riderStatus.value}`);
      
      // ✅ Update alertness status
      alertnessStatus.value = data.alertnessStatus || 'Normal';
      alertnessSubtitle.value = alertnessStatus.value === 'Normal' ? 'No drowsiness detected' : 'Drowsiness detected!';
      
      // ⚠️ REMOVED: Alcohol status is now handled by dedicated alcoholRef listener
      // This prevents conflicts between two listeners updating the same data
      
      // ✅ Alert for drowsiness only (alcohol alerts handled by alcoholRef listener)
      if (alertnessStatus.value !== 'Normal') {
        const newAlert = {
          type: 'drowsiness',
          message: '😴 Drowsiness Detected',
          details: alertnessSubtitle.value,
          time: new Date().toLocaleTimeString()
        };
        alerts.value.unshift(newAlert);
        console.log('[ALERT] ✅ Drowsiness alert added:', newAlert);
        console.log('[ALERT] Total alerts:', alerts.value.length);
        playSound();
        if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
      }
    } else {
      console.log('[DEBUG] No helmet status data received');
    }
  });

  if (!tripsRef.value) {
    console.error('[TRIPS] tripsRef is null, cannot set up listener');
    return;
  }
  onValue(tripsRef.value, async (snapshot) => {
    const data = snapshot.val();
    if (data) {
      const tripList = Object.entries(data).map(([id, trip]) => ({ id, ...trip }));
      tripList.sort((a, b) => {
        const aTime = typeof a.startTime === 'string' ? parseInt(a.startTime) : a.startTime;
        const bTime = typeof b.startTime === 'string' ? parseInt(b.startTime) : b.startTime;
        return bTime - aTime;
      });
      recentTrips.value = tripList.slice(0, 5);
    }
  });

  // ✅ Real-time device connection monitoring (helmet & motorcycle)
  setupDeviceListeners();

  // ✅ NEW: Listen for anti-theft alerts
  setupAntiTheftListener();

  // ✅ Listen for Arduino-generated alerts (alcohol, helmet, crash, speed, etc.)
  setupArduinoAlertsListener();
};

// ✅ FIXED: Wait for user authentication before setting up Firebase
// ✅ REMOVED: Duplicate onMounted - merged into main onMounted below

// Show/Hide Alerts Panel
const toggleAlerts = () => {
  showAlerts.value = !showAlerts.value;
};

// Update speed limit in Firebase
const updateSpeedLimitInFirebase = () => {
  if (!userId.value) return;
  set(dbRef(database, `helmet_public/${userId.value}/settings/speedLimit`), speedLimit.value).catch(error => {
    console.error("Failed to update speed limit:", error);
  });
};

// Delete a trip by ID
const deleteTrip = async (tripId) => {
  if (!userId.value) return;
  const confirmDelete = confirm("Are you sure you want to delete this trip?");
  if (!confirmDelete) return;
  const tripRef = dbRef(database, `helmet_public/${userId.value}/trips/${tripId}`);
  try {
    await remove(tripRef);
    recentTrips.value = recentTrips.value.filter(trip => trip.id !== tripId);
  } catch (error) {
    console.error("Failed to delete trip:", error);
    alert("Failed to delete trip.");
  }
};

// Determine if crash alert should be shown
const isCrashActive = (index) => {
  const event = crashEvents.value[index];
  return event && event.timestamp > lastCrashTimestamp;
};

// Clear crash alert manually
const clearCrashAlert = (index) => {
  const event = crashEvents.value[index];
  if (event) {
    console.log('[CRASH] Clearing crash alert for event:', event);
    lastCrashTimestamp = event.timestamp;
    localStorage.setItem(`lastCrashTimestamp_${userId.value}`, event.timestamp.toString());
    
    // Stop any ongoing crash animation
    if (crashInterval) {
      clearInterval(crashInterval);
      crashInterval = null;
    }
    
    // Reset crash display to stable
    crashDisplayStatus.value = 'Stable';
    crashDisplayMessage.value = 'Vehicle Stable';
    
    console.log('[CRASH] ✓ Alert cleared, status reset to Stable');
  }
};

// ✅ Track dismissed alerts in localStorage
const dismissedAlertsKey = `dismissedAlerts_${userId.value}`;
const alertDismissalTime = ref(0); // Start at 0 to show all alerts initially

// Load dismissed alerts timestamp from localStorage
const loadDismissalTime = () => {
  const stored = localStorage.getItem(dismissedAlertsKey);
  if (stored) {
    alertDismissalTime.value = parseInt(stored);
    console.log('[ALERTS] Loaded dismissal time:', new Date(alertDismissalTime.value).toLocaleString());
  }
};

// Check if an alert should be shown (not dismissed)
const shouldShowAlert = (alertTimestamp) => {
  // If no timestamp provided, always show
  if (!alertTimestamp) return true;
  
  // Show alert only if it's newer than the last dismissal
  return alertTimestamp > alertDismissalTime.value;
};

// ✅ Helper function to add alerts with automatic dismissal checking
const addAlert = (type, message, details = '') => {
  const alertTime = Date.now();
  
  // Check if alert should be shown
  if (!shouldShowAlert(alertTime)) {
    console.log('[ALERTS] Alert dismissed:', message);
    return false;
  }
  
  // Add alert
  alerts.value.unshift({
    type,
    message,
    details,
    time: new Date().toLocaleTimeString(),
    timestamp: alertTime
  });
  
  // Keep only last 10 alerts
  if (alerts.value.length > 10) {
    alerts.value = alerts.value.slice(0, 10);
  }
  
  return true;
};

// ✅ NEW: Clear all alerts from the alerts list & stop all alarms
const clearAllAlerts = () => {
  console.log('[ALERTS] Clearing all alerts');
  
  // Record the current time as dismissal time
  alertDismissalTime.value = Date.now();
  localStorage.setItem(dismissedAlertsKey, alertDismissalTime.value.toString());
  console.log('[ALERTS] Dismissal time saved:', new Date(alertDismissalTime.value).toLocaleString());
  
  // Clear alerts array
  alerts.value = [];
  
  // Stop sound and mute alarms
  stopSound();
  antiTheftAlert.value.active = false;
  
  // Also stop crash animation if running
  if (crashInterval) {
    clearInterval(crashInterval);
    crashInterval = null;
  }
  
  // Reset crash display
  crashDisplayStatus.value = 'Stable';
  crashDisplayMessage.value = 'Vehicle Stable';
};

// Clear All Crash Locations
const clearAllCrashLocations = async () => {
  while (crashEvents.value.length > 0) {
    await deleteCrashEvent(0);
  }
  crashDisplayStatus.value = 'Stable';
  crashDisplayMessage.value = 'Vehicle Stable';
};

// Delete Crash Event
const deleteCrashEvent = async (index) => {
  try {
    const event = crashEvents.value[index];
    if (!event) return;
    
    console.log('[CRASH] Deleting crash event:', event);
    
    // Remove from local array
    crashEvents.value.splice(index, 1);
    
    // Also delete from Firebase to prevent it from reappearing
    const crashesRef = dbRef(database, `helmet_public/${userId.value}/crashes`);
    const snapshot = await get(crashesRef);
    
    if (snapshot.exists()) {
      const crashes = snapshot.val();
      // Find and delete the crash with matching timestamp
      for (const [key, crash] of Object.entries(crashes)) {
        if (crash.timestamp === event.timestamp) {
          await remove(dbRef(database, `helmet_public/${userId.value}/crashes/${key}`));
          console.log('[CRASH] ✓ Deleted crash from Firebase:', key);
          break;
        }
      }
    }
    
    console.log('[CRASH] ✓ Crash event deleted successfully');
  } catch (error) {
    console.error('[CRASH] Error deleting crash event:', error);
    alert('Failed to delete crash event. Please try again.');
  }
};

let lastPhoneSpeedPos = null;
let lastPhoneSpeedTime = null;

// Start continuous GPS speed monitoring (with delta-distance accuracy & auto-fallback)
const startGPSSpeedMonitoring = (silent = true) => {
  if (!navigator.geolocation) {
    if (!silent) alert('Geolocation is not supported by your browser');
    return;
  }

  if (gpsWatchId !== null) {
    // Already monitoring
    return;
  }

  isGPSSpeedActive.value = true;
  gpsSpeedSource.value = 'phone';

  gpsWatchId = navigator.geolocation.watchPosition(
    (position) => {
      const lat = position.coords.latitude;
      const lng = position.coords.longitude;
      const rawSpeed = position.coords.speed; // Speed in meters per second
      
      let finalSpeed = 0;
      // 1. Check hardware chip speed (m/s -> km/h)
      if (rawSpeed !== null && !isNaN(rawSpeed) && rawSpeed > 0) {
        finalSpeed = Math.round(rawSpeed * 3.6 * 10) / 10;
      } else if (lastPhoneSpeedPos && lastPhoneSpeedTime) {
        // 2. Accurate Haversine delta calculation (handles devices that report null speed during motion)
        const dtSeconds = (position.timestamp - lastPhoneSpeedTime) / 1000;
        if (dtSeconds >= 0.4 && dtSeconds <= 12) {
          const dM = computeDistanceKm(lastPhoneSpeedPos.lat, lastPhoneSpeedPos.lng, lat, lng) * 1000;
          if (dM >= 1.5) {
            const calcKmh = (dM / dtSeconds) * 3.6;
            if (calcKmh <= 160) finalSpeed = Math.round(calcKmh * 10) / 10;
          }
        }
      }
      lastPhoneSpeedPos = { lat, lng };
      lastPhoneSpeedTime = position.timestamp || Date.now();

      // Only take Phone GPS location & speed if hardware GPS module has NO 3D lock
      if (!hasHardwareGpsFix.value) {
        location.value = { lat, lng };
        currentSpeed.value = finalSpeed;
        speedHistory.value.push(finalSpeed);
        if (speedHistory.value.length > 10) speedHistory.value.shift();
        
        // Check for overspeed
        if (finalSpeed > speedLimit.value && !isOverSpeed.value) {
          isOverSpeed.value = true;
          handleOverspeed({ speed: finalSpeed, limit: speedLimit.value });
        } else if (finalSpeed <= speedLimit.value) {
          isOverSpeed.value = false;
        }

        updateTripStatistics(finalSpeed);
        recordTripPoint(lat, lng, finalSpeed, 'phone_auto_fallback');

        // Non-destructive update in Firebase live
        if (userId.value) {
          const locationRef = dbRef(database, `helmet_public/${userId.value}/live`);
          update(locationRef, {
            locationLat: lat,
            locationLng: lng,
            speed: finalSpeed,
            timestamp: Date.now(),
            gpsSource: 'phone_auto_fallback'
          }).catch(() => {});
        }
      }
    },
    (error) => {
      console.warn('[DASHBOARD] GPS speed monitoring notice:', error.message);
      if (!silent) stopGPSSpeedMonitoring();
    },
    { enableHighAccuracy: true, maximumAge: 1000, timeout: 8000 }
  );
};

// Stop GPS speed monitoring
const stopGPSSpeedMonitoring = () => {
  if (gpsWatchId !== null) {
    navigator.geolocation.clearWatch(gpsWatchId);
    gpsWatchId = null;
    isGPSSpeedActive.value = false;
    gpsSpeedSource.value = 'module';
    
    alerts.value.unshift({
      type: 'info',
      message: 'GPS Speed Monitoring Stopped',
      details: 'Switched back to module data',
      time: new Date().toLocaleTimeString()
    });
    if (alerts.value.length > 5) alerts.value.pop();
  }
};

// Toggle GPS speed monitoring
const toggleGPSSpeedMonitoring = () => {
  if (isGPSSpeedActive.value) {
    stopGPSSpeedMonitoring();
  } else {
    startGPSSpeedMonitoring();
  }
};

// Update trip statistics
const updateTripStatistics = (speed) => {
  // Initialize trip if not started
  if (!tripStartTime) {
    tripStartTime = Date.now();
  }
  
  // Calculate duration
  const durationMs = Date.now() - tripStartTime;
  const durationMin = Math.floor(durationMs / 60000);
  tripStats.value.duration = durationMin.toString();
  
  // Update max speed
  if (speed > parseFloat(tripStats.value.maxSpeed)) {
    tripStats.value.maxSpeed = speed.toFixed(0);
  }
  
  // Calculate average speed
  if (speed > 0) {
    speedSum += speed;
    speedCount++;
    tripStats.value.avgSpeed = (speedSum / speedCount).toFixed(0);
  }
  
  // Estimate distance (speed * time)
  if (speed > 0 && speedCount > 0) {
    tripDistance = (speedSum / speedCount) * (durationMin / 60);
    tripStats.value.distance = tripDistance.toFixed(1);
  }
  
  // Calculate safety score (decrease for violations)
  let safetyScore = 100;
  safetyScore -= tripStats.value.harshBraking * 5;
  safetyScore -= tripStats.value.rapidAccel * 3;
  safetyScore -= tripStats.value.sharpTurns * 2;
  if (isOverSpeed.value) safetyScore -= 10;
  tripStats.value.safetyScore = Math.max(0, safetyScore);
};

// ==========================================
// 🏍️ REAL-TIME TRIP SESSION RECORDER
// ==========================================
const activeTripSession = ref(null);
let tripSaveTimeout = null;

// Reverse geocode cache to reduce network calls
const geocodeCache = new Map();
const reverseGeocodeCoord = async (lat, lng) => {
  const numLat = parseFloat(lat);
  const numLng = parseFloat(lng);
  if (isNaN(numLat) || isNaN(numLng) || (numLat === 0 && numLng === 0)) return null;
  const key = `${numLat.toFixed(4)},${numLng.toFixed(4)}`;
  if (geocodeCache.has(key)) return geocodeCache.get(key);

  try {
    const res = await fetch(`https://nominatim.openstreetmap.org/reverse?lat=${numLat}&lon=${numLng}&format=json&addressdetails=1`, {
      headers: { 'User-Agent': 'VigiLERT Motorcycle Safety System' }
    });
    if (!res.ok) throw new Error('Geocoding failed');
    const data = await res.json();
    const addr = data.address || {};
    let parts = [];
    if (addr.road) parts.push(addr.road);
    if (addr.suburb) parts.push(addr.suburb);
    if (addr.city) parts.push(addr.city);
    else if (addr.town) parts.push(addr.town);
    else if (addr.municipality) parts.push(addr.municipality);
    let formatted = parts.join(', ');
    if (!formatted) formatted = data.display_name?.split(',').slice(0, 3).join(',') || `${numLat.toFixed(5)}, ${numLng.toFixed(5)}`;
    geocodeCache.set(key, formatted);
    return formatted;
  } catch (e) {
    return `${numLat.toFixed(5)}, ${numLng.toFixed(5)}`;
  }
};

// Haversine formula to compute distance in km
const computeDistanceKm = (lat1, lon1, lat2, lon2) => {
  const R = 6371; // km
  const dLat = (lat2 - lat1) * Math.PI / 180;
  const dLon = (lon2 - lon1) * Math.PI / 180;
  const a =
    Math.sin(dLat / 2) * Math.sin(dLat / 2) +
    Math.cos(lat1 * Math.PI / 180) * Math.cos(lat2 * Math.PI / 180) *
    Math.sin(dLon / 2) * Math.sin(dLon / 2);
  const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
  return R * c;
};

// Persist trip to Firebase (both primary and backup paths)
const saveTripSessionToFirebase = async (session) => {
  if (!session) return;
  const uid = userId.value || authStore.user?.uid || localStorage.getItem('vigilert_user_uid') || 'DMIARybyNHVhzwAmrx2QQjdpSb13';
  if (!uid) return;

  const durationMs = Math.max(1000, (session.endTime || Date.now()) - session.startTime);
  let avgSpeed = 0;
  if (session.speedSamples && session.speedSamples.length > 0) {
    const sum = session.speedSamples.reduce((a, b) => a + b, 0);
    avgSpeed = Math.round((sum / session.speedSamples.length) * 10) / 10;
  } else if (session.totalDistanceKm > 0 && durationMs > 0) {
    avgSpeed = Math.round((session.totalDistanceKm / (durationMs / 3600000)) * 10) / 10;
  }

  const tripRecord = {
    id: session.id,
    startTime: session.startTime,
    endTime: session.endTime || Date.now(),
    timestamp: session.endTime || session.timestamp || Date.now(),
    duration: durationMs,
    distance: Math.round(session.totalDistanceKm * 100) / 100,
    maxSpeed: session.maxSpeed || 0,
    avgSpeed: avgSpeed || 0,
    startLat: session.startLat || 0,
    startLng: session.startLng || 0,
    endLat: session.endLat || session.startLat || 0,
    endLng: session.endLng || session.startLng || 0,
    startLocationName: session.startLocationName || `${Number(session.startLat || 0).toFixed(5)}, ${Number(session.startLng || 0).toFixed(5)}`,
    endLocationName: session.endLocationName || `${Number(session.endLat || session.startLat || 0).toFixed(5)}, ${Number(session.endLng || session.startLng || 0).toFixed(5)}`,
    routeCoords: session.routeCoords || [],
    hasGPS: session.hasGPS ?? true,
    source: session.source || 'phone_gps',
    createdAt: session.startTime,
    status: session.status || 'active'
  };

  try {
    await set(dbRef(database, `helmet_public/${uid}/trips/${session.id}`), tripRecord);
    await set(dbRef(database, `${uid}/trips/${session.id}`), tripRecord).catch(() => {});

    // Update in-memory recent trips
    const existingIndex = recentTrips.value.findIndex(t => t.id === session.id);
    if (existingIndex >= 0) {
      recentTrips.value[existingIndex] = tripRecord;
    } else {
      recentTrips.value.unshift(tripRecord);
      if (recentTrips.value.length > 5) recentTrips.value.pop();
    }
  } catch (err) {
    console.error('[TRIP RECORDER] Failed saving trip to Firebase:', err);
  }
};

// Debounce continuous trip saves
const debouncedSaveActiveTrip = () => {
  if (tripSaveTimeout) clearTimeout(tripSaveTimeout);
  tripSaveTimeout = setTimeout(() => {
    if (activeTripSession.value) {
      saveTripSessionToFirebase(activeTripSession.value);
    }
  }, 3000);
};

// Start a new trip session (immediately records FROM point)
const startTripSession = async (trigger = 'auto_location', initLat = null, initLng = null, source = 'gps') => {
  if (activeTripSession.value) return; // Already recording

  const startLat = initLat !== null ? Number(initLat) : (Number(location.value?.lat) || 0);
  const startLng = initLng !== null ? Number(initLng) : (Number(location.value?.lng) || 0);
  const hasGps = (startLat !== 0 && startLng !== 0);

  const sessionId = 'trip_' + Date.now();
  const startTime = Date.now();
  const initialCoordName = hasGps ? `${startLat.toFixed(5)}, ${startLng.toFixed(5)}` : 'Stationary Base';

  activeTripSession.value = {
    id: sessionId,
    startTime: startTime,
    endTime: startTime,
    timestamp: startTime,
    trigger,
    startLat,
    startLng,
    endLat: startLat,
    endLng: startLng,
    startLocationName: initialCoordName,
    endLocationName: initialCoordName,
    maxSpeed: Number(currentSpeed.value) || 0,
    speedSamples: [Number(currentSpeed.value) || 0],
    totalDistanceKm: 0,
    lastCoord: hasGps ? { lat: startLat, lng: startLng } : null,
    routeCoords: hasGps ? [{ lat: startLat, lng: startLng, time: startTime }] : [],
    hasGPS: hasGps,
    source: source || gpsSpeedSource.value || 'phone_gps',
    status: 'active'
  };

  console.log('[TRIP RECORDER] 🚀 Trip session started:', sessionId, { startLat, startLng, trigger });

  // Save immediately so trip appears right away in Trip History
  await saveTripSessionToFirebase(activeTripSession.value);

  // Reverse geocode start location asynchronously
  if (hasGps) {
    reverseGeocodeCoord(startLat, startLng).then(addr => {
      if (addr && activeTripSession.value && activeTripSession.value.id === sessionId) {
        activeTripSession.value.startLocationName = addr;
        if (activeTripSession.value.endLocationName === initialCoordName) {
          activeTripSession.value.endLocationName = addr;
        }
        saveTripSessionToFirebase(activeTripSession.value);
      }
    });
  }
};

// Record continuous coordinates / distance during the trip (FROM & TO tracking)
const recordTripPoint = (lat, lng, speed = 0, source = 'gps') => {
  const numLat = parseFloat(lat);
  const numLng = parseFloat(lng);
  if (isNaN(numLat) || isNaN(numLng) || (numLat === 0 && numLng === 0)) return;

  const numSpeed = parseFloat(speed) || 0;
  updateTripStatistics(numSpeed);

  // Auto-start trip session immediately on first valid location fix if none exists!
  if (!activeTripSession.value) {
    startTripSession(numSpeed >= 5 ? 'motion' : 'auto_location', numLat, numLng, source);
    return;
  }

  const session = activeTripSession.value;

  if (numSpeed > session.maxSpeed) {
    session.maxSpeed = Math.round(numSpeed * 10) / 10;
  }
  if (numSpeed > 0) {
    session.speedSamples.push(numSpeed);
  }

  session.hasGPS = true;
  session.endTime = Date.now();
  session.timestamp = Date.now();

  // If start coordinate was unset, establish it as FROM
  if (!session.startLat || session.startLat === 0) {
    session.startLat = numLat;
    session.startLng = numLng;
    session.startLocationName = `${numLat.toFixed(5)}, ${numLng.toFixed(5)}`;
    reverseGeocodeCoord(numLat, numLng).then(addr => {
      if (addr && activeTripSession.value) {
        activeTripSession.value.startLocationName = addr;
        saveTripSessionToFirebase(activeTripSession.value);
      }
    });
  }

  // Calculate distance moved from previous point
  if (session.lastCoord) {
    const d = computeDistanceKm(session.lastCoord.lat, session.lastCoord.lng, numLat, numLng);
    // Real movement between 3m and 5km per tick
    if (d >= 0.003 && d <= 5.0) {
      session.totalDistanceKm += d;
      session.lastCoord = { lat: numLat, lng: numLng };
      session.endLat = numLat;
      session.endLng = numLng;
      session.routeCoords.push({ lat: numLat, lng: numLng, time: Date.now() });
      if (session.routeCoords.length > 500) {
        session.routeCoords.splice(1, 1);
      }
      // Reverse geocode latest TO position asynchronously
      reverseGeocodeCoord(numLat, numLng).then(addr => {
        if (addr && activeTripSession.value) {
          activeTripSession.value.endLocationName = addr;
        }
      });
    }
  } else {
    session.lastCoord = { lat: numLat, lng: numLng };
    session.endLat = numLat;
    session.endLng = numLng;
  }

  // Continuously persist latest TO position & route
  debouncedSaveActiveTrip();
};

// Conclude and push finalized trip to Firebase (never discarded)
const endTripSessionAndSave = async () => {
  if (!activeTripSession.value) return;
  const session = activeTripSession.value;
  activeTripSession.value = null; // Clear state

  const endTime = Date.now();
  session.endTime = endTime;
  session.timestamp = endTime;
  session.status = 'completed';

  console.log('[TRIP RECORDER] 💾 Finalizing trip to Firebase:', session.id);
  await saveTripSessionToFirebase(session);

  const durationMs = Math.max(1000, endTime - session.startTime);
  alerts.value.unshift({
    type: 'success',
    message: 'Trip Saved to History',
    details: `${session.totalDistanceKm.toFixed(2)} km • ${Math.round(durationMs / 60000)} min journey logged`,
    time: new Date().toLocaleTimeString()
  });
  if (alerts.value.length > 5) alerts.value.pop();
};

// Handle location update from GPS (with automatic fallback for location & speed)
const handleLocationUpdate = async (newLocation) => {
  if (newLocation.lat && newLocation.lng) {
    // If hardware GPS module has NO 3D satellite fix, automatically use Phone GPS for location and speed!
    if (!hasHardwareGpsFix.value || newLocation.source === 'phone') {
      location.value = { lat: newLocation.lat, lng: newLocation.lng };
      if (typeof newLocation.speed !== 'undefined') {
        const phoneSpd = Math.max(0, Number(newLocation.speed) || 0);
        currentSpeed.value = phoneSpd;
        speedHistory.value.push(phoneSpd);
        if (speedHistory.value.length > 10) speedHistory.value.shift();
        isOverSpeed.value = phoneSpd > speedLimit.value;
        updateTripStatistics(phoneSpd);
      }
    }
    recordTripPoint(newLocation.lat, newLocation.lng, currentSpeed.value, newLocation.source || 'phone');
  }
  
  // Non-destructive update in Firebase so engineRunning, mpu6050, antiTheftArmed are preserved
  try {
    const locationRef = dbRef(database, `helmet_public/${userId.value}/live`);
    await update(locationRef, {
      locationLat: newLocation.lat,
      locationLng: newLocation.lng,
      speed: currentSpeed.value || 0,
      timestamp: Date.now(),
      gpsSource: newLocation.source || 'phone'
    });
    
    alerts.value.unshift({
      type: 'success',
      message: 'Location Updated',
      details: `New location: ${newLocation.lat.toFixed(6)}, ${newLocation.lng.toFixed(6)}`,
      time: new Date().toLocaleTimeString()
    });
    if (alerts.value.length > 5) alerts.value.pop();
  } catch (error) {
    console.error('Failed to update location:', error);
  }
};

// -- SOS State & Modals -----------------------------------------------------
const showSosConfirmModal  = ref(false);
const showSosSentModal     = ref(false);
const showIncomingSosModal = ref(false);
const showSosErrorModal    = ref(false);
const isSendingSos         = ref(false);
const incomingSosData      = ref(null);
const sosErrorMessage      = ref('');

// Open Trigger SOS Confirmation Modal (Replaces browser alert/confirm)
const triggerSOS = () => {
  showSosConfirmModal.value = true;
};

// Confirm and Send SOS Broadcast
const confirmAndSendSOS = async () => {
  isSendingSos.value = true;
  try {
    const timestamp = Date.now();
    const lat = location.value?.lat || null;
    const lng = location.value?.lng || null;
    const locString = lat && lng ? `${lat.toFixed(6)}, ${lng.toFixed(6)}` : 'Location acquired from live stream';

    // 1. Add visual alert to rider's alert list
    alerts.value.unshift({
      type: 'danger',
      message: '🆘 EMERGENCY SOS BROADCASTED',
      details: `Location: ${locString}`,
      time: new Date().toLocaleTimeString(),
      timestamp
    });

    // 2. Write to Firebase /sos list (so emergency contact dashboard receives it in real-time)
    const sosRef = dbRef(database, `helmet_public/${userId.value}/sos/${timestamp}`);
    await set(sosRef, {
      timestamp,
      location: { lat, lng },
      triggeredBy: 'rider',
      status: 'active',
      riderName: user.value?.name || user.value?.displayName || 'Rider'
    });

    // 3. Write to /alerts path for emergency contact real-time push
    await push(dbRef(database, `helmet_public/${userId.value}/alerts`), {
      type: 'danger',
      message: '🚨 EMERGENCY SOS TRIGGERED BY RIDER!',
      details: `Rider has requested emergency assistance. Location: ${locString}`,
      timestamp,
      severity: 'critical'
    });

    // 4. Update live sosActive flag
    await set(dbRef(database, `helmet_public/${userId.value}/live/sosActive`), true);

    showSosConfirmModal.value = false;
    showSosSentModal.value = true;
  } catch (error) {
    console.error('[SOS] Failed to trigger SOS:', error);
    sosErrorMessage.value = 'Failed to broadcast SOS alert. Please check your network connection or call 911 directly.';
    showSosErrorModal.value = true;
  } finally {
    isSendingSos.value = false;
  }
};

// Resolve active SOS
const resolveSos = async () => {
  try {
    if (userId.value) {
      await set(dbRef(database, `helmet_public/${userId.value}/live/sosActive`), false);
    }
  } catch (e) {}
  showSosSentModal.value = false;
};

// Acknowledge Incoming SOS from Emergency Contact
const acknowledgeIncomingSos = () => {
  showIncomingSosModal.value = false;
  stopSound();
};

// Initialize crash event listener
const initializeCrashListener = () => {
  if (!userId.value || !crashRef.value) {
    console.warn('[CRASH] crashRef not ready, cannot setup listener');
    return;
  }
  console.log('[INIT] Setting up crash listener on path:', `/helmet_public/${userId.value}/crashes`);

  // Track Firebase keys we've already processed so we never show the same crash twice
  const seenCrashKeys = new Set();
  let initialCrashLoadDone = false;
  const sessionStartTime = Date.now();
  setTimeout(() => { initialCrashLoadDone = true; }, 3000);

  // Pre-index existing keys using get() so historical crashes don't trigger alerts on boot
  get(crashRef.value).then((snapshot) => {
    if (snapshot.exists()) {
      snapshot.forEach((child) => {
        seenCrashKeys.add(child.key);
      });
    }
    initialCrashLoadDone = true;
    console.log('[CRASH] Pre-existing crashes indexed:', seenCrashKeys.size);
  }).catch((err) => {
    console.warn('[CRASH] Error pre-indexing crashes:', err);
    initialCrashLoadDone = true;
  });

  onChildAdded(crashRef.value, (snapshot) => {
    const key = snapshot.key;
    const event = snapshot.val();

    if (!event) return;

    const eventTime = Number(event.timestamp || 0);

    const hasModuleGPS = Boolean(
      (event.hasGPS || event.gpsValid === true) &&
      event.lat && Number(event.lat) !== 0 &&
      event.lng && Number(event.lng) !== 0 &&
      event.gpsSource !== 'device_fallback'
    );

    let crashLat = hasModuleGPS ? Number(event.lat || (event.location && event.location.lat)) : null;
    let crashLng = hasModuleGPS ? Number(event.lng || (event.location && event.location.lng)) : null;
    let isDeviceFallback = false;

    // Auto-fallback: if GPS module coordinates are not available or 0, use device GPS
    if (!hasModuleGPS) {
      if ((event.resolvedByDevice || event.gpsSource === 'device_fallback') && event.lat && event.lng) {
        crashLat = Number(event.lat);
        crashLng = Number(event.lng);
        isDeviceFallback = true;
      } else {
        const devLat = location.value?.lat || (lastPhoneSpeedPos && lastPhoneSpeedPos.lat);
        const devLng = location.value?.lng || (lastPhoneSpeedPos && lastPhoneSpeedPos.lng);
        if (devLat && devLng && Number(devLat) !== 0 && Number(devLng) !== 0) {
          crashLat = Number(devLat);
          crashLng = Number(devLng);
          isDeviceFallback = true;
        }
      }
    }

    // Build crash event object
    const crashEvent = {
      key,
      timestamp: eventTime || Date.now(),
      impactStrength: event.impactStrength || (event.acceleration ? Number(event.acceleration).toFixed(2) : 'N/A'),
      roll: event.roll || event.leanAngle || 0,
      location: (crashLat && crashLng) 
        ? `${crashLat.toFixed(6)}, ${crashLng.toFixed(6)}${isDeviceFallback ? ' (Device GPS)' : ''}` 
        : 'Location acquiring...',
      lat: crashLat,
      lng: crashLng,
      hasGPS: Boolean(crashLat && crashLng),
      gpsSource: isDeviceFallback ? 'device_fallback' : (hasModuleGPS ? 'module' : 'none'),
      severity: event.severity || 'Medium',
      time: eventTime ? new Date(eventTime).toLocaleTimeString() : new Date().toLocaleTimeString(),
      speed: event.speed || 0
    };

    // If resolved via device fallback and Firebase doesn't have it yet, backfill Firebase so emergency contacts can see it!
    if (isDeviceFallback && userId.value && key && !event.resolvedByDevice) {
      update(dbRef(database, `helmet_public/${userId.value}/crashes/${key}`), {
        lat: crashLat,
        lng: crashLng,
        locationLat: crashLat,
        locationLng: crashLng,
        hasGPS: true,
        gpsSource: 'device_fallback',
        resolvedByDevice: true
      }).catch((err) => console.warn('[CRASH] Error updating Firebase crash location:', err));
    }

    // If coordinates are still unpopulated, attempt high-accuracy browser geolocation in background
    if (!crashLat && navigator.geolocation) {
      navigator.geolocation.getCurrentPosition(
        (pos) => {
          const dLat = pos.coords.latitude;
          const dLng = pos.coords.longitude;
          crashEvent.lat = dLat;
          crashEvent.lng = dLng;
          crashEvent.hasGPS = true;
          crashEvent.gpsSource = 'device_fallback';
          crashEvent.location = `${dLat.toFixed(6)}, ${dLng.toFixed(6)} (Device GPS)`;
          crashEvents.value = [crashEvent];

          if (userId.value && key) {
            update(dbRef(database, `helmet_public/${userId.value}/crashes/${key}`), {
              lat: dLat,
              lng: dLng,
              locationLat: dLat,
              locationLng: dLng,
              hasGPS: true,
              gpsSource: 'device_fallback',
              resolvedByDevice: true
            }).catch(() => {});
          }
        },
        () => {},
        { enableHighAccuracy: true, timeout: 5000, maximumAge: 30000 }
      );
    }

    // If historical (older than session start or during initial load), only store quietly for map, NEVER alert!
    const isHistorical = (!initialCrashLoadDone) || (eventTime > 0 && eventTime < sessionStartTime - 10000);

    if (isHistorical) {
      seenCrashKeys.add(key);
      if (crashEvents.value.length === 0) {
        crashEvents.value = [crashEvent];
      }
      return;
    }

    if (seenCrashKeys.has(key)) return;
    seenCrashKeys.add(key);

    console.log('[CRASH] 🚨 Real-time crash event received:', { key, event });

    // Keep only the latest crash marker on the map
    crashEvents.value = [crashEvent];

    // Update crash status card
    crashDisplayStatus.value = 'Alerting';
    crashDisplayMessage.value = 'Crash Detected!';

    // Add to alerts list
    alerts.value.unshift({
      type: 'crash',
      message: '🚨 CRASH DETECTED!',
      details: `Impact: ${crashEvent.impactStrength} g | Lean: ${Math.abs(crashEvent.roll).toFixed(1)}° | ${crashEvent.location}`,
      time: crashEvent.time,
      timestamp: Date.now()
    });
    if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);

    // Trigger animation and sound
    flashCrashMessage();
    playSound();

    console.log('[CRASH] ✅ Crash alert displayed');
  }, (error) => {
    console.error('[CRASH] Firebase crash listener error:', error);
  });
};

// Tracking heartbeat states across snapshots
let previousHelmetHeartbeat = null;
let previousMotorcycleHeartbeat = null;
let helmetInitialLoadDone = false;
let motorcycleInitialLoadDone = false;

// ✅ BEST SOLUTION: Real-time Firebase listeners for device status
const setupDeviceListeners = () => {
  if (!userId.value) return;
  console.log('[DEVICE LISTENERS] Setting up real-time device status monitoring...');
  
  // ✅ Helmet real-time listener (verifies active freshness to prevent offline false-positives)
  const helmetRef = dbRef(database, `helmet_public/${userId.value}/devices/helmet`);
  onValue(helmetRef, (snapshot) => {
    const data = snapshot.val();
    console.log('[HELMET] Firebase update received:', data);
    
    if (!data || data.status !== 'On') {
      if (helmetPaired.value) {
        console.log('[HELMET] Marking as DISCONNECTED');
        helmetPaired.value = false;
        helmetRawConnected.value = false;
      }
      return;
    }

    // Check if heartbeat timestamp is actually fresh (< 25s)
    const hbTime = Number(data.timestamp || data.lastHeartbeat || 0);
    const now = Date.now();
    const isStale = (hbTime > 0 && Math.abs(now - hbTime) > 25000);

    if (isStale) {
      console.log(`[HELMET] Persisted heartbeat is stale (${((now - hbTime)/1000).toFixed(1)}s old) — device is OFFLINE`);
      if (helmetPaired.value) {
        helmetPaired.value = false;
        helmetRawConnected.value = false;
      }
      return;
    }

    // Live heartbeat confirmed
    lastHelmetUpdate.value = now;
    if (!helmetPaired.value) {
      helmetPaired.value = true;
      helmetRawConnected.value = true;
      console.log('[HELMET] Live heartbeat active, marked as CONNECTED');
    }
  });
  
  // ✅ Motorcycle real-time listener (verifies active freshness to prevent offline false-positives)
  const motorcycleRef = dbRef(database, `helmet_public/${userId.value}/devices/motorcycle`);
  onValue(motorcycleRef, (snapshot) => {
    const data = snapshot.val();
    console.log('[MOTORCYCLE] Firebase update received:', data);
    
    if (!data || data.status !== 'On') {
      if (motorcyclePaired.value) {
        console.log('[MOTORCYCLE] Marking as DISCONNECTED');
        motorcyclePaired.value = false;
        motorcycleRawConnected.value = false;
      }
      return;
    }

    // Check if heartbeat timestamp is actually fresh (< 25s)
    const hbTime = Number(data.timestamp || data.lastHeartbeat || 0);
    const now = Date.now();
    const isStale = (hbTime > 0 && Math.abs(now - hbTime) > 25000);

    if (isStale) {
      console.log(`[MOTORCYCLE] Persisted heartbeat is stale (${((now - hbTime)/1000).toFixed(1)}s old) — device is OFFLINE`);
      if (motorcyclePaired.value) {
        motorcyclePaired.value = false;
        motorcycleRawConnected.value = false;
      }
      return;
    }

    // Live heartbeat confirmed
    lastMotorcycleUpdate.value = now;
    if (!motorcyclePaired.value) {
      motorcyclePaired.value = true;
      motorcycleRawConnected.value = true;
      console.log('[MOTORCYCLE] Live heartbeat active, marked as CONNECTED');
    }
  });
};

// ✅ FIX: Timeout check based on local browser timestamp when Firebase heartbeat arrived
let heartbeatCheckInterval = null;

const checkDeviceTimeouts = () => {
  const now = Date.now();
  
  const timeSinceHelmetUpdate = lastHelmetUpdate.value > 0 ? now - lastHelmetUpdate.value : Infinity;
  const timeSinceMotorcycleUpdate = lastMotorcycleUpdate.value > 0 ? now - lastMotorcycleUpdate.value : Infinity;
  
  // Helmet timeout check (20 seconds)
  if (helmetPaired.value && timeSinceHelmetUpdate > 20000) {
    console.log(`[HELMET] ⚠️ No heartbeat for ${(timeSinceHelmetUpdate/1000).toFixed(1)}s - marking as DISCONNECTED`);
    helmetPaired.value = false;
    helmetRawConnected.value = false;
  }
  
  // Motorcycle timeout check (20 seconds)
  if (motorcyclePaired.value && timeSinceMotorcycleUpdate > 20000) {
    console.log(`[MOTORCYCLE] ⚠️ No heartbeat for ${(timeSinceMotorcycleUpdate/1000).toFixed(1)}s - marking as DISCONNECTED`);
    motorcyclePaired.value = false;
    motorcycleRawConnected.value = false;
  }
};

// ✅ NEW: Keyboard shortcut handler
const handleKeyPress = (event) => {
  if (event.key === 'Escape' && showStopSoundButton.value) {
    stopSound();
  }
};

onMounted(() => {
  // ✅ Set app start time when component mounts
  appStartTime.value = Date.now();
  console.log('[INIT] App started at:', new Date(appStartTime.value).toLocaleString());
  
  // ✅ NEW: Add keyboard shortcut to stop sound (Escape key)
  document.addEventListener('keydown', handleKeyPress);
  
  // ✅ Load alert dismissal time
  loadDismissalTime();
  
  // ✅ Initialize rider status as Inactive until devices connect
  riderStatus.value = 'Inactive';
  riderSubtitle.value = 'Waiting for device connection...';
  
  // ✅ Watch for user authentication and setup Firebase listeners
  watch(userId, (newUserId, oldUserId) => {
    if (newUserId && newUserId !== oldUserId) {
      console.log('[AUTH] User authenticated:', newUserId);
      setupFirebaseListeners();  // ✅ CRITICAL: Setup alcohol and other listeners
    } else if (!newUserId && oldUserId) {
      console.log('[AUTH] User logged out');
      // Clear data when user logs out
      alerts.value = [];
      recentTrips.value = [];
      crashEvents.value = [];
      riderStatus.value = 'Inactive';
      riderSubtitle.value = 'Not logged in';
    }
  }, { immediate: true });
  
  // ✅ Automatically start/end trip session when engine starts or stops
  watch(engineRunning, (newRunning, oldRunning) => {
    if (newRunning && !oldRunning) {
      console.log('[ENGINE] Started -> Starting trip session');
      startTripSession('engine');
    } else if (!newRunning && oldRunning) {
      console.log('[ENGINE] Stopped -> Ending and saving trip session');
      endTripSessionAndSave();
    }
  });
  
  // ✅ Setup real-time device listeners (instant updates)
  setupDeviceListeners();
  
  // ✅ Setup engine control listeners
  setupEngineStatusListener();
  
  // ✅ Start backup timeout check (every 2 seconds)
  heartbeatCheckInterval = setInterval(checkDeviceTimeouts, 2000);
  
  console.log('[DASHBOARD] Device monitoring initialized with real-time listeners + 2s timeout checks');
  console.log('[ENGINE] Engine control system initialized');
  
  // ✅ Setup mobile navigation scroll detection
  window.addEventListener('scroll', handleScroll);
  handleScroll(); // Set initial active section

  // ✅ Auto-save active trip on page refresh or browser exit
  window.addEventListener('beforeunload', handleBeforeUnload);

  // ✅ Auto-start high-accuracy Phone GPS speed monitoring on mount
  startGPSSpeedMonitoring(true);
});

// ✅ INSTANT RESPONSIVE Engine Control Methods
const toggleEngine = async () => {
  if (alcoholDetected.value && !engineRunning.value) {
    alert('⚠️ Cannot start engine: Alcohol detected!\n\nPlease wait for alcohol to clear before starting the engine.');
    return;
  }

  const nextState = !engineRunning.value;

  // 1. Set pending state so the button shows visual loading feedback ("Starting Engine..." / "Stopping Engine...")
  enginePending.value = true;
  engineRunning.value = nextState;

  // 2. Play tactile/audio confirmation cue
  try {
    const ctx = new (window.AudioContext || window.webkitAudioContext)();
    const osc = ctx.createOscillator();
    const gain = ctx.createGain();
    osc.connect(gain);
    gain.connect(ctx.destination);
    osc.frequency.value = nextState ? 880 : 440;
    gain.gain.setValueAtTime(0.15, ctx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.01, ctx.currentTime + 0.15);
    osc.start();
    osc.stop(ctx.currentTime + 0.15);
  } catch (e) {}

  const uid = userId.value || authStore.user?.uid || 'DMIARybyNHVhzwAmrx2QQjdpSb13';

  // 3. Trip lifecycle trigger & safety state clearing
  if (nextState) {
    // When starting engine, instantly clear lingering crash status & silence alarms
    sensorData.value.crashDetected = false;
    crashDisplayStatus.value = 'Stable';
    crashDisplayMessage.value = 'Vehicle Stable';
    if (crashInterval) {
      clearInterval(crashInterval);
      crashInterval = null;
    }
    stopSound();
    startTripSession('engine');
  } else {
    endTripSessionAndSave();
  }

  try {
    // 4. Update Firebase instantly across all nodes simultaneously
    const updates = [
      set(dbRef(database, `${uid}/engineControl/command`), nextState ? 'START' : 'STOP'),
      set(dbRef(database, `${uid}/engineControl/startButton`), nextState),
      set(dbRef(database, `${uid}/engineControl/stopButton`), !nextState),
      set(dbRef(database, `${uid}/engineControl/engineRunning`), nextState),
      set(dbRef(database, `helmet_public/${uid}/live/engineRunning`), nextState),
      set(dbRef(database, `helmet_public/${uid}/devices/motorcycle/engineRunning`), nextState)
    ];
    if (nextState) {
      updates.push(set(dbRef(database, `helmet_public/${uid}/live/crashDetected`), false));
      updates.push(set(dbRef(database, `${uid}/engineControl/crashDetected`), false));
    }
    await Promise.all(updates);
    console.log(`[ENGINE] ⚡ Instant toggle command sent -> ${nextState ? 'START' : 'STOP'}`);

    // Auto-clear pending state after 3s (or earlier when Firebase confirms)
    setTimeout(() => {
      enginePending.value = false;
    }, 3000);
  } catch (error) {
    console.error('[ENGINE] Failed to toggle engine in Firebase:', error);
    // Revert state if Firebase rejected
    engineRunning.value = !nextState;
    enginePending.value = false;
    alert('Failed to control engine. Please check internet connection.');
  }
};

// handleEngineToggle — fast pass-through to toggleEngine
const handleEngineToggle = async () => {
  await toggleEngine();
};

const toggleAutoControl = async () => {
  try {
    autoEngineControl.value = !autoEngineControl.value;
    
    // Send auto control setting to Firebase
    const autoRef = dbRef(database, `${userId.value}/engineControl/autoMode`);
    await set(autoRef, autoEngineControl.value);
    
    console.log('[ENGINE] Auto control set to:', autoEngineControl.value);
    
    if (autoEngineControl.value) {
      alert('✅ Automatic Engine Control Enabled\n\n• Engine will auto-stop when alcohol detected\n• Engine will auto-start when alcohol clears');
    } else {
      alert('🔧 Manual Engine Control Enabled\n\n• Use dashboard button to control engine\n• No automatic control based on alcohol detection');
    }
    
  } catch (error) {
    console.error('[ENGINE] Failed to toggle auto control:', error);
  }
};

// ✅ FIXED: Manual engine status sync with better error handling
const syncEngineStatus = async () => {
  console.log('[ENGINE] 🔄 Manual sync requested...');
  
  try {
    // ✅ PRIMARY: Check live data first (most reliable)
    const liveRef = dbRef(database, `helmet_public/${userId.value}/live`);
    const liveSnapshot = await get(liveRef);
    const liveData = liveSnapshot.val();
    
    if (liveData && typeof liveData.engineRunning !== 'undefined') {
      engineRunning.value = liveData.engineRunning;
      console.log(`[ENGINE] ✅ Status synced from live data:`, liveData.engineRunning ? 'RUNNING' : 'STOPPED');
      
      // Also sync sensor data while we're here
      if (liveData.mpu6050) {
        sensorData.value.mpu6050 = {
          accelX: liveData.mpu6050.accelX || 0,
          accelY: liveData.mpu6050.accelY || 0,
          accelZ: liveData.mpu6050.accelZ || 0,
          totalAccel: liveData.mpu6050.totalAccel || 0,
          roll: liveData.mpu6050.roll || 0,
          pitch: liveData.mpu6050.pitch || 0,
          lastUpdate: Date.now()
        };
        console.log(`[SENSORS] MPU6050 data synced - Accel: ${liveData.mpu6050.totalAccel}g, Roll: ${liveData.mpu6050.roll}°`);
      }
      
      if (liveData.gpsValid && liveData.locationLat && liveData.locationLng) {
        location.value = {
          lat: liveData.locationLat,
          lng: liveData.locationLng
        };
        currentSpeed.value = liveData.speed || 0;
        gpsConnected.value = liveData.gpsValid;
        console.log(`[SENSORS] GPS data synced - Speed: ${liveData.speed}km/h, Location: ${liveData.locationLat}, ${liveData.locationLng}`);
      }
      
      return; // Success, no need to check other paths
    }
    
    // ✅ FALLBACK: Check device heartbeat
    const deviceRef = dbRef(database, `helmet_public/${userId.value}/devices/motorcycle`);
    const deviceSnapshot = await get(deviceRef);
    const deviceData = deviceSnapshot.val();
    
    if (deviceData && typeof deviceData.engineRunning !== 'undefined') {
      engineRunning.value = deviceData.engineRunning;
      console.log(`[ENGINE] ✅ Status synced from device heartbeat:`, deviceData.engineRunning ? 'RUNNING' : 'STOPPED');
      return;
    }
    
    console.log('[ENGINE] ⚠️ No engine status found in any path');
    
  } catch (error) {
    console.error('[ENGINE] ❌ Sync error:', error);
  }
  
  // Also sync alcohol status
  try {
    const alcoholSyncRef = dbRef(database, `helmet_public/${userId.value}/alcohol/status`);
    const alcoholSnapshot = await get(alcoholSyncRef);
    const alcoholData = alcoholSnapshot.val();
    
    if (alcoholData !== null) {
      const isAlcoholDetected = alcoholData.status === 'Danger' || alcoholData.status === 'danger' || alcoholData.status === 'DANGER' || alcoholData.detected === true || (alcoholData.sensorValue >= 1500 && alcoholData.sensorValue > 0);
      alcoholDetected.value = isAlcoholDetected;
      console.log('[ALCOHOL] ✅ Status synced:', isAlcoholDetected ? 'DETECTED' : 'SAFE');
      if (isAlcoholDetected && engineRunning.value) {
        console.log('[SAFETY] Alcohol detected on sync while engine is running! Shutting down engine...');
        engineRunning.value = false;
        endTripSessionAndSave();
        const uid = userId.value || 'DMIARybyNHVhzwAmrx2QQjdpSb13';
        set(dbRef(database, `${uid}/engineControl/command`), 'STOP');
        set(dbRef(database, `${uid}/engineControl/stopButton`), true);
        set(dbRef(database, `${uid}/engineControl/startButton`), false);
        set(dbRef(database, `${uid}/engineControl/engineRunning`), false);
        set(dbRef(database, `helmet_public/${uid}/live/engineRunning`), false);
        set(dbRef(database, `helmet_public/${uid}/devices/motorcycle/engineRunning`), false);
      }
    }
  } catch (error) {
    console.log('[ALCOHOL] Error syncing status:', error);
  }
  
  console.log('[ENGINE] 🔄 Manual sync complete');
};

// ✅ FIXED: Monitor engine status from Firebase - Dedicated relay listener
const setupEngineStatusListener = () => {
  console.log('[ENGINE] Setting up engine status listeners...');
  
  // Listen to engine running state from live
  const engineRelayRef = dbRef(database, `helmet_public/${userId.value}/live/engineRunning`);
  onValue(engineRelayRef, (snapshot) => {
    const val = snapshot.val();
    if (val !== null && typeof val !== 'undefined') {
      const isRunning = Boolean(val);
      if (engineRunning.value !== isRunning) {
        engineRunning.value = isRunning;
        enginePending.value = false;
        sensorData.value.engineRunning = isRunning;
        console.log(`[ENGINE] Status updated:`, isRunning ? 'RUNNING' : 'STOPPED');
      }
    }
  });

  // Monitor auto control setting
  const autoRef = dbRef(database, `${userId.value}/engineControl/autoMode`);
  onValue(autoRef, (snapshot) => {
    const data = snapshot.val();
    if (data !== null) {
      autoEngineControl.value = data;
      console.log('[ENGINE] Auto control updated:', data);
    }
  });
  
  console.log('[ENGINE] ✅ Engine status listeners initialized');
};

// ✅ NEW: Mobile Navigation Functions
const scrollToSection = (sectionId) => {
  activeSection.value = sectionId;
  const element = document.getElementById(sectionId);
  if (element) {
    const offset = 80; // Account for any fixed headers
    const elementPosition = element.offsetTop - offset;
    
    window.scrollTo({
      top: elementPosition,
      behavior: 'smooth'
    });
  }
};

// ✅ NEW: Auto-detect active section on scroll
const handleScroll = () => {
  const sections = ['status', 'engine', 'map', 'trips', 'alerts'];
  const scrollPosition = window.scrollY + 200; // Offset for better detection
  
  for (const sectionId of sections) {
    const element = document.getElementById(sectionId);
    if (element) {
      const elementTop = element.offsetTop;
      const elementBottom = elementTop + element.offsetHeight;
      
      if (scrollPosition >= elementTop && scrollPosition < elementBottom) {
        activeSection.value = sectionId;
        break;
      }
    }
  }
};

// ✅ Listen for alerts written by the Arduino to helmet_public/{uid}/alerts/
// These include: alcohol detected, helmet disconnect, crash, speed violations, etc.
const setupArduinoAlertsListener = () => {
  if (!userId.value) return;
  console.log('[ALERTS] Setting up Arduino alerts listener...');

  const arduinoAlertsRef = dbRef(database, `helmet_public/${userId.value}/alerts`);

  // Track seen keys — onChildAdded fires for existing children on load,
  // then fires again for each new child. We pre-index existing keys to prevent stale alerts.
  const seenAlertKeys = new Set();
  let initialAlertLoadDone = false;

  get(arduinoAlertsRef).then((snapshot) => {
    if (snapshot.exists()) {
      snapshot.forEach((child) => {
        seenAlertKeys.add(child.key);
      });
    }
    initialAlertLoadDone = true;
    console.log('[ALERTS] Pre-existing alerts indexed:', seenAlertKeys.size);
  }).catch((err) => {
    console.warn('[ALERTS] Error pre-indexing alerts:', err);
    initialAlertLoadDone = true;
  });

  const sessionStartTime = Date.now();

  onChildAdded(arduinoAlertsRef, (snapshot) => {
    const key = snapshot.key;
    const data = snapshot.val();
    if (!data) return;

    const alertTimestamp = Number(data.timestamp || 0);

    // Skip pre-existing records from before dashboard opened
    if (alertTimestamp > 0 && alertTimestamp < sessionStartTime - 10000) {
      seenAlertKeys.add(key);
      return;
    }

    // Mark pre-existing alerts as seen during initial load
    if (!initialAlertLoadDone) {
      seenAlertKeys.add(key);
      return;
    }

    if (seenAlertKeys.has(key)) return;
    seenAlertKeys.add(key);

    const type = data.type || 'info';

    console.log('[ALERTS] ✅ New Arduino alert received:', data.message, '| Type:', type);

    // Add to alerts list
    alerts.value.unshift({
      type,
      message: data.message || 'System Alert',
      details: data.details || '',
      time: new Date().toLocaleTimeString(),
      timestamp: alertTimestamp || Date.now()
    });
    if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);

    // Update crash status card if it's a crash alert
    if (type === 'crash') {
      crashDisplayStatus.value = 'Alerting';
      crashDisplayMessage.value = 'Crash Detected!';
    }

    // Play sound for actionable alerts
    if (['danger', 'warning', 'crash', 'alcohol', 'theft'].includes(type)) {
      playSound(type);
    }
  });

  console.log('[ALERTS] ✅ Arduino alerts listener active');
};

// ✅ NEW: Setup anti-theft alert listener
const lastDismissedTheftTime = ref(0);

const setupAntiTheftListener = () => {
  if (!userId.value) return;
  console.log('[ANTI-THEFT] Setting up alert listener...');
  
  // Listen for theft alert events (from logTheftToFirebase)
  const theftAlertsRef = dbRef(database, `helmet_public/${userId.value}/theft_alerts`);
  const seenTheftKeys = new Set();
  const sessionStartTime = Date.now();

  onChildAdded(theftAlertsRef, (snapshot) => {
    const key = snapshot.key;
    const alert = snapshot.val();
    if (!alert) return;

    const alertTime = Number(alert.timestamp || alert.alertTime || 0);

    // CRITICAL 1: Skip all historical theft alerts stored prior to this page session
    if (alertTime > 0 && alertTime < sessionStartTime - 10000) {
      seenTheftKeys.add(key);
      return;
    }

    if (seenTheftKeys.has(key)) return;
    seenTheftKeys.add(key);

    // CRITICAL 2: Only trigger if motorcycle hardware is ACTIVELY paired & online!
    // If the hardware is powered off, never show a false anti-theft alert
    if (!motorcyclePaired.value) {
      console.log('[ANTI-THEFT] Skipping theft alert: motorcycle hardware is offline');
      return;
    }

    // CRITICAL 3: Skip if user has already dismissed this alert incident
    if (alertTime > 0 && alertTime <= lastDismissedTheftTime.value) {
      return;
    }

    console.log('[ANTI-THEFT] 🚨 Real-time theft alert received from active bike:', alert);

    antiTheftAlert.value = {
      active: true,
      message: alert.message || alert.details || 'Unauthorized movement detected!',
      time: new Date().toLocaleTimeString(),
      level: alert.alertLevel || 1,
      severity: alert.severity || 'high',
      timestamp: alertTime || Date.now()
    };

    alerts.value.unshift({
      type: 'theft',
      message: '🚨 Anti-Theft Alert',
      details: antiTheftAlert.value.message,
      time: antiTheftAlert.value.time,
      timestamp: alertTime || Date.now()
    });

    playSound('theft');
    if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
  });
};

// ✅ Dismiss anti-theft alert & permanently mute current theft alarm
const dismissAntiTheftAlert = () => {
  antiTheftAlert.value.active = false;
  lastDismissedTheftTime.value = Date.now();
  stopSound();
  console.log('[ANTI-THEFT] Alert dismissed & muted by user');
};

// ✅ NEW: Weather Functions
const getBrowserLocation = () => {
  return new Promise((resolve) => {
    if (!navigator.geolocation) {
      resolve({ lat: 14.5995, lng: 120.9842 }); // Last-resort fallback
      return;
    }
    navigator.geolocation.getCurrentPosition(
      (pos) => resolve({ lat: pos.coords.latitude, lng: pos.coords.longitude }),
      () => resolve({ lat: 14.5995, lng: 120.9842 }), // Denied or unavailable
      { timeout: 5000, maximumAge: 60000 }
    );
  });
};

const fetchWeather = async () => {
  try {
    weather.value.loading = true;
    weather.value.error = null;
    
    // Use motorcycle GPS if available, otherwise fall back to browser geolocation
    let lat = location.value.lat;
    let lng = location.value.lng;

    if (!lat || !lng) {
      const browserPos = await getBrowserLocation();
      lat = browserPos.lat;
      lng = browserPos.lng;
    }
    
    // Using Open-Meteo API (free, no API key required)
    const weatherUrl = `https://api.open-meteo.com/v1/forecast?latitude=${lat}&longitude=${lng}&current=temperature_2m,relative_humidity_2m,apparent_temperature,weather_code,wind_speed_10m,visibility&timezone=auto`;
    
    const response = await fetch(weatherUrl);
    if (!response.ok) throw new Error('Weather data unavailable');
    
    const data = await response.json();
    const current = data.current;
    
    // Get location name using reverse geocoding
    const locationUrl = `https://nominatim.openstreetmap.org/reverse?lat=${lat}&lon=${lng}&format=json`;
    const locationResponse = await fetch(locationUrl);
    const locationData = await locationResponse.json();
    
    weather.value = {
      loading: false,
      error: null,
      location: locationData.address?.city || locationData.address?.town || locationData.address?.municipality || locationData.address?.village || locationData.address?.county || locationData.address?.state || 'Current Location',
      temperature: Math.round(current.temperature_2m),
      feelsLike: Math.round(current.apparent_temperature),
      description: getWeatherDescription(current.weather_code),
      condition: getWeatherCondition(current.weather_code),
      windSpeed: Math.round(current.wind_speed_10m),
      humidity: current.relative_humidity_2m,
      visibility: Math.round(current.visibility / 1000), // Convert to km
      lastUpdate: new Date().toLocaleTimeString()
    };
    
    console.log('[WEATHER] Data updated:', weather.value);
  } catch (error) {
    console.error('[WEATHER] Error fetching data:', error);
    weather.value.loading = false;
    weather.value.error = 'Unable to load weather data';
  }
};

const refreshWeather = () => {
  console.log('[WEATHER] Refreshing data...');
  fetchWeather();
};

const getWeatherDescription = (code) => {
  const descriptions = {
    0: 'Clear Sky',
    1: 'Mainly Clear',
    2: 'Partly Cloudy',
    3: 'Overcast',
    45: 'Foggy',
    48: 'Foggy',
    51: 'Light Drizzle',
    53: 'Drizzle',
    55: 'Heavy Drizzle',
    61: 'Light Rain',
    63: 'Rain',
    65: 'Heavy Rain',
    71: 'Light Snow',
    73: 'Snow',
    75: 'Heavy Snow',
    77: 'Snow Grains',
    80: 'Light Showers',
    81: 'Showers',
    82: 'Heavy Showers',
    85: 'Light Snow Showers',
    86: 'Snow Showers',
    95: 'Thunderstorm',
    96: 'Thunderstorm with Hail',
    99: 'Severe Thunderstorm'
  };
  return descriptions[code] || 'Unknown';
};

const getWeatherCondition = (code) => {
  if (code === 0 || code === 1) return 'clear';
  if (code === 2 || code === 3) return 'cloudy';
  if (code >= 45 && code <= 48) return 'fog';
  if (code >= 51 && code <= 67) return 'rain';
  if (code >= 71 && code <= 86) return 'snow';
  if (code >= 95) return 'storm';
  return 'unknown';
};

const getWeatherEmoji = (condition) => {
  const emojis = {
    clear: '☀️',
    cloudy: '☁️',
    fog: '🌫️',
    rain: '🌧️',
    snow: '❄️',
    storm: '⛈️'
  };
  return emojis[condition] || '🌤️';
};

const getRidingSafetyLevel = () => {
  const temp = weather.value.temperature;
  const wind = weather.value.windSpeed;
  const condition = weather.value.condition;
  
  // Dangerous conditions
  if (condition === 'storm' || condition === 'snow') return 'Dangerous';
  if (wind > 40) return 'Dangerous';
  
  // Poor conditions
  if (condition === 'rain' && wind > 25) return 'Poor';
  if (temp < 10 || temp > 38) return 'Poor';
  if (wind > 30) return 'Poor';
  
  // Fair conditions
  if (condition === 'rain') return 'Fair';
  if (condition === 'fog') return 'Fair';
  if (wind > 20) return 'Fair';
  
  // Good conditions
  return 'Excellent';
};

const getRidingSafetyColor = () => {
  const level = getRidingSafetyLevel();
  if (level === 'Dangerous') return 'bg-red-50 border-red-500';
  if (level === 'Poor') return 'bg-orange-50 border-orange-500';
  if (level === 'Fair') return 'bg-yellow-50 border-yellow-500';
  return 'bg-green-50 border-green-500';
};

const getRidingSafetyIcon = () => {
  const level = getRidingSafetyLevel();
  if (level === 'Dangerous') return 'dangerous';
  if (level === 'Poor') return 'warning';
  if (level === 'Fair') return 'info';
  return 'check_circle';
};

const getRidingSafetyIconColor = () => {
  const level = getRidingSafetyLevel();
  if (level === 'Dangerous') return 'text-red-400';
  if (level === 'Poor') return 'text-orange-400';
  if (level === 'Fair') return 'text-yellow-400';
  return 'text-green-400';
};

const getRidingSafetyTextColor = () => {
  const level = getRidingSafetyLevel();
  if (level === 'Dangerous') return 'text-red-300';
  if (level === 'Poor') return 'text-orange-300';
  if (level === 'Fair') return 'text-yellow-300';
  return 'text-green-300';
};

const getRidingSafetyMessage = () => {
  const level = getRidingSafetyLevel();
  if (level === 'Dangerous') return 'Not recommended to ride. Severe weather conditions.';
  if (level === 'Poor') return 'Ride with extreme caution. Challenging conditions.';
  if (level === 'Fair') return 'Acceptable conditions. Stay alert and ride carefully.';
  return 'Perfect conditions for riding. Enjoy your trip!';
};

// ✅ NEW: Get current time for user header
const getCurrentTime = () => {
  const now = new Date();
  return now.toLocaleTimeString('en-US', { 
    hour: '2-digit', 
    minute: '2-digit',
    hour12: true 
  });
};

const getWeatherAlerts = () => {
  const alerts = [];
  const temp = weather.value.temperature;
  const wind = weather.value.windSpeed;
  const condition = weather.value.condition;
  const visibility = weather.value.visibility;
  
  if (condition === 'rain') {
    alerts.push({
      type: 'rain',
      icon: 'water_drop',
      color: 'text-blue-600',
      message: 'Wet roads - Reduce speed and increase following distance'
    });
  }
  
  if (condition === 'storm') {
    alerts.push({
      type: 'storm',
      icon: 'thunderstorm',
      color: 'text-red-600',
      message: 'Thunderstorm warning - Seek shelter immediately'
    });
  }
  
  if (wind > 30) {
    alerts.push({
      type: 'wind',
      icon: 'air',
      color: 'text-orange-600',
      message: `Strong winds (${wind} km/h) - Maintain firm grip on handlebars`
    });
  }
  
  if (temp > 35) {
    alerts.push({
      type: 'heat',
      icon: 'wb_sunny',
      color: 'text-orange-600',
      message: 'High temperature - Stay hydrated and take breaks'
    });
  }
  
  if (temp < 15) {
    alerts.push({
      type: 'cold',
      icon: 'ac_unit',
      color: 'text-blue-600',
      message: 'Cold weather - Wear appropriate gear and warm up engine'
    });
  }
  
  if (visibility < 5) {
    alerts.push({
      type: 'visibility',
      icon: 'visibility_off',
      color: 'text-yellow-600',
      message: 'Low visibility - Use headlights and reduce speed'
    });
  }
  
  if (condition === 'fog') {
    alerts.push({
      type: 'fog',
      icon: 'cloud',
      color: 'text-gray-600',
      message: 'Foggy conditions - Use fog lights and ride slowly'
    });
  }
  
  return alerts;
};

// Handle browser close or page unload
const handleBeforeUnload = () => {
  if (activeTripSession.value) {
    endTripSessionAndSave();
  }
};

// Cleanup on component unmount
onBeforeUnmount(() => {
  window.removeEventListener('beforeunload', handleBeforeUnload);
  // ✅ NEW: Cleanup audio and keyboard event listener
  if (currentAudio) {
    currentAudio.pause();
    currentAudio = null;
  }
  document.removeEventListener('keydown', handleKeyPress);
  
  // Stop GPS monitoring if active
  if (gpsWatchId !== null) {
    navigator.geolocation.clearWatch(gpsWatchId);
    gpsWatchId = null;
  }
  
  // Clear crash interval if active
  if (crashInterval) {
    clearInterval(crashInterval);
  }
  
  // Clear heartbeat check interval
  if (heartbeatCheckInterval) {
    clearInterval(heartbeatCheckInterval);
  }
  
  // Save active trip session if in progress
  if (activeTripSession.value) {
    endTripSessionAndSave();
  }
  
  // ✅ NEW: Clean up scroll listener
  window.removeEventListener('scroll', handleScroll);
});
</script>

<style scoped>
/* Custom Scrollbar */
.custom-scrollbar::-webkit-scrollbar {
  width: 10px;
}

.custom-scrollbar::-webkit-scrollbar-track {
  background: linear-gradient(to bottom, #EDE8F5, #f5f3f7);
  border-radius: 10px;
  margin: 4px;
}

.custom-scrollbar::-webkit-scrollbar-thumb {
  background: linear-gradient(to bottom, #7091E6, #5571c6);
  border-radius: 10px;
  border: 2px solid #EDE8F5;
  transition: all 0.3s ease;
}

.custom-scrollbar::-webkit-scrollbar-thumb:hover {
  background: linear-gradient(to bottom, #3D52A0, #2a3a70);
  border-color: #d5d0e0;
}

/* Range Slider Custom Styling */
input[type="range"].slider-thumb::-webkit-slider-thumb {
  appearance: none;
  width: 24px;
  height: 24px;
  border-radius: 50%;
  background: linear-gradient(135deg, #3D52A0, #2a3a70);
  cursor: pointer;
  box-shadow: 0 4px 12px rgba(61, 82, 160, 0.4);
  border: 3px solid white;
  transition: all 0.3s ease;
}

input[type="range"].slider-thumb::-webkit-slider-thumb:hover {
  transform: scale(1.2);
  box-shadow: 0 6px 20px rgba(61, 82, 160, 0.6);
}

input[type="range"].slider-thumb::-moz-range-thumb {
  width: 24px;
  height: 24px;
  border-radius: 50%;
  background: linear-gradient(135deg, #3D52A0, #2a3a70);
  cursor: pointer;
  box-shadow: 0 4px 12px rgba(61, 82, 160, 0.4);
  border: 3px solid white;
  transition: all 0.3s ease;
}

input[type="range"].slider-thumb::-moz-range-thumb:hover {
  transform: scale(1.2);
  box-shadow: 0 6px 20px rgba(61, 82, 160, 0.6);
}

/* Glassmorphism effect */
.backdrop-blur-md {
  backdrop-filter: blur(12px);
  -webkit-backdrop-filter: blur(12px);
}

.backdrop-blur-lg {
  backdrop-filter: blur(16px);
  -webkit-backdrop-filter: blur(16px);
}

/* Smooth animations */
@keyframes float {
  0%, 100% {
    transform: translateY(0px);
  }
  50% {
    transform: translateY(-10px);
  }
}

.group:hover .material-icons {
  animation: float 2s ease-in-out infinite;
}

/* Shadow utilities */
.shadow-3xl {
  box-shadow: 0 35px 60px -15px rgba(0, 0, 0, 0.3);
}
</style>
