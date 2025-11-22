<template>
  <div class="min-h-screen flex flex-col bg-gradient-to-br from-[#EDE8F5] via-[#f5f3f7] to-[#e8e4f0]">
    <!-- User Greeting/Header -->
    <div class="flex flex-col items-center justify-center py-6 px-4">
      <div class="relative overflow-hidden">
        <div class="absolute inset-0 bg-gradient-to-r from-[#7091E6] via-[#5571c6] to-[#3D52A0] opacity-90 blur-xl"></div>
        <div class="relative flex items-center gap-4 bg-white/20 backdrop-blur-md border border-white/30 px-8 py-4 rounded-2xl shadow-2xl">
          <div class="relative">
            <span v-if="authStore.user && authStore.user.photoURL">
              <img :src="authStore.user.photoURL" alt="User Avatar"
                class="w-12 h-12 rounded-full border-3 border-white shadow-lg ring-2 ring-white/50"
                @error="onAvatarError" />
            </span>
            <span v-else class="material-icons text-4xl text-white drop-shadow-lg">account_circle</span>
            <div class="absolute -bottom-1 -right-1 w-4 h-4 bg-green-400 rounded-full border-2 border-white"></div>
          </div>
          <div>
            <p class="text-xs text-white/80 font-medium uppercase tracking-wider">Welcome Back</p>
            <p class="font-bold text-xl text-white drop-shadow-md">
              <span v-if="authStore.user?.displayName">{{ authStore.user.displayName }}</span>
              <span v-else-if="authStore.user?.email">{{ authStore.user.email }}</span>
              <span v-else>User</span>
            </p>
          </div>
        </div>
      </div>
    </div>

    <!-- ⚠️ HELMET REMOVED WARNING BANNER -->
    <div v-if="riderStatus === 'Inactive' && previousHelmetState !== null" 
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

    <!-- Main Dashboard -->
    <main class="flex-1 px-4 md:px-8 py-6">
      <!-- System Status Bar -->
      <div class="grid grid-cols-1 md:grid-cols-3 gap-4 mb-6">
        <!-- Helmet-Motorcycle Pairing Status -->
        <div class="relative overflow-hidden bg-white/80 backdrop-blur-lg rounded-2xl shadow-lg p-5 border border-white/50">
          <div class="flex items-center justify-between">
            <div class="flex items-center gap-3">
              <div :class="['p-3 rounded-xl', helmetPaired && motorcyclePaired ? 'bg-green-500' : 'bg-red-500']">
                <span class="material-icons text-white text-2xl">link</span>
              </div>
              <div>
                <p class="text-xs text-gray-500 font-medium uppercase">Pairing Status</p>
                <p :class="['text-lg font-bold', helmetPaired && motorcyclePaired ? 'text-green-600' : 'text-red-600']">
                  {{ helmetPaired && motorcyclePaired ? 'Connected' : 'Disconnected' }}
                </p>
              </div>
            </div>
            <div class="flex flex-col gap-1">
              <div class="flex items-center gap-2">
                <span class="material-icons text-sm" :class="helmetPaired ? 'text-green-500' : 'text-gray-400'">sports_motorsports</span>
                <span class="text-xs font-medium" :class="helmetPaired ? 'text-green-600' : 'text-gray-500'">Helmet</span>
              </div>
              <div class="flex items-center gap-2">
                <span class="material-icons text-sm" :class="motorcyclePaired ? 'text-green-500' : 'text-gray-400'">two_wheeler</span>
                <span class="text-xs font-medium" :class="motorcyclePaired ? 'text-green-600' : 'text-gray-500'">Motorcycle</span>
              </div>
            </div>
          </div>
        </div>

        <!-- Device Health -->
        <div class="relative overflow-hidden bg-white/80 backdrop-blur-lg rounded-2xl shadow-lg p-5 border border-white/50">
          <div class="flex items-center justify-between">
            <div class="flex items-center gap-3">
              <div class="p-3 rounded-xl bg-gradient-to-br from-blue-500 to-blue-600">
                <span class="material-icons text-white text-2xl">battery_charging_full</span>
              </div>
              <div>
                <p class="text-xs text-gray-500 font-medium uppercase">Device Health</p>
                <p class="text-lg font-bold text-blue-600">{{ deviceBattery }}%</p>
              </div>
            </div>
            <div class="flex flex-col gap-1 text-right">
              <div class="flex items-center gap-2">
                <span class="material-icons text-sm" :class="gsmConnected ? 'text-green-500' : 'text-red-500'">signal_cellular_alt</span>
                <span class="text-xs font-medium">GSM</span>
              </div>
              <div class="flex items-center gap-2">
                <span class="material-icons text-sm" :class="gpsConnected ? 'text-green-500' : 'text-red-500'">gps_fixed</span>
                <span class="text-xs font-medium">GPS</span>
              </div>
            </div>
          </div>
        </div>

        <!-- Emergency SOS Button -->
        <div class="relative overflow-hidden bg-gradient-to-br from-red-500 to-red-600 rounded-2xl shadow-lg p-5 border border-white/50 cursor-pointer hover:scale-105 transition-all duration-300" @click="triggerSOS">
          <div class="flex items-center justify-between">
            <div class="flex items-center gap-3">
              <div class="p-3 rounded-xl bg-white/20 backdrop-blur-sm animate-pulse">
                <span class="material-icons text-white text-3xl">emergency</span>
              </div>
              <div>
                <p class="text-xs text-white/80 font-medium uppercase">Emergency</p>
                <p class="text-xl font-bold text-white">SOS Alert</p>
              </div>
            </div>
            <span class="material-icons text-white text-4xl">touch_app</span>
          </div>
        </div>
      </div>

      <!-- Top Cards -->
      <div class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-5 gap-5 mb-8">
        <!-- Rider Status -->
        <div :class="[
          'group relative overflow-hidden text-white rounded-3xl shadow-xl p-6 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer',
          riderStatus === 'Inactive' ? 'bg-gradient-to-br from-red-600 via-red-500 to-red-700 animate-pulse' : 'bg-gradient-to-br from-[#3D52A0] via-[#4a5fb8] to-[#2a3a70]'
        ]">
          <div class="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-4">
              <div :class="['p-3 rounded-xl', riderStatus === 'Inactive' ? 'bg-white/30 animate-bounce' : 'bg-white/20']">
                <svg class="w-10 h-10 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M16 7a4 4 0 11-8 0 4 4 0 018 0zM12 14a7 7 0 00-7 7h14a7 7 0 00-7-7z"/>
                </svg>
              </div>
              <div :class="['w-3 h-3 rounded-full', riderStatus === 'Inactive' ? 'bg-yellow-300 animate-ping' : 'bg-green-400 animate-pulse']"></div>
            </div>
            <div class="mt-auto">
              <p class="text-sm opacity-80 mb-1">Rider Status</p>
              <p class="text-2xl font-bold">{{ riderStatus }}</p>
              <p class="text-xs opacity-70 mt-1">{{ riderSubtitle }}</p>
              <p v-if="riderStatus === 'Inactive'" class="text-xs font-bold text-yellow-300 mt-2 animate-pulse">
                ⚠️ HELMET REMOVED!
              </p>
            </div>
          </div>
        </div>
        
        <!-- Current Speed -->
        <div :class="[
          'group relative overflow-hidden text-white rounded-3xl shadow-xl p-6 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer',
          isOverSpeed ? 'bg-gradient-to-br from-red-500 via-red-600 to-red-700 animate-pulse' : 'bg-gradient-to-br from-[#7091E6] via-[#6081d6] to-[#5571c6]'
        ]">
          <div class="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-4">
              <div class="bg-white/20 p-3 rounded-xl">
                <svg class="w-10 h-10 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M13 10V3L4 14h7v7l9-11h-7z"/>
                </svg>
              </div>
              <div :class="['w-3 h-3 rounded-full', isOverSpeed ? 'bg-yellow-300 animate-pulse' : 'bg-blue-300']"></div>
            </div>
            <div class="mt-auto">
              <p class="text-sm opacity-80 mb-1">Current Speed</p>
              <p class="text-2xl font-bold">{{ currentSpeedText }}</p>
              <div class="flex items-center gap-2 mt-1">
                <span class="material-icons text-xs opacity-70">{{ gpsSpeedSource === 'phone' ? 'smartphone' : 'router' }}</span>
                <p class="text-xs opacity-70">{{ isOverSpeed ? 'Over speed limit!' : gpsSpeedSource === 'phone' ? 'Phone GPS' : 'GPS Module' }}</p>
              </div>
            </div>
          </div>
        </div>
        
        <!-- Alertness -->
        <div :class="[
          'group relative overflow-hidden rounded-3xl shadow-xl p-6 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer',
          alertnessStatus === 'Normal' ? 'bg-gradient-to-br from-[#8697C4] via-[#7687b4] to-[#6677a4] text-white' : 'bg-gradient-to-br from-yellow-400 via-yellow-500 to-yellow-600 text-gray-800'
        ]">
          <div class="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-4">
              <div class="bg-white/20 p-3 rounded-xl">
                <svg class="w-10 h-10" :class="alertnessStatus === 'Normal' ? 'text-white' : 'text-gray-800'" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 12a3 3 0 11-6 0 3 3 0 016 0z"/>
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M2.458 12C3.732 7.943 7.523 5 12 5c4.478 0 8.268 2.943 9.542 7-1.274 4.057-5.064 7-9.542 7-4.477 0-8.268-2.943-9.542-7z"/>
                </svg>
              </div>
              <div :class="['w-3 h-3 rounded-full', alertnessStatus === 'Normal' ? 'bg-green-400' : 'bg-orange-400 animate-pulse']"></div>
            </div>
            <div class="mt-auto">
              <p class="text-sm opacity-80 mb-1">Alertness</p>
              <p class="text-2xl font-bold">{{ alertnessStatus }}</p>
              <p class="text-xs opacity-70 mt-1">{{ alertnessSubtitle }}</p>
            </div>
          </div>
        </div>
        
        <!-- Alcohol Detection -->
        <div :class="[
          'group relative overflow-hidden rounded-3xl shadow-xl p-6 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer',
          alcoholStatus === 'Safe' ? 'bg-gradient-to-br from-[#3D52A0] via-[#4a5fb8] to-[#2a3a70] text-white' : 'bg-gradient-to-br from-red-500 via-red-600 to-red-700 text-white'
        ]">
          <div class="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-4">
              <div class="bg-white/20 p-3 rounded-xl">
                <svg class="w-10 h-10 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12l2 2 4-4m5.618-4.016A11.955 11.955 0 0112 2.944a11.955 11.955 0 01-8.618 3.04A12.02 12.02 0 003 9c0 5.591 3.824 10.29 9 11.622 5.176-1.332 9-6.03 9-11.622 0-1.042-.133-2.052-.382-3.016z"/>
                </svg>
              </div>
              <div :class="['w-3 h-3 rounded-full', alcoholStatus === 'Safe' ? 'bg-green-400' : 'bg-red-300 animate-pulse']"></div>
            </div>
            <div class="mt-auto">
              <p class="text-sm opacity-80 mb-1">Alcohol Detection</p>
              <p class="text-2xl font-bold">{{ alcoholStatus }}</p>
              <p class="text-xs opacity-70 mt-1">{{ alcoholSubtitle }}</p>
            </div>
          </div>
        </div>
        
        <!-- Crash Detection Card -->
        <div :class="[
          'group relative overflow-hidden rounded-3xl shadow-xl p-6 transition-all duration-500 hover:scale-105 hover:shadow-2xl cursor-pointer',
          crashDisplayStatus === 'Stable' ? 'bg-gradient-to-br from-green-600 via-green-700 to-green-800 text-white' : 'bg-gradient-to-br from-red-600 via-red-700 to-red-800 text-white animate-pulse'
        ]">
          <div class="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
          <div class="relative flex flex-col h-full">
            <div class="flex items-center justify-between mb-4">
              <div class="bg-white/20 p-3 rounded-xl">
                <svg class="w-10 h-10 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 9v2m0 4h.01m-6.938 4h13.856c1.54 0 2.502-1.667 1.732-3L13.732 4c-.77-1.333-2.694-1.333-3.464 0L3.34 16c-.77 1.333.192 3 1.732 3z"/>
                </svg>
              </div>
              <div :class="['w-3 h-3 rounded-full', crashDisplayStatus === 'Stable' ? 'bg-emerald-300' : 'bg-yellow-300 animate-pulse']"></div>
            </div>
            <div class="mt-auto">
              <p class="text-sm opacity-80 mb-1">Vehicle Status</p>
              <p class="text-2xl font-bold">{{ crashDisplayMessage }}</p>
              <p class="text-xs opacity-70 mt-1">{{ crashDisplayStatus === 'Stable' ? 'No incidents' : 'Alert active' }}</p>
            </div>
          </div>
        </div>
      </div>

      <!-- ✅ DEBUG PANEL - Real-time Data Monitor -->
      <div class="relative overflow-hidden bg-gradient-to-br from-yellow-50 to-orange-50 backdrop-blur-lg rounded-3xl shadow-2xl p-6 mb-8 border-4 border-yellow-400">
        <div class="flex items-center gap-3 mb-4">
          <span class="material-icons text-3xl text-yellow-600">bug_report</span>
          <h3 class="text-xl font-bold text-yellow-800">🔍 Real-Time Data Monitor (Debug)</h3>
        </div>
        <div class="grid grid-cols-1 md:grid-cols-3 gap-4 font-mono text-sm">
          <div class="bg-white p-4 rounded-xl border-2 border-yellow-300">
            <p class="font-bold text-gray-700 mb-2">🍺 Alcohol Detection</p>
            <p class="text-xs"><span class="font-semibold">Status:</span> <span :class="alcoholStatus === 'Danger' ? 'text-red-600 font-bold' : 'text-green-600'">{{ alcoholStatus }}</span></p>
            <p class="text-xs"><span class="font-semibold">Subtitle:</span> {{ alcoholSubtitle }}</p>
            <p class="text-xs"><span class="font-semibold">Sensor Value:</span> {{ sensorData.alcohol.value }}</p>
            <p class="text-xs"><span class="font-semibold">Last Update:</span> {{ new Date(sensorData.alcohol.lastUpdate).toLocaleTimeString() }}</p>
          </div>
          <div class="bg-white p-4 rounded-xl border-2 border-blue-300">
            <p class="font-bold text-gray-700 mb-2">👤 Rider Status</p>
            <p class="text-xs"><span class="font-semibold">Status:</span> <span :class="riderStatus === 'Active' ? 'text-green-600' : 'text-red-600'">{{ riderStatus }}</span></p>
            <p class="text-xs"><span class="font-semibold">Subtitle:</span> {{ riderSubtitle }}</p>
            <p class="text-xs"><span class="font-semibold">Helmet Paired:</span> {{ helmetPaired ? '✓ Yes' : '✗ No' }}</p>
            <p class="text-xs"><span class="font-semibold">Motorcycle Paired:</span> {{ motorcyclePaired ? '✓ Yes' : '✗ No' }}</p>
          </div>
          <div class="bg-white p-4 rounded-xl border-2 border-purple-300">
            <p class="font-bold text-gray-700 mb-2">👁️ Alertness</p>
            <p class="text-xs"><span class="font-semibold">Status:</span> <span :class="alertnessStatus === 'Normal' ? 'text-green-600' : 'text-orange-600'">{{ alertnessStatus }}</span></p>
            <p class="text-xs"><span class="font-semibold">Subtitle:</span> {{ alertnessSubtitle }}</p>
          </div>
        </div>
        <div class="mt-4 bg-white p-4 rounded-xl border-2 border-green-300">
          <p class="font-bold text-gray-700 mb-2">📊 Live Data from Firebase</p>
          <div class="grid grid-cols-2 md:grid-cols-4 gap-2 text-xs">
            <p><span class="font-semibold">Speed:</span> {{ currentSpeed.toFixed(1) }} kph</p>
            <p><span class="font-semibold">Battery:</span> {{ sensorData.battery.voltage }}</p>
            <p><span class="font-semibold">GSM:</span> {{ sensorData.gsm.signal }}</p>
            <p><span class="font-semibold">GPS Sats:</span> {{ sensorData.gps.satellites }}</p>
          </div>
        </div>
        <p class="text-xs text-yellow-700 mt-3 text-center">
          ⚠️ This panel shows real-time reactive data. If values don't match Firebase, check browser console for listener logs.
        </p>
      </div>

      <!-- Electrical Diagnostics Panel -->
      <div class="relative overflow-hidden bg-white/80 backdrop-blur-lg rounded-3xl shadow-2xl p-8 mb-8 border border-white/50">
        <div class="absolute top-0 left-0 w-64 h-64 bg-gradient-to-br from-purple-500/10 to-blue-500/10 rounded-full -ml-32 -mt-32"></div>
        <h3 class="relative text-2xl font-bold text-[#3D52A0] mb-6 flex items-center gap-3">
          <div class="bg-gradient-to-br from-purple-500 to-purple-600 p-3 rounded-2xl shadow-lg">
            <span class="material-icons text-3xl text-white">electrical_services</span>
          </div>
          <span>Motorcycle Electrical Diagnostics</span>
        </h3>
        <div class="relative grid grid-cols-2 md:grid-cols-4 gap-4">
          <!-- Headlight -->
          <div :class="['p-4 rounded-xl border-2 transition-all duration-300', electricalDiagnostics.headlight ? 'bg-green-50 border-green-500' : 'bg-red-50 border-red-500']">
            <div class="flex flex-col items-center gap-2">
              <span class="material-icons text-4xl" :class="electricalDiagnostics.headlight ? 'text-green-600' : 'text-red-600'">lightbulb</span>
              <p class="text-sm font-semibold text-gray-700">Headlight</p>
              <p :class="['text-xs font-bold', electricalDiagnostics.headlight ? 'text-green-600' : 'text-red-600']">
                {{ electricalDiagnostics.headlight ? 'OK' : 'FAULT' }}
              </p>
            </div>
          </div>
          
          <!-- Taillight -->
          <div :class="['p-4 rounded-xl border-2 transition-all duration-300', electricalDiagnostics.taillight ? 'bg-green-50 border-green-500' : 'bg-red-50 border-red-500']">
            <div class="flex flex-col items-center gap-2">
              <span class="material-icons text-4xl" :class="electricalDiagnostics.taillight ? 'text-green-600' : 'text-red-600'">highlight</span>
              <p class="text-sm font-semibold text-gray-700">Taillight</p>
              <p :class="['text-xs font-bold', electricalDiagnostics.taillight ? 'text-green-600' : 'text-red-600']">
                {{ electricalDiagnostics.taillight ? 'OK' : 'FAULT' }}
              </p>
            </div>
          </div>
          
          <!-- Brake Light -->
          <div :class="['p-4 rounded-xl border-2 transition-all duration-300', electricalDiagnostics.brakeLight ? 'bg-green-50 border-green-500' : 'bg-red-50 border-red-500']">
            <div class="flex flex-col items-center gap-2">
              <span class="material-icons text-4xl" :class="electricalDiagnostics.brakeLight ? 'text-green-600' : 'text-red-600'">traffic</span>
              <p class="text-sm font-semibold text-gray-700">Brake Light</p>
              <p :class="['text-xs font-bold', electricalDiagnostics.brakeLight ? 'text-green-600' : 'text-red-600']">
                {{ electricalDiagnostics.brakeLight ? 'OK' : 'FAULT' }}
              </p>
            </div>
          </div>
          
          <!-- Signal Lights -->
          <div :class="['p-4 rounded-xl border-2 transition-all duration-300', electricalDiagnostics.signalLights ? 'bg-green-50 border-green-500' : 'bg-red-50 border-red-500']">
            <div class="flex flex-col items-center gap-2">
              <span class="material-icons text-4xl" :class="electricalDiagnostics.signalLights ? 'text-green-600' : 'text-red-600'">turn_slight_right</span>
              <p class="text-sm font-semibold text-gray-700">Signal Lights</p>
              <p :class="['text-xs font-bold', electricalDiagnostics.signalLights ? 'text-green-600' : 'text-red-600']">
                {{ electricalDiagnostics.signalLights ? 'OK' : 'FAULT' }}
              </p>
            </div>
          </div>
        </div>
      </div>

      <!-- Speed Limit Control -->
      <div class="relative overflow-hidden bg-gradient-to-r from-amber-400 via-yellow-400 to-amber-500 rounded-3xl shadow-2xl p-8 mb-8 transition-all duration-500 hover:shadow-3xl group">
        <div class="absolute inset-0 bg-gradient-to-r from-yellow-300/20 to-amber-400/20 opacity-0 group-hover:opacity-100 transition-opacity duration-500"></div>
        <div class="relative">
          <div class="flex justify-between items-center mb-5">
            <label for="speed-limit" class="font-bold text-gray-900 flex items-center gap-3 text-lg">
              <div class="bg-white/30 backdrop-blur-sm p-2 rounded-xl">
                <span class="material-icons text-3xl text-gray-800">tune</span>
              </div>
              <span>Speed Limit Control</span>
            </label>
            <div class="relative">
              <div class="absolute inset-0 bg-white/40 blur-xl rounded-2xl"></div>
              <span class="relative text-3xl font-black text-gray-900 bg-white/60 backdrop-blur-md px-6 py-3 rounded-2xl shadow-lg border border-white/50">
                {{ speedLimit }} <span class="text-xl font-semibold">km/h</span>
              </span>
            </div>
          </div>
          <div class="relative">
            <input type="range" min="0" max="120" step="5" v-model.number="speedLimit"
              @change="updateSpeedLimitInFirebase"
              class="w-full h-4 bg-white/40 backdrop-blur-sm rounded-full cursor-pointer focus:outline-none focus:ring-4 focus:ring-white/50 accent-gray-800 transition-all appearance-none slider-thumb" />
            <div class="flex justify-between mt-3 text-sm font-bold text-gray-800">
              <span class="bg-white/30 px-3 py-1 rounded-lg">1 km/h</span>
              <span class="bg-white/30 px-3 py-1 rounded-lg">120 km/h</span>
            </div>
          </div>
        </div>
      </div>

      <!-- Sensor Health Panel -->
      <SensorHealthPanel 
        :gps-connected="gpsConnected"
        :gsm-connected="gsmConnected"
        :device-battery="deviceBattery"
        :alcohol-status="alcoholStatus"
        :sensor-data="sensorData"
        class="mb-8"
      />

      <!-- Trip Statistics -->
      <TripStatistics 
        :stats="tripStats"
        class="mb-8"
      />

      <!-- GPS Speed Monitoring Control -->
      <div class="relative overflow-hidden bg-gradient-to-r from-blue-500 via-blue-600 to-blue-700 rounded-3xl shadow-2xl p-8 mb-8 transition-all duration-500 hover:shadow-3xl group">
        <div class="absolute inset-0 bg-gradient-to-r from-blue-400/20 to-blue-600/20 opacity-0 group-hover:opacity-100 transition-opacity duration-500"></div>
        <div class="relative">
          <div class="flex flex-col md:flex-row justify-between items-start md:items-center gap-4">
            <div class="flex items-center gap-4">
              <div class="bg-white/30 backdrop-blur-sm p-3 rounded-xl">
                <span class="material-icons text-4xl text-white">speed</span>
              </div>
              <div>
                <h3 class="font-bold text-white text-xl mb-1">Real-Time GPS Speed Monitoring</h3>
                <p class="text-sm text-white/80">
                  {{ isGPSSpeedActive ? 'Active - Using phone GPS for speed tracking' : 'Inactive - Using GPS module data' }}
                </p>
                <div class="flex items-center gap-2 mt-2">
                  <span class="w-2 h-2 rounded-full" :class="isGPSSpeedActive ? 'bg-green-400 animate-pulse' : 'bg-gray-300'"></span>
                  <span class="text-xs text-white/70 font-semibold">
                    Source: {{ gpsSpeedSource === 'phone' ? 'Phone GPS' : 'GPS Module' }}
                  </span>
                </div>
              </div>
            </div>
            <button
              @click="toggleGPSSpeedMonitoring"
              :class="[
                'inline-flex items-center gap-3 px-6 py-4 rounded-xl font-bold text-lg transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105',
                isGPSSpeedActive 
                  ? 'bg-red-500 hover:bg-red-600 text-white' 
                  : 'bg-white hover:bg-gray-100 text-blue-600'
              ]"
            >
              <span class="material-icons text-2xl">
                {{ isGPSSpeedActive ? 'gps_off' : 'gps_fixed' }}
              </span>
              <span>{{ isGPSSpeedActive ? 'Stop Monitoring' : 'Start Monitoring' }}</span>
            </button>
          </div>
        </div>
      </div>

      <!-- Tabs -->
      <div class="mb-6 relative z-10">
        <div class="bg-white/80 backdrop-blur-md rounded-2xl shadow-xl p-4 border border-white/50">
          <TabGroup :tabs="['My Location', 'Speed Data']" v-model="activeTab" />
        </div>
      </div>

      <!-- Tab Content -->
      <div class="relative z-0">
        <div v-show="activeTab === 'My Location'" class="bg-white/90 backdrop-blur-lg rounded-3xl shadow-2xl p-6 md:p-8 mb-6 border-2 border-white/50 transition-all duration-300">
          <LocationSection :location="location" :user="user" :crash-events="crashEvents" @update-location="handleLocationUpdate" />
        </div>
        <div v-show="activeTab === 'Speed Data'" class="bg-white/90 backdrop-blur-lg rounded-3xl shadow-2xl p-6 md:p-8 mb-6 border-2 border-white/50 transition-all duration-300">
          <SpeedDataSection
            :currentSpeed="currentSpeed"
            :speedLimit="speedLimit"
            :speedHistory="speedHistory"
            @overspeed="handleOverspeed"
          />
        </div>
      </div>

      <!-- Recent Alerts Section -->
      <div class="relative overflow-hidden bg-white/80 backdrop-blur-lg rounded-3xl shadow-2xl p-8 mb-6 border border-white/50 transition-all duration-300 hover:shadow-3xl">
        <div class="absolute top-0 right-0 w-64 h-64 bg-gradient-to-br from-[#7091E6]/10 to-[#3D52A0]/10 rounded-full -mr-32 -mt-32"></div>
        <h3 class="relative font-bold text-2xl mb-6 text-[#3D52A0] flex items-center justify-between">
          <span class="flex items-center gap-3">
            <div class="bg-gradient-to-br from-[#7091E6] to-[#3D52A0] p-3 rounded-2xl shadow-lg">
              <span class="material-icons text-3xl text-white">notifications_active</span>
            </div>
            <span>Recent Alerts</span>
          </span>
          <button @click="toggleAlerts"
            class="text-sm font-semibold px-6 py-3 rounded-xl bg-gradient-to-r from-[#7091E6] to-[#5571c6] text-white hover:from-[#3D52A0] hover:to-[#2a3a70] transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105">
            {{ showAlerts ? 'Hide' : 'Show' }}
          </button>
        </h3>
        <div v-if="showAlerts" class="relative max-h-64 overflow-y-auto custom-scrollbar">
          <RecentAlerts :alerts="alerts" :crash-events="crashEvents" />
        </div>
      </div>

      <!-- Recent Trips Preview -->
      <section class="mb-6">
        <div class="relative overflow-hidden bg-white/80 backdrop-blur-lg rounded-3xl shadow-2xl p-8 border border-white/50 transition-all duration-300 hover:shadow-3xl">
          <div class="absolute top-0 left-0 w-64 h-64 bg-gradient-to-br from-[#3D52A0]/10 to-[#7091E6]/10 rounded-full -ml-32 -mt-32"></div>
          <h3 class="relative text-2xl font-bold text-[#3D52A0] mb-6 flex items-center gap-3">
            <div class="bg-gradient-to-br from-[#3D52A0] to-[#2a3a70] p-3 rounded-2xl shadow-lg">
              <span class="material-icons text-3xl text-white">route</span>
            </div>
            <span>Recent Trips</span>
          </h3>
          <div v-if="recentTrips.length > 0"
            class="relative space-y-4 max-h-64 overflow-y-auto custom-scrollbar">
            <div v-for="trip in recentTrips" :key="trip.id" 
              class="group relative overflow-hidden p-6 border-2 border-white/50 rounded-2xl hover:border-[#7091E6] hover:shadow-xl transition-all duration-300 bg-gradient-to-br from-white to-[#EDE8F5]/30 backdrop-blur-sm">
              <div class="absolute top-0 right-0 w-32 h-32 bg-gradient-to-br from-[#7091E6]/10 to-transparent rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
              <div class="relative">
                <div class="flex items-center gap-2 mb-3">
                  <div class="bg-gradient-to-r from-[#7091E6] to-[#5571c6] px-3 py-1 rounded-lg">
                    <p class="text-xs text-white font-bold uppercase tracking-wider">Route Details</p>
                  </div>
                </div>
                <div class="space-y-2 mb-4">
                  <p class="text-sm flex items-start gap-2">
                    <span class="material-icons text-[#3D52A0] text-lg">trip_origin</span>
                    <span><strong class="text-[#3D52A0]">From:</strong> <span class="text-gray-700">{{ trip.startLocationName || formatLatLng(trip.startLat, trip.startLng) }}</span></span>
                  </p>
                  <p class="text-sm flex items-start gap-2">
                    <span class="material-icons text-[#3D52A0] text-lg">location_on</span>
                    <span><strong class="text-[#3D52A0]">To:</strong> <span class="text-gray-700">{{ trip.endLocationName || formatLatLng(trip.endLat, trip.endLng) }}</span></span>
                  </p>
                  <p class="text-sm flex items-center gap-2">
                    <span class="material-icons text-[#3D52A0] text-lg">speed</span>
                    <span><strong class="text-[#3D52A0]">Max Speed:</strong> <span class="text-gray-700 font-semibold">{{ trip.maxSpeed || 'N/A' }} km/h</span></span>
                  </p>
                </div>
                <div class="flex gap-3">
                  <a :href="getGoogleMapsLink(trip)" target="_blank" rel="noopener noreferrer"
                    class="flex-1 inline-flex items-center justify-center gap-2 px-4 py-3 bg-gradient-to-r from-[#7091E6] to-[#5571c6] text-white text-sm font-semibold rounded-xl hover:from-[#3D52A0] hover:to-[#2a3a70] transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105">
                    <span class="material-icons text-lg">navigation</span>
                    Navigate
                  </a>
                  <button @click="deleteTrip(trip.id)"
                    class="inline-flex items-center justify-center gap-2 px-4 py-3 bg-gradient-to-r from-red-500 to-red-600 text-white text-sm font-semibold rounded-xl hover:from-red-600 hover:to-red-700 transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105">
                    <span class="material-icons text-lg">delete</span>
                  </button>
                </div>
              </div>
            </div>
          </div>
          <div v-else class="relative p-12 text-center">
            <div class="inline-block p-6 bg-gradient-to-br from-gray-100 to-gray-200 rounded-full mb-4">
              <span class="material-icons text-7xl text-gray-400">explore_off</span>
            </div>
            <p class="text-gray-600 font-semibold text-lg">No trips found yet</p>
            <p class="text-sm text-gray-400 mt-2">Your trip history will appear here</p>
          </div>
        </div>
      </section>

      <!-- Crash Events -->
      <section class="mt-8" v-if="crashEvents.length > 0">
        <div class="relative overflow-hidden bg-gradient-to-br from-red-50 to-red-100/50 backdrop-blur-lg rounded-3xl shadow-2xl p-8 border-l-8 border-red-500">
          <div class="absolute top-0 right-0 w-96 h-96 bg-red-500/10 rounded-full -mr-48 -mt-48"></div>
          <h3 class="relative font-bold text-2xl mb-6 text-red-600 flex items-center gap-3">
            <div class="bg-gradient-to-br from-red-500 to-red-600 p-3 rounded-2xl shadow-lg animate-pulse">
              <span class="material-icons text-3xl text-white">warning</span>
            </div>
            <span>Crash Site Locations</span>
          </h3>
          <div class="relative max-h-64 overflow-y-auto space-y-4 custom-scrollbar">
            <div v-for="(event, index) in crashEvents" :key="index" 
              class="group relative overflow-hidden p-6 border-2 border-red-300 rounded-2xl bg-white hover:shadow-xl transition-all duration-300">
              <div class="absolute top-0 right-0 w-32 h-32 bg-red-500/10 rounded-full -mr-16 -mt-16 group-hover:scale-150 transition-transform duration-500"></div>
              <div class="relative">
                <div class="space-y-2 mb-4">
                  <p class="text-sm flex items-center gap-2">
                    <span class="material-icons text-red-600 text-lg">flash_on</span>
                    <span><strong class="text-red-700">Impact:</strong> <span class="text-gray-700 font-semibold">{{ event.impactStrength }} g</span></span>
                  </p>
                  <p class="text-sm flex items-start gap-2">
                    <span class="material-icons text-red-600 text-lg">location_on</span>
                    <span><strong class="text-red-700">Location:</strong> <span class="text-gray-700">{{ event.location }}</span></span>
                  </p>
                </div>
                <div class="flex flex-wrap gap-3">
                  <a :href="getGoogleMapsLink(event.lat, event.lng)" target="_blank" rel="noopener noreferrer"
                    class="inline-flex items-center gap-2 px-5 py-3 bg-gradient-to-r from-green-600 to-green-700 text-white text-sm font-semibold rounded-xl hover:from-green-700 hover:to-green-800 transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105">
                    <span class="material-icons text-lg">location_on</span>
                    See Location
                  </a>
                  <button @click="deleteCrashEvent(index)"
                    class="inline-flex items-center gap-2 px-5 py-3 bg-gradient-to-r from-red-600 to-red-700 text-white text-sm font-semibold rounded-xl hover:from-red-700 hover:to-red-800 transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105">
                    <span class="material-icons text-lg">delete</span>
                    Delete
                  </button>
                  <button v-if="isCrashActive(index)" @click="clearCrashAlert(index)"
                    class="inline-flex items-center gap-2 px-5 py-3 bg-gradient-to-r from-gray-600 to-gray-700 text-white text-sm font-semibold rounded-xl hover:from-gray-700 hover:to-gray-800 transition-all duration-300 shadow-lg hover:shadow-xl transform hover:scale-105">
                    <span class="material-icons text-lg">clear</span>
                    Clear Alert
                  </button>
                </div>
              </div>
            </div>
          </div>
        </div>
      </section>
    </main>

    <!-- Footer -->
    <footer class="relative overflow-hidden bg-gradient-to-r from-[#3D52A0] via-[#4a5fb8] to-[#3D52A0] text-center py-6 text-white border-t-4 border-white/20 mt-auto backdrop-blur-sm">
      <div class="absolute inset-0 bg-gradient-to-t from-black/10 to-transparent"></div>
      <div class="relative">
        <p class="font-semibold text-sm tracking-wide">© 2025 VIGILERT. All rights reserved.</p>
        <p class="text-xs text-white/70 mt-1">Smart Helmet Safety System</p>
      </div>
    </footer>
  </div>
</template>

<script setup>
import { ref, computed, onMounted, onBeforeUnmount, watch, nextTick } from 'vue';
import { useAuthStore } from '../stores/auth';
import { database } from '../firebase/config';
import { ref as dbRef, set, onValue, onChildAdded, remove } from 'firebase/database';

// Components
import TabGroup from '../components/TabGroup.vue';
import DashboardCard from '../components/DashboardCard.vue';
import LocationSection from '../components/LocationSection.vue';
import SpeedDataSection from '../components/SpeedDataSection.vue';
import DiagnosticsSection from '../components/DiagnosticsSection.vue';
import RecentAlerts from '../components/RecentAlerts.vue';
import SensorHealthPanel from '../components/SensorHealthPanel.vue';
import TripStatistics from '../components/TripStatistics.vue';

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
const diagnostics = ref([]);
const alerts = ref([]);
const activeTab = ref('My Location');
const location = ref({ lat: null, lng: null });
const user = ref({ name: 'Loading...' });
const recentTrips = ref([]);
const crashEvents = ref([]);
const isOverSpeed = ref(false);
const showAlerts = ref(true);

// New states for enhanced features
const helmetPaired = ref(false);
const motorcyclePaired = ref(false);
const deviceBattery = ref(85);
const gsmConnected = ref(true);
const gpsConnected = ref(true);
const electricalDiagnostics = ref({
  headlight: true,
  taillight: true,
  brakeLight: true,
  signalLights: true
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

const userId = 'MnzBjTBslZNijOkq732PE91hHa23'; // Firebase UID

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
  heartRate: {
    bpm: 0,
    lastUpdate: Date.now()
  },
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

// Play alert sound
const playSound = () => {
  try {
    const audio = new Audio('/sounds/alert.mp3');
    audio.play().catch(err => {
      console.warn("Audio playback failed:", err);
    });
  } catch (err) {
    console.error("Error creating audio object:", err);
  }
};

// Flash Crash Message
const flashCrashMessage = () => {
  clearInterval(crashInterval);
  flashCount = 0;
  crashDisplayStatus.value = 'Alerting';
  crashDisplayMessage.value = 'Crash Detected';
  playSound();
  alerts.value.unshift({
    type: 'danger',
    message: 'Crash Detected',
    details: 'Impact Strength: High',
    time: new Date().toLocaleTimeString()
  });
  if (alerts.value.length > 5) alerts.value.pop();
  crashInterval = setInterval(() => {
    if (flashCount >= 3) {
      clearInterval(crashInterval);
      crashDisplayStatus.value = 'Stable';
      crashDisplayMessage.value = 'Vehicle Stable';
    }
    crashDisplayMessage.value = crashDisplayMessage.value === 'Crash Detected' ? 'Vehicle Stable' : 'Crash Detected';
    flashCount++;
  }, 2000);
};

// Handle Overspeed Event
const handleOverspeed = (payload) => {
  playSound(); // Triggers alert sound
  alerts.value.unshift({
    type: 'danger',
    message: 'Speed Limit Exceeded!',
    details: `Current: ${payload.speed} kph | Limit: ${payload.limit} kph`,
    time: new Date().toLocaleTimeString()
  });
  if (alerts.value.length > 5) alerts.value.pop();
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

// Firebase References
const helmetPublicRef = dbRef(database, `helmet_public/${userId}`);
const helmetRef = dbRef(database, `helmet_public/${userId}/helmetStatus/status`);
const tripsRef = dbRef(database, `helmet_public/${userId}/trips`);
const crashRef = dbRef(database, `helmet_public/${userId}/crashes`);
const alcoholRef = dbRef(database, `helmet_public/${userId}/alcohol/status`);
const speedLimitRef = dbRef(database, `helmet_public/${userId}/settings/speedLimit`);

onMounted(() => {
  const storedLastCrashTime = localStorage.getItem(`lastCrashTimestamp_${userId}`);
  if (storedLastCrashTime) lastCrashTimestamp = parseInt(storedLastCrashTime);

  onValue(speedLimitRef, (snapshot) => {
    const data = snapshot.val();
    if (data !== null) speedLimit.value = data;
  });

  // Listen for helmet pairing status with heartbeat check
  const helmetStatusRef = dbRef(database, `helmet_public/${userId}/devices/helmet`);
  onValue(helmetStatusRef, (snapshot) => {
    const data = snapshot.val();
    console.log('[DEBUG] Helmet device data from Firebase:', data);
    
    if (data && data.status === 'On' && data.lastHeartbeat) {
      // Check if heartbeat is recent (within last 10 seconds)
      const now = Date.now();
      const lastBeat = data.lastHeartbeat;
      const timeSinceLastBeat = now - lastBeat;
      
      // Consider connected if heartbeat within 10 seconds
      helmetPaired.value = timeSinceLastBeat < 10000;
      console.log(`[DEBUG] Helmet: ${helmetPaired.value ? 'CONNECTED' : 'DISCONNECTED'} (last beat: ${timeSinceLastBeat}ms ago)`);
    } else {
      helmetPaired.value = false;
      console.log('[DEBUG] Helmet: DISCONNECTED (no data or status Off)');
    }
  });

  // Listen for motorcycle pairing status with heartbeat check
  const motorcycleStatusRef = dbRef(database, `helmet_public/${userId}/devices/motorcycle`);
  onValue(motorcycleStatusRef, (snapshot) => {
    const data = snapshot.val();
    console.log('[DEBUG] Motorcycle device data from Firebase:', data);
    
    if (data && data.status === 'On' && data.lastHeartbeat) {
      // Check if heartbeat is recent (within last 10 seconds)
      const now = Date.now();
      const lastBeat = data.lastHeartbeat;
      const timeSinceLastBeat = now - lastBeat;
      
      // Consider connected if heartbeat within 10 seconds
      motorcyclePaired.value = timeSinceLastBeat < 10000;
      console.log(`[DEBUG] Motorcycle: ${motorcyclePaired.value ? 'CONNECTED' : 'DISCONNECTED'} (last beat: ${timeSinceLastBeat}ms ago)`);
    } else {
      motorcyclePaired.value = false;
      console.log('[DEBUG] Motorcycle: DISCONNECTED (no data or status Off)');
    }
  });

  // Listen for device health
  const deviceHealthRef = dbRef(database, `helmet_public/${userId}/deviceHealth`);
  onValue(deviceHealthRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      deviceBattery.value = data.battery || 85;
      gsmConnected.value = data.gsm !== false;
      gpsConnected.value = data.gps !== false;
    }
  });

  // Listen for electrical diagnostics
  const electricalRef = dbRef(database, `helmet_public/${userId}/electrical`);
  onValue(electricalRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      electricalDiagnostics.value = {
        headlight: data.headlight !== false,
        taillight: data.taillight !== false,
        brakeLight: data.brakeLight !== false,
        signalLights: data.signalLights !== false
      };
      
      // Check for any faults and alert
      const faults = [];
      if (!data.headlight) faults.push('Headlight');
      if (!data.taillight) faults.push('Taillight');
      if (!data.brakeLight) faults.push('Brake Light');
      if (!data.signalLights) faults.push('Signal Lights');
      
      if (faults.length > 0) {
        alerts.value.unshift({
          type: 'warning',
          message: 'Electrical Fault Detected',
          details: `Faulty components: ${faults.join(', ')}`,
          time: new Date().toLocaleTimeString()
        });
        if (alerts.value.length > 5) alerts.value.pop();
      }
    }
  });

  initializeCrashListener();

  // ✅ ENHANCED: Real-time Alcohol Detection Listener with Force Update
  onValue(alcoholRef, (snapshot) => {
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
      
      // ✅ Update alcohol status with force reactivity using nextTick
      if (status === "Danger") {
        console.log('🚨🚨🚨 ALCOHOL DANGER DETECTED! 🚨🚨🚨');
        console.log(`   Value ${sensorValue} exceeds threshold ${threshold}`);
        
        // Force update refs with immediate assignment
        alcoholStatus.value = 'Danger';
        alcoholSubtitle.value = `Alcohol Detected! Value: ${sensorValue}`;
        
        console.log(`[UPDATE] alcoholStatus AFTER update: ${alcoholStatus.value}`);
        console.log(`[UPDATE] alcoholSubtitle AFTER update: ${alcoholSubtitle.value}`);
        
        // Verify the update stuck
        setTimeout(() => {
          console.log(`[VERIFY] alcoholStatus 100ms later: ${alcoholStatus.value}`);
          console.log(`[VERIFY] alcoholSubtitle 100ms later: ${alcoholSubtitle.value}`);
        }, 100);
        
        // Add alert
        alerts.value.unshift({
          type: 'danger',
          message: '🚨 Alcohol Detected!',
          details: `Sensor Value: ${sensorValue} (Threshold: ${threshold})`,
          time: timestamp
        });
        playSound();
        if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
        
        console.log('[ALERT] Alert added to dashboard');
      } else {
        console.log('✓ Alcohol status: SAFE');
        alcoholStatus.value = 'Safe';
        alcoholSubtitle.value = `No alcohol detected (Value: ${sensorValue})`;
        
        console.log(`[UPDATE] alcoholStatus AFTER update: ${alcoholStatus.value}`);
        console.log(`[UPDATE] alcoholSubtitle AFTER update: ${alcoholSubtitle.value}`);
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

  onValue(helmetPublicRef, (snapshot) => {
    const data = snapshot.val();
    if (data?.live) {
      const liveData = data.live;
      location.value = {
        lat: Number(liveData.locationLat),
        lng: Number(liveData.locationLng)
      };
      const rawSpeed = parseFloat(liveData.speed) || 0;
      currentSpeed.value = rawSpeed < 0.1 ? 0 : rawSpeed;
      speedHistory.value.push(currentSpeed.value);
      if (speedHistory.value.length > 10) speedHistory.value.shift();
      isOverSpeed.value = currentSpeed.value > speedLimit.value;
      
      // Update trip statistics
      updateTripStatistics(currentSpeed.value);
      
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
      
      // ✅ Update heart rate data
      if (liveData.heartRate) {
        sensorData.value.heartRate = {
          bpm: liveData.heartRate.bpm || 0,
          lastUpdate: Date.now()
        };
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
  onValue(helmetRef, (snapshot) => {
    const data = snapshot.val();
    console.log('[DEBUG] Helmet status data from Firebase:', data);
    
    if (data) {
      const isConnected = data.helmetConnected === true || data.helmetConnected === 'true';
      
      // ✅ Detect helmet removal and trigger warning
      if (previousHelmetState.value === true && isConnected === false) {
        // Helmet was just removed!
        console.log('[WARNING] Helmet REMOVED - Triggering alert!');
        
        alerts.value.unshift({
          type: 'danger',
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
      
      riderStatus.value = isConnected ? 'Active' : 'Inactive';
      riderSubtitle.value = isConnected ? 'Helmet connected' : 'Helmet not connected';
      
      console.log(`[DEBUG] Helmet Connected: ${isConnected}, Rider Status: ${riderStatus.value}`);
      
      // ✅ Update alertness status
      alertnessStatus.value = data.alertnessStatus || 'Normal';
      alertnessSubtitle.value = alertnessStatus.value === 'Normal' ? 'No drowsiness detected' : 'Drowsiness detected!';
      
      // ⚠️ REMOVED: Alcohol status is now handled by dedicated alcoholRef listener
      // This prevents conflicts between two listeners updating the same data
      
      // ✅ Alert for drowsiness only (alcohol alerts handled by alcoholRef listener)
      if (alertnessStatus.value !== 'Normal') {
        alerts.value.unshift({
          type: 'danger',
          message: 'Drowsiness Detected',
          details: alertnessSubtitle.value,
          time: new Date().toLocaleTimeString()
        });
        playSound();
        if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
      }
    } else {
      console.log('[DEBUG] No helmet status data received');
    }
  });

  onValue(tripsRef, async (snapshot) => {
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
});

// Show/Hide Alerts Panel
const toggleAlerts = () => {
  showAlerts.value = !showAlerts.value;
};

// Update speed limit in Firebase
const updateSpeedLimitInFirebase = () => {
  set(dbRef(database, `helmet_public/${userId}/settings/speedLimit`), speedLimit.value).catch(error => {
    console.error("Failed to update speed limit:", error);
  });
};

// Delete a trip by ID
const deleteTrip = async (tripId) => {
  const confirmDelete = confirm("Are you sure you want to delete this trip?");
  if (!confirmDelete) return;
  const tripRef = dbRef(database, `helmet_public/${userId}/trips/${tripId}`);
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
  if (event && event.timestamp > lastCrashTimestamp) {
    lastCrashTimestamp = event.timestamp;
    localStorage.setItem(`lastCrashTimestamp_${userId}`, event.timestamp.toString());
    crashDisplayStatus.value = 'Stable';
    crashDisplayMessage.value = 'Vehicle Stable';
  }
};

// Delete Crash Event
const deleteCrashEvent = (index) => {
  crashEvents.value.splice(index, 1);
};

// Start continuous GPS speed monitoring
const startGPSSpeedMonitoring = () => {
  if (!navigator.geolocation) {
    alert('Geolocation is not supported by your browser');
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
      const speed = position.coords.speed; // Speed in meters per second
      
      // Update location
      location.value = { lat, lng };
      
      // Convert speed from m/s to km/h
      if (speed !== null && speed !== undefined) {
        const speedKmh = speed * 3.6; // Convert m/s to km/h
        const finalSpeed = speedKmh < 0.5 ? 0 : speedKmh; // Filter out very small values
        
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
      }
      
      // Update Firebase with GPS phone data
      const locationRef = dbRef(database, `helmet_public/${userId}/live`);
      set(locationRef, {
        locationLat: lat,
        locationLng: lng,
        speed: currentSpeed.value,
        timestamp: Date.now(),
        source: 'phone_gps'
      }).catch(error => {
        console.error('Failed to update location:', error);
      });
    },
    (error) => {
      console.error('GPS monitoring error:', error);
      stopGPSSpeedMonitoring();
      
      let errorMessage = 'GPS monitoring failed. ';
      switch(error.code) {
        case error.PERMISSION_DENIED:
          errorMessage += 'Please allow location access.';
          break;
        case error.POSITION_UNAVAILABLE:
          errorMessage += 'Location unavailable.';
          break;
        case error.TIMEOUT:
          errorMessage += 'Request timed out.';
          break;
      }
      
      alerts.value.unshift({
        type: 'warning',
        message: 'GPS Monitoring Stopped',
        details: errorMessage,
        time: new Date().toLocaleTimeString()
      });
      if (alerts.value.length > 5) alerts.value.pop();
    },
    {
      enableHighAccuracy: true,
      timeout: 10000,
      maximumAge: 0
    }
  );

  alerts.value.unshift({
    type: 'success',
    message: 'GPS Speed Monitoring Active',
    details: 'Real-time speed tracking from phone GPS',
    time: new Date().toLocaleTimeString()
  });
  if (alerts.value.length > 5) alerts.value.pop();
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

// Handle location update from GPS (one-time update)
const handleLocationUpdate = async (newLocation) => {
  location.value = newLocation;
  
  // Update location in Firebase
  try {
    const locationRef = dbRef(database, `helmet_public/${userId}/live`);
    await set(locationRef, {
      locationLat: newLocation.lat,
      locationLng: newLocation.lng,
      speed: currentSpeed.value,
      timestamp: Date.now(),
      source: 'phone_gps_manual'
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

// Trigger SOS Alert
const triggerSOS = async () => {
  const confirmed = confirm('⚠️ EMERGENCY SOS\n\nThis will send an emergency alert to all your emergency contacts with your current location.\n\nAre you sure you want to trigger SOS?');
  
  if (!confirmed) return;
  
  try {
    // Play alert sound
    playSound();
    
    // Add to alerts
    alerts.value.unshift({
      type: 'danger',
      message: '🆘 EMERGENCY SOS TRIGGERED',
      details: `Location: ${location.value.lat ? `${location.value.lat.toFixed(6)}, ${location.value.lng.toFixed(6)}` : 'Unknown'}`,
      time: new Date().toLocaleTimeString()
    });
    
    // Log SOS event to Firebase
    const sosRef = dbRef(database, `helmet_public/${userId}/sos/${Date.now()}`);
    await set(sosRef, {
      timestamp: Date.now(),
      location: {
        lat: location.value.lat,
        lng: location.value.lng
      },
      triggeredBy: 'manual',
      status: 'active'
    });
    
    alert('✅ SOS Alert Sent!\n\nEmergency contacts have been notified with your location.');
  } catch (error) {
    console.error('Failed to trigger SOS:', error);
    alert('❌ Failed to send SOS alert. Please try again or call emergency services directly.');
  }
};

// Initialize crash event listener
const initializeCrashListener = () => {
  console.log('[INIT] Setting up crash listener on path:', `/helmet_public/${userId}/crashes`);
  
  onChildAdded(crashRef, (snapshot) => {
    const event = snapshot.val();
    console.log('[CRASH] New crash event received from Firebase:', event);
    
    // More lenient validation - just check if event exists and has timestamp
    if (!event || !event.timestamp) {
      console.warn("[CRASH] Invalid crash event (missing timestamp)", event);
      return;
    }
    
    console.log('[CRASH] ✓ Valid crash event:', {
      timestamp: event.timestamp,
      impact: event.impactStrength,
      roll: event.roll,
      hasGPS: event.hasGPS,
      location: event.hasGPS ? `${event.lat}, ${event.lng}` : 'No GPS'
    });
    
    const eventTime = event.timestamp;
    
    // Add to crash events array (for map display)
    const crashEvent = {
      timestamp: eventTime,
      impactStrength: event.impactStrength || "N/A",
      roll: event.roll || event.leanAngle || 0,
      location: event.hasGPS ? `${event.lat}, ${event.lng}` : 'No GPS',
      lat: event.lat,
      lng: event.lng,
      hasGPS: event.hasGPS || false
    };
    
    crashEvents.value.push(crashEvent);
    console.log('[CRASH] Crash added to array. Total crashes:', crashEvents.value.length);
    
    // Add alert notification
    alerts.value.unshift({
      type: 'danger',
      message: '🚨 CRASH DETECTED!',
      details: `Impact: ${event.impactStrength || 'N/A'} g | Location: ${crashEvent.location}`,
      time: new Date().toLocaleTimeString()
    });
    if (alerts.value.length > 10) alerts.value = alerts.value.slice(0, 10);
    
    // Flash crash message for recent crashes
    if (!lastCrashTimestamp || eventTime > lastCrashTimestamp) {
      lastCrashTimestamp = eventTime;
      localStorage.setItem(`lastCrashTimestamp_${userId}`, eventTime.toString());
      console.log('[CRASH] Triggering crash alert animation...');
      flashCrashMessage();
      playSound();
    }
  }, (error) => {
    console.error("[CRASH] Firebase crash listener error:", error);
  });
};

// Periodic heartbeat check (every 5 seconds)
let heartbeatCheckInterval = null;

const checkDeviceHeartbeats = () => {
  // Re-check helmet
  const helmetRef = dbRef(database, `helmet_public/${userId}/devices/helmet`);
  onValue(helmetRef, (snapshot) => {
    const data = snapshot.val();
    if (data && data.status === 'On' && data.lastHeartbeat) {
      const timeSinceLastBeat = Date.now() - data.lastHeartbeat;
      helmetPaired.value = timeSinceLastBeat < 10000;
    } else {
      helmetPaired.value = false;
    }
  }, { onlyOnce: true });
  
  // Re-check motorcycle
  const motorcycleRef = dbRef(database, `helmet_public/${userId}/devices/motorcycle`);
  onValue(motorcycleRef, (snapshot) => {
    const data = snapshot.val();
    if (data && data.status === 'On' && data.lastHeartbeat) {
      const timeSinceLastBeat = Date.now() - data.lastHeartbeat;
      motorcyclePaired.value = timeSinceLastBeat < 10000;
    } else {
      motorcyclePaired.value = false;
    }
  }, { onlyOnce: true });
};

onMounted(() => {
  // ... existing onMounted code ...
  
  // Start periodic heartbeat check
  heartbeatCheckInterval = setInterval(checkDeviceHeartbeats, 5000);
});

// Cleanup on component unmount
onBeforeUnmount(() => {
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