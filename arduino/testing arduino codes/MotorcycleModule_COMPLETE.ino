#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>

// ═══════════════════════════════════════════════════════════════════════════
// VigiLERT Motorcycle Module - COMPLETE FUNCTIONAL VERSION
// ═══════════════════════════════════════════════════════════════════════════
// ✅ Fully integrated with Dashboard.vue
// ✅ All security features: Dual relay, WiFi watchdog, Anti-theft
// ✅ Engine authorization system (like SMS command)
// ✅ Connection stability with graceful degradation
// ✅ Physical key blocking via ignition relay
// ═══════════════════════════════════════════════════════════════════════════

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Configuration
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

// Owner phone numbers (like reference code)
const String authorizedNumber1 = "+639675715673";  // Primary authorized number
const String authorizedNumber2 = "+639542583234";  // Secondary authorized number  
const String smsPassword = "1234";                 // Password for SMS commands
String lastSMSSender = "";                         // Store sender for replies

// Pin Assignments - DUAL RELAY SYSTEM
const int relayPin = 13;              // Starter relay (temporary, 3-second max)
const int ignitionRelayPin = 27;      // Ignition relay (continuous power control)
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

// ✅ SIMPLE SECURITY: Like working reference code
// Engine is OFF by default, only turns ON when explicitly commanded
bool engineRunning = false;
bool engineAllowed = false;  // Simple flag - no complex authorization needed
bool crashDetected = false;
const float ACCEL_THRESHOLD = 15.0;

// Current sensor readings
float currentRoll = 0.0;
float currentTotalAccel = 0.0;
float currentSpeed = 0.0;

// GPS and trip tracking
unsigned long lastGPSUpdate = 0;
const unsigned long GPS_UPDATE_INTERVAL = 500;

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

// Crash detection
unsigned long lastCrashTime = 0;
const unsigned long CRASH_COOLDOWN = 3000;

// Alcohol detection
bool alcoholDetected = false;
unsigned long lastAlcoholCheck = 0;
const unsigned long ALCOHOL_CHECK_INTERVAL = 250;

// ✅ CONNECTION STABILITY: Graceful degradation system
struct ConnectionHealth {
  bool isConnected = false;
  unsigned long lastSuccessTime = 0;
  unsigned long lastCheckTime = 0;
  int consecutiveFailures = 0;
  int consecutiveSuccesses = 0;
  bool isStable = false;
  unsigned long stableStartTime = 0;
};

ConnectionHealth wifiHealth;
ConnectionHealth helmetHealth;

// Graceful degradation timeouts
const unsigned long WIFI_WARNING_TIMEOUT = 10000;    // 10 seconds warning
const unsigned long WIFI_CRITICAL_TIMEOUT = 30000;   // 30 seconds critical
const unsigned long WIFI_SHUTDOWN_TIMEOUT = 60000;   // 60 seconds shutdown

const unsigned long HELMET_WARNING_TIMEOUT = 15000;  // 15 seconds warning  
const unsigned long HELMET_CRITICAL_TIMEOUT = 45000; // 45 seconds critical
const unsigned long HELMET_SHUTDOWN_TIMEOUT = 90000; // 90 seconds shutdown

// Stability requirements
const int STABILITY_REQUIRED_SUCCESSES = 3;
const unsigned long STABILITY_DURATION = 10000;
const unsigned long CONNECTION_CHECK_INTERVAL = 2000;

// Hysteresis protection
bool engineStoppedByWifi = false;
bool engineStoppedByHelmet = false;
unsigned long lastEngineStopTime = 0;
const unsigned long ENGINE_RESTART_COOLDOWN = 30000;

// MPU6050 control
const bool MPU6050_ENABLED = true;  // Set to false if not connected

// Legacy compatibility
const unsigned long HELMET_TIMEOUT = 5000;
const unsigned long HELMET_FORCE_OFF_TIMEOUT = 10000;
const unsigned long WIFI_TIMEOUT = 5000;
const unsigned long HELMET_CHECK_INTERVAL = 2000;

bool helmetConnected = false;
unsigned long lastHelmetUpdateTime = 0;
uint64_t lastHelmetHeartbeat = 0;
bool helmetStatusForcedOff = false;
unsigned long lastWiFiConnected = 0;
unsigned long lastHelmetCheck = 0;

// ✅ WIFI WATCHDOG: Theft prevention system
bool wifiWatchdogActive = false;
unsigned long wifiDisconnectedTime = 0;
const unsigned long WIFI_WATCHDOG_TIMEOUT = 3000;  // 3 seconds
bool emergencyShutdownTriggered = false;

// Startup WiFi check
unsigned long deviceBootTime = 0;
const unsigned long STARTUP_WIFI_TIMEOUT = 6000;  // 6 seconds
bool startupWiFiCheckComplete = false;
bool startupShutdownTriggered = false;

// Starter safety
unsigned long starterStartTime = 0;
unsigned long lastStarterAttempt = 0;
bool starterActive = false;
const unsigned long STARTER_TIMEOUT = 3000;
const unsigned long STARTER_COOLDOWN = 10000;

// Security system
bool securitySystemActive = true;
unsigned long lastSecurityCheck = 0;
const unsigned long SECURITY_CHECK_INTERVAL = 500;

// Engine control
bool autoEngineControl = false;
bool engineStartRequested = false;

// Dashboard integration
bool lastDashboardButtonState = false;
unsigned long lastButtonCheck = 0;
const unsigned long BUTTON_CHECK_INTERVAL = 100;

// Device heartbeat
unsigned long lastHeartbeat = 0;
const unsigned long HEARTBEAT_INTERVAL = 1000;

// Network security
unsigned long lastWiFiConnection = 0;
unsigned long lastFirebaseSuccess = 0;
const unsigned long NETWORK_TIMEOUT = 30000;
const unsigned long FIREBASE_TIMEOUT = 60000;
bool networkSecurityEnabled = true;
bool systemInSecureMode = false;

// Real-time monitoring
unsigned long lastRealtimeUpdate = 0;
const unsigned long REALTIME_INTERVAL = 250;
unsigned long lastSensorPrint = 0;
const unsigned long SENSOR_PRINT_INTERVAL = 500;
unsigned long lastQuickStatus = 0;
const unsigned long QUICK_STATUS_INTERVAL = 1000;

// ✅ ANTI-THEFT: Ultra-sensitive vibration detection
bool antiTheftEnabled = false;
bool antiTheftArmed = false;
unsigned long engineOffTime = 0;
const unsigned long ARM_DELAY = 10000;  // 10 seconds to arm
unsigned long lastTheftAlert = 0;
const unsigned long THEFT_ALERT_COOLDOWN = 60000;  // 1 minute cooldown
bool theftAlertSent = false;
int theftDetectionCount = 0;
const int THEFT_DETECTION_REQUIRED = 1;  // Immediate alert
unsigned long lastVibrationTime = 0;
const unsigned long VIBRATION_DEBOUNCE = 50;  // 50ms ultra-sensitive
int consecutiveVibrations = 0;

// GSM status
bool gsmReady = false;

// Firebase paths
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
const String buttonPath = "/" + userUID + "/engineControl/startButton.json?auth=" + firebaseAuth;
const String livePath = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;
const String alcoholPath = "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;

// Function declarations
void updateConnectionHealth(ConnectionHealth &health, bool currentStatus, const char* connectionName);
String getConnectionStatus(ConnectionHealth &health, unsigned long warningTimeout, unsigned long criticalTimeout, unsigned long shutdownTimeout);
bool shouldShutdownForConnection(ConnectionHealth &health, unsigned long shutdownTimeout);
void checkConnectionStability();
void triggerGracefulShutdown(String reason);
void triggerSecurityShutdown(String reason);
void checkWiFiWatchdog();
void checkHelmetConnection();
void forceHelmetStatusOff();
void startTrip();
void updateTripData();
void endTrip();
void saveTripToFirebase();
float calculateDistance(double lat1, double lng1, double lat2, double lng2);
void logSecurityEventToFirebase(String eventType);
void printSecurityStatus();
void processSMSCommands();
void handleIncomingCall(String response);
void handleIncomingSMS(String response);
bool validateSMSPassword(String message);
void processSMSCommand(String command);
void sendSMSReply(String message);
void sendSMSToNumber(String phoneNumber, String message);
void checkSignalStrength();

void setup() {
  Serial.begin(115200);
  delay(100);
  
  deviceBootTime = millis();
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   MOTORCYCLE MODULE - COMPLETE        ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ 🔒 Dual Relay Security System         ║");
  Serial.println("║ 🛡️ WiFi Watchdog Protection           ║");
  Serial.println("║ 🚨 Ultra-Sensitive Anti-Theft         ║");
  Serial.println("║ 📱 Full Dashboard Integration          ║");
  Serial.println("║ ⚡ Engine Authorization Required       ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  // Initialize MPU6050
  if (MPU6050_ENABLED) {
    Wire.begin(21, 22);
    while (!mpu.begin()) {
      Serial.println("[ERROR] Failed to find MPU6050 chip. Retrying in 2s...");
      delay(2000);
    }
    Serial.println("[SUCCESS] MPU6050 initialized.");
    
    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  } else {
    Serial.println("[INFO] MPU6050 bypassed for testing");
  }

  // Initialize GPS
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("[SETUP] GPS Serial started on RX:16, TX:17");

  // Initialize GSM
  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);
  Serial.println("[SETUP] GSM Serial started on RX:4, TX:5");
  initializeGSM();

  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(ignitionRelayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);
  pinMode(vibrationSensorPin, INPUT);

  // ✅ CRITICAL: Initialize relays to match reference code logic
  // Reference code: HIGH = Engine allowed, LOW = Engine blocked
  digitalWrite(relayPin, LOW);           // Starter BLOCKED
  digitalWrite(ignitionRelayPin, LOW);   // Ignition BLOCKED - BLOCKS PHYSICAL KEY
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  
  // ✅ SIMPLE SECURITY: Engine blocked by default (like reference code)
  engineRunning = false;
  engineAllowed = false;
  
  delay(100);
  Serial.println("\n[SETUP] ═══════════════════════════════════");
  Serial.println("[SETUP] 🔒 DUAL RELAY SECURITY SYSTEM:");
  Serial.println("[SETUP] GPIO 13: Starter Relay (temporary)");
  Serial.println("[SETUP] GPIO 27: Ignition Relay (continuous)");
  Serial.printf("[SETUP] Starter: GPIO %d = %d (LOW/BLOCKED)\n", relayPin, digitalRead(relayPin));
  Serial.printf("[SETUP] Ignition: GPIO %d = %d (LOW/BLOCKED)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
  Serial.println("[SETUP] 🔒 ENGINE BLOCKED - Use START command or dashboard");
  Serial.println("[SETUP] 🔒 PHYSICAL KEY BLOCKED - Ignition relay OFF");
  Serial.println("[SETUP] ═══════════════════════════════════\n");

  connectToWiFi();
  
  // Send initial heartbeat
  sendMotorcycleHeartbeat(true);
  lastHeartbeat = millis();

  // Vibration sensor baseline check
  delay(500);
  int baselineReading = digitalRead(vibrationSensorPin);
  Serial.printf("\n[VIBRATION] 🔍 Sensor baseline: GPIO %d = %s\n", 
                vibrationSensorPin, baselineReading ? "HIGH" : "LOW");
  
  if (baselineReading == LOW) {
    Serial.println("[VIBRATION] ✅ Sensor ready (LOW = no vibration)");
  } else {
    Serial.println("[VIBRATION] ⚠️ WARNING: Sensor reading HIGH - check wiring!");
  }

  Serial.println("\n📋 SERIAL COMMANDS:");
  Serial.println("   START / START ENGINE - Start engine");
  Serial.println("   STOP / STOP ENGINE - Stop engine");
  Serial.println("   TEST RELAYS / FORCE RELAY OFF");
  Serial.println("   CONNECTION STATUS / GSM STATUS");
  Serial.println("   ARM THEFT / DISARM THEFT");
  Serial.println("   SECURITY STATUS");
  Serial.println("   TEST DASHBOARD / TEST SMS");
  Serial.println("   STATUS\n");
  
  Serial.println("📱 SMS COMMANDS (Format: " + smsPassword + " COMMAND):");
  Serial.println("   " + smsPassword + " START - Start engine");
  Serial.println("   " + smsPassword + " STOP - Stop engine");
  Serial.println("   " + smsPassword + " STATUS - Get system status");
  Serial.println("   " + smsPassword + " LOCATE - Get GPS location");
  Serial.println("   " + smsPassword + " ARM - Arm anti-theft");
  Serial.println("   " + smsPassword + " DISARM - Disarm anti-theft");
  Serial.println("   " + smsPassword + " HELP - Show SMS commands\n");
  
  Serial.println("📞 CALL COMMANDS:");
  Serial.println("   Call from " + authorizedNumber1 + " - Toggle engine");
  Serial.println("   Call from " + authorizedNumber2 + " - Toggle engine\n");
  
  Serial.println("🔒 SECURITY NOTICE:");
  Serial.println("   Engine is BLOCKED by default for security");
  Serial.println("   Use 'START' command or dashboard button to start");
  Serial.println("   Physical key is blocked until engine is started\n");
}

void loop() {
  // ✅ SIMPLE SECURITY: No complex authorization timeouts
  // Engine is OFF by default, only turns ON when explicitly commanded
  // This matches the working reference code behavior

  // ✅ PRIORITY: WiFi Watchdog - Check FIRST for theft prevention
  checkWiFiWatchdog();
  
  // ✅ CONNECTION MONITORING: Graceful degradation system
  checkConnectionStability();
  
  // ✅ GPS: Read data continuously
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
  
  // ✅ TRIP TRACKING: Update continuously
  updateTripData();
  
  // Send heartbeat every 1 second
  if (millis() - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    bool heartbeatSent = sendMotorcycleHeartbeat(true);
    if (heartbeatSent) {
      lastFirebaseSuccess = millis();
      lastWiFiConnected = millis();
    }
    lastHeartbeat = millis();
  }

  // ✅ SMS COMMAND PROCESSING: Like reference code
  processSMSCommands();
  
  handleDashboardButton();

  // ✅ SERIAL COMMANDS: Handle user input
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
    else if (cmd == "AUTHORIZE ENGINE" || cmd == "ENABLE ENGINE" || cmd == "START") {
      // ✅ SIMPLE: Just like SMS command in reference code
      startEngine();
    }
    else if (cmd == "DISABLE ENGINE" || cmd == "LOCK ENGINE" || cmd == "STOP") {
      // ✅ SIMPLE: Just like SMS command in reference code
      stopEngine();
    }
    else if (cmd == "CONNECTION STATUS" || cmd == "CONN STATUS") {
      Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.println("📡 CONNECTION STABILITY STATUS");
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      
      String wifiStatus = getConnectionStatus(wifiHealth, WIFI_WARNING_TIMEOUT, 
                                             WIFI_CRITICAL_TIMEOUT, WIFI_SHUTDOWN_TIMEOUT);
      String helmetStatus = getConnectionStatus(helmetHealth, HELMET_WARNING_TIMEOUT, 
                                               HELMET_CRITICAL_TIMEOUT, HELMET_SHUTDOWN_TIMEOUT);
      
      Serial.printf("WiFi Status: %s\n", wifiStatus.c_str());
      Serial.printf("  - Connected: %s\n", wifiHealth.isConnected ? "YES" : "NO");
      Serial.printf("  - Stable: %s\n", wifiHealth.isStable ? "YES" : "NO");
      Serial.printf("  - Last Success: %lu ms ago\n", millis() - wifiHealth.lastSuccessTime);
      
      Serial.printf("\nHelmet Status: %s\n", helmetStatus.c_str());
      Serial.printf("  - Connected: %s\n", helmetHealth.isConnected ? "YES" : "NO");
      Serial.printf("  - Stable: %s\n", helmetHealth.isStable ? "YES" : "NO");
      Serial.printf("  - Last Success: %lu ms ago\n", millis() - helmetHealth.lastSuccessTime);
      
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    }
    else if (cmd == "TEST RELAYS" || cmd == "RELAY TEST") {
      Serial.println("\n🧪 TESTING RELAY OPERATION");
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      
      Serial.println("Step 1: Both relays BLOCKED (should block engine)");
      digitalWrite(relayPin, LOW);        // Starter BLOCKED
      digitalWrite(ignitionRelayPin, LOW); // Ignition BLOCKED
      Serial.printf("Starter: GPIO %d = %d (LOW/BLOCKED)\n", relayPin, digitalRead(relayPin));
      Serial.printf("Ignition: GPIO %d = %d (LOW/BLOCKED)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
      Serial.println("🔒 Physical key should NOT start engine now");
      Serial.println("⏳ Test physical key for 10 seconds...");
      delay(10000);
      
      Serial.println("\nStep 2: Ignition relay ALLOW (should allow engine start)");
      digitalWrite(ignitionRelayPin, HIGH);  // Ignition ALLOW
      Serial.printf("Ignition: GPIO %d = %d (HIGH/ALLOW)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
      Serial.println("✅ Physical key should start engine now");
      Serial.println("⏳ Test physical key for 10 seconds...");
      delay(10000);
      
      Serial.println("\nStep 3: Return to blocked state");
      digitalWrite(ignitionRelayPin, LOW); // Ignition BLOCKED
      Serial.printf("Ignition: GPIO %d = %d (LOW/BLOCKED)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
      Serial.println("🔒 Physical key should NOT start engine again");
      
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.println("🧪 RELAY TEST COMPLETE");
    }
    else if (cmd == "FORCE RELAY OFF" || cmd == "BLOCK ENGINE") {
      digitalWrite(relayPin, LOW);        // Starter BLOCKED
      digitalWrite(ignitionRelayPin, LOW); // Ignition BLOCKED
      engineRunning = false;
      engineAllowed = false;
      
      Serial.println("🔒 RELAYS FORCED BLOCKED - ENGINE BLOCKED");
      Serial.printf("Starter: GPIO %d = %d (LOW/BLOCKED)\n", relayPin, digitalRead(relayPin));
      Serial.printf("Ignition: GPIO %d = %d (LOW/BLOCKED)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
      Serial.println("🔒 Physical key should be completely blocked");
    }
    else if (cmd == "STATUS") {
      printSystemStatus();
    }
    else if (cmd == "SECURITY STATUS") {
      printSecurityStatus();
    }
    else if (cmd == "TEST DASHBOARD") {
      Serial.println("\n🧪 TESTING DASHBOARD INTEGRATION...");
      Serial.println("[TEST] Checking dashboard button path: " + buttonPath);
      Serial.println("[TEST] Current engine status: " + String(engineRunning ? "RUNNING" : "STOPPED"));
      Serial.println("[TEST] Engine allowed: " + String(engineAllowed ? "YES" : "NO"));
      Serial.println("[TEST] Dashboard button state: " + String(lastDashboardButtonState ? "PRESSED" : "NORMAL"));
      Serial.println("[TEST] To test: Click 'Turn On Engine' button in dashboard");
    }
    else if (cmd == "TEST SMS") {
      Serial.println("\n🧪 TESTING SMS SYSTEM...");
      Serial.println("[TEST] GSM Ready: " + String(gsmReady ? "YES" : "NO"));
      Serial.println("[TEST] Authorized numbers:");
      Serial.println("[TEST]   Primary: " + authorizedNumber1);
      Serial.println("[TEST]   Secondary: " + authorizedNumber2);
      Serial.println("[TEST] SMS Password: " + smsPassword);
      Serial.println("[TEST] Example SMS: '" + smsPassword + " START'");
      Serial.println("[TEST] Example Call: Call from authorized number to toggle engine");
      
      if (gsmReady) {
        Serial.println("[TEST] Sending test SMS to primary number...");
        sendSMSToNumber(authorizedNumber1, "VigiLERT Test: System is working! Engine: " + String(engineRunning ? "Running" : "Stopped"));
      }
    }
    else if (cmd == "GSM STATUS") {
      Serial.println("\n📱 GSM MODULE STATUS:");
      Serial.println("GSM Ready: " + String(gsmReady ? "YES ✅" : "NO ❌"));
      Serial.println("Last SMS Sender: " + (lastSMSSender != "" ? lastSMSSender : "None"));
      checkSignalStrength();
    }
  }

  // ✅ SENSOR READINGS: Read MPU6050 continuously
  if (MPU6050_ENABLED) {
    mpu.getEvent(&accel, &gyro, &temp);
    currentTotalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                             accel.acceleration.y * accel.acceleration.y +
                             accel.acceleration.z * accel.acceleration.z);
    currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
  } else {
    // Simulate normal values when MPU6050 is not connected
    currentTotalAccel = 9.8;  // Normal gravity
    currentRoll = 0.0;        // Level position
  }
  float leanAngle = abs(currentRoll);

  // ✅ ANTI-THEFT: Ultra-sensitive vibration detection (direct in loop)
  static int lastVibrationReading = LOW;
  static unsigned long lastVibrationTrigger = 0;
  
  if (!engineRunning) {
    // Enable and arm anti-theft system
    if (!antiTheftEnabled) {
      antiTheftEnabled = true;
      engineOffTime = millis();
      consecutiveVibrations = 0;
      Serial.println("\n[ANTI-THEFT] 🛡️ System enabled - arming in 10 seconds...");
    }
    
    if (!antiTheftArmed && (millis() - engineOffTime >= ARM_DELAY)) {
      antiTheftArmed = true;
      theftDetectionCount = 0;
      theftAlertSent = false;
      consecutiveVibrations = 0;
      lastVibrationReading = digitalRead(vibrationSensorPin);
      Serial.println("\n[ANTI-THEFT] 🛡️ ARMED! Ultra-sensitive detection active...");
      Serial.printf("[ANTI-THEFT] 📍 Monitoring GPIO %d (any movement triggers alarm)\n", vibrationSensorPin);
      
      // Short beep to confirm arming
      for (int i = 0; i < 2; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        delay(100);
      }
    }
    
    // ✅ INSTANT DETECTION: Check sensor every loop
    if (antiTheftArmed) {
      int currentReading = digitalRead(vibrationSensorPin);
      unsigned long currentTime = millis();
      
      // Detect ANY state change
      if (currentReading != lastVibrationReading) {
        unsigned long timeSinceLastTrigger = currentTime - lastVibrationTrigger;
        
        // Simple debounce (50ms)
        if (timeSinceLastTrigger >= VIBRATION_DEBOUNCE) {
          consecutiveVibrations++;
          lastVibrationTrigger = currentTime;
          
          Serial.printf("\n🚨🚨 [ANTI-THEFT] VIBRATION DETECTED #%d! 🚨🚨\n", consecutiveVibrations);
          Serial.printf("[ANTI-THEFT] 📍 GPIO %d: %s → %s (movement detected)!\n", 
                        vibrationSensorPin,
                        lastVibrationReading ? "HIGH" : "LOW",
                        currentReading ? "HIGH" : "LOW");

          // ✅ FAST BUZZER: 8 quick beeps
          for (int i = 0; i < 8; i++) {
            digitalWrite(buzzerPin, HIGH);
            digitalWrite(lightIndicatorPin, HIGH);
            delay(80);
            digitalWrite(buzzerPin, LOW);
            digitalWrite(lightIndicatorPin, LOW);
            delay(80);
          }

          // ✅ SMS alert with cooldown
          unsigned long timeSinceLastAlert = currentTime - lastTheftAlert;
          
          if (timeSinceLastAlert >= THEFT_ALERT_COOLDOWN) {
            Serial.println("[ANTI-THEFT] 📱 Sending SMS + Firebase alert...");
            triggerTheftAlert();
            lastTheftAlert = currentTime;
            theftAlertSent = true;
          } else {
            Serial.printf("[ANTI-THEFT] ⏳ SMS cooldown active (%lu seconds remaining)\n", 
                          (THEFT_ALERT_COOLDOWN - timeSinceLastAlert) / 1000);
          }
        }
      }
      
      lastVibrationReading = currentReading;
    }
  } else {
    // Engine running - disarm anti-theft
    if (antiTheftArmed) {
      Serial.println("[ANTI-THEFT] 🔓 Disarmed - Engine running");
      antiTheftArmed = false;
      antiTheftEnabled = false;
      theftDetectionCount = 0;
      theftAlertSent = false;
      consecutiveVibrations = 0;
    }
  }

  // ✅ CRASH DETECTION: Continuous monitoring
  if (MPU6050_ENABLED && engineRunning && (currentTotalAccel >= ACCEL_THRESHOLD || leanAngle > 40) && !crashDetected) {
    unsigned long timeSinceLastCrash = millis() - lastCrashTime;
    if (timeSinceLastCrash >= CRASH_COOLDOWN) {
      Serial.printf("[CRASH] Detected! Accel: %.2f g, Lean: %.1f°\n", currentTotalAccel, leanAngle);
      triggerCrashShutdown(currentTotalAccel, currentRoll);
    }
  }

  if (crashDetected && MPU6050_ENABLED) {
    bool isUpright = (leanAngle < 30);
    bool isStable = (currentTotalAccel < ACCEL_THRESHOLD - 2.0);
    if (isUpright && isStable) {
      crashDetected = false;
      Serial.println("[INFO] ✓ Crash cleared");
    }
  }

  // ✅ HELMET CONNECTION: Monitor every 2 seconds
  if (millis() - lastHelmetCheck > HELMET_CHECK_INTERVAL) {
    checkHelmetConnection();
    lastHelmetCheck = millis();
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

  // ✅ SAFETY OVERRIDE: Block relay if alcohol detected
  if (alcoholDetected) {
    digitalWrite(relayPin, LOW);  // LOW = BLOCK (like reference code)
  }

  // ✅ REAL-TIME: Fast sensor monitoring every 500ms
  if (millis() - lastSensorPrint > SENSOR_PRINT_INTERVAL) {
    Serial.printf("[SENSORS] Accel: %.2f g | Lean: %.1f° | Speed: %.1f kph | Vibration: %d %s\n", 
                  currentTotalAccel, abs(currentRoll), currentSpeed, digitalRead(vibrationSensorPin),
                  MPU6050_ENABLED ? "" : "(MPU6050 BYPASSED)");
    lastSensorPrint = millis();
  }

  // ✅ QUICK STATUS: System status every 1 second
  if (millis() - lastQuickStatus > QUICK_STATUS_INTERVAL) {
    Serial.printf("[STATUS] Engine:%s | Relay:%s | Helmet:%s | Alcohol:%s | Security:%s\n",
                  engineRunning ? "RUN" : "STOP",
                  digitalRead(relayPin) ? "OFF" : "ON",
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

  // ✅ STARTER SAFETY: Check starter timeout
  checkStarterTimeout();
  
  yield();  // Allow WiFi/system tasks to run
}
// ═══════════════════════════════════════════════════════════════════════════
// CORE FUNCTIONS - Motorcycle Module
// ═══════════════════════════════════════════════════════════════════════════

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
  
  // Send to both authorized numbers
  sendSMSToNumber(authorizedNumber1, message);
  delay(2000);  // Wait between SMS sends
  sendSMSToNumber(authorizedNumber2, message);

  logTheftToFirebase(location);
}

void sendSMSToNumber(String phoneNumber, String message) {
  Serial.println("[SMS] 📱 Sending to: " + phoneNumber);
  
  gsmSerial.print("AT+CMGS=\"" + phoneNumber + "\"\r");
  delay(1000);
  
  if (gsmSerial.find(">")) {
    gsmSerial.print(message);
    delay(100);
    gsmSerial.write(26);  // Ctrl+Z to send
    Serial.println("[SMS] ✅ SMS sent to " + phoneNumber);
  } else {
    Serial.println("[SMS] ❌ Failed to send SMS to " + phoneNumber);
  }
  
  delay(3000);
}

void initializeGSM() {
  Serial.println("\n[GSM] Initializing...");
  delay(3000);
  
  // Basic AT command test
  gsmSerial.println("AT");
  delay(1000);
  
  if (gsmSerial.available()) {
    String response = gsmSerial.readString();
    if (response.indexOf("OK") != -1) {
      gsmReady = true;
      Serial.println("[GSM] ✅ AT command OK!");
    }
  }

  if (!gsmReady) {
    Serial.println("[GSM] ⚠️ Not responding to AT command");
    return;
  }

  // Set SMS text mode (like reference code)
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  
  // Set SMS notification mode (like reference code)
  gsmSerial.println("AT+CNMI=1,2,0,0,0");
  delay(1000);
  
  // Check signal strength
  checkSignalStrength();
  
  Serial.println("[GSM] ✅ Initialization complete!");
  Serial.println("[GSM] 📱 Ready to receive SMS commands");
  Serial.println("[GSM] 📞 Ready to receive calls");
}

void checkSignalStrength() {
  gsmSerial.println("AT+CSQ");
  delay(1000);
  
  while (gsmSerial.available()) {
    String response = gsmSerial.readString();
    Serial.println("[GSM] Signal Strength: " + response);
  }
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
  
  // ✅ DUAL RELAY SHUTDOWN: Cut ignition (this stops the engine)
  digitalWrite(ignitionRelayPin, LOW);  // LOW = BLOCK (like reference code)
  digitalWrite(relayPin, LOW);          // LOW = BLOCK (like reference code)
  engineRunning = false;
  
  Serial.printf("🚨 Ignition Relay GPIO %d = %d (OFF)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
  Serial.printf("🚨 Starter Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
  
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
  
  // ✅ SIMPLE SECURITY: Like reference code - just check basic safety
  // No complex authorization system needed
  
  // ✅ SAFETY CHECKS (keep these for safety)
  String wifiStatus = getConnectionStatus(wifiHealth, WIFI_WARNING_TIMEOUT, 
                                         WIFI_CRITICAL_TIMEOUT, WIFI_SHUTDOWN_TIMEOUT);
  String helmetStatus = getConnectionStatus(helmetHealth, HELMET_WARNING_TIMEOUT, 
                                           HELMET_CRITICAL_TIMEOUT, HELMET_SHUTDOWN_TIMEOUT);
  
  Serial.printf("[ENGINE] WiFi: %s\n", wifiStatus.c_str());
  Serial.printf("[ENGINE] Helmet: %s\n", helmetStatus.c_str());
  
  bool alcoholOK = !alcoholDetected;
  bool crashOK = !crashDetected;
  
  Serial.printf("[ENGINE] Alcohol: %s\n", alcoholOK ? "SAFE ✅" : "DETECTED ❌");
  Serial.printf("[ENGINE] Crash: %s\n", crashOK ? "NONE ✅" : "DETECTED ❌");
  Serial.printf("[ENGINE] Current state: %s\n", engineRunning ? "RUNNING" : "STOPPED");
  
  // ✅ BASIC SAFETY CHECKS (only block for serious issues)
  if (alcoholDetected) {
    Serial.println("\n❌ ENGINE START BLOCKED - ALCOHOL DETECTED!");
    Serial.println("💡 TIP: Wait for alcohol levels to clear");
    
    digitalWrite(relayPin, LOW);  // LOW = BLOCK (like reference code)
    
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
  
  if (crashDetected) {
    Serial.println("\n❌ ENGINE START BLOCKED - CRASH DETECTED!");
    Serial.println("💡 TIP: Clear crash alert first");
    
    digitalWrite(relayPin, LOW);  // LOW = BLOCK (like reference code)
    
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
  
  // ✅ SIMPLE RELAY CONTROL: Like reference code
  // Turn ON ignition relay - allows physical key to work
  digitalWrite(ignitionRelayPin, HIGH);  // HIGH = ALLOW (like reference code)
  Serial.println("✅ Ignition relay ON - Physical key now works");
  delay(500); // Wait for ignition to stabilize
  
  // Engage starter motor (temporary)
  digitalWrite(relayPin, HIGH);  // HIGH = ALLOW (like reference code)
  starterActive = true;
  starterStartTime = millis();
  lastStarterAttempt = millis();
  Serial.println("✅ Starter relay ON (will auto-disengage after 3 seconds)");
  
  // ✅ SIMPLE STATE: Like reference code
  engineRunning = true;
  engineAllowed = true;
  engineStartRequested = false;
  
  Serial.printf("✅ Starter Relay GPIO %d = %d (HIGH/ALLOW)\n", relayPin, digitalRead(relayPin));
  Serial.printf("✅ Ignition Relay GPIO %d = %d (HIGH/ALLOW)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
  
  // Start trip tracking
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
  
  // End trip tracking before stopping engine
  endTrip();
  
  // ✅ SIMPLE RELAY CONTROL: Like reference code
  // Turn OFF both relays - blocks physical key completely
  digitalWrite(ignitionRelayPin, LOW);  // LOW = BLOCK (like reference code)
  Serial.println("🛑 Ignition relay OFF (physical key blocked)");
  
  digitalWrite(relayPin, LOW);  // LOW = BLOCK (like reference code)
  Serial.println("🛑 Starter relay OFF");
  
  // ✅ SIMPLE STATE: Like reference code
  engineRunning = false;
  engineAllowed = false;
  
  Serial.printf("🛑 Starter Relay GPIO %d = %d (LOW/BLOCK)\n", relayPin, digitalRead(relayPin));
  Serial.printf("🛑 Ignition Relay GPIO %d = %d (LOW/BLOCK)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
  Serial.println("🔒 Physical key is now BLOCKED");
  
  sendLiveToFirebase();
  
  engineOffTime = millis();
  antiTheftEnabled = true;
  antiTheftArmed = false;
  Serial.println("[ANTI-THEFT] 🛡️ Will arm in 10 seconds...");
}

// ✅ STARTER SAFETY: Check starter timeout in main loop
void checkStarterTimeout() {
  if (starterActive && (millis() - starterStartTime >= STARTER_TIMEOUT)) {
    // Disengage starter after timeout
    digitalWrite(relayPin, LOW);  // LOW = BLOCK (like reference code)
    starterActive = false;
    Serial.println("✅ Starter disengaged (3-second timeout)");
    Serial.printf("✅ Starter Relay GPIO %d = %d (BLOCKED)\n", relayPin, digitalRead(relayPin));
    
    // Ignition relay stays ON to keep engine running
    // Only stopEngine() will turn off ignition
  }
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
  doc["relayStatus"] = digitalRead(relayPin) ? "ALLOW" : "BLOCK";
  
  // ✅ GPS speed and location
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
  StaticJsonDocument<256> engineDoc;
  engineDoc["engineRunning"] = engineRunning;
  engineDoc["engineAllowed"] = engineAllowed;
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
    lastWiFiConnection = millis();
    lastFirebaseSuccess = millis();
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
    
    // ✅ If no data or null, assume Safe
    if (response == "null" || response.length() <= 2) {
      if (alcoholDetected) {
        Serial.println("\n✅ Alcohol cleared (no data)");
        alcoholDetected = false;
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
    
    // ✅ Trust helmet's status first, use sensor value as backup
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
      const int HELMET_THRESHOLD = 600;
      
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
      
      if (currentAlcoholState) {
        digitalWrite(relayPin, LOW);  // LOW = BLOCK (like reference code)
        
        if (engineRunning) {
          triggerAlcoholShutdown();
        }
      }
    }
    
    // Debug output
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
  
  // ✅ DUAL RELAY SHUTDOWN
  digitalWrite(ignitionRelayPin, LOW);  // LOW = BLOCK (like reference code)
  digitalWrite(relayPin, LOW);          // LOW = BLOCK (like reference code)
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
  
  Serial.printf("Engine Allowed: %s\n", engineAllowed ? "YES ✅" : "NO ❌");
  Serial.printf("Engine Running: %s\n", engineRunning ? "YES ✅" : "NO ❌");
  
  Serial.printf("Helmet: %s\n", helmetConnected ? "CONNECTED ✅" : "DISCONNECTED ❌");
  Serial.printf("Starter Relay: GPIO %d = %d (%s)\n", relayPin, digitalRead(relayPin), 
                digitalRead(relayPin) ? "OFF" : "ON");
  Serial.printf("Ignition Relay: GPIO %d = %d (%s)\n", ignitionRelayPin, digitalRead(ignitionRelayPin), 
                digitalRead(ignitionRelayPin) ? "OFF" : "ON");
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
  http.setTimeout(1000);
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
          // ✅ SIMPLE: Just start engine like SMS command
          Serial.println("[DASHBOARD] Starting engine via dashboard button...");
          startEngine();
        } else {
          Serial.println("[DASHBOARD] Stopping engine via dashboard button...");
          stopEngine();
        }
        
        clearDashboardButton();
      }
    }
  }
  
  http.end();
  
  // Check autoMode setting
  HTTPClient http2;
  http2.setTimeout(1000);
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
  
  // ✅ SIMPLE SECURITY: No complex authorization needed
}

void clearDashboardButton() {
  HTTPClient http;
  http.setTimeout(1000);
  http.begin(firebaseHost + buttonPath);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT("false");
  
  if (code == HTTP_CODE_OK) {
    Serial.println("[DASHBOARD] ✓ Button cleared (ready for next press)");
    lastDashboardButtonState = false;
  } else {
    Serial.printf("[DASHBOARD] ✗ Button clear failed: %d\n", code);
  }
  
  http.end();
}

// ✅ SMS COMMAND PROCESSING: Like reference code
void processSMSCommands() {
  while (gsmSerial.available()) {
    String response = gsmSerial.readString();
    Serial.println("[GSM] Response: " + response);
    
    // Handle incoming calls (like reference code)
    if (response.indexOf("RING") != -1) {
      handleIncomingCall(response);
    }
    
    // Handle incoming SMS (like reference code)
    if (response.indexOf("+CMT:") != -1) {
      handleIncomingSMS(response);
    }
  }
}

void handleIncomingCall(String response) {
  int numberStart = response.indexOf("\"") + 1;
  int numberEnd = response.indexOf("\"", numberStart);
  
  if (numberStart == 0 || numberEnd == -1) {
    Serial.println("[CALL] Failed to parse caller number");
    return;
  }
  
  String callerNumber = response.substring(numberStart, numberEnd);
  callerNumber.trim();
  
  Serial.println("[CALL] Incoming call from: " + callerNumber);
  
  // Check if caller is authorized (like reference code)
  if (callerNumber != authorizedNumber1 && callerNumber != authorizedNumber2) {
    Serial.println("[CALL] ❌ Unauthorized caller, rejecting...");
    gsmSerial.println("ATH");  // Hang up
    return;
  }
  
  Serial.println("[CALL] ✅ Authorized caller - toggling engine");
  
  // Toggle engine state (like reference code)
  if (engineRunning) {
    stopEngine();
    sendSMSReply("Engine Stopped via Call.");
  } else {
    startEngine();
    sendSMSReply("Engine Started via Call.");
  }
  
  gsmSerial.println("ATH");  // Hang up
  delay(1000);
}

void handleIncomingSMS(String response) {
  // Parse sender number
  int numberStart = response.indexOf("\"") + 1;
  int numberEnd = response.indexOf("\"", numberStart);
  
  if (numberStart == 0 || numberEnd == -1) {
    Serial.println("[SMS] Failed to parse sender number");
    return;
  }
  
  String senderNumber = response.substring(numberStart, numberEnd);
  senderNumber.trim();
  
  Serial.println("[SMS] Message from: " + senderNumber);
  
  // Check if sender is authorized (like reference code)
  if (senderNumber != authorizedNumber1 && senderNumber != authorizedNumber2) {
    Serial.println("[SMS] ❌ Unauthorized sender: " + senderNumber);
    return;
  }
  
  lastSMSSender = senderNumber;  // Store for replies
  
  // Parse SMS body
  int bodyStart = response.indexOf("\r\n", numberEnd) + 2;
  if (bodyStart < 2 || bodyStart >= response.length()) {
    Serial.println("[SMS] Failed to parse SMS body");
    return;
  }
  
  String smsBody = response.substring(bodyStart);
  smsBody.trim();
  smsBody.toUpperCase();  // Convert to uppercase for easier matching
  
  Serial.println("[SMS] Message body: " + smsBody);
  
  // Validate password and process command (like reference code)
  if (validateSMSPassword(smsBody)) {
    String command = smsBody.substring(smsPassword.length() + 1);  // Remove password + space
    command.trim();
    processSMSCommand(command);
  } else {
    Serial.println("[SMS] ❌ Invalid password");
    sendSMSReply("Invalid password. Format: " + smsPassword + " COMMAND");
  }
}

bool validateSMSPassword(String message) {
  return message.startsWith(smsPassword + " ");
}

void processSMSCommand(String command) {
  Serial.println("[SMS] Processing command: " + command);
  
  if (command == "START" || command == "START ENGINE") {
    startEngine();
    sendSMSReply("Engine Started via SMS.");
  }
  else if (command == "STOP" || command == "STOP ENGINE") {
    stopEngine();
    sendSMSReply("Engine Stopped via SMS.");
  }
  else if (command == "STATUS") {
    String statusMsg = "Engine: " + String(engineRunning ? "Running" : "Stopped");
    statusMsg += "\nAlcohol: " + String(alcoholDetected ? "Detected" : "Safe");
    statusMsg += "\nCrash: " + String(crashDetected ? "Detected" : "None");
    statusMsg += "\nAnti-theft: " + String(antiTheftArmed ? "Armed" : "Disarmed");
    sendSMSReply(statusMsg);
  }
  else if (command == "LOCATE" || command == "LOCATION") {
    if (gps.location.isValid()) {
      String locationMsg = "GPS Location:\nLat: " + String(gps.location.lat(), 6);
      locationMsg += "\nLng: " + String(gps.location.lng(), 6);
      locationMsg += "\nSpeed: " + String(currentSpeed, 1) + " kph";
      locationMsg += "\nGoogle Maps: https://maps.google.com/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
      sendSMSReply(locationMsg);
    } else {
      sendSMSReply("GPS location not available.");
    }
  }
  else if (command == "ARM THEFT" || command == "ARM") {
    antiTheftArmed = true;
    antiTheftEnabled = true;
    sendSMSReply("Anti-theft system ARMED.");
  }
  else if (command == "DISARM THEFT" || command == "DISARM") {
    antiTheftArmed = false;
    antiTheftEnabled = false;
    theftDetectionCount = 0;
    sendSMSReply("Anti-theft system DISARMED.");
  }
  else if (command == "HELP") {
    String helpMsg = "Available commands:\nSTART - Start engine\nSTOP - Stop engine";
    helpMsg += "\nSTATUS - Get status\nLOCATE - Get GPS location";
    helpMsg += "\nARM - Arm anti-theft\nDISARM - Disarm anti-theft";
    sendSMSReply(helpMsg);
  }
  else {
    Serial.println("[SMS] ❌ Unknown command: " + command);
    sendSMSReply("Unknown command. Send 'HELP' for available commands.");
  }
}

void sendSMSReply(String message) {
  if (lastSMSSender == "") {
    Serial.println("[SMS] ❌ No sender to reply to");
    return;
  }
  
  Serial.println("[SMS] 📱 Sending reply to: " + lastSMSSender);
  Serial.println("[SMS] Message: " + message);
  
  gsmSerial.print("AT+CMGS=\"" + lastSMSSender + "\"\r");
  delay(1000);
  
  if (gsmSerial.find(">")) {
    gsmSerial.print(message);
    delay(100);
    gsmSerial.write(26);  // Ctrl+Z to send
    Serial.println("[SMS] ✅ SMS sent successfully");
  } else {
    Serial.println("[SMS] ❌ Failed to send SMS");
  }
  
  delay(3000);
}

// ✅ CONNECTION STABILITY: Graceful degradation system
void updateConnectionHealth(ConnectionHealth &health, bool currentStatus, const char* connectionName) {
  unsigned long currentTime = millis();
  health.lastCheckTime = currentTime;
  
  if (currentStatus) {
    health.lastSuccessTime = currentTime;
    health.consecutiveFailures = 0;
    health.consecutiveSuccesses++;
    
    if (health.consecutiveSuccesses >= STABILITY_REQUIRED_SUCCESSES && !health.isStable) {
      health.isStable = true;
      health.stableStartTime = currentTime;
      Serial.printf("[%s] Connection STABILIZED ✅ (%d consecutive successes)\n", 
                    connectionName, health.consecutiveSuccesses);
    }
  } else {
    health.consecutiveSuccesses = 0;
    health.consecutiveFailures++;
    
    if (health.isStable) {
      health.isStable = false;
      Serial.printf("[%s] Connection UNSTABLE ⚠️ (failure detected)\n", connectionName);
    }
  }
  
  health.isConnected = currentStatus;
}

String getConnectionStatus(ConnectionHealth &health, unsigned long warningTimeout, 
                          unsigned long criticalTimeout, unsigned long shutdownTimeout) {
  unsigned long timeSinceSuccess = millis() - health.lastSuccessTime;
  
  if (health.isConnected && health.isStable) {
    return "STABLE";
  } else if (health.isConnected) {
    return "CONNECTING";
  } else if (timeSinceSuccess < warningTimeout) {
    return "RECONNECTING";
  } else if (timeSinceSuccess < criticalTimeout) {
    return "WARNING";
  } else if (timeSinceSuccess < shutdownTimeout) {
    return "CRITICAL";
  } else {
    return "FAILED";
  }
}

bool shouldShutdownForConnection(ConnectionHealth &health, unsigned long shutdownTimeout) {
  unsigned long timeSinceSuccess = millis() - health.lastSuccessTime;
  return (timeSinceSuccess >= shutdownTimeout) && health.lastSuccessTime > 0;
}

void checkConnectionStability() {
  unsigned long currentTime = millis();
  
  static unsigned long lastStabilityCheck = 0;
  if (currentTime - lastStabilityCheck < CONNECTION_CHECK_INTERVAL) {
    return;
  }
  lastStabilityCheck = currentTime;
  
  // ✅ WiFi Health Check
  bool wifiConnected = (WiFi.status() == WL_CONNECTED);
  updateConnectionHealth(wifiHealth, wifiConnected, "WIFI");
  
  // ✅ Helmet Health Check  
  bool helmetOK = helmetConnected;
  updateConnectionHealth(helmetHealth, helmetOK, "HELMET");
  
  // ✅ Get current status
  String wifiStatus = getConnectionStatus(wifiHealth, WIFI_WARNING_TIMEOUT, 
                                         WIFI_CRITICAL_TIMEOUT, WIFI_SHUTDOWN_TIMEOUT);
  String helmetStatus = getConnectionStatus(helmetHealth, HELMET_WARNING_TIMEOUT, 
                                           HELMET_CRITICAL_TIMEOUT, HELMET_SHUTDOWN_TIMEOUT);
  
  // ✅ Status change notifications
  static String lastWifiStatus = "";
  static String lastHelmetStatus = "";
  
  if (wifiStatus != lastWifiStatus) {
    Serial.printf("[WIFI] Status changed: %s → %s\n", lastWifiStatus.c_str(), wifiStatus.c_str());
    lastWifiStatus = wifiStatus;
  }
  
  if (helmetStatus != lastHelmetStatus) {
    Serial.printf("[HELMET] Status changed: %s → %s\n", lastHelmetStatus.c_str(), helmetStatus.c_str());
    lastHelmetStatus = helmetStatus;
  }
  
  // ✅ Only shutdown after ALL grace periods exhausted
  bool wifiShutdownRequired = shouldShutdownForConnection(wifiHealth, WIFI_SHUTDOWN_TIMEOUT);
  bool helmetShutdownRequired = shouldShutdownForConnection(helmetHealth, HELMET_SHUTDOWN_TIMEOUT);
  
  if (engineRunning && (wifiShutdownRequired || helmetShutdownRequired)) {
    Serial.println("\n🚨 GRACEFUL SHUTDOWN - CONNECTION FAILURE");
    
    if (wifiShutdownRequired) {
      Serial.printf("📡 WiFi failed for %lu seconds (>%lu limit)\n", 
                    (currentTime - wifiHealth.lastSuccessTime) / 1000, WIFI_SHUTDOWN_TIMEOUT / 1000);
      engineStoppedByWifi = true;
    }
    
    if (helmetShutdownRequired) {
      Serial.printf("🪖 Helmet failed for %lu seconds (>%lu limit)\n", 
                    (currentTime - helmetHealth.lastSuccessTime) / 1000, HELMET_SHUTDOWN_TIMEOUT / 1000);
      engineStoppedByHelmet = true;
    }
    
    triggerGracefulShutdown("Connection Stability Failure");
    lastEngineStopTime = currentTime;
  }
  
  // ✅ Status display every 10 seconds
  static unsigned long lastStatusDisplay = 0;
  if (currentTime - lastStatusDisplay >= 10000) {
    Serial.printf("[STABILITY] WiFi:%s | Helmet:%s | Engine:%s\n",
                  wifiStatus.c_str(), helmetStatus.c_str(), 
                  engineRunning ? "RUNNING" : "STOPPED");
    lastStatusDisplay = currentTime;
  }
}

void triggerGracefulShutdown(String reason) {
  Serial.println("\n🔄 GRACEFUL SHUTDOWN INITIATED");
  Serial.println("Reason: " + reason);
  Serial.println("Time: " + String(millis()) + " ms");
  
  stopEngine();
  
  Serial.println("🔄 Graceful shutdown completed - connections will be monitored for recovery");
  
  logSecurityEventToFirebase("Graceful Shutdown - " + reason);
}
void triggerSecurityShutdown(String reason) {
  Serial.println("\n🚨 SECURITY SHUTDOWN INITIATED 🚨");
  Serial.println("Reason: " + reason);
  Serial.println("Time: " + String(millis()) + " ms");
  
  // Force relay BLOCKED
  digitalWrite(relayPin, LOW);           // LOW = BLOCK (like reference code)
  digitalWrite(ignitionRelayPin, LOW);   // LOW = BLOCK (like reference code)
  engineRunning = false;
  
  endTrip();
  
  Serial.printf("🛑 Engine STOPPED | Starter GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
  Serial.printf("🛑 Engine STOPPED | Ignition GPIO %d = %d (OFF)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
  
  // Security alert sequence (10 loud beeps)
  for (int i = 0; i < 10; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(150);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(lightIndicatorPin, LOW);
    delay(150);
  }
  
  logSecurityEventToFirebase(reason);
}

// ✅ WIFI WATCHDOG: Emergency engine cutoff if WiFi disconnects while relay is OFF
void checkWiFiWatchdog() {
  unsigned long currentTime = millis();
  bool wifiConnected = (WiFi.status() == WL_CONNECTED);
  bool relayIsOff = (digitalRead(relayPin) == HIGH);  // HIGH = OFF for active-low relay
  
  // ✅ STARTUP WiFi CHECK: If WiFi not connected within 6 seconds of boot, cut engine
  if (!startupWiFiCheckComplete) {
    unsigned long timeSinceBoot = currentTime - deviceBootTime;
    
    if (wifiConnected) {
      startupWiFiCheckComplete = true;
      Serial.println("\n[STARTUP CHECK] ✅ WiFi connected successfully!");
      Serial.printf("[STARTUP CHECK] Connected in %lu ms\n", timeSinceBoot);
    } else if (timeSinceBoot >= STARTUP_WIFI_TIMEOUT && !startupShutdownTriggered) {
      startupShutdownTriggered = true;
      startupWiFiCheckComplete = true;
      
      Serial.println("\n🚨🚨🚨 [STARTUP CHECK] WiFi CONNECTION FAILED! 🚨🚨🚨");
      Serial.printf("[STARTUP CHECK] No WiFi after %lu ms (>%lu ms limit)\n", 
                    timeSinceBoot, STARTUP_WIFI_TIMEOUT);
      Serial.println("[STARTUP CHECK] 🔒 THEFT PREVENTION: Engine may have been started before device boot!");
      Serial.println("[STARTUP CHECK] 🔒 Cutting engine power NOW!");
      
      // ✅ DUAL RELAY SHUTDOWN
      Serial.println("[STARTUP CHECK] 🔒 Cutting ignition relay...");
      digitalWrite(ignitionRelayPin, LOW);  // LOW = BLOCK (like reference code)
      digitalWrite(relayPin, LOW);          // LOW = BLOCK (like reference code)
      engineRunning = false;
      
      Serial.printf("[STARTUP CHECK] ✅ Ignition Relay GPIO %d = %d (OFF)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
      Serial.printf("[STARTUP CHECK] ✅ Starter Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
      Serial.println("[STARTUP CHECK] ✅ Engine power CUT - Motorcycle secured!");
      
      // Loud alarm sequence
      for (int i = 0; i < 20; i++) {
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(lightIndicatorPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(lightIndicatorPin, LOW);
        delay(100);
      }
    } else {
      static unsigned long lastStartupWarning = 0;
      if (currentTime - lastStartupWarning >= 1000) {
        unsigned long timeRemaining = STARTUP_WIFI_TIMEOUT - timeSinceBoot;
        Serial.printf("[STARTUP CHECK] ⏳ Waiting for WiFi... (%lu ms remaining)\n", timeRemaining);
        lastStartupWarning = currentTime;
      }
    }
  }
  
  // ✅ ACTIVATE WATCHDOG: When relay is OFF (security mode)
  if (relayIsOff && !wifiWatchdogActive) {
    wifiWatchdogActive = true;
    Serial.println("\n[WIFI WATCHDOG] 🛡️ ACTIVATED - Monitoring WiFi connection");
    Serial.println("[WIFI WATCHDOG] 🔒 Relay is OFF - Any WiFi disconnection will trigger emergency shutdown");
  }
  
  // ✅ DEACTIVATE WATCHDOG: When relay is ON (normal operation)
  if (!relayIsOff && wifiWatchdogActive) {
    wifiWatchdogActive = false;
    emergencyShutdownTriggered = false;
    Serial.println("\n[WIFI WATCHDOG] ✅ DEACTIVATED - Relay is ON (normal operation)");
  }
  
  // ✅ MONITOR: Check WiFi status when watchdog is active
  if (wifiWatchdogActive) {
    if (wifiConnected) {
      wifiDisconnectedTime = 0;
      emergencyShutdownTriggered = false;
    } else {
      if (wifiDisconnectedTime == 0) {
        wifiDisconnectedTime = currentTime;
        Serial.println("\n⚠️⚠️⚠️ [WIFI WATCHDOG] WiFi DISCONNECTED! ⚠️⚠️⚠️");
        Serial.printf("[WIFI WATCHDOG] 🔒 Emergency shutdown in %lu seconds...\n", WIFI_WATCHDOG_TIMEOUT / 1000);
        Serial.println("[WIFI WATCHDOG] 🚨 This prevents theft by WiFi disconnection!");
      }
      
      unsigned long disconnectedDuration = currentTime - wifiDisconnectedTime;
      
      // ✅ EMERGENCY SHUTDOWN: WiFi disconnected for too long
      if (disconnectedDuration >= WIFI_WATCHDOG_TIMEOUT && !emergencyShutdownTriggered) {
        emergencyShutdownTriggered = true;
        
        Serial.println("\n🚨🚨🚨 [WIFI WATCHDOG] EMERGENCY SHUTDOWN! 🚨🚨🚨");
        Serial.printf("[WIFI WATCHDOG] WiFi disconnected for %lu ms (>%lu ms limit)\n", 
                      disconnectedDuration, WIFI_WATCHDOG_TIMEOUT);
        Serial.println("[WIFI WATCHDOG] 🔒 THEFT PREVENTION: Cutting engine power!");
        
        // ✅ DUAL RELAY SHUTDOWN
        Serial.println("[WIFI WATCHDOG] 🔒 Cutting ignition relay...");
        digitalWrite(ignitionRelayPin, LOW);  // LOW = BLOCK (like reference code)
        digitalWrite(relayPin, LOW);          // LOW = BLOCK (like reference code)
        engineRunning = false;
        
        Serial.printf("[WIFI WATCHDOG] ✅ Ignition Relay GPIO %d = %d (OFF)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
        Serial.printf("[WIFI WATCHDOG] ✅ Starter Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
        Serial.println("[WIFI WATCHDOG] ✅ Engine power CUT - Motorcycle secured!");
        
        // Loud alarm sequence
        for (int i = 0; i < 20; i++) {
          digitalWrite(buzzerPin, HIGH);
          digitalWrite(lightIndicatorPin, HIGH);
          delay(100);
          digitalWrite(buzzerPin, LOW);
          digitalWrite(lightIndicatorPin, LOW);
          delay(100);
        }
      }
      
      // ✅ WARNING BEEPS: Every second while disconnected
      static unsigned long lastWarningBeep = 0;
      if (currentTime - lastWarningBeep >= 1000 && !emergencyShutdownTriggered) {
        digitalWrite(buzzerPin, HIGH);
        delay(50);
        digitalWrite(buzzerPin, LOW);
        lastWarningBeep = currentTime;
        
        unsigned long timeRemaining = WIFI_WATCHDOG_TIMEOUT - disconnectedDuration;
        Serial.printf("[WIFI WATCHDOG] ⚠️ WiFi still disconnected! Shutdown in %lu ms...\n", timeRemaining);
      }
    }
    
    // ✅ STATUS: Show watchdog status every 5 seconds
    static unsigned long lastWatchdogStatus = 0;
    if (currentTime - lastWatchdogStatus >= 5000) {
      Serial.printf("[WIFI WATCHDOG] Status: %s | Relay: %s | Engine: %s\n",
                    wifiConnected ? "WiFi OK ✅" : "WiFi LOST ❌",
                    relayIsOff ? "OFF (Secured)" : "ON",
                    engineRunning ? "RUNNING" : "STOPPED");
      lastWatchdogStatus = currentTime;
    }
  }
}
// ✅ HELMET CONNECTION: Monitor with timeout detection
void checkHelmetConnection() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.setTimeout(1000);
  String helmetPath = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
  http.begin(helmetPath);
  
  int httpCode = http.GET();
  
  static unsigned long lastDebugRead = 0;
  if (millis() - lastDebugRead > 5000) {
    Serial.printf("[HELMET DEBUG] HTTP Code: %d\n", httpCode);
    lastDebugRead = millis();
  }
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    
    static unsigned long lastResponseDebug = 0;
    if (millis() - lastResponseDebug > 5000) {
      Serial.println("[HELMET DEBUG] Response: " + response);
      lastResponseDebug = millis();
    }
    
    // ✅ Check status first
    bool statusIsOn = (response.indexOf("\"status\":\"On\"") != -1);
    
    if (!statusIsOn) {
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
        lastHelmetHeartbeat = helmetHeartbeat;
        lastHelmetUpdateTime = millis();
        helmetStatusForcedOff = false;
        
        if (!helmetConnected) {
          Serial.println("\n[HELMET] ✅ Connected!");
        }
        helmetConnected = true;
        
        Serial.printf("[HELMET DEBUG] Heartbeat updated! New: %llu, lastHelmetUpdateTime: %lu\n", 
                      helmetHeartbeat, lastHelmetUpdateTime);
      } else {
        unsigned long timeSinceUpdate = millis() - lastHelmetUpdateTime;
        Serial.printf("[HELMET DEBUG] Heartbeat unchanged. Time since update: %lu ms\n", timeSinceUpdate);
        
        // ✅ After 10 seconds, force status to "Off" in Firebase
        if (timeSinceUpdate > HELMET_FORCE_OFF_TIMEOUT && !helmetStatusForcedOff) {
          Serial.printf("\n[HELMET] 🚨 FORCING STATUS TO OFF (no update for %lu ms)\n", timeSinceUpdate);
          forceHelmetStatusOff();
          helmetStatusForcedOff = true;
          helmetConnected = false;
        }
        // After 5 seconds, mark as disconnected locally
        else if (timeSinceUpdate > HELMET_TIMEOUT) {
          if (helmetConnected) {
            Serial.printf("\n[HELMET] ⚠️ Disconnected (no heartbeat update for %lu ms)\n", timeSinceUpdate);
          }
          helmetConnected = false;
        }
      }
    } else {
      if (!helmetConnected) {
        Serial.println("\n[HELMET] ✅ Connected (no timestamp check)");
      }
      helmetConnected = true;
      lastHelmetUpdateTime = millis();
      Serial.printf("[HELMET DEBUG] No heartbeat parsing, but status On. Updated lastHelmetUpdateTime: %lu\n", lastHelmetUpdateTime);
    }
  } else {
    if (helmetConnected) {
      Serial.printf("\n[HELMET] ⚠️ Disconnected (Firebase read failed: HTTP %d)\n", httpCode);
    }
    helmetConnected = false;
  }
  
  http.end();
  
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 1000) {
    Serial.printf("[HELMET] Status: %s\n", helmetConnected ? "CONNECTED ✅" : "DISCONNECTED ❌");
    lastDebug = millis();
  }
}

void forceHelmetStatusOff() {
  if (WiFi.status() != WL_CONNECTED) return;

  Serial.println("[HELMET] 📝 Updating Firebase status to 'Off'...");
  
  StaticJsonDocument<128> doc;
  doc["status"] = "Off";
  doc["lastHeartbeat"] = lastHelmetHeartbeat;
  doc["timestamp"] = lastHelmetHeartbeat;
  doc["forcedOffByMotorcycle"] = true;
  
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

// ✅ TRIP TRACKING FUNCTIONS
void startTrip() {
  Serial.println("\n🗺️ STARTING TRIP TRACKING");
  
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
  
  if (gps.location.isValid()) {
    double currentLat = gps.location.lat();
    double currentLng = gps.location.lng();
    
    if (!currentTrip.hasValidStart) {
      currentTrip.fromLat = currentLat;
      currentTrip.fromLng = currentLng;
      currentTrip.hasValidStart = true;
      
      Serial.printf("[TRIP] 🚀 FROM: %.6f, %.6f\n", currentLat, currentLng);
      Serial.println("[TRIP] 📍 Trip origin set!");
    }
    
    currentTrip.toLat = currentLat;
    currentTrip.toLng = currentLng;
    
    if (lastValidLat != 0.0 && lastValidLng != 0.0) {
      float distance = calculateDistance(lastValidLat, lastValidLng, currentLat, currentLng);
      currentTrip.totalDistance += distance;
    }
    
    lastValidLat = currentLat;
    lastValidLng = currentLng;
    
    if (gps.speed.isValid()) {
      float speed = gps.speed.kmph();
      if (speed > currentTrip.maxSpeed) {
        currentTrip.maxSpeed = speed;
      }
    }
    
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
  
  if (currentTrip.hasValidStart && currentTrip.toLat != 0.0 && currentTrip.toLng != 0.0) {
    Serial.printf("[TRIP] 🚀 FROM: %.6f, %.6f\n", currentTrip.fromLat, currentTrip.fromLng);
    Serial.printf("[TRIP] 🏁 TO: %.6f, %.6f\n", currentTrip.toLat, currentTrip.toLng);
    Serial.printf("[TRIP] 📊 Distance: %.2f km | Max Speed: %.1f kph\n", 
                  currentTrip.totalDistance, currentTrip.maxSpeed);
    
    uint64_t duration = currentTrip.endTime - currentTrip.startTime;
    Serial.printf("[TRIP] ⏱️ Duration: %llu minutes\n", duration / 60000);
    
    saveTripToFirebase();
  } else {
    Serial.println("[TRIP] ⚠️ No valid GPS data - trip not saved");
  }
  
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
  doc["from"]["address"] = "GPS Location";
  
  // To coordinates  
  doc["to"]["lat"] = currentTrip.toLat;
  doc["to"]["lng"] = currentTrip.toLng;
  doc["to"]["address"] = "GPS Location";
  
  // Trip statistics
  doc["distance"] = currentTrip.totalDistance;
  doc["maxSpeed"] = currentTrip.maxSpeed;
  doc["avgSpeed"] = (currentTrip.totalDistance / ((currentTrip.endTime - currentTrip.startTime) / 3600000.0));
  
  // Additional metadata
  doc["vehicleType"] = "motorcycle";
  doc["userId"] = userUID;
  doc["tripId"] = String(currentTrip.startTime);
  
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

// ✅ SECURITY EVENT LOGGING
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

// ✅ COMPREHENSIVE SECURITY STATUS
void printSecurityStatus() {
  unsigned long currentTime = millis();
  unsigned long timeSinceWiFi = currentTime - lastWiFiConnected;
  unsigned long timeSinceHelmet = currentTime - lastHelmetUpdateTime;
  
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🔒 COMPREHENSIVE SECURITY STATUS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  Serial.printf("Security System: %s\n", securitySystemActive ? "ACTIVE 🔒" : "DISABLED ⚠️");
  Serial.printf("Secure Mode: %s\n", systemInSecureMode ? "ENGAGED 🚨" : "NORMAL ✅");
  
  Serial.println("\n📡 CONNECTIVITY STATUS:");
  Serial.printf("WiFi: %s (%lu ms ago)\n", 
                (timeSinceWiFi <= WIFI_SHUTDOWN_TIMEOUT) ? "CONNECTED ✅" : "TIMEOUT ❌", 
                timeSinceWiFi);
  Serial.printf("WiFi Timeout Limit: %lu ms\n", WIFI_SHUTDOWN_TIMEOUT);
  
  Serial.printf("Helmet: %s (%lu ms ago)\n", 
                (timeSinceHelmet <= HELMET_SHUTDOWN_TIMEOUT || lastHelmetUpdateTime == 0) ? "CONNECTED ✅" : "TIMEOUT ❌", 
                timeSinceHelmet);
  Serial.printf("Helmet Timeout Limit: %lu ms\n", HELMET_SHUTDOWN_TIMEOUT);
  
  Serial.println("\n🛡️ SAFETY STATUS:");
  Serial.printf("Engine: %s\n", engineRunning ? "RUNNING 🟢" : "STOPPED 🔴");
  Serial.printf("Alcohol: %s\n", alcoholDetected ? "DETECTED ❌" : "SAFE ✅");
  Serial.printf("Crash: %s\n", crashDetected ? "DETECTED ❌" : "NONE ✅");
  Serial.printf("Anti-Theft: %s\n", antiTheftArmed ? "ARMED 🛡️" : "DISARMED 🔓");
  
  Serial.println("\n🔑 PHYSICAL KEY STATUS:");
  Serial.println("Physical Key: Not monitored (dual relay security active)");
  Serial.printf("Key Security: %s\n", securitySystemActive ? "PROTECTED 🔒" : "UNPROTECTED ⚠️");
  
  Serial.println("\n⚡ RELAY STATUS:");
  Serial.printf("Starter Relay GPIO %d: %d (%s)\n", relayPin, digitalRead(relayPin), 
                digitalRead(relayPin) ? "OFF" : "ON");
  Serial.printf("Ignition Relay GPIO %d: %d (%s)\n", ignitionRelayPin, digitalRead(ignitionRelayPin), 
                digitalRead(ignitionRelayPin) ? "OFF" : "ON");
  
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