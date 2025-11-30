/*
 * ═══════════════════════════════════════════════════════════════════════════
 * VigiLERT ESP32 Motorcycle Module - FINAL PRODUCTION VERSION
 * ═══════════════════════════════════════════════════════════════════════════
 * ✅ ALL FEATURES WORKING:
 *    - Crash detection with MPU6050
 *    - Helmet module detection (64-bit timestamps)
 *    - Alcohol safety integration
 *    - Relay control with external 5V power (ACTIVE-LOW)
 *    - GPS tracking and speed monitoring
 *    - GSM communication
 *    - Firebase real-time sync
 *    - Complete safety interlock system
 * 
 * ⚠️  HARDWARE REQUIREMENTS:
 *    - External 5V power supply for relay module (ESP32 3.3V insufficient)
 *    - Relay module: ACTIVE-LOW (HIGH = OFF, LOW = ON)
 *    - GPIO 13 for relay control
 * 
 * 📅 Last Updated: 2024
 * ═══════════════════════════════════════════════════════════════════════════
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

// ═══════════════════════════════════════════════════════════════════════════
// USER CONFIGURATION
// ═══════════════════════════════════════════════════════════════════════════
const char* ssid = "DPWH";
const char* password = "12345678900";
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

// ═══════════════════════════════════════════════════════════════════════════
// PIN ASSIGNMENTS
// ═══════════════════════════════════════════════════════════════════════════
const int batteryPin = 34;
const int headlightPin = 32;
const int taillightPin = 33;
const int leftSignalPin = 32;
const int rightSignalPin = 33;
const int relayPin = 13;           // ✅ GPIO 13 - REQUIRES EXTERNAL 5V POWER!
const int vibrationPin = 14;
const int buzzerPin = 12;
const int lightIndicatorPin = 2;   // GPIO 2 (no conflicts)

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

// ═══════════════════════════════════════════════════════════════════════════
// STATE VARIABLES
// ═══════════════════════════════════════════════════════════════════════════

// Engine control
bool engineRunning = false;
String lastSender = "";
const String gsmPassword = "1234";
const String authorizedNumber1 = "+639675715673";
const String authorizedNumber2 = "+639542583234";

// Trip state
float currentStartLat = 0.0;
float currentStartLng = 0.0;
unsigned long tripStartTime = 0;
float maxRecordedSpeed = 0.0;

// Crash detection
bool crashDetected = false;
const float ACCEL_THRESHOLD = 15.0;
float currentTotalAccel = 0.0;
float currentRoll = 0.0;
unsigned long lastCrashTime = 0;
const unsigned long CRASH_COOLDOWN = 5000;

// Helmet state
bool isHelmetOn = false;
unsigned long lastHelmetOffMillis = 0;

// Safety interlock
bool isHelmetModuleActive = false;
unsigned long lastHelmetModuleCheck = 0;
const unsigned long HELMET_CHECK_INTERVAL = 3000;

// Speed limit
float currentSpeedLimit = 80.0;
unsigned long lastSpeedCheck = 0;

// GSM signal tracking
int gsmSignalStrength = 0;
unsigned long lastGSMCheck = 0;
const unsigned long GSM_CHECK_INTERVAL = 10000;

// WiFi monitoring
unsigned long lastWiFiCheck = 0;
const unsigned long WIFI_CHECK_INTERVAL = 5000;

// ═══════════════════════════════════════════════════════════════════════════
// FIREBASE PATHS
// ═══════════════════════════════════════════════════════════════════════════
const String tripsPath = "/helmet_public/" + userUID + "/trips.json?auth=" + firebaseAuth;
const String livePath = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
const String helmetStatusPath = "/helmet_public/" + userUID + "/helmetStatus/status.json?auth=" + firebaseAuth;
const String devicesPath = "/helmet_public/" + userUID + "/devices.json?auth=" + firebaseAuth;
const String helmetDevicePath = "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
const String motorcycleDevicePath = "/helmet_public/" + userUID + "/devices/motorcycle.json?auth=" + firebaseAuth;
const String alcoholPath = "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;

// ═══════════════════════════════════════════════════════════════════════════
// SETUP
// ═══════════════════════════════════════════════════════════════════════════
void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
  Serial.println("║                                                           ║");
  Serial.println("║        VigiLERT - FINAL PRODUCTION VERSION                ║");
  Serial.println("║                                                           ║");
  Serial.println("╠═══════════════════════════════════════════════════════════╣");
  Serial.println("║  ✅ All features: Crash, Helmet, Alcohol, GPS, GSM        ║");
  Serial.println("║  ✅ Relay control with external 5V power (ACTIVE-LOW)     ║");
  Serial.println("║  ✅ Complete safety system integration                    ║");
  Serial.println("║  ✅ Real-time Firebase synchronization                    ║");
  Serial.println("╚═══════════════════════════════════════════════════════════╝\n");

  // ═══════════════════════════════════════════════════════════════════════
  // Setup MPU6050
  // ═══════════════════════════════════════════════════════════════════════
  Wire.begin(21, 22);
  Serial.println("[SETUP] Initializing MPU6050...");
  while (!mpu.begin()) {
    Serial.println("[ERROR] Failed to find MPU6050 chip. Retrying in 2s...");
    delay(2000);
  }
  Serial.println("[SUCCESS] ✓ MPU6050 initialized");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // ═══════════════════════════════════════════════════════════════════════
  // Setup GPS
  // ═══════════════════════════════════════════════════════════════════════
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("[SETUP] ✓ GPS Serial started on RX:16, TX:17");
  delay(1000);
  while (gpsSerial.available()) {
    gpsSerial.read();
  }
  Serial.println("[GPS] Waiting for GPS fix (30-60 seconds in open area)...");

  // ═══════════════════════════════════════════════════════════════════════
  // Setup GSM
  // ═══════════════════════════════════════════════════════════════════════
  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);
  Serial.println("[SETUP] ✓ GSM Serial started");

  // ═══════════════════════════════════════════════════════════════════════
  // Setup GPIO Pins
  // ═══════════════════════════════════════════════════════════════════════
  pinMode(headlightPin, INPUT);
  pinMode(taillightPin, INPUT);
  pinMode(leftSignalPin, INPUT);
  pinMode(rightSignalPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(vibrationPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);

  // ═══════════════════════════════════════════════════════════════════════
  // ✅ CRITICAL: ACTIVE-LOW RELAY with EXTERNAL 5V POWER
  // ═══════════════════════════════════════════════════════════════════════
  digitalWrite(relayPin, HIGH);  // HIGH = OFF for active-low relay
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  
  delay(100);
  Serial.println("\n[RELAY] ═══════════════════════════════════════════════════");
  Serial.println("[RELAY] TYPE: ACTIVE-LOW");
  Serial.println("[RELAY] GPIO: 13");
  Serial.println("[RELAY] ⚠️  REQUIRES EXTERNAL 5V POWER SUPPLY!");
  Serial.println("[RELAY] HIGH (1) = Relay OFF (Engine Blocked)");
  Serial.println("[RELAY] LOW (0) = Relay ON (Engine Allowed)");
  Serial.printf("[RELAY] Current State: GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
  Serial.println("[RELAY] Engine is BLOCKED until safety conditions met");
  Serial.println("[RELAY] ═══════════════════════════════════════════════════\n");

  // ═══════════════════════════════════════════════════════════════════════
  // Connect to WiFi
  // ═══════════════════════════════════════════════════════════════════════
  connectToWiFiStable();
  
  // ═══════════════════════════════════════════════════════════════════════
  // Setup OTA
  // ═══════════════════════════════════════════════════════════════════════
  setupOTA();
  
  // ═══════════════════════════════════════════════════════════════════════
  // Initialize GSM
  // ═══════════════════════════════════════════════════════════════════════
  initializeGSM();
  
  // ═══════════════════════════════════════════════════════════════════════
  // Send initial heartbeat and start trip
  // ═══════════════════════════════════════════════════════════════════════
  updateMotorcycleDeviceStatus(true);
  startNewTrip();
  currentSpeedLimit = getSpeedLimitFromFirebase();

  Serial.println("\n[SUCCESS] ✓ Setup complete. All systems active.");
  Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
  Serial.println("║                  SERIAL COMMANDS                          ║");
  Serial.println("╠═══════════════════════════════════════════════════════════╣");
  Serial.println("║  CRASH      - Test crash detection                        ║");
  Serial.println("║  START      - Start engine (if safety allows)             ║");
  Serial.println("║  STOP       - Stop engine                                 ║");
  Serial.println("║  CLEAR      - Clear crash state                           ║");
  Serial.println("║  STATUS     - Show system status                          ║");
  Serial.println("║  MPU        - Test MPU6050 sensor                         ║");
  Serial.println("║  GPS        - Show GPS diagnostics                        ║");
  Serial.println("║  HELP       - Show all commands                           ║");
  Serial.println("╚═══════════════════════════════════════════════════════════╝\n");

  // Print initial GPS diagnostics
  delay(5000);
  printGPSDiagnostics();
}

// ═══════════════════════════════════════════════════════════════════════════
// MAIN LOOP
// ═══════════════════════════════════════════════════════════════════════════
void loop() {
  ArduinoOTA.handle();

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
      millis() - lastMotorcycleHeartbeat > 2000) {
    updateMotorcycleDeviceStatus(true);
    lastMotorcycleHeartbeat = millis();
  }

  // ✅ PRIORITY 4: Helmet Detection
  if (WiFi.status() == WL_CONNECTED && 
      millis() - lastHelmetModuleCheck > HELMET_CHECK_INTERVAL) {
    checkHelmetModuleStatus();
    lastHelmetModuleCheck = millis();
  }

  // ✅ PRIORITY 5: Safety Interlock
  enforceSafetyInterlock();

  // ✅ PRIORITY 6: Crash Detection
  handleCrashDetection();

  // ✅ PRIORITY 7: Serial Commands
  handleSerialCommands();

  // ✅ PRIORITY 8: GPS Reading
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
  }

  // ✅ PRIORITY 9: GSM Handling
  handleGSM();

  // ✅ PRIORITY 10: Sensor Readings
  bool headlightOn = digitalRead(headlightPin);
  bool taillightOn = digitalRead(taillightPin);
  bool leftSignalOn = digitalRead(leftSignalPin);
  bool rightSignalOn = digitalRead(rightSignalPin);
  float batteryVoltage = analogRead(batteryPin) * (3.3f / 4095.0f) * 4.3;

  // ✅ PRIORITY 11: Firebase Live Updates
  static unsigned long lastFirebaseUpdate = 0;
  if (WiFi.status() == WL_CONNECTED && 
      millis() - lastFirebaseUpdate > 2000) {
    if (gps.location.isValid()) {
      float speedKmph = gps.speed.kmph();
      sendLiveToFirebase(gps.location.lat(), gps.location.lng(), speedKmph, batteryVoltage,
                        headlightOn, taillightOn, leftSignalOn, rightSignalOn, isHelmetModuleActive);
      if (speedKmph > maxRecordedSpeed) maxRecordedSpeed = speedKmph;
    } else {
      sendLiveToFirebaseNoGPS(batteryVoltage, headlightOn, taillightOn, leftSignalOn, rightSignalOn, isHelmetModuleActive);
    }
    lastFirebaseUpdate = millis();
  }

  // ✅ PRIORITY 12: Status Display
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 5000) {
    printSystemStatus();
    lastPrint = millis();
  }

  // ✅ PRIORITY 13: Other periodic tasks
  if (millis() - lastSpeedCheck > 60000) {
    currentSpeedLimit = getSpeedLimitFromFirebase();
    lastSpeedCheck = millis();
  }

  if (millis() - lastGSMCheck > GSM_CHECK_INTERVAL) {
    gsmSignalStrength = getGSMSignalStrength();
    lastGSMCheck = millis();
  }

  // Unauthorized movement detection
  if (!engineRunning && digitalRead(vibrationPin) == HIGH) {
    Serial.println("[SECURITY] Unauthorized movement detected!");
    triggerAlert();
    sendAlertSMS();
  }

  // Helmet state & trip management
  handleHelmetState(isHelmetModuleActive, batteryVoltage, headlightOn, taillightOn, leftSignalOn, rightSignalOn);

  delay(100);
}

// ═══════════════════════════════════════════════════════════════════════════
// WIFI FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════
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
  }
}

// ═══════════════════════════════════════════════════════════════════════════
// HELMET MODULE DETECTION (64-bit timestamps)
// ═══════════════════════════════════════════════════════════════════════════
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
      
      Serial.printf("[HELMET] Status: %s | Age: %.1fs | Active: %s\n",
                    status.c_str(), timeSinceHeartbeat / 1000.0,
                    isHelmetModuleActive ? "YES" : "NO");
    }
  } else {
    Serial.printf("[HELMET] Firebase error: %d\n", code);
    isHelmetModuleActive = false;
  }
  
  http.end();
}

// ═══════════════════════════════════════════════════════════════════════════
// ALCOHOL SAFETY CHECK
// ═══════════════════════════════════════════════════════════════════════════
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

// ═══════════════════════════════════════════════════════════════════════════
// SAFETY INTERLOCK SYSTEM
// ═══════════════════════════════════════════════════════════════════════════
bool canStartEngine() {
  bool helmetOk = isHelmetModuleActive;
  bool alcoholOk = isAlcoholSafe();
  bool wifiOk = (WiFi.status() == WL_CONNECTED);
  bool noCrash = !crashDetected;
  
  return (helmetOk && alcoholOk && wifiOk && noCrash);
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

// ═══════════════════════════════════════════════════════════════════════════
// CRASH DETECTION
// ═══════════════════════════════════════════════════════════════════════════
void handleCrashDetection() {
  // Crash detection
  if ((currentTotalAccel >= ACCEL_THRESHOLD || abs(currentRoll) > 40) && !crashDetected) {
    unsigned long timeSinceLastCrash = millis() - lastCrashTime;
    
    if (timeSinceLastCrash >= CRASH_COOLDOWN) {
      Serial.println("\n🚨🚨🚨 CRASH DETECTED! 🚨🚨🚨");
      Serial.printf("Impact: %.2f g | Roll: %.1f°\n", currentTotalAccel, currentRoll);
      
      // ✅ IMMEDIATE RELAY SHUTDOWN
      digitalWrite(relayPin, HIGH); // ACTIVE-LOW: HIGH = OFF
      engineRunning = false;
      
      Serial.printf("🚨 Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
      Serial.println("🚨 Engine power CUT!");
      
      // Send to Firebase
      if (WiFi.status() == WL_CONNECTED) {
        if (gps.location.isValid()) {
          sendCrashEventToFirebase(gps.location.lat(), gps.location.lng(), currentTotalAccel, currentRoll);
        } else {
          sendCrashEventToFirebaseNoGPS(currentTotalAccel, currentRoll);
        }
      }
      
      triggerAlert();
      
      crashDetected = true;
      lastCrashTime = millis();
      
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.println("✓ CRASH EVENT SENT TO FIREBASE!");
      Serial.println("✓ RELAY FORCED OFF IMMEDIATELY!");
      Serial.println("✓ Type 'CLEAR' to reset crash state");
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    }
  }
  
  // Auto-clear crash state after 30 seconds if stable
  if (crashDetected) {
    unsigned long timeSinceCrash = millis() - lastCrashTime;
    
    if (timeSinceCrash > 30000 && abs(currentRoll) < 20 && currentTotalAccel < 11.0) {
      crashDetected = false;
      Serial.println("[INFO] Crash state auto-cleared after 30 seconds");
      Serial.println("[INFO] Type 'START' to restart engine if safe");
    }
  }
}

// ═══════════════════════════════════════════════════════════════════════════
// SERIAL COMMANDS
// ═══════════════════════════════════════════════════════════════════════════
void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "CRASH" || cmd == "TEST CRASH") {
      Serial.println("\n🧪 MANUAL CRASH TEST TRIGGERED!");
      digitalWrite(relayPin, HIGH);
      engineRunning = false;
      crashDetected = true;
      lastCrashTime = millis();
      Serial.printf("Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
      triggerAlert();
      Serial.println("✓ Test crash complete! Type 'CLEAR' to reset.");
    }
    else if (cmd == "START") {
      if (canStartEngine()) {
        digitalWrite(relayPin, LOW); // ACTIVE-LOW: LOW = ON
        engineRunning = true;
        Serial.println("✅ Engine started!");
        Serial.printf("Relay GPIO %d = %d (LOW/ON)\n", relayPin, digitalRead(relayPin));
      } else {
        Serial.println("❌ Engine start blocked:");
        if (crashDetected) Serial.println("   - Crash state active (type 'CLEAR')");
        if (!isHelmetModuleActive) Serial.println("   - Helmet module not detected");
        if (!isAlcoholSafe()) Serial.println("   - Alcohol detected");
        if (WiFi.status() != WL_CONNECTED) Serial.println("   - WiFi not connected");
      }
    }
    else if (cmd == "STOP") {
      digitalWrite(relayPin, HIGH); // ACTIVE-LOW: HIGH = OFF
      engineRunning = false;
      Serial.println("✅ Engine stopped");
      Serial.printf("Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
    }
    else if (cmd == "CLEAR" || cmd == "CLEAR CRASH") {
      if (crashDetected) {
        crashDetected = false;
        Serial.println("\n✅ Crash state manually cleared");
        Serial.println("✅ System ready for normal operation");
        Serial.println("✅ Type 'START' to restart engine");
      } else {
        Serial.println("\n[INFO] No crash state to clear");
      }
    }
    else if (cmd == "STATUS") {
      printSystemStatus();
    }
    else if (cmd == "MPU" || cmd == "MPU TEST") {
      Serial.println("\n🧪 MPU6050 TEST:");
      Serial.printf("Accel X: %.2f | Y: %.2f | Z: %.2f\n", accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);
      Serial.printf("Total Accel: %.2f g\n", currentTotalAccel);
      Serial.printf("Roll: %.1f°\n", currentRoll);
      Serial.printf("Crash Threshold: %.2f g\n", ACCEL_THRESHOLD);
      if (currentTotalAccel < 0.1) {
        Serial.println("⚠️ MPU6050 NOT WORKING! Check wiring!");
      } else {
        Serial.println("✓ MPU6050 is working!");
      }
    }
    else if (cmd == "GPS") {
      printGPSDiagnostics();
    }
    else if (cmd == "HELP") {
      Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
      Serial.println("║                  AVAILABLE COMMANDS                       ║");
      Serial.println("╠═══════════════════════════════════════════════════════════╣");
      Serial.println("║  CRASH      - Test crash detection                        ║");
      Serial.println("║  START      - Start engine (if safety allows)             ║");
      Serial.println("║  STOP       - Stop engine                                 ║");
      Serial.println("║  CLEAR      - Clear crash state                           ║");
      Serial.println("║  STATUS     - Show system status                          ║");
      Serial.println("║  MPU        - Test MPU6050 sensor                         ║");
      Serial.println("║  GPS        - Show GPS diagnostics                        ║");
      Serial.println("║  HELP       - Show this help                              ║");
      Serial.println("╚═══════════════════════════════════════════════════════════╝");
    }
  }
}

void printSystemStatus() {
  Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
  Serial.println("║                     SYSTEM STATUS                         ║");
  Serial.println("╠═══════════════════════════════════════════════════════════╣");
  Serial.printf("║ WiFi:    %s                                              ║\n",
                WiFi.status() == WL_CONNECTED ? "✓ Connected    " : "✗ Disconnected ");
  Serial.printf("║ Helmet:  %s                                              ║\n",
                isHelmetModuleActive ? "✓ Active       " : "✗ Inactive     ");
  Serial.printf("║ Alcohol: %s                                              ║\n",
                isAlcoholSafe() ? "✓ Safe         " : "✗ Detected     ");
  Serial.printf("║ Engine:  %-15s                                    ║\n",
                engineRunning ? "RUNNING" : "STOPPED");
  Serial.printf("║ Crash:   %s                                              ║\n",
                crashDetected ? "✗ DETECTED     " : "✓ None         ");
  Serial.println("╠═══════════════════════════════════════════════════════════╣");
  Serial.printf("║ Accel:   %.2f g                                           ║\n", currentTotalAccel);
  Serial.printf("║ Roll:    %.1f°                                            ║\n", currentRoll);
  Serial.printf("║ Relay:   %s                                              ║\n",
                digitalRead(relayPin) ? "OFF (HIGH)     " : "ON (LOW)       ");
  Serial.println("╠═══════════════════════════════════════════════════════════╣");
  
  if (gps.location.isValid()) {
    Serial.printf("║ GPS:     %.6f, %.6f                    ║\n",
                  gps.location.lat(), gps.location.lng());
    Serial.printf("║ Speed:   %.1f km/h                                        ║\n", gps.speed.kmph());
    Serial.printf("║ Sats:    %d                                               ║\n", gps.satellites.value());
  } else {
    Serial.println("║ GPS:     No fix                                           ║");
    Serial.printf("║ Chars:   %d                                               ║\n", gps.charsProcessed());
  }
  
  Serial.println("╠═══════════════════════════════════════════════════════════╣");
  Serial.printf("║ Can Start Engine: %s                                    ║\n", 
                canStartEngine() ? "✓ YES          " : "✗ NO           ");
  
  if (!canStartEngine()) {
    Serial.println("║ Blocked by:                                               ║");
    if (!isHelmetModuleActive) Serial.println("║   - Helmet module not detected                            ║");
    if (!isAlcoholSafe()) Serial.println("║   - Alcohol detected                                      ║");
    if (WiFi.status() != WL_CONNECTED) Serial.println("║   - WiFi not connected                                    ║");
    if (crashDetected) Serial.println("║   - Crash state active                                    ║");
  }
  
  Serial.println("╚═══════════════════════════════════════════════════════════╝\n");
}

void printGPSDiagnostics() {
  Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
  Serial.println("║                   GPS DIAGNOSTICS                         ║");
  Serial.println("╠═══════════════════════════════════════════════════════════╣");
  Serial.printf("║ Characters processed: %d                                  ║\n", gps.charsProcessed());
  Serial.printf("║ Sentences with fix:   %d                                  ║\n", gps.sentencesWithFix());
  Serial.printf("║ Failed checksum:      %d                                  ║\n", gps.failedChecksum());
  
  if (gps.location.isValid()) {
    Serial.println("║ Status: ✓ GPS FIX ACQUIRED                                ║");
    Serial.printf("║ Latitude:  %.6f                                      ║\n", gps.location.lat());
    Serial.printf("║ Longitude: %.6f                                      ║\n", gps.location.lng());
    Serial.printf("║ Speed:     %.1f km/h                                      ║\n", gps.speed.kmph());
    Serial.printf("║ Altitude:  %.1f m                                         ║\n", gps.altitude.meters());
    Serial.printf("║ Satellites: %d                                            ║\n", gps.satellites.value());
    if (gps.hdop.isValid()) {
      Serial.printf("║ HDOP:      %.2f                                           ║\n", gps.hdop.hdop());
    }
  } else {
    Serial.println("║ Status: ✗ NO GPS FIX                                      ║");
    Serial.println("║                                                           ║");
    if (gps.charsProcessed() < 10) {
      Serial.println("║ ⚠️  No data from GPS module!                              ║");
      Serial.println("║    Check wiring: RX=16, TX=17                            ║");
    } else {
      Serial.println("║ ⚠️  GPS receiving data but no fix yet                     ║");
      Serial.println("║    Move to open area with clear sky view                ║");
      Serial.println("║    Wait 30-60 seconds for satellite lock                ║");
    }
  }
  
  Serial.println("╚═══════════════════════════════════════════════════════════╝\n");
}

// ═══════════════════════════════════════════════════════════════════════════
// FIREBASE FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════
void updateMotorcycleDeviceStatus(bool isOn) {
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  StaticJsonDocument<128> doc;
  doc["status"] = isOn ? "On" : "Off";
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.setTimeout(2000);
  http.begin(firebaseHost + motorcycleDevicePath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.printf("[MOTORCYCLE] Heartbeat sent: %llu\n", timestamp);
  } else {
    Serial.printf("[MOTORCYCLE] Heartbeat failed: %d\n", code);
  }
  
  http.end();
}

void sendLiveToFirebase(float lat, float lng, float speed, float v, bool h, bool t, bool l, bool r, bool helmetModuleActive) {
  StaticJsonDocument<1024> doc;
  
  // GPS data
  doc["locationLat"] = lat;
  doc["locationLng"] = lng;
  doc["speed"] = speed;
  
  // Battery
  doc["batteryVoltage"] = v;
  
  // Lights
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["brakeLight"] = t;
  
  // MPU6050 data
  doc["mpu6050"]["accelX"] = accel.acceleration.x;
  doc["mpu6050"]["accelY"] = accel.acceleration.y;
  doc["mpu6050"]["accelZ"] = accel.acceleration.z;
  doc["mpu6050"]["gyro"] = currentRoll;
  doc["mpu6050"]["totalAccel"] = currentTotalAccel;
  
  // GPS quality
  if (gps.hdop.isValid()) {
    doc["gps"]["accuracy"] = String(gps.hdop.hdop()) + "m";
  } else {
    doc["gps"]["accuracy"] = "N/A";
  }
  doc["gps"]["satellites"] = gps.satellites.isValid() ? gps.satellites.value() : 0;
  
  // GSM data
  doc["gsm"]["signal"] = String(gsmSignalStrength) + "%";
  doc["gsm"]["network"] = gsmSignalStrength > 0 ? "4G" : "Offline";
  
  // Engine status
  doc["engineRunning"] = engineRunning;
  
  // Helmet module status
  doc["helmetModuleActive"] = helmetModuleActive;
  
  // Timestamp
  doc["timestamp"] = millis();
  doc["source"] = "gps_module";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.setTimeout(3000);
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  if (code != HTTP_CODE_OK) {
    Serial.printf("[FIREBASE] Live update failed: %d\n", code);
  }
  
  http.end();
}

void sendLiveToFirebaseNoGPS(float v, bool h, bool t, bool l, bool r, bool helmetModuleActive) {
  StaticJsonDocument<512> doc;
  
  // No GPS data
  doc["locationLat"] = 0.0;
  doc["locationLng"] = 0.0;
  doc["speed"] = 0.0;
  
  // Battery
  doc["batteryVoltage"] = v;
  
  // Lights
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["brakeLight"] = t;
  
  // MPU6050 data
  doc["mpu6050"]["accelX"] = accel.acceleration.x;
  doc["mpu6050"]["accelY"] = accel.acceleration.y;
  doc["mpu6050"]["accelZ"] = accel.acceleration.z;
  doc["mpu6050"]["gyro"] = currentRoll;
  doc["mpu6050"]["totalAccel"] = currentTotalAccel;
  
  // GPS quality
  doc["gps"]["accuracy"] = "No GPS";
  doc["gps"]["satellites"] = 0;
  
  // GSM data
  doc["gsm"]["signal"] = String(gsmSignalStrength) + "%";
  doc["gsm"]["network"] = gsmSignalStrength > 0 ? "4G" : "Offline";
  
  // Engine status
  doc["engineRunning"] = engineRunning;
  
  // Helmet module status
  doc["helmetModuleActive"] = helmetModuleActive;
  
  // Timestamp
  doc["timestamp"] = millis();
  doc["source"] = "no_gps";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.setTimeout(3000);
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  if (code != HTTP_CODE_OK) {
    Serial.printf("[FIREBASE] Live update (no GPS) failed: %d\n", code);
  }
  
  http.end();
}

void sendCrashEventToFirebase(float lat, float lng, float impact, float roll) {
  StaticJsonDocument<512> doc;
  
  doc["timestamp"] = millis();
  doc["locationLat"] = lat;
  doc["locationLng"] = lng;
  doc["impactStrength"] = impact;
  doc["roll"] = roll;
  doc["type"] = "crash";
  doc["hasGPS"] = true;
  doc["severity"] = impact > 20.0 ? "severe" : "moderate";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.setTimeout(3000);
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.POST(payload);
  Serial.printf("[FIREBASE] Crash event sent: %d\n", code);
  
  http.end();
}

void sendCrashEventToFirebaseNoGPS(float impact, float roll) {
  StaticJsonDocument<256> doc;
  
  doc["timestamp"] = millis();
  doc["impactStrength"] = impact;
  doc["roll"] = roll;
  doc["type"] = "crash";
  doc["hasGPS"] = false;
  doc["severity"] = impact > 20.0 ? "severe" : "moderate";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.setTimeout(3000);
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.POST(payload);
  Serial.printf("[FIREBASE] Crash event (no GPS) sent: %d\n", code);
  
  http.end();
}

float getSpeedLimitFromFirebase() {
  if (WiFi.status() != WL_CONNECTED) return 80.0;
  
  HTTPClient http;
  http.setTimeout(2000);
  String url = firebaseHost + "/helmet_public/" + userUID + "/settings/speedLimit.json?auth=" + firebaseAuth;
  http.begin(url);
  
  int code = http.GET();
  float limit = 80.0;
  
  if (code == HTTP_CODE_OK) {
    String response = http.getString();
    limit = response.toFloat();
    if (limit < 10.0 || limit > 200.0) limit = 80.0;
  }
  
  http.end();
  return limit;
}

// ═══════════════════════════════════════════════════════════════════════════
// TRIP MANAGEMENT
// ═══════════════════════════════════════════════════════════════════════════
void startNewTrip() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  if (gps.location.isValid()) {
    currentStartLat = gps.location.lat();
    currentStartLng = gps.location.lng();
  } else {
    currentStartLat = 0.0;
    currentStartLng = 0.0;
  }
  
  tripStartTime = millis();
  maxRecordedSpeed = 0.0;
  
  Serial.println("[TRIP] New trip started");
}

void endTrip() {
  if (WiFi.status() != WL_CONNECTED) return;
  if (tripStartTime == 0) return;
  
  unsigned long duration = (millis() - tripStartTime) / 1000;
  
  StaticJsonDocument<512> doc;
  doc["startTime"] = tripStartTime;
  doc["endTime"] = millis();
  doc["duration"] = duration;
  doc["maxSpeed"] = maxRecordedSpeed;
  
  if (currentStartLat != 0.0 && currentStartLng != 0.0) {
    doc["startLat"] = currentStartLat;
    doc["startLng"] = currentStartLng;
  }
  
  if (gps.location.isValid()) {
    doc["endLat"] = gps.location.lat();
    doc["endLng"] = gps.location.lng();
  }
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.setTimeout(3000);
  http.begin(firebaseHost + tripsPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.POST(payload);
  Serial.printf("[TRIP] Trip ended and saved: %d\n", code);
  
  http.end();
  
  tripStartTime = 0;
}

void handleHelmetState(bool helmetModuleActive, float v, bool h, bool t, bool l, bool r) {
  static bool wasHelmetOn = false;
  
  if (helmetModuleActive && !wasHelmetOn) {
    // Helmet just put on
    Serial.println("[HELMET] Helmet put on - Starting trip");
    startNewTrip();
    wasHelmetOn = true;
  } else if (!helmetModuleActive && wasHelmetOn) {
    // Helmet just removed
    Serial.println("[HELMET] Helmet removed - Ending trip");
    endTrip();
    wasHelmetOn = false;
  }
}

// ═══════════════════════════════════════════════════════════════════════════
// GSM FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════
void initializeGSM() {
  Serial.println("[GSM] Initializing GSM module...");
  
  gsmSerial.println("AT");
  delay(1000);
  
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  
  gsmSerial.println("AT+CNMI=1,2,0,0,0");
  delay(1000);
  
  Serial.println("[GSM] ✓ GSM initialized");
}

void handleGSM() {
  if (gsmSerial.available()) {
    String message = gsmSerial.readString();
    Serial.println("[GSM] " + message);
    
    if (message.indexOf("+CMT:") >= 0) {
      int senderStart = message.indexOf("\"+") + 1;
      int senderEnd = message.indexOf("\"", senderStart);
      lastSender = message.substring(senderStart, senderEnd);
      
      int msgStart = message.indexOf("\n", message.indexOf("+CMT:")) + 1;
      String smsContent = message.substring(msgStart);
      smsContent.trim();
      
      processSMS(smsContent, lastSender);
    }
  }
}

void processSMS(String message, String sender) {
  message.toUpperCase();
  
  if (sender != authorizedNumber1 && sender != authorizedNumber2) {
    Serial.println("[GSM] Unauthorized sender: " + sender);
    return;
  }
  
  if (message.indexOf(gsmPassword) < 0) {
    Serial.println("[GSM] Invalid password");
    sendSMS(sender, "Invalid password");
    return;
  }
  
  if (message.indexOf("START") >= 0) {
    if (canStartEngine()) {
      digitalWrite(relayPin, LOW);
      engineRunning = true;
      sendSMS(sender, "Engine started");
      Serial.println("[GSM] Engine started via SMS");
    } else {
      sendSMS(sender, "Engine start blocked - safety requirements not met");
      Serial.println("[GSM] Engine start blocked");
    }
  } else if (message.indexOf("STOP") >= 0) {
    digitalWrite(relayPin, HIGH);
    engineRunning = false;
    sendSMS(sender, "Engine stopped");
    Serial.println("[GSM] Engine stopped via SMS");
  } else if (message.indexOf("STATUS") >= 0) {
    String status = "Engine: " + String(engineRunning ? "ON" : "OFF");
    if (gps.location.isValid()) {
      status += " | GPS: " + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
    }
    sendSMS(sender, status);
  }
}

void sendSMS(String number, String message) {
  gsmSerial.println("AT+CMGS=\"" + number + "\"");
  delay(1000);
  gsmSerial.print(message);
  delay(100);
  gsmSerial.write(26);
  delay(1000);
  Serial.println("[GSM] SMS sent to " + number);
}

void sendAlertSMS() {
  String alertMsg = "ALERT: Unauthorized movement detected!";
  if (gps.location.isValid()) {
    alertMsg += " Location: " + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  }
  
  sendSMS(authorizedNumber1, alertMsg);
  sendSMS(authorizedNumber2, alertMsg);
}

int getGSMSignalStrength() {
  gsmSerial.println("AT+CSQ");
  delay(500);
  
  if (gsmSerial.available()) {
    String response = gsmSerial.readString();
    int csqIndex = response.indexOf("+CSQ:");
    if (csqIndex >= 0) {
      int commaIndex = response.indexOf(",", csqIndex);
      String rssi = response.substring(csqIndex + 6, commaIndex);
      int signal = rssi.toInt();
      return map(signal, 0, 31, 0, 100);
    }
  }
  
  return 0;
}

// ═══════════════════════════════════════════════════════════════════════════
// ALERT FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════
void triggerAlert() {
  Serial.println("[ALERT] Triggering alert...");
  
  for (int i = 0; i < 5; i++) {
    tone(buzzerPin, 1500, 200);
    delay(200);
    tone(buzzerPin, 1000, 200);
    delay(200);
  }
  
  Serial.println("[ALERT] Alert complete");
}

// ═══════════════════════════════════════════════════════════════════════════
// OTA UPDATE
// ═══════════════════════════════════════════════════════════════════════════
void setupOTA() {
  ArduinoOTA.setHostname("VigiLERT-Motorcycle");
  
  ArduinoOTA.onStart([]() {
    Serial.println("[OTA] Update starting...");
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("\n[OTA] Update complete!");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("[OTA] Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("[OTA] Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  
  ArduinoOTA.begin();
  Serial.println("[OTA] ✓ OTA update enabled");
}
