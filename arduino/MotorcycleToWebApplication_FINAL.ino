#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>

// ═══════════════════════════════════════════════════════════════════════════
// VigiLERT Motorcycle Module - FINAL PRODUCTION VERSION
// ═══════════════════════════════════════════════════════════════════════════
// ✅ Synchronized with Dashboard.vue
// ✅ All features: Crash, Anti-theft, Engine control
// ✅ Correct Firebase paths
// ═══════════════════════════════════════════════════════════════════════════

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Configuration
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

// Owner phone number
const String ownerPhoneNumber = "+639675715673";

// Pin Assignments
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
const int physicalKeyPin = 14;  // ✅ NEW: Physical key switch input
bool lastKeyState = LOW;

// Current sensor readings
float currentRoll = 0.0;
float currentTotalAccel = 0.0;

// GPS speed
float currentSpeed = 0.0;
unsigned long lastGPSUpdate = 0;
const unsigned long GPS_UPDATE_INTERVAL = 500;  // Update every 500ms

// ✅ NEW: Trip tracking
struct TripData {
  double fromLat = 0.0;
  double fromLng = 0.0;
  double toLat = 0.0;
  double toLng = 0.0;
  uint64_t startTime = 0;
  uint64_t endTime = 0;
  float maxSpeed = 0.0;
  float totalDistance = 0.0;
  bool isActive = false;
  bool hasValidStart = false;
};

TripData currentTrip;
double lastValidLat = 0.0;
double lastValidLng = 0.0;

// Crash detection timing
unsigned long lastCrashTime = 0;
const unsigned long CRASH_COOLDOWN = 3000;  // ✅ Reduced to 3 seconds for faster re-detection

// Alcohol detection
bool alcoholDetected = false;
bool lastAlcoholState = false;
unsigned long lastAlcoholCheck = 0;
const unsigned long ALCOHOL_CHECK_INTERVAL = 250;  // Check every 250ms

// ✅ ENHANCED: Comprehensive security system with 5-second timeouts
bool helmetConnected = false;
unsigned long lastHelmetCheck = 0;
const unsigned long HELMET_CHECK_INTERVAL = 500;  // Check every 500ms
uint64_t lastHelmetHeartbeat = 0;  // Track last helmet heartbeat time
const unsigned long HELMET_TIMEOUT = 5000;  // ✅ 5-second timeout for helmet
const unsigned long HELMET_FORCE_OFF_TIMEOUT = 10000;  // Force status to Off after 10 seconds
unsigned long lastHelmetUpdateTime = 0;  // Track when we last saw an update
bool helmetStatusForcedOff = false;  // Track if we forced the status to Off

// ✅ NEW: WiFi connection monitoring for security
unsigned long lastWiFiCheck = 0;
unsigned long lastWiFiConnected = 0;
const unsigned long WIFI_CHECK_INTERVAL = 1000;  // Check WiFi every 1 second
const unsigned long WIFI_TIMEOUT = 5000;  // ✅ 5-second WiFi timeout
bool wifiSecurityEnabled = true;

// ✅ NEW: Comprehensive security state
bool securitySystemActive = true;
unsigned long lastSecurityCheck = 0;
const unsigned long SECURITY_CHECK_INTERVAL = 500;  // Check security every 500ms

// Automatic engine control
bool autoEngineControl = false;
bool engineStartRequested = false;

// Dashboard button control
bool lastDashboardButtonState = false;
unsigned long lastButtonCheck = 0;
const unsigned long BUTTON_CHECK_INTERVAL = 100;  // Check every 100ms

// Device heartbeat
unsigned long lastHeartbeat = 0;
const unsigned long HEARTBEAT_INTERVAL = 1000;  // 1 second

// ✅ NEW: Network security timeouts
unsigned long lastWiFiConnection = 0;
unsigned long lastFirebaseSuccess = 0;
const unsigned long NETWORK_TIMEOUT = 30000;  // 30 seconds without network = auto shutdown
const unsigned long FIREBASE_TIMEOUT = 60000;  // 60 seconds without Firebase = auto shutdown
bool networkSecurityEnabled = true;
bool systemInSecureMode = false;

// ✅ NEW: Real-time monitoring intervals
unsigned long lastRealtimeUpdate = 0;
const unsigned long REALTIME_INTERVAL = 250;  // 250ms = 4 times per second
unsigned long lastSensorPrint = 0;
const unsigned long SENSOR_PRINT_INTERVAL = 500;  // 500ms sensor readings
unsigned long lastQuickStatus = 0;
const unsigned long QUICK_STATUS_INTERVAL = 1000;  // 1 second quick status

// Anti-theft system
bool antiTheftEnabled = false;
bool antiTheftArmed = false;
unsigned long engineOffTime = 0;
const unsigned long ARM_DELAY = 10000;  // ✅ Reduced to 10 seconds
unsigned long lastTheftAlert = 0;
const unsigned long THEFT_ALERT_COOLDOWN = 60000;  // ✅ Reduced to 1 minute
bool theftAlertSent = false;
int theftDetectionCount = 0;
const int THEFT_DETECTION_REQUIRED = 1;  // ✅ Immediate alert on first detection
unsigned long lastVibrationTime = 0;
const unsigned long VIBRATION_DEBOUNCE = 10;  // ✅ Ultra-fast response (10ms)

// Escalating buzzer alert system
int consecutiveVibrations = 0;

// GSM status
bool gsmReady = false;

// Firebase paths
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
const String buttonPath = "/" + userUID + "/engineControl/startButton.json?auth=" + firebaseAuth;
const String livePath = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;
const String alcoholPath = "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;

// ✅ NEW: Function declarations for security system
void checkComprehensiveSecurity();
void triggerSecurityShutdown(String reason);
void checkPhysicalKeyWithSecurity();
void logSecurityEventToFirebase(String eventType);
void printSecurityStatus();

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   MOTORCYCLE MODULE - FINAL VERSION    ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ ⚡ 1s heartbeat                        ║");
  Serial.println("║ ✅ Dashboard synchronized              ║");
  Serial.println("║ ✅ All features enabled                ║");
  Serial.println("║ 🔒 5-second security timeouts          ║");
  Serial.println("║ 🛡️ Comprehensive theft protection     ║");
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
  pinMode(physicalKeyPin, INPUT_PULLUP);  // ✅ NEW: Physical key switch

  // ✅ FIX: Initialize relay to OFF (LOW for ACTIVE-HIGH relay)
  digitalWrite(relayPin, LOW);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  
  delay(100);
  Serial.println("\n[SETUP] ═══════════════════════════════════");
  Serial.println("[SETUP] RELAY TYPE: ACTIVE-HIGH");
  Serial.println("[SETUP] GPIO 13: Relay Control");
  Serial.println("[SETUP] ⚠️  REQUIRES EXTERNAL 5V POWER!");
  Serial.printf("[SETUP] Relay: %d (LOW/OFF)\n", digitalRead(relayPin));
  Serial.println("[SETUP] ═══════════════════════════════════\n");

  connectToWiFi();
  
  // Send initial heartbeat
  sendMotorcycleHeartbeat(true);
  lastHeartbeat = millis();

  Serial.println("\n📋 SERIAL COMMANDS:");
  Serial.println("   START ENGINE / STOP ENGINE");
  Serial.println("   ARM THEFT / DISARM THEFT");
  Serial.println("   SECURITY ON / SECURITY OFF");
  Serial.println("   SECURITY STATUS");
  Serial.println("   TEST VIBRATION");
  Serial.println("   MONITOR (10s real-time data)");
  Serial.println("   STATUS\n");
}

void loop() {
  // ✅ ENHANCED: Comprehensive security monitoring (5-second timeouts)
  if (millis() - lastSecurityCheck >= SECURITY_CHECK_INTERVAL) {
    checkComprehensiveSecurity();
    lastSecurityCheck = millis();
  }
  
  // ✅ NEW: Read GPS data continuously for real-time speed
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  
  // Update GPS speed every 500ms
  if (millis() - lastGPSUpdate >= GPS_UPDATE_INTERVAL) {
    if (gps.speed.isValid()) {
      currentSpeed = gps.speed.kmph();
    }
    lastGPSUpdate = millis();
  }
  
  // ✅ NEW: Update trip data continuously
  updateTripData();
  
  // ✅ ENHANCED: Monitor physical key switch (with comprehensive security)
  if (securitySystemActive) {
    checkPhysicalKeyWithSecurity();
  } else {
    checkPhysicalKey();  // Fallback to basic key monitoring
  }
  
  // Send heartbeat every 1 second
  if (millis() - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    bool heartbeatSent = sendMotorcycleHeartbeat(true);
    if (heartbeatSent) {
      lastFirebaseSuccess = millis();
      lastWiFiConnected = millis();  // ✅ Track successful WiFi communication
    }
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
    else if (cmd == "STATUS") {
      printSystemStatus();
    }
    else if (cmd == "CRASH") {
      triggerCrashShutdown(20.0, 45.0);
    }
    else if (cmd == "SECURITY ON") {
      securitySystemActive = true;
      Serial.println("✅ Comprehensive security ENABLED!");
      Serial.println("🔒 5-second timeout protection active");
    }
    else if (cmd == "SECURITY OFF") {
      securitySystemActive = false;
      systemInSecureMode = false;
      Serial.println("⚠️ Comprehensive security DISABLED!");
      Serial.println("⚠️ WARNING: Vehicle is now vulnerable to theft!");
    }
    else if (cmd == "SECURITY STATUS") {
      printSecurityStatus();
    }
    else if (cmd == "TEST VIBRATION") {
      Serial.println("🧪 TESTING VIBRATION SENSOR...");
      int reading = digitalRead(vibrationSensorPin);
      Serial.printf("📍 GPIO %d current reading: %d\n", vibrationSensorPin, reading);
      Serial.printf("🛡️ Anti-theft armed: %s\n", antiTheftArmed ? "YES" : "NO");
      Serial.printf("🛡️ Anti-theft enabled: %s\n", antiTheftEnabled ? "YES" : "NO");
      
      // Force trigger for testing
      if (antiTheftArmed) {
        Serial.println("🚨 FORCE TRIGGERING VIBRATION ALERT!");
        consecutiveVibrations++;
        Serial.printf("🚨 [TEST] VIBRATION DETECTED #%d!\n", consecutiveVibrations);
        
        // Test buzzer
        for (int i = 0; i < 3; i++) {
          digitalWrite(buzzerPin, HIGH);
          digitalWrite(lightIndicatorPin, HIGH);
          delay(200);
          digitalWrite(buzzerPin, LOW);
          digitalWrite(lightIndicatorPin, LOW);
          delay(200);
        }
      } else {
        Serial.println("⚠️ Anti-theft not armed - cannot test");
      }
    }
    else if (cmd == "MONITOR") {
      Serial.println("📊 REAL-TIME MONITORING MODE (10 seconds)");
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      
      unsigned long monitorStart = millis();
      while (millis() - monitorStart < 10000) {  // 10 seconds
        // Read all sensors
        mpu.getEvent(&accel, &gyro, &temp);
        float totalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                               accel.acceleration.y * accel.acceleration.y +
                               accel.acceleration.z * accel.acceleration.z);
        float roll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
        int vibration = digitalRead(vibrationSensorPin);
        int physicalKey = digitalRead(physicalKeyPin);
        
        Serial.printf("[LIVE] Accel:%.1fg Roll:%.1f° Vib:%d Key:%d Speed:%.1fkph WiFi:%s\n",
                      totalAccel, roll, vibration, physicalKey, currentSpeed,
                      WiFi.status() == WL_CONNECTED ? "OK" : "FAIL");
        
        delay(200);  // 5 times per second
      }
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.println("📊 MONITORING COMPLETE");
    }
  }

  // ✅ OPTIMIZED: Read MPU6050 continuously (no delay) for instant crash detection
  mpu.getEvent(&accel, &gyro, &temp);
  currentTotalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                           accel.acceleration.y * accel.acceleration.y +
                           accel.acceleration.z * accel.acceleration.z);
  currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
  float leanAngle = abs(currentRoll);

  // Anti-theft system
  static int vibrationCheckCounter = 0;
  vibrationCheckCounter++;
  
  // ✅ FIX: Check anti-theft EVERY loop for instant response
  if (!engineRunning) {
    handleAntiTheftWithVibrationSensor();
  } else {
    if (antiTheftArmed) {
      Serial.println("[ANTI-THEFT] 🔓 Disarmed - Engine running");
      antiTheftArmed = false;
      theftDetectionCount = 0;
      theftAlertSent = false;
    }
  }

  // ✅ OPTIMIZED: Crash detection with continuous monitoring
  if (engineRunning && (currentTotalAccel >= ACCEL_THRESHOLD || leanAngle > 40) && !crashDetected) {
    unsigned long timeSinceLastCrash = millis() - lastCrashTime;
    if (timeSinceLastCrash >= CRASH_COOLDOWN) {
      Serial.printf("[CRASH] Detected! Accel: %.2f g, Lean: %.1f°\n", currentTotalAccel, leanAngle);
      triggerCrashShutdown(currentTotalAccel, currentRoll);
    } else {
      // Still in cooldown period
      static unsigned long lastCooldownMsg = 0;
      if (millis() - lastCooldownMsg > 1000) {
        Serial.printf("[CRASH] In cooldown (%lu ms remaining)\n", CRASH_COOLDOWN - timeSinceLastCrash);
        lastCooldownMsg = millis();
      }
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

  // ✅ NEW: Helmet connection monitoring
  if (millis() - lastHelmetCheck > HELMET_CHECK_INTERVAL) {
    checkHelmetConnection();
    lastHelmetCheck = millis();
  }
  
  // ✅ ENHANCED: Auto-shutdown logic is now handled in checkComprehensiveSecurity()
  // This ensures consistent 5-second timeout enforcement

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

  // ✅ FIX: Safety override - turn relay OFF if alcohol detected
  if (alcoholDetected) {
    digitalWrite(relayPin, LOW);  // LOW = OFF for ACTIVE-HIGH relay
  }

  // ✅ REAL-TIME: Fast sensor monitoring every 500ms
  if (millis() - lastSensorPrint > SENSOR_PRINT_INTERVAL) {
    Serial.printf("[SENSORS] Accel: %.2f g | Lean: %.1f° | Speed: %.1f kph | Vibration: %d\n", 
                  currentTotalAccel, abs(currentRoll), currentSpeed, digitalRead(vibrationSensorPin));
    lastSensorPrint = millis();
  }

  // ✅ QUICK STATUS: System status every 1 second
  if (millis() - lastQuickStatus > QUICK_STATUS_INTERVAL) {
    Serial.printf("[STATUS] Engine:%s | Relay:%s | Helmet:%s | Alcohol:%s | Security:%s\n",
                  engineRunning ? "RUN" : "STOP",
                  digitalRead(relayPin) ? "ON" : "OFF",
                  helmetConnected ? "CONN" : "DISC",
                  alcoholDetected ? "DANGER" : "SAFE",
                  systemInSecureMode ? "SECURE" : "NORMAL");
    lastQuickStatus = millis();
  }

  // ✅ REAL-TIME: Firebase updates every 250ms for faster dashboard response
  if (millis() - lastRealtimeUpdate > REALTIME_INTERVAL) {
    sendLiveToFirebase();
    lastRealtimeUpdate = millis();
  }

  // ✅ FIX: Remove delay for faster crash detection
  // MPU6050 reads continuously without delay
  yield();  // Allow WiFi/system tasks to run
}

bool sendMotorcycleHeartbeat(bool isActive) {
  if (WiFi.status() != WL_CONNECTED) return false;

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
    Serial.printf("[HEARTBEAT] ✓ Sent (timestamp: %llu)\n", timestamp);
    return true;
  } else {
    Serial.printf("[HEARTBEAT] ✗ Failed: HTTP %d\n", code);
    return false;
  }
  
  http.end();
  return false;
}

void handleAntiTheftWithVibrationSensor() {
  // ✅ FAST DEBUG: Show current sensor reading every 500ms
  static unsigned long lastSensorDebug = 0;
  if (millis() - lastSensorDebug > 500) {
    int currentReading = digitalRead(vibrationSensorPin);
    Serial.printf("[VIBRATION] Pin %d = %d | Armed: %s | Enabled: %s\n", 
                  vibrationSensorPin, currentReading,
                  antiTheftArmed ? "YES" : "NO",
                  antiTheftEnabled ? "YES" : "NO");
    lastSensorDebug = millis();
  }

  if (!antiTheftEnabled) {
    antiTheftEnabled = true;
    engineOffTime = millis();
    consecutiveVibrations = 0;
    Serial.println("\n[ANTI-THEFT] 🛡️ System enabled - arming in 10 seconds...");
    return;
  }

  if (!antiTheftArmed && (millis() - engineOffTime >= ARM_DELAY)) {
    antiTheftArmed = true;
    theftDetectionCount = 0;
    theftAlertSent = false;
    consecutiveVibrations = 0;
    Serial.println("\n[ANTI-THEFT] 🛡️ ARMED! Vibration sensor active...");
    Serial.printf("[ANTI-THEFT] 📍 Monitoring GPIO %d for HIGH signals\n", vibrationSensorPin);
    
    // Short beep to confirm arming
    for (int i = 0; i < 2; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    }
  }

  if (antiTheftArmed) {
    int vibrationDetected = digitalRead(vibrationSensorPin);
    
    // ✅ ENHANCED: More sensitive detection
    if (vibrationDetected == HIGH) {
      unsigned long timeSinceLastVibration = millis() - lastVibrationTime;
      
      // ✅ ULTRA-FAST: Reduced debounce to 10ms
      if (timeSinceLastVibration >= VIBRATION_DEBOUNCE) {
        theftDetectionCount++;
        consecutiveVibrations++;
        lastVibrationTime = millis();
        
        Serial.printf("\n🚨🚨 [ANTI-THEFT] VIBRATION DETECTED #%d! 🚨🚨\n", consecutiveVibrations);
        Serial.printf("[ANTI-THEFT] 📍 GPIO %d went HIGH!\n", vibrationSensorPin);

        // ✅ IMMEDIATE buzzer response
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(lightIndicatorPin, HIGH);
        delay(200);  // Longer initial beep
        digitalWrite(buzzerPin, LOW);
        digitalWrite(lightIndicatorPin, LOW);
        
        // ✅ Additional alert sequence
        for (int i = 0; i < 5; i++) {
          digitalWrite(buzzerPin, HIGH);
          digitalWrite(lightIndicatorPin, HIGH);
          delay(100);
          digitalWrite(buzzerPin, LOW);
          digitalWrite(lightIndicatorPin, LOW);
          delay(100);
        }

        // ✅ ALWAYS send SMS alert on vibration (with cooldown to prevent spam)
        unsigned long timeSinceLastAlert = millis() - lastTheftAlert;
        
        if (timeSinceLastAlert >= THEFT_ALERT_COOLDOWN) {
          Serial.println("[ANTI-THEFT] 📱 Sending SMS + Firebase alert...");
          triggerTheftAlert();
          lastTheftAlert = millis();
          theftAlertSent = true;
          theftDetectionCount = 0;
        } else {
          Serial.printf("[ANTI-THEFT] ⏳ SMS cooldown active (%lu seconds remaining)\n", 
                        (THEFT_ALERT_COOLDOWN - timeSinceLastAlert) / 1000);
        }
      } else {
        // Still in debounce period
        Serial.printf("[VIBRATION] Debounce active (%lu ms remaining)\n", 
                      VIBRATION_DEBOUNCE - timeSinceLastVibration);
      }
    }
    
    // ✅ ENHANCED: Show LOW to HIGH transitions
    static int lastVibrationReading = LOW;
    if (vibrationDetected != lastVibrationReading) {
      Serial.printf("[VIBRATION] Pin %d changed: %s → %s\n", 
                    vibrationSensorPin,
                    lastVibrationReading ? "HIGH" : "LOW",
                    vibrationDetected ? "HIGH" : "LOW");
      lastVibrationReading = vibrationDetected;
    }
    
    // Reset counters after 10 seconds of no vibration
    if (consecutiveVibrations > 0 && (millis() - lastVibrationTime) > 10000) {
      consecutiveVibrations = 0;
      Serial.println("[ANTI-THEFT] ✓ Vibration counter reset");
    }
  }
}

void triggerTheftAlert() {
  Serial.println("\n🚨🚨🚨 THEFT ALERT! 🚨🚨🚨");
  
  for (int i = 0; i < 10; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(50);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(lightIndicatorPin, LOW);
    delay(50);
  }

  String location = "Location unavailable";
  if (gps.location.isValid()) {
    location = "https://maps.google.com/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  }

  String message = "VIGILERT THEFT ALERT!\nUnauthorized movement detected!\n" + location;

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

  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = timestamp;
  doc["type"] = "theft_attempt";
  doc["location"] = location;
  doc["vibrationDetected"] = true;
  doc["armed"] = antiTheftArmed;
  doc["alertTime"] = timestamp;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String theftPath = "/helmet_public/" + userUID + "/theft_alerts.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + theftPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.POST(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.println("[FIREBASE] ✓ Theft alert logged");
  } else {
    Serial.printf("[FIREBASE] ✗ Theft alert failed: %d\n", code);
  }
  
  http.end();
}

void triggerCrashShutdown(float impact, float roll) {
  Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
  Serial.printf("Impact: %.2f g | Roll: %.1f°\n", impact, roll);
  
  // ✅ FIX: LOW = OFF for ACTIVE-HIGH relay
  digitalWrite(relayPin, LOW);
  engineRunning = false;
  
  Serial.printf("🚨 Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
  
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
  Serial.println("\n[ENGINE] startEngine() called");
  
  // ✅ ENHANCED: Comprehensive security checks before engine start
  unsigned long currentTime = millis();
  unsigned long timeSinceWiFi = currentTime - lastWiFiConnected;
  unsigned long timeSinceHelmet = currentTime - lastHelmetUpdateTime;
  
  bool wifiOK = (timeSinceWiFi <= WIFI_TIMEOUT);
  bool helmetOK = (timeSinceHelmet <= HELMET_TIMEOUT || lastHelmetUpdateTime == 0 || helmetConnected);
  bool alcoholOK = !alcoholDetected;
  bool crashOK = !crashDetected;
  
  Serial.printf("[ENGINE] WiFi: %s (%lu ms ago)\n", wifiOK ? "OK ✅" : "TIMEOUT ❌", timeSinceWiFi);
  Serial.printf("[ENGINE] Helmet: %s (%lu ms ago)\n", helmetOK ? "OK ✅" : "TIMEOUT ❌", timeSinceHelmet);
  Serial.printf("[ENGINE] Alcohol: %s\n", alcoholOK ? "SAFE ✅" : "DETECTED ❌");
  Serial.printf("[ENGINE] Crash: %s\n", crashOK ? "NONE ✅" : "DETECTED ❌");
  Serial.printf("[ENGINE] Current state: %s\n", engineRunning ? "RUNNING" : "STOPPED");
  
  // ✅ 1. WiFi Security Check (5-second timeout)
  if (!wifiOK) {
    Serial.println("\n❌ ENGINE START BLOCKED - WiFi TIMEOUT!");
    Serial.printf("💡 WiFi disconnected for %lu ms (>%lu ms limit)\n", timeSinceWiFi, WIFI_TIMEOUT);
    Serial.println("💡 TIP: Check WiFi connection and wait for reconnection");
    
    digitalWrite(relayPin, LOW);
    
    // WiFi timeout alert (3 short beeps)
    for (int i = 0; i < 3; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(150);
      digitalWrite(buzzerPin, LOW);
      delay(150);
    }
    
    logSecurityEventToFirebase("Engine Start Blocked - WiFi Timeout");
    return;
  }
  
  // ✅ 2. Helmet Security Check (5-second timeout)
  if (!helmetOK && lastHelmetUpdateTime > 0) {
    Serial.println("\n❌ ENGINE START BLOCKED - HELMET TIMEOUT!");
    Serial.printf("💡 Helmet disconnected for %lu ms (>%lu ms limit)\n", timeSinceHelmet, HELMET_TIMEOUT);
    Serial.printf("💡 DEBUG: currentTime=%lu, lastHelmetUpdateTime=%lu\n", currentTime, lastHelmetUpdateTime);
    Serial.printf("💡 DEBUG: helmetConnected=%s, lastHelmetHeartbeat=%llu\n", 
                  helmetConnected ? "true" : "false", lastHelmetHeartbeat);
    Serial.println("💡 TIP: Turn on helmet and wait for connection");
    
    digitalWrite(relayPin, LOW);
    
    // Helmet timeout alert (2 short beeps)
    for (int i = 0; i < 2; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    }
    
    logSecurityEventToFirebase("Engine Start Blocked - Helmet Timeout");
    return;
  }
  
  // ✅ 3. Alcohol Safety Check
  if (!alcoholOK) {
    Serial.println("\n❌ ENGINE START BLOCKED - ALCOHOL DETECTED!");
    Serial.println("💡 TIP: Wait for alcohol levels to clear");
    Serial.println("💡 TIP: Helmet should send 'Safe' status");
    
    if (autoEngineControl) {
      engineStartRequested = true;
    }
    
    digitalWrite(relayPin, LOW);
    
    // Alcohol alert (3 long beeps)
    for (int i = 0; i < 3; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(buzzerPin, LOW);
      delay(200);
    }
    
    logSecurityEventToFirebase("Engine Start Blocked - Alcohol Detected");
    return;
  }
  
  // ✅ 4. Crash Safety Check
  if (!crashOK) {
    Serial.println("\n❌ ENGINE START BLOCKED - CRASH DETECTED!");
    Serial.println("💡 TIP: Clear crash alert first");
    Serial.println("💡 TIP: Ensure vehicle is upright and stable");
    
    digitalWrite(relayPin, LOW);
    
    // Crash alert (4 urgent beeps)
    for (int i = 0; i < 4; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(250);
      digitalWrite(buzzerPin, LOW);
      delay(150);
    }
    
    logSecurityEventToFirebase("Engine Start Blocked - Crash Detected");
    return;
  }

  Serial.println("\n✅ Starting engine...");
  
  // ✅ FIX: HIGH = ON for ACTIVE-HIGH relay
  digitalWrite(relayPin, HIGH);
  engineRunning = true;
  engineStartRequested = false;
  
  Serial.printf("✅ Relay GPIO %d = %d (ON)\n", relayPin, digitalRead(relayPin));
  
  // ✅ NEW: Start trip tracking
  startTrip();
  
  sendLiveToFirebase();
  
  if (antiTheftArmed) {
    antiTheftArmed = false;
    antiTheftEnabled = false;
    theftDetectionCount = 0;
    Serial.println("[ANTI-THEFT] 🔓 Disarmed");
  }
}

void stopEngine() {
  Serial.println("\n🛑 Stopping engine...");
  
  // ✅ NEW: End trip tracking before stopping engine
  endTrip();
  
  // ✅ FIX: LOW = OFF for ACTIVE-HIGH relay
  digitalWrite(relayPin, LOW);
  engineRunning = false;
  
  Serial.printf("🛑 Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
  
  sendLiveToFirebase();
  
  engineOffTime = millis();
  antiTheftEnabled = true;
  antiTheftArmed = false;
  Serial.println("[ANTI-THEFT] 🛡️ Will arm in 10 seconds...");
}

void sendCrashToFirebase(float impact, float roll) {
  if (WiFi.status() != WL_CONNECTED) return;

  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = timestamp;
  doc["impactStrength"] = impact;
  doc["roll"] = roll;
  doc["leanAngle"] = abs(roll);
  doc["hasGPS"] = gps.location.isValid();
  doc["type"] = "crash";
  doc["severity"] = impact > 20.0 ? "severe" : "moderate";
  
  if (gps.location.isValid()) {
    doc["lat"] = gps.location.lat();
    doc["lng"] = gps.location.lng();
    doc["locationLat"] = gps.location.lat();
    doc["locationLng"] = gps.location.lng();
  }
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.POST(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.println("[FIREBASE] ✓ Crash event logged");
  } else {
    Serial.printf("[FIREBASE] ✗ Crash event failed: %d\n", code);
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
  // ✅ FIX: HIGH = ON, LOW = OFF for ACTIVE-HIGH relay
  doc["relayStatus"] = digitalRead(relayPin) ? "ON" : "OFF";
  
  // ✅ NEW: Add GPS speed and location
  doc["speed"] = currentSpeed;
  if (gps.location.isValid()) {
    doc["locationLat"] = gps.location.lat();
    doc["locationLng"] = gps.location.lng();
    doc["gpsValid"] = true;
  } else {
    doc["gpsValid"] = false;
  }
  
  doc["timestamp"] = millis();
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.PUT(payload);
  http.end();
  
  // Also update engineControl path
  StaticJsonDocument<128> engineDoc;
  engineDoc["engineRunning"] = engineRunning;
  engineDoc["autoMode"] = autoEngineControl;
  engineDoc["timestamp"] = millis();
  
  String enginePayload;
  serializeJson(engineDoc, enginePayload);
  
  HTTPClient http2;
  String enginePath = "/" + userUID + "/engineControl.json?auth=" + firebaseAuth;
  http2.begin(firebaseHost + enginePath);
  http2.addHeader("Content-Type", "application/json");
  http2.PATCH(enginePayload);
  http2.end();
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
    lastWiFiConnection = millis();  // ✅ NEW: Track connection time
    lastFirebaseSuccess = millis(); // ✅ NEW: Reset Firebase timer on WiFi connect
  } else {
    Serial.println("\n[WIFI] ❌ Failed!");
  }
}

void checkAlcoholStatus() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.setTimeout(2000);
  http.begin(firebaseHost + alcoholPath);
  
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    
    // ✅ FIX: If no data or null, assume Safe
    if (response == "null" || response.length() <= 2) {
      if (alcoholDetected) {
        Serial.println("\n✅ Alcohol cleared (no data)");
        alcoholDetected = false;
        lastAlcoholState = false;
      }
      http.end();
      return;
    }

    bool currentAlcoholState = false;
    int sensorValue = 0;
    
    // Parse sensor value first
    if (response.indexOf("sensorValue") != -1) {
      int sensorStart = response.indexOf("sensorValue") + 13;
      int sensorEnd = response.indexOf(",", sensorStart);
      if (sensorEnd == -1) sensorEnd = response.indexOf("}", sensorStart);
      
      if (sensorEnd > sensorStart) {
        String sensorStr = response.substring(sensorStart, sensorEnd);
        sensorValue = sensorStr.toInt();
      }
    }
    
    // ✅ FIX: Trust helmet's status first, use sensor value as backup
    // Check if helmet explicitly sent a status
    bool hasHelmetStatus = false;
    
    if (response.indexOf("\"status\":\"Safe\"") != -1 || response.indexOf("\"status\":\"safe\"") != -1) {
      currentAlcoholState = false;
      hasHelmetStatus = true;
      Serial.printf("\n✅ Alcohol CLEARED! (Helmet says: Safe, Value: %d)\n", sensorValue);
    } else if (response.indexOf("\"status\":\"Danger\"") != -1 || response.indexOf("\"status\":\"danger\"") != -1) {
      currentAlcoholState = true;
      hasHelmetStatus = true;
      Serial.printf("\n🚨 ALCOHOL DETECTED! (Helmet says: Danger, Value: %d)\n", sensorValue);
    }
    
    // Fallback: If no status from helmet, use sensor value with helmet's threshold (600)
    if (!hasHelmetStatus) {
      const int HELMET_THRESHOLD = 600;  // Match helmet's threshold
      
      if (sensorValue > HELMET_THRESHOLD) {
        currentAlcoholState = true;
        Serial.printf("\n🚨 ALCOHOL DETECTED! (Value: %d > %d)\n", sensorValue, HELMET_THRESHOLD);
      } else {
        currentAlcoholState = false;
        Serial.printf("[ALCOHOL] Safe (Value: %d <= %d)\n", sensorValue, HELMET_THRESHOLD);
      }
    }

    // Update state
    if (currentAlcoholState != alcoholDetected) {
      alcoholDetected = currentAlcoholState;
      lastAlcoholState = currentAlcoholState;
      
      if (currentAlcoholState) {
        // ✅ FIX: LOW = OFF for ACTIVE-HIGH relay
        digitalWrite(relayPin, LOW);
        
        if (engineRunning) {
          triggerAlcoholShutdown();
        }
      }
    }
    
    // ✅ FAST DEBUG: Alcohol status every 1 second
    static unsigned long lastDebug = 0;
    if (millis() - lastDebug > 1000) {
      Serial.printf("[ALCOHOL] Status: %s | Value: %d | Threshold: 600\n", 
                    alcoholDetected ? "DANGER" : "SAFE", 
                    sensorValue);
      lastDebug = millis();
    }
  } else {
    Serial.printf("[ALCOHOL] ✗ Firebase read failed: HTTP %d\n", httpCode);
  }
  
  http.end();
}

void triggerAlcoholShutdown() {
  Serial.println("\n🚨 ALCOHOL - EMERGENCY SHUTDOWN!");
  
  // ✅ FIX: LOW = OFF for ACTIVE-HIGH relay
  digitalWrite(relayPin, LOW);
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
  Serial.printf("Helmet: %s\n", helmetConnected ? "CONNECTED ✅" : "DISCONNECTED ❌");
  Serial.printf("Relay: GPIO %d = %d (%s)\n", relayPin, digitalRead(relayPin), 
                digitalRead(relayPin) ? "ON" : "OFF");
  Serial.printf("Engine: %s\n", engineRunning ? "RUNNING" : "STOPPED");
  Serial.printf("Crash: %s\n", crashDetected ? "YES" : "NO");
  Serial.printf("Alcohol: %s\n", alcoholDetected ? "YES" : "NO");
  Serial.printf("Auto Control: %s\n", autoEngineControl ? "ON" : "OFF");
  Serial.printf("Anti-Theft: %s\n", antiTheftArmed ? "ARMED" : "DISARMED");
  Serial.printf("WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED" : "DISCONNECTED");
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
  http.setTimeout(1000);  // ✅ FIX: 1 second timeout for fast response
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
  
  // Check autoMode setting
  HTTPClient http2;
  http2.setTimeout(1000);  // ✅ FIX: 1 second timeout
  String autoModePath = "/" + userUID + "/engineControl/autoMode.json?auth=" + firebaseAuth;
  http2.begin(firebaseHost + autoModePath);
  
  int autoCode = http2.GET();
  
  if (autoCode == HTTP_CODE_OK) {
    String autoResponse = http2.getString();
    bool dashboardAutoMode = (autoResponse.indexOf("true") != -1);
    
    if (dashboardAutoMode != autoEngineControl) {
      autoEngineControl = dashboardAutoMode;
      Serial.printf("[ENGINE] Auto control updated: %s\n", 
                    autoEngineControl ? "ON" : "OFF");
    }
  }
  
  http2.end();
}

void clearDashboardButton() {
  HTTPClient http;
  http.setTimeout(1000);  // ✅ FIX: 1 second timeout
  http.begin(firebaseHost + buttonPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT("false");
  
  if (code == HTTP_CODE_OK) {
    Serial.println("[DASHBOARD] ✓ Button cleared (ready for next press)");
    // ✅ FIX: Reset the state so button can be pressed again
    lastDashboardButtonState = false;
  } else {
    Serial.printf("[DASHBOARD] ✗ Button clear failed: %d\n", code);
  }
  
  http.end();
}

// ✅ MODIFIED: Monitor physical key switch with security override
void checkPhysicalKey() {
  bool currentKeyState = digitalRead(physicalKeyPin);
  
  // Detect key state change (with debounce)
  if (currentKeyState != lastKeyState) {
    delay(50);  // Debounce
    currentKeyState = digitalRead(physicalKeyPin);
    
    if (currentKeyState != lastKeyState) {
      lastKeyState = currentKeyState;
      
      // ✅ SECURITY: Check if system is in secure mode
      if (systemInSecureMode) {
        Serial.println("\n🚨 PHYSICAL KEY BLOCKED - SYSTEM IN SECURE MODE!");
        Serial.println("💡 Network/Firebase connection required for engine control");
        
        // Force relay OFF regardless of key position
        digitalWrite(relayPin, LOW);
        engineRunning = false;
        
        // Alert beeps
        for (int i = 0; i < 5; i++) {
          digitalWrite(buzzerPin, HIGH);
          delay(100);
          digitalWrite(buzzerPin, LOW);
          delay(100);
        }
        return;
      }
      
      // ✅ OPTION 1: If using voltage divider (12V key → 3.3V GPIO)
      // Key turned ON (HIGH when key provides voltage)
      if (currentKeyState == HIGH && !engineRunning) {
        Serial.println("\n🔑 PHYSICAL KEY TURNED ON!");
        startEngine();
      }
      // Key turned OFF (LOW when no voltage)
      else if (currentKeyState == LOW && engineRunning) {
        Serial.println("\n🔑 PHYSICAL KEY TURNED OFF!");
        stopEngine();
      }
      
      // ✅ OPTION 2: If using switch to GND (uncomment below, comment above)
      // Key turned ON (LOW because of INPUT_PULLUP)
      // if (currentKeyState == LOW && !engineRunning) {
      //   Serial.println("\n🔑 PHYSICAL KEY TURNED ON!");
      //   startEngine();
      // }
      // // Key turned OFF
      // else if (currentKeyState == HIGH && engineRunning) {
      //   Serial.println("\n🔑 PHYSICAL KEY TURNED OFF!");
      //   stopEngine();
      // }
    }
  }
}

// ✅ ENHANCED: Comprehensive security system with 5-second timeouts
void checkComprehensiveSecurity() {
  if (!securitySystemActive) return;
  
  unsigned long currentTime = millis();
  
  // ✅ 1. WiFi Connection Monitoring (5-second timeout)
  if (WiFi.status() == WL_CONNECTED) {
    lastWiFiConnected = currentTime;
  }
  unsigned long timeSinceWiFi = currentTime - lastWiFiConnected;
  bool wifiTimeout = (timeSinceWiFi > WIFI_TIMEOUT);
  
  // ✅ 2. Helmet Connection Monitoring (5-second timeout)  
  unsigned long timeSinceHelmet = currentTime - lastHelmetUpdateTime;
  bool helmetTimeout = (timeSinceHelmet > HELMET_TIMEOUT && lastHelmetUpdateTime > 0 && !helmetConnected);
  
  // ✅ 3. Security Violations Detection
  bool securityViolation = wifiTimeout || helmetTimeout;
  
  // ✅ 4. Auto-Shutdown Logic
  if (securityViolation && engineRunning) {
    Serial.println("\n🚨🚨🚨 SECURITY VIOLATION - AUTO SHUTDOWN! 🚨🚨🚨");
    
    if (wifiTimeout) {
      Serial.printf("📡 WiFi TIMEOUT: %lu ms (>%lu ms limit)\n", timeSinceWiFi, WIFI_TIMEOUT);
      Serial.println("🔒 Thieves may have disconnected WiFi!");
    }
    
    if (helmetTimeout) {
      Serial.printf("🪖 HELMET TIMEOUT: %lu ms (>%lu ms limit)\n", timeSinceHelmet, HELMET_TIMEOUT);
      Serial.println("🔒 Helmet turned off or disconnected!");
    }
    
    // Force engine shutdown
    triggerSecurityShutdown("Security Violation");
  }
  
  // ✅ 5. Physical Key Security Override
  if (securityViolation) {
    systemInSecureMode = true;
  } else {
    systemInSecureMode = false;
  }
  
  // ✅ 6. Debug Output (every 3 seconds)
  static unsigned long lastSecurityDebug = 0;
  if (currentTime - lastSecurityDebug > 3000) {
    Serial.printf("[SECURITY] WiFi: %s (%lu ms) | Helmet: %s (%lu ms) | Engine: %s\n",
                  wifiTimeout ? "TIMEOUT ❌" : "OK ✅", timeSinceWiFi,
                  helmetTimeout ? "TIMEOUT ❌" : "OK ✅", timeSinceHelmet,
                  engineRunning ? "RUNNING" : "STOPPED");
    lastSecurityDebug = currentTime;
  }
}

// ✅ NEW: Security shutdown with detailed logging
void triggerSecurityShutdown(String reason) {
  Serial.println("\n🚨 SECURITY SHUTDOWN INITIATED 🚨");
  Serial.println("Reason: " + reason);
  Serial.println("Time: " + String(millis()) + " ms");
  
  // Force relay OFF
  digitalWrite(relayPin, LOW);
  engineRunning = false;
  
  // End current trip
  endTrip();
  
  Serial.printf("🛑 Engine STOPPED | Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
  
  // Security alert sequence (10 loud beeps)
  for (int i = 0; i < 10; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(150);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(lightIndicatorPin, LOW);
    delay(150);
  }
  
  // Log security event to Firebase
  logSecurityEventToFirebase(reason);
}

// ✅ NEW: Enhanced physical key with security checks
void checkPhysicalKeyWithSecurity() {
  bool currentKeyState = digitalRead(physicalKeyPin);
  
  // Detect key state change (with debounce)
  if (currentKeyState != lastKeyState) {
    delay(50);  // Debounce
    currentKeyState = digitalRead(physicalKeyPin);
    
    if (currentKeyState != lastKeyState) {
      lastKeyState = currentKeyState;
      
      // ✅ SECURITY: Check comprehensive security before allowing key operation
      unsigned long currentTime = millis();
      unsigned long timeSinceWiFi = currentTime - lastWiFiConnected;
      unsigned long timeSinceHelmet = currentTime - lastHelmetUpdateTime;
      
      bool wifiOK = (timeSinceWiFi <= WIFI_TIMEOUT);
      bool helmetOK = (timeSinceHelmet <= HELMET_TIMEOUT || lastHelmetUpdateTime == 0 || helmetConnected);
      bool securityOK = wifiOK && helmetOK;
      
      if (!securityOK) {
        Serial.println("\n🚨 PHYSICAL KEY BLOCKED - SECURITY VIOLATION!");
        Serial.printf("WiFi Status: %s (%lu ms ago)\n", wifiOK ? "OK" : "TIMEOUT", timeSinceWiFi);
        Serial.printf("Helmet Status: %s (%lu ms ago)\n", helmetOK ? "OK" : "TIMEOUT", timeSinceHelmet);
        Serial.println("💡 Both devices must be online for physical key to work");
        
        // Force relay OFF regardless of key position
        digitalWrite(relayPin, LOW);
        engineRunning = false;
        
        // Security alert beeps
        for (int i = 0; i < 5; i++) {
          digitalWrite(buzzerPin, HIGH);
          delay(200);
          digitalWrite(buzzerPin, LOW);
          delay(200);
        }
        
        // Log attempted theft
        logSecurityEventToFirebase("Physical Key Blocked - Security Violation");
        return;
      }
      
      // ✅ Security OK - Allow normal key operation
      if (currentKeyState == HIGH && !engineRunning) {
        Serial.println("\n🔑 PHYSICAL KEY TURNED ON! (Security verified ✅)");
        startEngine();
      }
      else if (currentKeyState == LOW && engineRunning) {
        Serial.println("\n🔑 PHYSICAL KEY TURNED OFF!");
        stopEngine();
      }
    }
  }
}

// ✅ NEW: Check helmet connection status with timeout detection
void checkHelmetConnection() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.setTimeout(1000);
  String helmetPath = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
  http.begin(helmetPath);
  
  int httpCode = http.GET();
  
  // ✅ DEBUG: Show what we received
  static unsigned long lastDebugRead = 0;
  if (millis() - lastDebugRead > 5000) {
    Serial.printf("[HELMET DEBUG] HTTP Code: %d\n", httpCode);
    lastDebugRead = millis();
  }
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    
    // ✅ DEBUG: Print response once every 5 seconds
    static unsigned long lastResponseDebug = 0;
    if (millis() - lastResponseDebug > 5000) {
      Serial.println("[HELMET DEBUG] Response: " + response);
      lastResponseDebug = millis();
    }
    
    // ✅ FIX: Check status first
    bool statusIsOn = (response.indexOf("\"status\":\"On\"") != -1);
    
    if (!statusIsOn) {
      // Status is explicitly "Off" - helmet turned off
      if (helmetConnected) {
        Serial.println("\n[HELMET] ⚠️ Disconnected (status: Off)");
      }
      helmetConnected = false;
      lastHelmetHeartbeat = 0;
      lastHelmetUpdateTime = 0;
      helmetStatusForcedOff = false;
      http.end();
      return;
    }
    
    // Status is "On" - now check if heartbeat is updating
    int heartbeatStart = response.indexOf("\"lastHeartbeat\":") + 16;
    int heartbeatEnd = response.indexOf(",", heartbeatStart);
    if (heartbeatEnd == -1) heartbeatEnd = response.indexOf("}", heartbeatStart);
    
    if (heartbeatEnd > heartbeatStart) {
      String heartbeatStr = response.substring(heartbeatStart, heartbeatEnd);
      uint64_t helmetHeartbeat = (uint64_t)heartbeatStr.toDouble();
      
      // Check if heartbeat has changed (helmet is still alive)
      if (helmetHeartbeat != lastHelmetHeartbeat) {
        // Heartbeat updated!
        lastHelmetHeartbeat = helmetHeartbeat;
        lastHelmetUpdateTime = millis();
        helmetStatusForcedOff = false;  // Reset forced off flag
        
        if (!helmetConnected) {
          Serial.println("\n[HELMET] ✅ Connected!");
        }
        helmetConnected = true;
        
        // ✅ DEBUG: Show heartbeat update
        Serial.printf("[HELMET DEBUG] Heartbeat updated! New: %llu, lastHelmetUpdateTime: %lu\n", 
                      helmetHeartbeat, lastHelmetUpdateTime);
      } else {
        // ✅ DEBUG: Show when heartbeat hasn't changed
        unsigned long timeSinceUpdate = millis() - lastHelmetUpdateTime;
        Serial.printf("[HELMET DEBUG] Heartbeat unchanged. Time since update: %lu ms\n", timeSinceUpdate);
        
        // ✅ NEW: After 10 seconds, force status to "Off" in Firebase
        if (timeSinceUpdate > HELMET_FORCE_OFF_TIMEOUT && !helmetStatusForcedOff) {
          Serial.printf("\n[HELMET] 🚨 FORCING STATUS TO OFF (no update for %lu ms)\n", timeSinceUpdate);
          forceHelmetStatusOff();
          helmetStatusForcedOff = true;
          helmetConnected = false;
        }
        // After 3 seconds, mark as disconnected locally
        else if (timeSinceUpdate > HELMET_TIMEOUT) {
          if (helmetConnected) {
            Serial.printf("\n[HELMET] ⚠️ Disconnected (no heartbeat update for %lu ms)\n", timeSinceUpdate);
          }
          helmetConnected = false;
        }
        // else: still within timeout, keep current connection status
      }
    } else {
      // Can't parse heartbeat - assume connected if status is On
      if (!helmetConnected) {
        Serial.println("\n[HELMET] ✅ Connected (no timestamp check)");
      }
      helmetConnected = true;
      lastHelmetUpdateTime = millis();  // ✅ Always update time when helmet is connected
      Serial.printf("[HELMET DEBUG] No heartbeat parsing, but status On. Updated lastHelmetUpdateTime: %lu\n", lastHelmetUpdateTime);
    }
  } else {
    if (helmetConnected) {
      Serial.printf("\n[HELMET] ⚠️ Disconnected (Firebase read failed: HTTP %d)\n", httpCode);
    }
    helmetConnected = false;
  }
  
  http.end();
  
  // ✅ FAST DEBUG: Helmet status every 1 second
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 1000) {
    Serial.printf("[HELMET] Status: %s\n", helmetConnected ? "CONNECTED ✅" : "DISCONNECTED ❌");
    lastDebug = millis();
  }
}

// ✅ NEW: Force helmet status to "Off" in Firebase
void forceHelmetStatusOff() {
  if (WiFi.status() != WL_CONNECTED) return;

  Serial.println("[HELMET] 📝 Updating Firebase status to 'Off'...");
  
  StaticJsonDocument<128> doc;
  doc["status"] = "Off";
  doc["lastHeartbeat"] = lastHelmetHeartbeat;  // Keep last known heartbeat
  doc["timestamp"] = lastHelmetHeartbeat;
  doc["forcedOffByMotorcycle"] = true;  // Flag to indicate this was forced
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String helmetPath = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
  http.begin(helmetPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.println("[HELMET] ✅ Status forced to 'Off' in Firebase");
  } else {
    Serial.printf("[HELMET] ❌ Failed to update status: HTTP %d\n", code);
  }
  
  http.end();
}
// ✅ NEW: Trip tracking functions
void startTrip() {
  Serial.println("\n🗺️ STARTING TRIP TRACKING");
  
  // Initialize trip data
  currentTrip.isActive = true;
  currentTrip.hasValidStart = false;
  currentTrip.startTime = 1700000000000ULL + (uint64_t)millis();
  currentTrip.maxSpeed = 0.0;
  currentTrip.totalDistance = 0.0;
  currentTrip.fromLat = 0.0;
  currentTrip.fromLng = 0.0;
  currentTrip.toLat = 0.0;
  currentTrip.toLng = 0.0;
  
  Serial.println("[TRIP] 📍 Waiting for first GPS fix...");
}

void updateTripData() {
  if (!currentTrip.isActive || !engineRunning) return;
  
  // Check if GPS has valid location
  if (gps.location.isValid()) {
    double currentLat = gps.location.lat();
    double currentLng = gps.location.lng();
    
    // Set "From" coordinates on first valid GPS reading
    if (!currentTrip.hasValidStart) {
      currentTrip.fromLat = currentLat;
      currentTrip.fromLng = currentLng;
      currentTrip.hasValidStart = true;
      
      Serial.printf("[TRIP] 🚀 FROM: %.6f, %.6f\n", currentLat, currentLng);
      Serial.println("[TRIP] 📍 Trip origin set!");
    }
    
    // Always update "To" coordinates with latest valid position
    currentTrip.toLat = currentLat;
    currentTrip.toLng = currentLng;
    
    // Calculate distance if we have a previous position
    if (lastValidLat != 0.0 && lastValidLng != 0.0) {
      float distance = calculateDistance(lastValidLat, lastValidLng, currentLat, currentLng);
      currentTrip.totalDistance += distance;
    }
    
    // Update last valid position
    lastValidLat = currentLat;
    lastValidLng = currentLng;
    
    // Track maximum speed
    if (gps.speed.isValid()) {
      float speed = gps.speed.kmph();
      if (speed > currentTrip.maxSpeed) {
        currentTrip.maxSpeed = speed;
      }
    }
    
    // Debug output every 10 seconds
    static unsigned long lastTripDebug = 0;
    if (millis() - lastTripDebug > 10000) {
      Serial.printf("[TRIP] Current: %.6f, %.6f | Distance: %.2f km | Max Speed: %.1f kph\n",
                    currentLat, currentLng, currentTrip.totalDistance, currentTrip.maxSpeed);
      lastTripDebug = millis();
    }
  }
}

void endTrip() {
  if (!currentTrip.isActive) return;
  
  Serial.println("\n🏁 ENDING TRIP TRACKING");
  
  currentTrip.endTime = 1700000000000ULL + (uint64_t)millis();
  currentTrip.isActive = false;
  
  // Only save trip if we have valid start and end coordinates
  if (currentTrip.hasValidStart && currentTrip.toLat != 0.0 && currentTrip.toLng != 0.0) {
    Serial.printf("[TRIP] 🚀 FROM: %.6f, %.6f\n", currentTrip.fromLat, currentTrip.fromLng);
    Serial.printf("[TRIP] 🏁 TO: %.6f, %.6f\n", currentTrip.toLat, currentTrip.toLng);
    Serial.printf("[TRIP] 📊 Distance: %.2f km | Max Speed: %.1f kph\n", 
                  currentTrip.totalDistance, currentTrip.maxSpeed);
    
    // Calculate trip duration
    uint64_t duration = currentTrip.endTime - currentTrip.startTime;
    Serial.printf("[TRIP] ⏱️ Duration: %llu minutes\n", duration / 60000);
    
    // Save trip to Firebase
    saveTripToFirebase();
  } else {
    Serial.println("[TRIP] ⚠️ No valid GPS data - trip not saved");
  }
  
  // Reset for next trip
  lastValidLat = 0.0;
  lastValidLng = 0.0;
}

void saveTripToFirebase() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  Serial.println("[TRIP] 💾 Saving trip to Firebase...");
  
  StaticJsonDocument<512> doc;
  doc["startTime"] = currentTrip.startTime;
  doc["endTime"] = currentTrip.endTime;
  doc["duration"] = currentTrip.endTime - currentTrip.startTime;
  
  // From coordinates
  doc["from"]["lat"] = currentTrip.fromLat;
  doc["from"]["lng"] = currentTrip.fromLng;
  doc["from"]["address"] = "GPS Location"; // Could be enhanced with reverse geocoding
  
  // To coordinates  
  doc["to"]["lat"] = currentTrip.toLat;
  doc["to"]["lng"] = currentTrip.toLng;
  doc["to"]["address"] = "GPS Location"; // Could be enhanced with reverse geocoding
  
  // Trip statistics
  doc["distance"] = currentTrip.totalDistance;
  doc["maxSpeed"] = currentTrip.maxSpeed;
  doc["avgSpeed"] = (currentTrip.totalDistance / ((currentTrip.endTime - currentTrip.startTime) / 3600000.0)); // km/h
  
  // Additional metadata
  doc["vehicleType"] = "motorcycle";
  doc["userId"] = userUID;
  doc["tripId"] = String(currentTrip.startTime); // Use start time as unique ID
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String tripPath = "/helmet_public/" + userUID + "/trips.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + tripPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.POST(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.println("[TRIP] ✅ Trip saved to Firebase!");
  } else {
    Serial.printf("[TRIP] ❌ Failed to save trip: HTTP %d\n", code);
  }
  
  http.end();
}

// Calculate distance between two GPS coordinates (Haversine formula)
float calculateDistance(double lat1, double lng1, double lat2, double lng2) {
  const float R = 6371.0; // Earth's radius in kilometers
  
  float dLat = radians(lat2 - lat1);
  float dLng = radians(lng2 - lng1);
  
  float a = sin(dLat/2) * sin(dLat/2) + 
            cos(radians(lat1)) * cos(radians(lat2)) * 
            sin(dLng/2) * sin(dLng/2);
  
  float c = 2 * atan2(sqrt(a), sqrt(1-a));
  
  return R * c; // Distance in kilometers
}

// ✅ NEW: Log security events to Firebase
void logSecurityEventToFirebase(String eventType) {
  if (WiFi.status() != WL_CONNECTED) return;
  
  Serial.println("[SECURITY] 📝 Logging security event: " + eventType);
  
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = timestamp;
  doc["type"] = "security_event";
  doc["event"] = eventType;
  doc["engineRunning"] = engineRunning;
  doc["wifiConnected"] = (WiFi.status() == WL_CONNECTED);
  doc["helmetConnected"] = helmetConnected;
  doc["alcoholDetected"] = alcoholDetected;
  doc["crashDetected"] = crashDetected;
  
  // Add location if available
  if (gps.location.isValid()) {
    doc["lat"] = gps.location.lat();
    doc["lng"] = gps.location.lng();
    doc["hasGPS"] = true;
  } else {
    doc["hasGPS"] = false;
  }
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String securityPath = "/helmet_public/" + userUID + "/security_events.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + securityPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.POST(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.println("[SECURITY] ✅ Security event logged to Firebase");
  } else {
    Serial.printf("[SECURITY] ❌ Failed to log security event: HTTP %d\n", code);
  }
  
  http.end();
}

// ✅ NEW: Print comprehensive security status
void printSecurityStatus() {
  unsigned long currentTime = millis();
  unsigned long timeSinceWiFi = currentTime - lastWiFiConnected;
  unsigned long timeSinceHelmet = currentTime - lastHelmetUpdateTime;
  
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🔒 COMPREHENSIVE SECURITY STATUS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // Security System Status
  Serial.printf("Security System: %s\n", securitySystemActive ? "ACTIVE 🔒" : "DISABLED ⚠️");
  Serial.printf("Secure Mode: %s\n", systemInSecureMode ? "ENGAGED 🚨" : "NORMAL ✅");
  
  Serial.println("\n📡 CONNECTIVITY STATUS:");
  Serial.printf("WiFi: %s (%lu ms ago)\n", 
                (timeSinceWiFi <= WIFI_TIMEOUT) ? "CONNECTED ✅" : "TIMEOUT ❌", 
                timeSinceWiFi);
  Serial.printf("WiFi Timeout Limit: %lu ms\n", WIFI_TIMEOUT);
  
  Serial.printf("Helmet: %s (%lu ms ago)\n", 
                (timeSinceHelmet <= HELMET_TIMEOUT || lastHelmetUpdateTime == 0) ? "CONNECTED ✅" : "TIMEOUT ❌", 
                timeSinceHelmet);
  Serial.printf("Helmet Timeout Limit: %lu ms\n", HELMET_TIMEOUT);
  
  Serial.println("\n🛡️ SAFETY STATUS:");
  Serial.printf("Engine: %s\n", engineRunning ? "RUNNING 🟢" : "STOPPED 🔴");
  Serial.printf("Alcohol: %s\n", alcoholDetected ? "DETECTED ❌" : "SAFE ✅");
  Serial.printf("Crash: %s\n", crashDetected ? "DETECTED ❌" : "NONE ✅");
  Serial.printf("Anti-Theft: %s\n", antiTheftArmed ? "ARMED 🛡️" : "DISARMED 🔓");
  
  Serial.println("\n🔑 PHYSICAL KEY STATUS:");
  Serial.printf("Key Position: %s\n", digitalRead(physicalKeyPin) ? "ON" : "OFF");
  Serial.printf("Key Security: %s\n", securitySystemActive ? "PROTECTED 🔒" : "UNPROTECTED ⚠️");
  
  Serial.println("\n⚡ RELAY STATUS:");
  Serial.printf("Relay GPIO %d: %d (%s)\n", relayPin, digitalRead(relayPin), 
                digitalRead(relayPin) ? "ON" : "OFF");
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // Security Recommendations
  if (!securitySystemActive) {
    Serial.println("⚠️  WARNING: Security system is DISABLED!");
    Serial.println("💡 Recommendation: Enable with 'SECURITY ON' command");
  }
  
  if (timeSinceWiFi > WIFI_TIMEOUT) {
    Serial.println("⚠️  WARNING: WiFi connection timeout!");
    Serial.println("💡 Recommendation: Check WiFi network and reconnect");
  }
  
  if (timeSinceHelmet > HELMET_TIMEOUT && lastHelmetUpdateTime > 0) {
    Serial.println("⚠️  WARNING: Helmet connection timeout!");
    Serial.println("💡 Recommendation: Turn on helmet and ensure WiFi connection");
  }
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}