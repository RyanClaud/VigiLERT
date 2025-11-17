<template>
  <div class="space-y-6 p-4">
    <div class="bg-gradient-to-br from-[#3D52A0] to-[#2a3a70] shadow-xl rounded-2xl px-6 py-8 sm:p-8">
      <div class="md:grid md:grid-cols-3 md:gap-8">
        <div class="md:col-span-1 mb-6 md:mb-0">
          <div class="flex items-center gap-3 mb-3">
            <span class="material-icons text-4xl text-white">contacts</span>
            <h3 class="text-2xl font-bold text-white">Emergency Contacts</h3>
          </div>
          <p class="text-sm text-white/90 leading-relaxed">
            Add and manage your emergency contacts. They will be notified during critical safety alerts and incidents.
          </p>
        </div>
        <div class="mt-5 md:mt-0 md:col-span-2">
          <!-- Add Contact Form -->
          <form @submit.prevent="addContact" class="space-y-5 bg-white/10 backdrop-blur-sm rounded-xl p-6">
            <div class="grid grid-cols-1 gap-5 sm:grid-cols-2">
              <div class="sm:col-span-1">
                <label for="name" class="block text-sm font-semibold text-white mb-2 flex items-center gap-2">
                  <span class="material-icons text-sm">person</span>
                  Name
                </label>
                <input
                  type="text"
                  id="name"
                  v-model="newContact.name"
                  class="shadow-sm focus:ring-2 focus:ring-[#7091E6] focus:border-[#7091E6] block w-full text-sm border-2 border-white/30 rounded-xl px-4 py-3 bg-white text-[#3D52A0] placeholder-gray-400 transition-all duration-300"
                  placeholder="Enter full name"
                  required
                />
              </div>

              <div class="sm:col-span-1">
                <label for="phone" class="block text-sm font-semibold text-white mb-2 flex items-center gap-2">
                  <span class="material-icons text-sm">phone</span>
                  Phone Number
                </label>
                <input
                  type="tel"
                  id="phone"
                  v-model="newContact.phone"
                  class="shadow-sm focus:ring-2 focus:ring-[#7091E6] focus:border-[#7091E6] block w-full text-sm border-2 border-white/30 rounded-xl px-4 py-3 bg-white text-[#3D52A0] placeholder-gray-400 transition-all duration-300"
                  placeholder="+1 (555) 000-0000"
                  required
                />
              </div>

              <div class="sm:col-span-2">
                <label for="relationship" class="block text-sm font-semibold text-white mb-2 flex items-center gap-2">
                  <span class="material-icons text-sm">family_restroom</span>
                  Relationship
                </label>
                <input
                  type="text"
                  id="relationship"
                  v-model="newContact.relationship"
                  class="shadow-sm focus:ring-2 focus:ring-[#7091E6] focus:border-[#7091E6] block w-full text-sm border-2 border-white/30 rounded-xl px-4 py-3 bg-white text-[#3D52A0] placeholder-gray-400 transition-all duration-300"
                  placeholder="e.g., Spouse, Parent, Friend"
                  required
                />
              </div>
            </div>

            <div class="flex justify-end pt-2">
              <button
                type="submit"
                class="inline-flex items-center gap-2 justify-center py-3 px-6 border border-transparent shadow-lg text-sm font-semibold rounded-xl text-white bg-gradient-to-r from-[#7091E6] to-[#5571c6] hover:from-[#5571c6] hover:to-[#3D52A0] focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-[#7091E6] transition-all duration-300 transform hover:scale-105"
              >
                <span class="material-icons text-sm">add_circle</span>
                Add Contact
              </button>
            </div>
          </form>

          <!-- Contacts List -->
          <div class="mt-8">
            <h4 class="text-lg font-semibold text-white mb-4 flex items-center gap-2">
              <span class="material-icons">list</span>
              Your Emergency Contacts
            </h4>
            <div v-if="contacts.length > 0" class="space-y-3 max-h-96 overflow-y-auto custom-scrollbar">
              <div v-for="contact in contacts" :key="contact.id"
                class="bg-white rounded-xl shadow-md hover:shadow-lg transition-all duration-300 p-5 border-l-4 border-[#7091E6]">
                <div class="flex items-start justify-between">
                  <div class="flex items-start gap-4 flex-1">
                    <div class="flex-shrink-0 bg-gradient-to-br from-[#7091E6] to-[#3D52A0] rounded-full p-3">
                      <span class="material-icons text-white text-2xl">account_circle</span>
                    </div>
                    <div class="flex-1">
                      <h4 class="text-lg font-semibold text-[#3D52A0] mb-1">{{ contact.name }}</h4>
                      <p class="text-sm text-[#7091E6] font-medium mb-2 flex items-center gap-1">
                        <span class="material-icons text-xs">label</span>
                        {{ contact.relationship }}
                      </p>
                      <p class="text-sm text-gray-600 flex items-center gap-1">
                        <span class="material-icons text-xs">phone</span>
                        {{ contact.phone }}
                      </p>
                    </div>
                  </div>
                  <button
                    @click="deleteContact(contact.id)"
                    class="flex items-center gap-1 px-3 py-2 text-red-600 hover:text-white hover:bg-red-600 rounded-lg transition-all duration-300 font-medium text-sm"
                  >
                    <span class="material-icons text-sm">delete</span>
                    Delete
                  </button>
                </div>
              </div>
            </div>
            <div v-else class="bg-white/10 backdrop-blur-sm rounded-xl p-8 text-center">
              <span class="material-icons text-6xl text-white/50 mb-3">person_add_disabled</span>
              <p class="text-white font-medium">No emergency contacts added yet</p>
              <p class="text-sm text-white/70 mt-1">Add your first contact using the form above</p>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue';
import { database, auth } from '../firebase/config';
import { ref as dbRef, push, remove, onValue, set } from 'firebase/database';
import { onAuthStateChanged } from 'firebase/auth';

const contacts = ref([]);
const newContact = ref({
  name: '',
  phone: '',
  relationship: ''
});
const userId = ref(null);

onAuthStateChanged(auth, (user) => {
  userId.value = user ? user.uid : null;
  if (userId.value) {
    loadContacts();
  }
});

function loadContacts() {
  const contactsRef = dbRef(database, `users/${userId.value}/emergencyContacts`);
  onValue(contactsRef, (snapshot) => {
    const data = snapshot.val();
    if (data) {
      contacts.value = Object.entries(data).map(([id, contact]) => ({
        id,
        ...contact
      })).sort((a, b) => b.createdAt - a.createdAt);
    } else {
      contacts.value = [];
    }
  });
}

const addContact = async () => {
  if (!userId.value) {
    alert('Please log in to add emergency contacts');
    return;
  }
  try {
    const contactsRef = dbRef(database, `users/${userId.value}/emergencyContacts`);
    const newContactRef = push(contactsRef);
    await set(newContactRef, {
      ...newContact.value,
      createdAt: Date.now()
    });
    newContact.value = { name: '', phone: '', relationship: '' };
    alert('Emergency contact added successfully!');
  } catch (error) {
    console.error('Error adding contact:', error);
    alert('Failed to add contact. Please try again.');
  }
};

const deleteContact = async (contactId) => {
  if (!userId.value) return;
  if (confirm('Are you sure you want to delete this contact?')) {
    try {
      const contactRef = dbRef(database, `users/${userId.value}/emergencyContacts/${contactId}`);
      await remove(contactRef);
    } catch (error) {
      console.error('Error deleting contact:', error);
      alert('Failed to delete contact. Please try again.');
    }
  }
};
</script>

<style scoped>
.custom-scrollbar::-webkit-scrollbar {
  width: 8px;
}

.custom-scrollbar::-webkit-scrollbar-track {
  background: #EDE8F5;
  border-radius: 10px;
}

.custom-scrollbar::-webkit-scrollbar-thumb {
  background: #7091E6;
  border-radius: 10px;
}

.custom-scrollbar::-webkit-scrollbar-thumb:hover {
  background: #3D52A0;
}
</style> 