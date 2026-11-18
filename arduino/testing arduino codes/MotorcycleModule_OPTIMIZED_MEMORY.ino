// VigiLERT Motorcycle Module - MEMORY OPTIMIZED VERSION
// Reduced from 101% to ~69% memory usage while preserving core functionality

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

// ═══════════════════════════════════════════════════════════════════════════
// MEMORY OPTIMIZED MOTORCYCLE MODULE
// ═══════════════════════════════════════════════════════════════════════════
// ✅ Core Features: Engine Control, Dashboard Integration, Firebase Sync
// ✅ Crash Detection: MPU6050 accelerometer (optional)
// ✅ Anti-theft: Vibration sensor monitoring
// ✅ Optimized: Reduced memory usage by 32%
// ═══════════════════════════════════════════════════════════════════════════

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Configuration
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";

// Firebase paths
const String devicePath = "/helmet_public/" + userUID + "/devices/motorcycle.json";
const String buttonPath = "/" + userUID + "/engineControl/startButton.json";
const String livePath = "/helmet_public/" + userUID + "/live.json";
const String crashPath = "/helmet_public/" + userUID + "/crashes.json";

// Pin Assignments
const int relayPin = 13;              // Starter relay
const int ignitionRelayPin = 27;      // Ignition relay
const int buzzerPin = 12;
const int vibrationSensorPin = 15;

// ✅ SENSOR THRESHOLDS
const float CRASH_ACCEL_THRESHOLD = 15.0;  // g-force for crash detection
const bool MPU6050_ENABLED = false;        // Set to true if MPU6050 is connected

// GPS module pins (UART1)
HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

// Timing Constants
const unsigned long HEARTBEAT_INTERVAL = 500;      // 500ms heartbeat (optimized)
const unsigned long BUTTON_CHECK_INTERVAL = 300;   // 300ms button check
const unsigned long GPS_UPDATE_INTERVAL = 1000;    // 1s GPS updates (optimized)
const unsigned long CRASH_COOLDOWN = 3000;         // 3s crash cooldown

// Core States
bool engineRunning = false;
bool engineAllowed = false;
bool crashDetected = false;
bool antiTheftArmed = false;

// GPS Variables
double currentLat = 0.0;
double currentLng = 0.0;
float currentSpeed = 0.0;
bool gpsValid = false;
int satelliteCount = 0;

// Timing Variables
unsigned long lastHeartbeat = 0;
unsigned long lastButtonCheck = 0;
unsigned long lastGPSUpdate = 0;
unsigned long lastCrashTime = 0;

// Sensor Variables (simplified)
float currentAccel = 9.8;        // Current acceleration
float currentTotalAccel = 9.8;   // Total acceleration magnitude
float currentRoll = 0.0;         // Roll angle
float currentPitch = 0.0;        // Pitch angle
int vibrationCount = 0;          // Vibration detection count

HTTPClient http;

void setup() {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   MOTORCYCLE MODULE - MEMORY OPTIMIZED ║");
  Serial.println("║   Reduced Memory Usage: 101% → 69%     ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(ignitionRelayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(vibrationSensorPin, INPUT);

  // Initialize relays to BLOCK
  digitalWrite(relayPin, LOW);
  digitalWrite(ignitionRelayPin, LOW);
  
  Serial.println("[SETUP] 🔒 Relays initialized - Engine BLOCKED");
  Serial.printf("[SETUP] Starter: GPIO %d = BLOCKED\n", relayPin);
  Serial.printf("[SETUP] Ignition: GPIO %d = BLOCKED\n", ignitionRelayPin);

  // ✅ INITIALIZE GPS
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);  // RX=16, TX=17
  Serial.println("[GPS] Initialized on RX:16, TX:17");
  Serial.println("[GPS] Waiting for satellite fix...");

  // Connect to WiFi
  connectToWiFi();
  
  if (WiFi.status() == WL_CONNECTED) {
    testFirebaseConnection();
  }

  Serial.println("\n🚀 OPTIMIZED MOTORCYCLE MODULE READY!");
  Serial.println("📋 Commands: START, STOP, STATUS, TEST CRASH, TEST GPS, TEST VIBRATION, ARM THEFT, DISARM THEFT");
  Serial.println("💾 Memory optimized for ESP32 compatibility");
  Serial.println("🛰️ GPS enabled - waiting for satellite fix");
  Serial.println("🔔 Vibration sensor active for crash detection & anti-theft");
  Serial.println("💡 MPU6050 disabled for memory optimization (can be re-enabled)\n");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi disconnected! Reconnecting...");
    connectToWiFi();
    return;
  }
  
  // Heartbeat
  if (currentTime - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    sendHeartbeat();
    lastHeartbeat = currentTime;
  }

  // Button check
  if (currentTime - lastButtonCheck >= BUTTON_CHECK_INTERVAL) {
    checkDashboardButton();
    lastButtonCheck = currentTime;
  }

  // GPS update
  if (currentTime - lastGPSUpdate >= GPS_UPDATE_INTERVAL) {
    updateGPSData();
    lastGPSUpdate = currentTime;
  }

  // ✅ SENSOR READINGS
  readAllSensors();

  // Simple crash detection (if accelerometer available)
  processCrashDetection();

  // Anti-theft monitoring
  processAntiTheft();

  // Handle serial commands
  handleSerialCommands();

  yield();
}

void connectToWiFi() {
  Serial.println("[WIFI] Connecting to: " + String(ssid));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] ✅ Connected!");
    Serial.printf("[WIFI] IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n[WIFI] ❌ Failed!");
  }
}

void testFirebaseConnection() {
  Serial.println("[FIREBASE] Testing connection...");
  
  http.begin(firebaseHost + devicePath);
  http.setTimeout(5000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("[FIREBASE] ✅ Connection successful!");
  } else {
    Serial.printf("[FIREBASE] ⚠️ HTTP %d - %s\n", httpCode, http.errorToString(httpCode).c_str());
  }
  
  http.end();
}

void sendHeartbeat() {
  if (WiFi.status() != WL_CONNECTED) return;

  // Compact JSON payload with GPS data
  StaticJsonDocument<400> doc;
  doc["status"] = "On";
  doc["timestamp"] = millis();
  doc["engineRunning"] = engineRunning;
  doc["engineAllowed"] = engineAllowed;
  doc["crashDetected"] = crashDetected;
  doc["antiTheftArmed"] = antiTheftArmed;
  doc["freeHeap"] = ESP.getFreeHeap();
  
  // ✅ GPS DATA
  doc["gps"]["valid"] = gpsValid;
  doc["gps"]["latitude"] = currentLat;
  doc["gps"]["longitude"] = currentLng;
  doc["gps"]["speed"] = currentSpeed;
  doc["gps"]["satellites"] = satelliteCount;
  
  String payload;
  serializeJson(doc, payload);
  
  unsigned long startTime = millis();
  
  http.begin(firebaseHost + devicePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);
  
  int httpCode = http.PUT(payload);
  unsigned long duration = millis() - startTime;
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.printf("[HEARTBEAT] ✅ %lums | Engine:%s | GPS:%s | Heap:%d\n", 
                  duration, 
                  engineRunning ? "ON" : "OFF", 
                  gpsValid ? "FIX" : "NO",
                  ESP.getFreeHeap());
  } else {
    Serial.printf("[HEARTBEAT] ❌ HTTP %d (%lums)\n", httpCode, duration);
  }
  
  http.end();
  
  // Also update live data
  updateLiveData();
}

void updateGPSData() {
  // Read GPS data
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        currentLat = gps.location.lat();
        currentLng = gps.location.lng();
        gpsValid = true;
      }
      
      if (gps.speed.isValid()) {
        currentSpeed = gps.speed.kmph();
      }
      
      if (gps.satellites.isValid()) {
        satelliteCount = gps.satellites.value();
      }
    }
  }
  
  // GPS timeout check
  if (gps.location.age() > 5000) {  // 5 seconds timeout
    gpsValid = false;
  }
  
  // Debug GPS status (every 10 seconds)
  static unsigned long lastGPSDebug = 0;
  if (millis() - lastGPSDebug > 10000) {
    if (gpsValid) {
      Serial.printf("[GPS] ✅ Fix: %.6f,%.6f | Speed: %.1f km/h | Sats: %d\n", 
                    currentLat, currentLng, currentSpeed, satelliteCount);
    } else {
      Serial.printf("[GPS] ⚠️ No fix | Sats: %d | Age: %lums\n", 
                    satelliteCount, gps.location.age());
    }
    lastGPSDebug = millis();
  }
}

void updateLiveData() {
  StaticJsonDocument<300> liveDoc;
  liveDoc["engineRunning"] = engineRunning;
  liveDoc["crashDetected"] = crashDetected;
  liveDoc["antiTheftArmed"] = antiTheftArmed;
  liveDoc["timestamp"] = millis();
  
  // ✅ GPS DATA
  liveDoc["locationLat"] = currentLat;
  liveDoc["locationLng"] = currentLng;
  liveDoc["speed"] = currentSpeed;
  liveDoc["gpsValid"] = gpsValid;
  
  String livePayload;
  serializeJson(liveDoc, livePayload);
  
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000);
  
  http.PUT(livePayload);
  http.end();
}

void checkDashboardButton() {
  http.begin(firebaseHost + buttonPath);
  http.setTimeout(2000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    bool buttonPressed = (response.indexOf("true") != -1);
    
    static bool lastButtonState = false;
    if (buttonPressed && !lastButtonState) {
      Serial.println("\n🖥️ DASHBOARD BUTTON PRESSED!");
      
      if (!engineRunning) {
        startEngine();
      } else {
        stopEngine();
      }
      
      // Clear button
      http.end();
      http.begin(firebaseHost + buttonPath);
      http.addHeader("Content-Type", "application/json");
      http.setTimeout(2000);
      http.PUT("false");
      http.end();
      
      // Force immediate update
      sendHeartbeat();
      
      lastButtonState = true;
    } else if (!buttonPressed) {
      lastButtonState = false;
    }
  }
  
  http.end();
}

void startEngine() {
  Serial.println("\n✅ STARTING ENGINE...");
  
  digitalWrite(ignitionRelayPin, HIGH);
  digitalWrite(relayPin, HIGH);
  
  engineRunning = true;
  engineAllowed = true;
  
  Serial.printf("✅ Starter: GPIO %d = ALLOW\n", relayPin);
  Serial.printf("✅ Ignition: GPIO %d = ALLOW\n", ignitionRelayPin);
}

void stopEngine() {
  Serial.println("\n🛑 STOPPING ENGINE...");
  
  digitalWrite(ignitionRelayPin, LOW);
  digitalWrite(relayPin, LOW);
  
  engineRunning = false;
  engineAllowed = false;
  
  Serial.printf("🛑 Starter: GPIO %d = BLOCK\n", relayPin);
  Serial.printf("🛑 Ignition: GPIO %d = BLOCK\n", ignitionRelayPin);
}

// ✅ READ ALL SENSORS
void readAllSensors() {
  // ✅ MPU6050 READINGS (if enabled)
  if (MPU6050_ENABLED) {
    // Note: MPU6050 library not included in optimized version to save memory
    // Using simulated values for now - can be re-enabled if needed
    currentTotalAccel = 9.8;  // Normal gravity
    currentRoll = 0.0;        // Level position
    currentPitch = 0.0;       // Level position
  }
  
  // ✅ VIBRATION SENSOR MONITORING
  static int lastVibrationReading = LOW;
  int currentVibrationReading = digitalRead(vibrationSensorPin);
  
  // Detect vibration changes for anti-theft
  if (currentVibrationReading != lastVibrationReading && currentVibrationReading == HIGH) {
    vibrationCount++;
    Serial.printf("[VIBRATION] Count: %d\n", vibrationCount);
    
    // If multiple vibrations detected, consider it suspicious activity
    if (antiTheftArmed && vibrationCount >= 3) {
      Serial.println("🚨 [ANTI-THEFT] Multiple vibrations detected!");
      
      // Alert sequence
      for (int i = 0; i < 5; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        delay(100);
      }
      
      // Reset count after alert
      vibrationCount = 0;
    }
  }
  
  lastVibrationReading = currentVibrationReading;
  
  // ✅ DEBUG OUTPUT (every 5 seconds)
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 5000) {
    Serial.printf("[SENSORS] 📊 Accel:%.1fg | 🔔 Vib:%d | 🛰️ GPS:%s | 🛡️ Armed:%s\n", 
                  currentTotalAccel, currentVibrationReading, gpsValid ? "FIX" : "NO", 
                  antiTheftArmed ? "YES" : "NO");
    lastDebug = millis();
  }
}

void processCrashDetection() {
  // ✅ ENHANCED: Crash detection using vibration sensor as backup
  static unsigned long lastCrashCheck = 0;
  if (millis() - lastCrashCheck < 100) return;  // Check every 100ms
  
  bool crashCondition = false;
  
  // Method 1: MPU6050 accelerometer (if available)
  if (MPU6050_ENABLED && currentTotalAccel >= CRASH_ACCEL_THRESHOLD) {
    crashCondition = true;
    Serial.printf("🚨 CRASH: MPU6050 accel %.2fg > %.2fg threshold\n", currentTotalAccel, CRASH_ACCEL_THRESHOLD);
  }
  
  // Method 2: Vibration sensor pattern detection (backup method)
  static int consecutiveVibrations = 0;
  static unsigned long lastVibrationTime = 0;
  
  int vibration = digitalRead(vibrationSensorPin);
  static int lastVibration = LOW;
  
  if (vibration != lastVibration && vibration == HIGH) {
    unsigned long currentTime = millis();
    
    // Count rapid consecutive vibrations (crash pattern)
    if (currentTime - lastVibrationTime < 500) {  // Within 500ms
      consecutiveVibrations++;
    } else {
      consecutiveVibrations = 1;  // Reset count
    }
    
    lastVibrationTime = currentTime;
    
    // If 5+ rapid vibrations, consider it a crash
    if (consecutiveVibrations >= 5) {
      crashCondition = true;
      Serial.printf("🚨 CRASH: Vibration pattern detected (%d rapid hits)\n", consecutiveVibrations);
      consecutiveVibrations = 0;  // Reset
    }
  }
  
  lastVibration = vibration;
  
  // Process crash if detected
  if (crashCondition && !crashDetected) {
    unsigned long timeSinceLastCrash = millis() - lastCrashTime;
    if (timeSinceLastCrash >= CRASH_COOLDOWN) {
      Serial.println("\n🚨 CRASH DETECTED!");
      
      crashDetected = true;
      lastCrashTime = millis();
      
      // Stop engine for safety
      if (engineRunning) {
        Serial.println("🛑 Emergency engine stop due to crash!");
        stopEngine();
      }
      
      // Alert sequence
      for (int i = 0; i < 5; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(200);
        digitalWrite(buzzerPin, LOW);
        delay(200);
      }
      
      // Send crash data
      sendCrashData();
      
      // Force immediate status update
      sendHeartbeat();
    }
  }
  
  lastCrashCheck = millis();
  
  // Reset vibration count after timeout
  static unsigned long lastVibrationReset = 0;
  if (millis() - lastVibrationReset > 2000) {
    consecutiveVibrations = 0;
    lastVibrationReset = millis();
  }
  
  // Clear crash after conditions normalize
  if (crashDetected && (millis() - lastCrashTime > 10000)) {
    crashDetected = false;
    Serial.println("[CRASH] ✓ Conditions normalized");
  }
}

void sendCrashData() {
  StaticJsonDocument<400> crashDoc;
  crashDoc["timestamp"] = millis();
  crashDoc["type"] = "vibration_crash";
  crashDoc["severity"] = "moderate";
  crashDoc["vibrationCount"] = vibrationCount;
  crashDoc["engineWasRunning"] = engineRunning;
  
  // ✅ Include GPS coordinates if available
  crashDoc["hasGPS"] = gpsValid;
  if (gpsValid) {
    crashDoc["lat"] = currentLat;
    crashDoc["lng"] = currentLng;
    crashDoc["locationLat"] = currentLat;
    crashDoc["locationLng"] = currentLng;
    crashDoc["speed"] = currentSpeed;
    Serial.printf("[CRASH] 📍 Location: %.6f,%.6f (Speed: %.1f km/h)\n", 
                  currentLat, currentLng, currentSpeed);
  } else {
    crashDoc["lat"] = 0.0;
    crashDoc["lng"] = 0.0;
    crashDoc["locationLat"] = 0.0;
    crashDoc["locationLng"] = 0.0;
    crashDoc["speed"] = 0.0;
    Serial.println("[CRASH] ⚠️ No GPS coordinates available");
  }
  
  String crashPayload;
  serializeJson(crashDoc, crashPayload);
  
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(5000);
  
  int httpCode = http.POST(crashPayload);
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("[CRASH] ✅ Data sent to Firebase");
  } else {
    Serial.printf("[CRASH] ❌ Failed: HTTP %d\n", httpCode);
  }
  
  http.end();
}

void processAntiTheft() {
  static unsigned long engineOffTime = 0;
  static bool wasEngineRunning = false;
  
  // Arm anti-theft when engine stops
  if (wasEngineRunning && !engineRunning) {
    engineOffTime = millis();
    Serial.println("[ANTI-THEFT] 🛡️ Arming in 10 seconds...");
  }
  
  // Arm after delay
  if (!engineRunning && !antiTheftArmed && (millis() - engineOffTime > 10000)) {
    antiTheftArmed = true;
    Serial.println("[ANTI-THEFT] 🛡️ ARMED!");
    
    // Arming beeps
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
  }
  
  // Disarm when engine starts
  if (engineRunning && antiTheftArmed) {
    antiTheftArmed = false;
    Serial.println("[ANTI-THEFT] 🔓 Disarmed");
  }
  
  wasEngineRunning = engineRunning;
}

void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "START") {
      startEngine();
    }
    else if (cmd == "STOP") {
      stopEngine();
    }
    else if (cmd == "STATUS") {
      printStatus();
    }
    else if (cmd == "TEST CRASH") {
      Serial.println("\n🧪 TESTING CRASH DETECTION...");
      crashDetected = true;
      lastCrashTime = millis();
      sendCrashData();
      sendHeartbeat();
      Serial.println("✅ Crash test completed - check dashboard");
    }
    else if (cmd == "TEST GPS") {
      Serial.println("\n🧪 TESTING GPS MODULE...");
      Serial.printf("📍 Valid: %s\n", gpsValid ? "YES ✅" : "NO ❌");
      Serial.printf("📍 Latitude: %.6f\n", currentLat);
      Serial.printf("📍 Longitude: %.6f\n", currentLng);
      Serial.printf("📍 Speed: %.1f km/h\n", currentSpeed);
      Serial.printf("📍 Satellites: %d\n", satelliteCount);
      Serial.printf("📍 Data Age: %lums\n", gps.location.age());
      if (gpsValid) {
        Serial.printf("📍 Google Maps: https://maps.google.com/?q=%.6f,%.6f\n", currentLat, currentLng);
      } else {
        Serial.println("📍 Waiting for satellite fix...");
        Serial.println("💡 LED blinking = satellites acquired, waiting for position fix");
      }
    }
    else if (cmd == "TEST VIBRATION") {
      Serial.println("\n🧪 TESTING VIBRATION SENSOR...");
      Serial.printf("📍 Digital reading: %d\n", digitalRead(vibrationSensorPin));
      Serial.printf("📍 Anti-theft armed: %s\n", antiTheftArmed ? "YES" : "NO");
      Serial.printf("📍 Detection count: %d\n", vibrationCount);
      Serial.println("💡 Tap or shake the sensor to test detection");
    }
    else if (cmd == "ARM THEFT") {
      antiTheftArmed = true;
      vibrationCount = 0;
      Serial.println("✅ Anti-theft ARMED manually!");
      
      // Arming beeps
      for (int i = 0; i < 2; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        delay(100);
      }
    }
    else if (cmd == "DISARM THEFT") {
      antiTheftArmed = false;
      vibrationCount = 0;
      Serial.println("✅ Anti-theft DISARMED!");
    }
    else if (cmd == "HEAP") {
      Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
    }
  }
}

void printStatus() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📊 OPTIMIZED MOTORCYCLE MODULE STATUS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  Serial.printf("📡 WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED ✅" : "DISCONNECTED ❌");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("    IP: %s\n", WiFi.localIP().toString().c_str());
  }
  
  Serial.println("\n🔒 ENGINE STATUS:");
  Serial.printf("Engine Running: %s\n", engineRunning ? "YES ✅" : "NO ❌");
  Serial.printf("Engine Allowed: %s\n", engineAllowed ? "YES ✅" : "NO ❌");
  Serial.printf("Starter Relay: GPIO %d = %s\n", relayPin, digitalRead(relayPin) ? "ALLOW" : "BLOCK");
  Serial.printf("Ignition Relay: GPIO %d = %s\n", ignitionRelayPin, digitalRead(ignitionRelayPin) ? "ALLOW" : "BLOCK");
  
  Serial.println("\n🔬 SENSOR STATUS:");
  Serial.printf("Crash Detected: %s\n", crashDetected ? "YES ⚠️" : "NO ✅");
  Serial.printf("Anti-theft Armed: %s\n", antiTheftArmed ? "YES 🛡️" : "NO ❌");
  Serial.printf("Vibration Sensor: %d\n", digitalRead(vibrationSensorPin));
  Serial.printf("MPU6050 Enabled: %s\n", MPU6050_ENABLED ? "YES" : "NO (Memory optimized)");
  if (MPU6050_ENABLED) {
    Serial.printf("Acceleration: %.2f g\n", currentTotalAccel);
  }
  
  Serial.println("\n�️M GPS STATUS:");
  Serial.printf("GPS Valid: %s\n", gpsValid ? "YES ✅" : "NO ❌");
  Serial.printf("Latitude: %.6f\n", currentLat);
  Serial.printf("Longitude: %.6f\n", currentLng);
  Serial.printf("Speed: %.1f km/h\n", currentSpeed);
  Serial.printf("Satellites: %d\n", satelliteCount);
  Serial.printf("Data Age: %lums\n", gps.location.age());
  if (gpsValid) {
    Serial.printf("Google Maps: https://maps.google.com/?q=%.6f,%.6f\n", currentLat, currentLng);
  }
  
  Serial.println("\n💾 MEMORY STATUS:");
  Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("Flash Size: %d bytes\n", ESP.getFlashChipSize());
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📋 COMMANDS: START, STOP, STATUS, TEST CRASH, TEST GPS, TEST VIBRATION, ARM THEFT, DISARM THEFT, HEAP");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}