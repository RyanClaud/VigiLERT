/* 
 * VigiLERT ESP32 Motorcycle Module - GSM REMOVED
 * ✅ ACTIVE-LOW relay logic
 * ✅ WiFi + Firebase communication
 * ✅ Crash detection with MPU6050
 * ✅ Alcohol detection monitoring
 * ✅ GPS tracking
 * ✅ Dashboard engine control
 * ✅ Automatic engine control
 * ❌ GSM module removed (no SMS commands)
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

// ✅ Alcohol detection system with real-time monitoring
bool alcoholDetected = false;
bool lastAlcoholState = false;
unsigned long lastAlcoholCheck = 0;
const unsigned long ALCOHOL_CHECK_INTERVAL = 500; // Check every 500ms
unsigned long alcoholDetectionTime = 0;

// ✅ Automatic engine control
bool autoEngineControl = false;
bool engineStartRequested = false;

// ✅ Dashboard button control
bool lastDashboardButtonState = false;
unsigned long lastButtonCheck = 0;
const unsigned long BUTTON_CHECK_INTERVAL = 1000; // Check every 1 second

// Firebase paths
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
const String buttonPath = "/" + userUID + "/engineControl/startButton.json?auth=" + firebaseAuth;
const String livePath = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;

// Alcohol detection paths
const String alcoholPaths[] = {
  "/" + userUID + "/alcohol/status/status.json?auth=" + firebaseAuth,
  "/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth,
  "/" + userUID + "/alcohol.json?auth=" + firebaseAuth,
  "/helmet_public/" + userUID + "/alcohol.json?auth=" + firebaseAuth,
  "/helmet_public/" + userUID + "/helmetStatus/alcohol.json?auth=" + firebaseAuth,
  "/" + userUID + ".json?auth=" + firebaseAuth
};
const int numAlcoholPaths = 6;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n[SETUP] Starting VigiLERT System - NO GSM VERSION...");
  
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
  
  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);
  
  // ✅ CRITICAL: CONSISTENT ACTIVE-LOW RELAY LOGIC
  digitalWrite(relayPin, HIGH);   // HIGH = OFF (Engine Blocked)
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  delay(100);
  
  Serial.println("\n[SETUP] ═══════════════════════════════════");
  Serial.println("[SETUP] RELAY TYPE: ACTIVE-LOW");
  Serial.println("[SETUP] GPIO: 13 (Safe, no conflicts)");
  Serial.println("[SETUP] HIGH (1) = Relay OFF (Engine Blocked)");
  Serial.println("[SETUP] LOW (0) = Relay ON (Engine Allowed)");
  Serial.printf("[SETUP] Relay initialized to: %d (should be 1 = HIGH/OFF)\n", digitalRead(relayPin));
  Serial.println("[SETUP] Engine is BLOCKED until safety conditions met");
  Serial.println("[SETUP] ═══════════════════════════════════\n");
  
  // Connect to WiFi for Firebase communication
  connectToWiFi();
  
  Serial.println("[SUCCESS] Setup complete. Ready for real-time monitoring.");
  Serial.println("\n🎮 CONTROLS:");
  Serial.println("   Use dashboard button to start/stop engine");
  Serial.println("   Engine will auto-stop if alcohol detected");
  Serial.println("   Engine will auto-start when alcohol clears");
  Serial.println("\n📋 SERIAL COMMANDS:");
  Serial.println("   Type 'CRASH' to test crash detection");
  Serial.println("   Type 'ALCOHOL' to test alcohol detection");
  Serial.println("   Type 'AUTO ON' to enable automatic engine control");
  Serial.println("   Type 'AUTO OFF' to disable automatic engine control");
  Serial.println("   Type 'START ENGINE' to manually start engine");
  Serial.println("   Type 'STOP ENGINE' to manually stop engine");
  Serial.println("   Type 'STATUS' for system status\n");
}

void loop() {
  // ✅ Handle dashboard button via Firebase
  handleDashboardButton();
  
  // Serial command handler
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "CRASH") {
      Serial.println("\n🧪 MANUAL CRASH TEST TRIGGERED!");
      triggerCrashShutdown(20.0, 45.0);
    }
    else if (cmd == "AUTO ON") {
      Serial.println("\n✅ AUTOMATIC ENGINE CONTROL ENABLED");
      autoEngineControl = true;
    }
    else if (cmd == "AUTO OFF") {
      Serial.println("\n🛑 AUTOMATIC ENGINE CONTROL DISABLED");
      autoEngineControl = false;
    }
    else if (cmd == "STATUS") {
      printSystemStatus();
    }
    else if (cmd == "ALCOHOL") {
      Serial.println("\n🧪 MANUAL ALCOHOL TEST TRIGGERED!");
      alcoholDetected = true;
      alcoholDetectionTime = millis();
      digitalWrite(relayPin, HIGH);  // Force OFF
      if (engineRunning) {
        triggerAlcoholShutdown();
      }
    }
    else if (cmd == "CLEAR ALCOHOL") {
      Serial.println("\n🧪 CLEARING ALCOHOL TEST...");
      alcoholDetected = false;
    }
    else if (cmd == "START ENGINE") {
      Serial.println("\n🧪 MANUAL ENGINE START TEST...");
      startEngine();
    }
    else if (cmd == "STOP ENGINE") {
      Serial.println("\n🧪 MANUAL ENGINE STOP TEST...");
      stopEngine();
    }
  }
  
  // Read MPU6050 data
  mpu.getEvent(&accel, &gyro, &temp);
  
  currentTotalAccel = sqrt(
    accel.acceleration.x * accel.acceleration.x +
    accel.acceleration.y * accel.acceleration.y +
    accel.acceleration.z * accel.acceleration.z
  );
  
  currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
  float leanAngle = abs(currentRoll);
  
  // ✅ CRASH DETECTION: Immediate relay shutdown
  if ((currentTotalAccel >= ACCEL_THRESHOLD || leanAngle > 40) && !crashDetected) {
    unsigned long timeSinceLastCrash = millis() - lastCrashTime;
    if (timeSinceLastCrash >= CRASH_COOLDOWN) {
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
  
  // ✅ Alcohol detection monitoring
  if (millis() - lastAlcoholCheck > ALCOHOL_CHECK_INTERVAL) {
    checkAlcoholStatus();
    lastAlcoholCheck = millis();
  }
  
  // ✅ AUTOMATIC ENGINE CONTROL based on alcohol detection
  if (autoEngineControl) {
    if (alcoholDetected && engineRunning) {
      Serial.println("\n🚨 AUTO-SHUTDOWN: Alcohol detected!");
      stopEngine();
      engineStartRequested = true;  // Remember start request
    }
    else if (!alcoholDetected && engineStartRequested && !engineRunning) {
      Serial.println("\n✅ AUTO-START: Alcohol cleared - restarting engine!");
      startEngine();
    }
  }
  
  // ✅ SAFETY OVERRIDE: Always force relay OFF if alcohol detected
  if (alcoholDetected) {
    digitalWrite(relayPin, HIGH);  // ACTIVE-LOW: HIGH = OFF
  }
  
  // Status monitoring
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 2000) {
    Serial.printf("[STATUS] Engine: %s | Relay: %s | Alcohol: %s | Auto: %s\n",
      engineRunning ? "RUN" : "STOP",
      digitalRead(relayPin) ? "OFF" : "ON",
      alcoholDetected ? "DANGER" : "SAFE",
      autoEngineControl ? "ON" : "OFF"
    );
    lastPrint = millis();
  }
  
  // ✅ Send live data to Firebase
  static unsigned long lastFirebaseUpdate = 0;
  if (millis() - lastFirebaseUpdate > 2000) {
    sendLiveToFirebase();
    lastFirebaseUpdate = millis();
  }
  
  delay(50);
}

// ✅ CRASH SHUTDOWN
void triggerCrashShutdown(float impact, float roll) {
  Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
  Serial.printf("Impact: %.2f g | Roll: %.1f°\n", impact, roll);
  
  digitalWrite(relayPin, HIGH);  // ACTIVE-LOW: HIGH = OFF
  engineRunning = false;
  
  Serial.printf("🚨 Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
  
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
}

// ✅ ENGINE CONTROL
void startEngine() {
  if (alcoholDetected) {
    Serial.println("\n❌ ENGINE START BLOCKED - ALCOHOL DETECTED!");
    if (autoEngineControl) {
      engineStartRequested = true;
    }
    digitalWrite(relayPin, HIGH);  // Keep OFF
    
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
  digitalWrite(relayPin, LOW);   // ACTIVE-LOW: LOW = ON
  engineRunning = true;
  engineStartRequested = false;
  Serial.printf("✅ Relay GPIO %d = %d (ON)\n", relayPin, digitalRead(relayPin));
}

void stopEngine() {
  Serial.println("\n🛑 Stopping engine...");
  digitalWrite(relayPin, HIGH);  // ACTIVE-LOW: HIGH = OFF
  engineRunning = false;
  Serial.printf("🛑 Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
}

void sendCrashToFirebase(float impact, float roll) {
  Serial.printf("📡 Sending crash to Firebase: Impact=%.2f, Roll=%.1f\n", impact, roll);
  
  if (WiFi.status() != WL_CONNECTED) return;
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = millis();
  doc["impactStrength"] = impact;
  doc["roll"] = roll;
  doc["leanAngle"] = abs(roll);
  doc["hasGPS"] = false;  // Add GPS data if available
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(payload);
  
  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
    Serial.println("[FIREBASE] ✅ Crash data sent");
  } else {
    Serial.printf("[FIREBASE] ❌ Failed: %d\n", httpCode);
  }
  http.end();
}

// ✅ Send live data to Firebase
void sendLiveToFirebase() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  StaticJsonDocument<512> doc;
  doc["engineRunning"] = engineRunning;
  doc["mpu6050"]["accelX"] = accel.acceleration.x;
  doc["mpu6050"]["accelY"] = accel.acceleration.y;
  doc["mpu6050"]["accelZ"] = accel.acceleration.z;
  doc["mpu6050"]["totalAccel"] = currentTotalAccel;
  doc["mpu6050"]["roll"] = currentRoll;
  doc["crashDetected"] = crashDetected;
  doc["alcoholDetected"] = alcoholDetected;
  doc["autoEngineControl"] = autoEngineControl;
  doc["relayState"] = digitalRead(relayPin);
  doc["relayStatus"] = digitalRead(relayPin) ? "OFF" : "ON";
  doc["timestamp"] = millis();
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.PUT(payload);
  
  if (httpCode == HTTP_CODE_OK) {
    Serial.printf("[FIREBASE] ✅ Live data sent - Engine: %s\n", 
      engineRunning ? "RUNNING" : "STOPPED");
  }
  http.end();
}

// ✅ WiFi connection
void connectToWiFi() {
  Serial.println("\n[WIFI] Connecting...");
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
    Serial.println("\n[WIFI] ❌ Connection failed!");
  }
}

// ✅ Check alcohol status from Firebase
void checkAlcoholStatus() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  for (int i = 0; i < numAlcoholPaths; i++) {
    HTTPClient http;
    http.begin(firebaseHost + alcoholPaths[i]);
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      
      if (response == "null" || response.length() <= 2) {
        http.end();
        continue;
      }
      
      bool currentAlcoholState = false;
      
      // Check for danger indicators
      if (response.indexOf("Danger") != -1 || 
          response.indexOf("danger") != -1 ||
          response.indexOf("Detected") != -1) {
        currentAlcoholState = true;
      }
      
      // Check sensor value
      if (response.indexOf("sensorValue") != -1) {
        int sensorStart = response.indexOf("sensorValue") + 13;
        int sensorEnd = response.indexOf(",", sensorStart);
        if (sensorEnd == -1) sensorEnd = response.indexOf("}", sensorStart);
        
        if (sensorEnd > sensorStart) {
          String sensorStr = response.substring(sensorStart, sensorEnd);
          int sensorValue = sensorStr.toInt();
          
          if (sensorValue > 2000) {  // Threshold
            currentAlcoholState = true;
          }
        }
      }
      
      // Check for state change
      if (currentAlcoholState != lastAlcoholState) {
        lastAlcoholState = currentAlcoholState;
        
        if (currentAlcoholState) {
          Serial.println("\n🚨 ALCOHOL DETECTED FROM HELMET!");
          alcoholDetected = true;
          alcoholDetectionTime = millis();
          digitalWrite(relayPin, HIGH);  // Force OFF
          
          if (engineRunning) {
            triggerAlcoholShutdown();
          }
        } else {
          Serial.println("\n✅ Alcohol cleared");
          alcoholDetected = false;
        }
      }
      
      http.end();
      return;
    }
    http.end();
  }
}

// ✅ Alcohol shutdown
void triggerAlcoholShutdown() {
  Serial.println("\n🚨 ALCOHOL DETECTED - EMERGENCY SHUTDOWN!");
  
  digitalWrite(relayPin, HIGH);  // ACTIVE-LOW: HIGH = OFF
  engineRunning = false;
  
  Serial.printf("🚨 Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
  
  // Alert pattern
  for (int i = 0; i < 10; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(lightIndicatorPin, LOW);
    delay(300);
  }
}

// ✅ System status
void printSystemStatus() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🔍 SYSTEM STATUS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.printf("Relay: GPIO %d = %d (%s)\n", relayPin, digitalRead(relayPin), 
    digitalRead(relayPin) ? "OFF" : "ON");
  Serial.printf("Engine: %s\n", engineRunning ? "RUNNING" : "STOPPED");
  Serial.printf("Crash: %s\n", crashDetected ? "YES" : "NO");
  Serial.printf("Alcohol: %s\n", alcoholDetected ? "YES" : "NO");
  Serial.printf("Auto Control: %s\n", autoEngineControl ? "ON" : "OFF");
  Serial.printf("WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED" : "DISCONNECTED");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}

// ✅ Handle dashboard button
void handleDashboardButton() {
  if (millis() - lastButtonCheck > BUTTON_CHECK_INTERVAL) {
    checkDashboardButton();
    lastButtonCheck = millis();
  }
}

// ✅ Check dashboard button from Firebase
void checkDashboardButton() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  HTTPClient http;
  http.begin(firebaseHost + buttonPath);
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    bool currentButtonState = (response.indexOf("true") != -1);
    
    if (currentButtonState != lastDashboardButtonState) {
      lastDashboardButtonState = currentButtonState;
      
      if (currentButtonState) {
        Serial.println("\n🖥️ DASHBOARD BUTTON PRESSED!");
        
        if (!engineRunning) {
          engineStartRequested = true;
          startEngine();
        } else {
          stopEngine();
          engineStartRequested = false;
        }
        
        clearDashboardButton();
      }
    }
  }
  http.end();
}

// ✅ Clear dashboard button
void clearDashboardButton() {
  HTTPClient http;
  http.begin(firebaseHost + buttonPath);
  http.addHeader("Content-Type", "application/json");
  http.PUT("false");
  http.end();
}
