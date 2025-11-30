/* 
 * VigiLERT ESP32 Motorcycle Module - ANTI-THEFT with Vibration Sensor
 * ✅ All previous features
 * ✅ NEW: Dedicated vibration sensor for anti-theft
 * ✅ NEW: GSM SMS alerts for unauthorized movement
 * ✅ Separate sensors: MPU6050 for crash, Vibration sensor for theft
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

// ✅ Owner phone number for SMS alerts
const String ownerPhoneNumber = "+639675715673";

// ======= PIN ASSIGNMENTS =======
const int relayPin = 13;
const int buzzerPin = 12;
const int lightIndicatorPin = 2;

// ✅ NEW: Vibration sensor pin (digital output - HIGH when vibration detected)
const int vibrationSensorPin = 15;  // GPIO 15 - Change if needed

// GSM module pins (UART2)
#define GSM_RX 4
#define GSM_TX 5
HardwareSerial gsmSerial(2);

// GPS module pins (UART1)
HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

// MPU6050 (for crash detection only)
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
const unsigned long CRASH_COOLDOWN = 5000;

// Alcohol detection
bool alcoholDetected = false;
bool lastAlcoholState = false;
unsigned long lastAlcoholCheck = 0;
const unsigned long ALCOHOL_CHECK_INTERVAL = 500;
unsigned long alcoholDetectionTime = 0;

// Automatic engine control
bool autoEngineControl = false;
bool engineStartRequested = false;

// Dashboard button control
bool lastDashboardButtonState = false;
unsigned long lastButtonCheck = 0;
const unsigned long BUTTON_CHECK_INTERVAL = 1000;

// ✅ Anti-theft system with vibration sensor
bool antiTheftEnabled = false;
bool antiTheftArmed = false;
unsigned long engineOffTime = 0;
const unsigned long ARM_DELAY = 30000;  // 30 seconds to arm
unsigned long lastTheftAlert = 0;
const unsigned long THEFT_ALERT_COOLDOWN = 300000;  // 5 minutes between alerts
bool theftAlertSent = false;
int theftDetectionCount = 0;
const int THEFT_DETECTION_REQUIRED = 3;  // Require 3 detections
unsigned long lastVibrationTime = 0;
const unsigned long VIBRATION_DEBOUNCE = 500;  // 500ms debounce

// GSM status
bool gsmReady = false;

// Firebase paths
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
const String buttonPath = "/" + userUID + "/engineControl/startButton.json?auth=" + firebaseAuth;
const String livePath = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;
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
  
  Serial.println("\n[SETUP] Starting VigiLERT System - ANTI-THEFT with Vibration Sensor...");
  
  // Initialize MPU6050 (for crash detection)
  Wire.begin(21, 22);
  while (!mpu.begin()) {
    Serial.println("[ERROR] Failed to find MPU6050 chip. Retrying in 2s...");
    delay(2000);
  }
  Serial.println("[SUCCESS] MPU6050 initialized for crash detection.");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  // Initialize GPS
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("[SETUP] GPS Serial started on RX:16, TX:17");
  
  // Initialize GSM
  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);
  Serial.println("[SETUP] GSM Serial started on RX:4, TX:5");
  initializeGSM();
  
  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);
  pinMode(vibrationSensorPin, INPUT);  // ✅ Vibration sensor as input
  
  digitalWrite(relayPin, HIGH);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  delay(100);
  
  Serial.println("\n[SETUP] ═══════════════════════════════════");
  Serial.println("[SETUP] RELAY TYPE: ACTIVE-LOW");
  Serial.println("[SETUP] GPIO 13: Relay Control");
  Serial.println("[SETUP] GPIO 15: Vibration Sensor (Anti-Theft)");
  Serial.println("[SETUP] HIGH = Relay OFF (Engine Blocked)");
  Serial.println("[SETUP] LOW = Relay ON (Engine Allowed)");
  Serial.printf("[SETUP] Relay: %d (HIGH/OFF)\n", digitalRead(relayPin));
  Serial.println("[SETUP] ═══════════════════════════════════\n");
  
  Serial.println("[SETUP] 🛡️ ANTI-THEFT SYSTEM READY");
  Serial.println("[SETUP] Vibration sensor will activate 30s after engine stops");
  Serial.println("[SETUP] SMS alerts to: " + ownerPhoneNumber);
  
  connectToWiFi();
  
  Serial.println("\n📋 SERIAL COMMANDS:");
  Serial.println("   CRASH - Test crash detection");
  Serial.println("   ALCOHOL - Test alcohol detection");
  Serial.println("   AUTO ON/OFF - Toggle automatic control");
  Serial.println("   START ENGINE / STOP ENGINE - Manual control");
  Serial.println("   ARM THEFT - Manually arm anti-theft");
  Serial.println("   DISARM THEFT - Disarm anti-theft");
  Serial.println("   TEST THEFT - Test theft detection");
  Serial.println("   TEST SMS - Test SMS sending");
  Serial.println("   TEST VIBRATION - Test vibration sensor");
  Serial.println("   STATUS - System status\n");
}

void loop() {
  handleDashboardButton();
  
  // Serial commands
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "CRASH") {
      triggerCrashShutdown(20.0, 45.0);
    }
    else if (cmd == "AUTO ON") {
      autoEngineControl = true;
      Serial.println("✅ Auto control enabled");
    }
    else if (cmd == "AUTO OFF") {
      autoEngineControl = false;
      Serial.println("🛑 Auto control disabled");
    }
    else if (cmd == "STATUS") {
      printSystemStatus();
    }
    else if (cmd == "ALCOHOL") {
      alcoholDetected = true;
      digitalWrite(relayPin, HIGH);
      if (engineRunning) triggerAlcoholShutdown();
    }
    else if (cmd == "START ENGINE") {
      startEngine();
    }
    else if (cmd == "STOP ENGINE") {
      stopEngine();
    }
    else if (cmd == "ARM THEFT") {
      Serial.println("🛡️ Manually arming anti-theft...");
      antiTheftArmed = true;
      antiTheftEnabled = true;
      Serial.println("✅ Anti-theft ARMED!");
    }
    else if (cmd == "DISARM THEFT") {
      Serial.println("🔓 Disarming anti-theft...");
      antiTheftArmed = false;
      antiTheftEnabled = false;
      theftDetectionCount = 0;
      Serial.println("✅ Anti-theft DISARMED!");
    }
    else if (cmd == "TEST THEFT") {
      Serial.println("🧪 Testing theft detection...");
      triggerTheftAlert();
    }
    else if (cmd == "TEST SMS") {
      Serial.println("🧪 Testing SMS...");
      sendSMS(ownerPhoneNumber, "VigiLERT Test: SMS system working!");
    }
    else if (cmd == "TEST VIBRATION") {
      Serial.println("🧪 Testing vibration sensor...");
      Serial.printf("Vibration sensor pin %d = %d\n", vibrationSensorPin, digitalRead(vibrationSensorPin));
      Serial.println("Shake the motorcycle to test...");
      for (int i = 0; i < 20; i++) {
        int reading = digitalRead(vibrationSensorPin);
        Serial.printf("Reading %d: %d %s\n", i+1, reading, reading == HIGH ? "VIBRATION!" : "");
        delay(500);
      }
    }
  }
  
  // Read MPU6050 (for crash detection only)
  mpu.getEvent(&accel, &gyro, &temp);
  
  currentTotalAccel = sqrt(
    accel.acceleration.x * accel.acceleration.x +
    accel.acceleration.y * accel.acceleration.y +
    accel.acceleration.z * accel.acceleration.z
  );
  
  currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
  float leanAngle = abs(currentRoll);
  
  // ✅ ANTI-THEFT SYSTEM - Uses vibration sensor when engine OFF
  if (!engineRunning) {
    handleAntiTheftWithVibrationSensor();
  } else {
    // Reset anti-theft when engine is running
    if (antiTheftArmed) {
      Serial.println("[ANTI-THEFT] 🔓 Disarmed - Engine running");
      antiTheftArmed = false;
      theftDetectionCount = 0;
      theftAlertSent = false;
    }
  }
  
  // Crash detection (only when engine running)
  if (engineRunning && (currentTotalAccel >= ACCEL_THRESHOLD || leanAngle > 40) && !crashDetected) {
    unsigned long timeSinceLastCrash = millis() - lastCrashTime;
    if (timeSinceLastCrash >= CRASH_COOLDOWN) {
      triggerCrashShutdown(currentTotalAccel, currentRoll);
    }
  }
  
  if (crashDetected) {
    bool isUpright = (leanAngle < 30);
    bool isStable = (currentTotalAccel < ACCEL_THRESHOLD - 2.0);
    if (isUpright && isStable) {
      crashDetected = false;
      Serial.println("[INFO] ✓ Crash cleared");
    }
  }
  
  // Alcohol monitoring
  if (millis() - lastAlcoholCheck > ALCOHOL_CHECK_INTERVAL) {
    checkAlcoholStatus();
    lastAlcoholCheck = millis();
  }
  
  // Automatic engine control
  if (autoEngineControl) {
    if (alcoholDetected && engineRunning) {
      Serial.println("\n🚨 AUTO-SHUTDOWN: Alcohol detected!");
      stopEngine();
      engineStartRequested = true;
    }
    else if (!alcoholDetected && engineStartRequested && !engineRunning) {
      Serial.println("\n✅ AUTO-START: Alcohol cleared!");
      startEngine();
    }
  }
  
  // Safety override
  if (alcoholDetected) {
    digitalWrite(relayPin, HIGH);
  }
  
  // Status monitoring
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 5000) {
    Serial.printf("[STATUS] Engine:%s | Relay:%s | Alcohol:%s | AntiTheft:%s | Vibration:%d\n",
      engineRunning ? "RUN" : "STOP",
      digitalRead(relayPin) ? "OFF" : "ON",
      alcoholDetected ? "DANGER" : "SAFE",
      antiTheftArmed ? "ARMED" : "DISARMED",
      digitalRead(vibrationSensorPin)
    );
    lastPrint = millis();
  }
  
  // Firebase updates
  static unsigned long lastFirebaseUpdate = 0;
  if (millis() - lastFirebaseUpdate > 2000) {
    sendLiveToFirebase();
    lastFirebaseUpdate = millis();
  }
  
  delay(50);
}

// ✅ NEW: Anti-theft handler with dedicated vibration sensor
void handleAntiTheftWithVibrationSensor() {
  // Enable anti-theft when engine stops
  if (!antiTheftEnabled) {
    antiTheftEnabled = true;
    engineOffTime = millis();
    Serial.println("\n[ANTI-THEFT] 🛡️ System enabled - arming in 30 seconds...");
    Serial.println("[ANTI-THEFT] Vibration sensor will monitor for movement");
    return;
  }
  
  // Arm after delay
  if (!antiTheftArmed && (millis() - engineOffTime >= ARM_DELAY)) {
    antiTheftArmed = true;
    theftDetectionCount = 0;
    theftAlertSent = false;
    Serial.println("\n[ANTI-THEFT] 🛡️ ARMED! Vibration sensor active...");
    
    // Beep to indicate armed
    for (int i = 0; i < 2; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    }
  }
  
  // Monitor vibration sensor when armed
  if (antiTheftArmed) {
    int vibrationDetected = digitalRead(vibrationSensorPin);
    
    // Check for vibration (HIGH = vibration detected)
    if (vibrationDetected == HIGH) {
      unsigned long timeSinceLastVibration = millis() - lastVibrationTime;
      
      // Debounce - only count if enough time has passed
      if (timeSinceLastVibration >= VIBRATION_DEBOUNCE) {
        theftDetectionCount++;
        lastVibrationTime = millis();
        
        Serial.printf("[ANTI-THEFT] ⚠️ VIBRATION DETECTED! (Count: %d/%d)\n", 
          theftDetectionCount, THEFT_DETECTION_REQUIRED);
        
        // Flash LED to indicate detection
        digitalWrite(lightIndicatorPin, HIGH);
        delay(100);
        digitalWrite(lightIndicatorPin, LOW);
        
        // Require multiple detections to avoid false alarms
        if (theftDetectionCount >= THEFT_DETECTION_REQUIRED) {
          unsigned long timeSinceLastAlert = millis() - lastTheftAlert;
          
          if (!theftAlertSent || timeSinceLastAlert >= THEFT_ALERT_COOLDOWN) {
            triggerTheftAlert();
            lastTheftAlert = millis();
            theftAlertSent = true;
            theftDetectionCount = 0;  // Reset counter
          }
        }
      }
    } else {
      // Decay detection count if no vibration for 10 seconds
      if (theftDetectionCount > 0 && (millis() - lastVibrationTime) > 10000) {
        theftDetectionCount--;
        Serial.printf("[ANTI-THEFT] Detection count decayed to %d\n", theftDetectionCount);
      }
    }
  }
}

// ✅ Trigger theft alert with SMS
void triggerTheftAlert() {
  Serial.println("\n🚨🚨🚨 THEFT ALERT! UNAUTHORIZED MOVEMENT DETECTED! 🚨🚨🚨");
  
  // Sound alarm
  for (int i = 0; i < 10; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(lightIndicatorPin, LOW);
    delay(100);
  }
  
  // Get GPS location if available
  String location = "Location unavailable";
  if (gps.location.isValid()) {
    location = "https://maps.google.com/?q=" + 
               String(gps.location.lat(), 6) + "," + 
               String(gps.location.lng(), 6);
  }
  
  // Send SMS alert
  String message = "VIGILERT THEFT ALERT!\n";
  message += "Unauthorized movement detected!\n";
  message += location + "\n";
  message += "Check your motorcycle NOW!";
  
  Serial.println("[ANTI-THEFT] 📱 Sending SMS alert...");
  bool smsSent = sendSMS(ownerPhoneNumber, message);
  
  if (smsSent) {
    Serial.println("[ANTI-THEFT] ✅ SMS alert sent successfully!");
  } else {
    Serial.println("[ANTI-THEFT] ❌ Failed to send SMS alert");
  }
  
  // Log to Firebase
  logTheftToFirebase(location);
  
  Serial.println("[ANTI-THEFT] 🚨 Alert complete - monitoring continues...");
}

// ✅ Initialize GSM module
void initializeGSM() {
  Serial.println("\n[GSM] Initializing module...");
  
  delay(3000);  // Wait for GSM to boot
  
  // Test AT command
  gsmSerial.println("AT");
  delay(1000);
  if (gsmSerial.available()) {
    String response = gsmSerial.readString();
    Serial.println("[GSM] Response: " + response);
    if (response.indexOf("OK") != -1) {
      gsmReady = true;
      Serial.println("[GSM] ✅ Module ready!");
    }
  }
  
  if (!gsmReady) {
    Serial.println("[GSM] ⚠️ Module not responding - SMS alerts may not work");
    return;
  }
  
  // Set SMS mode to text
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  
  // Check signal strength
  gsmSerial.println("AT+CSQ");
  delay(1000);
  if (gsmSerial.available()) {
    String signal = gsmSerial.readString();
    Serial.println("[GSM] Signal: " + signal);
  }
  
  Serial.println("[GSM] ✅ Initialization complete!");
}

// ✅ Send SMS function
bool sendSMS(String phoneNumber, String message) {
  if (!gsmReady) {
    Serial.println("[GSM] ❌ Module not ready");
    return false;
  }
  
  Serial.println("[GSM] 📱 Sending SMS to: " + phoneNumber);
  Serial.println("[GSM] Message: " + message);
  
  // Set SMS mode
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  
  // Set phone number
  gsmSerial.println("AT+CMGS=\"" + phoneNumber + "\"");
  delay(1000);
  
  // Send message
  gsmSerial.print(message);
  delay(500);
  
  // Send Ctrl+Z to indicate end of message
  gsmSerial.write(26);
  delay(5000);  // Wait for send confirmation
  
  // Check response
  bool success = false;
  if (gsmSerial.available()) {
    String response = gsmSerial.readString();
    Serial.println("[GSM] Response: " + response);
    if (response.indexOf("OK") != -1 || response.indexOf("+CMGS") != -1) {
      success = true;
    }
  }
  
  return success;
}

// ✅ Log theft event to Firebase
void logTheftToFirebase(String location) {
  if (WiFi.status() != WL_CONNECTED) return;
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = millis();
  doc["type"] = "theft_attempt";
  doc["location"] = location;
  doc["vibrationDetected"] = true;
  doc["armed"] = antiTheftArmed;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String theftPath = "/helmet_public/" + userUID + "/theft_alerts.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + theftPath);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(payload);
  
  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
    Serial.println("[FIREBASE] ✅ Theft alert logged");
  }
  http.end();
}

void triggerCrashShutdown(float impact, float roll) {
  Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
  Serial.printf("Impact: %.2f g | Roll: %.1f°\n", impact, roll);
  
  digitalWrite(relayPin, HIGH);
  engineRunning = false;
  
  sendCrashToFirebase(impact, roll);
  crashDetected = true;
  lastCrashTime = millis();
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(lightIndicatorPin, LOW);
    delay(200);
  }
}

void startEngine() {
  if (alcoholDetected) {
    Serial.println("\n❌ ENGINE START BLOCKED - ALCOHOL DETECTED!");
    if (autoEngineControl) {
      engineStartRequested = true;
    }
    digitalWrite(relayPin, HIGH);
    for (int i = 0; i < 3; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(buzzerPin, LOW);
      delay(200);
    }
    return;
  }
  
  Serial.println("\n✅ Starting engine...");
  digitalWrite(relayPin, LOW);
  engineRunning = true;
  engineStartRequested = false;
  
  // Disarm anti-theft when engine starts
  if (antiTheftArmed) {
    antiTheftArmed = false;
    antiTheftEnabled = false;
    theftDetectionCount = 0;
    Serial.println("[ANTI-THEFT] 🔓 Disarmed - Engine started");
  }
  
  Serial.printf("✅ Relay GPIO %d = %d (ON)\n", relayPin, digitalRead(relayPin));
}

void stopEngine() {
  Serial.println("\n🛑 Stopping engine...");
  digitalWrite(relayPin, HIGH);
  engineRunning = false;
  
  // Enable anti-theft when engine stops
  engineOffTime = millis();
  antiTheftEnabled = true;
  antiTheftArmed = false;
  Serial.println("[ANTI-THEFT] 🛡️ Will arm in 30 seconds...");
  
  Serial.printf("🛑 Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
}

void sendCrashToFirebase(float impact, float roll) {
  if (WiFi.status() != WL_CONNECTED) return;
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = millis();
  doc["impactStrength"] = impact;
  doc["roll"] = roll;
  doc["leanAngle"] = abs(roll);
  doc["hasGPS"] = gps.location.isValid();
  if (gps.location.isValid()) {
    doc["lat"] = gps.location.lat();
    doc["lng"] = gps.location.lng();
  }
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(payload);
  
  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
    Serial.println("[FIREBASE] ✅ Crash data sent");
  }
  http.end();
}

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
  doc["antiTheftArmed"] = antiTheftArmed;
  doc["vibrationSensor"] = digitalRead(vibrationSensorPin);
  doc["relayState"] = digitalRead(relayPin);
  doc["relayStatus"] = digitalRead(relayPin) ? "OFF" : "ON";
  doc["timestamp"] = millis();
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.PUT(payload);
  http.end();
}

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
      
      if (response.indexOf("Danger") != -1 || 
          response.indexOf("danger") != -1 ||
          response.indexOf("Detected") != -1) {
        currentAlcoholState = true;
      }
      
      if (response.indexOf("sensorValue") != -1) {
        int sensorStart = response.indexOf("sensorValue") + 13;
        int sensorEnd = response.indexOf(",", sensorStart);
        if (sensorEnd == -1) sensorEnd = response.indexOf("}", sensorStart);
        
        if (sensorEnd > sensorStart) {
          String sensorStr = response.substring(sensorStart, sensorEnd);
          int sensorValue = sensorStr.toInt();
          if (sensorValue > 2000) {
            currentAlcoholState = true;
          }
        }
      }
      
      if (currentAlcoholState != lastAlcoholState) {
        lastAlcoholState = currentAlcoholState;
        
        if (currentAlcoholState) {
          Serial.println("\n🚨 ALCOHOL DETECTED!");
          alcoholDetected = true;
          digitalWrite(relayPin, HIGH);
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

void triggerAlcoholShutdown() {
  Serial.println("\n🚨 ALCOHOL DETECTED - EMERGENCY SHUTDOWN!");
  
  digitalWrite(relayPin, HIGH);
  engineRunning = false;
  
  for (int i = 0; i < 10; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(lightIndicatorPin, LOW);
    delay(300);
  }
}

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
  Serial.printf("Anti-Theft: %s\n", antiTheftArmed ? "ARMED" : "DISARMED");
  Serial.printf("Vibration Sensor: GPIO %d = %d\n", vibrationSensorPin, digitalRead(vibrationSensorPin));
  Serial.printf("GSM: %s\n", gsmReady ? "READY" : "NOT READY");
  Serial.printf("WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED" : "DISCONNECTED");
  Serial.printf("Owner Phone: %s\n", ownerPhoneNumber.c_str());
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}

void handleDashboardButton() {
  if (millis() - lastButtonCheck > BUTTON_CHECK_INTERVAL) {
    checkDashboardButton();
    lastButtonCheck = millis();
  }
}

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

void clearDashboardButton() {
  HTTPClient http;
  http.begin(firebaseHost + buttonPath);
  http.addHeader("Content-Type", "application/json");
  http.PUT("false");
  http.end();
}
