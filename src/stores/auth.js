import { defineStore } from 'pinia';
import { auth, database } from '../firebase/config';
import { onAuthStateChanged } from 'firebase/auth';
import { ref as dbRef, onValue } from 'firebase/database';

export const useAuthStore = defineStore('auth', {
  state: () => ({
    user: null,
    loading: true,
    customPhotoURL: ''
  }),
  
  getters: {
    isAuthenticated: (state) => !!state.user,
    userProfile: (state) => state.user,
    photoURL: (state) => state.customPhotoURL || state.user?.photoURL || ''
  },
  
  actions: {
    init() {
      let rtdbPhotoUnsub = null;
      onAuthStateChanged(auth, (user) => {
        this.user = user;
        this.loading = false;

        if (rtdbPhotoUnsub) {
          rtdbPhotoUnsub();
          rtdbPhotoUnsub = null;
        }

        if (user) {
          // Pre-load from localStorage for instant display
          const cachedPhoto = localStorage.getItem(`vigilert_user_photo_${user.uid}`);
          if (cachedPhoto) {
            this.customPhotoURL = cachedPhoto;
          }

          // Listen to Realtime Database for profile photo updates
          const userPhotoRef = dbRef(database, `users/${user.uid}/photoURL`);
          rtdbPhotoUnsub = onValue(userPhotoRef, (snap) => {
            const val = snap.val();
            if (val) {
              this.customPhotoURL = val;
              localStorage.setItem(`vigilert_user_photo_${user.uid}`, val);
            } else if (!this.customPhotoURL && user.photoURL) {
              this.customPhotoURL = user.photoURL;
            }
          });
        } else {
          this.customPhotoURL = '';
        }
      });
    },
    
    setPhotoURL(url) {
      this.customPhotoURL = url;
      if (this.user?.uid) {
        if (url) {
          localStorage.setItem(`vigilert_user_photo_${this.user.uid}`, url);
        } else {
          localStorage.removeItem(`vigilert_user_photo_${this.user.uid}`);
        }
      }
    },
    
    async logout() {
      try {
        await auth.signOut();
        this.user = null;
        this.customPhotoURL = '';
      } catch (error) {
        console.error('Error logging out:', error);
        throw error;
      }
    }
  }
}); 