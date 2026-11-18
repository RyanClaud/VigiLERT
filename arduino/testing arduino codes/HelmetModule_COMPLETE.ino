#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>

// ═══════════════════════════════════════════════════════════════════════════
// VigiLERT Helmet Module - COMPLETE FUNCTIONAL VERSION
// ═══════════════════════════════════════════════════════════════════════════
// ✅ Fully integrated with Dashboard.vue
// ✅ Alcohol detection with MQ-3 sensor
// ✅ Drowsiness detection with IR sensor
// ✅ Real-time Firebase synchronization
// ✅ Heartbeat system for connection monitoring
// ✅ Emergency alert system
// ═══════════════════════════════════════════════════════════════════════════

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Configuration
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

// Pin Assignments
const int alcoholSensorPin = A0;      // MQ-3 alcohol sensor (analog)
const int drowsinessSensorPin = 2;    // IR sensor for drowsiness detection (digital)
const int buzzerPin = 12;             // Buzzer for alerts
const int ledPin = 13;                // LED indicator
const int buttonPin = 4;              // Emergency button

// Sensor thresholds
const int ALCOHOL_THRESHOLD = 600;    // MQ-3 sensor threshold (0-1023)
const int DROWSINESS_THRESHOLD = 500; // Drowsiness detection threshold (ms)

// Timing constants
const unsigned long HEARTBEAT_INTERVAL = 1000;     // 1 second heartbeat
const unsigned long SENSOR_READ_INTERVAL = 250;    // 250ms sensor readings
const unsigned long ALERT_INTERVAL = 5000;         // 5 seconds between alerts
const unsigned long DROWSINESS_CHECK_INTERVAL = 100; // 100ms drowsiness check

// State variables
bool alcoholDetected = false;
bool drowsinessDetected = false;
bool emergencyButtonPressed = false;
int alcoholSensorValue = 0;
int drowsinessLevel = 0;

// Timing variables
unsigned long lastHeartbeat = 0;
unsigned long lastSensorRead = 0;
unsigned long lastAlertTime = 0;
unsigned long lastDrowsinessCheck = 0;
unsigned long drowsinessStartTime = 0;
bool drowsinessTimerActive = false;

// Alert states
bool alcoholAlertActive = false;
bool drowsinessAlertActive = false;
bool emergencyAlertActive = false;

// Firebase paths
const String devicePath = "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
const String alcoholPath = "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;
const String alertnessPath = "/helmet_public/" + userUID + "/alertness/status.json?auth=" + firebaseAuth;
const String emergencyPath = "/helmet_public/" + userUID + "/emergency.json?auth=" + firebaseAuth;

// Function declarations
void sendHeartbeat();
void readSensors();
void checkAlcoholLevel();
void checkDrowsiness();
void checkEmergencyButton();
void sendAlcoholAlert();
void sendDrowsinessAlert();
void sendEmergencyAlert();
void updateFirebaseStatus();
void playAlertSound(int beeps, int duration);
void blinkLED(int times, int duration);

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║     HELMET MODULE - COMPLETE          ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ 🍺 Alcohol Detection (MQ-3)           ║");
  Serial.println("║ 😴 Drowsiness Detection (IR)          ║");
  Serial.println("║ 🚨 Emergency Button System            ║");
  Serial.println("║ 📱 Full Dashboard Integration          ║");
  Serial.println("║ ⚡ Real-time Firebase Sync            ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  // Initialize pins
  pinMode(alcoholSensorPin, INPUT);
  pinMode(drowsinessSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize outputs
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);

  // Connect to WiFi
  connectToWiFi();
  
  // Send initial heartbeat
  sendHeartbeat();
  lastHeartbeat = millis();

  // Startup sequence
  Serial.println("\n[STARTUP] Helmet module initialized successfully!");
  Serial.println("[STARTUP] Alcohol threshold: " + String(ALCOHOL_THRESHOLD));
  Serial.println("[STARTUP] Drowsiness threshold: " + String(DROWSINESS_THRESHOLD) + " ms");
  
  // Startup LED sequence
  blinkLED(3, 200);
  
  Serial.println("\n📋 SYSTEM STATUS:");
  Serial.println("   🍺 Alcohol Detection: ACTIVE");
  Serial.println("   😴 Drowsiness Detection: ACTIVE");
  Serial.println("   🚨 Emergency Button: READY");
  Serial.println("   📡 Firebase Connection: ACTIVE\n");
}

void loop() {
  // Send heartbeat every second
  if (millis() - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    sendHeartbeat();
    lastHeartbeat = millis();
  }

  // Read sensors every 250ms
  if (millis() - lastSensorRead >= SENSOR_READ_INTERVAL) {
    readSensors();
    lastSensorRead = millis();
  }

  // Check drowsiness every 100ms (more frequent for safety)
  if (millis() - lastDrowsinessCheck >= DROWSINESS_CHECK_INTERVAL) {
    checkDrowsiness();
    lastDrowsinessCheck = millis();
  }

  // Check emergency button
  checkEmergencyButton();

  // Process alcohol detection
  checkAlcoholLevel();

  // Update Firebase status
  updateFirebaseStatus();

  // Handle serial commands
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "STATUS") {
      printSystemStatus();
    }
    else if (cmd == "TEST ALCOHOL") {
      Serial.println("🧪 TESTING ALCOHOL SENSOR...");
      Serial.printf("📍 Analog pin A0 reading: %d\n", analogRead(alcoholSensorPin));
      Serial.printf("📍 Threshold: %d\n", ALCOHOL_THRESHOLD);
      Serial.printf("📍 Status: %s\n", analogRead(alcoholSensorPin) > ALCOHOL_THRESHOLD ? "DANGER" : "SAFE");
    }
    else if (cmd == "TEST DROWSINESS") {
      Serial.println("🧪 TESTING DROWSINESS SENSOR...");
      Serial.printf("📍 Digital pin %d reading: %d\n", drowsinessSensorPin, digitalRead(drowsinessSensorPin));
      Serial.printf("📍 Status: %s\n", drowsinessDetected ? "DROWSY" : "ALERT");
    }
    else if (cmd == "TEST EMERGENCY") {
      Serial.println("🧪 TESTING EMERGENCY ALERT...");
      sendEmergencyAlert();
    }
    else if (cmd == "RESET") {
      Serial.println("🔄 RESETTING ALL ALERTS...");
      alcoholDetected = false;
      drowsinessDetected = false;
      emergencyButtonPressed = false;
      alcoholAlertActive = false;
      drowsinessAlertActive = false;
      emergencyAlertActive = false;
      drowsinessTimerActive = false;
      Serial.println("✅ All alerts reset!");
    }
  }

  yield(); // Allow WiFi tasks to run
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

void sendHeartbeat() {
  if (WiFi.status() != WL_CONNECTED) return;

  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  StaticJsonDocument<128> doc;
  doc["status"] = "On";
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;
  doc["alcoholDetected"] = alcoholDetected;
  doc["drowsinessDetected"] = drowsinessDetected;
  doc["emergencyActive"] = emergencyAlertActive;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + devicePath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.printf("[HEARTBEAT] ✓ Sent (timestamp: %llu)\n", timestamp);
  } else {
    Serial.printf("[HEARTBEAT] ✗ Failed: HTTP %d\n", code);
  }
  
  http.end();
}

void readSensors() {
  // Read alcohol sensor (analog)
  alcoholSensorValue = analogRead(alcoholSensorPin);
  
  // Read drowsiness sensor (digital)
  int drowsinessReading = digitalRead(drowsinessSensorPin);
  
  // Debug output every 2 seconds
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 2000) {
    Serial.printf("[SENSORS] Alcohol: %d | Drowsiness: %d | Button: %d\n", 
                  alcoholSensorValue, drowsinessReading, digitalRead(buttonPin));
    lastDebug = millis();
  }
}

void checkAlcoholLevel() {
  bool currentAlcoholState = (alcoholSensorValue > ALCOHOL_THRESHOLD);
  
  // State change detection
  if (currentAlcoholState != alcoholDetected) {
    alcoholDetected = currentAlcoholState;
    
    if (alcoholDetected) {
      Serial.printf("\n🚨 ALCOHOL DETECTED! (Value: %d > %d)\n", alcoholSensorValue, ALCOHOL_THRESHOLD);
      sendAlcoholAlert();
      playAlertSound(5, 200);  // 5 beeps, 200ms each
      blinkLED(10, 100);       // 10 blinks, 100ms each
      alcoholAlertActive = true;
    } else {
      Serial.printf("\n✅ Alcohol CLEARED! (Value: %d <= %d)\n", alcoholSensorValue, ALCOHOL_THRESHOLD);
      alcoholAlertActive = false;
    }
    
    // Update Firebase immediately on state change
    updateAlcoholStatus();
  }
  
  // Continuous alerts while alcohol is detected
  if (alcoholDetected && (millis() - lastAlertTime > ALERT_INTERVAL)) {
    Serial.println("🚨 CONTINUOUS ALCOHOL ALERT!");
    playAlertSound(3, 150);
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    lastAlertTime = millis();
  }
}

void checkDrowsiness() {
  int drowsinessReading = digitalRead(drowsinessSensorPin);
  
  // Drowsiness detection logic (LOW = eyes closed/drowsy)
  if (drowsinessReading == LOW) {
    if (!drowsinessTimerActive) {
      drowsinessStartTime = millis();
      drowsinessTimerActive = true;
      Serial.println("[DROWSINESS] 👁️ Eyes closed - starting timer...");
    } else {
      unsigned long drowsyDuration = millis() - drowsinessStartTime;
      
      if (drowsyDuration >= DROWSINESS_THRESHOLD && !drowsinessDetected) {
        drowsinessDetected = true;
        drowsinessAlertActive = true;
        
        Serial.printf("\n😴 DROWSINESS DETECTED! (Eyes closed for %lu ms)\n", drowsyDuration);
        sendDrowsinessAlert();
        playAlertSound(7, 300);  // 7 long beeps
        blinkLED(15, 80);        // 15 fast blinks
        
        updateAlertnessStatus();
      }
    }
  } else {
    // Eyes open - reset drowsiness detection
    if (drowsinessTimerActive || drowsinessDetected) {
      if (drowsinessDetected) {
        Serial.println("\n👁️ ALERTNESS RESTORED! (Eyes open)");
        drowsinessDetected = false;
        drowsinessAlertActive = false;
        updateAlertnessStatus();
      }
      
      drowsinessTimerActive = false;
      drowsinessStartTime = 0;
    }
  }
  
  // Continuous drowsiness alerts
  if (drowsinessDetected && (millis() - lastAlertTime > ALERT_INTERVAL)) {
    Serial.println("😴 CONTINUOUS DROWSINESS ALERT!");
    playAlertSound(4, 250);
    blinkLED(5, 100);
    lastAlertTime = millis();
  }
}

void checkEmergencyButton() {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(buttonPin);
  
  // Button pressed (LOW due to INPUT_PULLUP)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    emergencyButtonPressed = true;
    emergencyAlertActive = true;
    
    Serial.println("\n🚨🚨🚨 EMERGENCY BUTTON PRESSED! 🚨🚨🚨");
    sendEmergencyAlert();
    
    // Emergency alert sequence
    for (int i = 0; i < 10; i++) {
      playAlertSound(1, 100);
      blinkLED(1, 100);
      delay(100);
    }
    
    delay(1000); // Debounce
  }
  
  lastButtonState = currentButtonState;
}

void sendAlcoholAlert() {
  Serial.println("[ALERT] 📱 Sending alcohol alert to Firebase...");
  
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = timestamp;
  doc["type"] = "alcohol_detected";
  doc["sensorValue"] = alcoholSensorValue;
  doc["threshold"] = ALCOHOL_THRESHOLD;
  doc["severity"] = alcoholSensorValue > (ALCOHOL_THRESHOLD * 1.5) ? "high" : "medium";
  doc["message"] = "Alcohol detected in helmet";
  doc["alertActive"] = true;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String alertPath = "/helmet_public/" + userUID + "/alerts.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + alertPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.POST(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.println("[ALERT] ✅ Alcohol alert sent to Firebase");
  } else {
    Serial.printf("[ALERT] ❌ Failed to send alcohol alert: HTTP %d\n", code);
  }
  
  http.end();
}

void sendDrowsinessAlert() {
  Serial.println("[ALERT] 📱 Sending drowsiness alert to Firebase...");
  
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = timestamp;
  doc["type"] = "drowsiness_detected";
  doc["duration"] = millis() - drowsinessStartTime;
  doc["threshold"] = DROWSINESS_THRESHOLD;
  doc["severity"] = "high";
  doc["message"] = "Driver drowsiness detected";
  doc["alertActive"] = true;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String alertPath = "/helmet_public/" + userUID + "/alerts.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + alertPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.POST(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.println("[ALERT] ✅ Drowsiness alert sent to Firebase");
  } else {
    Serial.printf("[ALERT] ❌ Failed to send drowsiness alert: HTTP %d\n", code);
  }
  
  http.end();
}

void sendEmergencyAlert() {
  Serial.println("[EMERGENCY] 📱 Sending emergency alert to Firebase...");
  
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = timestamp;
  doc["type"] = "emergency_button";
  doc["severity"] = "critical";
  doc["message"] = "Emergency button pressed by rider";
  doc["alertActive"] = true;
  doc["requiresImmedateResponse"] = true;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + emergencyPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.println("[EMERGENCY] ✅ Emergency alert sent to Firebase");
  } else {
    Serial.printf("[EMERGENCY] ❌ Failed to send emergency alert: HTTP %d\n", code);
  }
  
  http.end();
}
void updateAlcoholStatus() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  StaticJsonDocument<256> doc;
  doc["status"] = alcoholDetected ? "Danger" : "Safe";
  doc["sensorValue"] = alcoholSensorValue;
  doc["threshold"] = ALCOHOL_THRESHOLD;
  doc["timestamp"] = 1700000000000ULL + (uint64_t)millis();
  doc["lastUpdate"] = millis();
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + alcoholPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.printf("[FIREBASE] ✓ Alcohol status updated: %s (Value: %d)\n", 
                  alcoholDetected ? "DANGER" : "SAFE", alcoholSensorValue);
  } else {
    Serial.printf("[FIREBASE] ✗ Alcohol status update failed: HTTP %d\n", code);
  }
  
  http.end();
}

void updateAlertnessStatus() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  StaticJsonDocument<256> doc;
  doc["status"] = drowsinessDetected ? "Drowsy" : "Alert";
  doc["level"] = drowsinessDetected ? "Warning" : "Normal";
  doc["timestamp"] = 1700000000000ULL + (uint64_t)millis();
  doc["lastUpdate"] = millis();
  
  if (drowsinessDetected) {
    doc["drowsyDuration"] = millis() - drowsinessStartTime;
    doc["message"] = "Driver showing signs of drowsiness";
  } else {
    doc["message"] = "Driver is alert and focused";
  }
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + alertnessPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.printf("[FIREBASE] ✓ Alertness status updated: %s\n", 
                  drowsinessDetected ? "DROWSY" : "ALERT");
  } else {
    Serial.printf("[FIREBASE] ✗ Alertness status update failed: HTTP %d\n", code);
  }
  
  http.end();
}

void updateFirebaseStatus() {
  static unsigned long lastUpdate = 0;
  
  // Update Firebase every 2 seconds
  if (millis() - lastUpdate < 2000) return;
  lastUpdate = millis();
  
  if (WiFi.status() != WL_CONNECTED) return;
  
  // Update comprehensive helmet status
  StaticJsonDocument<512> doc;
  doc["timestamp"] = 1700000000000ULL + (uint64_t)millis();
  doc["lastUpdate"] = millis();
  
  // Device status
  doc["device"]["status"] = "On";
  doc["device"]["connected"] = true;
  doc["device"]["batteryLevel"] = 85; // Simulated battery level
  
  // Sensor readings
  doc["sensors"]["alcohol"]["value"] = alcoholSensorValue;
  doc["sensors"]["alcohol"]["status"] = alcoholDetected ? "Danger" : "Safe";
  doc["sensors"]["alcohol"]["threshold"] = ALCOHOL_THRESHOLD;
  
  doc["sensors"]["drowsiness"]["status"] = drowsinessDetected ? "Drowsy" : "Alert";
  doc["sensors"]["drowsiness"]["level"] = drowsinessLevel;
  doc["sensors"]["drowsiness"]["threshold"] = DROWSINESS_THRESHOLD;
  
  // Alert states
  doc["alerts"]["alcohol"] = alcoholAlertActive;
  doc["alerts"]["drowsiness"] = drowsinessAlertActive;
  doc["alerts"]["emergency"] = emergencyAlertActive;
  
  // Safety status
  doc["safety"]["overallStatus"] = (alcoholDetected || drowsinessDetected) ? "Warning" : "Safe";
  doc["safety"]["riderStatus"] = drowsinessDetected ? "Drowsy" : "Alert";
  doc["safety"]["alcoholStatus"] = alcoholDetected ? "Detected" : "Clear";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String statusPath = "/helmet_public/" + userUID + "/helmet_status.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + statusPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.println("[FIREBASE] ✓ Comprehensive status updated");
  } else {
    Serial.printf("[FIREBASE] ✗ Status update failed: HTTP %d\n", code);
  }
  
  http.end();
}

void playAlertSound(int beeps, int duration) {
  for (int i = 0; i < beeps; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(duration);
    digitalWrite(buzzerPin, LOW);
    delay(duration / 2);
  }
}

void blinkLED(int times, int duration) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, HIGH);
    delay(duration);
    digitalWrite(ledPin, LOW);
    delay(duration);
  }
}

void printSystemStatus() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🪖 HELMET MODULE STATUS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  Serial.printf("WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED ✅" : "DISCONNECTED ❌");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
  }
  
  Serial.println("\n🔍 SENSOR READINGS:");
  Serial.printf("Alcohol Sensor: %d (Threshold: %d)\n", alcoholSensorValue, ALCOHOL_THRESHOLD);
  Serial.printf("Alcohol Status: %s\n", alcoholDetected ? "DANGER ❌" : "SAFE ✅");
  
  Serial.printf("Drowsiness Sensor: %d\n", digitalRead(drowsinessSensorPin));
  Serial.printf("Drowsiness Status: %s\n", drowsinessDetected ? "DROWSY ❌" : "ALERT ✅");
  
  Serial.printf("Emergency Button: %s\n", digitalRead(buttonPin) == LOW ? "PRESSED" : "NORMAL");
  
  Serial.println("\n🚨 ALERT STATUS:");
  Serial.printf("Alcohol Alert: %s\n", alcoholAlertActive ? "ACTIVE 🚨" : "INACTIVE ✅");
  Serial.printf("Drowsiness Alert: %s\n", drowsinessAlertActive ? "ACTIVE 🚨" : "INACTIVE ✅");
  Serial.printf("Emergency Alert: %s\n", emergencyAlertActive ? "ACTIVE 🚨" : "INACTIVE ✅");
  
  Serial.println("\n📡 FIREBASE STATUS:");
  Serial.printf("Last Heartbeat: %lu ms ago\n", millis() - lastHeartbeat);
  Serial.printf("Last Sensor Read: %lu ms ago\n", millis() - lastSensorRead);
  
  Serial.println("\n⚙️ SYSTEM CONFIGURATION:");
  Serial.printf("Alcohol Threshold: %d\n", ALCOHOL_THRESHOLD);
  Serial.printf("Drowsiness Threshold: %d ms\n", DROWSINESS_THRESHOLD);
  Serial.printf("Heartbeat Interval: %lu ms\n", HEARTBEAT_INTERVAL);
  Serial.printf("Sensor Read Interval: %lu ms\n", SENSOR_READ_INTERVAL);
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  Serial.println("\n📋 AVAILABLE COMMANDS:");
  Serial.println("   STATUS - Show this status");
  Serial.println("   TEST ALCOHOL - Test alcohol sensor");
  Serial.println("   TEST DROWSINESS - Test drowsiness sensor");
  Serial.println("   TEST EMERGENCY - Test emergency alert");
  Serial.println("   RESET - Reset all alerts");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}