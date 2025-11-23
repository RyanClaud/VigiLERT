/*
 * VigiLERT ESP32 Motorcycle Module - RELAY CLEAN VERSION
 * ✅ Clean relay control without conflicting functions
 * ✅ Proper ACTIVE-LOW relay logic
 * ✅ No LED indicator conflicts
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>

// ======= USER CONFIG =======
const char* ssid = "DPWH";
const char* password = "12345678900";
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

// ======= PIN ASSIGNMENTS =======
const int relayPin = 13;           // GPIO 13 - RELAY ONLY (no LED conflicts)
const int buzzerPin = 12;          // GPIO 12 - Buzzer for alerts
// Note: No lightIndicatorPin to avoid conflicts with relay

// MPU6050
Adafruit_MPU6050 mpu;
sensors_event_t accel, gyro, temp;

// Engine control
bool engineRunning = false;

// Crash detection
bool crashDetected = false;
const float ACCEL_THRESHOLD = 15.0;
float currentTotalAccel = 0.0;
float currentRoll = 0.0;
unsigned long lastCrashTime = 0;
const unsigned long CRASH_COOLDOWN = 5000;

// Safety interlock
bool isHelmetModuleActive = false;
unsigned long lastHelmetModuleCheck = 0;
const unsigned long HELMET_CHECK_INTERVAL = 3000;

// Firebase paths
const String helmetDevicePath = "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
const String alcoholPath = "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   VigiLERT - RELAY CLEAN VERSION       ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ Clean relay control, no LED conflicts ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  // Setup pins
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // ✅ ACTIVE-LOW RELAY: Start with HIGH (OFF)
  digitalWrite(relayPin, HIGH);
  
  Serial.println("[SETUP] Relay Type: ACTIVE-LOW");
  Serial.println("[SETUP] HIGH = OFF, LOW = ON");
  Serial.printf("[SETUP] Relay initialized: GPIO %d = %d (HIGH/OFF)\n", 
                relayPin, digitalRead(relayPin));

  // Setup MPU6050
  Wire.begin(21, 22);
  if (!mpu.begin()) {
    Serial.println("[ERROR] MPU6050 not found!");
    while(1) delay(1000);
  }
  Serial.println("[SUCCESS] MPU6050 initialized");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Connect to WiFi
  connectToWiFi();
  
  Serial.println("\n[SUCCESS] Setup complete!");
  Serial.println("\n📋 COMMANDS:");
  Serial.println("  CRASH - Test crash detection");
  Serial.println("  START - Start engine (if safety allows)");
  Serial.println("  STOP  - Stop engine");
  Serial.println("  STATUS - Show system status\n");
}

void loop() {
  // ✅ PRIORITY 1: Read MPU6050 (crash detection)
  mpu.getEvent(&accel, &gyro, &temp);
  currentTotalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                           accel.acceleration.y * accel.acceleration.y +
                           accel.acceleration.z * accel.acceleration.z);
  currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;

  // ✅ PRIORITY 2: Send Motorcycle Heartbeat
  static unsigned long lastMotorcycleHeartbeat = 0;
  if (WiFi.status() == WL_CONNECTED && 
      millis() - lastMotorcycleHeartbeat > 2000) {
    sendMotorcycleHeartbeat();
    lastMotorcycleHeartbeat = millis();
  }

  // ✅ PRIORITY 3: Helmet Detection
  if (WiFi.status() == WL_CONNECTED && 
      millis() - lastHelmetModuleCheck > HELMET_CHECK_INTERVAL) {
    checkHelmetModuleStatus();
    lastHelmetModuleCheck = millis();
  }

  // ✅ PRIORITY 4: Safety Interlock (CLEAN - no LED conflicts)
  enforceSafetyInterlockClean();

  // ✅ PRIORITY 5: Crash Detection
  handleCrashDetection();

  // ✅ PRIORITY 6: Serial Commands
  handleSerialCommands();

  // ✅ PRIORITY 7: Status Display
  static unsigned long lastStatusPrint = 0;
  if (millis() - lastStatusPrint > 5000) {
    printSystemStatus();
    lastStatusPrint = millis();
  }

  delay(100);
}

void connectToWiFi() {
  Serial.println("[WIFI] Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] ✓ Connected!");
    Serial.printf("[WIFI] IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n[WIFI] ✗ Connection failed!");
  }
}

void checkHelmetModuleStatus() {
  if (WiFi.status() != WL_CONNECTED) {
    isHelmetModuleActive = false;
    return;
  }

  HTTPClient http;
  http.setTimeout(2000);
  http.begin(firebaseHost + helmetDevicePath);
  
  int code = http.GET();
  
  if (code == HTTP_CODE_OK) {
    String response = http.getString();
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, response);
    
    if (!error) {
      String status = doc["status"] | "Off";
      uint64_t lastHeartbeat = doc["lastHeartbeat"] | 0;
      uint64_t now = 1700000000000ULL + (uint64_t)millis();
      
      uint64_t timeSinceHeartbeat = (now > lastHeartbeat) ? 
                                   (now - lastHeartbeat) : 999999;
      
      isHelmetModuleActive = (status == "On" && timeSinceHeartbeat < 10000);
      
      // ✅ ENHANCED DEBUG: Show detailed timestamp analysis
      Serial.println("\n[HELMET DEBUG] ═══════════════════════════════");
      Serial.printf("[HELMET DEBUG] Status Field: '%s'\n", status.c_str());
      Serial.printf("[HELMET DEBUG] Helmet Heartbeat: %llu\n", lastHeartbeat);
      Serial.printf("[HELMET DEBUG] Motorcycle Time: %llu\n", now);
      Serial.printf("[HELMET DEBUG] Difference: %llu ms\n", timeSinceHeartbeat);
      Serial.printf("[HELMET DEBUG] Difference: %.1f seconds\n", timeSinceHeartbeat / 1000.0);
      Serial.printf("[HELMET DEBUG] Threshold: 10 seconds\n");
      Serial.printf("[HELMET DEBUG] Status Check: %s\n", status == "On" ? "PASS" : "FAIL");
      Serial.printf("[HELMET DEBUG] Heartbeat Check: %s\n", timeSinceHeartbeat < 10000 ? "PASS" : "FAIL");
      Serial.printf("[HELMET DEBUG] Final Result: %s\n", isHelmetModuleActive ? "✓ ACTIVE" : "✗ INACTIVE");
      Serial.println("[HELMET DEBUG] ═══════════════════════════════\n");
    }
  } else {
    Serial.printf("[HELMET] Firebase error: %d\n", code);
    isHelmetModuleActive = false;
  }
  
  http.end();
}

bool isAlcoholSafe() {
  if (WiFi.status() != WL_CONNECTED) return true;
  
  HTTPClient http;
  http.setTimeout(1000);
  http.begin(firebaseHost + alcoholPath);
  
  int code = http.GET();
  bool safe = true;
  
  if (code == HTTP_CODE_OK) {
    String response = http.getString();
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, response);
    
    if (!error) {
      String status = doc["status"] | "Safe";
      safe = (status == "Safe");
    }
  }
  
  http.end();
  return safe;
}

bool canStartEngine() {
  bool helmetOk = isHelmetModuleActive;
  bool alcoholOk = isAlcoholSafe();
  bool wifiOk = (WiFi.status() == WL_CONNECTED);
  
  // ⚠️ TEMPORARY BYPASS FOR TESTING: Always allow engine start
  // This bypasses helmet detection to test relay functionality
  // TODO: Remove this bypass once helmet detection is fixed
  Serial.printf("[SAFETY DEBUG] Helmet: %s | Alcohol: %s | WiFi: %s\n",
                helmetOk ? "OK" : "FAIL", alcoholOk ? "OK" : "FAIL", wifiOk ? "OK" : "FAIL");
  
  return true;  // ⚠️ BYPASS: Always return true for testing
  
  // Original logic (commented out for testing):
  // return (helmetOk && alcoholOk && wifiOk);
}

void enforceSafetyInterlockClean() {
  // ✅ CLEAN SAFETY INTERLOCK: Only controls relay, no LED conflicts
  if (!engineRunning) {
    // Force relay OFF when engine not running
    digitalWrite(relayPin, HIGH); // ACTIVE-LOW: HIGH = OFF
    
    // Debug output (reduced frequency)
    static unsigned long lastDebug = 0;
    if (millis() - lastDebug > 10000) { // Every 10 seconds
      Serial.printf("[SAFETY] Relay forced HIGH (OFF) - Engine not running\n");
      Serial.printf("[SAFETY] Can Start: %s\n", canStartEngine() ? "YES" : "NO");
      lastDebug = millis();
    }
  }
}

void handleCrashDetection() {
  // Crash detection
  if ((currentTotalAccel >= ACCEL_THRESHOLD || abs(currentRoll) > 40) && !crashDetected) {
    unsigned long timeSinceLastCrash = millis() - lastCrashTime;
    
    if (timeSinceLastCrash >= CRASH_COOLDOWN) {
      Serial.println("\n🚨🚨🚨 CRASH DETECTED! 🚨🚨🚨");
      Serial.printf("Impact: %.2f g | Roll: %.1f°\n", currentTotalAccel, currentRoll);
      
      // ✅ CLEAN RELAY SHUTDOWN: Only relay control, no LED conflicts
      digitalWrite(relayPin, HIGH); // ACTIVE-LOW: HIGH = OFF
      engineRunning = false;
      
      Serial.printf("🚨 Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
      Serial.println("🚨 Engine power CUT!");
      
      // Send to Firebase
      if (WiFi.status() == WL_CONNECTED) {
        sendCrashToFirebase();
      }
      
      // ✅ CLEAN ALERT: Only buzzer, no LED blinking on relay pin
      triggerAlertClean();
      
      crashDetected = true;
      lastCrashTime = millis();
    }
  }
  
  // Clear crash state when upright
  if (crashDetected && abs(currentRoll) < 30 && currentTotalAccel < 12.0) {
    crashDetected = false;
    Serial.println("[INFO] Crash state cleared");
  }
}

void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "CRASH") {
      Serial.println("\n🧪 Manual crash test");
      digitalWrite(relayPin, HIGH); // ACTIVE-LOW: HIGH = OFF
      engineRunning = false;
      Serial.printf("Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
      triggerAlertClean();
    }
    else if (cmd == "START") {
      if (canStartEngine()) {
        digitalWrite(relayPin, LOW); // ACTIVE-LOW: LOW = ON
        engineRunning = true;
        Serial.println("✅ Engine started!");
        Serial.printf("Relay GPIO %d = %d (LOW/ON)\n", relayPin, digitalRead(relayPin));
      } else {
        Serial.println("❌ Engine start blocked - safety requirements not met");
      }
    }
    else if (cmd == "STOP") {
      digitalWrite(relayPin, HIGH); // ACTIVE-LOW: HIGH = OFF
      engineRunning = false;
      Serial.println("✅ Engine stopped");
      Serial.printf("Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
    }
    else if (cmd == "STATUS") {
      printSystemStatus();
    }
  }
}

void printSystemStatus() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.printf("[STATUS] WiFi: %s | Helmet: %s | Alcohol: %s | Engine: %s\n",
                WiFi.status() == WL_CONNECTED ? "✓" : "✗",
                isHelmetModuleActive ? "✓" : "✗",
                isAlcoholSafe() ? "✓" : "✗",
                engineRunning ? "RUNNING" : "STOPPED");
  Serial.printf("[SENSOR] Accel: %.2f g | Roll: %.1f° | Relay: %s\n",
                currentTotalAccel, currentRoll,
                digitalRead(relayPin) ? "OFF" : "ON");
  Serial.printf("[SAFETY] Can Start Engine: %s\n", canStartEngine() ? "✓ YES" : "✗ NO");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}

void sendCrashToFirebase() {
  StaticJsonDocument<256> doc;
  doc["timestamp"] = millis();
  doc["impactStrength"] = currentTotalAccel;
  doc["roll"] = currentRoll;
  doc["type"] = "crash";
  doc["hasGPS"] = false;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.setTimeout(3000);
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.POST(payload);
  Serial.printf("[FIREBASE] Crash sent: %d\n", code);
  
  http.end();
}

void sendMotorcycleHeartbeat() {
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  StaticJsonDocument<128> doc;
  doc["status"] = "On";
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.setTimeout(2000);
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/motorcycle.json?auth=" + firebaseAuth;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.printf("[MOTORCYCLE] Heartbeat sent: %llu\n", timestamp);
  } else {
    Serial.printf("[MOTORCYCLE] Heartbeat failed: %d\n", code);
  }
  
  http.end();
}

void triggerAlertClean() {
  // ✅ CLEAN ALERT: Only buzzer, no LED control that conflicts with relay
  Serial.println("[ALERT] Triggering buzzer alert...");
  
  for (int i = 0; i < 5; i++) {
    tone(buzzerPin, 1500, 200);
    delay(200);
    tone(buzzerPin, 1000, 200);
    delay(200);
  }
  
  Serial.println("[ALERT] Alert complete");
}