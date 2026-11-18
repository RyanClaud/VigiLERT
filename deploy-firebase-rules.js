// Deploy Firebase Rules Script
// Run this to update your Firebase database rules

const { exec } = require('child_process');

console.log('🔥 Deploying Firebase Database Rules...\n');

// Deploy database rules
exec('firebase deploy --only database', (error, stdout, stderr) => {
  if (error) {
    console.error('❌ Deployment failed:', error);
    console.error('Make sure you have Firebase CLI installed and are logged in:');
    console.error('  npm install -g firebase-tools');
    console.error('  firebase login');
    console.error('  firebase use --add (select your project)');
    return;
  }

  if (stderr) {
    console.error('⚠️ Warnings:', stderr);
  }

  console.log('✅ Firebase rules deployed successfully!');
  console.log(stdout);
  
  console.log('\n📋 Updated Rules Summary:');
  console.log('✅ helmet_public/{uid} - Full read/write access (for Arduino)');
  console.log('✅ {uid}/engineControl - Full read/write access (for dashboard buttons)');
  console.log('✅ {uid}/alcohol - Full read/write access (for helmet data)');
  console.log('✅ {uid}/antiTheft - Full read/write access (for security features)');
  
  console.log('\n🧪 Test your Arduino now - it should connect successfully!');
});

// Alternative: Manual deployment instructions
console.log('💡 Manual Deployment:');
console.log('1. Run: firebase deploy --only database');
console.log('2. Or go to Firebase Console → Database → Rules');
console.log('3. Copy the rules from database.rules.json');
console.log('4. Click "Publish"\n');