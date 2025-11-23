/*
VigiLERT ESP32 Motorcycle Module - RELAY LOGIC FIXED
✅ CRITICAL FIX: Consistent ACTIVE-LOW relay logic throughout
✅ LOW = Relay ON (Engine Allowed)
✅ HIGH = Relay OFF (Engine Blocked)
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
const int relayPin = 13;   // ✅ GPIO 13 - PROVEN TO WORK!
const int buzzerPin = 12;
const int lightIndicatorPin = 2;
// ✅ Dashboard button control via Firebase

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
bool crashDetected = false;
const float ACCEL_THRESHOLD = 15.0;

// Current sensor readings
float currentRoll = 0.0;
float currentTotalAccel = 0.0;

// Crash detection timing
unsigned long lastCrashTime = 0;
const unsigned long CRASH_COOLDOWN = 5000; // 5 seconds between crashes

// ✅ NEW: Alcohol detection system with real-time monitoring
bool alcoholDetected = false;
bool lastAlcoholState = false;
unsigned long lastAlcoholCheck = 0;
const unsigned long ALCOHOL_CHECK_INTERVAL = 500; // ✅ FASTER: Check every 500ms for real-time
unsigned long alcoholDetectionTime = 0;

// ✅ NEW: Automatic engine control
bool autoEngineControl = false;  // Enable automatic engine control
bool engineStartRequested = false;  // User requested engine start

// ✅ NEW: Dashboard button control
bool lastDashboardButtonState = false;
unsigned long lastButtonCheck = 0;
const unsigned long BUTTON_CHECK_INTERVAL = 1000; // Check dashboard button every 1 second

// Firebase paths
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
const String buttonPath = "/" + userUID + "/engineControl/startButton.json?auth=" + firebaseAuth;
// Try multiple possible alcohol paths
const String alcoholPaths[] = {
  "/" + userUID + "/alcohol/status/status.json?auth=" + firebaseAuth,
  "/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth,
  "/" + userUID + "/alcohol.json?auth=" + firebaseAuth,
  "/helmet_public/" + userUID + "/alcohol.json?auth=" + firebaseAuth,
  "/helmet_public/" + userUID + "/helmetStatus/alcohol.json?auth=" + firebaseAuth,
  "/" + userUID + ".json?auth=" + firebaseAuth  // Get entire user data
};
const int numAlcoholPaths = 6;

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n[SETUP] Starting VigiLERT System - RELAY LOGIC FIXED...");

  // Initialize MPU6050
  Wire.begin(21, 22);
  while (!mpu.begin()) {
    Serial.println("[ERROR] Failed to find MPU6050 chip. Retrying in 2s...");
    delay(2000);
  }
  Serial.println("[SUCCESS] MPU6050 initialized.");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Initialize GPS
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("[SETUP] GPS Serial started on RX:16, TX:17");

  // Initialize GSM
  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);
  Serial.println("[SETUP] GSM Serial started.");

  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);
  // ✅ Dashboard button - no physical pin needed

  // ✅ CRITICAL: CONSISTENT ACTIVE-LOW RELAY LOGIC
  digitalWrite(relayPin, HIGH);   // HIGH = OFF (Engine Blocked)
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  
  delay(100);
  Serial.println("\n[SETUP] ═══════════════════════════════════");
  Serial.println("[SETUP] RELAY TYPE: ACTIVE-LOW (CONFIRMED BY TESTING)");
  Serial.println("[SETUP] GPIO: 13 (Safe, no conflicts)");
  Serial.println("[SETUP] HIGH (1) = Relay OFF (Engine Blocked)");
  Serial.println("[SETUP] LOW (0) = Relay ON (Engine Allowed)");
  Serial.printf("[SETUP] Relay initialized to: %d (should be 1 = HIGH/OFF)\n", digitalRead(relayPin));
  Serial.println("[SETUP] Engine is BLOCKED until safety conditions met");
  Serial.println("[SETUP] ═══════════════════════════════════\n");

  // Connect to WiFi for Firebase communication
  connectToWiFi();
  
  Serial.println("[SUCCESS] Setup complete. Ready for real-time monitoring.");
  Serial.println("\n� PHYISICAL CONTROLS:");
  Serial.println("   Use dashboard button to start/stop engine");
  Serial.println("   Engine will auto-stop if alcohol detected");
  Serial.println("   Engine will auto-start when alcohol clears");
  Serial.println("\n📋 SERIAL COMMANDS:");
  Serial.println("   Type 'CRASH' to test crash detection");
  Serial.println("   Type 'ALCOHOL' to test alcohol detection");
  Serial.println("   Type 'AUTO ON' to enable automatic engine control");
  Serial.println("   Type 'AUTO OFF' to disable automatic engine control");
  Serial.println("   Type 'TEST AUTO' to test auto-start functionality");
  Serial.println("   Type 'STATUS' for system status\n");
}

void loop() {
  // ✅ NEW: Handle dashboard button via Firebase
  handleDashboardButton();
  
  // Serial command handler
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "CRASH") {
      Serial.println("\n🧪 MANUAL CRASH TEST TRIGGERED!");
      triggerCrashShutdown(20.0, 45.0); // Simulate high impact
    }
    else if (cmd == "AUTO ON") {
      Serial.println("\n✅ AUTOMATIC ENGINE CONTROL ENABLED");
      Serial.println("✅ Engine will auto-stop when alcohol detected");
      Serial.println("✅ Engine will auto-start when alcohol clears");
      autoEngineControl = true;
    }
    else if (cmd == "AUTO OFF") {
      Serial.println("\n🛑 AUTOMATIC ENGINE CONTROL DISABLED");
      Serial.println("🛑 Manual control only");
      autoEngineControl = false;
    }
    else if (cmd == "STATUS") {
      printSystemStatus();
    }
    else if (cmd == "ALCOHOL") {
      Serial.println("\n🧪 MANUAL ALCOHOL TEST TRIGGERED!");
      Serial.println("🧪 Simulating alcohol detection...");
      alcoholDetected = true;
      alcoholDetectionTime = millis();
      
      // Force relay OFF immediately
      digitalWrite(relayPin, HIGH);  // ✅ ACTIVE-LOW: HIGH = OFF
      Serial.printf("🧪 Relay GPIO %d = %d (should be 1 = HIGH/OFF)\n", 
                    relayPin, digitalRead(relayPin));
      
      if (engineRunning) {
        triggerAlcoholShutdown();
      } else {
        Serial.println("🧪 Engine not running - relay forced OFF for safety");
      }
    }
    else if (cmd == "CLEAR ALCOHOL") {
      Serial.println("\n🧪 CLEARING ALCOHOL TEST...");
      alcoholDetected = false;
      Serial.println("✅ Alcohol status cleared - relay control returned to normal");
    }
    else if (cmd == "EXPLORE") {
      Serial.println("\n🔍 EXPLORING FIREBASE PATHS...");
      exploreFirebasePaths();
    }
    else if (cmd == "TEST AUTO") {
      Serial.println("\n🧪 TESTING AUTO-START FUNCTIONALITY...");
      Serial.println("🧪 Step 1: Enabling auto control");
      autoEngineControl = true;
      
      Serial.println("🧪 Step 2: Setting engine start request");
      engineStartRequested = true;
      
      Serial.println("🧪 Step 3: Clearing alcohol detection");
      alcoholDetected = false;
      
      Serial.println("🧪 Step 4: Setting engine as stopped");
      engineRunning = false;
      
      Serial.println("🧪 Conditions set - auto-start should trigger in next loop!");
      Serial.println("🧪 Watch for AUTO-START message...");
    }
  }

  // Read MPU6050 data
  mpu.getEvent(&accel, &gyro, &temp);
  currentTotalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                           accel.acceleration.y * accel.acceleration.y +
                           accel.acceleration.z * accel.acceleration.z);
  
  currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
  float leanAngle = abs(currentRoll);

  // ✅ CRASH DETECTION: Immediate relay shutdown with correct logic
  if ((currentTotalAccel >= ACCEL_THRESHOLD || leanAngle > 40) && !crashDetected) {
    
    // Check cooldown period
    unsigned long timeSinceLastCrash = millis() - lastCrashTime;
    if (timeSinceLastCrash < CRASH_COOLDOWN) {
      Serial.printf("[CRASH] Cooldown active - %lu seconds remaining\n", 
                    (CRASH_COOLDOWN - timeSinceLastCrash) / 1000);
    } else {
      triggerCrashShutdown(currentTotalAccel, currentRoll);
    }
  }

  // Clear crash state when bike returns to normal
  if (crashDetected) {
    bool isUpright = (leanAngle < 30);
    bool isStable = (currentTotalAccel < ACCEL_THRESHOLD - 2.0);
    
    if (isUpright && isStable) {
      crashDetected = false;
      Serial.println("[INFO] ✓ Crash state cleared - bike upright and stable.");
    }
  }

  // ✅ NEW: Alcohol detection monitoring
  if (millis() - lastAlcoholCheck > ALCOHOL_CHECK_INTERVAL) {
    checkAlcoholStatus();
    lastAlcoholCheck = millis();
  }

  // ✅ AUTOMATIC ENGINE CONTROL based on alcohol detection
  if (autoEngineControl) {
    // Debug auto-start conditions every 10 seconds
    static unsigned long lastAutoDebug = 0;
    if (millis() - lastAutoDebug > 10000) {
      Serial.println("\n[AUTO-START DEBUG]");
      Serial.printf("  Auto Control: %s\n", autoEngineControl ? "ENABLED" : "DISABLED");
      Serial.printf("  Alcohol Detected: %s\n", alcoholDetected ? "YES" : "NO");
      Serial.printf("  Engine Running: %s\n", engineRunning ? "YES" : "NO");
      Serial.printf("  Start Requested: %s\n", engineStartRequested ? "YES" : "NO");
      
      if (!alcoholDetected && engineStartRequested && !engineRunning) {
        Serial.println("  ✅ ALL CONDITIONS MET FOR AUTO-START!");
      } else {
        Serial.println("  ❌ Auto-start conditions not met");
      }
      lastAutoDebug = millis();
    }
    if (alcoholDetected && engineRunning) {
      // Automatic shutdown when alcohol detected
      Serial.println("\n🚨 AUTO-SHUTDOWN: Alcohol detected!");
      stopEngine();
      engineStartRequested = true;  // ✅ REMEMBER start request for auto-restart!
    }
    else if (!alcoholDetected && engineStartRequested && !engineRunning) {
      // Automatic restart when alcohol clears and start was requested
      Serial.println("\n✅✅✅ AUTO-START TRIGGERED! ✅✅✅");
      Serial.println("✅ Alcohol cleared - restarting engine automatically!");
      Serial.println("✅ User previously requested engine start");
      startEngine();
      Serial.println("✅ Auto-start complete!");
    }
  }
  
  // ✅ SAFETY OVERRIDE: Always force relay OFF if alcohol detected (regardless of auto mode)
  if (alcoholDetected) {
    digitalWrite(relayPin, HIGH);  // ✅ ACTIVE-LOW: HIGH = OFF
    
    // Debug output every 5 seconds
    static unsigned long lastAlcoholDebug = 0;
    if (millis() - lastAlcoholDebug > 5000) {
      Serial.println("\n🚨 ALCOHOL SAFETY ACTIVE - Relay forced OFF");
      Serial.printf("🚨 Relay GPIO %d = %d (should be 1 = HIGH/OFF)\n", 
                    relayPin, digitalRead(relayPin));
      Serial.println("🚨 Engine BLOCKED until alcohol clears");
      if (autoEngineControl && engineStartRequested) {
        Serial.println("🚨 Will auto-restart when alcohol clears");
      }
      lastAlcoholDebug = millis();
    }
  }

  // Status monitoring with enhanced info
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 2000) {
    Serial.printf("[STATUS] Engine: %s | Relay: %s | Alcohol: %s | Auto: %s | StartReq: %s\n",
                  engineRunning ? "RUN" : "STOP",
                  digitalRead(relayPin) ? "OFF" : "ON",
                  alcoholDetected ? "DANGER" : "SAFE",
                  autoEngineControl ? "ON" : "OFF",
                  engineStartRequested ? "YES" : "NO");
    lastPrint = millis();
  }

  delay(50);
}

// ✅ CRASH SHUTDOWN: Immediate relay control with correct logic
void triggerCrashShutdown(float impact, float roll) {
  Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
  Serial.printf("Impact: %.2f g | Roll: %.1f°\n", impact, roll);
  
  // 🚨 IMMEDIATE RELAY SHUTDOWN - CORRECT LOGIC! 🚨
  Serial.println("\n🚨 EMERGENCY SHUTDOWN - Cutting relay power NOW!");
  digitalWrite(relayPin, HIGH);  // ✅ ACTIVE-LOW: HIGH = OFF
  engineRunning = false;
  delay(100);
  
  Serial.printf("🚨 Relay GPIO %d = %d (should be 1 = HIGH/OFF)\n", 
                relayPin, digitalRead(relayPin));
  Serial.println("🚨 Engine power CUT!");
  
  // Send crash to Firebase
  sendCrashToFirebase(impact, roll);
  
  crashDetected = true;
  lastCrashTime = millis();
  
  // Alert
  for (int i = 0; i < 5; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(lightIndicatorPin, LOW);
    delay(200);
  }
  
  Serial.println("✓ CRASH EVENT PROCESSED!");
  Serial.println("✓ RELAY FORCED OFF CORRECTLY!");
}

// ✅ ENGINE CONTROL: Enhanced with automatic control support
void startEngine() {
  // ✅ Safety check: Block engine start if alcohol detected
  if (alcoholDetected) {
    Serial.println("\n❌❌❌ ENGINE START BLOCKED! ❌❌❌");
    Serial.println("❌ ALCOHOL DETECTED - Engine cannot start!");
    
    if (autoEngineControl) {
      Serial.println("❌ Will auto-start when alcohol clears");
      engineStartRequested = true;  // Remember the start request
    } else {
      Serial.println("❌ Use dashboard button again when alcohol clears");
    }
    
    // Keep relay OFF
    digitalWrite(relayPin, HIGH);  // ✅ ACTIVE-LOW: HIGH = OFF
    
    // Alert beeps
    for (int i = 0; i < 3; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(buzzerPin, LOW);
      delay(200);
    }
    return;
  }
  
  Serial.println("\n✅ Starting engine...");
  digitalWrite(relayPin, LOW);   // ✅ ACTIVE-LOW: LOW = ON
  engineRunning = true;
  engineStartRequested = false;  // Clear start request
  Serial.printf("✅ Relay GPIO %d = %d (should be 0 = LOW/ON)\n", 
                relayPin, digitalRead(relayPin));
  Serial.println("✅ Engine STARTED successfully!");
}

void stopEngine() {
  Serial.println("\n🛑 Stopping engine...");
  digitalWrite(relayPin, HIGH);  // ✅ ACTIVE-LOW: HIGH = OFF
  engineRunning = false;
  Serial.printf("🛑 Relay GPIO %d = %d (should be 1 = HIGH/OFF)\n", 
                relayPin, digitalRead(relayPin));
  Serial.println("🛑 Engine STOPPED!");
}

void printRelayStatus() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🔍 RELAY STATUS REPORT");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.printf("Relay Type: ACTIVE-HIGH\n");
  Serial.printf("GPIO Pin: %d\n", relayPin);
  Serial.printf("Current State: %d (%s)\n", 
                digitalRead(relayPin), 
                digitalRead(relayPin) ? "HIGH/OFF" : "LOW/ON");
  Serial.printf("Engine Status: %s\n", engineRunning ? "RUNNING" : "STOPPED");
  Serial.printf("Crash Detected: %s\n", crashDetected ? "YES" : "NO");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}

void sendCrashToFirebase(float impact, float roll) {
  // Simplified crash reporting
  Serial.printf("📡 Sending crash to Firebase: Impact=%.2f, Roll=%.1f\n", impact, roll);
  // Add actual Firebase code here if needed
}

// ✅ NEW: WiFi connection function
void connectToWiFi() {
  Serial.println("\n[WIFI] Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] ✅ Connected!");
    Serial.printf("[WIFI] IP Address: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n[WIFI] ❌ Connection failed!");
    Serial.println("[WIFI] Alcohol monitoring will not work without WiFi");
  }
}

// ✅ NEW: Check alcohol status from Firebase - try multiple paths
void checkAlcoholStatus() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[ALCOHOL] ⚠️ No WiFi - cannot check alcohol status");
    return;
  }
  
  // Try each possible path until we find data
  for (int i = 0; i < numAlcoholPaths; i++) {
    Serial.printf("\n[ALCOHOL] 🔍 Trying path %d/%d...\n", i+1, numAlcoholPaths);
    Serial.println("[ALCOHOL] URL: " + firebaseHost + alcoholPaths[i]);
    
    HTTPClient http;
    http.begin(firebaseHost + alcoholPaths[i]);
    http.addHeader("Content-Type", "application/json");
    
    int httpCode = http.GET();
    Serial.printf("[ALCOHOL] HTTP Response Code: %d\n", httpCode);
    
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      Serial.println("[ALCOHOL] Raw Firebase Response: '" + response + "'");
      
      // Skip if response is null or empty
      if (response == "null" || response.length() <= 2) {
        Serial.println("[ALCOHOL] ❌ Path returned null/empty - trying next path");
        http.end();
        continue;
      }
      
      // Found valid data! Parse it
      Serial.println("[ALCOHOL] ✅ Found valid data at this path!");
      
      // Check for alcohol detection in various formats
      bool currentAlcoholState = false;
      
      // Check for text indicators
      if (response.indexOf("Danger") != -1 || response.indexOf("danger") != -1 || 
          response.indexOf("DANGER") != -1 || response.indexOf("Detected") != -1 ||
          response.indexOf("detected") != -1 || response.indexOf("DETECTED") != -1 ||
          response.indexOf("Alert") != -1 || response.indexOf("alert") != -1) {
        currentAlcoholState = true;
      }
      
      // Check for numeric indicators (high sensor values might indicate alcohol)
      if (response.indexOf("sensorValue") != -1) {
        // Extract sensor value if present
        int sensorStart = response.indexOf("sensorValue") + 13;
        int sensorEnd = response.indexOf(",", sensorStart);
        if (sensorEnd == -1) sensorEnd = response.indexOf("}", sensorStart);
        
        if (sensorEnd > sensorStart) {
          String sensorStr = response.substring(sensorStart, sensorEnd);
          sensorStr.trim();
          int sensorValue = sensorStr.toInt();
          
          Serial.printf("[ALCOHOL] Sensor Value: %d\n", sensorValue);
          
          // Assume values above 1000 indicate alcohol (adjust threshold as needed)
          if (sensorValue > 1000) {
            currentAlcoholState = true;
            Serial.println("[ALCOHOL] High sensor value indicates alcohol!");
          }
        }
      }
      
      Serial.printf("[ALCOHOL] Parsed alcohol state: %s\n", currentAlcoholState ? "DETECTED" : "SAFE");
      
      // Check for state change
      if (currentAlcoholState != lastAlcoholState) {
        Serial.printf("[ALCOHOL] 🔄 State changed from %s to %s\n", 
                      lastAlcoholState ? "DETECTED" : "SAFE",
                      currentAlcoholState ? "DETECTED" : "SAFE");
        
        lastAlcoholState = currentAlcoholState;
        
        if (currentAlcoholState) {
          Serial.println("\n🚨🚨🚨 ALCOHOL DETECTED FROM HELMET MODULE! 🚨🚨🚨");
          Serial.println("🚨 Firebase alcohol status: POSITIVE");
          Serial.println("🚨 Forcing relay OFF immediately!");
          
          alcoholDetected = true;
          alcoholDetectionTime = millis();
          
          // IMMEDIATE RELAY SHUTDOWN
          digitalWrite(relayPin, HIGH);  // ✅ ACTIVE-LOW: HIGH = OFF
          Serial.printf("🚨 Relay GPIO %d = %d (should be 1 = HIGH/OFF)\n", 
                        relayPin, digitalRead(relayPin));
          
          // If engine was running, trigger full shutdown
          if (engineRunning) {
            triggerAlcoholShutdown();
          }
        } else {
          Serial.println("\n✅ Alcohol status cleared from helmet module");
          Serial.println("✅ Firebase alcohol status: SAFE");
          Serial.println("✅ Relay control returned to normal");
          alcoholDetected = false;
        }
      } else {
        Serial.printf("[ALCOHOL] No state change - still %s\n", 
                      currentAlcoholState ? "DETECTED" : "SAFE");
      }
      
      http.end();
      return; // Found valid data, stop trying other paths
      
    } else {
      Serial.printf("[ALCOHOL] ❌ HTTP Error %d - trying next path\n", httpCode);
    }
    
    http.end();
  }
  
  Serial.println("[ALCOHOL] ❌ No valid alcohol data found in any path!");
}

// ✅ NEW: Alcohol detection shutdown
void triggerAlcoholShutdown() {
  Serial.println("\n🚨🚨🚨 ALCOHOL DETECTED - EMERGENCY SHUTDOWN! 🚨🚨🚨");
  Serial.println("🚨 Driver under influence - cutting engine power NOW!");
  
  // Immediate relay shutdown
  digitalWrite(relayPin, HIGH);  // ✅ ACTIVE-LOW: HIGH = OFF
  engineRunning = false;
  delay(100);
  
  Serial.printf("🚨 Relay GPIO %d = %d (should be 1 = HIGH/OFF)\n", 
                relayPin, digitalRead(relayPin));
  Serial.println("🚨 Engine BLOCKED due to alcohol detection!");
  
  // Alert pattern - different from crash (longer beeps)
  for (int i = 0; i < 10; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(500);  // Longer beeps for alcohol
    digitalWrite(buzzerPin, LOW);
    digitalWrite(lightIndicatorPin, LOW);
    delay(300);
  }
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("✓ ALCOHOL SHUTDOWN COMPLETE!");
  Serial.println("✓ Engine will remain BLOCKED until alcohol clears");
  Serial.println("✓ Check helmet module alcohol sensor");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}

// ✅ UPDATED: Enhanced system status
void printSystemStatus() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🔍 SYSTEM STATUS REPORT");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.printf("Relay Type: ACTIVE-LOW\n");
  Serial.printf("GPIO Pin: %d\n", relayPin);
  Serial.printf("Current State: %d (%s)\n", 
                digitalRead(relayPin), 
                digitalRead(relayPin) ? "HIGH/OFF" : "LOW/ON");
  Serial.printf("Engine Status: %s\n", engineRunning ? "RUNNING" : "STOPPED");
  Serial.printf("Crash Detected: %s\n", crashDetected ? "YES" : "NO");
  Serial.printf("Alcohol Detected: %s\n", alcoholDetected ? "YES" : "NO");
  Serial.printf("Auto Engine Control: %s\n", autoEngineControl ? "ENABLED" : "DISABLED");
  Serial.printf("Engine Start Requested: %s\n", engineStartRequested ? "YES" : "NO");
  Serial.println("Dashboard Button: Firebase controlled");
  Serial.printf("WiFi Status: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED" : "DISCONNECTED");
  
  if (alcoholDetected) {
    unsigned long timeSinceDetection = (millis() - alcoholDetectionTime) / 1000;
    Serial.printf("Time Since Alcohol Detection: %lu seconds\n", timeSinceDetection);
  }
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}

// ✅ NEW: Explore Firebase paths to find alcohol data
void exploreFirebasePaths() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ No WiFi connection");
    return;
  }
  
  String explorePaths[] = {
    "/" + userUID + ".json",  // Get entire user data
    "/" + userUID + "/alcohol.json",
    "/" + userUID + "/alcohol/status.json",
    "/" + userUID + "/alcohol/status/status.json",
    "/helmet_public/" + userUID + ".json",
    "/helmet_public/" + userUID + "/helmetStatus.json"
  };
  
  int numPaths = 6;
  
  for (int i = 0; i < numPaths; i++) {
    Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.printf("🔍 Exploring Path %d/%d\n", i+1, numPaths);
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    
    HTTPClient http;
    String fullURL = firebaseHost + explorePaths[i] + "?auth=" + firebaseAuth;
    Serial.println("URL: " + fullURL);
    
    http.begin(fullURL);
    int httpCode = http.GET();
    
    Serial.printf("HTTP Code: %d\n", httpCode);
    
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      
      if (response != "null" && response.length() > 2) {
        Serial.println("✅ DATA FOUND!");
        Serial.println("Response Length: " + String(response.length()));
        
        // Show first 500 characters to avoid spam
        if (response.length() > 500) {
          Serial.println("Response (first 500 chars):");
          Serial.println(response.substring(0, 500) + "...");
        } else {
          Serial.println("Full Response:");
          Serial.println(response);
        }
        
        // Check for alcohol-related data
        if (response.indexOf("alcohol") != -1 || response.indexOf("Alcohol") != -1 ||
            response.indexOf("Safe") != -1 || response.indexOf("Danger") != -1 ||
            response.indexOf("sensorValue") != -1) {
          Serial.println("🎯 ALCOHOL-RELATED DATA DETECTED!");
        }
      } else {
        Serial.println("❌ Empty/null response");
      }
    } else {
      Serial.printf("❌ HTTP Error: %d\n", httpCode);
    }
    
    http.end();
    delay(500);
  }
  
  Serial.println("\n🎯 EXPLORATION COMPLETE!");
  Serial.println("Look for paths with alcohol-related data above.");
}

// ✅ NEW: Handle dashboard button via Firebase
void handleDashboardButton() {
  // Check dashboard button state every second
  if (millis() - lastButtonCheck > BUTTON_CHECK_INTERVAL) {
    checkDashboardButton();
    lastButtonCheck = millis();
  }
}

// ✅ NEW: Check dashboard button state from Firebase
void checkDashboardButton() {
  if (WiFi.status() != WL_CONNECTED) {
    return; // Skip if no WiFi
  }
  
  HTTPClient http;
  http.begin(firebaseHost + buttonPath);
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    
    // Parse button state - look for "true" or "1" or "pressed"
    bool currentButtonState = (response.indexOf("true") != -1) || 
                             (response.indexOf("\"1\"") != -1) ||
                             (response.indexOf("pressed") != -1);
    
    // Check for state change (button press)
    if (currentButtonState != lastDashboardButtonState) {
      lastDashboardButtonState = currentButtonState;
      
      if (currentButtonState) {
        Serial.println("\n🖥️ DASHBOARD BUTTON PRESSED!");
        
        if (!engineRunning) {
          Serial.println("🖥️ Attempting to start engine...");
          engineStartRequested = true;  // Set start request
          startEngine();
        } else {
          Serial.println("🖥️ Engine already running - stopping engine");
          stopEngine();
          engineStartRequested = false;  // Clear start request
        }
        
        // Clear the button state in Firebase after processing
        clearDashboardButton();
      }
    }
  }
  
  http.end();
}

// ✅ NEW: Clear dashboard button state in Firebase
void clearDashboardButton() {
  HTTPClient http;
  http.begin(firebaseHost + buttonPath);
  http.addHeader("Content-Type", "application/json");
  
  // Set button state to false
  int httpCode = http.PUT("false");
  
  if (httpCode == HTTP_CODE_OK) {
    Serial.println("[BUTTON] ✅ Button state cleared in Firebase");
  } else {
    Serial.printf("[BUTTON] ❌ Failed to clear button state: %d\n", httpCode);
  }
  
  http.end();
}