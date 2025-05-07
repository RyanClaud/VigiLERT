const { initializeApp } = require('firebase/app');
const { getDatabase, ref, get, set } = require('firebase/database');

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
const db = getDatabase(app);

async function migrate() {
  const usersRef = ref(db, 'users');
  const snapshot = await get(usersRef);
  if (!snapshot.exists()) {
    console.log('No users found.');
    return;
  }
  const users = snapshot.val();
  let migratedCount = 0;
  for (const userId in users) {
    const user = users[userId];
    if (user.emergencyContacts) {
      await set(ref(db, `emergency_contacts/${userId}`), user.emergencyContacts);
      console.log(`Migrated contacts for user: ${userId}`);
      migratedCount++;
    }
  }
  if (migratedCount === 0) {
    console.log('No emergency contacts found to migrate.');
  } else {
    console.log('Migration complete!');
  }
}

migrate();