import { defineStore } from 'pinia';
import { auth } from '../firebase/config';
import { onAuthStateChanged } from 'firebase/auth';

export const useAuthStore = defineStore('auth', {
  state: () => ({
    user: null,
    loading: true
  }),
  
  getters: {
    isAuthenticated: (state) => !!state.user,
    userProfile: (state) => state.user
  },
  
  actions: {
    init() {
      onAuthStateChanged(auth, (user) => {
        this.user = user;
        this.loading = false;
      });
    },
    
    async logout() {
      try {
        await auth.signOut();
        this.user = null;
      } catch (error) {
        console.error('Error logging out:', error);
        throw error;
      }
    }
  }
}); 