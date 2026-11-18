// VigiLERT Engine Control Test Script
// This script tests the engine control functionality between dashboard and Arduino

const admin = require('firebase-admin');

// Initialize Firebase Admin (you'll need to add your service account key)
// const serviceAccount = require('./path-to-your-service-account-key.json');
// admin.initializeApp({
//   credential: admin.credential.cert(serviceAccount),
//   databaseURL: 'https://vigilance-shield-default-rtdb.firebaseio.com'
// });

const db = admin.database();
const userUID = 'MnzBjTBslZNijOkq732PE91hHa23';

async function testEngineControl() {
  console.log('🧪 Testing Engine Control System...\n');
  
  try {
    // Test 1: Check current engine status
    console.log('📊 Test 1: Reading current engine status...');
    
    const paths = [
      `helmet_public/${userUID}/live`,
      `helmet_public/${userUID}/devices/motorcycle`,
      `${userUID}/engineControl`
    ];
    
    for (const path of paths) {
      try {
        const snapshot = await db.ref(path).once('value');
        const data = snapshot.val();
        console.log(`   ${path}:`, data?.engineRunning || 'No engine data');
      } catch (error) {
        console.log(`   ${path}: Error -`, error.message);
      }
    }
    
    // Test 2: Simulate dashboard button press
    console.log('\n🖱️ Test 2: Simulating dashboard button press...');
    await db.ref(`${userUID}/engineControl/startButton`).set(true);
    console.log('   ✅ Button press sent to Firebase');
    
    // Wait for Arduino to process
    await new Promise(resolve => setTimeout(resolve, 2000));
    
    // Check if button was cleared
    const buttonSnapshot = await db.ref(`${userUID}/engineControl/startButton`).once('value');
    const buttonState = buttonSnapshot.val();
    console.log('   Button state after 2s:', buttonState);
    
    // Test 3: Monitor live data updates
    console.log('\n📡 Test 3: Monitoring live data for 10 seconds...');
    
    const liveRef = db.ref(`helmet_public/${userUID}/live`);
    let updateCount = 0;
    
    const listener = liveRef.on('value', (snapshot) => {
      const data = snapshot.val();
      updateCount++;
      
      if (data) {
        console.log(`   Update ${updateCount}:`, {
          engineRunning: data.engineRunning,
          timestamp: new Date(data.timestamp).toLocaleTimeString(),
          mpu6050: data.mpu6050 ? `${data.mpu6050.totalAccel?.toFixed(2)}g` : 'No MPU data',
          gps: data.gpsValid ? `${data.speed?.toFixed(1)}km/h` : 'No GPS',
          sensors: {
            crash: data.crashDetected,
            alcohol: data.alcoholDetected,
            antiTheft: data.antiTheftArmed
          }
        });
      }
    });
    
    // Monitor for 10 seconds
    await new Promise(resolve => setTimeout(resolve, 10000));
    liveRef.off('value', listener);
    
    console.log(`\n📈 Received ${updateCount} live data updates`);
    
    // Test 4: Test manual sync
    console.log('\n🔄 Test 4: Testing manual status sync...');
    
    const finalSnapshot = await db.ref(`helmet_public/${userUID}/live`).once('value');
    const finalData = finalSnapshot.val();
    
    if (finalData) {
      console.log('   Final engine status:', finalData.engineRunning ? 'RUNNING' : 'STOPPED');
      console.log('   Last update:', new Date(finalData.timestamp).toLocaleString());
      console.log('   Sensor data available:', {
        mpu6050: !!finalData.mpu6050,
        gps: !!finalData.gpsValid,
        location: !!(finalData.locationLat && finalData.locationLng)
      });
    }
    
    console.log('\n✅ Engine control test completed!');
    console.log('\n💡 If you see regular updates and the button gets cleared, the system is working correctly.');
    
  } catch (error) {
    console.error('❌ Test failed:', error);
  }
  
  process.exit(0);
}

// Run the test
if (require.main === module) {
  console.log('⚠️  Note: You need to initialize Firebase Admin with your service account key');
  console.log('⚠️  Uncomment the Firebase initialization code at the top of this file\n');
  
  // Uncomment the line below when you've set up Firebase Admin
  // testEngineControl();
}

module.exports = { testEngineControl };