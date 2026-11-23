<template>
  <div class="space-y-6 pb-12 font-['Inter',sans-serif]">
    
    <!-- Emergency Contacts Main Card -->
    <div class="bg-[#0B2540]/80 border border-[#00B85C]/20 shadow-2xl rounded-3xl p-6 sm:p-8 backdrop-blur-md relative overflow-hidden">
      <!-- Ambient Glow -->
      <div class="absolute -top-24 -right-24 w-96 h-96 bg-[#00B85C]/10 rounded-full blur-3xl pointer-events-none"></div>

      <div class="md:grid md:grid-cols-3 md:gap-8">
        
        <!-- Left: Information Column -->
        <div class="md:col-span-1 mb-6 md:mb-0">
          <div class="flex items-center gap-3 mb-3">
            <div class="w-12 h-12 rounded-2xl bg-[#00B85C]/15 border border-[#00B85C]/30 flex items-center justify-center text-[#00B85C] shadow-md shadow-[#00B85C]/10">
              <span class="material-icons text-3xl">contacts</span>
            </div>
            <div>
              <h2 class="text-2xl font-black text-white tracking-tight">Emergency</h2>
              <span class="text-xs font-bold text-[#00B85C] uppercase tracking-wider">SMS Alert Network</span>
            </div>
          </div>
          <p class="text-sm text-slate-300 leading-relaxed mt-2">
            Register trusted contacts who will automatically receive urgent SMS alerts containing your live GPS coordinates during accident or theft events.
          </p>

          <div class="mt-6 p-4 rounded-2xl bg-[#061B2B]/60 border border-white/10 space-y-2.5">
            <div class="flex items-center gap-2 text-xs font-semibold text-emerald-400">
              <span class="material-icons text-sm">bolt</span>
              <span>Cellular SMS Dispatch Ready</span>
            </div>
            <div class="flex items-center gap-2 text-xs font-semibold text-slate-300">
              <span class="material-icons text-sm text-[#00B85C]">gps_fixed</span>
              <span>Google Maps Link Included in Alerts</span>
            </div>
            <div class="flex items-center gap-2 text-xs font-semibold text-slate-300">
              <span class="material-icons text-sm text-[#00B85C]">shield</span>
              <span>Synced Directly with Motorcycle GSM</span>
            </div>
          </div>
        </div>

        <!-- Right: Add Contact Form & List -->
        <div class="mt-4 md:mt-0 md:col-span-2">
          
          <!-- Add Contact Form -->
          <form @submit.prevent="addContact" class="space-y-4 bg-[#061B2B]/60 border border-white/10 backdrop-blur-sm rounded-2xl p-5 sm:p-6 shadow-inner">
            <h3 class="text-sm font-bold text-white uppercase tracking-wider flex items-center gap-2 mb-1">
              <span class="material-icons text-base text-[#00B85C]">person_add</span>
              Add New Contact
            </h3>

            <div class="grid grid-cols-1 gap-4 sm:grid-cols-2">
              <div class="sm:col-span-1">
                <label for="name" class="block text-xs font-bold text-slate-300 uppercase tracking-wider mb-1.5 flex items-center gap-1.5">
                  <span class="material-icons text-xs text-[#00B85C]">person</span>
                  Full Name
                </label>
                <input
                  type="text"
                  id="name"
                  v-model="newContact.name"
                  class="w-full bg-[#0B2540]/60 border border-white/10 rounded-xl px-4 py-2.5 text-sm text-white placeholder-slate-500 focus:outline-none focus:border-[#00B85C] focus:ring-2 focus:ring-[#00B85C]/20 transition-all"
                  placeholder="e.g. Maria Santos"
                  required
                />
              </div>

              <div class="sm:col-span-1">
                <label for="phone" class="block text-xs font-bold text-slate-300 uppercase tracking-wider mb-1.5 flex items-center gap-1.5">
                  <span class="material-icons text-xs text-[#00B85C]">phone</span>
                  Phone Number
                </label>
                <input
                  type="tel"
                  id="phone"
                  v-model="newContact.phone"
                  class="w-full bg-[#0B2540]/60 border border-white/10 rounded-xl px-4 py-2.5 text-sm text-white placeholder-slate-500 focus:outline-none focus:border-[#00B85C] focus:ring-2 focus:ring-[#00B85C]/20 transition-all"
                  placeholder="e.g. +63 912 345 6789"
                  required
                />
              </div>

              <div class="sm:col-span-2">
                <label for="relationship" class="block text-xs font-bold text-slate-300 uppercase tracking-wider mb-1.5 flex items-center gap-1.5">
                  <span class="material-icons text-xs text-[#00B85C]">family_restroom</span>
                  Relationship
                </label>
                <input
                  type="text"
                  id="relationship"
                  v-model="newContact.relationship"
                  class="w-full bg-[#0B2540]/60 border border-white/10 rounded-xl px-4 py-2.5 text-sm text-white placeholder-slate-500 focus:outline-none focus:border-[#00B85C] focus:ring-2 focus:ring-[#00B85C]/20 transition-all"
                  placeholder="e.g. Spouse, Parent, Sibling, Friend"
                  required
                />
              </div>
            </div>

            <div class="flex justify-end pt-2">
              <button
                type="submit"
                class="inline-flex items-center gap-2 justify-center py-2.5 px-6 border border-transparent shadow-lg shadow-[#00B85C]/25 text-sm font-bold rounded-xl text-white bg-gradient-to-r from-[#00B85C] to-[#087A45] hover:from-[#00c965] hover:to-[#0a8c4f] focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-[#00B85C] transition-all transform hover:scale-[1.02] active:scale-[0.98]"
              >
                <span class="material-icons text-sm">add_circle</span>
                Save Contact
              </button>
            </div>
          </form>

          <!-- Contacts List -->
          <div class="mt-8">
            <div class="flex items-center justify-between mb-4">
              <h3 class="text-base font-bold text-white flex items-center gap-2">
                <span class="material-icons text-[#00B85C]">format_list_bulleted</span>
                <span>Your Registered Contacts</span>
              </h3>
              <span class="text-xs font-bold px-2.5 py-0.5 rounded-full bg-[#00B85C]/15 text-[#00B85C] border border-[#00B85C]/30">
                {{ contacts.length }} Active
              </span>
            </div>

            <div v-if="contacts.length > 0" class="space-y-3 max-h-96 overflow-y-auto custom-scrollbar pr-1">
              <div v-for="contact in contacts" :key="contact.id"
                class="bg-[#061B2B]/75 rounded-2xl border border-white/10 hover:border-[#00B85C]/40 transition-all duration-300 p-4 sm:p-5 shadow-lg group">
                <div class="flex items-start justify-between gap-4">
                  <div class="flex items-start gap-3.5 flex-1">
                    <div class="flex-shrink-0 bg-[#00B85C]/15 border border-[#00B85C]/30 rounded-xl p-2.5 text-[#00B85C] group-hover:scale-105 transition-transform">
                      <span class="material-icons text-2xl">person</span>
                    </div>
                    <div class="flex-1 min-w-0">
                      <h4 class="text-base font-bold text-white mb-0.5 truncate">{{ contact.name }}</h4>
                      <p class="text-xs text-[#00B85C] font-semibold mb-1.5 flex items-center gap-1">
                        <span class="material-icons text-[13px]">label</span>
                        {{ contact.relationship }}
                      </p>
                      <p class="text-xs text-slate-300 flex items-center gap-1.5 font-mono">
                        <span class="material-icons text-[13px] text-slate-400">phone</span>
                        {{ contact.phone }}
                      </p>
                    </div>
                  </div>
                  
                  <button
                    @click="deleteContact(contact.id)"
                    class="flex items-center gap-1 px-3 py-1.5 text-red-400 hover:text-white bg-red-500/10 hover:bg-red-500 border border-red-500/20 hover:border-red-500 rounded-xl transition-all duration-200 font-semibold text-xs flex-shrink-0"
                    title="Remove Contact"
                  >
                    <span class="material-icons text-xs">delete</span>
                    <span>Remove</span>
                  </button>
                </div>
              </div>
            </div>

            <div v-else class="bg-[#061B2B]/40 border border-white/5 rounded-2xl p-8 text-center">
              <span class="material-icons text-5xl text-white/20 mb-3 block">contact_phone</span>
              <p class="text-white font-bold text-base">No Emergency Contacts Registered</p>
              <p class="text-xs text-slate-400 mt-1 max-w-sm mx-auto">
                Add at least one emergency contact above so the VigiLERT system can dispatch automated alerts during emergencies.
              </p>
            </div>
          </div>

        </div>

      </div>
    </div>

  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue';
import { useAuthStore } from '../stores/auth';
import { database } from '../firebase/config';
import { ref as dbRef, onValue, set, remove, push } from 'firebase/database';

const authStore = useAuthStore();
const contacts = ref([]);
const newContact = ref({
  name: '',
  phone: '',
  relationship: ''
});

// Fetch contacts from Firebase
onMounted(() => {
  if (authStore.user) {
    const contactsRef = dbRef(database, `users/${authStore.user.uid}/emergencyContacts`);
    onValue(contactsRef, (snapshot) => {
      const data = snapshot.val();
      if (data) {
        contacts.value = Object.keys(data).map(key => ({
          id: key,
          ...data[key]
        }));
      } else {
        contacts.value = [];
      }
    });
  }
});

// Add new contact
const addContact = async () => {
  if (!authStore.user) return;

  try {
    const contactData = {
      name: newContact.value.name,
      phone: newContact.value.phone,
      relationship: newContact.value.relationship,
      createdAt: Date.now()
    };

    const contactsRef = dbRef(database, `users/${authStore.user.uid}/emergencyContacts`);
    const newContactRef = push(contactsRef);
    await set(newContactRef, contactData);

    // Also mirror to helmet_public so emergency login and hardware lookups can access seamlessly
    try {
      const publicRef = dbRef(database, `helmet_public/${authStore.user.uid}/emergencyContacts/${newContactRef.key}`);
      await set(publicRef, contactData);
    } catch (e) {
      console.warn('Could not mirror contact to helmet_public:', e);
    }

    // Reset form
    newContact.value = {
      name: '',
      phone: '',
      relationship: ''
    };
  } catch (error) {
    console.error('Error adding contact:', error);
  }
};

// Delete contact
const deleteContact = async (contactId) => {
  if (!authStore.user) return;

  try {
    const contactRef = dbRef(database, `users/${authStore.user.uid}/emergencyContacts/${contactId}`);
    await remove(contactRef);

    try {
      const publicContactRef = dbRef(database, `helmet_public/${authStore.user.uid}/emergencyContacts/${contactId}`);
      await remove(publicContactRef);
    } catch (e) {
      console.warn('Could not remove contact from helmet_public:', e);
    }
  } catch (error) {
    console.error('Error deleting contact:', error);
  }
};
</script>
