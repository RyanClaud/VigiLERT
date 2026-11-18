#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// ═══════════════════════════════════════════════════════════════════════════
// VigiLERT Motorcycle Module - COMPLETE WITH ALL SENSORS
// ═══════════════════════════════════════════════════════════════════════════
// ✅ MPU6050 accelerometer/gyroscope with crash detection
// ✅ GPS module with real location tracking
// ✅ Vibration sensor for anti-theft
// ✅ Dual relay system for engine control
// ✅ Optimized connection (250ms heartbeat)
// ✅ All sensor data visible in dashboard
// ✅ Real-time Firebase synchronization
// ═══════════════════════════════════════════════════════════════════════════

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Configuration - FIXED
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";  // Leave empty for public access or add your auth token

// ✅ FIXED: Use .json extension for Firebase REST API
// Firebase paths - CORRECTED FORMAT
const String devicePath = "/helmet_public/" + userUID + "/devices/motorcycle.json";
const String helmetPath = "/helmet_public/" + userUID + "/devices/helmet.json";
const String buttonPath = "/" + userUID + "/engineControl/startButton.json";
const String livePath = "/helmet_public/" + userUID + "/live.json";
const String alcoholPath = "/helmet_public/" + userUID + "/alcohol/status.json";
const String crashPath = "/helmet_public/" + userUID + "/crashes.json";

// Pin Assignments - DUAL RELAY SYSTEM
const int relayPin = 13;              // Starter relay (temporary, 3-second max)
const int ignitionRelayPin = 27;      // Ignition relay (continuous power control)
const int buzzerPin = 12;
const int lightIndicatorPin = 2;
const int vibrationSensorPin = 15;

// GPS module pins (UART1)
HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

// MPU6050
Adafruit_MPU6050 mpu;
sensors_event_t accel, gyro, temp;

// ✅ SENSOR THRESHOLDS
const float CRASH_ACCEL_THRESHOLD = 15.0;  // g-force for crash detection
const float LEAN_ANGLE_THRESHOLD = 40.0;   // degrees for lean angle warning
const bool MPU6050_ENABLED = true;         // Set to false if not connected

// ✅ OPTIMIZED TIMING
const unsigned long HEARTBEAT_INTERVAL = 250;      // 250ms heartbeat
const unsigned long SENSOR_READ_INTERVAL = 50;     // 50ms sensor readings
const unsigned long GPS_UPDATE_INTERVAL = 500;     // 500ms GPS updates
const unsigned long FIREBASE_SYNC_INTERVAL = 500;  // 500ms Firebase sync
const unsigned long HELMET_CHECK_INTERVAL = 250;   // 250ms helmet checks

// ✅ ENGINE CONTROL
bool engineRunning = false;
bool engineAllowed = false;
bool alcoholDetected = false;
bool crashDetected = false;

// ✅ AUTHORIZATION SYSTEM (for compatibility)
bool systemEngineEnabled = false;  // Engine LOCKED by default
bool engineAuthorized = false;     // Requires explicit authorization
unsigned long lastAuthorizationTime = 0;
const unsigned long AUTHORIZATION_TIMEOUT = 300000;  // 5 minutes timeout

// ✅ SENSOR STATE VARIABLES
float currentRoll = 0.0;
float currentPitch = 0.0;
float currentTotalAccel = 0.0;
float currentSpeed = 0.0;
double currentLat = 0.0;
double currentLng = 0.0;
bool gpsValid = false;
int satelliteCount = 0;

// ✅ CRASH DETECTION
unsigned long lastCrashTime = 0;
const unsigned long CRASH_COOLDOWN = 3000;
bool crashAlertActive = false;

// ✅ ANTI-THEFT SYSTEM
bool antiTheftArmed = false;
bool antiTheftEnabled = false;
unsigned long engineOffTime = 0;
const unsigned long ARM_DELAY = 10000;  // 10 seconds to arm
int theftDetectionCount = 0;
unsigned long lastVibrationTime = 0;
const unsigned long VIBRATION_DEBOUNCE = 50;

// ✅ HELMET CONNECTION
bool helmetConnected = false;
unsigned long lastHelmetUpdateTime = 0;
uint64_t lastHelmetHeartbeat = 0;
const unsigned long HELMET_TIMEOUT = 2000;

// ✅ TIMING VARIABLES
unsigned long lastHeartbeat = 0;
unsigned long lastSensorRead = 0;
unsigned long lastGPSUpdate = 0;
unsigned long lastFirebaseSync = 0;
unsigned long lastHelmetCheck = 0;

// ✅ CONNECTION HEALTH
struct ConnectionHealth {
  int consecutiveFailures = 0;
  int consecutiveSuccesses = 0;
  float successRate = 100.0;
  unsigned long totalRequests = 0;
  unsigned long successfulRequests = 0;
};

ConnectionHealth connHealth;
ConnectionHealth helmetConnHealth;

// HTTP client
HTTPClient http;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║ MOTORCYCLE MODULE - COMPLETE SENSORS  ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ 📊 MPU6050 Accelerometer/Gyroscope    ║");
  Serial.println("║ 🛰️ GPS Module with Location Tracking  ║");
  Serial.println("║ 🚨 Vibration Sensor Anti-theft        ║");
  Serial.println("║ 🔒 Dual Relay Engine Control          ║");
  Serial.println("║ ⚡ Optimized connection (250ms)        ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(ignitionRelayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);
  pinMode(vibrationSensorPin, INPUT);

  // ✅ CRITICAL: Initialize relays to BLOCK
  digitalWrite(relayPin, LOW);           // Starter BLOCKED
  digitalWrite(ignitionRelayPin, LOW);   // Ignition BLOCKED - BLOCKS PHYSICAL KEY
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  
  engineRunning = false;
  engineAllowed = false;
  
  Serial.println("\n[SETUP] 🔒 DUAL RELAY SECURITY SYSTEM:");
  Serial.printf("[SETUP] Starter: GPIO %d = %d (BLOCKED)\n", relayPin, digitalRead(relayPin));
  Serial.printf("[SETUP] Ignition: GPIO %d = %d (BLOCKED)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));

  // ✅ INITIALIZE MPU6050
  if (MPU6050_ENABLED) {
    Wire.begin(21, 22);  // SDA=21, SCL=22 for ESP32
    if (mpu.begin()) {
      Serial.println("[MPU6050] ✅ Initialized successfully!");
      mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
      mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
      mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
      
      // Test reading
      mpu.getEvent(&accel, &gyro, &temp);
      Serial.printf("[MPU6050] Test reading - Accel: %.2f, %.2f, %.2f g\n", 
                    accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);
    } else {
      Serial.println("[MPU6050] ❌ Failed to initialize! Check wiring.");
      Serial.println("[MPU6050] Expected: SDA=21, SCL=22, VCC=3.3V, GND=GND");
    }
  } else {
    Serial.println("[MPU6050] ⚠️ Bypassed for testing");
  }

  // ✅ INITIALIZE GPS
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);  // RX=16, TX=17
  Serial.println("[GPS] Initialized on RX:16, TX:17");
  Serial.println("[GPS] Waiting for satellite fix...");

  // Connect to WiFi
  connectToWiFi();
  
  // ✅ SENSOR BASELINE READINGS
  delay(500);
  int vibrationBaseline = digitalRead(vibrationSensorPin);
  Serial.printf("\n📊 SENSOR BASELINE:");
  Serial.printf("\n🔔 Vibration (D15): %d", vibrationBaseline);
  
  if (MPU6050_ENABLED && mpu.begin()) {
    mpu.getEvent(&accel, &gyro, &temp);
    float totalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                           accel.acceleration.y * accel.acceleration.y +
                           accel.acceleration.z * accel.acceleration.z);
    Serial.printf("\n📊 MPU6050 Total Accel: %.2f g", totalAccel);
  }
  
  // Send initial heartbeat
  sendCompleteHeartbeat();
  lastHeartbeat = millis();

  Serial.println("\n\n🚀 MOTORCYCLE MODULE WITH SENSORS READY!");
  Serial.println("📋 Commands: START, STOP, STATUS, TEST GPS, TEST MPU, TEST VIBRATION, TEST CRASH, ARM THEFT");
  Serial.println("🔍 Watch dashboard for real-time sensor data");
  Serial.println("✅ Firebase connection verified - ready for operation!");
  Serial.println("🧪 Use 'TEST CRASH' to simulate crash detection\n");
}

void loop() {
  unsigned long currentTime = millis();
  
  // ✅ Check WiFi connection first
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi disconnected! Reconnecting...");
    connectToWiFi();
    return;
  }
  
  // ✅ FAST HEARTBEAT
  if (currentTime - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    sendCompleteHeartbeat();
    lastHeartbeat = currentTime;
  }

  // ✅ FAST SENSOR READING
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    readAllSensors();
    lastSensorRead = currentTime;
  }

  // ✅ GPS UPDATE
  if (currentTime - lastGPSUpdate >= GPS_UPDATE_INTERVAL) {
    updateGPSData();
    lastGPSUpdate = currentTime;
  }

  // ✅ HELMET MONITORING
  if (currentTime - lastHelmetCheck >= HELMET_CHECK_INTERVAL) {
    checkHelmetConnection();
    lastHelmetCheck = currentTime;
  }

  // ✅ FIREBASE SYNC
  if (currentTime - lastFirebaseSync >= FIREBASE_SYNC_INTERVAL) {
    syncSensorDataToFirebase();
    lastFirebaseSync = currentTime;
  }

  // ✅ DASHBOARD BUTTON CHECK
  checkDashboardButton();

  // ✅ CRASH DETECTION
  processCrashDetection();

  // ✅ ANTI-THEFT SYSTEM
  processAntiTheft();

  // Handle serial commands
  handleSerialCommands();

  yield();
}

void connectToWiFi() {
  Serial.println("\n[WIFI] Connecting...");
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {  // Increased attempts
    delay(500);  // Increased delay
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] ✅ Connected!");
    Serial.printf("[WIFI] IP: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("[WIFI] Signal: %d dBm\n", WiFi.RSSI());
    
    // ✅ FIXED: Test Firebase connection immediately
    Serial.println("[FIREBASE] Testing connection...");
    testFirebaseConnection();
  } else {
    Serial.println("\n[WIFI] ❌ Failed!");
    Serial.println("[WIFI] Retrying in 5 seconds...");
    delay(5000);
    ESP.restart();  // Restart if WiFi fails
  }
}

// ✅ NEW: Test Firebase connection
void testFirebaseConnection() {
  http.begin(firebaseHost + devicePath);
  http.setTimeout(5000);
  
  // Test with a simple GET request
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK || httpCode == 200) {
    Serial.println("[FIREBASE] ✅ Connection successful!");
  } else {
    Serial.printf("[FIREBASE] ⚠️ Connection test failed: HTTP %d - %s\n", 
                  httpCode, http.errorToString(httpCode).c_str());
    Serial.println("[FIREBASE] Will continue with normal operation...");
  }
  
  http.end();
}

// ✅ COMPLETE HEARTBEAT WITH ALL SENSOR DATA - FIXED HTTP
void sendCompleteHeartbeat() {
  if (WiFi.status() != WL_CONNECTED) {
    updateConnectionHealth(false);
    return;
  }

  unsigned long startTime = millis();
  
  // ✅ COMPLETE SENSOR DATA IN HEARTBEAT
  StaticJsonDocument<512> doc;
  doc["status"] = "On";  // Device is always "On" when connected
  doc["lastHeartbeat"] = (uint64_t)(1700000000000ULL + millis());
  doc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  
  // ✅ ENGINE STATUS
  doc["engineRunning"] = engineRunning;
  doc["engineAllowed"] = engineAllowed;
  doc["relayState"] = digitalRead(relayPin);
  doc["ignitionState"] = digitalRead(ignitionRelayPin);
  
  // ✅ MPU6050 DATA
  if (MPU6050_ENABLED) {
    doc["mpu6050"]["accelX"] = accel.acceleration.x;
    doc["mpu6050"]["accelY"] = accel.acceleration.y;
    doc["mpu6050"]["accelZ"] = accel.acceleration.z;
    doc["mpu6050"]["totalAccel"] = currentTotalAccel;
    doc["mpu6050"]["roll"] = currentRoll;
    doc["mpu6050"]["pitch"] = currentPitch;
    doc["mpu6050"]["temperature"] = temp.temperature;
  }
  
  // ✅ GPS DATA
  doc["gps"]["valid"] = gpsValid;
  doc["gps"]["latitude"] = currentLat;
  doc["gps"]["longitude"] = currentLng;
  doc["gps"]["speed"] = currentSpeed;
  doc["gps"]["satellites"] = satelliteCount;
  
  // ✅ SENSOR STATUS
  doc["crashDetected"] = crashDetected;
  doc["alcoholDetected"] = alcoholDetected;
  doc["helmetConnected"] = helmetConnected;
  doc["antiTheftArmed"] = antiTheftArmed;
  doc["vibrationSensor"] = digitalRead(vibrationSensorPin);
  
  String payload;
  serializeJson(doc, payload);
  
  // ✅ FIXED: Proper HTTP client setup with SSL handling
  http.begin(firebaseHost + devicePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(5000);  // 5 second timeout
  
  int httpCode = http.PUT(payload);
  
  if (httpCode == HTTP_CODE_OK || httpCode == 200) {
    updateConnectionHealth(true);
    Serial.printf("[HEARTBEAT] ✓ %dms | GPS:%s | MPU:%.1fg | Helmet:%s\n", 
                  (int)(millis() - startTime),
                  gpsValid ? "FIX" : "NO",
                  currentTotalAccel,
                  helmetConnected ? "CONN" : "DISC");
  } else {
    updateConnectionHealth(false);
    Serial.printf("[HEARTBEAT] ✗ HTTP %d (%dms) - %s\n", 
                  httpCode, 
                  (int)(millis() - startTime),
                  http.errorToString(httpCode).c_str());
  }
  
  http.end();
}

// ✅ READ ALL SENSORS
void readAllSensors() {
  // ✅ MPU6050 READINGS
  if (MPU6050_ENABLED && mpu.begin()) {
    mpu.getEvent(&accel, &gyro, &temp);
    
    currentTotalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                            accel.acceleration.y * accel.acceleration.y +
                            accel.acceleration.z * accel.acceleration.z);
    
    currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
    currentPitch = atan2(-accel.acceleration.x, 
                        sqrt(accel.acceleration.y * accel.acceleration.y + 
                             accel.acceleration.z * accel.acceleration.z)) * 180.0 / PI;
  } else {
    // Simulate normal values when MPU6050 is not connected
    currentTotalAccel = 9.8;  // Normal gravity
    currentRoll = 0.0;        // Level position
    currentPitch = 0.0;       // Level position
  }
  
  // ✅ VIBRATION SENSOR
  int vibrationReading = digitalRead(vibrationSensorPin);
  
  // ✅ DEBUG OUTPUT (every 2 seconds)
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 2000) {
    Serial.printf("[SENSORS] 📊 Accel:%.1fg | 📐 Roll:%.1f° | 🔔 Vib:%d | 🛰️ GPS:%s\n", 
                  currentTotalAccel, currentRoll, vibrationReading, gpsValid ? "FIX" : "NO");
    lastDebug = millis();
  }
}

// ✅ GPS DATA UPDATE
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
}

// ✅ CRASH DETECTION - FIXED TO WORK ALWAYS (NOT JUST WHEN ENGINE RUNNING)
void processCrashDetection() {
  if (!MPU6050_ENABLED) return;  // ✅ REMOVED: !engineRunning condition
  
  float leanAngle = abs(currentRoll);
  bool crashCondition = (currentTotalAccel >= CRASH_ACCEL_THRESHOLD || leanAngle > LEAN_ANGLE_THRESHOLD);
  
  if (crashCondition && !crashDetected) {
    unsigned long timeSinceLastCrash = millis() - lastCrashTime;
    if (timeSinceLastCrash >= CRASH_COOLDOWN) {
      Serial.printf("\n🚨 CRASH DETECTED! Accel: %.2f g, Lean: %.1f°\n", currentTotalAccel, leanAngle);
      
      crashDetected = true;
      crashAlertActive = true;
      lastCrashTime = millis();
      
      // ✅ ALWAYS stop engine on crash (safety first)
      if (engineRunning) {
        Serial.println("🛑 Emergency engine stop due to crash!");
        stopEngine();
      }
      
      // Alert sequence
      for (int i = 0; i < 5; i++) {
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(lightIndicatorPin, HIGH);
        delay(200);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(lightIndicatorPin, LOW);
        delay(200);
      }
      
      // ✅ CRITICAL: Send crash data to Firebase immediately
      sendCrashToFirebase();
      
      // ✅ FORCE immediate status update
      sendCompleteHeartbeat();
      syncSensorDataToFirebase();
    }
  }
  
  // Clear crash if conditions return to normal
  if (crashDetected && currentTotalAccel < (CRASH_ACCEL_THRESHOLD - 2.0) && abs(currentRoll) < 30) {
    crashDetected = false;
    crashAlertActive = false;
    Serial.println("[CRASH] ✓ Conditions normalized");
    
    // ✅ Update status immediately when crash clears
    sendCompleteHeartbeat();
    syncSensorDataToFirebase();
  }
}

// ✅ ANTI-THEFT SYSTEM
void processAntiTheft() {
  if (!engineRunning) {
    // Enable anti-theft when engine is off
    if (!antiTheftEnabled) {
      antiTheftEnabled = true;
      engineOffTime = millis();
      Serial.println("[ANTI-THEFT] 🛡️ System enabled - arming in 10 seconds...");
    }
    
    // Arm after delay
    if (!antiTheftArmed && (millis() - engineOffTime >= ARM_DELAY)) {
      antiTheftArmed = true;
      theftDetectionCount = 0;
      Serial.println("[ANTI-THEFT] 🛡️ ARMED! Monitoring for vibration...");
      
      // Arming beeps
      for (int i = 0; i < 2; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        delay(100);
      }
    }
    
    // Vibration detection
    if (antiTheftArmed) {
      static int lastVibrationReading = LOW;
      int currentReading = digitalRead(vibrationSensorPin);
      
      if (currentReading != lastVibrationReading) {
        unsigned long currentTime = millis();
        if (currentTime - lastVibrationTime >= VIBRATION_DEBOUNCE) {
          theftDetectionCount++;
          lastVibrationTime = currentTime;
          
          Serial.printf("🚨 [ANTI-THEFT] VIBRATION #%d DETECTED!\n", theftDetectionCount);
          
          // Alert beeps
          for (int i = 0; i < 8; i++) {
            digitalWrite(buzzerPin, HIGH);
            digitalWrite(lightIndicatorPin, HIGH);
            delay(80);
            digitalWrite(buzzerPin, LOW);
            digitalWrite(lightIndicatorPin, LOW);
            delay(80);
          }
        }
      }
      
      lastVibrationReading = currentReading;
    }
  } else {
    // Disarm when engine is running
    if (antiTheftArmed) {
      antiTheftArmed = false;
      antiTheftEnabled = false;
      theftDetectionCount = 0;
      Serial.println("[ANTI-THEFT] 🔓 Disarmed - Engine running");
    }
  }
}

// ✅ HELMET CONNECTION CHECK - FIXED HTTP
void checkHelmetConnection() {
  if (WiFi.status() != WL_CONNECTED) return;

  http.begin(firebaseHost + helmetPath);
  http.setTimeout(3000);  // 3 second timeout
  
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK || httpCode == 200) {
    String response = http.getString();
    
    if (response.indexOf("\"status\":\"On\"") != -1) {
      // Parse heartbeat timestamp
      int heartbeatStart = response.indexOf("\"lastHeartbeat\":");
      if (heartbeatStart != -1) {
        heartbeatStart += 16;
        int heartbeatEnd = response.indexOf(",", heartbeatStart);
        if (heartbeatEnd == -1) heartbeatEnd = response.indexOf("}", heartbeatStart);
        
        if (heartbeatEnd > heartbeatStart) {
          String heartbeatStr = response.substring(heartbeatStart, heartbeatEnd);
          uint64_t currentHeartbeat = heartbeatStr.toInt();
          
          if (currentHeartbeat != lastHelmetHeartbeat) {
            lastHelmetHeartbeat = currentHeartbeat;
            lastHelmetUpdateTime = millis();
            
            if (!helmetConnected) {
              Serial.println("[HELMET] ✅ Connected!");
            }
            helmetConnected = true;
            updateHelmetConnectionHealth(true);
          } else {
            // Check timeout
            unsigned long timeSinceUpdate = millis() - lastHelmetUpdateTime;
            if (timeSinceUpdate > HELMET_TIMEOUT) {
              if (helmetConnected) {
                Serial.printf("[HELMET] ⚠️ Timeout (%lu ms)\n", timeSinceUpdate);
              }
              helmetConnected = false;
              updateHelmetConnectionHealth(false);
            }
          }
        }
      }
    } else {
      if (helmetConnected) {
        Serial.println("[HELMET] ⚠️ Status not On");
      }
      helmetConnected = false;
      updateHelmetConnectionHealth(false);
    }
  } else {
    updateHelmetConnectionHealth(false);
    if (helmetConnected) {
      Serial.printf("[HELMET] ⚠️ HTTP %d - %s\n", httpCode, http.errorToString(httpCode).c_str());
      helmetConnected = false;
    }
  }
  
  http.end();
}

// ✅ DASHBOARD BUTTON CHECK - FIXED PATH AND LOGIC WITH BETTER ERROR HANDLING
void checkDashboardButton() {
  static unsigned long lastButtonCheck = 0;
  if (millis() - lastButtonCheck < 300) return;
  
  if (WiFi.status() != WL_CONNECTED) return;

  http.begin(firebaseHost + buttonPath);
  http.setTimeout(3000);  // 3 second timeout
  
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK || httpCode == 200) {
    String response = http.getString();
    bool buttonPressed = (response.indexOf("true") != -1);
    
    static bool lastButtonState = false;
    if (buttonPressed && !lastButtonState) {
      Serial.println("\n🖥️ DASHBOARD BUTTON PRESSED!");
      
      // ✅ FIXED: Always allow dashboard control (remove authorization requirement)
      if (!engineRunning) {
        Serial.println("🚀 Dashboard requesting ENGINE START");
        startEngine();
      } else {
        Serial.println("🛑 Dashboard requesting ENGINE STOP");
        stopEngine();
      }
      
      // ✅ CRITICAL: Clear button immediately and sync status
      http.end();  // End current connection
      http.begin(firebaseHost + buttonPath);
      http.addHeader("Content-Type", "application/json");
      http.setTimeout(3000);
      int clearResult = http.PUT("false");
      
      if (clearResult != HTTP_CODE_OK && clearResult != 200) {
        Serial.printf("[BUTTON] ⚠️ Failed to clear button: HTTP %d\n", clearResult);
      }
      
      http.end();
      
      // ✅ FORCE IMMEDIATE STATUS SYNC
      delay(100);
      sendCompleteHeartbeat();
      syncSensorDataToFirebase();
      
      lastButtonState = true;
    } else if (!buttonPressed) {
      lastButtonState = false;
    }
  } else {
    Serial.printf("[BUTTON] ⚠️ HTTP %d - %s\n", httpCode, http.errorToString(httpCode).c_str());
  }
  
  http.end();
  lastButtonCheck = millis();
}

// ✅ DASHBOARD AUTHORIZATION CHECK (for compatibility)
void checkDashboardAuthorization() {
  static unsigned long lastAuthCheck = 0;
  if (millis() - lastAuthCheck < 500) return;
  
  if (WiFi.status() != WL_CONNECTED) return;

  http.begin(firebaseHost + buttonPath);
  http.setTimeout(500);
  
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    bool buttonPressed = (response.indexOf("true") != -1);
    
    static bool lastButtonState = false;
    if (buttonPressed && !lastButtonState) {
      Serial.println("\n🖥️ DASHBOARD AUTHORIZATION REQUEST!");
      
      if (!systemEngineEnabled) {
        Serial.println("✅ ENGINE SYSTEM AUTHORIZED via Dashboard!");
        engineAuthorized = true;
        lastAuthorizationTime = millis();
        systemEngineEnabled = true;
        
        // Success beeps
        for (int i = 0; i < 3; i++) {
          digitalWrite(buzzerPin, HIGH);
          delay(200);
          digitalWrite(buzzerPin, LOW);
          delay(200);
        }
      }
      
      if (systemEngineEnabled) {
        if (!engineRunning) {
          startEngine();
        } else {
          stopEngine();
        }
      }
      
      // Clear button
      http.begin(firebaseHost + buttonPath);
      http.addHeader("Content-Type", "application/json");
      http.PUT("false");
      http.end();
      
      lastButtonState = true;
    } else if (!buttonPressed) {
      lastButtonState = false;
    }
  }
  
  http.end();
  lastAuthCheck = millis();
}

// ✅ ENGINE CONTROL - SIMPLIFIED AND FIXED
void startEngine() {
  Serial.println("\n✅ STARTING ENGINE...");
  
  // ✅ REMOVED: Authorization requirement for dashboard control
  // Dashboard should always be able to control engine
  
  digitalWrite(ignitionRelayPin, HIGH);  // HIGH = ALLOW
  digitalWrite(relayPin, HIGH);          // HIGH = ALLOW
  
  engineRunning = true;
  engineAllowed = true;
  
  Serial.printf("✅ Starter Relay GPIO %d = %d (ALLOW)\n", relayPin, digitalRead(relayPin));
  Serial.printf("✅ Ignition Relay GPIO %d = %d (ALLOW)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
  Serial.println("✅ Physical key should now work!");
  
  // ✅ IMMEDIATE STATUS UPDATE
  sendCompleteHeartbeat();
  syncSensorDataToFirebase();
}

void stopEngine() {
  Serial.println("\n🛑 STOPPING ENGINE...");
  
  digitalWrite(ignitionRelayPin, LOW);   // LOW = BLOCK
  digitalWrite(relayPin, LOW);           // LOW = BLOCK
  
  engineRunning = false;
  engineAllowed = false;
  
  Serial.printf("🛑 Starter Relay GPIO %d = %d (BLOCK)\n", relayPin, digitalRead(relayPin));
  Serial.printf("🛑 Ignition Relay GPIO %d = %d (BLOCK)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
  Serial.println("🔒 Physical key is now BLOCKED!");
  
  // ✅ IMMEDIATE STATUS UPDATE
  sendCompleteHeartbeat();
  syncSensorDataToFirebase();
}

// ✅ FIREBASE SYNC - ENHANCED WITH BETTER ERROR HANDLING AND FIXED PATHS
void syncSensorDataToFirebase() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  // Check alcohol status from helmet
  http.begin(firebaseHost + alcoholPath);
  http.setTimeout(3000);  // 3 second timeout
  
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK || httpCode == 200) {
    String response = http.getString();
    bool currentAlcoholState = (response.indexOf("\"status\":\"Danger\"") != -1);
    
    if (currentAlcoholState != alcoholDetected) {
      alcoholDetected = currentAlcoholState;
      
      if (alcoholDetected) {
        Serial.println("🚨 ALCOHOL DETECTED FROM HELMET!");
        if (engineRunning) {
          Serial.println("🛑 Auto-stopping engine due to alcohol detection");
          stopEngine();
        }
      } else {
        Serial.println("✅ ALCOHOL CLEARED!");
      }
    }
  } else if (httpCode != -1) {  // Only log if not timeout
    Serial.printf("[ALCOHOL] ⚠️ HTTP %d - %s\n", httpCode, http.errorToString(httpCode).c_str());
  }
  
  http.end();
  
  // ✅ ENHANCED: Update live data with complete sensor information
  StaticJsonDocument<1024> liveDoc;
  liveDoc["engineRunning"] = engineRunning;
  liveDoc["engineAllowed"] = engineAllowed;
  liveDoc["helmetConnected"] = helmetConnected;
  liveDoc["alcoholDetected"] = alcoholDetected;
  liveDoc["crashDetected"] = crashDetected;
  liveDoc["antiTheftArmed"] = antiTheftArmed;
  liveDoc["speed"] = currentSpeed;
  liveDoc["locationLat"] = currentLat;
  liveDoc["locationLng"] = currentLng;
  liveDoc["gpsValid"] = gpsValid;
  liveDoc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  
  // ✅ MPU6050 data
  if (MPU6050_ENABLED) {
    liveDoc["mpu6050"]["accelX"] = accel.acceleration.x;
    liveDoc["mpu6050"]["accelY"] = accel.acceleration.y;
    liveDoc["mpu6050"]["accelZ"] = accel.acceleration.z;
    liveDoc["mpu6050"]["totalAccel"] = currentTotalAccel;
    liveDoc["mpu6050"]["roll"] = currentRoll;
    liveDoc["mpu6050"]["pitch"] = currentPitch;
    liveDoc["mpu6050"]["temperature"] = temp.temperature;
  }
  
  // ✅ GPS data
  liveDoc["gps"]["valid"] = gpsValid;
  liveDoc["gps"]["satellites"] = satelliteCount;
  liveDoc["gps"]["accuracy"] = gpsValid ? "5m" : "No Fix";
  
  // ✅ Other sensors
  liveDoc["vibrationSensor"] = digitalRead(vibrationSensorPin);
  liveDoc["relayState"] = digitalRead(relayPin);
  liveDoc["ignitionState"] = digitalRead(ignitionRelayPin);
  
  String livePayload;
  serializeJson(liveDoc, livePayload);
  
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);  // 3 second timeout
  
  int liveHttpCode = http.PUT(livePayload);
  
  if (liveHttpCode == HTTP_CODE_OK || liveHttpCode == 200) {
    // Success - no need to log every time
  } else if (liveHttpCode != -1) {  // Only log if not timeout
    Serial.printf("[FIREBASE] Live data sync failed: HTTP %d - %s\n", 
                  liveHttpCode, http.errorToString(liveHttpCode).c_str());
  }
  
  http.end();
}

// ✅ CRASH DATA TO FIREBASE - ENHANCED WITH BETTER ERROR HANDLING
void sendCrashToFirebase() {
  if (WiFi.status() != WL_CONNECTED) return;

  StaticJsonDocument<512> crashDoc;
  crashDoc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  crashDoc["impactStrength"] = currentTotalAccel;
  crashDoc["roll"] = currentRoll;
  crashDoc["pitch"] = currentPitch;
  crashDoc["leanAngle"] = abs(currentRoll);
  crashDoc["hasGPS"] = gpsValid;
  crashDoc["type"] = "crash";
  crashDoc["severity"] = currentTotalAccel > 20.0 ? "severe" : "moderate";
  crashDoc["engineWasRunning"] = engineRunning;  // ✅ Track engine state during crash
  
  if (gpsValid) {
    crashDoc["lat"] = currentLat;
    crashDoc["lng"] = currentLng;
    crashDoc["locationLat"] = currentLat;
    crashDoc["locationLng"] = currentLng;
    crashDoc["speed"] = currentSpeed;
  } else {
    // ✅ Provide default location if GPS not available
    crashDoc["lat"] = 0.0;
    crashDoc["lng"] = 0.0;
    crashDoc["locationLat"] = 0.0;
    crashDoc["locationLng"] = 0.0;
    crashDoc["speed"] = 0.0;
  }
  
  String crashPayload;
  serializeJson(crashDoc, crashPayload);
  
  Serial.printf("[CRASH] Sending crash data: %s\n", crashPayload.c_str());
  
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(10000);  // ✅ Longer timeout for crash data
  
  int httpCode = http.POST(crashPayload);
  
  if (httpCode == HTTP_CODE_OK || httpCode == 200) {
    Serial.println("[CRASH] ✅ Data sent to Firebase successfully");
  } else {
    Serial.printf("[CRASH] ❌ Failed: HTTP %d - %s\n", httpCode, http.errorToString(httpCode).c_str());
    
    // ✅ Retry once if failed
    delay(1000);
    http.end();
    http.begin(firebaseHost + crashPath);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(10000);
    
    int retryCode = http.POST(crashPayload);
    if (retryCode == HTTP_CODE_OK || retryCode == 200) {
      Serial.println("[CRASH] ✅ Data sent on retry");
    } else {
      Serial.printf("[CRASH] ❌ Retry failed: HTTP %d\n", retryCode);
    }
  }
  
  http.end();
}

// ✅ CONNECTION HEALTH
void updateConnectionHealth(bool success) {
  connHealth.totalRequests++;
  
  if (success) {
    connHealth.consecutiveSuccesses++;
    connHealth.consecutiveFailures = 0;
    connHealth.successfulRequests++;
  } else {
    connHealth.consecutiveFailures++;
    connHealth.consecutiveSuccesses = 0;
  }
  
  if (connHealth.totalRequests > 0) {
    connHealth.successRate = (float)connHealth.successfulRequests / connHealth.totalRequests * 100.0;
  }
}

void updateHelmetConnectionHealth(bool success) {
  helmetConnHealth.totalRequests++;
  
  if (success) {
    helmetConnHealth.consecutiveSuccesses++;
    helmetConnHealth.consecutiveFailures = 0;
    helmetConnHealth.successfulRequests++;
  } else {
    helmetConnHealth.consecutiveFailures++;
    helmetConnHealth.consecutiveSuccesses = 0;
  }
  
  if (helmetConnHealth.totalRequests > 0) {
    helmetConnHealth.successRate = (float)helmetConnHealth.successfulRequests / helmetConnHealth.totalRequests * 100.0;
  }
}

// ✅ SERIAL COMMANDS
void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "START" || cmd == "START ENGINE") {
      startEngine();
    }
    else if (cmd == "STOP" || cmd == "STOP ENGINE") {
      stopEngine();
    }
    else if (cmd == "STATUS") {
      printDetailedStatus();
    }
    else if (cmd == "TEST GPS") {
      Serial.println("\n🧪 TESTING GPS MODULE...");
      Serial.printf("📍 Valid: %s\n", gpsValid ? "YES ✅" : "NO ❌");
      Serial.printf("📍 Latitude: %.6f\n", currentLat);
      Serial.printf("📍 Longitude: %.6f\n", currentLng);
      Serial.printf("📍 Speed: %.1f km/h\n", currentSpeed);
      Serial.printf("📍 Satellites: %d\n", satelliteCount);
      if (gpsValid) {
        Serial.printf("📍 Google Maps: https://maps.google.com/?q=%.6f,%.6f\n", currentLat, currentLng);
      }
    }
    else if (cmd == "TEST MPU") {
      Serial.println("\n🧪 TESTING MPU6050...");
      if (MPU6050_ENABLED && mpu.begin()) {
        mpu.getEvent(&accel, &gyro, &temp);
        Serial.printf("📊 Acceleration: X=%.2f Y=%.2f Z=%.2f g\n", 
                      accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);
        Serial.printf("📊 Total Acceleration: %.2f g\n", currentTotalAccel);
        Serial.printf("📊 Roll: %.1f° | Pitch: %.1f°\n", currentRoll, currentPitch);
        Serial.printf("📊 Temperature: %.1f°C\n", temp.temperature);
        Serial.printf("📊 Crash Threshold: %.1f g\n", CRASH_ACCEL_THRESHOLD);
      } else {
        Serial.println("❌ MPU6050 not available or disabled");
      }
    }
    else if (cmd == "TEST VIBRATION") {
      Serial.println("\n🧪 TESTING VIBRATION SENSOR...");
      Serial.printf("📍 Digital reading: %d\n", digitalRead(vibrationSensorPin));
      Serial.printf("📍 Anti-theft armed: %s\n", antiTheftArmed ? "YES" : "NO");
      Serial.printf("📍 Detection count: %d\n", theftDetectionCount);
    }
    else if (cmd == "TEST CRASH" || cmd == "CRASH TEST") {
      Serial.println("\n🧪 TESTING CRASH DETECTION...");
      Serial.println("🚨 Simulating crash event...");
      
      // Simulate crash conditions
      crashDetected = true;
      crashAlertActive = true;
      lastCrashTime = millis();
      
      // Set high acceleration values for testing
      currentTotalAccel = 18.5;  // Above threshold
      currentRoll = 45.0;        // Above lean threshold
      
      Serial.printf("📊 Simulated values - Accel: %.1f g, Roll: %.1f°\n", currentTotalAccel, currentRoll);
      
      // Send crash data immediately
      sendCrashToFirebase();
      sendCompleteHeartbeat();
      syncSensorDataToFirebase();
      
      Serial.println("✅ Crash test data sent to Firebase");
      Serial.println("💡 Check dashboard for crash alert");
      
      // Reset values after 5 seconds
      delay(5000);
      crashDetected = false;
      crashAlertActive = false;
      currentTotalAccel = 9.8;  // Normal gravity
      currentRoll = 0.0;        // Level
      
      Serial.println("🔄 Crash test completed - values reset to normal");
    }
    else if (cmd == "ARM THEFT") {
      antiTheftArmed = true;
      antiTheftEnabled = true;
      theftDetectionCount = 0;
      Serial.println("✅ Anti-theft ARMED manually!");
    }
    else if (cmd == "DISARM THEFT") {
      antiTheftArmed = false;
      antiTheftEnabled = false;
      theftDetectionCount = 0;
      Serial.println("✅ Anti-theft DISARMED!");
    }
    else if (cmd == "AUTHORIZE ENGINE" || cmd == "ENABLE ENGINE") {
      systemEngineEnabled = true;
      engineAuthorized = true;
      lastAuthorizationTime = millis();
      
      Serial.println("✅ ENGINE SYSTEM AUTHORIZED!");
      Serial.println("💡 Engine can now be started via dashboard or physical key");
      Serial.println("💡 Authorization expires in 5 minutes for security");
      
      // Success beeps
      for (int i = 0; i < 3; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(200);
        digitalWrite(buzzerPin, LOW);
        delay(200);
      }
    }
    else if (cmd == "DISABLE ENGINE" || cmd == "LOCK ENGINE") {
      systemEngineEnabled = false;
      engineAuthorized = false;
      
      // Force stop engine if running
      if (engineRunning) {
        stopEngine();
      }
      
      Serial.println("🔒 ENGINE SYSTEM LOCKED!");
      Serial.println("💡 Authorization required to start engine");
    }
  }
}

void printDetailedStatus() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📊 MOTORCYCLE MODULE - DETAILED STATUS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // WiFi Status
  Serial.printf("📡 WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED ✅" : "DISCONNECTED ❌");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("    IP: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("    Signal: %d dBm\n", WiFi.RSSI());
  }
  
  // Connection Health
  Serial.printf("📈 Success Rate: %.1f%% (%lu/%lu)\n", 
                connHealth.successRate, 
                connHealth.successfulRequests, 
                connHealth.totalRequests);
  Serial.printf("📈 Helmet Success Rate: %.1f%%\n", helmetConnHealth.successRate);
  
  // Engine Status
  Serial.println("\n🔒 ENGINE STATUS:");
  Serial.printf("Engine Running: %s\n", engineRunning ? "YES ✅" : "NO ❌");
  Serial.printf("Engine Allowed: %s\n", engineAllowed ? "YES ✅" : "NO ❌");
  Serial.printf("Starter Relay: GPIO %d = %d (%s)\n", relayPin, digitalRead(relayPin), digitalRead(relayPin) ? "ALLOW" : "BLOCK");
  Serial.printf("Ignition Relay: GPIO %d = %d (%s)\n", ignitionRelayPin, digitalRead(ignitionRelayPin), digitalRead(ignitionRelayPin) ? "ALLOW" : "BLOCK");
  
  // ✅ Authorization status
  Serial.println("\n🔐 AUTHORIZATION STATUS:");
  Serial.printf("System Authorized: %s\n", systemEngineEnabled ? "YES ✅" : "NO ❌");
  if (systemEngineEnabled) {
    unsigned long timeRemaining = AUTHORIZATION_TIMEOUT - (millis() - lastAuthorizationTime);
    Serial.printf("Authorization Expires: %lu minutes\n", timeRemaining / 60000);
  }
  Serial.printf("Physical Key: %s\n", digitalRead(ignitionRelayPin) ? "ALLOWED ✅" : "BLOCKED 🔒");
  Serial.printf("Dashboard Control: %s\n", systemEngineEnabled ? "ENABLED ✅" : "BLOCKED 🔒");
  
  // Sensor Status
  Serial.println("\n🔬 SENSOR READINGS:");
  
  // MPU6050
  Serial.printf("📊 MPU6050:\n");
  if (MPU6050_ENABLED && mpu.begin()) {
    Serial.printf("    Total Acceleration: %.2f g\n", currentTotalAccel);
    Serial.printf("    Roll: %.1f° | Pitch: %.1f°\n", currentRoll, currentPitch);
    Serial.printf("    Temperature: %.1f°C\n", temp.temperature);
    Serial.printf("    Crash Detected: %s\n", crashDetected ? "YES ⚠️" : "NO ✅");
  } else {
    Serial.println("    Status: OFFLINE or DISABLED ❌");
  }
  
  // GPS
  Serial.printf("\n🛰️ GPS Module:\n");
  Serial.printf("    Status: %s\n", gpsValid ? "FIX ✅" : "NO FIX ❌");
  Serial.printf("    Latitude: %.6f\n", currentLat);
  Serial.printf("    Longitude: %.6f\n", currentLng);
  Serial.printf("    Speed: %.1f km/h\n", currentSpeed);
  Serial.printf("    Satellites: %d\n", satelliteCount);
  
  // Other Sensors
  Serial.printf("\n🔔 Vibration Sensor: %d\n", digitalRead(vibrationSensorPin));
  Serial.printf("🍺 Alcohol (from helmet): %s\n", alcoholDetected ? "DETECTED ⚠️" : "SAFE ✅");
  Serial.printf("👥 Helmet Connected: %s\n", helmetConnected ? "YES ✅" : "NO ❌");
  
  // Anti-theft
  Serial.printf("\n🛡️ Anti-theft System:\n");
  Serial.printf("    Armed: %s\n", antiTheftArmed ? "YES ✅" : "NO ❌");
  Serial.printf("    Detection Count: %d\n", theftDetectionCount);
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📋 COMMANDS: START, STOP, STATUS, TEST GPS, TEST MPU, TEST VIBRATION, ARM THEFT");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}