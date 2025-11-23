/*
VigiLERT ESP32 Motorcycle Module - OPTIMIZED VERSION
✅ Faster Firebase updates
✅ Minimal serial output
✅ Real-time crash detection
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

// ✅ PERFORMANCE MODE - Minimal output for maximum speed
#define DEBUG_MODE false  // Set to true for debugging
#define SHOW_MPU_READINGS true  // Show MPU data for monitoring

// ======= PIN ASSIGNMENTS =======
const int batteryPin = 34;
const int headlightPin = 32;
const int taillightPin = 33;
const int leftSignalPin = 32;  // Moved from 25
const int rightSignalPin = 33;  // Moved from 26
const int relayPin = 25;  // Single relay on GPIO 25
const int vibrationPin = 14;
const int buzzerPin = 12;
const int lightIndicatorPin = 13;

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
String lastSender = "";
const String gsmPassword = "1234";
const String authorizedNumber1 = "+639675715673";
const String authorizedNumber2 = "+639542583234";

// Trip state
float currentStartLat = 0.0;
float currentStartLng = 0.0;
unsigned long tripStartTime = 0;
float maxRecordedSpeed = 0.0;

// Crash & helmet state
bool crashDetected = false;
const float ACCEL_THRESHOLD = 20.0;  // Increased from 15.0g to 20.0g (only severe impacts)
bool isHelmetOn = false;
unsigned long lastHelmetOffMillis = 0;

// Safety interlock
bool isHelmetModuleActive = false;
unsigned long lastHelmetModuleCheck = 0;
const unsigned long HELMET_CHECK_INTERVAL = 5000;

// Speed limit
float currentSpeedLimit = 80.0;
unsigned long lastSpeedCheck = 0;

// GSM signal tracking
int gsmSignalStrength = 0;
unsigned long lastGSMCheck = 0;
const unsigned long GSM_CHECK_INTERVAL = 10000;

// Store current sensor readings
float currentRoll = 0.0;
float currentTotalAccel = 0.0;
float previousTotalAccel = 9.81;
unsigned long lastCrashCheck = 0;
const unsigned long CRASH_CHECK_INTERVAL = 100;

// Crash cooldown
unsigned long lastCrashTime = 0;
const unsigned long CRASH_COOLDOWN = 5000;

// Firebase paths
const String tripsPath = "/helmet_public/" + userUID + "/trips.json?auth=" + firebaseAuth;
const String livePath = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
const String helmetStatusPath = "/helmet_public/" + userUID + "/helmetStatus/status.json?auth=" + firebaseAuth;
const String devicesPath = "/helmet_public/" + userUID + "/devices.json?auth=" + firebaseAuth;

// ======= SETUP =======
void setup() {
  Serial.begin(115200);
  delay(100);
  
  #if DEBUG_MODE
    Serial.println("\n[SETUP] Starting VigiLERT System - OPTIMIZED VERSION...");
  #endif

  Wire.begin(21, 22);
  while (!mpu.begin()) {
    #if DEBUG_MODE
      Serial.println("[ERROR] MPU6050 not found. Retrying...");
    #endif
    delay(2000);
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  delay(500);
  while (gpsSerial.available()) gpsSerial.read();

  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);

  pinMode(headlightPin, INPUT);
  pinMode(taillightPin, INPUT);
  pinMode(leftSignalPin, INPUT);
  pinMode(rightSignalPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(vibrationPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);

  // ✅ IMPORTANT: Most relay modules are ACTIVE-LOW
  // LOW = Relay ON, HIGH = Relay OFF
  digitalWrite(relayPin, HIGH);  // Start with relay OFF (active-LOW logic)
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  
  Serial.println("✅ Relay initialized:");
  Serial.printf("   GPIO %d: HIGH (Relay OFF)\n", relayPin);

  connectToWiFi();
  setupOTA();
  initializeGSM();
  
  updateMotorcycleDeviceStatus(true);
  startNewTrip();
  currentSpeedLimit = getSpeedLimitFromFirebase();

  #if DEBUG_MODE
    Serial.println("[SUCCESS] Setup complete!");
    Serial.println("Commands: CRASH, STATUS, HELP");
  #endif
}

// ======= MAIN LOOP =======
void loop() {
  ArduinoOTA.handle();

  // Serial commands (minimal)
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();

    if (cmd == "CRASH") {
      Serial.println("\n🧪 ========== CRASH TEST ==========");
      Serial.printf("WiFi Status: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED" : "DISCONNECTED");
      Serial.printf("Firebase Host: %s\n", firebaseHost.c_str());
      Serial.printf("User UID: %s\n", userUID.c_str());
      Serial.printf("Crash Path: %s\n", crashPath.c_str());
      
      if (gps.location.isValid()) {
        Serial.printf("Sending crash WITH GPS: %.6f, %.6f\n", gps.location.lat(), gps.location.lng());
        sendCrashEventToFirebase(gps.location.lat(), gps.location.lng(), 20.0, currentRoll);
      } else {
        Serial.println("Sending crash WITHOUT GPS");
        sendCrashEventToFirebaseNoGPS(20.0, currentRoll);
      }
      triggerAlert();
      crashDetected = true;
      Serial.println("✓ Test crash sent - check Firebase and dashboard!");
      Serial.println("==================================\n");
    }
    else if (cmd == "STATUS") {
      Serial.printf("Engine: %s | Helmet: %s | Alcohol: %s | GPS: %s\n",
        engineRunning ? "ON" : "OFF",
        isHelmetModuleActive ? "ON" : "OFF",
        isAlcoholSafe() ? "SAFE" : "DANGER",
        gps.location.isValid() ? "VALID" : "NO FIX");
      Serial.printf("GPS: Lat=%.6f Lng=%.6f Sats=%d\n",
        gps.location.lat(), gps.location.lng(), gps.satellites.value());
    }
    else if (cmd == "GPS") {
      Serial.printf("GPS Status: %s\n", gps.location.isValid() ? "VALID" : "NO FIX");
      Serial.printf("Lat: %.6f | Lng: %.6f\n", gps.location.lat(), gps.location.lng());
      Serial.printf("Satellites: %d | HDOP: %.1f\n", gps.satellites.value(), gps.hdop.hdop());
      Serial.printf("Chars processed: %d | Sentences: %d\n", gps.charsProcessed(), gps.sentencesWithFix());
    }
    else if (cmd == "RELAY") {
      Serial.println("\n🧪 ========== RELAY TEST ==========");
      Serial.println("Testing relay on GPIO 2...");
      
      Serial.println("1. Setting relay HIGH (should turn OFF for active-LOW)");
      digitalWrite(relayPin, HIGH);
      delay(1000);
      
      Serial.println("2. Setting relay LOW (should turn ON for active-LOW)");
      digitalWrite(relayPin, LOW);
      delay(1000);
      
      Serial.println("3. Setting relay HIGH again");
      digitalWrite(relayPin, HIGH);
      
      Serial.println("✓ Relay test complete!");
      Serial.println("Did you see the LED change? Did you hear clicking?");
      Serial.println("==================================\n");
    }
    else if (cmd == "RELAYON") {
      Serial.println("🔴 Forcing relay ON (LOW signal)");
      digitalWrite(relayPin, LOW);
      Serial.println("✓ Relay should be ON now - LED should be lit");
    }
    else if (cmd == "RELAYOFF") {
      Serial.println("🟢 Forcing relay OFF (HIGH signal)");
      digitalWrite(relayPin, HIGH);
      Serial.println("✓ Relay should be OFF now - LED should be off");
    }
    else if (cmd == "TESTCRASH") {
      Serial.println("\n🧪 FORCING CRASH STATE (bypassing confirmation)");
      crashDetected = true;
      digitalWrite(relayPin, HIGH);  // Force relay OFF
      Serial.println("✓ Crash state set to TRUE");
      Serial.println("✓ Relay forced HIGH (OFF)");
      Serial.println("✓ Check if relay LED turned OFF!");
    }
    else if (cmd == "CLEAR") {
      Serial.println("🔄 Clearing crash state");
      crashDetected = false;
      Serial.println("✓ Crash cleared - relay can be controlled normally");
    }
    else if (cmd == "HELP") {
      Serial.println("Commands: CRASH, TESTCRASH, CLEAR, RELAY, RELAYON, RELAYOFF, STATUS, GPS, HELP");
    }
  }

  // GPS reading (no debug output)
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  handleGSM();

  // Sensor readings
  bool headlightOn = digitalRead(headlightPin);
  bool taillightOn = digitalRead(taillightPin);
  bool leftSignalOn = digitalRead(leftSignalPin);
  bool rightSignalOn = digitalRead(rightSignalPin);
  float batteryVoltage = analogRead(batteryPin) * (3.3f / 4095.0f) * 4.3;

  // MPU6050 data
  mpu.getEvent(&accel, &gyro, &temp);
  currentTotalAccel = sqrt(
    accel.acceleration.x * accel.acceleration.x +
    accel.acceleration.y * accel.acceleration.y +
    accel.acceleration.z * accel.acceleration.z
  );
  currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
  float leanAngle = abs(currentRoll);

  // GSM signal check
  if (millis() - lastGSMCheck > GSM_CHECK_INTERVAL) {
    gsmSignalStrength = getGSMSignalStrength();
    lastGSMCheck = millis();
  }

  // ✅ CRITICAL: Check helmet and alcohol status (every 10 seconds)
  if (millis() - lastHelmetModuleCheck > 10000) {  // Every 10s
    checkHelmetModuleStatus();  // Check if helmet is connected
    lastHelmetModuleCheck = millis();
  }

  // ✅ COMPREHENSIVE SAFETY INTERLOCK - Check all 3 conditions
  static unsigned long lastSafetyCheck = 0;
  if (millis() - lastSafetyCheck > 100) {  // Check every 100ms
    
    // Get current safety status
    bool helmetConnected = isHelmetModuleActive;
    bool alcoholSafe = isAlcoholSafe();
    bool noCrash = !crashDetected;
    
    // Determine if engine should be allowed
    bool safeToRun = helmetConnected && alcoholSafe && noCrash;
    
    // ✅ RELAY CONTROL: Turn OFF if ANY safety condition fails
    if (!safeToRun) {
      digitalWrite(relayPin, HIGH);  // Force relay OFF (active-LOW logic)
      
      // If engine was running, stop it
      if (engineRunning) {
        engineRunning = false;
        Serial.println("\n⚠️⚠️⚠️ SAFETY SHUTDOWN ⚠️⚠️⚠️");
        if (!helmetConnected) Serial.println("  ❌ Helmet disconnected");
        if (!alcoholSafe) Serial.println("  ❌ Alcohol detected");
        if (!noCrash) Serial.println("  ❌ Crash detected");
        Serial.printf("Setting GPIO %d to HIGH (relay OFF)\n", relayPin);
        Serial.printf("GPIO %d state: %d\n", relayPin, digitalRead(relayPin));
        Serial.println("✓ Relay OFF - Engine stopped\n");
      }
      
      // Debug: Print relay state every 5 seconds when not safe
      static unsigned long lastDebug = 0;
      if (millis() - lastDebug > 5000) {
        Serial.printf("[DEBUG] Safety: Helmet=%s Alcohol=%s Crash=%s | GPIO %d=%d\n",
          helmetConnected ? "OK" : "FAIL",
          alcoholSafe ? "OK" : "FAIL", 
          noCrash ? "OK" : "FAIL",
          relayPin, digitalRead(relayPin));
        lastDebug = millis();
      }
      
      // LED indicator - different patterns for different states
      static unsigned long lastBlink = 0;
      
      if (crashDetected) {
        // CRASH STATE: Fast blinking (200ms) - RED ALERT
        if (millis() - lastBlink > 200) {
          digitalWrite(lightIndicatorPin, !digitalRead(lightIndicatorPin));
          lastBlink = millis();
        }
      } else if (!alcoholSafe) {
        // ALCOHOL DETECTED: Medium blinking (300ms) - DANGER
        if (millis() - lastBlink > 300) {
          digitalWrite(lightIndicatorPin, !digitalRead(lightIndicatorPin));
          lastBlink = millis();
        }
      } else if (!helmetConnected) {
        // HELMET DISCONNECTED: Slow blinking (500ms) - WARNING
        if (millis() - lastBlink > 500) {
          digitalWrite(lightIndicatorPin, !digitalRead(lightIndicatorPin));
          lastBlink = millis();
        }
      }
    } else {
      // All safety conditions met - relay can be controlled normally
      if (engineRunning) {
        digitalWrite(relayPin, LOW);  // Relay ON (active-LOW logic)
        digitalWrite(lightIndicatorPin, LOW);  // LED off when running
      } else {
        digitalWrite(relayPin, HIGH);  // Relay OFF (active-LOW logic)
        digitalWrite(lightIndicatorPin, HIGH);  // LED solid when ready
      }
    }
    
    lastSafetyCheck = millis();
  }

  // Heartbeat (REDUCED FREQUENCY for speed)
  static unsigned long lastMotorcycleHeartbeat = 0;
  if (millis() - lastMotorcycleHeartbeat > 30000) {  // Every 30s instead of 3s
    // updateMotorcycleDeviceStatus(true);  // DISABLED - HTTP request blocks loop
    lastMotorcycleHeartbeat = millis();
  }

  // ✅ REAL-TIME MPU6050 DISPLAY - Shows sensor values continuously
  #if SHOW_MPU_READINGS
  static unsigned long lastMPUDisplay = 0;
  if (millis() - lastMPUDisplay > 50) {  // Update 20 times per second for ultra-fast monitoring!
    Serial.printf("MPU: Accel=%.2fg Roll=%.1f° Pitch=%.1f° | Threshold=%.1fg | Crash=%s\n",
      currentTotalAccel, currentRoll, 
      atan2(-accel.acceleration.x, sqrt(accel.acceleration.y * accel.acceleration.y + accel.acceleration.z * accel.acceleration.z)) * 180.0 / PI,
      ACCEL_THRESHOLD,
      crashDetected ? "YES" : "NO");
    lastMPUDisplay = millis();
  }
  #endif

  // ✅ ULTRA-ACCURATE CRASH DETECTION - Fast response with confirmation
  static int crashConfirmCount = 0;
  static unsigned long firstCrashDetection = 0;
  
  // ✅ ADJUSTED: Only trigger on SEVERE lean (beyond normal riding angles)
  // Normal motorcycle lean in turns: 20-30° (safe)
  // Crash/fall lean: >60° (dangerous)
  bool crashCondition = (currentTotalAccel >= ACCEL_THRESHOLD || currentRoll < -60 || currentRoll > 60);
  
  if (crashCondition && !crashDetected) {
    unsigned long timeSinceLastCrash = millis() - lastCrashTime;
    
    if (timeSinceLastCrash >= CRASH_COOLDOWN) {
      // Confirmation logic: Need 2 consecutive readings within 100ms
      if (crashConfirmCount == 0) {
        firstCrashDetection = millis();
        crashConfirmCount = 1;
      } else if (millis() - firstCrashDetection < 100) {
        crashConfirmCount++;
      } else {
        // Reset if too much time passed
        crashConfirmCount = 1;
        firstCrashDetection = millis();
      }
      
      // Trigger crash after 3 confirmations (reduces false positives)
      if (crashConfirmCount >= 3) {
        Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
        Serial.printf("Impact: %.2fg | Roll: %.1f°\n", currentTotalAccel, currentRoll);
        
        // Send to Firebase immediately (no delays)
        if (gps.location.isValid()) {
          Serial.printf("📍 Sending WITH GPS: %.6f, %.6f\n", gps.location.lat(), gps.location.lng());
          sendCrashEventToFirebase(gps.location.lat(), gps.location.lng(), currentTotalAccel, currentRoll);
        } else {
          Serial.println("⚠️ Sending WITHOUT GPS (will not show on map)");
          sendCrashEventToFirebaseNoGPS(currentTotalAccel, currentRoll);
        }
        
        crashDetected = true;
        lastCrashTime = millis();
        crashConfirmCount = 0;  // Reset confirmation counter
        
        // ✅ CRITICAL: Immediately cut power to relay (safety feature)
        Serial.println("🚨 EMERGENCY SHUTDOWN - Cutting relay power!");
        Serial.printf("Setting GPIO %d to HIGH (Relay OFF)\n", relayPin);
        digitalWrite(relayPin, HIGH);  // Force relay OFF immediately (active-LOW logic)
        Serial.printf("GPIO %d state: %d (should be 1 = HIGH)\n", relayPin, digitalRead(relayPin));
        digitalWrite(lightIndicatorPin, LOW);  // Turn off indicator
        engineRunning = false;  // Update engine state
        Serial.println("✓ Relay command sent - Check if relay LED is OFF");
        
        triggerAlert();
        
        Serial.println("✓ Relay OFF - Engine cannot start");
        Serial.println("✓ Crash sent to Firebase - check dashboard!\n");
      }
    }
  } else if (!crashCondition) {
    // Reset confirmation if no crash condition
    crashConfirmCount = 0;
  }

  // Clear crash state when stable
  if (crashDetected) {
    bool isUpright = (currentRoll > -30 && currentRoll < 30);
    bool isStable = (currentTotalAccel < ACCEL_THRESHOLD - 2.0);
    
    if (isUpright && isStable) {
      crashDetected = false;
      #if DEBUG_MODE
        Serial.println("✓ Crash cleared");
      #endif
    }
  }

  // Unauthorized movement
  if (!engineRunning && digitalRead(vibrationPin) == HIGH) {
    triggerAlert();
    sendAlertSMS();
  }

  // Helmet state management
  handleHelmetState(isHelmetModuleActive, batteryVoltage, headlightOn, taillightOn, leftSignalOn, rightSignalOn);

  // ✅ Firebase live updates (DISABLED for ultra-fast MPU reading)
  static unsigned long lastFirebaseUpdate = 0;
  if (millis() - lastFirebaseUpdate >= 5000) {  // Every 5s (HTTP blocks loop!)
    lastFirebaseUpdate = millis();
    // DISABLED - Enable only if you need live updates (will slow down MPU reading)
    
    // DISABLED for ultra-fast MPU reading
    // if (gps.location.isValid()) {
    //   float speedKmph = gps.speed.kmph();
    //   sendLiveToFirebase(gps.location.lat(), gps.location.lng(), speedKmph, batteryVoltage,
    //                      headlightOn, taillightOn, leftSignalOn, rightSignalOn, isHelmetModuleActive);
    //   if (speedKmph > maxRecordedSpeed) maxRecordedSpeed = speedKmph;
    // } else {
    //   sendLiveToFirebaseNoGPS(batteryVoltage, headlightOn, taillightOn, leftSignalOn, rightSignalOn, isHelmetModuleActive);
    // }
  }

  // Speed limit update
  if (millis() - lastSpeedCheck > 60000) {
    currentSpeedLimit = getSpeedLimitFromFirebase();
    lastSpeedCheck = millis();
  }

  // ✅ ULTRA-FAST: Minimal delay for instant response
  delay(5);  // Only 5ms delay for maximum speed!
}

// ======= OPTIMIZED FUNCTIONS =======

int getGSMSignalStrength() {
  gsmSerial.println("AT+CSQ");
  delay(300);  // Reduced delay
  
  String response = "";
  unsigned long timeout = millis() + 500;
  while (millis() < timeout && gsmSerial.available()) {
    response += (char)gsmSerial.read();
  }

  int csqIndex = response.indexOf("+CSQ:");
  if (csqIndex != -1) {
    int commaIndex = response.indexOf(",", csqIndex);
    if (commaIndex != -1) {
      String rssiStr = response.substring(csqIndex + 6, commaIndex);
      rssiStr.trim();
      int rssi = rssiStr.toInt();
      if (rssi == 99 || rssi == 0) return 0;
      return (rssi * 100) / 31;
    }
  }
  return 0;
}

void sendLiveToFirebase(float lat, float lng, float speed, float v, bool h, bool t, bool l, bool r, bool helmetModuleActive) {
  StaticJsonDocument<512> doc;  // Reduced size
  
  doc["locationLat"] = lat;
  doc["locationLng"] = lng;
  doc["speed"] = speed;
  doc["batteryVoltage"] = v;
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["brakeLight"] = t;
  
  doc["mpu6050"]["accelX"] = accel.acceleration.x;
  doc["mpu6050"]["accelY"] = accel.acceleration.y;
  doc["mpu6050"]["accelZ"] = accel.acceleration.z;
  doc["mpu6050"]["gyro"] = currentRoll;
  doc["mpu6050"]["totalAccel"] = currentTotalAccel;
  
  doc["gps"]["satellites"] = gps.satellites.isValid() ? gps.satellites.value() : 0;
  doc["gsm"]["signal"] = String(gsmSignalStrength) + "%";
  doc["engineRunning"] = engineRunning;
  doc["helmetModuleActive"] = helmetModuleActive;
  doc["timestamp"] = millis();

  String payload;
  serializeJson(doc, payload);

  HTTPClient http;
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000);  // 2 second timeout
  http.PUT(payload);
  http.end();
}

void sendLiveToFirebaseNoGPS(float v, bool h, bool t, bool l, bool r, bool helmetModuleActive) {
  StaticJsonDocument<512> doc;
  
  doc["batteryVoltage"] = v;
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["brakeLight"] = t;
  
  doc["mpu6050"]["accelX"] = accel.acceleration.x;
  doc["mpu6050"]["accelY"] = accel.acceleration.y;
  doc["mpu6050"]["accelZ"] = accel.acceleration.z;
  doc["mpu6050"]["gyro"] = currentRoll;
  doc["mpu6050"]["totalAccel"] = currentTotalAccel;
  
  doc["gps"]["satellites"] = 0;
  doc["gsm"]["signal"] = String(gsmSignalStrength) + "%";
  doc["engineRunning"] = engineRunning;
  doc["helmetModuleActive"] = helmetModuleActive;
  doc["timestamp"] = millis();

  String payload;
  serializeJson(doc, payload);

  HTTPClient http;
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000);
  http.PUT(payload);
  http.end();
}

unsigned long gpsToUnixTime() {
  if (gps.time.isValid() && gps.date.isValid()) {
    tmElements_t tm;
    tm.Year = gps.date.year() - 1970;
    tm.Month = gps.date.month();
    tm.Day = gps.date.day();
    tm.Hour = gps.time.hour();
    tm.Minute = gps.time.minute();
    tm.Second = gps.time.second();
    return makeTime(tm);
  }
  return millis() / 1000UL;
}

void checkHelmetModuleStatus() {
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
  http.begin(url);
  http.setTimeout(2000);
  int code = http.GET();
  
  if (code == HTTP_CODE_OK) {
    String response = http.getString();
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, response);
    
    if (!error) {
      String status = doc["status"] | "Off";
      unsigned long lastHeartbeat = doc["lastHeartbeat"] | 0;
      unsigned long now = 1700000000000UL + millis();
      unsigned long timeSinceHeartbeat = (now > lastHeartbeat) ? (now - lastHeartbeat) : 999999;
      
      isHelmetModuleActive = (status == "On" && timeSinceHeartbeat < 15000);
    }
  } else {
    isHelmetModuleActive = false;
  }
  
  http.end();
}

void updateMotorcycleDeviceStatus(bool isOn) {
  unsigned long timestamp = 1700000000000UL + millis();
  
  StaticJsonDocument<128> doc;
  doc["status"] = isOn ? "On" : "Off";
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;

  String payload;
  serializeJson(doc, payload);

  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/motorcycle.json?auth=" + firebaseAuth;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000);
  http.PUT(payload);
  http.end();
}

bool isAlcoholSafe() {
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;
  http.begin(url);
  http.setTimeout(2000);
  int code = http.GET();
  
  if (code == HTTP_CODE_OK) {
    String response = http.getString();
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, response);
    
    if (!error) {
      String status = doc["status"] | "Safe";
      http.end();
      return (status == "Safe");
    }
  }
  http.end();
  return true;
}

bool canStartEngine() {
  bool helmetModuleOn = isHelmetModuleActive;
  bool motorcycleOn = true;
  bool alcoholSafe = isAlcoholSafe();
  
  #if DEBUG_MODE
    Serial.printf("Safety: Helmet=%s Alcohol=%s\n",
      helmetModuleOn ? "OK" : "NO",
      alcoholSafe ? "OK" : "NO");
  #endif
  
  return (motorcycleOn && helmetModuleOn && alcoholSafe);
}

void startEngine() {
  if (!canStartEngine()) {
    Serial.println("❌ ENGINE BLOCKED");
    return;
  }
  
  digitalWrite(relayPin, LOW);  // Turn relay ON (active-LOW logic)
  engineRunning = true;
  sendSMS("Engine Started");
  Serial.println("✅ ENGINE STARTED - Relay ON");
}

void stopEngine() {
  digitalWrite(relayPin, HIGH);  // Turn relay OFF (active-LOW logic)
  engineRunning = false;
  sendSMS("Engine Stopped");
  Serial.println("⏹️ ENGINE STOPPED - Relay OFF");
}

void handleGSM() {
  while (gsmSerial.available()) {
    String response = gsmSerial.readString();
    if (response.indexOf("RING") != -1) handleIncomingCall(response);
    if (response.indexOf("+CMT:") != -1) handleIncomingSMS(response);
  }
}

bool initializeGSM() {
  gsmSerial.println("AT"); delay(500);
  gsmSerial.println("AT+CMGF=1"); delay(500);
  gsmSerial.println("AT+CNMI=1,2,0,0,0"); delay(500);
  return true;
}

void handleIncomingCall(String response) {
  int start = response.indexOf("\"") + 1;
  int end = response.indexOf("\"", start);
  String caller = response.substring(start, end);
  
  if (caller != authorizedNumber1 && caller != authorizedNumber2) {
    gsmSerial.println("ATH");
    return;
  }
  
  if (engineRunning) stopEngine();
  else startEngine();
  
  gsmSerial.println("ATH");
}

void handleIncomingSMS(String response) {
  int start = response.indexOf("\"") + 1;
  int end = response.indexOf("\"", start);
  String sender = response.substring(start, end);
  sender.trim();
  
  if (sender != authorizedNumber1 && sender != authorizedNumber2) return;
  
  lastSender = sender;
  int bodyStart = response.indexOf("\r\n", end) + 2;
  String smsBody = response.substring(bodyStart);
  smsBody.trim();
  
  if (smsBody.startsWith(gsmPassword + " ")) {
    String command = smsBody.substring(gsmPassword.length() + 1);
    command.trim();
    command.toUpperCase();
    
    if (command == "START") startEngine();
    else if (command == "STOP") stopEngine();
    else if (command == "LOCATE") {
      if (gps.location.isValid()) {
        String locMsg = "GPS: " + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
        sendSMS(locMsg);
      } else {
        sendSMS("GPS unavailable");
      }
    }
    else if (command == "STATUS") {
      String statusMsg = "Helmet:" + String(isHelmetModuleActive ? "ON" : "OFF") +
                        " Engine:" + String(engineRunning ? "ON" : "OFF") +
                        " Alcohol:" + String(isAlcoholSafe() ? "OK" : "DANGER");
      sendSMS(statusMsg);
    }
  }
}

void sendSMS(String msg) {
  gsmSerial.print("AT+CMGS=\"" + lastSender + "\"\r");
  delay(500);
  if (gsmSerial.find(">")) {
    gsmSerial.print(msg);
    delay(100);
    gsmSerial.write(26);
  }
}

void triggerAlert() {
  for (int i = 0; i < 5; i++) {  // Reduced from 15 to 5
    digitalWrite(lightIndicatorPin, HIGH);
    tone(buzzerPin, 1500);
    delay(200);
    digitalWrite(lightIndicatorPin, LOW);
    noTone(buzzerPin);
    delay(200);
  }
}

void sendAlertSMS() {
  sendSMS("ALERT! Unauthorized movement!");
}

void sendCrashEventToFirebase(float lat, float lng, float impact, float leanAngle) {
  // Check WiFi connection first
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ WiFi not connected! Cannot send crash to Firebase.");
    return;
  }
  
  StaticJsonDocument<256> doc;
  
  // Use current time in milliseconds
  unsigned long timestamp = 1700000000000UL + millis();
  doc["timestamp"] = timestamp;
  doc["lat"] = lat;
  doc["lng"] = lng;
  doc["hasGPS"] = true;
  doc["impactStrength"] = impact;
  doc["roll"] = leanAngle;
  doc["type"] = "crash";

  String payload;
  serializeJson(doc, payload);
  
  Serial.println("📤 Crash payload (with GPS):");
  Serial.println(payload);
  
  String url = firebaseHost + crashPath;
  Serial.println("📍 Sending to: " + url);

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(5000);  // Increased timeout
  
  Serial.println("⏳ Sending POST request...");
  int code = http.POST(payload);
  
  Serial.printf("📥 Firebase response: %d\n", code);
  
  if (code == 200 || code == 201) {
    Serial.println("✅ Crash saved to Firebase!");
    String response = http.getString();
    Serial.println("Response body: " + response);
  } else if (code > 0) {
    Serial.printf("❌ Firebase error: %d\n", code);
    String response = http.getString();
    Serial.println("Error response: " + response);
  } else {
    Serial.printf("❌ HTTP request failed: %s\n", http.errorToString(code).c_str());
  }
  
  http.end();
}

void sendCrashEventToFirebaseNoGPS(float impact, float leanAngle) {
  // Check WiFi connection first
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ WiFi not connected! Cannot send crash to Firebase.");
    return;
  }
  
  StaticJsonDocument<256> doc;
  
  // Use current time in milliseconds
  unsigned long timestamp = 1700000000000UL + millis();
  doc["timestamp"] = timestamp;
  doc["hasGPS"] = false;
  doc["impactStrength"] = impact;
  doc["roll"] = leanAngle;
  doc["type"] = "crash";

  String payload;
  serializeJson(doc, payload);
  
  Serial.println("📤 Crash payload (no GPS):");
  Serial.println(payload);
  
  String url = firebaseHost + crashPath;
  Serial.println("📍 Sending to: " + url);

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(5000);  // Increased timeout
  
  Serial.println("⏳ Sending POST request...");
  int code = http.POST(payload);
  
  Serial.printf("📥 Firebase response: %d\n", code);
  
  if (code == 200 || code == 201) {
    Serial.println("✅ Crash saved to Firebase!");
    String response = http.getString();
    Serial.println("Response body: " + response);
  } else if (code > 0) {
    Serial.printf("❌ Firebase error: %d\n", code);
    String response = http.getString();
    Serial.println("Error response: " + response);
  } else {
    Serial.printf("❌ HTTP request failed: %s\n", http.errorToString(code).c_str());
  }
  
  http.end();
}

float getSpeedLimitFromFirebase() {
  HTTPClient http;
  http.begin(firebaseHost + "/helmet_public/" + userUID + "/settings/speedLimit.json?auth=" + firebaseAuth);
  http.setTimeout(2000);
  int code = http.GET();
  
  float limit = 80.0;
  if (code == HTTP_CODE_OK) {
    String res = http.getString();
    limit = res.toFloat();
  }
  http.end();
  return limit;
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("WiFi");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    Serial.print(".");
    delay(500);
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi OK");
  } else {
    Serial.println("\n❌ WiFi FAILED");
  }
}

void setupOTA() {
  ArduinoOTA.setHostname("HelmetSafetySystem");
  ArduinoOTA.setPassword("password");
  ArduinoOTA.begin();
}

void startNewTrip() {
  tripStartTime = millis();
  currentStartLat = gps.location.isValid() ? gps.location.lat() : 0.0;
  currentStartLng = gps.location.isValid() ? gps.location.lng() : 0.0;
  maxRecordedSpeed = 0;
}

void endCurrentTrip(float endLat, float endLng, float speed, float voltage, bool h, bool t, bool l, bool r, unsigned long startT, unsigned long endT) {
  StaticJsonDocument<512> doc;
  doc["startLat"] = currentStartLat;
  doc["startLng"] = currentStartLng;
  doc["endLat"] = endLat;
  doc["endLng"] = endLng;
  doc["startTime"] = startT;
  doc["endTime"] = endT;
  doc["duration"] = (endT - startT) / 60000.0;
  doc["maxSpeed"] = speed;
  doc["distance"] = sqrt(pow(endLat - currentStartLat, 2) + pow(endLng - currentStartLng, 2)) * 111.0 * 1000;
  doc["batteryVoltage"] = voltage;
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["status"] = "completed";

  String payload;
  serializeJson(doc, payload);

  HTTPClient http;
  http.begin(firebaseHost + tripsPath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);
  http.POST(payload);
  http.end();
}

void handleHelmetState(bool currentHelmetState, float voltage, bool h, bool t, bool l, bool r) {
  if (currentHelmetState && !isHelmetOn) {
    startNewTrip();
    isHelmetOn = true;
    crashDetected = false;
  }
  
  if (!currentHelmetState && isHelmetOn) {
    unsigned long endTime = millis();
    endCurrentTrip(
      gps.location.isValid() ? gps.location.lat() : currentStartLat,
      gps.location.isValid() ? gps.location.lng() : currentStartLng,
      gps.speed.kmph(), voltage, h, t, l, r,
      tripStartTime, endTime
    );
    
    isHelmetOn = false;
    lastHelmetOffMillis = millis();
    crashDetected = false;
    
    if (engineRunning) {
      Serial.println("⚠️ Helmet removed - stopping engine");
      stopEngine();
    }
  }
}
