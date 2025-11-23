/*
 * VigiLERT ESP32 Motorcycle Module - WiFi STABLE VERSION
 * ✅ Optimized for stable WiFi connection
 * ✅ Reduced operations that interfere with WiFi
 * ✅ Prioritizes helmet detection and relay control
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
const int relayPin = 13;           // GPIO 13 - PROVEN TO WORK!
const int lightIndicatorPin = 2;   // GPIO 2 (no conflicts)
const int buzzerPin = 12;

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
const unsigned long HELMET_CHECK_INTERVAL = 3000; // Check every 3 seconds

// WiFi monitoring
unsigned long lastWiFiCheck = 0;
const unsigned long WIFI_CHECK_INTERVAL = 5000; // Check every 5 seconds

// Firebase paths
const String helmetDevicePath = "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
const String alcoholPath = "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   VigiLERT - WiFi STABLE VERSION       ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ Optimized for stable WiFi connection  ║");
  Serial.println("║ Reduced operations, faster detection  ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  // Setup pins
  pinMode(relayPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // ✅ ACTIVE-LOW RELAY: Start with HIGH (OFF)
  digitalWrite(relayPin, HIGH);
  digitalWrite(lightIndicatorPin, LOW);
  
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

  // Connect to WiFi with stability focus
  connectToWiFiStable();
  
  Serial.println("\n[SUCCESS] Setup complete!");
  Serial.println("\n📋 COMMANDS:");
  Serial.println("  CRASH - Test crash detection");
  Serial.println("  START - Start engine (if safety allows)");
  Serial.println("  STOP  - Stop engine");
  Serial.println("  WIFI  - Check WiFi status\n");
}

void loop() {
  // ✅ PRIORITY 1: Maintain WiFi Connection
  if (millis() - lastWiFiCheck > WIFI_CHECK_INTERVAL) {
    checkWiFiStability();
    lastWiFiCheck = millis();
  }

  // ✅ PRIORITY 2: Read MPU6050 (crash detection)
  mpu.getEvent(&accel, &gyro, &temp);
  currentTotalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                           accel.acceleration.y * accel.acceleration.y +
                           accel.acceleration.z * accel.acceleration.z);
  currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;

  // ✅ PRIORITY 3: Send Motorcycle Heartbeat
  static unsigned long lastMotorcycleHeartbeat = 0;
  if (WiFi.status() == WL_CONNECTED && 
      millis() - lastMotorcycleHeartbeat > 2000) { // Every 2 seconds
    sendMotorcycleHeartbeat();
    lastMotorcycleHeartbeat = millis();
  }

  // ✅ PRIORITY 4: Helmet Detection (only if WiFi stable)
  if (WiFi.status() == WL_CONNECTED && 
      millis() - lastHelmetModuleCheck > HELMET_CHECK_INTERVAL) {
    checkHelmetModuleStatusFast();
    lastHelmetModuleCheck = millis();
  }

  // ✅ PRIORITY 5: Safety Interlock
  enforceSafetyInterlock();

  // ✅ PRIORITY 6: Crash Detection
  handleCrashDetection();

  // ✅ PRIORITY 7: Serial Commands
  handleSerialCommands();

  // ✅ PRIORITY 8: Status Display (reduced frequency)
  static unsigned long lastStatusPrint = 0;
  if (millis() - lastStatusPrint > 5000) { // Every 5 seconds
    printSystemStatus();
    lastStatusPrint = millis();
  }

  // ✅ Minimal delay for stability
  delay(100);
}

void connectToWiFiStable() {
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
    Serial.printf("[WIFI] Signal: %d dBm\n", WiFi.RSSI());
  } else {
    Serial.println("\n[WIFI] ✗ Connection failed!");
  }
}

void checkWiFiStability() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WIFI] ⚠️ Disconnected! Reconnecting...");
    connectToWiFiStable();
  } else {
    // Check signal strength
    int rssi = WiFi.RSSI();
    if (rssi < -80) {
      Serial.printf("[WIFI] ⚠️ Weak signal: %d dBm\n", rssi);
    }
  }
}

void checkHelmetModuleStatusFast() {
  if (WiFi.status() != WL_CONNECTED) {
    isHelmetModuleActive = false;
    return;
  }

  HTTPClient http;
  http.setTimeout(2000); // 2 second timeout
  http.begin(firebaseHost + helmetDevicePath);
  
  int code = http.GET();
  
  if (code == HTTP_CODE_OK) {
    String response = http.getString();
    
    // ✅ DEBUG: Show raw Firebase response
    Serial.println("\n[HELMET DEBUG] ═══════════════════════════════");
    Serial.println("[HELMET DEBUG] Raw Firebase Response:");
    Serial.println(response);
    
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, response);
    
    if (!error) {
      String status = doc["status"] | "Off";
      // ✅ FIX: Use 64-bit integers to prevent overflow
      uint64_t lastHeartbeat = doc["lastHeartbeat"] | 0;
      uint64_t now = 1700000000000ULL + (uint64_t)millis();
      
      // ✅ DEBUG: Show timestamp comparison
      Serial.printf("[HELMET DEBUG] Status Field: '%s'\n", status.c_str());
      Serial.printf("[HELMET DEBUG] Helmet Heartbeat: %llu\n", lastHeartbeat);
      Serial.printf("[HELMET DEBUG] Motorcycle Time: %llu\n", now);
      Serial.printf("[HELMET DEBUG] Difference: %llu\n", 
                    (now > lastHeartbeat) ? (now - lastHeartbeat) : (lastHeartbeat - now));
      
      uint64_t timeSinceHeartbeat = (now > lastHeartbeat) ? 
                                   (now - lastHeartbeat) : 999999;
      
      isHelmetModuleActive = (status == "On" && timeSinceHeartbeat < 10000);
      
      Serial.printf("[HELMET DEBUG] Time Since Heartbeat: %.1f seconds\n", timeSinceHeartbeat / 1000.0);
      Serial.printf("[HELMET DEBUG] Threshold: 10 seconds\n");
      Serial.printf("[HELMET DEBUG] Status Check: %s\n", status == "On" ? "PASS" : "FAIL");
      Serial.printf("[HELMET DEBUG] Heartbeat Check: %s\n", timeSinceHeartbeat < 10000 ? "PASS" : "FAIL");
      Serial.printf("[HELMET DEBUG] Final Result: %s\n", isHelmetModuleActive ? "✓ ACTIVE" : "✗ INACTIVE");
      Serial.println("[HELMET DEBUG] ═══════════════════════════════\n");
    } else {
      Serial.printf("[HELMET] JSON Parse Error: %s\n", error.c_str());
    }
  } else {
    Serial.printf("[HELMET] Firebase error: %d\n", code);
    isHelmetModuleActive = false;
  }
  
  http.end();
}

bool isAlcoholSafe() {
  if (WiFi.status() != WL_CONNECTED) return true; // Default safe if no connection
  
  HTTPClient http;
  http.setTimeout(1000); // 1 second timeout
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
  
  return (helmetOk && alcoholOk && wifiOk);
}

void enforceSafetyInterlock() {
  if (!engineRunning) {
    // Force relay OFF when engine not running
    digitalWrite(relayPin, HIGH); // ACTIVE-LOW: HIGH = OFF
    
    // Visual indicator
    if (canStartEngine()) {
      digitalWrite(lightIndicatorPin, HIGH); // Solid = ready
    } else {
      // Blink = not ready
      static unsigned long lastBlink = 0;
      if (millis() - lastBlink > 500) {
        digitalWrite(lightIndicatorPin, !digitalRead(lightIndicatorPin));
        lastBlink = millis();
      }
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
      
      // IMMEDIATE RELAY SHUTDOWN
      digitalWrite(relayPin, HIGH); // ACTIVE-LOW: HIGH = OFF
      engineRunning = false;
      
      Serial.printf("🚨 Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
      Serial.println("🚨 Engine power CUT!");
      
      // Send to Firebase (if WiFi available)
      if (WiFi.status() == WL_CONNECTED) {
        sendCrashToFirebase();
      }
      
      // Alert
      triggerAlert();
      
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
      digitalWrite(relayPin, HIGH);
      engineRunning = false;
      Serial.printf("Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
      triggerAlert();
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
    else if (cmd == "WIFI") {
      Serial.printf("[WIFI] Status: %s\n", WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
      if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("[WIFI] IP: %s | Signal: %d dBm\n", 
                      WiFi.localIP().toString().c_str(), WiFi.RSSI());
      }
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
  // ✅ FIX: Use 64-bit integer to prevent overflow
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

void triggerAlert() {
  for (int i = 0; i < 5; i++) {
    tone(buzzerPin, 1500, 200);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(200);
    digitalWrite(lightIndicatorPin, LOW);
    delay(200);
  }
}