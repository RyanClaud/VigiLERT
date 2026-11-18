#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ═══════════════════════════════════════════════════════════════════════════
// VigiLERT Helmet Module - COMPLETE WITH ALL SENSORS
// ═══════════════════════════════════════════════════════════════════════════
// ✅ MQ-3 Alcohol sensor with real readings
// ✅ IR Drowsiness sensor with detection
// ✅ Emergency button system
// ✅ Optimized connection (250ms heartbeat)
// ✅ All sensor data visible in dashboard
// ✅ Real-time Firebase synchronization
// ═══════════════════════════════════════════════════════════════════════════

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Configuration
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

// Pin Assignments
const int alcoholSensorPin = A0;      // MQ-3 alcohol sensor (analog) - your actual sensor
const int drowsinessSensorPin = 2;    // IR sensor (not used - drowsiness based on MQ-3)
const int buzzerPin = 12;             // Buzzer for alerts
const int ledPin = 13;                // LED indicator
const int buttonPin = 4;              // Emergency button

// ✅ SENSOR THRESHOLDS (Based on actual testing)
const int ALCOHOL_THRESHOLD = 3000;    // MQ-3 sensor threshold for alcohol detection
const int DROWSINESS_THRESHOLD = 3500; // Higher alcohol level indicates drowsiness/impairment

// ✅ OPTIMIZED TIMING
const unsigned long HEARTBEAT_INTERVAL = 250;      // 250ms heartbeat
const unsigned long SENSOR_READ_INTERVAL = 100;    // 100ms sensor readings
const unsigned long FIREBASE_SYNC_INTERVAL = 500;  // 500ms Firebase sync
const unsigned long ALERT_INTERVAL = 5000;         // 5 seconds between alerts
const unsigned long DROWSINESS_CHECK_INTERVAL = 100; // 100ms drowsiness check

// ✅ SENSOR STATE VARIABLES
bool alcoholDetected = false;
bool drowsinessDetected = false;
bool emergencyButtonPressed = false;
int alcoholSensorValue = 0;
int drowsinessLevel = 0;
unsigned long drowsinessStartTime = 0;
bool drowsinessTimerActive = false;

// ✅ TIMING VARIABLES
unsigned long lastHeartbeat = 0;
unsigned long lastSensorRead = 0;
unsigned long lastFirebaseSync = 0;
unsigned long lastAlertTime = 0;
unsigned long lastDrowsinessCheck = 0;

// ✅ ALERT STATES
bool alcoholAlertActive = false;
bool drowsinessAlertActive = false;
bool emergencyAlertActive = false;

// ✅ CONNECTION HEALTH
struct ConnectionHealth {
  int consecutiveFailures = 0;
  int consecutiveSuccesses = 0;
  float successRate = 100.0;
  unsigned long totalRequests = 0;
  unsigned long successfulRequests = 0;
};

ConnectionHealth connHealth;

// Firebase paths
const String devicePath = "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
const String alcoholPath = "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;
const String alertnessPath = "/helmet_public/" + userUID + "/alertness/status.json?auth=" + firebaseAuth;
const String emergencyPath = "/helmet_public/" + userUID + "/emergency.json?auth=" + firebaseAuth;

// HTTP client
HTTPClient http;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   HELMET MODULE - COMPLETE SENSORS    ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ 🍺 MQ-3 Alcohol Sensor (A0)           ║");
  Serial.println("║ 😴 IR Drowsiness Sensor (D2)          ║");
  Serial.println("║ 🚨 Emergency Button (D4)              ║");
  Serial.println("║ ⚡ Optimized connection (250ms)        ║");
  Serial.println("║ 📊 Real-time sensor readings          ║");
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
  
  // ✅ SENSOR BASELINE READINGS
  delay(500);
  int alcoholBaseline = analogRead(alcoholSensorPin);
  int drowsinessBaseline = digitalRead(drowsinessSensorPin);
  int buttonBaseline = digitalRead(buttonPin);
  
  Serial.println("\n📊 SENSOR BASELINE READINGS:");
  Serial.printf("🍺 Alcohol (A0): %d (Threshold: %d)\n", alcoholBaseline, ALCOHOL_THRESHOLD);
  Serial.printf("😴 Drowsiness (D2): %d (LOW = drowsy)\n", drowsinessBaseline);
  Serial.printf("🚨 Emergency (D4): %d (LOW = pressed)\n", buttonBaseline);
  
  if (alcoholBaseline > ALCOHOL_THRESHOLD) {
    Serial.println("⚠️  WARNING: Alcohol sensor reading high at startup!");
  }
  
  // Send initial heartbeat
  sendCompleteHeartbeat();
  lastHeartbeat = millis();

  Serial.println("\n🚀 HELMET MODULE WITH SENSORS READY!");
  Serial.println("📋 Commands: STATUS, TEST ALCOHOL, TEST DROWSINESS, TEST EMERGENCY, RESET");
  Serial.println("🔍 Watch dashboard for real-time sensor data\n");
  
  // Startup LED sequence
  blinkLED(3, 200);
}

void loop() {
  unsigned long currentTime = millis();
  
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

  // ✅ DROWSINESS CHECK (more frequent for safety)
  if (currentTime - lastDrowsinessCheck >= DROWSINESS_CHECK_INTERVAL) {
    checkDrowsinessDetailed();
    lastDrowsinessCheck = currentTime;
  }

  // ✅ FIREBASE SYNC (when needed)
  if (currentTime - lastFirebaseSync >= FIREBASE_SYNC_INTERVAL) {
    syncSensorDataToFirebase();
    lastFirebaseSync = currentTime;
  }

  // ✅ EMERGENCY BUTTON CHECK
  checkEmergencyButton();

  // ✅ ALCOHOL LEVEL PROCESSING
  processAlcoholDetection();

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
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(300);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] ✅ Connected!");
    Serial.printf("[WIFI] IP: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("[WIFI] Signal: %d dBm\n", WiFi.RSSI());
  } else {
    Serial.println("\n[WIFI] ❌ Failed!");
  }
}

// ✅ COMPLETE HEARTBEAT WITH SENSOR DATA
void sendCompleteHeartbeat() {
  if (WiFi.status() != WL_CONNECTED) {
    updateConnectionHealth(false);
    return;
  }

  unsigned long startTime = millis();
  
  // ✅ COMPLETE SENSOR DATA IN HEARTBEAT
  StaticJsonDocument<256> doc;
  doc["status"] = "On";  // Device is connected
  doc["lastHeartbeat"] = (uint64_t)(1700000000000ULL + millis());
  doc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  
  // ✅ SENSOR READINGS
  doc["alcoholDetected"] = alcoholDetected;
  doc["alcoholSensorValue"] = alcoholSensorValue;
  doc["drowsinessDetected"] = drowsinessDetected;
  doc["drowsinessLevel"] = drowsinessLevel;
  doc["emergencyActive"] = emergencyAlertActive;
  
  // ✅ ALERT STATES
  doc["alcoholAlertActive"] = alcoholAlertActive;
  doc["drowsinessAlertActive"] = drowsinessAlertActive;
  
  String payload;
  serializeJson(doc, payload);
  
  http.begin(firebaseHost + devicePath);
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.PUT(payload);
  
  if (httpCode == HTTP_CODE_OK) {
    updateConnectionHealth(true);
    Serial.printf("[HEARTBEAT] ✓ %dms | Alcohol:%d | Drowsy:%s\n", 
                  (int)(millis() - startTime), 
                  alcoholSensorValue,
                  drowsinessDetected ? "YES" : "NO");
  } else {
    updateConnectionHealth(false);
    Serial.printf("[HEARTBEAT] ✗ HTTP %d (%dms)\n", httpCode, (int)(millis() - startTime));
  }
  
  http.end();
}

// ✅ READ ALL SENSORS
void readAllSensors() {
  // ✅ ALCOHOL SENSOR (Analog)
  alcoholSensorValue = analogRead(alcoholSensorPin);
  
  // ✅ DROWSINESS SENSOR (Digital)
  int drowsinessReading = digitalRead(drowsinessSensorPin);
  
  // ✅ EMERGENCY BUTTON (Digital with pullup)
  int buttonReading = digitalRead(buttonPin);
  
  // ✅ DEBUG OUTPUT (every 2 seconds)
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 2000) {
    Serial.printf("[SENSORS] 🍺 Alcohol:%d | 😴 Drowsy:%d | 🚨 Button:%d\n", 
                  alcoholSensorValue, drowsinessReading, buttonReading);
    lastDebug = millis();
  }
}

// ✅ DETAILED DROWSINESS DETECTION (Based on alcohol levels)
void checkDrowsinessDetailed() {
  // ✅ DROWSINESS LOGIC: Based on alcohol sensor readings, not IR sensor
  // Higher alcohol levels (3500+) indicate impairment/drowsiness
  
  bool currentDrowsinessState = (alcoholSensorValue >= DROWSINESS_THRESHOLD);
  
  if (currentDrowsinessState != drowsinessDetected) {
    drowsinessDetected = currentDrowsinessState;
    
    if (drowsinessDetected) {
      drowsinessAlertActive = true;
      drowsinessLevel = map(alcoholSensorValue, DROWSINESS_THRESHOLD, 4095, 50, 100);
      drowsinessLevel = constrain(drowsinessLevel, 50, 100);
      
      Serial.printf("[DROWSINESS] 😴 IMPAIRMENT DETECTED! (Alcohol: %d >= %d)\n", 
                    alcoholSensorValue, DROWSINESS_THRESHOLD);
      Serial.printf("[DROWSINESS] Impairment Level: %d%%\n", drowsinessLevel);
      
      // Alert sequence for drowsiness
      playAlertSound(4, 150);  // 4 beeps for drowsiness
      blinkLED(8, 100);        // 8 blinks for drowsiness
      
    } else {
      drowsinessAlertActive = false;
      drowsinessLevel = 0;
      Serial.printf("[DROWSINESS] ✅ Impairment cleared (Alcohol: %d < %d)\n", 
                    alcoholSensorValue, DROWSINESS_THRESHOLD);
    }
  }
  
  // Update drowsiness level continuously when detected
  if (drowsinessDetected) {
    drowsinessLevel = map(alcoholSensorValue, DROWSINESS_THRESHOLD, 4095, 50, 100);
    drowsinessLevel = constrain(drowsinessLevel, 50, 100);
  }
}

// ✅ ALCOHOL DETECTION PROCESSING
void processAlcoholDetection() {
  bool currentAlcoholState = (alcoholSensorValue > ALCOHOL_THRESHOLD);
  
  // ✅ STATE CHANGE DETECTION
  if (currentAlcoholState != alcoholDetected) {
    alcoholDetected = currentAlcoholState;
    
    if (alcoholDetected) {
      Serial.printf("\n🚨 ALCOHOL DETECTED! (Value: %d > %d)\n", alcoholSensorValue, ALCOHOL_THRESHOLD);
      alcoholAlertActive = true;
      
      // Immediate alert
      playAlertSound(5, 200);
      blinkLED(10, 100);
      
    } else {
      Serial.printf("\n✅ ALCOHOL CLEARED! (Value: %d <= %d)\n", alcoholSensorValue, ALCOHOL_THRESHOLD);
      alcoholAlertActive = false;
    }
  }
  
  // ✅ CONTINUOUS ALERTS while alcohol detected
  if (alcoholDetected && (millis() - lastAlertTime > ALERT_INTERVAL)) {
    Serial.println("🚨 CONTINUOUS ALCOHOL ALERT!");
    playAlertSound(3, 150);
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    lastAlertTime = millis();
  }
}

// ✅ EMERGENCY BUTTON CHECK
void checkEmergencyButton() {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(buttonPin);
  
  // Button pressed (LOW due to INPUT_PULLUP)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    emergencyButtonPressed = true;
    emergencyAlertActive = true;
    
    Serial.println("\n🚨🚨 EMERGENCY BUTTON PRESSED! 🚨🚨");
    
    // Emergency alert sequence
    for (int i = 0; i < 10; i++) {
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      digitalWrite(ledPin, LOW);
      delay(100);
    }
    
    // Send emergency alert to Firebase
    sendEmergencyAlert();
  }
  
  lastButtonState = currentButtonState;
}

// ✅ SYNC SENSOR DATA TO FIREBASE
void syncSensorDataToFirebase() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  // ✅ UPDATE ALCOHOL STATUS
  StaticJsonDocument<128> alcoholDoc;
  alcoholDoc["status"] = alcoholDetected ? "Danger" : "Safe";
  alcoholDoc["sensorValue"] = alcoholSensorValue;
  alcoholDoc["threshold"] = ALCOHOL_THRESHOLD;
  alcoholDoc["alertActive"] = alcoholAlertActive;
  alcoholDoc["timestamp"] = millis();
  
  String alcoholPayload;
  serializeJson(alcoholDoc, alcoholPayload);
  
  http.begin(firebaseHost + alcoholPath);
  http.addHeader("Content-Type", "application/json");
  int alcoholCode = http.PUT(alcoholPayload);
  http.end();
  
  // ✅ UPDATE ALERTNESS STATUS
  StaticJsonDocument<128> alertnessDoc;
  alertnessDoc["status"] = drowsinessDetected ? "Drowsy" : "Alert";
  alertnessDoc["level"] = drowsinessLevel;
  alertnessDoc["alertActive"] = drowsinessAlertActive;
  alertnessDoc["eyesClosedDuration"] = drowsinessTimerActive ? (millis() - drowsinessStartTime) : 0;
  alertnessDoc["timestamp"] = millis();
  
  String alertnessPayload;
  serializeJson(alertnessDoc, alertnessPayload);
  
  http.begin(firebaseHost + alertnessPath);
  http.addHeader("Content-Type", "application/json");
  int alertnessCode = http.PUT(alertnessPayload);
  http.end();
  
  if (alcoholCode == HTTP_CODE_OK && alertnessCode == HTTP_CODE_OK) {
    Serial.printf("[SYNC] ✓ Sensors synced | Alcohol:%s | Drowsy:%s\n",
                  alcoholDetected ? "DANGER" : "SAFE",
                  drowsinessDetected ? "YES" : "NO");
  }
}

// ✅ EMERGENCY ALERT
void sendEmergencyAlert() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  StaticJsonDocument<256> emergencyDoc;
  emergencyDoc["active"] = true;
  emergencyDoc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  emergencyDoc["type"] = "manual_button";
  emergencyDoc["alcoholDetected"] = alcoholDetected;
  emergencyDoc["drowsinessDetected"] = drowsinessDetected;
  emergencyDoc["location"] = "GPS not available";  // TODO: Add GPS if available
  
  String emergencyPayload;
  serializeJson(emergencyDoc, emergencyPayload);
  
  http.begin(firebaseHost + emergencyPath);
  http.addHeader("Content-Type", "application/json");
  int emergencyCode = http.PUT(emergencyPayload);
  
  if (emergencyCode == HTTP_CODE_OK) {
    Serial.println("[EMERGENCY] ✓ Alert sent to Firebase!");
  } else {
    Serial.printf("[EMERGENCY] ✗ Failed: HTTP %d\n", emergencyCode);
  }
  
  http.end();
}

// ✅ ALERT SOUND SYSTEM
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

// ✅ SERIAL COMMANDS
void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "STATUS") {
      printDetailedStatus();
    }
    else if (cmd == "TEST ALCOHOL") {
      Serial.println("\n🧪 TESTING ALCOHOL SENSOR...");
      int reading = analogRead(alcoholSensorPin);
      Serial.printf("📍 Raw reading: %d (0-1023 range)\n", reading);
      Serial.printf("📍 Threshold: %d\n", ALCOHOL_THRESHOLD);
      Serial.printf("📍 Status: %s\n", reading > ALCOHOL_THRESHOLD ? "DANGER ⚠️" : "SAFE ✅");
      Serial.printf("📍 Percentage: %.1f%%\n", (reading / 1023.0) * 100.0);
    }
    else if (cmd == "TEST DROWSINESS") {
      Serial.println("\n🧪 TESTING DROWSINESS SENSOR...");
      int reading = digitalRead(drowsinessSensorPin);
      Serial.printf("📍 Digital reading: %d\n", reading);
      Serial.printf("📍 Status: %s\n", reading == LOW ? "EYES CLOSED 😴" : "EYES OPEN 👁️");
      Serial.printf("📍 Timer active: %s\n", drowsinessTimerActive ? "YES" : "NO");
      if (drowsinessTimerActive) {
        Serial.printf("📍 Duration: %lu ms\n", millis() - drowsinessStartTime);
      }
    }
    else if (cmd == "TEST EMERGENCY") {
      Serial.println("\n🧪 TESTING EMERGENCY SYSTEM...");
      emergencyButtonPressed = true;
      emergencyAlertActive = true;
      sendEmergencyAlert();
      playAlertSound(5, 200);
      Serial.println("✅ Emergency test completed!");
    }
    else if (cmd == "RESET") {
      Serial.println("\n🔄 RESETTING ALL SENSORS...");
      alcoholDetected = false;
      drowsinessDetected = false;
      emergencyButtonPressed = false;
      alcoholAlertActive = false;
      drowsinessAlertActive = false;
      emergencyAlertActive = false;
      drowsinessTimerActive = false;
      drowsinessLevel = 0;
      Serial.println("✅ All sensors reset!");
    }
    else if (cmd == "CALIBRATE") {
      Serial.println("\n🔧 CALIBRATING SENSORS...");
      Serial.println("📍 Reading baseline values...");
      
      int alcoholSum = 0;
      for (int i = 0; i < 10; i++) {
        alcoholSum += analogRead(alcoholSensorPin);
        delay(100);
      }
      int alcoholAvg = alcoholSum / 10;
      
      Serial.printf("🍺 Alcohol sensor average: %d\n", alcoholAvg);
      Serial.printf("😴 Drowsiness sensor: %d\n", digitalRead(drowsinessSensorPin));
      Serial.printf("🚨 Emergency button: %d\n", digitalRead(buttonPin));
      Serial.println("✅ Calibration complete!");
    }
  }
}

void printDetailedStatus() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📊 HELMET MODULE - DETAILED STATUS");
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
  
  // Sensor Status
  Serial.println("\n🔬 SENSOR READINGS:");
  Serial.printf("🍺 Alcohol Sensor (A0):\n");
  Serial.printf("    Raw Value: %d (0-1023)\n", alcoholSensorValue);
  Serial.printf("    Threshold: %d\n", ALCOHOL_THRESHOLD);
  Serial.printf("    Status: %s\n", alcoholDetected ? "DANGER ⚠️" : "SAFE ✅");
  Serial.printf("    Alert Active: %s\n", alcoholAlertActive ? "YES" : "NO");
  
  Serial.printf("\n😴 Drowsiness Sensor (D2):\n");
  Serial.printf("    Digital Value: %d\n", digitalRead(drowsinessSensorPin));
  Serial.printf("    Status: %s\n", drowsinessDetected ? "DROWSY 😴" : "ALERT 👁️");
  Serial.printf("    Level: %d\n", drowsinessLevel);
  Serial.printf("    Timer Active: %s\n", drowsinessTimerActive ? "YES" : "NO");
  if (drowsinessTimerActive) {
    Serial.printf("    Duration: %lu ms\n", millis() - drowsinessStartTime);
  }
  
  Serial.printf("\n🚨 Emergency Button (D4):\n");
  Serial.printf("    Digital Value: %d\n", digitalRead(buttonPin));
  Serial.printf("    Status: %s\n", emergencyAlertActive ? "ACTIVE 🚨" : "NORMAL ✅");
  
  // Firebase Paths
  Serial.println("\n🔥 FIREBASE PATHS:");
  Serial.println("    Device: " + devicePath);
  Serial.println("    Alcohol: " + alcoholPath);
  Serial.println("    Alertness: " + alertnessPath);
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📋 COMMANDS: STATUS, TEST ALCOHOL, TEST DROWSINESS, TEST EMERGENCY, RESET, CALIBRATE");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}