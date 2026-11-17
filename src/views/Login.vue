<template>
  <div class="min-h-screen relative flex flex-col justify-between bg-[#061B2B] text-white overflow-x-hidden select-none font-['Inter',sans-serif]">
    
    <!-- Background Futuristic Effects -->
    <div class="fixed inset-0 pointer-events-none z-0 overflow-hidden">
      <!-- Dark Blue Radial Center Gradient -->
      <div class="absolute inset-0 bg-[radial-gradient(circle_at_30%_35%,#0B2540_0%,#061B2B_75%)]"></div>
      
      <!-- Faint Cyber Grid Pattern -->
      <div class="absolute inset-0 opacity-[0.04]"
        style="background-image: linear-gradient(rgba(112,227,74,0.3) 1px, transparent 1px), linear-gradient(90deg, rgba(112,227,74,0.3) 1px, transparent 1px); background-size: 48px 48px;">
      </div>

      <!-- Top-Left Neon Cyber Stripes Accent -->
      <svg class="absolute -top-6 -left-6 w-56 h-56 sm:w-72 sm:h-72 opacity-90" viewBox="0 0 200 200" fill="none">
        <path d="M-20 80 L80 -20 L110 -20 L-20 110 Z" fill="url(#cyber-grad-1)"/>
        <path d="M-20 130 L130 -20 L150 -20 L-20 150 Z" fill="url(#cyber-grad-2)"/>
        <path d="M-20 170 L170 -20 L185 -20 L-20 185 Z" fill="#00B85C" fill-opacity="0.6"/>
        <defs>
          <linearGradient id="cyber-grad-1" x1="0" y1="0" x2="1" y2="1">
            <stop offset="0%" stop-color="#70E34A"/>
            <stop offset="100%" stop-color="#00B85C"/>
          </linearGradient>
          <linearGradient id="cyber-grad-2" x1="0" y1="0" x2="1" y2="1">
            <stop offset="0%" stop-color="#00B85C"/>
            <stop offset="100%" stop-color="#061B2B"/>
          </linearGradient>
        </defs>
      </svg>

      <!-- Bottom-Left Neon Cyber Stripes Accent -->
      <svg class="absolute -bottom-10 -left-10 w-64 h-64 sm:w-80 sm:h-80 opacity-90" viewBox="0 0 220 220" fill="none">
        <path d="M-20 100 L100 220 L125 220 L-20 75 Z" fill="url(#cyber-grad-3)"/>
        <path d="M-20 50 L150 220 L170 220 L-20 30 Z" fill="#00B85C" fill-opacity="0.5"/>
        <defs>
          <linearGradient id="cyber-grad-3" x1="0" y1="1" x2="1" y2="0">
            <stop offset="0%" stop-color="#70E34A"/>
            <stop offset="100%" stop-color="#00B85C"/>
          </linearGradient>
        </defs>
      </svg>

      <!-- Top-Right Ambient Glow -->
      <div class="absolute -top-24 -right-24 w-96 h-96 bg-[#00B85C]/10 rounded-full blur-3xl"></div>
      
      <!-- Bottom-Right Cyber Corner Accent -->
      <svg class="absolute -bottom-8 -right-8 w-48 h-48 opacity-75" viewBox="0 0 160 160" fill="none">
        <path d="M180 60 L60 180 L80 180 L180 80 Z" fill="#00B85C" fill-opacity="0.4"/>
        <path d="M180 100 L100 180 L120 180 L180 120 Z" fill="#70E34A" fill-opacity="0.3"/>
      </svg>
    </div>

    <!-- Main Content Container -->
    <div class="relative z-10 w-full max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-6 sm:py-8 lg:py-10 flex-1 flex items-center">
      <div class="w-full grid grid-cols-1 lg:grid-cols-12 gap-8 lg:gap-8 xl:gap-12 items-center">

        <!-- ========================================== -->
        <!-- LEFT PANEL: BRANDING & SYSTEM OVERVIEW     -->
        <!-- ========================================== -->
        <div class="lg:col-span-7 xl:col-span-7 flex flex-col justify-center space-y-6 lg:space-y-7">
          
          <!-- Branding Header -->
          <div class="flex items-center gap-4 sm:gap-5">
            <!-- Shield Helmet Logo -->
            <div class="relative flex-shrink-0 w-16 h-16 sm:w-20 sm:h-20 flex items-center justify-center">
              <img 
                src="/images/shield-logo.png" 
                alt="VigiLERT Shield Logo" 
                class="w-full h-full object-contain drop-shadow-[0_0_15px_rgba(0,184,92,0.4)]"
                @error="logoFallback = true"
                v-if="!logoFallback"
              />
              <!-- Vector Fallback if image not ready -->
              <div v-else class="w-16 h-16 rounded-2xl bg-[#0B2540] border-2 border-[#00B85C] flex items-center justify-center shadow-[0_0_20px_rgba(0,184,92,0.4)]">
                <svg class="w-9 h-9 text-[#00B85C]" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12l2 2 4-4m5.618-4.016A11.955 11.955 0 0112 2.944a11.955 11.955 0 01-8.618 3.04A12.02 12.02 0 003 9c0 5.591 3.824 10.29 9 11.622 5.176-1.332 9-6.03 9-11.622 0-1.042-.133-2.052-.382-3.016z" />
                </svg>
              </div>
            </div>

            <!-- Title & Tagline -->
            <div class="flex flex-col">
              <div class="flex items-center flex-wrap gap-1 sm:gap-2">
                <h1 class="text-3xl sm:text-4xl lg:text-5xl font-extrabold tracking-tight text-white">
                  Vigi<span class="text-[#00B85C]">LERT</span>
                </h1>
                <!-- Emerald ECG Heartbeat Wave -->
                <svg class="h-6 sm:h-8 w-16 sm:w-24 text-[#00B85C] inline-block filter drop-shadow-[0_0_8px_rgba(0,184,92,0.6)]" viewBox="0 0 100 36" fill="none">
                  <path d="M0 18 H28 L36 4 L44 32 L52 10 L60 24 L68 18 H100" stroke="currentColor" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"/>
                </svg>
              </div>
              <p class="text-base sm:text-lg font-medium italic text-white/95 tracking-wide mt-0.5">
                Ride Smart. Stay Safe.
              </p>
            </div>
          </div>

          <!-- System Description -->
          <p class="text-sm sm:text-base text-slate-300 leading-relaxed max-w-xl font-normal">
            A comprehensive, IoT-powered motorcycle safety, emergency response, and anti-theft ecosystem designed to protect riders and their vehicles through real-time monitoring and connected technology.
          </p>

          <!-- Middle Section: Motorcycle Rider Visual + 6 Feature Cards -->
          <div class="flex flex-col sm:flex-row items-center gap-5 lg:gap-6 pt-1 pb-2">
            
            <!-- Motorcycle Visual with Cyber HUD Rings -->
            <div class="relative flex-shrink-0 w-44 sm:w-48 lg:w-52 aspect-[4/5] flex items-center justify-center">
              <!-- Concentric Animated HUD Tracking Reticle -->
              <div class="absolute inset-1 rounded-full border border-emerald-400/30 animate-[spin_25s_linear_infinite] pointer-events-none"></div>
              <div class="absolute -inset-1 rounded-full border border-dashed border-[#70E34A]/25 pointer-events-none"></div>
              <div class="absolute inset-6 rounded-full border border-cyan-400/20 pointer-events-none"></div>
              
              <!-- Rider Image -->
              <img 
                src="/images/rider.png" 
                alt="VigiLERT Smart Rider" 
                class="relative z-10 w-full h-full object-contain rounded-2xl drop-shadow-[0_12px_24px_rgba(0,0,0,0.6)] filter contrast-105"
                @error="riderFallback = true"
                v-if="!riderFallback"
              />
              <!-- Fallback Icon if image fails -->
              <div v-else class="relative z-10 w-36 h-36 rounded-full bg-[#0B2540] border border-[#00B85C]/40 flex flex-col items-center justify-center p-4 text-center">
                <svg class="w-12 h-12 text-[#00B85C] mb-1" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="1.5" d="M13 10V3L4 14h7v7l9-11h-7z" />
                </svg>
                <span class="text-xs text-white/80 font-semibold">Active Protection</span>
              </div>
            </div>

            <!-- 6 Visual Feature Highlight Cards (3x2 Grid) -->
            <div class="grid grid-cols-2 sm:grid-cols-3 gap-2.5 sm:gap-3 flex-1 w-full">
              
              <!-- Card 1: Crash Detection -->
              <div class="group relative flex flex-col items-center justify-center p-3 rounded-2xl bg-[#0B2540]/65 hover:bg-[#0B2540]/90 border border-[#00B85C]/25 hover:border-[#70E34A] backdrop-blur-md transition-all duration-300 hover:scale-105 hover:shadow-[0_0_20px_rgba(0,184,92,0.3)] cursor-pointer text-center min-h-[95px] sm:min-h-[105px]">
                <div class="w-9 h-9 rounded-xl bg-[#00B85C]/15 flex items-center justify-center mb-2 text-[#00B85C] group-hover:text-[#70E34A] group-hover:scale-110 transition-all">
                  <!-- Impact / Helmet Icon -->
                  <svg class="w-5 h-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M2 12a10 10 0 1 1 20 0 8 8 0 0 1-8 8H7a5 5 0 0 1-5-5v-3z"/>
                    <path d="M9 13l2-2 4 4"/>
                  </svg>
                </div>
                <span class="text-xs font-semibold text-white/90 group-hover:text-white leading-tight">Crash<br class="hidden sm:inline"/> Detection</span>
              </div>

              <!-- Card 2: Alcohol Monitoring -->
              <div class="group relative flex flex-col items-center justify-center p-3 rounded-2xl bg-[#0B2540]/65 hover:bg-[#0B2540]/90 border border-[#00B85C]/25 hover:border-[#70E34A] backdrop-blur-md transition-all duration-300 hover:scale-105 hover:shadow-[0_0_20px_rgba(0,184,92,0.3)] cursor-pointer text-center min-h-[95px] sm:min-h-[105px]">
                <div class="w-9 h-9 rounded-xl bg-[#00B85C]/15 flex items-center justify-center mb-2 text-[#00B85C] group-hover:text-[#70E34A] group-hover:scale-110 transition-all">
                  <!-- Flask / Beaker Icon -->
                  <svg class="w-5 h-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M10 2v7.527a2 2 0 0 1-.211.896L4.72 20.55a1 1 0 0 0 .9 1.45h12.76a1 1 0 0 0 .9-1.45l-5.069-10.127A2 2 0 0 1 14 9.527V2"/>
                    <path d="M8.5 2h7"/>
                    <path d="M7 16h10"/>
                  </svg>
                </div>
                <span class="text-xs font-semibold text-white/90 group-hover:text-white leading-tight">Alcohol<br class="hidden sm:inline"/> Monitoring</span>
              </div>

              <!-- Card 3: Anti-Theft Protection -->
              <div class="group relative flex flex-col items-center justify-center p-3 rounded-2xl bg-[#0B2540]/65 hover:bg-[#0B2540]/90 border border-[#00B85C]/25 hover:border-[#70E34A] backdrop-blur-md transition-all duration-300 hover:scale-105 hover:shadow-[0_0_20px_rgba(0,184,92,0.3)] cursor-pointer text-center min-h-[95px] sm:min-h-[105px]">
                <div class="w-9 h-9 rounded-xl bg-[#00B85C]/15 flex items-center justify-center mb-2 text-[#00B85C] group-hover:text-[#70E34A] group-hover:scale-110 transition-all">
                  <!-- Padlock Shield Icon -->
                  <svg class="w-5 h-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <rect x="5" y="11" width="14" height="10" rx="2"/>
                    <circle cx="12" cy="16" r="1"/>
                    <path d="M8 11V7a4 4 0 0 1 8 0v4"/>
                  </svg>
                </div>
                <span class="text-xs font-semibold text-white/90 group-hover:text-white leading-tight">Anti-Theft<br class="hidden sm:inline"/> Protection</span>
              </div>

              <!-- Card 4: Emergency Alerts -->
              <div class="group relative flex flex-col items-center justify-center p-3 rounded-2xl bg-[#0B2540]/65 hover:bg-[#0B2540]/90 border border-[#00B85C]/25 hover:border-[#70E34A] backdrop-blur-md transition-all duration-300 hover:scale-105 hover:shadow-[0_0_20px_rgba(0,184,92,0.3)] cursor-pointer text-center min-h-[95px] sm:min-h-[105px]">
                <div class="w-9 h-9 rounded-xl bg-[#00B85C]/15 flex items-center justify-center mb-2 text-[#00B85C] group-hover:text-[#70E34A] group-hover:scale-110 transition-all">
                  <!-- Siren / Bell Alert Icon -->
                  <svg class="w-5 h-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M6 8a6 6 0 0 1 12 0c0 7 3 9 3 9H3s3-2 3-9"/>
                    <path d="M10.3 21a1.94 1.94 0 0 0 3.4 0"/>
                    <path d="M4 2C2.8 3.7 2 5.7 2 8"/>
                    <path d="M22 8c0-2.3-.8-4.3-2-6"/>
                  </svg>
                </div>
                <span class="text-xs font-semibold text-white/90 group-hover:text-white leading-tight">Emergency<br class="hidden sm:inline"/> Alerts</span>
              </div>

              <!-- Card 5: Real-Time Tracking -->
              <div class="group relative flex flex-col items-center justify-center p-3 rounded-2xl bg-[#0B2540]/65 hover:bg-[#0B2540]/90 border border-[#00B85C]/25 hover:border-[#70E34A] backdrop-blur-md transition-all duration-300 hover:scale-105 hover:shadow-[0_0_20px_rgba(0,184,92,0.3)] cursor-pointer text-center min-h-[95px] sm:min-h-[105px]">
                <div class="w-9 h-9 rounded-xl bg-[#00B85C]/15 flex items-center justify-center mb-2 text-[#00B85C] group-hover:text-[#70E34A] group-hover:scale-110 transition-all">
                  <!-- Map Pin / Location Icon -->
                  <svg class="w-5 h-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M20 10c0 6-8 12-8 12s-8-6-8-12a8 8 0 0 1 16 0Z"/>
                    <circle cx="12" cy="10" r="3"/>
                  </svg>
                </div>
                <span class="text-xs font-semibold text-white/90 group-hover:text-white leading-tight">Real-Time<br class="hidden sm:inline"/> Tracking</span>
              </div>

              <!-- Card 6: Cloud Integration -->
              <div class="group relative flex flex-col items-center justify-center p-3 rounded-2xl bg-[#0B2540]/65 hover:bg-[#0B2540]/90 border border-[#00B85C]/25 hover:border-[#70E34A] backdrop-blur-md transition-all duration-300 hover:scale-105 hover:shadow-[0_0_20px_rgba(0,184,92,0.3)] cursor-pointer text-center min-h-[95px] sm:min-h-[105px]">
                <div class="w-9 h-9 rounded-xl bg-[#00B85C]/15 flex items-center justify-center mb-2 text-[#00B85C] group-hover:text-[#70E34A] group-hover:scale-110 transition-all">
                  <!-- Cloud Integration Icon -->
                  <svg class="w-5 h-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M17.5 19H9a7 7 0 1 1 6.71-9h1.79a4.5 4.5 0 1 1 0 9Z"/>
                  </svg>
                </div>
                <span class="text-xs font-semibold text-white/90 group-hover:text-white leading-tight">Cloud<br class="hidden sm:inline"/> Integration</span>
              </div>

            </div>
          </div>

          <!-- Bottom Branding Statement with Green Brush Underline -->
          <div class="pt-1">
            <p class="text-xl sm:text-2xl font-normal italic text-white tracking-wide">
              Your Safety. Our Priority.
            </p>
            <!-- Curved Emerald Swoosh Accent -->
            <svg class="w-40 sm:w-52 h-3.5 text-[#00B85C] mt-1" viewBox="0 0 200 14" fill="none">
              <path d="M2 10 C 60 14, 140 12, 198 2" stroke="currentColor" stroke-width="3.5" stroke-linecap="round"/>
            </svg>
          </div>

        </div>


        <!-- ========================================== -->
        <!-- RIGHT PANEL: AUTHENTICATION CARD           -->
        <!-- ========================================== -->
        <div class="lg:col-span-5 xl:col-span-5 flex justify-center lg:justify-end">
          <div class="w-full max-w-[430px] bg-white rounded-[32px] p-7 sm:p-9 shadow-[0_25px_60px_-15px_rgba(0,0,0,0.55)] border border-gray-100 text-[#061B2B] relative z-20">
            
            <!-- Card Header: Shield Helmet Logo -->
            <div class="flex flex-col items-center text-center">
              <div class="w-14 h-14 sm:w-16 sm:h-16 flex items-center justify-center mb-1">
                <img 
                  src="/images/shield-logo-card.png" 
                  alt="VigiLERT Shield Logo" 
                  class="w-full h-full object-contain"
                  @error="cardLogoFallback = true"
                  v-if="!cardLogoFallback"
                />
                <!-- Vector Fallback if cropped image not found -->
                <div v-else class="w-14 h-14 rounded-2xl bg-emerald-50 border-2 border-[#00B85C] flex items-center justify-center">
                  <svg class="w-8 h-8 text-[#00B85C]" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12l2 2 4-4m5.618-4.016A11.955 11.955 0 0112 2.944a11.955 11.955 0 01-8.618 3.04A12.02 12.02 0 003 9c0 5.591 3.824 10.29 9 11.622 5.176-1.332 9-6.03 9-11.622 0-1.042-.133-2.052-.382-3.016z" />
                  </svg>
                </div>
              </div>

              <h2 class="text-2xl sm:text-3xl font-extrabold text-[#061B2B] tracking-tight">
                Welcome Back
              </h2>
              <p class="text-sm text-[#64748B] mt-1 font-medium">
                Sign in to your VigiLERT account
              </p>

              <!-- Green Heartbeat Pulse Divider -->
              <div class="flex items-center justify-center w-full my-3.5">
                <svg class="w-48 h-3.5 text-[#00B85C]" viewBox="0 0 160 16" fill="none">
                  <path d="M0 8 H60 L66 1 L72 15 L78 3 L84 12 L90 8 H160" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
                </svg>
              </div>
            </div>

            <!-- Google Authentication Button -->
            <button 
              type="button" 
              @click="handleGoogleSignIn" 
              :disabled="loading"
              class="w-full flex items-center justify-center gap-3 bg-white hover:bg-gray-50/80 active:bg-gray-100 border border-gray-200/90 text-[#1E293B] font-semibold py-3 px-5 rounded-full shadow-sm hover:shadow transition-all duration-200 disabled:opacity-60 disabled:cursor-not-allowed group text-sm"
            >
              <!-- Official Multi-Color Google G -->
              <svg class="w-4 h-4 flex-shrink-0" viewBox="0 0 48 48">
                <path fill="#4285F4" d="M24 9.5c3.54 0 6.7 1.22 9.19 3.23l6.87-6.87C35.64 2.36 30.13 0 24 0 14.82 0 6.73 5.8 2.69 14.09l7.99 6.21C12.13 13.13 17.57 9.5 24 9.5z"/>
                <path fill="#34A853" d="M46.1 24.5c0-1.64-.15-3.22-.42-4.74H24v9.01h12.42c-.54 2.9-2.18 5.36-4.65 7.01l7.19 5.6C43.98 37.13 46.1 31.3 46.1 24.5z"/>
                <path fill="#FBBC05" d="M10.68 28.3a14.5 14.5 0 010-8.6l-7.99-6.21A24.01 24.01 0 000 24c0 3.77.9 7.34 2.69 10.91l7.99-6.21z"/>
                <path fill="#EA4335" d="M24 48c6.13 0 11.64-2.03 15.53-5.53l-7.19-5.6c-2.01 1.35-4.59 2.13-8.34 2.13-6.43 0-11.87-3.63-13.32-8.8l-7.99 6.21C6.73 42.2 14.82 48 24 48z"/>
              </svg>
              <span>Continue with Google</span>
            </button>

            <!-- Horizontal Divider -->
            <div class="flex items-center gap-3 my-4">
              <div class="flex-1 h-px bg-gray-200"></div>
              <span class="text-xs text-[#64748B] font-normal whitespace-nowrap">or sign in with email</span>
              <div class="flex-1 h-px bg-gray-200"></div>
            </div>

            <!-- Email & Password Form -->
            <form @submit.prevent="handleLogin" class="space-y-3.5">
              
              <!-- Email Input -->
              <div class="relative">
                <span class="absolute left-3.5 top-1/2 -translate-y-1/2 text-gray-400 pointer-events-none">
                  <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <rect width="20" height="16" x="2" y="4" rx="2"/>
                    <path d="m22 7-8.97 5.7a1.94 1.94 0 0 1-2.06 0L2 7"/>
                  </svg>
                </span>
                <input 
                  v-model="email" 
                  type="email" 
                  required 
                  autocomplete="email"
                  placeholder="Email address"
                  class="w-full bg-white border border-gray-200 text-[#061B2B] placeholder:text-gray-400 rounded-xl pl-10 pr-4 py-2.5 sm:py-3 text-sm focus:outline-none focus:border-[#00B85C] focus:ring-2 focus:ring-[#00B85C]/20 transition-all duration-200" 
                />
              </div>

              <!-- Password Input -->
              <div class="relative">
                <span class="absolute left-3.5 top-1/2 -translate-y-1/2 text-gray-400 pointer-events-none">
                  <svg class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <rect width="18" height="11" x="3" y="11" rx="2" ry="2"/>
                    <path d="M7 11V7a5 5 0 0 1 10 0v4"/>
                  </svg>
                </span>
                <input 
                  v-model="password" 
                  :type="showPassword ? 'text' : 'password'" 
                  required
                  autocomplete="current-password"
                  placeholder="Password"
                  class="w-full bg-white border border-gray-200 text-[#061B2B] placeholder:text-gray-400 rounded-xl pl-10 pr-11 py-2.5 sm:py-3 text-sm focus:outline-none focus:border-[#00B85C] focus:ring-2 focus:ring-[#00B85C]/20 transition-all duration-200" 
                />
                <!-- Visibility Toggle -->
                <button 
                  type="button" 
                  @click="showPassword = !showPassword"
                  class="absolute right-3.5 top-1/2 -translate-y-1/2 text-gray-400 hover:text-gray-600 transition-colors p-1"
                  aria-label="Toggle password visibility"
                >
                  <svg v-if="!showPassword" class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M2 12s3-7 10-7 10 7 10 7-3 7-10 7-10-7-10-7Z"/>
                    <circle cx="12" cy="12" r="3"/>
                  </svg>
                  <svg v-else class="w-4 h-4" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M9.88 9.88a3 3 0 1 0 4.24 4.24"/>
                    <path d="M10.73 5.08A10.43 10.43 0 0 1 12 5c7 0 10 7 10 7a13.16 13.16 0 0 1-1.67 2.68"/>
                    <path d="M6.61 6.61A13.526 13.526 0 0 0 2 12s3 7 10 7a9.74 9.74 0 0 0 5.39-1.61"/>
                    <line x1="2" x2="22" y1="2" y2="22"/>
                  </svg>
                </button>
              </div>

              <!-- Controls: Remember Me & Forgot Password -->
              <div class="flex items-center justify-between text-xs sm:text-sm pt-0.5">
                <label class="flex items-center gap-2 cursor-pointer text-[#64748B] hover:text-[#061B2B] transition-colors">
                  <input 
                    type="checkbox" 
                    v-model="rememberMe"
                    class="w-4 h-4 rounded border-gray-300 text-[#00B85C] focus:ring-[#00B85C] focus:ring-offset-0 cursor-pointer accent-[#00B85C]" 
                  />
                  <span>Remember me</span>
                </label>
                <button 
                  type="button" 
                  @click="openForgotPassword" 
                  class="text-[#00B85C] hover:text-[#087A45] font-semibold transition-colors focus:outline-none"
                >
                  Forgot password?
                </button>
              </div>

              <!-- Error Alert Message -->
              <div v-if="errorMsg" class="flex items-center gap-2 bg-red-50 border border-red-200 rounded-xl p-3 text-red-600 text-xs sm:text-sm animate-shake">
                <svg class="w-4 h-4 flex-shrink-0 text-red-500" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <circle cx="12" cy="12" r="10"/>
                  <line x1="12" x2="12" y1="8" y2="12"/>
                  <line x1="12" x2="12.01" y1="16" y2="16"/>
                </svg>
                <p class="font-medium">{{ errorMsg }}</p>
              </div>

              <!-- Success Alert Message (e.g. Password Reset) -->
              <div v-if="successMsg" class="flex items-center gap-2 bg-emerald-50 border border-emerald-200 rounded-xl p-3 text-emerald-700 text-xs sm:text-sm">
                <svg class="w-4 h-4 flex-shrink-0 text-[#00B85C]" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M20 6 9 17l-5-5"/>
                </svg>
                <p class="font-medium">{{ successMsg }}</p>
              </div>

              <!-- Primary Submit Button -->
              <button 
                type="submit" 
                :disabled="loading"
                class="w-full bg-gradient-to-r from-[#00B85C] to-[#087A45] hover:from-[#00c965] hover:to-[#0a8c4f] active:scale-[0.99] text-white font-bold py-3 px-6 rounded-xl shadow-lg shadow-[#00B85C]/25 hover:shadow-xl hover:shadow-[#00B85C]/35 transition-all duration-200 disabled:opacity-60 disabled:cursor-not-allowed flex items-center justify-center gap-2 text-sm sm:text-base group"
              >
                <svg v-if="loading" class="animate-spin -ml-1 mr-2 h-4 w-4 text-white" fill="none" viewBox="0 0 24 24">
                  <circle class="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" stroke-width="4"></circle>
                  <path class="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.291A7.962 7.962 0 014 12H0c0 3.042 1.135 5.824 3 7.938l3-2.647z"></path>
                </svg>
                <span>{{ loading ? 'Signing in...' : 'Sign In' }}</span>
                <svg v-if="!loading" class="w-4 h-4 transform group-hover:translate-x-1 transition-transform" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round">
                  <path d="M5 12h14"/>
                  <path d="m12 5 7 7-7 7"/>
                </svg>
              </button>
            </form>

            <!-- Registration Link -->
            <p class="text-center text-xs sm:text-sm text-[#64748B] mt-3.5">
              Don't have an account?
              <router-link to="/register" class="text-[#00B85C] hover:text-[#087A45] font-bold transition-colors ml-1">
                Create one
              </router-link>
            </p>

            <!-- Emergency Contact Login Button -->
            <div class="mt-4 pt-3.5 border-t border-gray-100">
              <button 
                type="button" 
                @click="goToEmergencyLogin"
                class="w-full flex items-center justify-center gap-2 bg-[#FEF2F2] hover:bg-[#FEE2E2] active:bg-[#FECACA] border border-[#FCA5A5] text-[#DC2626] font-bold py-2.5 sm:py-3 px-4 rounded-xl transition-all duration-200 text-xs sm:text-sm shadow-sm hover:shadow"
              >
                <!-- Emergency Alert Triangle Icon -->
                <svg class="w-4 h-4 text-[#DC2626]" viewBox="0 0 24 24" fill="currentColor">
                  <path fill-rule="evenodd" d="M9.401 3.003c1.155-2 4.043-2 5.197 0l7.355 12.748c1.154 2-.29 4.5-2.599 4.5H4.645c-2.309 0-3.752-2.5-2.598-4.5L9.4 3.003zM12 8.25a.75.75 0 01.75.75v3.75a.75.75 0 01-1.5 0V9a.75.75 0 01.75-.75zm0 8.25a.75.75 0 100-1.5.75.75 0 000 1.5z" clip-rule="evenodd" />
                </svg>
                <span>Emergency Contact Login</span>
              </button>
            </div>

          </div>
        </div>

      </div>
    </div>

    <!-- ========================================== -->
    <!-- BOTTOM FOOTER BAR                          -->
    <!-- ========================================== -->
    <footer class="relative z-10 w-full border-t border-white/5 bg-[#061B2B]/90 backdrop-blur-md py-3 px-4 sm:px-6 lg:px-8 text-xs text-slate-400">
      <div class="max-w-7xl mx-auto flex flex-col sm:flex-row items-center justify-between gap-2">
        <div class="flex items-center flex-wrap justify-center sm:justify-start gap-x-2 gap-y-1 font-medium">
          <span class="text-white font-semibold">VigiLERT</span>
          <span>•</span>
          <span>Motorcycle Safety</span>
          <span>•</span>
          <span>Emergency Response</span>
          <span>•</span>
          <span>Anti-Theft Protection</span>
        </div>
        <div class="flex items-center gap-1.5 text-[#00B85C] font-medium">
          <svg class="w-4 h-4" viewBox="0 0 24 24" fill="currentColor">
            <path fill-rule="evenodd" d="M12.516 2.17a.75.75 0 0 0-1.032 0 11.209 11.209 0 0 1-7.877 3.08.75.75 0 0 0-.722.515A12.74 12.74 0 0 0 2.25 9.75c0 5.942 4.064 10.933 9.563 12.348a.75.75 0 0 0 .374 0c5.499-1.415 9.563-6.406 9.563-12.348 0-1.39-.223-2.73-.635-3.985a.75.75 0 0 0-.722-.516l-.143.001c-2.996 0-5.717-1.17-7.734-3.08Zm3.094 8.016a.75.75 0 1 0-1.22-.872l-3.236 4.53L9.53 12.22a.75.75 0 0 0-1.06 1.06l2.25 2.25a.75.75 0 0 0 1.14-.094l3.75-5.25Z" clip-rule="evenodd" />
          </svg>
          <span>Secure & Trusted</span>
        </div>
      </div>
    </footer>

    <!-- ========================================== -->
    <!-- FORGOT PASSWORD MODAL                      -->
    <!-- ========================================== -->
    <div v-if="showForgotModal" class="fixed inset-0 z-50 flex items-center justify-center p-4 bg-black/60 backdrop-blur-sm animate-fadeIn">
      <div class="bg-white rounded-3xl p-6 sm:p-8 max-w-sm w-full shadow-2xl text-[#061B2B]">
        <div class="flex items-center justify-between mb-4">
          <h3 class="text-xl font-bold">Reset Password</h3>
          <button @click="showForgotModal = false" class="text-gray-400 hover:text-gray-600">
            <svg class="w-5 h-5" fill="none" viewBox="0 0 24 24" stroke="currentColor">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12"/>
            </svg>
          </button>
        </div>
        <p class="text-xs text-[#64748B] mb-4">
          Enter your registered email address and we'll send you a link to reset your password.
        </p>
        <form @submit.prevent="handleForgotPassword" class="space-y-4">
          <div>
            <label class="block text-xs font-semibold text-gray-700 mb-1">Email address</label>
            <input 
              v-model="forgotEmail" 
              type="email" 
              required 
              placeholder="you@example.com"
              class="w-full border border-gray-200 rounded-xl px-3.5 py-2.5 text-sm focus:outline-none focus:border-[#00B85C] focus:ring-2 focus:ring-[#00B85C]/20"
            />
          </div>
          <div v-if="forgotError" class="text-xs text-red-500 font-medium">
            {{ forgotError }}
          </div>
          <button 
            type="submit" 
            :disabled="forgotLoading"
            class="w-full bg-[#00B85C] hover:bg-[#087A45] text-white font-bold py-2.5 rounded-xl text-sm transition-all shadow-md hover:shadow-lg disabled:opacity-60"
          >
            {{ forgotLoading ? 'Sending link...' : 'Send Reset Link' }}
          </button>
        </form>
      </div>
    </div>

  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue';
import { useRouter } from 'vue-router';
import { auth } from '../firebase/config';
import { 
  signInWithEmailAndPassword, 
  GoogleAuthProvider, 
  signInWithPopup,
  sendPasswordResetEmail 
} from 'firebase/auth';

const router = useRouter();
const email = ref('');
const password = ref('');
const rememberMe = ref(false);
const loading = ref(false);
const errorMsg = ref('');
const successMsg = ref('');
const showPassword = ref(false);

// Fallbacks if images fail to load
const logoFallback = ref(false);
const cardLogoFallback = ref(false);
const riderFallback = ref(false);

// Forgot Password State
const showForgotModal = ref(false);
const forgotEmail = ref('');
const forgotLoading = ref(false);
const forgotError = ref('');

// Load remembered email on mount
onMounted(() => {
  const savedEmail = localStorage.getItem('vigilert_remember_email');
  if (savedEmail) {
    email.value = savedEmail;
    rememberMe.value = true;
  }
});

const handleLogin = async () => {
  loading.value = true;
  errorMsg.value = '';
  successMsg.value = '';
  try {
    await signInWithEmailAndPassword(auth, email.value, password.value);
    
    // Handle remember me
    if (rememberMe.value) {
      localStorage.setItem('vigilert_remember_email', email.value);
    } else {
      localStorage.removeItem('vigilert_remember_email');
    }

    router.push('/dashboard');
  } catch (error) {
    if (error.code === 'auth/invalid-credential' || error.code === 'auth/wrong-password' || error.code === 'auth/user-not-found') {
      errorMsg.value = 'Invalid email or password. Please try again.';
    } else if (error.code === 'auth/too-many-requests') {
      errorMsg.value = 'Too many failed login attempts. Please reset your password or try again later.';
    } else {
      errorMsg.value = error.message || 'Login failed. Please check your connection and try again.';
    }
  } finally {
    loading.value = false;
  }
};

const handleGoogleSignIn = async () => {
  loading.value = true;
  errorMsg.value = '';
  successMsg.value = '';
  try {
    const provider = new GoogleAuthProvider();
    const result = await signInWithPopup(auth, provider);
    const isNewUser = result._tokenResponse?.isNewUser ?? false;
    router.push(isNewUser ? '/device-setup?onboarding=true' : '/dashboard');
  } catch (error) {
    if (error.code !== 'auth/popup-closed-by-user') {
      errorMsg.value = error.message || 'Google sign-in failed.';
    }
  } finally {
    loading.value = false;
  }
};

const goToEmergencyLogin = () => {
  router.push('/emergency-login');
};

const openForgotPassword = () => {
  forgotEmail.value = email.value || '';
  forgotError.value = '';
  showForgotModal.value = true;
};

const handleForgotPassword = async () => {
  if (!forgotEmail.value) {
    forgotError.value = 'Please enter your email address.';
    return;
  }
  forgotLoading.value = true;
  forgotError.value = '';
  try {
    await sendPasswordResetEmail(auth, forgotEmail.value);
    showForgotModal.value = false;
    successMsg.value = `Password reset email sent to ${forgotEmail.value}. Check your inbox.`;
  } catch (error) {
    if (error.code === 'auth/user-not-found') {
      forgotError.value = 'No account found with this email.';
    } else {
      forgotError.value = error.message || 'Failed to send reset email. Please try again.';
    }
  } finally {
    forgotLoading.value = false;
  }
};
</script>

<style scoped>
@keyframes fadeIn {
  from { opacity: 0; transform: scale(0.96); }
  to { opacity: 1; transform: scale(1); }
}

.animate-fadeIn {
  animation: fadeIn 0.2s cubic-bezier(0.16, 1, 0.3, 1) forwards;
}

@keyframes shake {
  0%, 100% { transform: translateX(0); }
  20%, 60% { transform: translateX(-4px); }
  40%, 80% { transform: translateX(4px); }
}

.animate-shake {
  animation: shake 0.4s ease-in-out;
}
</style>
