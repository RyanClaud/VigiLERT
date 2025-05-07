<template>
  <div class="space-y-6">
    <div class="bg-[#3D52A0] shadow px-4 py-5 sm:rounded-lg sm:p-6">
      <div class="md:grid md:grid-cols-3 md:gap-6">
        <div class="md:col-span-1">
          <h3 class="text-lg font-medium leading-6 text-white">Emergency Contacts</h3>
          <p class="mt-1 text-sm text-white">
            <br>
            <br>
              
            Manage your emergency contacts for alerts and notifications.
          </p>
        </div>
        <div class="mt-5 md:mt-0 md:col-span-2">
          <!-- Add Contact Form -->
          <form @submit.prevent="addContact" class="space-y-6">
            <div class="grid grid-cols-1 gap-y-6 gap-x-4 sm:grid-cols-6">
              <div class="sm:col-span-3">
                <label for="name" class="block text-sm font-medium text-white">Name</label>
                <div class="mt-1">
                  <input
                    type="text"
                    id="name"
                    v-model="newContact.name"
                    class="shadow-sm focus:ring-[#7091E6] focus:border-[#7091E6] block w-full sm:text-sm border-[#ADBBD4] rounded-lg px-4 py-3 bg-white text-[#3D52A0] placeholder-[#ADBBD4] transition"
                    required
                  />
                </div>
              </div>

              <div class="sm:col-span-3">
                <label for="phone" class="block text-sm font-medium text-white">Phone Number</label>
                <div class="mt-1">
                  <input
                    type="tel"
                    id="phone"
                    v-model="newContact.phone"
                    class="shadow-sm focus:ring-[#7091E6] focus:border-[#7091E6] block w-full sm:text-sm border-[#ADBBD4] rounded-lg px-4 py-3 bg-white text-[#3D52A0] placeholder-[#ADBBD4] transition"
                    required
                  />
                </div>
              </div>

              <div class="sm:col-span-6">
                  <label for="relationship" class="block text-sm font-medium text-white">Relationship</label>
                <div class="mt-1">
                  <input
                    type="text"
                    id="relationship"
                    v-model="newContact.relationship"
                    class="shadow-sm focus:ring-[#7091E6] focus:border-[#7091E6] block w-full sm:text-sm border-[#ADBBD4] rounded-lg px-4 py-3 bg-white text-[#3D52A0] placeholder-[#ADBBD4] transition"
                    required
                  />
                </div>
              </div>
            </div>

            <div class="flex justify-end">
              <button
                type="submit"
                class="ml-3 inline-flex justify-center py-2 px-4 border border-transparent shadow-sm text-sm font-medium rounded-md text-white bg-blue-600 hover:bg-blue-700 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-blue-500"
              >
                Add Contact
              </button>
            </div>
          </form>

          <!-- Contacts List -->
          <div class="mt-8">
            <h4 class="text-lg font-medium text-[#3D52A0]">Your Emergency Contacts</h4>
            <div class="mt-4 bg-[#EDE8F5] shadow overflow-hidden sm:rounded-md">
              <ul role="list" class="divide-y divide-[#5e80bb]">
                <li v-for="contact in contacts" :key="contact.id">
                  <div class="px-4 py-4 sm:px-6">
                    <div class="flex items-center justify-between">
                      <div class="flex items-center">
                        <div class="flex-shrink-0">
                          <svg class="h-6 w-6 text-[#7091E6]" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M16 7a4 4 0 11-8 0 4 4 0 018 0zM12 14a7 7 0 00-7 7h14a7 7 0 00-7-7z" />
                          </svg>
                        </div>
                        <div class="ml-4">
                          <h4 class="text-lg font-medium text-[#3D52A0]">{{ contact.name }}</h4>
                          <p class="text-sm text-[#7091E6]">{{ contact.relationship }}</p>
                        </div>
                      </div>
                      <div class="ml-2 flex-shrink-0 flex">
                        <button
                          @click="deleteContact(contact.id)"
                          class="flex items-center gap-1 text-red-600 hover:text-red-900"
                        >
                          <svg class="h-5 w-5" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12" /></svg>
                          Delete
                        </button>
                      </div>
                    </div>
                    <div class="mt-2">
                      <p class="text-sm text-[#7091E6]">
                        Phone: {{ contact.phone }}
                      </p>
                    </div>
                  </div>
                </li>
              </ul>
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