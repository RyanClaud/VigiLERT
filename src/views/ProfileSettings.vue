<template>
  <div class="space-y-6 pb-16 font-['Inter',sans-serif] max-w-5xl mx-auto">
    
    <!-- Top Header Banner -->
    <div class="bg-[#0B2540]/85 border border-[#00B85C]/25 shadow-2xl rounded-3xl p-6 sm:p-8 backdrop-blur-md relative overflow-hidden">
      <!-- Glow -->
      <div class="absolute -top-24 -right-24 w-80 h-80 bg-[#00B85C]/10 rounded-full blur-3xl pointer-events-none"></div>

      <div class="flex flex-col sm:flex-row items-start sm:items-center justify-between gap-4">
        <div class="flex items-center gap-3.5">
          <div class="w-12 h-12 rounded-2xl bg-[#00B85C]/15 border border-[#00B85C]/30 flex items-center justify-center text-[#00B85C] shadow-md shadow-[#00B85C]/10">
            <span class="material-icons text-3xl">manage_accounts</span>
          </div>
          <div>
            <h1 class="text-2xl sm:text-3xl font-black text-white tracking-tight">Profile & Security Settings</h1>
            <p class="text-xs sm:text-sm font-semibold text-slate-300 mt-0.5">
              Manage your rider profile photo, personal information, and authentication credentials
            </p>
          </div>
        </div>

        <router-link
          to="/dashboard"
          class="inline-flex items-center gap-2 px-4 py-2 rounded-xl bg-white/5 hover:bg-white/10 text-slate-300 hover:text-white border border-white/10 text-xs font-bold transition-all"
        >
          <span class="material-icons text-sm">arrow_back</span>
          <span>Return to Dashboard</span>
        </router-link>
      </div>
    </div>

    <!-- Feedback Banner (Success / Error) -->
    <transition name="fade">
      <div v-if="statusMessage.text" 
        :class="[
          'rounded-2xl p-4 border flex items-start gap-3 shadow-lg transition-all',
          statusMessage.type === 'success' 
            ? 'bg-emerald-500/15 border-emerald-500/40 text-emerald-200' 
            : 'bg-red-500/15 border-red-500/40 text-red-200'
        ]"
      >
        <span class="material-icons text-xl flex-shrink-0 mt-0.5">
          {{ statusMessage.type === 'success' ? 'check_circle' : 'error' }}
        </span>
        <div class="flex-1 text-xs sm:text-sm font-medium">
          {{ statusMessage.text }}
        </div>
        <button @click="statusMessage.text = ''" class="text-white/60 hover:text-white">
          <span class="material-icons text-base">close</span>
        </button>
      </div>
    </transition>

    <div class="grid grid-cols-1 lg:grid-cols-12 gap-6">

      <!-- ======================================================== -->
      <!-- LEFT COLUMN: PROFILE PHOTO & PERSONAL INFO (5 Cols)     -->
      <!-- ======================================================== -->
      <div class="lg:col-span-5 space-y-6">

        <!-- Profile Picture Card -->
        <div class="bg-[#0B2540]/80 border border-white/10 rounded-3xl p-6 shadow-xl backdrop-blur-md">
          <h2 class="text-lg font-bold text-white flex items-center gap-2 mb-4">
            <span class="material-icons text-[#00B85C] text-xl">account_circle</span>
            <span>Rider Avatar</span>
          </h2>

          <div class="flex flex-col items-center text-center">
            <!-- Avatar Display Container -->
            <div class="relative group my-2">
              <div class="w-32 h-32 rounded-full overflow-hidden border-4 border-[#00B85C] bg-[#061B2B] shadow-2xl shadow-[#00B85C]/20 flex items-center justify-center relative">
                <img
                  v-if="profilePhotoUrl"
                  :src="profilePhotoUrl"
                  alt="Profile Avatar"
                  class="w-full h-full object-cover"
                />
                <span v-else class="text-4xl font-black text-white/80">
                  {{ (displayName || currentUser?.email || 'R')[0].toUpperCase() }}
                </span>

                <!-- Dark overlay on hover -->
                <div
                  @click="triggerFileInput"
                  class="absolute inset-0 bg-black/50 opacity-0 group-hover:opacity-100 transition-opacity flex flex-col items-center justify-center text-white cursor-pointer"
                >
                  <span class="material-icons text-2xl">photo_camera</span>
                  <span class="text-[10px] font-bold mt-1">Change</span>
                </div>
              </div>

              <!-- Floating Camera Icon Badge -->
              <button
                @click="triggerFileInput"
                type="button"
                class="absolute bottom-1 right-1 w-9 h-9 rounded-full bg-gradient-to-r from-[#00B85C] to-[#087A45] hover:scale-110 active:scale-95 text-white flex items-center justify-center shadow-lg border-2 border-[#0B2540] transition-all"
                title="Upload Photo"
              >
                <span class="material-icons text-sm">photo_camera</span>
              </button>
            </div>

            <!-- Hidden File Input -->
            <input
              ref="fileInputRef"
              type="file"
              accept="image/png, image/jpeg, image/jpg, image/webp"
              @change="handlePhotoUpload"
              class="hidden"
            />

            <!-- Info text -->
            <p class="text-xs text-slate-300 mt-2 max-w-xs">
              Upload a clear photo. This avatar is displayed on your account and appears directly on the live GPS map.
            </p>

            <!-- Actions -->
            <div class="flex items-center gap-2 mt-4 w-full">
              <button
                @click="triggerFileInput"
                :disabled="isUploadingPhoto"
                class="flex-1 py-2 px-3 bg-gradient-to-r from-[#00B85C] to-[#087A45] hover:from-[#00c965] hover:to-[#0a8c4f] text-white text-xs font-bold rounded-xl shadow-md transition-all flex items-center justify-center gap-1.5 disabled:opacity-50"
              >
                <span class="material-icons text-sm">upload</span>
                <span>{{ isUploadingPhoto ? 'Saving...' : 'Upload Photo' }}</span>
              </button>

              <button
                v-if="profilePhotoUrl"
                @click="removePhoto"
                :disabled="isUploadingPhoto"
                class="py-2 px-3 bg-red-500/10 hover:bg-red-500/20 text-red-400 border border-red-500/25 text-xs font-bold rounded-xl transition-all flex items-center justify-center gap-1"
                title="Remove current avatar"
              >
                <span class="material-icons text-sm">delete</span>
                <span>Remove</span>
              </button>
            </div>
          </div>
        </div>

        <!-- Live Map Marker Visual Preview -->
        <div class="bg-[#0B2540]/80 border border-white/10 rounded-3xl p-6 shadow-xl backdrop-blur-md">
          <div class="flex items-center justify-between mb-3">
            <h3 class="text-sm font-bold text-white flex items-center gap-1.5">
              <span class="material-icons text-[#00B85C] text-base">pin_drop</span>
              <span>Live Map Marker Preview</span>
            </h3>
            <span class="text-[10px] uppercase font-bold text-[#00B85C] bg-[#00B85C]/10 px-2 py-0.5 rounded-full border border-[#00B85C]/20">
              On Map
            </span>
          </div>

          <p class="text-xs text-slate-300 mb-4">
            This is how your real-time position pin appears on the live map:
          </p>

          <!-- Mock Map Container -->
          <div class="bg-[#061B2B] border border-white/10 rounded-2xl h-44 relative flex items-center justify-center overflow-hidden">
            <!-- Simulated Map Grid Lines -->
            <div class="absolute inset-0 opacity-[0.07]"
              style="background-image: linear-gradient(rgba(255,255,255,0.4) 1px, transparent 1px), linear-gradient(90deg, rgba(255,255,255,0.4) 1px, transparent 1px); background-size: 24px 24px;">
            </div>

            <!-- Faint Route Line -->
            <svg class="absolute inset-0 w-full h-full pointer-events-none opacity-40">
              <path d="M 20 130 Q 120 40 220 90 T 380 40" stroke="#00B85C" stroke-width="3" stroke-dasharray="6,4" fill="none"/>
            </svg>

            <!-- Custom Location Marker with Avatar -->
            <div class="relative flex items-center justify-center">
              <!-- Radar Pulse Ring -->
              <div class="absolute w-16 h-16 rounded-full bg-[#00B85C]/30 animate-ping pointer-events-none"></div>

              <!-- Main Pin Head -->
              <div class="relative w-12 h-12 rounded-[50%_50%_50%_0] -rotate-45 bg-[#00B85C] border-2 border-white shadow-xl flex items-center justify-center z-10">
                <!-- Rotated photo upright -->
                <div class="rotate-45 w-9 h-9 rounded-full overflow-hidden bg-[#061B2B] flex items-center justify-center border border-white/40">
                  <img
                    v-if="profilePhotoUrl"
                    :src="profilePhotoUrl"
                    alt="Map Avatar"
                    class="w-full h-full object-cover"
                  />
                  <span v-else class="text-xs font-black text-white">
                    {{ (displayName || currentUser?.email || 'R')[0].toUpperCase() }}
                  </span>
                </div>
              </div>

              <!-- Mini Vehicle Badge -->
              <div class="absolute -bottom-1 -right-1 w-5 h-5 rounded-full bg-[#061B2B] border border-white flex items-center justify-center text-[10px] z-20 shadow">
                🏍️
              </div>
            </div>

            <!-- Bottom Mock Map Status -->
            <div class="absolute bottom-2 left-2 right-2 bg-[#0B2540]/80 backdrop-blur-sm border border-white/10 rounded-xl px-2.5 py-1 flex items-center justify-between text-[10px] text-slate-300">
              <span>GPS Tracking Active</span>
              <span class="text-[#00B85C] font-semibold">Your Live Position</span>
            </div>
          </div>
        </div>

      </div>

      <!-- ======================================================== -->
      <!-- RIGHT COLUMN: ACCOUNT INFO & PASSWORD SECURITY (7 Cols)   -->
      <!-- ======================================================== -->
      <div class="lg:col-span-7 space-y-6">

        <!-- Personal Details Card -->
        <div class="bg-[#0B2540]/80 border border-white/10 rounded-3xl p-6 sm:p-7 shadow-xl backdrop-blur-md">
          <h2 class="text-lg font-bold text-white flex items-center gap-2 mb-4">
            <span class="material-icons text-[#00B85C] text-xl">badge</span>
            <span>Personal Information</span>
          </h2>

          <form @submit.prevent="updateRiderProfile" class="space-y-4">
            <!-- Full Name -->
            <div>
              <label for="profile-display-name" class="block text-xs font-bold text-slate-300 uppercase tracking-wider mb-1.5">
                Rider Display Name
              </label>
              <div class="relative">
                <span class="absolute left-3.5 top-1/2 -translate-y-1/2 text-slate-400 pointer-events-none">
                  <span class="material-icons text-lg">person</span>
                </span>
                <input
                  id="profile-display-name"
                  type="text"
                  v-model="displayName"
                  placeholder="e.g. Juan dela Cruz"
                  class="w-full pl-10 pr-4 py-2.5 bg-[#061B2B] border border-white/15 focus:border-[#00B85C] rounded-xl text-white text-sm outline-none transition-all focus:ring-1 focus:ring-[#00B85C]"
                  required
                />
              </div>
            </div>

            <!-- Email Address (Read Only) -->
            <div>
              <label for="profile-email" class="block text-xs font-bold text-slate-300 uppercase tracking-wider mb-1.5">
                Email Address
              </label>
              <div class="relative">
                <span class="absolute left-3.5 top-1/2 -translate-y-1/2 text-slate-400 pointer-events-none">
                  <span class="material-icons text-lg">email</span>
                </span>
                <input
                  id="profile-email"
                  type="email"
                  :value="currentUser?.email || ''"
                  readonly
                  class="w-full pl-10 pr-4 py-2.5 bg-[#061B2B]/60 border border-white/10 rounded-xl text-slate-400 text-sm outline-none cursor-not-allowed"
                />
                <span class="absolute right-3 top-1/2 -translate-y-1/2 text-[10px] font-bold px-2 py-0.5 rounded bg-emerald-500/15 text-emerald-400 border border-emerald-500/30">
                  Primary Auth
                </span>
              </div>
            </div>

            <!-- Connected Providers Info -->
            <div class="pt-2">
              <span class="block text-xs font-bold text-slate-300 uppercase tracking-wider mb-2">
                Authentication Providers Linked
              </span>
              <div class="flex flex-wrap items-center gap-2">
                <!-- Google Provider Pill -->
                <div
                  v-if="isGoogleLinked"
                  class="inline-flex items-center gap-2 px-3 py-1.5 rounded-xl bg-white/5 border border-white/10 text-xs text-white"
                >
                  <svg class="w-3.5 h-3.5" viewBox="0 0 48 48">
                    <path fill="#4285F4" d="M24 9.5c3.54 0 6.7 1.22 9.19 3.23l6.87-6.87C35.64 2.36 30.13 0 24 0 14.82 0 6.73 5.8 2.69 14.09l7.99 6.21C12.13 13.13 17.57 9.5 24 9.5z"/>
                    <path fill="#34A853" d="M46.1 24.5c0-1.64-.15-3.22-.42-4.74H24v9.01h12.42c-.54 2.9-2.18 5.36-4.65 7.01l7.19 5.6C43.98 37.13 46.1 31.3 46.1 24.5z"/>
                    <path fill="#FBBC05" d="M10.68 28.3a14.5 14.5 0 010-8.6l-7.99-6.21A24.01 24.01 0 000 24c0 3.77.9 7.34 2.69 10.91l7.99-6.21z"/>
                    <path fill="#EA4335" d="M24 48c6.13 0 11.64-2.03 15.53-5.53l-7.19-5.6c-2.01 1.35-4.59 2.13-8.34 2.13-6.43 0-11.87-3.63-13.32-8.8l-7.99 6.21C6.73 42.2 14.82 48 24 48z"/>
                  </svg>
                  <span>Google Account Linked</span>
                </div>

                <!-- Password Provider Pill -->
                <div
                  v-if="hasPasswordProvider"
                  class="inline-flex items-center gap-2 px-3 py-1.5 rounded-xl bg-emerald-500/10 border border-emerald-500/30 text-xs text-emerald-300"
                >
                  <span class="material-icons text-xs text-[#00B85C]">lock</span>
                  <span>Email & Password Active</span>
                </div>

                <!-- Missing Password Pill -->
                <div
                  v-else
                  class="inline-flex items-center gap-2 px-3 py-1.5 rounded-xl bg-amber-500/10 border border-amber-500/30 text-xs text-amber-300"
                >
                  <span class="material-icons text-xs text-amber-400">warning</span>
                  <span>No Password Created Yet</span>
                </div>
              </div>
            </div>

            <!-- Save Profile Details Button -->
            <div class="pt-2">
              <button
                type="submit"
                :disabled="isUpdatingProfile"
                class="px-5 py-2.5 bg-gradient-to-r from-[#00B85C] to-[#087A45] hover:from-[#00c965] hover:to-[#0a8c4f] text-white text-xs font-bold rounded-xl shadow-md transition-all flex items-center gap-2 disabled:opacity-50"
              >
                <span class="material-icons text-sm">save</span>
                <span>{{ isUpdatingProfile ? 'Saving Changes...' : 'Save Profile Changes' }}</span>
              </button>
            </div>
          </form>
        </div>

        <!-- ======================================================== -->
        <!-- PASSWORD MANAGEMENT & MANUAL LOGIN SETUP                 -->
        <!-- ======================================================== -->
        <div class="bg-[#0B2540]/80 border border-white/10 rounded-3xl p-6 sm:p-7 shadow-xl backdrop-blur-md">
          
          <!-- Section Title -->
          <div class="flex items-center justify-between mb-4 pb-3 border-b border-white/10">
            <h2 class="text-lg font-bold text-white flex items-center gap-2">
              <span class="material-icons text-[#00B85C] text-xl">security</span>
              <span>Password & Security</span>
            </h2>

            <span
              :class="[
                'text-[11px] font-bold px-2.5 py-1 rounded-full border',
                hasPasswordProvider 
                  ? 'bg-emerald-500/15 text-emerald-300 border-emerald-500/30' 
                  : 'bg-amber-500/15 text-amber-300 border-amber-500/30'
              ]"
            >
              {{ hasPasswordProvider ? 'Manual Login Enabled' : 'Action Needed for Manual Login' }}
            </span>
          </div>

          <!-- ============================================================== -->
          <!-- CASE A: Google user who has NOT set a password yet             -->
          <!-- ============================================================== -->
          <div v-if="!hasPasswordProvider" class="space-y-4">
            
            <!-- Explanatory Alert Banner -->
            <div class="bg-amber-500/10 border border-amber-500/30 rounded-2xl p-4 text-xs text-amber-200 space-y-1.5">
              <div class="flex items-center gap-2 font-bold text-amber-300 text-sm">
                <span class="material-icons text-base">info</span>
                <span>Create a Password for Manual Sign-In</span>
              </div>
              <p>
                You are currently signed in through <strong>Google Authentication</strong>. Because of this, your account doesn't have a manual password assigned yet.
              </p>
              <p class="text-slate-300">
                Set a password below so you can sign in with your email (<strong>{{ currentUser?.email }}</strong>) and your chosen password on any device without Google!
              </p>
            </div>

            <!-- Set Password Form -->
            <form @submit.prevent="handleLinkPassword" class="space-y-3.5">
              <!-- New Password -->
              <div>
                <label for="create-new-password" class="block text-xs font-bold text-slate-300 uppercase tracking-wider mb-1.5">
                  Create New Password
                </label>
                <div class="relative">
                  <span class="absolute left-3.5 top-1/2 -translate-y-1/2 text-slate-400 pointer-events-none">
                    <span class="material-icons text-lg">lock</span>
                  </span>
                  <input
                    id="create-new-password"
                    :type="showPassword ? 'text' : 'password'"
                    v-model="passwordForm.newPassword"
                    placeholder="Minimum 6 characters"
                    class="w-full pl-10 pr-10 py-2.5 bg-[#061B2B] border border-white/15 focus:border-[#00B85C] rounded-xl text-white text-sm outline-none transition-all focus:ring-1 focus:ring-[#00B85C]"
                    required
                    minlength="6"
                  />
                  <button
                    type="button"
                    @click="showPassword = !showPassword"
                    class="absolute right-3 top-1/2 -translate-y-1/2 text-slate-400 hover:text-white"
                  >
                    <span class="material-icons text-sm">{{ showPassword ? 'visibility_off' : 'visibility' }}</span>
                  </button>
                </div>
              </div>

              <!-- Confirm Password -->
              <div>
                <label for="confirm-create-password" class="block text-xs font-bold text-slate-300 uppercase tracking-wider mb-1.5">
                  Confirm Password
                </label>
                <div class="relative">
                  <span class="absolute left-3.5 top-1/2 -translate-y-1/2 text-slate-400 pointer-events-none">
                    <span class="material-icons text-lg">lock_clock</span>
                  </span>
                  <input
                    id="confirm-create-password"
                    :type="showPassword ? 'text' : 'password'"
                    v-model="passwordForm.confirmPassword"
                    placeholder="Re-enter password"
                    class="w-full pl-10 pr-4 py-2.5 bg-[#061B2B] border border-white/15 focus:border-[#00B85C] rounded-xl text-white text-sm outline-none transition-all focus:ring-1 focus:ring-[#00B85C]"
                    required
                    minlength="6"
                  />
                </div>
              </div>

              <button
                type="submit"
                :disabled="isUpdatingPassword"
                class="w-full py-3 bg-gradient-to-r from-[#00B85C] to-[#087A45] hover:from-[#00c965] hover:to-[#0a8c4f] text-white text-xs sm:text-sm font-bold rounded-xl shadow-lg shadow-[#00B85C]/25 transition-all flex items-center justify-center gap-2 disabled:opacity-50"
              >
                <span class="material-icons text-base">key</span>
                <span>{{ isUpdatingPassword ? 'Linking Password...' : 'Set Password & Enable Manual Login' }}</span>
              </button>
            </form>

          </div>

          <!-- ============================================================== -->
          <!-- CASE B: User ALREADY has a password linked                     -->
          <!-- ============================================================== -->
          <div v-else class="space-y-4">
            <p class="text-xs text-slate-300">
              Your account has manual email/password login enabled. You can update your password below at any time.
            </p>

            <form @submit.prevent="handleChangePassword" class="space-y-3.5">
              <!-- Current Password -->
              <div>
                <label for="profile-current-password" class="block text-xs font-bold text-slate-300 uppercase tracking-wider mb-1.5">
                  Current Password
                </label>
                <div class="relative">
                  <span class="absolute left-3.5 top-1/2 -translate-y-1/2 text-slate-400 pointer-events-none">
                    <span class="material-icons text-lg">lock_open</span>
                  </span>
                  <input
                    id="profile-current-password"
                    :type="showPassword ? 'text' : 'password'"
                    v-model="passwordForm.currentPassword"
                    placeholder="Enter current password"
                    class="w-full pl-10 pr-10 py-2.5 bg-[#061B2B] border border-white/15 focus:border-[#00B85C] rounded-xl text-white text-sm outline-none transition-all focus:ring-1 focus:ring-[#00B85C]"
                    required
                  />
                  <button
                    type="button"
                    @click="showPassword = !showPassword"
                    class="absolute right-3 top-1/2 -translate-y-1/2 text-slate-400 hover:text-white"
                  >
                    <span class="material-icons text-sm">{{ showPassword ? 'visibility_off' : 'visibility' }}</span>
                  </button>
                </div>
              </div>

              <!-- New Password -->
              <div>
                <label for="profile-new-password" class="block text-xs font-bold text-slate-300 uppercase tracking-wider mb-1.5">
                  New Password
                </label>
                <div class="relative">
                  <span class="absolute left-3.5 top-1/2 -translate-y-1/2 text-slate-400 pointer-events-none">
                    <span class="material-icons text-lg">lock</span>
                  </span>
                  <input
                    id="profile-new-password"
                    :type="showPassword ? 'text' : 'password'"
                    v-model="passwordForm.newPassword"
                    placeholder="Minimum 6 characters"
                    class="w-full pl-10 pr-4 py-2.5 bg-[#061B2B] border border-white/15 focus:border-[#00B85C] rounded-xl text-white text-sm outline-none transition-all focus:ring-1 focus:ring-[#00B85C]"
                    required
                    minlength="6"
                  />
                </div>
              </div>

              <!-- Confirm New Password -->
              <div>
                <label for="profile-confirm-password" class="block text-xs font-bold text-slate-300 uppercase tracking-wider mb-1.5">
                  Confirm New Password
                </label>
                <div class="relative">
                  <span class="absolute left-3.5 top-1/2 -translate-y-1/2 text-slate-400 pointer-events-none">
                    <span class="material-icons text-lg">lock_clock</span>
                  </span>
                  <input
                    id="profile-confirm-password"
                    :type="showPassword ? 'text' : 'password'"
                    v-model="passwordForm.confirmPassword"
                    placeholder="Re-enter new password"
                    class="w-full pl-10 pr-4 py-2.5 bg-[#061B2B] border border-white/15 focus:border-[#00B85C] rounded-xl text-white text-sm outline-none transition-all focus:ring-1 focus:ring-[#00B85C]"
                    required
                    minlength="6"
                  />
                </div>
              </div>

              <button
                type="submit"
                :disabled="isUpdatingPassword"
                class="w-full py-3 bg-gradient-to-r from-[#00B85C] to-[#087A45] hover:from-[#00c965] hover:to-[#0a8c4f] text-white text-xs sm:text-sm font-bold rounded-xl shadow-lg shadow-[#00B85C]/25 transition-all flex items-center justify-center gap-2 disabled:opacity-50"
              >
                <span class="material-icons text-base">published_with_changes</span>
                <span>{{ isUpdatingPassword ? 'Updating Password...' : 'Update Password' }}</span>
              </button>
            </form>
          </div>

          <!-- ============================================================== -->
          <!-- ADDITIONAL: Send Password Setup / Reset Email                  -->
          <!-- ============================================================== -->
          <div class="mt-6 pt-4 border-t border-white/10 flex flex-col sm:flex-row items-start sm:items-center justify-between gap-3">
            <div>
              <p class="text-xs font-bold text-white">Prefer to set password via email?</p>
              <p class="text-[11px] text-slate-400">
                We'll send an official password reset & setup link directly to {{ currentUser?.email }}.
              </p>
            </div>

            <button
              @click="handleSendResetEmail"
              :disabled="isSendingResetEmail"
              type="button"
              class="px-4 py-2 bg-white/5 hover:bg-white/10 active:bg-white/15 text-slate-200 hover:text-white border border-white/15 text-xs font-semibold rounded-xl transition-all flex items-center gap-1.5 whitespace-nowrap"
            >
              <span class="material-icons text-sm text-[#00B85C]">mark_email_read</span>
              <span>{{ isSendingResetEmail ? 'Sending Link...' : 'Send Reset Email' }}</span>
            </button>
          </div>

        </div>

      </div>

    </div>

  </div>
</template>

<script setup>
import { ref, computed, watch, onMounted } from 'vue';
import { useAuthStore } from '../stores/auth';
import { auth, database } from '../firebase/config';
import { 
  updateProfile, 
  updatePassword, 
  linkWithCredential, 
  EmailAuthProvider, 
  reauthenticateWithCredential,
  sendPasswordResetEmail
} from 'firebase/auth';
import { ref as dbRef, update } from 'firebase/database';

const authStore = useAuthStore();
const currentUser = computed(() => authStore.user || auth.currentUser);

// State
const displayName = ref('');
const profilePhotoUrl = ref('');
const fileInputRef = ref(null);
const isUploadingPhoto = ref(false);
const isUpdatingProfile = ref(false);
const isUpdatingPassword = ref(false);
const isSendingResetEmail = ref(false);
const showPassword = ref(false);

const statusMessage = ref({
  type: '',
  text: ''
});

const passwordForm = ref({
  currentPassword: '',
  newPassword: '',
  confirmPassword: ''
});

// Check linked providers
const isGoogleLinked = computed(() => {
  return currentUser.value?.providerData?.some(p => p.providerId === 'google.com') || false;
});

const hasPasswordProvider = computed(() => {
  return currentUser.value?.providerData?.some(p => p.providerId === 'password') || false;
});

// Initialize user data on load
onMounted(() => {
  if (currentUser.value) {
    displayName.value = currentUser.value.displayName || '';
    profilePhotoUrl.value = authStore.photoURL || currentUser.value.photoURL || '';
  }
});

// Watch for photo updates in authStore
watch(() => authStore.photoURL, (newPhoto) => {
  if (newPhoto) {
    profilePhotoUrl.value = newPhoto;
  }
});

const showMessage = (type, text) => {
  statusMessage.value = { type, text };
  window.scrollTo({ top: 0, behavior: 'smooth' });
};

// Trigger hidden file picker
const triggerFileInput = () => {
  if (fileInputRef.value) {
    fileInputRef.value.click();
  }
};

// Image Compression via HTML5 Canvas (Produces clean, high-performance base64 data URL)
const compressImage = (file) => {
  return new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.onload = (e) => {
      const img = new Image();
      img.onload = () => {
        const canvas = document.createElement('canvas');
        const maxDim = 200; // 200x200 is optimal for high DPI round avatar & map marker
        let width = img.width;
        let height = img.height;

        if (width > height) {
          if (width > maxDim) {
            height = Math.round((height * maxDim) / width);
            width = maxDim;
          }
        } else {
          if (height > maxDim) {
            width = Math.round((width * maxDim) / height);
            height = maxDim;
          }
        }

        canvas.width = width;
        canvas.height = height;
        const ctx = canvas.getContext('2d');
        ctx.drawImage(img, 0, 0, width, height);
        // Export as JPEG at 0.78 quality (~10-15KB)
        const dataUrl = canvas.toDataURL('image/jpeg', 0.78);
        resolve(dataUrl);
      };
      img.onerror = reject;
      img.src = e.target.result;
    };
    reader.onerror = reject;
    reader.readAsDataURL(file);
  });
};

// Handle Photo Upload (Guaranteed safe from Firebase Auth 2048-char URL limit)
const handlePhotoUpload = async (event) => {
  const file = event.target.files?.[0];
  if (!file) return;

  if (file.size > 10 * 1024 * 1024) {
    showMessage('error', 'Image size must be less than 10MB.');
    return;
  }

  isUploadingPhoto.value = true;
  try {
    const compressedDataUrl = await compressImage(file);
    profilePhotoUrl.value = compressedDataUrl;

    const uid = auth.currentUser?.uid;
    if (uid) {
      // 1. Update Pinia Store & LocalStorage immediately for instant UI responsiveness
      authStore.setPhotoURL(compressedDataUrl);

      // 2. Update Firebase Realtime Database (no 2048-character limit)
      await update(dbRef(database, `users/${uid}`), {
        photoURL: compressedDataUrl,
        updatedAt: Date.now()
      });
      await update(dbRef(database, `helmet_public/${uid}/profile`), {
        photoURL: compressedDataUrl,
        updatedAt: Date.now()
      }).catch(() => {});

      // 3. Only call updateProfile if URL is within Firebase Auth's 2048-char limit
      if (compressedDataUrl.length < 2048) {
        try {
          await updateProfile(auth.currentUser, { photoURL: compressedDataUrl });
        } catch (authErr) {
          console.debug('[AUTH] Profile photo URL skipped in Firebase Auth:', authErr);
        }
      }
    }

    showMessage('success', 'Profile photo updated successfully! It is now visible on your live GPS tracking map.');
  } catch (err) {
    console.error('Error saving photo:', err);
    showMessage('error', 'Failed to update profile photo: ' + (err.message || 'Please try another image'));
  } finally {
    isUploadingPhoto.value = false;
    if (fileInputRef.value) fileInputRef.value.value = '';
  }
};

// Remove Photo
const removePhoto = async () => {
  if (!confirm('Are you sure you want to remove your profile picture?')) return;
  isUploadingPhoto.value = true;
  try {
    profilePhotoUrl.value = '';
    const uid = auth.currentUser?.uid;
    if (uid) {
      authStore.setPhotoURL('');
      await update(dbRef(database, `users/${uid}`), { photoURL: '' });
      await update(dbRef(database, `helmet_public/${uid}/profile`), { photoURL: '' }).catch(() => {});
      try {
        await updateProfile(auth.currentUser, { photoURL: '' });
      } catch (authErr) {
        console.debug('[AUTH] Profile photo removal from Auth:', authErr);
      }
    }
    showMessage('success', 'Profile photo removed.');
  } catch (err) {
    console.error('Error removing photo:', err);
    showMessage('error', 'Failed to remove photo.');
  } finally {
    isUploadingPhoto.value = false;
  }
};

// Update Rider Display Name
const updateRiderProfile = async () => {
  if (!displayName.value.trim()) {
    showMessage('error', 'Please enter a valid display name.');
    return;
  }

  isUpdatingProfile.value = true;
  try {
    if (auth.currentUser) {
      await updateProfile(auth.currentUser, { displayName: displayName.value.trim() });
      const uid = auth.currentUser.uid;
      await update(dbRef(database, `users/${uid}`), {
        displayName: displayName.value.trim(),
        updatedAt: Date.now()
      });
      await update(dbRef(database, `helmet_public/${uid}/profile`), {
        displayName: displayName.value.trim()
      }).catch(() => {});

      if (authStore.user) {
        authStore.user.displayName = displayName.value.trim();
      }
    }
    showMessage('success', 'Profile information updated successfully!');
  } catch (err) {
    console.error('Error updating name:', err);
    showMessage('error', 'Failed to update profile: ' + err.message);
  } finally {
    isUpdatingProfile.value = false;
  }
};

// Link Password for Google-authenticated user (Solves manual sign-in issue)
const handleLinkPassword = async () => {
  if (passwordForm.value.newPassword !== passwordForm.value.confirmPassword) {
    showMessage('error', 'Passwords do not match. Please re-enter.');
    return;
  }

  if (passwordForm.value.newPassword.length < 6) {
    showMessage('error', 'Password must be at least 6 characters long.');
    return;
  }

  isUpdatingPassword.value = true;
  try {
    const email = auth.currentUser.email;
    const credential = EmailAuthProvider.credential(email, passwordForm.value.newPassword);

    // Link credential to allow manual email + password login
    await linkWithCredential(auth.currentUser, credential);

    // Refresh user state
    if (authStore.user) {
      authStore.user = { ...auth.currentUser };
    }

    passwordForm.value.newPassword = '';
    passwordForm.value.confirmPassword = '';
    showMessage(
      'success',
      'Password successfully created and linked! You can now log in using your email and password as well as with Google.'
    );
  } catch (err) {
    console.error('Error linking password:', err);
    let msg = err.message;
    if (err.code === 'auth/credential-already-in-use') {
      msg = 'This email account is already linked with another password credential.';
    } else if (err.code === 'auth/requires-recent-login') {
      msg = 'For security reasons, please log out and log in again before setting your password.';
    }
    showMessage('error', 'Failed to link password: ' + msg);
  } finally {
    isUpdatingPassword.value = false;
  }
};

// Change Password for user who already has a password provider
const handleChangePassword = async () => {
  if (passwordForm.value.newPassword !== passwordForm.value.confirmPassword) {
    showMessage('error', 'New passwords do not match.');
    return;
  }

  if (passwordForm.value.newPassword.length < 6) {
    showMessage('error', 'New password must be at least 6 characters long.');
    return;
  }

  isUpdatingPassword.value = true;
  try {
    const user = auth.currentUser;
    // Re-authenticate user with current password
    const credential = EmailAuthProvider.credential(user.email, passwordForm.value.currentPassword);
    await reauthenticateWithCredential(user, credential);

    // Update password
    await updatePassword(user, passwordForm.value.newPassword);

    passwordForm.value.currentPassword = '';
    passwordForm.value.newPassword = '';
    passwordForm.value.confirmPassword = '';

    showMessage('success', 'Your password has been updated successfully!');
  } catch (err) {
    console.error('Error updating password:', err);
    let msg = err.message;
    if (err.code === 'auth/wrong-password' || err.code === 'auth/invalid-credential') {
      msg = 'The current password you entered is incorrect.';
    } else if (err.code === 'auth/requires-recent-login') {
      msg = 'Please log out and log in again before changing your password.';
    }
    showMessage('error', 'Failed to update password: ' + msg);
  } finally {
    isUpdatingPassword.value = false;
  }
};

// Send password setup / reset email
const handleSendResetEmail = async () => {
  if (!currentUser.value?.email) return;
  isSendingResetEmail.value = true;
  try {
    await sendPasswordResetEmail(auth, currentUser.value.email);
    showMessage(
      'success',
      `Password reset/setup email sent to ${currentUser.value.email}! Follow the link in your inbox to set your password.`
    );
  } catch (err) {
    console.error('Error sending reset email:', err);
    showMessage('error', 'Failed to send reset email: ' + err.message);
  } finally {
    isSendingResetEmail.value = false;
  }
};
</script>

<style scoped>
.fade-enter-active,
.fade-leave-active {
  transition: opacity 0.3s ease;
}
.fade-enter-from,
.fade-leave-to {
  opacity: 0;
}
</style>
