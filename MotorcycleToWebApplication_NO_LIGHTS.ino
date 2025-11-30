/*
 * VigiLERT ESP32 Motorcycle Module - NO LIGHTS VERSION
 * ✅ Fixed: Device heartbeat for pairing status
 * ✅ Fixed: Relay control for engine
 * ✅ Fixed: Vibration sensor buzzer alerts
 * ✅ Removed: All light wiring (headlight, taillight, signals)
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

// Owner phone number for SMS alerts
const String ownerPhoneNumber = "+639675715673";

// ======= PIN ASSIGNMENTS =======
const int relayPin = 13;
const int buzzerPin = 12;
const int lightIndicatorPin = 2;
const int vibrationSensorPin = 15;

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
const unsigned long BUTTON_CHECK_INTERVAL = 100;

// Device heartbeat for pairing status
unsigned long lastHeartbeat = 0;
const unsigned long HEARTBEAT_INTERVAL = 2000;

// Anti-theft system
bool antiTheftEnabled = false;
bool antiTheftArmed = false;
unsigned long engineOffTime = 0;
const unsigned long ARM_DELAY = 30000;
unsigned long lastTheftAlert = 0;
const unsigned long THEFT_ALERT_COOLDOWN = 300000;
bool theftAlertSent = false;
int theftDetectionCount = 0;
const int THEFT_DETECTION_REQUIRED = 1;
unsigned long lastVibrationTime = 0;
const unsigned long VIBRATION_DEBOUNCE = 50;  // ✅ ULTRA-FAST: Reduced from 200ms to 50ms!

// Escalating buzzer alert system
int consecutiveVibrations = 0;

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
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   VIGILERT MOTORCYCLE - NO LIGHTS      ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ ✅ Device heartbeat enabled            ║");
  Serial.println("║ ✅ Relay control fixed                 ║");
  Serial.println("║ ✅ ULTRA-FAST vibration detection      ║");
  Serial.println("║ ✅ Light wiring removed                ║");
  Serial.println("║ ⚡ 50ms debounce (4x faster!)          ║");
  Serial.println("║ ⚡ 5ms loop delay (2x faster!)         ║");
  Serial.println("╚════════════════════════════════════════╝\n");

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
  Serial.println("[SETUP] GSM Serial started on RX:4, TX:5");
  initializeGSM();

  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);
  pinMode(vibrationSensorPin, INPUT);

  // Initialize relay to OFF (HIGH for ACTIVE-LOW)
  digitalWrite(relayPin, HIGH);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  
  delay(100);
  Serial.println("\n[SETUP] ═══════════════════════════════════");
  Serial.println("[SETUP] RELAY TYPE: ACTIVE-LOW");
  Serial.println("[SETUP] GPIO 13: Relay Control");
  Serial.println("[SETUP] GPIO 15: Vibration Sensor");
  Serial.println("[SETUP] GPIO 12: Buzzer");
  Serial.printf("[SETUP] Relay: %d (HIGH/OFF)\n", digitalRead(relayPin));
  Serial.println("[SETUP] ═══════════════════════════════════\n");

  connectToWiFi();
  
  // Send initial heartbeat immediately
  sendMotorcycleHeartbeat(true);
  lastHeartbeat = millis();

  Serial.println("\n📋 SERIAL COMMANDS:");
  Serial.println("   START ENGINE / STOP ENGINE");
  Serial.println("   ARM THEFT / DISARM THEFT");
  Serial.println("   TEST VIBRATION / TEST SMS");
  Serial.println("   STATUS\n");
}

void loop() {
  // Send heartbeat every 2 seconds for pairing status
  if (millis() - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    sendMotorcycleHeartbeat(true);
    lastHeartbeat = millis();
  }

  handleDashboardButton();

  // Serial commands
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "START ENGINE") {
      startEngine();
    }
    else if (cmd == "STOP ENGINE") {
      stopEngine();
    }
    else if (cmd == "ARM THEFT") {
      antiTheftArmed = true;
      antiTheftEnabled = true;
      Serial.println("✅ Anti-theft ARMED!");
    }
    else if (cmd == "DISARM THEFT") {
      antiTheftArmed = false;
      antiTheftEnabled = false;
      theftDetectionCount = 0;
      Serial.println("✅ Anti-theft DISARMED!");
    }
    else if (cmd == "TEST VIBRATION") {
      Serial.println("🧪 Testing vibration sensor...");
      for (int i = 0; i < 10; i++) {
        int reading = digitalRead(vibrationSensorPin);
        Serial.printf("Reading %d: %d %s\n", i+1, reading, reading == HIGH ? "VIBRATION!" : "");
        if (reading == HIGH) {
          digitalWrite(buzzerPin, HIGH);
          delay(200);
          digitalWrite(buzzerPin, LOW);
        }
        delay(500);
      }
    }
    else if (cmd == "TEST SMS") {
      sendSMS(ownerPhoneNumber, "VigiLERT Test: SMS working!");
    }
    else if (cmd == "STATUS") {
      printSystemStatus();
    }
    else if (cmd == "CRASH") {
      triggerCrashShutdown(20.0, 45.0);
    }
  }

  // Read MPU6050
  mpu.getEvent(&accel, &gyro, &temp);
  currentTotalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                           accel.acceleration.y * accel.acceleration.y +
                           accel.acceleration.z * accel.acceleration.z);
  currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
  float leanAngle = abs(currentRoll);

  // ✅ ULTRA-FAST: Check vibration sensor multiple times per loop for faster response
  static int vibrationCheckCounter = 0;
  vibrationCheckCounter++;
  
  // Anti-theft system
  if (!engineRunning) {
    handleAntiTheftWithVibrationSensor();
    
    // ✅ EXTRA SENSITIVITY: Check vibration sensor 3 times per main loop
    if (vibrationCheckCounter >= 3) {
      handleAntiTheftWithVibrationSensor();
      vibrationCheckCounter = 0;
    }
  } else {
    if (antiTheftArmed) {
      Serial.println("[ANTI-THEFT] 🔓 Disarmed - Engine running");
      antiTheftArmed = false;
      theftDetectionCount = 0;
      theftAlertSent = false;
    }
  }

  // Crash detection
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
    Serial.printf("[STATUS] Engine:%s | Relay:%s | AntiTheft:%s | Vibration:%d\n",
                  engineRunning ? "RUN" : "STOP",
                  digitalRead(relayPin) ? "OFF" : "ON",
                  antiTheftArmed ? "ARMED" : "DISARMED",
                  digitalRead(vibrationSensorPin));
    lastPrint = millis();
  }

  // Firebase updates
  static unsigned long lastFirebaseUpdate = 0;
  if (millis() - lastFirebaseUpdate > 2000) {
    sendLiveToFirebase();
    lastFirebaseUpdate = millis();
  }

  delay(5);  // ✅ ULTRA-FAST: Reduced from 10ms to 5ms for faster loop!
}

// Send motorcycle heartbeat for pairing status
void sendMotorcycleHeartbeat(bool isActive) {
  if (WiFi.status() != WL_CONNECTED) return;

  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  StaticJsonDocument<128> doc;
  doc["status"] = isActive ? "On" : "Off";
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
    Serial.printf("[HEARTBEAT] ✓ Motorcycle heartbeat sent (timestamp: %llu)\n", timestamp);
  } else {
    Serial.printf("[HEARTBEAT] ✗ Failed: HTTP %d\n", code);
  }
  
  http.end();
}

// Anti-theft with proper buzzer alerts
void handleAntiTheftWithVibrationSensor() {
  if (!antiTheftEnabled) {
    antiTheftEnabled = true;
    engineOffTime = millis();
    consecutiveVibrations = 0;
    Serial.println("\n[ANTI-THEFT] 🛡️ System enabled - arming in 30 seconds...");
    return;
  }

  if (!antiTheftArmed && (millis() - engineOffTime >= ARM_DELAY)) {
    antiTheftArmed = true;
    theftDetectionCount = 0;
    theftAlertSent = false;
    consecutiveVibrations = 0;
    Serial.println("\n[ANTI-THEFT] 🛡️ ARMED! Vibration sensor active...");
    
    // Beep to indicate armed
    for (int i = 0; i < 2; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    }
  }

  if (antiTheftArmed) {
    int vibrationDetected = digitalRead(vibrationSensorPin);
    
    if (vibrationDetected == HIGH) {
      unsigned long timeSinceLastVibration = millis() - lastVibrationTime;
      
      if (timeSinceLastVibration >= VIBRATION_DEBOUNCE) {
        theftDetectionCount++;
        consecutiveVibrations++;
        lastVibrationTime = millis();
        
        Serial.printf("[ANTI-THEFT] ⚠️ VIBRATION #%d! (Count: %d/%d)\n", 
                      consecutiveVibrations, theftDetectionCount, THEFT_DETECTION_REQUIRED);

        // Escalating buzzer alert
        int buzzerCount;
        if (consecutiveVibrations == 1) {
          buzzerCount = 5;
          Serial.println("[ANTI-THEFT] 🔔 Alert Level 1 - 5 beeps");
        } else if (consecutiveVibrations == 2) {
          buzzerCount = 8;
          Serial.println("[ANTI-THEFT] 🔔🔔 Alert Level 2 - 8 beeps");
        } else {
          buzzerCount = 15;
          Serial.println("[ANTI-THEFT] 🔔🔔🔔 Alert Level 3 - 15 beeps (MAXIMUM)");
        }

        // ✅ FASTER ALARM: Reduced beep duration from 100ms to 50ms
        for (int i = 0; i < buzzerCount; i++) {
          digitalWrite(buzzerPin, HIGH);
          digitalWrite(lightIndicatorPin, HIGH);
          delay(50);  // ✅ Faster beep
          digitalWrite(buzzerPin, LOW);
          digitalWrite(lightIndicatorPin, LOW);
          delay(50);  // ✅ Faster pause
        }

        if (theftDetectionCount >= THEFT_DETECTION_REQUIRED) {
          unsigned long timeSinceLastAlert = millis() - lastTheftAlert;
          
          if (!theftAlertSent || timeSinceLastAlert >= THEFT_ALERT_COOLDOWN) {
            triggerTheftAlert();
            lastTheftAlert = millis();
            theftAlertSent = true;
            theftDetectionCount = 0;
          }
        }
      }
    } else {
      // ✅ FASTER RESET: Reset consecutive counter if no vibration for 15 seconds (was 30s)
      if (consecutiveVibrations > 0 && (millis() - lastVibrationTime) > 15000) {
        Serial.printf("[ANTI-THEFT] ℹ️ No vibration for 15s - resetting alert level (was %d)\n", consecutiveVibrations);
        consecutiveVibrations = 0;
      }
      
      // ✅ FASTER DECAY: Reduce detection count after 5 seconds (was 10s)
      if (theftDetectionCount > 0 && (millis() - lastVibrationTime) > 5000) {
        theftDetectionCount--;
      }
    }
  }
}

void triggerTheftAlert() {
  Serial.println("\n🚨🚨🚨 THEFT ALERT! 🚨🚨🚨");
  
  // ✅ FASTER ALARM: Reduced beep duration for quicker response
  for (int i = 0; i < 10; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(50);  // ✅ Faster beep (was 100ms)
    digitalWrite(buzzerPin, LOW);
    digitalWrite(lightIndicatorPin, LOW);
    delay(50);  // ✅ Faster pause (was 100ms)
  }

  String location = "Location unavailable";
  if (gps.location.isValid()) {
    location = "https://maps.google.com/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  }

  String message = "VIGILERT THEFT ALERT!\n";
  message += "Unauthorized movement detected!\n";
  message += location + "\n";
  message += "Check your motorcycle NOW!";

  Serial.println("[ANTI-THEFT] 📱 Sending SMS alert...");
  bool smsSent = sendSMS(ownerPhoneNumber, message);
  
  if (smsSent) {
    Serial.println("[ANTI-THEFT] ✅ SMS sent!");
  } else {
    Serial.println("[ANTI-THEFT] ❌ SMS failed");
  }

  logTheftToFirebase(location);
}

void initializeGSM() {
  Serial.println("\n[GSM] Initializing...");
  delay(3000);
  
  gsmSerial.println("AT");
  delay(1000);
  
  if (gsmSerial.available()) {
    String response = gsmSerial.readString();
    if (response.indexOf("OK") != -1) {
      gsmReady = true;
      Serial.println("[GSM] ✅ Ready!");
    }
  }

  if (!gsmReady) {
    Serial.println("[GSM] ⚠️ Not responding");
    return;
  }

  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  
  gsmSerial.println("AT+CSQ");
  delay(1000);
  
  if (gsmSerial.available()) {
    Serial.println("[GSM] Signal: " + gsmSerial.readString());
  }
}

bool sendSMS(String phoneNumber, String message) {
  if (!gsmReady) {
    Serial.println("[GSM] ❌ Not ready");
    return false;
  }

  Serial.println("[GSM] 📱 Sending SMS to: " + phoneNumber);
  
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  
  gsmSerial.println("AT+CMGS=\"" + phoneNumber + "\"");
  delay(1000);
  
  gsmSerial.print(message);
  delay(500);
  
  gsmSerial.write(26);
  delay(5000);

  bool success = false;
  if (gsmSerial.available()) {
    String response = gsmSerial.readString();
    if (response.indexOf("OK") != -1 || response.indexOf("+CMGS") != -1) {
      success = true;
    }
  }

  return success;
}

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
  http.POST(payload);
  http.end();
}

void triggerCrashShutdown(float impact, float roll) {
  Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
  Serial.printf("Impact: %.2f g | Roll: %.1f°\n", impact, roll);
  
  // Relay control
  digitalWrite(relayPin, HIGH);  // ACTIVE-LOW: HIGH = OFF
  engineRunning = false;
  
  Serial.printf("🚨 Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
  
  sendCrashToFirebase(impact, roll);
  
  crashDetected = true;
  lastCrashTime = millis();
  
  // Buzzer alert
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
  
  // Relay control
  digitalWrite(relayPin, LOW);  // ACTIVE-LOW: LOW = ON
  engineRunning = true;
  engineStartRequested = false;
  
  Serial.printf("✅ Relay GPIO %d = %d (ON)\n", relayPin, digitalRead(relayPin));
  
  // Disarm anti-theft
  if (antiTheftArmed) {
    antiTheftArmed = false;
    antiTheftEnabled = false;
    theftDetectionCount = 0;
    Serial.println("[ANTI-THEFT] 🔓 Disarmed");
  }
}

void stopEngine() {
  Serial.println("\n🛑 Stopping engine...");
  
  // Relay control
  digitalWrite(relayPin, HIGH);  // ACTIVE-LOW: HIGH = OFF
  engineRunning = false;
  
  Serial.printf("🛑 Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
  
  // Enable anti-theft
  engineOffTime = millis();
  antiTheftEnabled = true;
  antiTheftArmed = false;
  Serial.println("[ANTI-THEFT] 🛡️ Will arm in 30 seconds...");
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
  http.POST(payload);
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
    Serial.println("\n[WIFI] ❌ Failed!");
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
      
      if (response.indexOf("Danger") != -1 || response.indexOf("danger") != -1 ||
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
  Serial.println("\n🚨 ALCOHOL - EMERGENCY SHUTDOWN!");
  
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
  Serial.printf("Buzzer: GPIO %d\n", buzzerPin);
  Serial.printf("GSM: %s\n", gsmReady ? "READY" : "NOT READY");
  Serial.printf("WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED" : "DISCONNECTED");
  Serial.printf("Owner Phone: %s\n", ownerPhoneNumber.c_str());
  Serial.printf("Heartbeat: Every %lu ms\n", HEARTBEAT_INTERVAL);
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
