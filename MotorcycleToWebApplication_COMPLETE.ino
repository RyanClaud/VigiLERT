/*
VigiLERT ESP32 Motorcycle Module - COMPLETE VERSION
✅ All sensor data sent to Firebase for real-time dashboard monitoring
✅ MPU6050 data, GPS quality, Engine status, Helmet switch, GSM signal
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <TimeLib.h>

// ======= USER CONFIG =======
const char* ssid = "DPWH";
const char* password = "12345678900";
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

// ======= PIN ASSIGNMENTS =======
const int batteryPin = 34;
// const int helmetSwitchPin = 39; // ❌ REMOVED - No physical helmet switch on motorcycle
const int headlightPin = 32;
const int taillightPin = 33;
const int leftSignalPin = 32;  // Moved from 25
const int rightSignalPin = 33;  // Moved from 26  
const int relayPin = 25;  // Changed from 27 to 25 (proven to work)
const int vibrationPin = 14;
const int buzzerPin = 12;
const int lightIndicatorPin = 13;

// GSM module pins (UART2)
#define GSM_RX 4
#define GSM_TX 5
HardwareSerial gsmSerial(2);

// GPS module pins (UART1)
HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

// MPU6050
Adafruit_MPU6050 mpu;
sensors_event_t accel, gyro, temp;

// Engine control
bool engineRunning = false;
String lastSender = "";
const String gsmPassword = "1234";
const String authorizedNumber1 = "+639675715673";
const String authorizedNumber2 = "+639542583234";

// Trip state
float currentStartLat = 0.0;
float currentStartLng = 0.0;
unsigned long tripStartTime = 0;
float maxRecordedSpeed = 0.0;

// Crash & helmet state
bool crashDetected = false;
const float ACCEL_THRESHOLD = 15.0;
bool isHelmetOn = false;
unsigned long lastHelmetOffMillis = 0;

// Safety interlock
bool isHelmetModuleActive = false;
unsigned long lastHelmetModuleCheck = 0;
const unsigned long HELMET_CHECK_INTERVAL = 5000;

// Speed limit
float currentSpeedLimit = 80.0;
unsigned long lastSpeedCheck = 0;

// ✅ NEW: GSM signal tracking
int gsmSignalStrength = 0;
unsigned long lastGSMCheck = 0;
const unsigned long GSM_CHECK_INTERVAL = 10000; // Check every 10 seconds

// ✅ NEW: Store current sensor readings for live updates
float currentRoll = 0.0;
float currentTotalAccel = 0.0;

// ✅ FIX: Crash detection based on CHANGE in acceleration (not absolute value)
float previousTotalAccel = 9.81; // Initialize with gravity
unsigned long lastCrashCheck = 0;
const unsigned long CRASH_CHECK_INTERVAL = 100; // Check every 100ms

// ✅ FIX: Prevent multiple crash markers - shorter cooldown for faster detection
unsigned long lastCrashTime = 0;
const unsigned long CRASH_COOLDOWN = 5000; // 5 seconds between crashes (reduced from 30s)

// Firebase paths
const String tripsPath = "/helmet_public/" + userUID + "/trips.json?auth=" + firebaseAuth;
const String livePath = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
const String helmetStatusPath = "/helmet_public/" + userUID + "/helmetStatus/status.json?auth=" + firebaseAuth;
const String devicesPath = "/helmet_public/" + userUID + "/devices.json?auth=" + firebaseAuth;

// ======= SETUP =======
void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n[SETUP] Starting VigiLERT System - COMPLETE VERSION...");

  Wire.begin(21, 22);
  while (!mpu.begin()) {
    Serial.println("[ERROR] Failed to find MPU6050 chip. Retrying in 2s...");
    delay(2000);
  }
  Serial.println("[SUCCESS] MPU6050 initialized.");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // ✅ Enhanced GPS initialization
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("[SETUP] GPS Serial started on RX:16, TX:17");
  
  // Wait for GPS to initialize
  delay(1000);
  
  // Clear any garbage data
  while (gpsSerial.available()) {
    gpsSerial.read();
  }
  
  Serial.println("[GPS] Waiting for GPS fix... This may take 30-60 seconds in open area.");
  Serial.println("[GPS] Make sure GPS antenna has clear view of sky.");

  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);
  Serial.println("[SETUP] GSM Serial started.");

  // pinMode(helmetSwitchPin, INPUT_PULLUP); // ❌ REMOVED - No physical helmet switch
  pinMode(headlightPin, INPUT);
  pinMode(taillightPin, INPUT);
  pinMode(leftSignalPin, INPUT);
  pinMode(rightSignalPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(vibrationPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);

  // ✅ CRITICAL: Force relay OFF at startup
  // ✅ ACTIVE-LOW RELAY: HIGH = OFF, LOW = ON
  digitalWrite(relayPin, HIGH);  // HIGH = OFF for active-low relay
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  
  // ✅ Verify relay is actually HIGH (OFF)
  delay(100);
  Serial.println("\n[SETUP] ═══════════════════════════════════");
  Serial.println("[SETUP] RELAY TYPE: ACTIVE-LOW");
  Serial.println("[SETUP] HIGH (1) = Relay OFF (Engine Blocked)");
  Serial.println("[SETUP] LOW (0) = Relay ON (Engine Allowed)");
  Serial.printf("[SETUP] Relay pin initialized to: %d (should be 1 = HIGH/OFF)\n", digitalRead(relayPin));
  Serial.println("[SETUP] Engine is BLOCKED until safety conditions met");
  Serial.println("[SETUP] ═══════════════════════════════════\n");

  connectToWiFi();
  setupOTA();
  initializeGSM();

  updateMotorcycleDeviceStatus(true);
  startNewTrip();
  currentSpeedLimit = getSpeedLimitFromFirebase();

  Serial.println("[SUCCESS] Setup complete. All sensors active.");
  Serial.println("\n📋 SERIAL COMMANDS AVAILABLE:");
  Serial.println("   Type 'CRASH' to test crash detection");
  Serial.println("   Type 'MPU' to test MPU6050 sensor");
  Serial.println("   Type 'GPS' for GPS diagnostics");
  Serial.println("   Type 'HELP' for all commands\n");
  
  // ✅ Print initial GPS diagnostics after 5 seconds
  delay(5000);
  printGPSDiagnostics();
}

// ======= MAIN LOOP =======
void loop() {
  ArduinoOTA.handle();
  
  // ✅ Serial command handler for testing
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "TEST CRASH" || cmd == "CRASH") {
      Serial.println("\n🧪 MANUAL CRASH TEST TRIGGERED!");
      Serial.println("Simulating crash event...");
      
      // Simulate crash with high acceleration
      float testAccel = 20.0; // Above threshold
      float testRoll = currentRoll;
      
      if (gps.location.isValid()) {
        Serial.printf("📍 Sending test crash WITH GPS: %.6f, %.6f\n", 
                      gps.location.lat(), gps.location.lng());
        Serial.printf("   Impact: %.2f g | Roll: %.1f°\n", testAccel, testRoll);
        sendCrashEventToFirebase(gps.location.lat(), gps.location.lng(), testAccel, testRoll);
      } else {
        Serial.println("⚠️ Sending test crash WITHOUT GPS");
        Serial.printf("   Impact: %.2f g | Roll: %.1f°\n", testAccel, testRoll);
        sendCrashEventToFirebaseNoGPS(testAccel, testRoll);
      }
      
      triggerAlert();
      Serial.println("✓ Test crash sent to Firebase!");
      Serial.println("✓ Check dashboard for crash marker and alert!");
      
      // Mark as crash detected to prevent auto-detection during test
      crashDetected = true;
    }
    else if (cmd == "MPU TEST" || cmd == "MPU") {
      Serial.println("\n🧪 MPU6050 TEST:");
      Serial.printf("Accel X: %.2f | Y: %.2f | Z: %.2f\n", 
                    accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);
      Serial.printf("Total Accel: %.2f g\n", currentTotalAccel);
      Serial.printf("Roll: %.1f°\n", currentRoll);
      Serial.printf("Crash Threshold: %.2f g\n", ACCEL_THRESHOLD);
      
      if (currentTotalAccel < 0.1) {
        Serial.println("⚠️ MPU6050 NOT WORKING! Check wiring!");
      } else {
        Serial.println("✓ MPU6050 is working!");
      }
    }
    else if (cmd == "ORIENT" || cmd == "ORIENTATION") {
      Serial.println("\n🧭 MPU6050 ORIENTATION TEST");
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.println("Tilt the motorcycle LEFT, RIGHT, FORWARD, BACKWARD");
      Serial.println("Watch which values change the most:");
      Serial.println("");
      
      for (int i = 0; i < 20; i++) {
        mpu.getEvent(&accel, &gyro, &temp);
        
        float totalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                               accel.acceleration.y * accel.acceleration.y +
                               accel.acceleration.z * accel.acceleration.z);
        
        float roll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
        float pitch = atan2(-accel.acceleration.x, sqrt(accel.acceleration.y * accel.acceleration.y + 
                                                        accel.acceleration.z * accel.acceleration.z)) * 180.0 / PI;
        
        Serial.printf("X: %6.2f | Y: %6.2f | Z: %6.2f | Roll: %6.1f° | Pitch: %6.1f° | Total: %.2f\n",
                      accel.acceleration.x, accel.acceleration.y, accel.acceleration.z,
                      roll, pitch, totalAccel);
        delay(500);
      }
      
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.println("Which axis changed the most when tilting LEFT/RIGHT?");
      Serial.println("That's the axis we should use for lean detection!");
    }
    else if (cmd == "HELP") {
      Serial.println("\n📋 AVAILABLE COMMANDS:");
      Serial.println("  CRASH or TEST CRASH - Trigger test crash event");
      Serial.println("  MPU or MPU TEST     - Test MPU6050 sensor");
      Serial.println("  ORIENT              - Test sensor orientation");
      Serial.println("  GPS                 - Show GPS diagnostics");
      Serial.println("  HELP                - Show this help");
    }
    else if (cmd == "GPS") {
      printGPSDiagnostics();
    }
  }
  
  // ✅ Enhanced GPS reading with debugging
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
    
    // Debug: Print raw GPS data every 5 seconds
    static unsigned long lastGPSDebug = 0;
    if (millis() - lastGPSDebug > 5000) {
      Serial.print("[GPS RAW] ");
      Serial.print(c);
      if (c == '\n') {
        lastGPSDebug = millis();
      }
    }
  }
  
  handleGSM();

  // Sensor readings
  // bool helmetSwitchState = (digitalRead(helmetSwitchPin) == LOW); // ❌ REMOVED
  bool headlightOn = digitalRead(headlightPin);
  bool taillightOn = digitalRead(taillightPin);
  bool leftSignalOn = digitalRead(leftSignalPin);
  bool rightSignalOn = digitalRead(rightSignalPin);
  float batteryVoltage = analogRead(batteryPin) * (3.3f / 4095.0f) * 4.3;

  // MPU6050 data
  mpu.getEvent(&accel, &gyro, &temp);
  currentTotalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                           accel.acceleration.y * accel.acceleration.y +
                           accel.acceleration.z * accel.acceleration.z);
  
  // ✅ FIX: Calculate ROLL (left/right lean) for motorcycle crash detection
  // Roll = rotation around X-axis (left/right lean)
  // Pitch = rotation around Y-axis (forward/backward tilt)
  currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
  float currentPitch = atan2(-accel.acceleration.x, sqrt(accel.acceleration.y * accel.acceleration.y + 
                                                          accel.acceleration.z * accel.acceleration.z)) * 180.0 / PI;
  
  // ✅ For motorcycle: Use absolute roll angle (left or right lean)
  float leanAngle = abs(currentRoll);

  // ✅ Periodic GSM signal check
  if (millis() - lastGSMCheck > GSM_CHECK_INTERVAL) {
    gsmSignalStrength = getGSMSignalStrength();
    lastGSMCheck = millis();
  }

  // Periodic helmet module check
  if (millis() - lastHelmetModuleCheck > HELMET_CHECK_INTERVAL) {
    checkHelmetModuleStatus();
    lastHelmetModuleCheck = millis();
  }

  // ✅ SAFETY INTERLOCK: Continuously enforce safety conditions
  // If engine is running but safety conditions are no longer met, shut it down
  if (engineRunning && !canStartEngine()) {
    Serial.println("\n⚠️⚠️⚠️ SAFETY VIOLATION DETECTED! ⚠️⚠️⚠️");
    Serial.println("Safety conditions no longer met - shutting down engine!");
    stopEngine();
  }
  
  // ✅ SAFETY INTERLOCK: Aggressively keep relay OFF if safety conditions not met
  // This prevents manual bypass of the relay
  static unsigned long lastSafetyCheck = 0;
  if (millis() - lastSafetyCheck > 100) { // Check every 100ms
    if (!engineRunning) {
      // ✅ ACTIVE-LOW: Force relay HIGH (OFF) when engine not running
      digitalWrite(relayPin, HIGH);
      
      // Debug output every 5 seconds
      static unsigned long lastDebug = 0;
      if (millis() - lastDebug > 5000) {
        Serial.println("\n[SAFETY] Forcing relay HIGH (OFF) - engine not running");
        Serial.printf("[SAFETY] Relay pin state: %d (should be 1 = HIGH/OFF)\n", digitalRead(relayPin));
        Serial.println("[SAFETY] ACTIVE-LOW relay: HIGH=OFF, LOW=ON");
        lastDebug = millis();
      }
      
      // Blink LED to indicate system is locked
      if (!canStartEngine()) {
        static unsigned long lastBlink = 0;
        if (millis() - lastBlink > 500) {
          digitalWrite(lightIndicatorPin, !digitalRead(lightIndicatorPin));
          lastBlink = millis();
        }
      } else {
        // Solid LED when ready to start
        digitalWrite(lightIndicatorPin, HIGH);
      }
    }
    lastSafetyCheck = millis();
  }

  // ✅ REAL-TIME: Ultra-fast heartbeat for real-time status monitoring
  static unsigned long lastMotorcycleHeartbeat = 0;
  const unsigned long MOTORCYCLE_HEARTBEAT_INTERVAL = 1000; // 1 second for ultra-fast updates
  if (millis() - lastMotorcycleHeartbeat > MOTORCYCLE_HEARTBEAT_INTERVAL) {
    updateMotorcycleDeviceStatus(true);
    lastMotorcycleHeartbeat = millis();
  }

  // ✅ Periodic GPS diagnostics (every 30 seconds)
  static unsigned long lastGPSDiag = 0;
  if (millis() - lastGPSDiag > 30000) {
    printGPSDiagnostics();
    lastGPSDiag = millis();
  }

  // ✅ Enhanced Serial monitoring with detailed GPS info and SAFETY STATUS
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 1000) {
    Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.printf("[SENSOR] Accel: %.2f | Lean: %.1f° | Pitch: %.1f° | Battery: %.2fV",
                  currentTotalAccel, leanAngle, currentPitch, batteryVoltage);
    
    // Detailed GPS information
    if (gps.location.isValid()) {
      Serial.printf(" | GPS: ✓ VALID");
      Serial.printf("\n[GPS] Lat: %.6f | Lng: %.6f | Speed: %.1f km/h",
                    gps.location.lat(), gps.location.lng(), gps.speed.kmph());
      Serial.printf(" | Sats: %d | HDOP: %.1f",
                    gps.satellites.value(), gps.hdop.hdop());
      if (gps.date.isValid() && gps.time.isValid()) {
        Serial.printf(" | Time: %02d:%02d:%02d",
                      gps.time.hour(), gps.time.minute(), gps.time.second());
      }
    } else {
      Serial.printf(" | GPS: ✗ NO FIX");
      Serial.printf("\n[GPS] Chars: %d | Sentences: %d | Failed: %d",
                    gps.charsProcessed(), gps.sentencesWithFix(), gps.failedChecksum());
      if (gps.charsProcessed() < 10) {
        Serial.print(" | ⚠️ NO DATA FROM GPS MODULE!");
      }
    }
    
    Serial.printf(" | GSM: %d%% | Engine: %s", 
                  gsmSignalStrength, engineRunning ? "RUNNING" : "STOPPED");
    
    // ✅ SAFETY STATUS DISPLAY (Firebase Communication Only)
    Serial.println("\n[SAFETY STATUS - Firebase Communication]");
    Serial.printf("  Helmet Module Active: %s (via Firebase)\n", isHelmetModuleActive ? "✓ YES" : "✗ NO");
    Serial.printf("  Alcohol Status: %s (via Firebase)\n", isAlcoholSafe() ? "✓ SAFE" : "✗ DANGER");
    Serial.printf("  Relay State: %s (ACTIVE-LOW)\n", digitalRead(relayPin) ? "HIGH (Engine Blocked)" : "LOW (Engine Allowed)");
    Serial.printf("  Can Start Engine: %s\n", canStartEngine() ? "✓ YES" : "✗ NO");
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    
    lastPrint = millis();
  }

  // ✅ MPU6050 Diagnostics - Check if sensor is working
  static unsigned long lastMPUCheck = 0;
  if (millis() - lastMPUCheck > 10000) { // Every 10 seconds
    if (currentTotalAccel < 0.1) {
      Serial.println("\n⚠️ WARNING: MPU6050 reading 0.00! Sensor may not be working!");
      Serial.println("   Check: 1) MPU6050 wiring (SDA:21, SCL:22)");
      Serial.println("          2) MPU6050 power (3.3V)");
      Serial.println("          3) I2C address (0x68 or 0x69)");
    }
    lastMPUCheck = millis();
  }

  // ✅ SIMPLE CRASH DETECTION: Based on total acceleration threshold (WORKING VERSION)
  // Detect when total acceleration exceeds threshold (indicates impact or severe tilt)
  if ((currentTotalAccel >= ACCEL_THRESHOLD || currentRoll < -47 || currentRoll > 40) && !crashDetected) {
    
    // ✅ FIX: Check cooldown period to prevent multiple crash markers
    unsigned long timeSinceLastCrash = millis() - lastCrashTime;
    if (timeSinceLastCrash < CRASH_COOLDOWN) {
      Serial.printf("[CRASH] Cooldown active - %lu seconds remaining\n", 
                    (CRASH_COOLDOWN - timeSinceLastCrash) / 1000);
      // Skip this crash detection
    } else {
      Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
      Serial.printf("Impact: %.2f g | Roll: %.1f° | Threshold: %.2f g\n", 
                    currentTotalAccel, currentRoll, ACCEL_THRESHOLD);
      
      // 🚨🚨🚨 IMMEDIATE RELAY SHUTDOWN - HIGHEST PRIORITY! 🚨🚨🚨
      Serial.println("\n🚨 EMERGENCY SHUTDOWN - Cutting relay power NOW!");
      digitalWrite(relayPin, HIGH);  // ✅ ACTIVE-LOW: HIGH = OFF
      engineRunning = false;         // Update engine state
      delay(100);                    // Give relay time to respond
      Serial.printf("🚨 Relay GPIO %d forced to: %d (should be 1 = HIGH/OFF)\n", 
                    relayPin, digitalRead(relayPin));
      Serial.println("🚨 Engine power CUT - relay LEDs (DS1/DS2) should turn OFF!");
      
      // ✅ Send crash event to Firebase (ONLY ONCE due to cooldown)
      if (gps.location.isValid()) {
        Serial.printf("📍 Sending crash WITH GPS: %.6f, %.6f\n", 
                      gps.location.lat(), gps.location.lng());
        sendCrashEventToFirebase(gps.location.lat(), gps.location.lng(), currentTotalAccel, currentRoll);
      } else {
        Serial.println("⚠️ Sending crash WITHOUT GPS");
        sendCrashEventToFirebaseNoGPS(currentTotalAccel, currentRoll);
      }
      
      crashDetected = true;
      lastCrashTime = millis(); // ✅ Record crash time for cooldown
      triggerAlert();
      
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.println("✓ CRASH EVENT SENT TO FIREBASE!");
      Serial.println("✓ RELAY FORCED OFF IMMEDIATELY!");
      Serial.println("✓ Cooldown active for 5 seconds");
      Serial.println("✓ CHECK DASHBOARD FOR:");
      Serial.println("  • Red crash marker on map");
      Serial.println("  • Crash alert notification");
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    }
  }

  // ✅ FIX: Clear crash state when bike returns to normal position
  // This allows detection of new crashes
  if (crashDetected) {
    // Check if bike is back to normal (upright and stable)
    bool isUpright = (currentRoll > -30 && currentRoll < 30);
    bool isStable = (currentTotalAccel < ACCEL_THRESHOLD - 2.0); // Below threshold with margin
    
    if (isUpright && isStable) {
      crashDetected = false;
      Serial.println("[INFO] ✓ Crash state cleared - bike upright and stable.");
      Serial.println("[INFO] Ready to detect new crashes.");
    }
  }

  // Unauthorized movement
  if (!engineRunning && digitalRead(vibrationPin) == HIGH) {
    Serial.println("[SECURITY] Unauthorized movement!");
    triggerAlert();
    sendAlertSMS();
  }

  // Helmet state & trip management (now uses Firebase helmet module status)
  handleHelmetState(isHelmetModuleActive, batteryVoltage, headlightOn, taillightOn, leftSignalOn, rightSignalOn);

  // ✅ REAL-TIME: Ultra-fast Firebase live updates
  static unsigned long lastFirebaseUpdate = 0;
  const unsigned long FIREBASE_UPDATE_INTERVAL = 300; // Update every 300ms (3.3x per second)
  
  if (millis() - lastFirebaseUpdate >= FIREBASE_UPDATE_INTERVAL) {
    lastFirebaseUpdate = millis();
    
    if (gps.location.isValid()) {
      float speedKmph = gps.speed.kmph();
      sendLiveToFirebase(gps.location.lat(), gps.location.lng(), speedKmph, batteryVoltage,
                         headlightOn, taillightOn, leftSignalOn, rightSignalOn, isHelmetModuleActive);
      if (speedKmph > maxRecordedSpeed) maxRecordedSpeed = speedKmph;
    } else {
      sendLiveToFirebaseNoGPS(batteryVoltage, headlightOn, taillightOn, leftSignalOn, rightSignalOn, isHelmetModuleActive);
    }
  }

  // Periodic speed limit update
  if (millis() - lastSpeedCheck > 60000) {
    currentSpeedLimit = getSpeedLimitFromFirebase();
    lastSpeedCheck = millis();
  }

  // ✅ REAL-TIME: Ultra-minimal delay for fastest updates (20ms)
  delay(20);
}

// ======= ✅ NEW: GPS DIAGNOSTICS =======
void printGPSDiagnostics() {
  Serial.println("\n========== GPS DIAGNOSTICS ==========");
  Serial.printf("GPS Module Status: %s\n", gpsSerial.available() > 0 ? "RECEIVING DATA" : "NO DATA");
  Serial.printf("Characters Processed: %d\n", gps.charsProcessed());
  Serial.printf("Sentences with Fix: %d\n", gps.sentencesWithFix());
  Serial.printf("Failed Checksums: %d\n", gps.failedChecksum());
  
  Serial.println("\n--- Location ---");
  Serial.printf("Valid: %s\n", gps.location.isValid() ? "YES" : "NO");
  if (gps.location.isValid()) {
    Serial.printf("Latitude: %.6f\n", gps.location.lat());
    Serial.printf("Longitude: %.6f\n", gps.location.lng());
    Serial.printf("Age: %d ms\n", gps.location.age());
  }
  
  Serial.println("\n--- Satellites ---");
  Serial.printf("Valid: %s\n", gps.satellites.isValid() ? "YES" : "NO");
  if (gps.satellites.isValid()) {
    Serial.printf("Count: %d\n", gps.satellites.value());
  }
  
  Serial.println("\n--- HDOP (Accuracy) ---");
  Serial.printf("Valid: %s\n", gps.hdop.isValid() ? "YES" : "NO");
  if (gps.hdop.isValid()) {
    Serial.printf("HDOP: %.1f (Lower is better, <5 is good)\n", gps.hdop.hdop());
  }
  
  Serial.println("\n--- Speed ---");
  Serial.printf("Valid: %s\n", gps.speed.isValid() ? "YES" : "NO");
  if (gps.speed.isValid()) {
    Serial.printf("Speed: %.1f km/h\n", gps.speed.kmph());
  }
  
  Serial.println("\n--- Date/Time ---");
  if (gps.date.isValid() && gps.time.isValid()) {
    Serial.printf("Date: %04d-%02d-%02d\n", gps.date.year(), gps.date.month(), gps.date.day());
    Serial.printf("Time: %02d:%02d:%02d UTC\n", gps.time.hour(), gps.time.minute(), gps.time.second());
  } else {
    Serial.println("Date/Time: INVALID");
  }
  
  Serial.println("\n--- Troubleshooting ---");
  if (gps.charsProcessed() < 10) {
    Serial.println("⚠️ PROBLEM: GPS module not sending data!");
    Serial.println("   Check: 1) GPS RX/TX wiring (RX:16, TX:17)");
    Serial.println("          2) GPS power supply (3.3V or 5V)");
    Serial.println("          3) GPS antenna connection");
  } else if (!gps.location.isValid()) {
    Serial.println("⚠️ PROBLEM: GPS receiving data but no fix!");
    Serial.println("   Check: 1) Move to open area with clear sky view");
    Serial.println("          2) Wait 30-60 seconds for initial fix");
    Serial.println("          3) Check GPS antenna is connected");
  } else {
    Serial.println("✓ GPS is working correctly!");
  }
  Serial.println("=====================================\n");
}

// ======= ✅ NEW: GET GSM SIGNAL STRENGTH =======
int getGSMSignalStrength() {
  gsmSerial.println("AT+CSQ");
  delay(500);
  
  String response = "";
  unsigned long timeout = millis() + 1000;
  while (millis() < timeout && gsmSerial.available()) {
    response += (char)gsmSerial.read();
  }
  
  // Parse response: +CSQ: <rssi>,<ber>
  int csqIndex = response.indexOf("+CSQ:");
  if (csqIndex != -1) {
    int commaIndex = response.indexOf(",", csqIndex);
    if (commaIndex != -1) {
      String rssiStr = response.substring(csqIndex + 6, commaIndex);
      rssiStr.trim();
      int rssi = rssiStr.toInt();
      
      // Convert RSSI to percentage (0-31 range, 99 = no signal)
      if (rssi == 99 || rssi == 0) return 0;
      return (rssi * 100) / 31;
    }
  }
  
  return 0; // No signal or parse error
}

// ======= ✅ ENHANCED: SEND LIVE DATA WITH ALL SENSORS =======
// ✅ FIXED: Parameter renamed from helmetSwitch to helmetModuleActive (Firebase communication only)
void sendLiveToFirebase(float lat, float lng, float speed, float v, bool h, bool t, bool l, bool r, bool helmetModuleActive) {
  StaticJsonDocument<1024> doc; // Increased size for more data
  
  // GPS data
  doc["locationLat"] = lat;
  doc["locationLng"] = lng;
  doc["speed"] = speed;
  
  // Battery
  doc["batteryVoltage"] = v;
  
  // Lights
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["brakeLight"] = t; // Use taillight as brake light (or add separate sensor)
  
  // ✅ NEW: MPU6050 data
  doc["mpu6050"]["accelX"] = accel.acceleration.x;
  doc["mpu6050"]["accelY"] = accel.acceleration.y;
  doc["mpu6050"]["accelZ"] = accel.acceleration.z;
  doc["mpu6050"]["gyro"] = currentRoll;
  doc["mpu6050"]["totalAccel"] = currentTotalAccel;
  
  // ✅ NEW: GPS quality
  if (gps.hdop.isValid()) {
    doc["gps"]["accuracy"] = String(gps.hdop.hdop()) + "m";
  } else {
    doc["gps"]["accuracy"] = "N/A";
  }
  doc["gps"]["satellites"] = gps.satellites.isValid() ? gps.satellites.value() : 0;
  
  // ✅ NEW: GSM data
  doc["gsm"]["signal"] = String(gsmSignalStrength) + "%";
  doc["gsm"]["network"] = gsmSignalStrength > 0 ? "4G" : "Offline";
  
  // ✅ NEW: Engine status
  doc["engineRunning"] = engineRunning;
  
  // ✅ FIXED: Helmet module status from Firebase (not physical switch)
  doc["helmetModuleActive"] = helmetModuleActive;
  
  // Timestamp
  doc["timestamp"] = millis();
  doc["source"] = "gps_module";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);
  
  if (code != HTTP_CODE_OK) {
    Serial.printf("[FIREBASE] Live update failed: %d\n", code);
  }
  
  http.end();
}

// ✅ ENHANCED: SEND LIVE DATA WITHOUT GPS
// ✅ FIXED: Parameter renamed from helmetSwitch to helmetModuleActive (Firebase communication only)
void sendLiveToFirebaseNoGPS(float v, bool h, bool t, bool l, bool r, bool helmetModuleActive) {
  StaticJsonDocument<1024> doc;
  
  // Battery
  doc["batteryVoltage"] = v;
  
  // Lights
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["brakeLight"] = t;
  
  // ✅ NEW: MPU6050 data
  doc["mpu6050"]["accelX"] = accel.acceleration.x;
  doc["mpu6050"]["accelY"] = accel.acceleration.y;
  doc["mpu6050"]["accelZ"] = accel.acceleration.z;
  doc["mpu6050"]["gyro"] = currentRoll;
  doc["mpu6050"]["totalAccel"] = currentTotalAccel;
  
  // ✅ NEW: GPS quality (no GPS)
  doc["gps"]["accuracy"] = "No GPS";
  doc["gps"]["satellites"] = 0;
  
  // ✅ NEW: GSM data
  doc["gsm"]["signal"] = String(gsmSignalStrength) + "%";
  doc["gsm"]["network"] = gsmSignalStrength > 0 ? "4G" : "Offline";
  
  // ✅ NEW: Engine status
  doc["engineRunning"] = engineRunning;
  
  // ✅ FIXED: Helmet module status from Firebase (not physical switch)
  doc["helmetModuleActive"] = helmetModuleActive;
  
  // Timestamp
  doc["timestamp"] = millis();
  doc["source"] = "no_gps";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.PUT(payload);
  http.end();
}

// ======= REST OF THE FUNCTIONS (unchanged from original) =======
// ... (keep all other functions as they were)

unsigned long gpsToUnixTime() {
  if (gps.time.isValid() && gps.date.isValid()) {
    tmElements_t tm;
    tm.Year = gps.date.year() - 1970;
    tm.Month = gps.date.month();
    tm.Day = gps.date.day();
    tm.Hour = gps.time.hour();
    tm.Minute = gps.time.minute();
    tm.Second = gps.time.second();
    return makeTime(tm);
  }
  return millis() / 1000UL;
}

void checkHelmetModuleStatus() {
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
  
  Serial.println("\n[HELMET CHECK] ═══════════════════════════════");
  Serial.println("[HELMET CHECK] Fetching helmet status from Firebase...");
  Serial.println("[HELMET CHECK] URL: " + url);
  
  http.begin(url);
  int code = http.GET();
  
  Serial.printf("[HELMET CHECK] HTTP Response Code: %d\n", code);
  
  if (code == HTTP_CODE_OK) {
    String response = http.getString();
    Serial.println("[HELMET CHECK] Raw Firebase Response:");
    Serial.println(response);
    
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, response);
    
    if (!error) {
      String status = doc["status"] | "Off";
      unsigned long lastHeartbeat = doc["lastHeartbeat"] | 0;
      
      // ✅ FIXED: Use millis-based timestamp instead of GPS time
      // Both modules use: 1700000000000UL + millis()
      unsigned long now = 1700000000000UL + millis();
      
      // Calculate time difference
      unsigned long timeSinceHeartbeat;
      if (now > lastHeartbeat) {
        timeSinceHeartbeat = now - lastHeartbeat;
      } else {
        timeSinceHeartbeat = 999999; // Very old if timestamp is in future
      }
      
      // Helmet is active if status is "On" AND heartbeat is recent (< 15 seconds)
      isHelmetModuleActive = (status == "On" && timeSinceHeartbeat < 15000);
      
      // Enhanced debug output
      Serial.println("[HELMET CHECK] ─────────────────────────────────");
      Serial.printf("[HELMET CHECK] Status Field: '%s'\n", status.c_str());
      Serial.printf("[HELMET CHECK] Last Heartbeat: %lu\n", lastHeartbeat);
      Serial.printf("[HELMET CHECK] Current Time: %lu\n", now);
      Serial.printf("[HELMET CHECK] Time Difference: %lu ms (%.1f seconds)\n", 
                    timeSinceHeartbeat, timeSinceHeartbeat / 1000.0);
      Serial.printf("[HELMET CHECK] Threshold: 15000 ms (15 seconds)\n");
      Serial.printf("[HELMET CHECK] Status Check: %s\n", status == "On" ? "PASS" : "FAIL");
      Serial.printf("[HELMET CHECK] Heartbeat Check: %s\n", timeSinceHeartbeat < 15000 ? "PASS" : "FAIL");
      Serial.printf("[HELMET CHECK] Final Result: %s\n", isHelmetModuleActive ? "✓ ACTIVE" : "✗ INACTIVE");
      Serial.println("[HELMET CHECK] ═══════════════════════════════\n");
    } else {
      Serial.println("[HELMET CHECK] ✗ JSON Parse Error!");
      Serial.printf("[HELMET CHECK] Error: %s\n", error.c_str());
      isHelmetModuleActive = false;
    }
  } else {
    Serial.printf("[HELMET CHECK] ✗ Firebase GET failed: %d\n", code);
    Serial.println("[HELMET CHECK] Check WiFi connection and Firebase URL");
    isHelmetModuleActive = false;
  }
  
  http.end();
}

void updateMotorcycleDeviceStatus(bool isOn) {
  // ✅ FIX: Use a base Unix timestamp + millis for consistent heartbeat
  // This ensures the timestamp is always increasing and comparable
  unsigned long timestamp = 1700000000000UL + millis();
  
  StaticJsonDocument<128> doc;
  doc["status"] = isOn ? "On" : "Off";
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/motorcycle.json?auth=" + firebaseAuth;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.printf("[FIREBASE] Motorcycle heartbeat sent: %lu\n", timestamp);
  } else {
    Serial.printf("[FIREBASE] Motorcycle heartbeat failed: %d\n", code);
  }
  
  http.end();
}

// ✅ SAFETY FEATURE 2: Check alcohol status from Firebase
bool isAlcoholSafe() {
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;
  http.begin(url);
  int code = http.GET();
  
  if (code == HTTP_CODE_OK) {
    String response = http.getString();
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, response);
    
    if (!error) {
      String status = doc["status"] | "Safe";
      http.end();
      return (status == "Safe");
    }
  }
  http.end();
  return true; // Default to safe if can't read
}

// ✅ COMPREHENSIVE SAFETY CHECK: All 3 safety features using single relay
// ✅ FIXED: Only uses Firebase communication - no physical helmet switch
bool canStartEngine() {
  // SAFETY FEATURE 1: Both devices must be ON (via Firebase heartbeat)
  bool helmetModuleOn = isHelmetModuleActive;
  bool motorcycleOn = true;
  
  // SAFETY FEATURE 2: No alcohol detected (via Firebase)
  bool alcoholSafe = isAlcoholSafe();
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   SAFETY CHECK - Engine Start Request ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.printf("║ %s Motorcycle Module: %-15s ║\n", motorcycleOn ? "✓" : "✗", motorcycleOn ? "ON" : "OFF");
  Serial.printf("║ %s Helmet Module: %-19s ║\n", helmetModuleOn ? "✓" : "✗", helmetModuleOn ? "ON" : "OFF");
  Serial.printf("║ %s Alcohol Test: %-20s ║\n", alcoholSafe ? "✓" : "✗", alcoholSafe ? "SAFE" : "DANGER");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ Communication: Firebase Only           ║");
  Serial.println("╚════════════════════════════════════════╝");
  
  if (!motorcycleOn) Serial.println("  ❌ BLOCKED: Motorcycle module is OFF");
  if (!helmetModuleOn) Serial.println("  ❌ BLOCKED: Helmet module is OFF (no heartbeat)");
  if (!alcoholSafe) Serial.println("  ❌ BLOCKED: Alcohol detected - unsafe to ride!");
  
  return (motorcycleOn && helmetModuleOn && alcoholSafe);
}

void startEngine() {
  if (!canStartEngine()) {
    Serial.println("\n❌❌❌ [SAFETY] ENGINE START BLOCKED! ❌❌❌");
    Serial.println("All safety requirements must be met:");
    Serial.println("  1. Both devices must be ON (via Firebase heartbeat)");
    Serial.println("  2. No alcohol detected (via Firebase status)");
    Serial.println("  3. Communication: Firebase only - no physical connections\n");
    return;
  }
  
  // ✅ ACTIVE-LOW: LOW = ON
  digitalWrite(relayPin, LOW);
  engineRunning = true;
  sendSMS("Engine Started");
  Serial.println("\n✅ [ENGINE] Started successfully!");
  Serial.println("✅ All safety checks passed");
  Serial.println("✅ Relay set to LOW (ON for active-low relay)\n");
}

void stopEngine() {
  // ✅ ACTIVE-LOW: HIGH = OFF
  digitalWrite(relayPin, HIGH);
  engineRunning = false;
  sendSMS("Engine Stopped");
  Serial.println("[ENGINE] Stopped.");
  Serial.println("[ENGINE] Relay set to HIGH (OFF for active-low relay)");
}

// ======= HELPER FUNCTIONS =======
void updateHelmetStatusInFirebase(bool helmetConnected, const String& alertness, float alcoholLevel) {
  StaticJsonDocument<256> doc;
  doc["helmetConnected"] = helmetConnected;
  doc["alertnessStatus"] = alertness;
  doc["alcoholLevel"] = alcoholLevel;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + helmetStatusPath);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);
  
  if (code == 200) {
    Serial.printf("\n[FIREBASE] Helmet status updated: %s\n", helmetConnected ? "Connected" : "Disconnected");
  }
  http.end();
}

void handleGSM() {
  while (gsmSerial.available()) {
    String response = gsmSerial.readString();
    if (response.length() > 0) {
      Serial.println("[GSM] " + response);
    }
    if (response.indexOf("RING") != -1) handleIncomingCall(response);
    if (response.indexOf("+CMT:") != -1) handleIncomingSMS(response);
  }
}

bool initializeGSM() {
  gsmSerial.println("AT"); 
  delay(1000);
  gsmSerial.println("AT+CMGF=1"); 
  delay(1000);
  gsmSerial.println("AT+CNMI=1,2,0,0,0"); 
  delay(1000);
  return true;
}

void handleIncomingCall(String response) {
  int start = response.indexOf("\"") + 1;
  int end = response.indexOf("\"", start);
  String caller = response.substring(start, end);
  
  if (caller != authorizedNumber1 && caller != authorizedNumber2) {
    gsmSerial.println("ATH");
    return;
  }
  
  if (engineRunning) stopEngine();
  else startEngine();
  
  gsmSerial.println("ATH");
}

void handleIncomingSMS(String response) {
  int start = response.indexOf("\"") + 1;
  int end = response.indexOf("\"", start);
  String sender = response.substring(start, end);
  sender.trim();
  
  if (sender != authorizedNumber1 && sender != authorizedNumber2) return;
  
  lastSender = sender;
  
  int bodyStart = response.indexOf("\r\n", end) + 2;
  String smsBody = response.substring(bodyStart);
  smsBody.trim();
  
  if (smsBody.startsWith(gsmPassword + " ")) {
    String command = smsBody.substring(gsmPassword.length() + 1);
    command.trim();
    command.toUpperCase();
    
    if (command == "START") startEngine();
    else if (command == "STOP") stopEngine();
    else if (command == "LOCATE") {
      if (gps.location.isValid()) {
        String locMsg = "GPS Lat: " + String(gps.location.lat(), 6) + " Lng: " + String(gps.location.lng(), 6);
        sendSMS(locMsg);
      } else sendSMS("GPS location not available.");
    }
    else if (command == "STATUS") {
      String statusMsg = "Helmet Module: ";
      statusMsg += isHelmetModuleActive ? "ON" : "OFF";
      statusMsg += " | Engine: ";
      statusMsg += engineRunning ? "RUNNING" : "STOPPED";
      statusMsg += " | Alcohol: ";
      statusMsg += isAlcoholSafe() ? "SAFE" : "DANGER";
      sendSMS(statusMsg);
    }
  }
}

void sendSMS(String msg) {
  gsmSerial.print("AT+CMGS=\"" + lastSender + "\"\r");
  delay(1000);
  if (gsmSerial.find(">")) {
    gsmSerial.print(msg);
    delay(100);
    gsmSerial.write(26);
  }
}

void triggerAlert() {
  int count = 0;
  bool ledState = false;
  unsigned long lastBlink = millis(), lastTone = millis();
  
  while (count < 15) {
    unsigned long now = millis();
    
    if (now - lastBlink >= 300) {
      ledState = !ledState;
      digitalWrite(lightIndicatorPin, ledState);
      lastBlink = now;
    }
    
    if (now - lastTone < 200) tone(buzzerPin, 1500);
    else if (now - lastTone < 400) tone(buzzerPin, 1000);
    else {
      lastTone = now;
      count++;
    }
  }
  
  noTone(buzzerPin);
  digitalWrite(lightIndicatorPin, LOW);
}

void sendAlertSMS() {
  sendSMS("ALERT! Unauthorized movement detected!");
}

void sendCrashEventToFirebase(float lat, float lng, float impact, float leanAngle) {
  Serial.println("\n[FIREBASE] Preparing crash event WITH GPS...");
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = gpsToUnixTime() * 1000UL;
  doc["lat"] = lat;
  doc["lng"] = lng;
  doc["hasGPS"] = true;
  doc["impactStrength"] = impact;
  doc["roll"] = leanAngle;  // Lean angle (left/right)
  doc["type"] = "crash";    // Mark as crash event
  
  String payload;
  serializeJson(doc, payload);
  
  Serial.println("[FIREBASE] Crash payload:");
  Serial.println(payload);
  Serial.println("[FIREBASE] Sending to: " + firebaseHost + crashPath);
  
  HTTPClient http;
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  int code = http.POST(payload);
  
  Serial.printf("[FIREBASE] HTTP Response Code: %d\n", code);
  
  if (code == HTTP_CODE_OK || code == HTTP_CODE_CREATED) {
    Serial.println("[FIREBASE] ✓✓✓ CRASH SENT SUCCESSFULLY! ✓✓✓");
    Serial.println("[FIREBASE] Dashboard should now show:");
    Serial.println("           • Red marker at GPS location");
    Serial.println("           • Crash alert notification");
    Serial.println("           • Impact and lean angle details");
  } else {
    Serial.printf("[FIREBASE] ✗✗✗ CRASH SEND FAILED! ✗✗✗\n");
    Serial.printf("[FIREBASE] Error code: %d\n", code);
    Serial.println("[FIREBASE] Check WiFi connection and Firebase URL");
  }
  
  http.end();
}

void sendCrashEventToFirebaseNoGPS(float impact, float leanAngle) {
  Serial.println("\n[FIREBASE] Preparing crash event WITHOUT GPS...");
  Serial.println("⚠️ No GPS fix - crash will be recorded but won't show on map");
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = gpsToUnixTime() * 1000UL;
  doc["hasGPS"] = false;
  doc["impactStrength"] = impact;
  doc["roll"] = leanAngle;  // Lean angle (left/right)
  doc["type"] = "crash";    // Mark as crash event
  
  String payload;
  serializeJson(doc, payload);
  
  Serial.println("[FIREBASE] Crash payload:");
  Serial.println(payload);
  
  HTTPClient http;
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  int code = http.POST(payload);
  
  Serial.printf("[FIREBASE] HTTP Response Code: %d\n", code);
  
  if (code == HTTP_CODE_OK || code == HTTP_CODE_CREATED) {
    Serial.println("[FIREBASE] ✓ Crash sent successfully!");
    Serial.println("⚠️ Crash recorded but won't show on map (no GPS coordinates)");
    Serial.println("   Dashboard will show crash in alerts list");
  } else {
    Serial.printf("[FIREBASE] ✗ Crash send failed! HTTP: %d\n", code);
  }
  
  http.end();
}

float getSpeedLimitFromFirebase() {
  HTTPClient http;
  http.begin(firebaseHost + "/helmet_public/" + userUID + "/settings/speedLimit.json?auth=" + firebaseAuth);
  int code = http.GET();
  
  float limit = 80.0;
  if (code == HTTP_CODE_OK) {
    String res = http.getString();
    limit = res.toFloat();
  }
  
  http.end();
  return limit;
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected IP: " + String(WiFi.localIP().toString()));
}

void setupOTA() {
  ArduinoOTA.setHostname("HelmetSafetySystem");
  ArduinoOTA.setPassword("password");
  ArduinoOTA.begin();
}

void startNewTrip() {
  tripStartTime = millis();
  currentStartLat = gps.location.isValid() ? gps.location.lat() : 0.0;
  currentStartLng = gps.location.isValid() ? gps.location.lng() : 0.0;
  maxRecordedSpeed = 0;
}

void endCurrentTrip(float endLat, float endLng, float speed, float voltage, bool h, bool t, bool l, bool r, unsigned long startT, unsigned long endT) {
  StaticJsonDocument<512> doc;
  doc["startLat"] = currentStartLat;
  doc["startLng"] = currentStartLng;
  doc["endLat"] = endLat;
  doc["endLng"] = endLng;
  doc["startTime"] = startT;
  doc["endTime"] = endT;
  doc["duration"] = (endT - startT) / 60000.0;
  doc["maxSpeed"] = speed;
  doc["distance"] = sqrt(pow(endLat - currentStartLat, 2) + pow(endLng - currentStartLng, 2)) * 111.0 * 1000;
  doc["batteryVoltage"] = voltage;
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["status"] = "completed";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + tripsPath);
  http.addHeader("Content-Type", "application/json");
  http.POST(payload);
  http.end();
}

void handleHelmetState(bool currentHelmetState, float voltage, bool h, bool t, bool l, bool r) {
  if (currentHelmetState && !isHelmetOn) {
    startNewTrip();
    isHelmetOn = true;
    crashDetected = false;
    // ✅ Removed: Only helmet module should update helmetStatus to avoid conflicts
    // updateHelmetStatusInFirebase(true, "Normal", 0.0);
    Serial.println("[HELMET] Helmet PUT ON");
  }
  
  if (!currentHelmetState && isHelmetOn) {
    unsigned long endTime = millis();
    endCurrentTrip(
      gps.location.isValid() ? gps.location.lat() : currentStartLat,
      gps.location.isValid() ? gps.location.lng() : currentStartLng,
      gps.speed.kmph(), voltage, h, t, l, r,
      tripStartTime, endTime
    );
    
    isHelmetOn = false;
    lastHelmetOffMillis = millis();
    crashDetected = false;
    // ✅ Removed: Only helmet module should update helmetStatus to avoid conflicts
    // updateHelmetStatusInFirebase(false, "Normal", 0.0);
    Serial.println("[HELMET] Helmet REMOVED");
    
    // Stop engine if helmet is removed while running
    if (engineRunning) {
      Serial.println("[SAFETY] Helmet removed - stopping engine!");
      stopEngine();
    }
  }
}
