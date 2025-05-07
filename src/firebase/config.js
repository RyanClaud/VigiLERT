import { initializeApp } from 'firebase/app';
import { getAuth } from 'firebase/auth';
import { getDatabase } from 'firebase/database';

const firebaseConfig = {
  apiKey: "AIzaSyCF6b0rG26cqljudVDw13eH9Q9FyAyzVfc",
  authDomain: "vigilance-shield.firebaseapp.com",
  databaseURL: "https://vigilance-shield-default-rtdb.firebaseio.com",
  projectId: "vigilance-shield",
  storageBucket: "vigilance-shield.firebasestorage.app",
  messagingSenderId: "771024278683",
  appId: "1:771024278683:web:5dffc3102fa3b1a67ee23e"
};

const app = initializeApp(firebaseConfig);
export const auth = getAuth(app);
export const database = getDatabase(app); 