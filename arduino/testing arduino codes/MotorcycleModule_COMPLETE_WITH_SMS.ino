// ═══════════════════════════════════════════════════════════════════════════
// VIGILERT MOTORCYCLE MODULE - COMPLETE WITH SMS ALERTS
// ═══════════════════════════════════════════════════════════════════════════
// ✅ FIXES: Dashboard button not working issue
// ✅ ADDS: SMS alerts for all emergency situations
// ✅ INTEGRATES: Real MPU6050 lean angle detection
// ✅ INCLUDES: All safety features with SMS notifications
// ✅ MAINTAINS: All existing functionality without errors
// ═══════════════════════════════════════════════════════════════════════════

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <MPU6050.h>

// ═══════════════════════════════════════════════════════════════════════════
// CONFIGURATION
// ═══════════════════════════════════════════════════════════════════════════

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "123456789000";

// Firebase Configuration
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";

// Firebase Paths - EXACT SAME AS YOUR WORKING VERSION
const String devicePath = "/helmet_public/" + userUID + "/devices/motorcycle.json";
const String helmetPath = "/helmet_public/" + userUID + "/devices/helmet.json";
const String buttonPath = "/" + userUID + "/engineControl/startButton.json";
const String livePath = "/helmet_public/" + userUID + "/live.json";
const String crashPath = "/helmet_public/" + userUID + "/crashes.json";
const String alcoholPath = "/helmet_public/" + userUID + "/alcohol/status.json";
const String speedLimitPath = "/" + userUID + "/speedLimit.json";
const String alertsPath = "/helmet_public/" + userUID + "/alerts.json";
const String tripsPath = "/helmet_public/" + userUID + "/trips.json";
const String emergencyContactPath = "/helmet_public/" + userUID + "/emergencyContact.json";

// Pin Assignments
const int relayPin = 13;              // Starter relay
const int ignitionRelayPin = 27;      // Ignition relay
const int buzzerPin = 12;
const int vibrationSensorPin = 15;

// ✅ MPU6050 I2C Configuration
const int SDA_PIN = 21;
const int SCL_PIN = 22;

// ✅ SIM800L GSM Configuration
const int GSM_RX_PIN = 25;           // Connect to SIM800L TX
const int GSM_TX_PIN = 26;           // Connect to SIM800L RX
const int GSM_POWER_PIN = 4;         // Connect to SIM800L PWRKEY (optional)

// ✅ ENHANCED SAFETY THRESHOLDS
const float LEAN_ANGLE_THRESHOLD = 40.0;        // 40° lean angle for crash detection
const float LEAN_ANGLE_WARNING = 30.0;          // 30° warning threshold
const float LEAN_ANGLE_CRITICAL = 50.0;         // 50° critical threshold for immediate shutdown
const float CRASH_ACCEL_THRESHOLD = 15.0;       // g-force threshold
const int ALCOHOL_THRESHOLD = 3000;             // Alcohol sensor threshold (from helmet)

// ✅ SAFETY TIMEOUTS
const unsigned long ALCOHOL_SHUTDOWN_DELAY = 2000;    // 2 seconds delay before engine shutdown
const unsigned long LEAN_ANGLE_TIMEOUT = 3000;       // 3 seconds to recover from lean
const unsigned long SAFETY_CHECK_INTERVAL = 100;     // 100ms safety checks
const unsigned long ENGINE_RESTART_COOLDOWN = 5000;   // 5 seconds before restart allowed

// ✅ TIMING INTERVALS
const unsigned long HEARTBEAT_INTERVAL = 500;      // 500ms heartbeat for real-time updates
const unsigned long BUTTON_CHECK_INTERVAL = 200;   // 200ms button check for responsiveness
const unsigned long GPS_UPDATE_INTERVAL = 1000;    // 1s GPS updates
const unsigned long SENSOR_READ_INTERVAL = 100;    // 100ms sensor readings
const unsigned long LIVE_DATA_INTERVAL = 500;      // 500ms live data updates
const unsigned long CRASH_COOLDOWN = 3000;         // 3s crash cooldown

// ✅ SMS CONFIGURATION
const unsigned long SMS_COOLDOWN = 30000;          // 30 seconds between SMS alerts
const unsigned long SMS_TIMEOUT = 10000;           // 10 seconds timeout for SMS operations

// ✅ HARDWARE SERIAL OBJECTS
HardwareSerial gpsSerial(1);    // GPS on Serial1
HardwareSerial gsmSerial(2);    // GSM on Serial2
TinyGPSPlus gps;
MPU6050 mpu;

// ✅ CORE STATES
bool engineRunning = false;
bool engineAllowed = false;
bool crashDetected = false;
bool antiTheftArmed = false;
bool helmetConnected = false;

// ✅ ENHANCED SAFETY STATES
bool alcoholDetected = false;
bool leanAngleExceeded = false;
bool leanAngleWarning = false;
bool safetyShutdownActive = false;
String shutdownReason = "";
unsigned long lastSafetyCheck = 0;
unsigned long alcoholDetectedTime = 0;
unsigned long leanAngleStartTime = 0;
unsigned long lastEngineShutdown = 0;

// ✅ MPU6050 SENSOR VARIABLES
int16_t ax, ay, az;
int16_t gx, gy, gz;
float roll = 0.0;
float pitch = 0.0;
float rollOffset = 0.0;
float pitchOffset = 0.0;
bool mpuInitialized = false;

// ✅ CALCULATED SENSOR VARIABLES
float currentAccel = 9.8;
float currentTotalAccel = 9.8;
float currentRoll = 0.0;
float currentPitch = 0.0;
float currentLeanAngle = 0.0;
int vibrationCount = 0;
int alcoholSensorValue = 0;

// ✅ GPS VARIABLES
double currentLat = 0.0;
double currentLng = 0.0;
float currentSpeed = 0.0;
bool gpsValid = false;
int satelliteCount = 0;

// ✅ DASHBOARD VARIABLES
int speedLimit = 60;
bool isOverSpeed = false;
String riderStatus = "Inactive";
String alertnessStatus = "Normal";

// ✅ TIMING VARIABLES
unsigned long lastHeartbeat = 0;
unsigned long lastButtonCheck = 0;
unsigned long lastGPSUpdate = 0;
unsigned long lastSensorRead = 0;
unsigned long lastLiveDataUpdate = 0;
unsigned long lastCrashTime = 0;

// ✅ TRIP TRACKING VARIABLES
bool tripActive = false;
unsigned long tripStartTime = 0;
double tripStartLat = 0.0;
double tripStartLng = 0.0;
float tripDistance = 0.0;
float tripMaxSpeed = 0.0;

// ✅ SMS VARIABLES
bool gsmInitialized = false;
String emergencyPhoneNumber = "";
unsigned long lastSMSTime = 0;
unsigned long lastCrashSMS = 0;
unsigned long lastAlcoholSMS = 0;
unsigned long lastTheftSMS = 0;
unsigned long lastSpeedSMS = 0;

// HTTP client
HTTPClient http;

// ═══════════════════════════════════════════════════════════════════════════
// GPS READER TASK — runs on Core 0, independent of main loop
// ═══════════════════════════════════════════════════════════════════════════
// The main loop (Core 1) makes blocking HTTP calls (400-800ms each).
// During those blocks the GPS UART hardware buffer (128 bytes) overflows
// and sentences get corrupted. Running GPS reading on Core 0 solves this.

void gpsReaderTask(void* parameter) {
  for (;;) {
    // Drain every available byte immediately — same as diagnostic test
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }
    vTaskDelay(1); // yield for 1ms — keeps the task from starving other Core 0 work
  }
}

// ═══════════════════════════════════════════════════════════════════════════
// SETUP
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("\n╔════════════════════════════════════════════════════════════╗");
  Serial.println("║   VIGILERT MOTORCYCLE MODULE - COMPLETE WITH SMS ALERTS   ║");
  Serial.println("╠════════════════════════════════════════════════════════════╣");
  Serial.println("║ 🔧 FIXES: Dashboard button not working issue              ║");
  Serial.println("║ 📱 ADDS: SMS alerts for all emergency situations          ║");
  Serial.println("║ 📐 INTEGRATES: Real MPU6050 lean angle detection          ║");
  Serial.println("║ 🛡️ INCLUDES: All safety features with SMS notifications   ║");
  Serial.println("╚════════════════════════════════════════════════════════════╝\n");

  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(ignitionRelayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(vibrationSensorPin, INPUT);
  pinMode(GSM_POWER_PIN, OUTPUT);

  // ✅ CRITICAL: Initialize relays to BLOCK
  digitalWrite(relayPin, LOW);           // Starter BLOCKED
  digitalWrite(ignitionRelayPin, LOW);   // Ignition BLOCKED - BLOCKS PHYSICAL KEY
  
  engineRunning = false;
  engineAllowed = false;
  safetyShutdownActive = false;
  
  Serial.println("[SETUP] 🔒 ENHANCED SAFETY SYSTEM:");
  Serial.printf("[SETUP] Starter: GPIO %d = BLOCKED\n", relayPin);
  Serial.printf("[SETUP] Ignition: GPIO %d = BLOCKED\n", ignitionRelayPin);
  Serial.println("[SETUP] Physical key is BLOCKED until authorized");

  // ✅ INITIALIZE MPU6050
  initializeMPU6050();

  // ✅ INITIALIZE GPS
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);  // RX=16, TX=17
  Serial.println("[GPS] Initialized on RX:16, TX:17");

  // ✅ GPS TASK ON CORE 0 — runs independently of main loop
  // This ensures GPS bytes are NEVER dropped even when HTTP calls block core 1
  xTaskCreatePinnedToCore(
    gpsReaderTask,   // function
    "GPS_Reader",    // name
    2048,            // stack size
    NULL,            // parameter
    1,               // priority
    NULL,            // handle
    0                // core 0 (main loop runs on core 1)
  );
  Serial.println("[GPS] Reader task started on Core 0");

  // ✅ INITIALIZE GSM MODULE
  initializeGSM();

  // Connect to WiFi
  connectToWiFi();
  
  if (WiFi.status() == WL_CONNECTED) {
    testFirebaseConnection();
    loadSpeedLimit();
    loadEmergencyContact();
    
    // ✅ FORCE IMMEDIATE CONNECTION TO DASHBOARD
    Serial.println("[SETUP] 🔄 Establishing dashboard connection...");
    for (int i = 0; i < 3; i++) {
      Serial.printf("Connection heartbeat %d/3...\n", i + 1);
      sendDashboardHeartbeat();
      delay(500);
    }
    Serial.println("[SETUP] ✅ Dashboard connection established!");
  }

  Serial.println("\n🚀 COMPLETE SYSTEM WITH SMS ALERTS READY!");
  Serial.println("🛡️ SAFETY FEATURES:");
  Serial.println("   • Real MPU6050 lean angle detection");
  Serial.println("   • Alcohol detection → Auto engine shutdown + SMS");
  Serial.println("   • Lean angle >40° → Auto engine shutdown + SMS");
  Serial.println("   • Crash detection → Emergency SMS with location");
  Serial.println("   • Anti-theft → Movement detection SMS");
  Serial.println("   • Speed monitoring → Over-speed SMS alerts");
  Serial.println("📋 Commands: START, STOP, STATUS, TEST SMS, SET PHONE");
  Serial.println("🖥️ Dashboard Control: Engine START/STOP buttons with enhanced detection");
  Serial.println("📱 SMS Alerts: All emergency situations notify emergency contact\n");
}

// ═══════════════════════════════════════════════════════════════════════════
// GSM/SMS INITIALIZATION AND FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void initializeGSM() {
  Serial.println("[GSM] Initializing SIM800L module...");
  
  // Initialize GSM serial
  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX_PIN, GSM_TX_PIN);
  
  // Power cycle GSM module
  digitalWrite(GSM_POWER_PIN, LOW);
  delay(1000);
  digitalWrite(GSM_POWER_PIN, HIGH);
  delay(2000);
  digitalWrite(GSM_POWER_PIN, LOW);
  
  Serial.println("[GSM] Waiting for module to initialize...");
  delay(5000);
  
  // Test GSM communication
  if (testGSMConnection()) {
    gsmInitialized = true;
    Serial.println("[GSM] ✅ SIM800L initialized successfully!");
    
    // Send initialization SMS
    if (!emergencyPhoneNumber.isEmpty()) {
      sendSMS("VIGILERT System Initialized. Motorcycle security system is now active.", "SYSTEM_INIT");
    }
  } else {
    gsmInitialized = false;
    Serial.println("[GSM] ❌ SIM800L initialization failed!");
    Serial.println("[GSM] Check wiring: RX→GPIO25, TX→GPIO26, VCC→4V, GND→GND");
  }
}

bool testGSMConnection() {
  Serial.println("[GSM] Testing AT communication...");
  
  // Clear serial buffer
  while (gsmSerial.available()) {
    gsmSerial.read();
  }
  
  // Send AT command
  gsmSerial.println("AT");
  
  unsigned long startTime = millis();
  String response = "";
  
  while (millis() - startTime < 5000) {
    if (gsmSerial.available()) {
      response += gsmSerial.readString();
      if (response.indexOf("OK") != -1) {
        Serial.println("[GSM] ✅ AT communication successful");
        
        // Check SIM card
        gsmSerial.println("AT+CPIN?");
        delay(1000);
        
        response = "";
        startTime = millis();
        while (millis() - startTime < 3000) {
          if (gsmSerial.available()) {
            response += gsmSerial.readString();
            break;
          }
        }
        
        if (response.indexOf("READY") != -1) {
          Serial.println("[GSM] ✅ SIM card ready");
          
          // Set SMS text mode
          gsmSerial.println("AT+CMGF=1");
          delay(1000);
          
          return true;
        } else {
          Serial.print("[GSM] ❌ SIM card not ready: ");
          Serial.println(response);
          return false;
        }
      }
    }
  }
  
  Serial.println("[GSM] ❌ No response to AT command");
  return false;
}

void loadEmergencyContact() {
  if (WiFi.status() != WL_CONNECTED) return;

  Serial.println("[SMS] Loading emergency contact...");
  
  http.begin(firebaseHost + emergencyContactPath);
  http.setTimeout(3000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    
    // Parse phone number from JSON
    int startIdx = response.indexOf("\"phoneNumber\":\"");
    if (startIdx != -1) {
      startIdx += 15; // Length of "phoneNumber":"
      int endIdx = response.indexOf("\"", startIdx);
      if (endIdx != -1) {
        emergencyPhoneNumber = response.substring(startIdx, endIdx);
        Serial.printf("[SMS] Emergency contact loaded: %s\n", emergencyPhoneNumber.c_str());
      }
    }
    
    if (emergencyPhoneNumber.isEmpty()) {
      Serial.println("[SMS] ⚠️ No emergency contact found in database");
    }
  } else {
    Serial.printf("[SMS] ❌ Failed to load emergency contact: HTTP %d\n", httpCode);
  }
  
  http.end();
}

bool sendSMS(String message, String alertType) {
  if (!gsmInitialized || emergencyPhoneNumber.isEmpty()) {
    Serial.printf("[SMS] ❌ Cannot send SMS - GSM:%s, Phone:%s\n", 
                  gsmInitialized ? "OK" : "FAIL", 
                  emergencyPhoneNumber.isEmpty() ? "EMPTY" : "OK");
    return false;
  }
  
  // Check SMS cooldown to prevent spam
  if (millis() - lastSMSTime < SMS_COOLDOWN) {
    Serial.printf("[SMS] ⏱️ SMS cooldown active (%lu seconds remaining)\n", 
                  (SMS_COOLDOWN - (millis() - lastSMSTime)) / 1000);
    return false;
  }
  
  Serial.printf("[SMS] 📱 Sending %s SMS to %s\n", alertType.c_str(), emergencyPhoneNumber.c_str());
  
  // Clear GSM serial buffer
  while (gsmSerial.available()) {
    gsmSerial.read();
  }
  
  // Set SMS text mode
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  
  // Set phone number
  gsmSerial.println("AT+CMGS=\"" + emergencyPhoneNumber + "\"");
  delay(1000);
  
  // Send message
  gsmSerial.print(message);
  delay(100);
  gsmSerial.write(26); // Ctrl+Z to send SMS
  
  // Wait for response
  unsigned long startTime = millis();
  String response = "";
  bool smsSent = false;
  
  while (millis() - startTime < SMS_TIMEOUT) {
    if (gsmSerial.available()) {
      response += gsmSerial.readString();
      if (response.indexOf("+CMGS:") != -1 || response.indexOf("OK") != -1) {
        smsSent = true;
        break;
      }
      if (response.indexOf("ERROR") != -1) {
        break;
      }
    }
  }
  
  if (smsSent) {
    lastSMSTime = millis();
    Serial.printf("[SMS] ✅ %s SMS sent successfully\n", alertType.c_str());
    
    // Log SMS to Firebase
    logSMSToFirebase(message, alertType);
    
    return true;
  } else {
    Serial.printf("[SMS] ❌ Failed to send %s SMS: %s\n", alertType.c_str(), response.c_str());
    return false;
  }
}

void logSMSToFirebase(String message, String alertType) {
  if (WiFi.status() != WL_CONNECTED) return;

  StaticJsonDocument<300> smsDoc;
  smsDoc["message"] = message;
  smsDoc["alertType"] = alertType;
  smsDoc["phoneNumber"] = emergencyPhoneNumber;
  smsDoc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  smsDoc["time"] = getCurrentTimeString();
  smsDoc["location"]["lat"] = currentLat;
  smsDoc["location"]["lng"] = currentLng;
  smsDoc["gpsValid"] = gpsValid;
  
  String smsPayload;
  serializeJson(smsDoc, smsPayload);
  
  String smsPath = "/helmet_public/" + userUID + "/smsLogs/" + String(millis()) + ".json";
  
  http.begin(firebaseHost + smsPath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000);
  
  int httpCode = http.PUT(smsPayload);
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("[SMS] ✅ SMS logged to Firebase");
  }
  
  http.end();
}

// ═══════════════════════════════════════════════════════════════════════════
// MPU6050 INITIALIZATION (from working test code)
// ═══════════════════════════════════════════════════════════════════════════

void initializeMPU6050() {
  Serial.println("[MPU6050] Initializing real sensor...");
  
  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.printf("[I2C] Initialized - SDA: GPIO%d, SCL: GPIO%d\n", SDA_PIN, SCL_PIN);

  // Initialize MPU6050
  mpu.initialize();
  
  // Test connection
  if (mpu.testConnection()) {
    Serial.println("[MPU6050] ✅ Connection successful!");
    
    // Get device ID
    uint8_t deviceId = mpu.getDeviceID();
    Serial.printf("[MPU6050] Device ID: 0x%02X\n", deviceId);
    
    // Configure sensor settings
    configureMPU6050();
    
    // Calibrate sensor
    calibrateMPU6050();
    
    mpuInitialized = true;
    Serial.println("[MPU6050] ✅ Real lean angle detection ACTIVE");
    
  } else {
    Serial.println("[MPU6050] ❌ Connection failed!");
    Serial.println("[MPU6050] Check wiring: VCC→3.3V, GND→GND, SDA→21, SCL→22");
    Serial.println("[MPU6050] ⚠️ Falling back to simulated lean angle data");
    mpuInitialized = false;
  }
}

void configureMPU6050() {
  // Set accelerometer range to ±2g (most sensitive)
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
  
  // Set gyroscope range to ±250°/s (most sensitive)
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  
  // Set digital low pass filter
  mpu.setDLPFMode(MPU6050_DLPF_BW_20);
  
  // Set sample rate to 100Hz
  mpu.setRate(9);
  
  Serial.println("[MPU6050] Configuration: ±2g, ±250°/s, 20Hz filter, 100Hz rate");
}

void calibrateMPU6050() {
  Serial.println("[MPU6050] Starting calibration...");
  Serial.println("[MPU6050] ⚠️ Ensure motorcycle is UPRIGHT and STATIONARY!");
  Serial.println("[MPU6050] Calibrating in 3 seconds...");
  
  for (int i = 3; i > 0; i--) {
    Serial.printf("[MPU6050] %d...\n", i);
    delay(1000);
  }
  
  Serial.println("[MPU6050] Calibrating... (5 seconds)");
  
  float rollSum = 0.0;
  float pitchSum = 0.0;
  int samples = 0;
  
  unsigned long calibrationStart = millis();
  while (millis() - calibrationStart < 5000) { // 5 seconds
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    // Calculate raw angles
    float rawRoll = atan2(ay, az) * 180.0 / PI;
    float rawPitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
    
    rollSum += rawRoll;
    pitchSum += rawPitch;
    samples++;
    
    delay(50);
  }
  
  // Calculate offsets
  rollOffset = rollSum / samples;
  pitchOffset = pitchSum / samples;
  
  Serial.printf("[MPU6050] ✅ Calibration complete! Samples: %d\n", samples);
  Serial.printf("[MPU6050] Roll offset: %.2f°\n", rollOffset);
  Serial.printf("[MPU6050] Pitch offset: %.2f°\n", pitchOffset);
  Serial.println("[MPU6050] Upright position = 0° lean angle");
}
// ═══════════════════════════════════════════════════════════════════════════
// MAIN LOOP
// ═══════════════════════════════════════════════════════════════════════════

void loop() {
  unsigned long currentTime = millis();
  
  // ✅ CRITICAL: SAFETY CHECKS (highest priority)
  if (currentTime - lastSafetyCheck >= SAFETY_CHECK_INTERVAL) {
    performSafetyChecks();
    lastSafetyCheck = currentTime;
  }
  
  // ✅ Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi disconnected! Reconnecting...");
    connectToWiFi();
    return;
  }
  
  // ✅ DASHBOARD HEARTBEAT - High frequency for real-time updates
  if (currentTime - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    sendDashboardHeartbeat();
    lastHeartbeat = currentTime;
  }

  // ✅ ENHANCED DASHBOARD BUTTON CHECK - Fixed for better detection
  if (currentTime - lastButtonCheck >= BUTTON_CHECK_INTERVAL) {
    checkDashboardButtonWithSafety();
    lastButtonCheck = currentTime;
  }

  // ✅ GPS UPDATE — drain serial buffer every loop, not just every 1000ms
  // GPS sends data continuously; missing bytes causes parse failures
  updateGPSData();

  // Full GPS status refresh every 1 second
  if (currentTime - lastGPSUpdate >= GPS_UPDATE_INTERVAL) {
    lastGPSUpdate = currentTime;
  }

  // ✅ SENSOR READINGS - NOW WITH REAL MPU6050
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    readAllSensors();
    lastSensorRead = currentTime;
  }

  // ✅ LIVE DATA UPDATE - For both dashboards
  if (currentTime - lastLiveDataUpdate >= LIVE_DATA_INTERVAL) {
    updateLiveDataForDashboards();
    lastLiveDataUpdate = currentTime;
  }

  // ✅ CRASH DETECTION WITH SMS
  processCrashDetection();

  // ✅ ANTI-THEFT SYSTEM WITH SMS
  processAntiTheft();

  // ✅ TRIP TRACKING
  processTripTracking();

  // ✅ SPEED MONITORING WITH SMS
  processSpeedMonitoring();

  // Handle serial commands
  handleSerialCommands();

  yield();
}

// ═══════════════════════════════════════════════════════════════════════════
// ENHANCED SAFETY SYSTEM WITH SMS ALERTS
// ═══════════════════════════════════════════════════════════════════════════

void performSafetyChecks() {
  // ✅ CHECK 1: ALCOHOL DETECTION FROM HELMET WITH SMS
  checkAlcoholSafety();
  
  // ✅ CHECK 2: REAL LEAN ANGLE DETECTION FROM MPU6050 WITH SMS
  checkLeanAngleSafety();
  
  // ✅ CHECK 3: HELMET CONNECTION STATUS
  checkHelmetConnection();
  
  // ✅ CHECK 4: SAFETY RECOVERY CONDITIONS
  checkSafetyRecovery();
}

void checkLeanAngleSafety() {
  // ✅ REAL MPU6050 LEAN ANGLE DETECTION (from working test code)
  if (mpuInitialized) {
    // Read real sensor data
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    // Calculate real angles (same as working test code)
    roll = atan2(ay, az) * 180.0 / PI - rollOffset;
    pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI - pitchOffset;
    
    // Calculate lean angle (absolute value of roll for motorcycle lean)
    currentLeanAngle = abs(roll);
    currentRoll = roll;
    currentPitch = pitch;
    
    // Calculate total acceleration
    currentTotalAccel = sqrt((ax/16384.0)*(ax/16384.0) + (ay/16384.0)*(ay/16384.0) + (az/16384.0)*(az/16384.0)) * 9.8;
    
  } else {
    // Fallback to simulated data if MPU6050 not available
    static unsigned long lastLeanUpdate = 0;
    if (millis() - lastLeanUpdate > 100) {
      static float simulatedLean = 0.0;
      
      if (engineRunning) {
        simulatedLean = random(0, 30) + (sin(millis() / 1000.0) * 5);
      } else {
        simulatedLean = random(0, 5);
      }
      
      currentLeanAngle = abs(simulatedLean);
      currentRoll = simulatedLean;
      currentPitch = random(-5, 5);
      lastLeanUpdate = millis();
    }
  }
  
  // ✅ LEAN ANGLE DETECTION LOGIC (40° threshold)
  if (currentLeanAngle >= LEAN_ANGLE_THRESHOLD && !leanAngleExceeded) {
    // Lean angle just exceeded
    leanAngleExceeded = true;
    leanAngleStartTime = millis();
    
    Serial.printf("\n⚠️ EXCESSIVE LEAN ANGLE! (%.1f° > %.1f°)\n", currentLeanAngle, LEAN_ANGLE_THRESHOLD);
    Serial.printf("⏱️ Recovery time: %.1f seconds...\n", LEAN_ANGLE_TIMEOUT / 1000.0);
    
    playAlertSound(3, 150);
    sendDashboardAlert("⚠️ Excessive lean angle detected: " + String(currentLeanAngle, 1) + "°", "danger");
    
    // ✅ SEND SMS ALERT FOR EXCESSIVE LEAN ANGLE
    String leanSMSMessage = "VIGILERT ALERT: Excessive lean angle detected (" + String(currentLeanAngle, 1) + "°). ";
    if (gpsValid) {
      leanSMSMessage += "Location: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
    } else {
      leanSMSMessage += "GPS location unavailable.";
    }
    sendSMS(leanSMSMessage, "LEAN_ANGLE");
  }
  
  // ✅ WARNING THRESHOLD
  if (currentLeanAngle >= LEAN_ANGLE_WARNING && !leanAngleWarning && !leanAngleExceeded) {
    leanAngleWarning = true;
    Serial.printf("\n⚠️ LEAN ANGLE WARNING! (%.1f° > %.1f°)\n", currentLeanAngle, LEAN_ANGLE_WARNING);
    sendDashboardAlert("⚠️ Lean angle warning: " + String(currentLeanAngle, 1) + "°", "warning");
  }
  
  // ✅ AUTOMATIC ENGINE SHUTDOWN ON EXCESSIVE LEAN
  if (leanAngleExceeded && engineRunning) {
    unsigned long timeSinceLean = millis() - leanAngleStartTime;
    
    // Check if still leaning after timeout
    if (timeSinceLean >= LEAN_ANGLE_TIMEOUT && currentLeanAngle >= LEAN_ANGLE_THRESHOLD) {
      Serial.printf("\n🛑 AUTOMATIC ENGINE SHUTDOWN - EXCESSIVE LEAN ANGLE! (%.1f°)\n", currentLeanAngle);
      
      safetyShutdownActive = true;
      shutdownReason = "Excessive Lean Angle";
      lastEngineShutdown = millis();
      
      // Force engine stop
      forceEngineShutdown("LEAN_ANGLE_EXCEEDED");
      
      // Mark as crash detected
      crashDetected = true;
      sendCrashDataToDashboards();
      
      // ✅ SEND EMERGENCY SMS FOR AUTOMATIC SHUTDOWN
      String shutdownSMSMessage = "VIGILERT EMERGENCY: Automatic engine shutdown due to excessive lean angle (" + String(currentLeanAngle, 1) + "°). Possible crash detected. ";
      if (gpsValid) {
        shutdownSMSMessage += "Location: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
      } else {
        shutdownSMSMessage += "GPS location unavailable.";
      }
      sendSMS(shutdownSMSMessage, "EMERGENCY_SHUTDOWN");
    }
  }
  
  // ✅ LEAN ANGLE RECOVERED
  if (leanAngleExceeded && currentLeanAngle < (LEAN_ANGLE_THRESHOLD - 5.0)) {
    leanAngleExceeded = false;
    Serial.printf("\n✅ LEAN ANGLE RECOVERED! (%.1f°)\n", currentLeanAngle);
    sendDashboardAlert("✅ Lean angle normalized: " + String(currentLeanAngle, 1) + "°", "success");
  }
  
  // ✅ WARNING CLEARED
  if (leanAngleWarning && currentLeanAngle < (LEAN_ANGLE_WARNING - 5.0)) {
    leanAngleWarning = false;
    Serial.printf("\n✅ LEAN ANGLE WARNING CLEARED! (%.1f°)\n", currentLeanAngle);
  }
}

void checkAlcoholSafety() {
  // Throttle: only check alcohol every 2 seconds to avoid HTTP congestion
  static unsigned long lastAlcoholCheck = 0;
  if (millis() - lastAlcoholCheck < 2000) return;
  lastAlcoholCheck = millis();

  // Get alcohol status from helmet module via Firebase
  if (WiFi.status() != WL_CONNECTED) return;

  http.begin(firebaseHost + alcoholPath);
  http.setTimeout(3000);
  
  int httpCode = http.GET();
  Serial.printf("[ALCOHOL] HTTP %d | engineRunning:%s | alcoholDetected:%s\n",
                httpCode, engineRunning ? "YES" : "NO", alcoholDetected ? "YES" : "NO");

  if (httpCode == 200) {
    String response = http.getString();
    
    // Parse alcohol status
    bool currentAlcoholDetected = (response.indexOf("\"Danger\"") != -1);
    
    // Extract sensor value if available
    int startIdx = response.indexOf("\"sensorValue\":");
    if (startIdx != -1) {
      startIdx += 14;
      int endIdx = response.indexOf(",", startIdx);
      if (endIdx == -1) endIdx = response.indexOf("}", startIdx);
      if (endIdx != -1) {
        alcoholSensorValue = response.substring(startIdx, endIdx).toInt();
      }
    }

    Serial.printf("[ALCOHOL] Status: %s | Value: %d\n",
                  currentAlcoholDetected ? "DANGER" : "SAFE", alcoholSensorValue);
    
    // ✅ ALCOHOL DETECTION LOGIC WITH SMS
    if (currentAlcoholDetected && !alcoholDetected) {
      alcoholDetected = true;
      alcoholDetectedTime = millis();
      
      Serial.printf("\n🚨 ALCOHOL DETECTED! (Value: %d)\n", alcoholSensorValue);
      Serial.printf("⏱️ Engine shutdown in %.1f seconds...\n", ALCOHOL_SHUTDOWN_DELAY / 1000.0);
      
      playAlertSound(5, 200);
      sendDashboardAlert("🚨 ALCOHOL DETECTED - Engine shutdown imminent", "danger");
      
      String alcoholSMSMessage = "VIGILERT ALERT: Alcohol detected (Level: " + String(alcoholSensorValue) + "). Engine shutdown in 2 seconds. ";
      if (gpsValid) {
        alcoholSMSMessage += "Location: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
      } else {
        alcoholSMSMessage += "GPS location unavailable.";
      }
      sendSMS(alcoholSMSMessage, "ALCOHOL_DETECTED");
    }
    
    // ✅ AUTOMATIC ENGINE SHUTDOWN ON ALCOHOL
    if (alcoholDetected && engineRunning) {
      unsigned long timeSinceDetection = millis() - alcoholDetectedTime;
      Serial.printf("[ALCOHOL] Time since detection: %lums / %lums\n", timeSinceDetection, ALCOHOL_SHUTDOWN_DELAY);
      
      if (timeSinceDetection >= ALCOHOL_SHUTDOWN_DELAY) {
        Serial.println("\n🛑 AUTOMATIC ENGINE SHUTDOWN - ALCOHOL DETECTED!");
        
        safetyShutdownActive = true;
        shutdownReason = "Alcohol Detection";
        lastEngineShutdown = millis();
        
        forceEngineShutdown("ALCOHOL_DETECTED");
        
        String shutdownSMSMessage = "VIGILERT EMERGENCY: Engine automatically shut down due to alcohol detection (Level: " + String(alcoholSensorValue) + "). Rider may be impaired. ";
        if (gpsValid) {
          shutdownSMSMessage += "Location: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
        } else {
          shutdownSMSMessage += "GPS location unavailable.";
        }
        sendSMS(shutdownSMSMessage, "ALCOHOL_SHUTDOWN");
      }
    }
    
    // ✅ ALCOHOL CLEARED
    if (!currentAlcoholDetected && alcoholDetected) {
      alcoholDetected = false;
      Serial.printf("\n✅ ALCOHOL CLEARED! (Value: %d)\n", alcoholSensorValue);
      sendDashboardAlert("✅ Alcohol levels normalized", "success");
    }
  } else {
    Serial.printf("[ALCOHOL] ❌ Failed to read alcohol status: HTTP %d\n", httpCode);
  }
  
  http.end();
}

void checkHelmetConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(firebaseHost + helmetPath);
    http.setTimeout(1000);
    
    int httpCode = http.GET();
    if (httpCode == 200) {
      String response = http.getString();
      bool currentHelmetConnected = (response.indexOf("\"On\"") != -1);
      
      if (currentHelmetConnected != helmetConnected) {
        helmetConnected = currentHelmetConnected;
        
        if (helmetConnected) {
          Serial.println("[HELMET] ✅ Connected");
          sendDashboardAlert("Helmet connected", "success");
        } else {
          Serial.println("[HELMET] ❌ Disconnected");
          sendDashboardAlert("⚠️ Helmet disconnected - engine shutting down for safety", "danger");
          
          // ✅ FORCE ENGINE SHUTDOWN when helmet disconnects while riding
          if (engineRunning) {
            Serial.println("[HELMET] 🛑 Engine shutdown triggered by helmet disconnect");
            safetyShutdownActive = true;
            shutdownReason = "Helmet Disconnected";
            lastEngineShutdown = millis();
            forceEngineShutdown("HELMET_DISCONNECTED");
            
            String helmetSMSMessage = "VIGILERT ALERT: Helmet disconnected while riding. Engine automatically stopped for safety. ";
            if (gpsValid) {
              helmetSMSMessage += "Location: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
            }
            sendSMS(helmetSMSMessage, "HELMET_DISCONNECT");
          }
        }
      }
    }
    
    http.end();
  }
}

void checkSafetyRecovery() {
  bool canRestart = true;
  String blockingReasons = "";
  
  if (alcoholDetected) {
    canRestart = false;
    blockingReasons += "Alcohol detected; ";
  }
  
  if (leanAngleExceeded) {
    canRestart = false;
    blockingReasons += "Excessive lean angle; ";
  }
  
  // ✅ AUTOMATIC CRASH RECOVERY - Clear crash if conditions are stable
  if (crashDetected) {
    // Check if motorcycle has been stable for recovery period
    unsigned long timeSinceCrash = millis() - lastCrashTime;
    bool stableConditions = (currentLeanAngle < 15.0) && (currentTotalAccel < 12.0);
    
    if (timeSinceCrash > 10000 && stableConditions) { // 10 seconds stable
      crashDetected = false;
      Serial.println("\n✅ CRASH RECOVERY - Stable conditions detected, crash flag cleared");
      sendDashboardAlert("✅ Crash recovery - motorcycle stable, restart available", "success");
    } else {
      canRestart = false;
      if (timeSinceCrash < 10000) {
        blockingReasons += "Crash recovery (" + String((10000 - timeSinceCrash) / 1000) + "s); ";
      } else {
        blockingReasons += "Unstable conditions (lean:" + String(currentLeanAngle, 1) + "°, accel:" + String(currentTotalAccel, 1) + "g); ";
      }
    }
  }
  
  if (safetyShutdownActive && (millis() - lastEngineShutdown < ENGINE_RESTART_COOLDOWN)) {
    canRestart = false;
    blockingReasons += "Cooldown period; ";
  }
  
  if (safetyShutdownActive && canRestart) {
    safetyShutdownActive = false;
    shutdownReason = "";
    Serial.println("\n✅ SAFETY CONDITIONS NORMALIZED - Engine restart allowed");
    sendDashboardAlert("✅ Safety conditions normalized - engine restart available", "success");
  }
  
  engineAllowed = canRestart;
}

void forceEngineShutdown(String reason) {
  Serial.print("\n🚨 FORCE ENGINE SHUTDOWN: ");
  Serial.println(reason);
  
  digitalWrite(relayPin, LOW);
  digitalWrite(ignitionRelayPin, LOW);
  
  engineRunning = false;
  engineAllowed = false;
  riderStatus = "Safety Shutdown";
  
  for (int i = 0; i < 10; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
  
  sendDashboardHeartbeat();
  updateLiveDataForDashboards();
  
  Serial.printf("🛑 Engine shutdown complete - Reason: %s\n", reason.c_str());
}

// ═══════════════════════════════════════════════════════════════════════════
// WIFI & FIREBASE CONNECTION
// ═══════════════════════════════════════════════════════════════════════════

void connectToWiFi() {
  Serial.print("[WIFI] Connecting to: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
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
    Serial.printf("[WIFI] Signal: %d dBm\n", WiFi.RSSI());
  } else {
    Serial.println("\n[WIFI] ❌ Failed!");
  }
}

void testFirebaseConnection() {
  Serial.println("[FIREBASE] Testing dashboard integration...");
  
  http.begin(firebaseHost + devicePath);
  http.setTimeout(5000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("[FIREBASE] ✅ Dashboard integration ready!");
  } else {
    Serial.printf("[FIREBASE] ⚠️ HTTP %d - %s\n", httpCode, http.errorToString(httpCode).c_str());
  }
  
  http.end();
}

// ═══════════════════════════════════════════════════════════════════════════
// ENHANCED DASHBOARD INTEGRATION WITH BUTTON FIX
// ═══════════════════════════════════════════════════════════════════════════

void sendDashboardHeartbeat() {
  if (WiFi.status() != WL_CONNECTED) return;

  StaticJsonDocument<800> doc;
  doc["status"] = "On";
  doc["lastHeartbeat"] = (uint64_t)(1700000000000ULL + millis());
  doc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  
  // ✅ ENGINE STATUS
  doc["engineRunning"] = engineRunning;
  doc["engineAllowed"] = engineAllowed;
  doc["relayState"] = digitalRead(relayPin);
  doc["ignitionState"] = digitalRead(ignitionRelayPin);
  
  // ✅ ENHANCED SAFETY STATUS WITH REAL MPU6050 DATA
  doc["safetyShutdownActive"] = safetyShutdownActive;
  doc["shutdownReason"] = shutdownReason;
  doc["alcoholDetected"] = alcoholDetected;
  doc["alcoholSensorValue"] = alcoholSensorValue;
  doc["leanAngleExceeded"] = leanAngleExceeded;
  doc["leanAngleWarning"] = leanAngleWarning;
  doc["currentLeanAngle"] = currentLeanAngle;
  doc["mpuInitialized"] = mpuInitialized;
  
  // ✅ SMS STATUS
  doc["gsmInitialized"] = gsmInitialized;
  doc["emergencyContact"] = emergencyPhoneNumber;
  doc["smsEnabled"] = !emergencyPhoneNumber.isEmpty();
  
  // ✅ GPS DATA
  doc["gps"]["valid"] = gpsValid;
  doc["gps"]["latitude"] = currentLat;
  doc["gps"]["longitude"] = currentLng;
  doc["gps"]["speed"] = currentSpeed;
  doc["gps"]["satellites"] = satelliteCount;
  doc["gps"]["accuracy"] = gpsValid ? "5m" : "No Fix";
  
  // ✅ REAL SENSOR DATA FROM MPU6050
  doc["sensors"]["acceleration"] = currentTotalAccel;
  doc["sensors"]["roll"] = currentRoll;
  doc["sensors"]["pitch"] = currentPitch;
  doc["sensors"]["leanAngle"] = currentLeanAngle;
  doc["sensors"]["mpuConnected"] = mpuInitialized;
  doc["sensors"]["rawAccelX"] = mpuInitialized ? ax : 0;
  doc["sensors"]["rawAccelY"] = mpuInitialized ? ay : 0;
  doc["sensors"]["rawAccelZ"] = mpuInitialized ? az : 0;
  
  // ✅ SYSTEM STATUS
  doc["crashDetected"] = crashDetected;
  doc["helmetConnected"] = helmetConnected;
  doc["antiTheftArmed"] = antiTheftArmed;
  doc["vibrationSensor"] = digitalRead(vibrationSensorPin);
  
  // ✅ SYSTEM HEALTH
  doc["freeHeap"] = ESP.getFreeHeap();
  doc["wifiSignal"] = WiFi.RSSI();
  doc["uptime"] = millis();
  doc["connected"] = true;
  doc["deviceType"] = "motorcycle";
  doc["version"] = "complete_with_sms_v1.0";
  
  String payload;
  serializeJson(doc, payload);
  
  unsigned long startTime = millis();
  
  http.begin(firebaseHost + devicePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);
  
  int httpCode = http.PUT(payload);
  unsigned long duration = millis() - startTime;
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.printf("[DASHBOARD] ✅ %lums | Engine:%s | Lean:%.1f° | MPU:%s | SMS:%s\n", 
                  duration, 
                  engineRunning ? "ON" : "OFF",
                  currentLeanAngle,
                  mpuInitialized ? "OK" : "SIM",
                  gsmInitialized ? "OK" : "FAIL");
  } else {
    Serial.printf("[DASHBOARD] ❌ HTTP %d (%lums)\n", httpCode, duration);
  }
  
  http.end();
}

void updateLiveDataForDashboards() {
  if (WiFi.status() != WL_CONNECTED) return;

  StaticJsonDocument<700> liveDoc;
  
  // ✅ ENGINE & SAFETY STATUS
  liveDoc["engineRunning"] = engineRunning;
  liveDoc["engineAllowed"] = engineAllowed;
  liveDoc["crashDetected"] = crashDetected;
  liveDoc["antiTheftArmed"] = antiTheftArmed;
  liveDoc["alcoholDetected"] = alcoholDetected;
  liveDoc["helmetConnected"] = helmetConnected;
  
  // ✅ ENHANCED SAFETY DATA WITH REAL MPU6050
  liveDoc["safetyShutdownActive"] = safetyShutdownActive;
  liveDoc["shutdownReason"] = shutdownReason;
  liveDoc["leanAngleExceeded"] = leanAngleExceeded;
  liveDoc["leanAngleWarning"] = leanAngleWarning;
  liveDoc["currentLeanAngle"] = currentLeanAngle;
  liveDoc["alcoholSensorValue"] = alcoholSensorValue;
  liveDoc["mpuInitialized"] = mpuInitialized;
  
  // ✅ SMS STATUS
  liveDoc["gsmInitialized"] = gsmInitialized;
  liveDoc["smsEnabled"] = !emergencyPhoneNumber.isEmpty();
  
  // ✅ REAL SENSOR READINGS
  liveDoc["sensorData"]["roll"] = currentRoll;
  liveDoc["sensorData"]["pitch"] = currentPitch;
  liveDoc["sensorData"]["leanAngle"] = currentLeanAngle;
  liveDoc["sensorData"]["acceleration"] = currentTotalAccel;
  liveDoc["sensorData"]["mpuStatus"] = mpuInitialized ? "Connected" : "Simulated";
  
  // ✅ LOCATION & SPEED
  liveDoc["locationLat"] = currentLat;
  liveDoc["locationLng"] = currentLng;
  liveDoc["speed"] = currentSpeed;
  liveDoc["gpsValid"] = gpsValid;
  liveDoc["isOverSpeed"] = isOverSpeed;
  
  // ✅ RIDER STATUS WITH SAFETY INFO
  if (safetyShutdownActive) {
    liveDoc["riderStatus"] = "Safety Shutdown";
  } else if (alcoholDetected) {
    liveDoc["riderStatus"] = "Alcohol Detected";
  } else if (leanAngleExceeded) {
    liveDoc["riderStatus"] = "Excessive Lean";
  } else if (leanAngleWarning) {
    liveDoc["riderStatus"] = "Lean Warning";
  } else {
    liveDoc["riderStatus"] = riderStatus;
  }
  
  liveDoc["alertnessStatus"] = alertnessStatus;
  liveDoc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  
  String livePayload;
  serializeJson(liveDoc, livePayload);
  
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000);
  
  int httpCode = http.PUT(livePayload);
  
  if (httpCode != 200 && httpCode != HTTP_CODE_OK) {
    Serial.printf("[LIVE DATA] ❌ HTTP %d\n", httpCode);
  }
  
  http.end();
}

void checkDashboardButtonWithSafety() {
  if (WiFi.status() != WL_CONNECTED) return;

  http.begin(firebaseHost + buttonPath);
  http.setTimeout(3000); // ✅ INCREASED TIMEOUT FOR BETTER RELIABILITY
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    
    // ✅ ENHANCED BUTTON DETECTION - More robust parsing
    bool buttonPressed = false;
    
    // Check for various true values
    if (response.indexOf("true") != -1 || 
        response.indexOf("\"true\"") != -1 || 
        response.indexOf(":true") != -1) {
      buttonPressed = true;
    }
    
    // Debug button state
    static unsigned long lastButtonDebug = 0;
    if (millis() - lastButtonDebug > 5000) { // Debug every 5 seconds
      Serial.printf("[BUTTON] Response: %s | Pressed: %s\n", 
                    response.c_str(), buttonPressed ? "YES" : "NO");
      lastButtonDebug = millis();
    }
    
    static bool lastButtonState = false;
    if (buttonPressed && !lastButtonState) {
      Serial.println("\n🖥️ DASHBOARD BUTTON PRESSED!");
      
      if (!engineRunning) {
        if (validateSafetyConditions()) {
          Serial.println("🚀 Dashboard requesting ENGINE START - Safety validated");
          startEngine();
          sendDashboardAlert("Engine started via dashboard", "success");
        } else {
          Serial.println("🚫 ENGINE START BLOCKED - Safety conditions not met");
          String reason = getSafetyBlockingReasons();
          sendDashboardAlert("🚫 Engine start blocked: " + reason, "danger");
          
          // ✅ SEND SMS FOR BLOCKED ENGINE START
          String blockSMSMessage = "VIGILERT ALERT: Engine start blocked via dashboard. Reason: " + reason + ". ";
          if (gpsValid) {
            blockSMSMessage += "Location: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
          }
          sendSMS(blockSMSMessage, "ENGINE_BLOCKED");
        }
      } else {
        Serial.println("🛑 Dashboard requesting ENGINE STOP");
        stopEngine();
        sendDashboardAlert("Engine stopped via dashboard", "info");
      }
      
      // ✅ CLEAR BUTTON IMMEDIATELY WITH RETRY
      for (int retry = 0; retry < 3; retry++) {
        http.end();
        http.begin(firebaseHost + buttonPath);
        http.addHeader("Content-Type", "application/json");
        http.setTimeout(3000);
        int clearResult = http.PUT("false");
        
        if (clearResult == 200 || clearResult == HTTP_CODE_OK) {
          Serial.println("[BUTTON] ✅ Button cleared successfully");
          break;
        } else {
          Serial.printf("[BUTTON] ⚠️ Button clear attempt %d failed: HTTP %d\n", retry + 1, clearResult);
          delay(100);
        }
      }
      
      // ✅ FORCE IMMEDIATE DASHBOARD UPDATE
      sendDashboardHeartbeat();
      updateLiveDataForDashboards();
      
      lastButtonState = true;
    } else if (!buttonPressed) {
      lastButtonState = false;
    }
  } else {
    Serial.printf("[BUTTON] ❌ HTTP %d - %s\n", httpCode, http.errorToString(httpCode).c_str());
  }
  
  http.end();
}

bool validateSafetyConditions() {
  if (alcoholDetected) {
    Serial.println("[SAFETY] ❌ Alcohol detected");
    return false;
  }
  
  if (leanAngleExceeded) {
    Serial.println("[SAFETY] ❌ Excessive lean angle");
    return false;
  }
  
  if (crashDetected) {
    Serial.println("[SAFETY] ❌ Crash detected");
    return false;
  }
  
  if (safetyShutdownActive) {
    Serial.println("[SAFETY] ❌ Safety shutdown active");
    return false;
  }
  
  if (millis() - lastEngineShutdown < ENGINE_RESTART_COOLDOWN) {
    Serial.printf("[SAFETY] ❌ Cooldown period (%lu ms remaining)\n", 
                  ENGINE_RESTART_COOLDOWN - (millis() - lastEngineShutdown));
    return false;
  }
  
  Serial.println("[SAFETY] ✅ All conditions safe for engine start");
  return true;
}

String getSafetyBlockingReasons() {
  String reasons = "";
  
  if (alcoholDetected) {
    reasons += "Alcohol detected (" + String(alcoholSensorValue) + "); ";
  }
  
  if (leanAngleExceeded) {
    reasons += "Excessive lean angle (" + String(currentLeanAngle, 1) + "°); ";
  }
  
  if (crashDetected) {
    unsigned long timeSinceCrash = millis() - lastCrashTime;
    bool stableConditions = (currentLeanAngle < 15.0) && (currentTotalAccel < 12.0);
    
    if (timeSinceCrash < 10000) {
      reasons += "Crash recovery (" + String((10000 - timeSinceCrash) / 1000) + "s remaining); ";
    } else if (!stableConditions) {
      reasons += "Unstable conditions (lean:" + String(currentLeanAngle, 1) + "°, accel:" + String(currentTotalAccel, 1) + "g); ";
    } else {
      reasons += "Crash detected (use RESET SAFETY to clear); ";
    }
  }
  
  if (safetyShutdownActive) {
    reasons += "Safety shutdown (" + shutdownReason + "); ";
  }
  
  if (millis() - lastEngineShutdown < ENGINE_RESTART_COOLDOWN) {
    unsigned long remaining = ENGINE_RESTART_COOLDOWN - (millis() - lastEngineShutdown);
    reasons += "Cooldown (" + String(remaining / 1000) + "s); ";
  }
  
  if (reasons.length() > 0) {
    reasons = reasons.substring(0, reasons.length() - 2);
  }
  
  return reasons;
}

// ═══════════════════════════════════════════════════════════════════════════
// ENGINE CONTROL FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void startEngine() {
  Serial.println("\n✅ STARTING ENGINE - Safety validated");
  
  digitalWrite(relayPin, HIGH);
  digitalWrite(ignitionRelayPin, HIGH);
  
  engineRunning = true;
  riderStatus = "Active";
  
  playAlertSound(2, 100);
  
  Serial.println("🚀 Engine started successfully!");
  Serial.println("🔓 Ignition relay: ENABLED (Physical key now works)");
  Serial.println("🔓 Starter relay: ENABLED");
  
  // ✅ SEND SMS FOR ENGINE START
  String startSMSMessage = "VIGILERT: Engine started successfully. ";
  if (gpsValid) {
    startSMSMessage += "Location: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
  } else {
    startSMSMessage += "GPS location unavailable.";
  }
  sendSMS(startSMSMessage, "ENGINE_START");
}

void stopEngine() {
  Serial.println("\n🛑 STOPPING ENGINE");
  
  digitalWrite(relayPin, LOW);
  digitalWrite(ignitionRelayPin, LOW);
  
  engineRunning = false;
  riderStatus = "Inactive";
  
  if (tripActive) {
    endTrip();
  }
  
  playAlertSound(1, 200);
  
  Serial.println("🔒 Engine stopped successfully!");
  Serial.println("🔒 Ignition relay: DISABLED (Physical key blocked)");
  Serial.println("🔒 Starter relay: DISABLED");
  
  // ✅ SEND SMS FOR ENGINE STOP
  String stopSMSMessage = "VIGILERT: Engine stopped. ";
  if (gpsValid) {
    stopSMSMessage += "Location: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
  } else {
    stopSMSMessage += "GPS location unavailable.";
  }
  sendSMS(stopSMSMessage, "ENGINE_STOP");
}

// ═══════════════════════════════════════════════════════════════════════════
// SENSOR READING FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void readAllSensors() {
  // ✅ VIBRATION SENSOR
  int vibrationReading = digitalRead(vibrationSensorPin);
  if (vibrationReading == HIGH) {
    vibrationCount++;
  }
  
  // MPU6050 readings are handled in checkLeanAngleSafety()
  // GPS readings are handled in updateGPSData()
}

void updateGPSData() {
  // ── GPS bytes are now read by gpsReaderTask on Core 0 ──────────────────
  // This function only extracts the already-parsed values from TinyGPS++.
  // No serial reading here — the task handles that continuously.

  if (gps.location.isValid()) {
    currentLat = gps.location.lat();
    currentLng = gps.location.lng();
    gpsValid   = true;
  }

  if (gps.speed.isValid()) {
    currentSpeed = gps.speed.kmph();
  }

  if (gps.satellites.isValid()) {
    satelliteCount = gps.satellites.value();
  }

  // Invalidate if the last fix is older than 15 seconds
  if (gpsValid && gps.location.age() > 15000) {
    gpsValid = false;
    Serial.println("[GPS] ⚠️ Fix lost — age > 15s");
  }

  // Debug every 5 seconds — same format as diagnostic test
  static unsigned long lastGPSDebug = 0;
  if (millis() - lastGPSDebug > 5000) {
    lastGPSDebug = millis();

    if (gps.charsProcessed() == 0) {
      Serial.println("[GPS] 🔴 NO DATA — check wiring: GPS TX → GPIO16, VCC=3.3V, GND");
    } else if (gps.failedChecksum() > 10 && gps.sentencesWithFix() == 0) {
      Serial.println("[GPS] 🟡 DATA but checksum errors — possible baud rate mismatch");
    } else {
      Serial.printf("[GPS] Valid:%s | Sats:%d | Speed:%.1f km/h | Lat:%.6f | Lng:%.6f | Chars:%lu | Sentences:%lu | Fails:%lu\n",
                    gpsValid ? "YES" : "NO",
                    satelliteCount,
                    currentSpeed,
                    currentLat,
                    currentLng,
                    gps.charsProcessed(),
                    gps.sentencesWithFix(),
                    gps.failedChecksum());
    }
  }
}

// ═══════════════════════════════════════════════════════════════════════════
// CRASH DETECTION WITH SMS ALERTS
// ═══════════════════════════════════════════════════════════════════════════

void processCrashDetection() {
  if (crashDetected && (millis() - lastCrashTime < CRASH_COOLDOWN)) {
    return; // Still in cooldown
  }
  
  bool crashCondition = false;
  String crashReason = "";
  
  // ✅ METHOD 1: EXCESSIVE LEAN ANGLE (from MPU6050)
  if (currentLeanAngle >= LEAN_ANGLE_CRITICAL) {
    crashCondition = true;
    crashReason = "Excessive lean angle (" + String(currentLeanAngle, 1) + "°)";
  }
  
  // ✅ METHOD 2: HIGH G-FORCE IMPACT
  if (currentTotalAccel >= CRASH_ACCEL_THRESHOLD) {
    crashCondition = true;
    if (crashReason.length() > 0) crashReason += " + ";
    crashReason += "High impact (" + String(currentTotalAccel, 1) + "g)";
  }
  
  // ✅ METHOD 3: VIBRATION PATTERN ANALYSIS
  static unsigned long lastVibrationReset = 0;
  if (millis() - lastVibrationReset > 1000) {
    if (vibrationCount > 10 && engineRunning) { // High vibration while running
      crashCondition = true;
      if (crashReason.length() > 0) crashReason += " + ";
      crashReason += "Abnormal vibration (" + String(vibrationCount) + " hits/s)";
    }
    vibrationCount = 0;
    lastVibrationReset = millis();
  }
  
  if (crashCondition && !crashDetected) {
    crashDetected = true;
    lastCrashTime = millis();
    
    Serial.print("\n🚨 CRASH DETECTED! Reason: ");
    Serial.println(crashReason);
    
    playAlertSound(10, 100);
    sendCrashDataToDashboards();
    
    // ✅ SEND EMERGENCY SMS FOR CRASH DETECTION
    String crashSMSMessage = "VIGILERT EMERGENCY: CRASH DETECTED! Reason: " + crashReason + ". ";
    if (gpsValid) {
      crashSMSMessage += "Location: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6) + " ";
      crashSMSMessage += "Speed: " + String(currentSpeed, 1) + " km/h. ";
    } else {
      crashSMSMessage += "GPS location unavailable. ";
    }
    crashSMSMessage += "Please check on rider immediately!";
    
    sendSMS(crashSMSMessage, "CRASH_EMERGENCY");
    
    if (engineRunning) {
      forceEngineShutdown("CRASH_DETECTED");
    }
  }
}

void sendCrashDataToDashboards() {
  if (WiFi.status() != WL_CONNECTED) return;

  unsigned long crashTimestamp = (uint64_t)(1700000000000ULL + millis());

  StaticJsonDocument<500> crashDoc;
  crashDoc["detected"]       = true;
  crashDoc["timestamp"]      = crashTimestamp;
  crashDoc["time"]           = getCurrentTimeString();

  // ── Location fields — flat structure matching dashboard listener ──────
  crashDoc["hasGPS"]         = gpsValid;
  crashDoc["lat"]            = gpsValid ? currentLat : 0.0;
  crashDoc["lng"]            = gpsValid ? currentLng : 0.0;
  crashDoc["gpsValid"]       = gpsValid;

  // ── Impact data — field names matching dashboard listener ─────────────
  crashDoc["impactStrength"] = String(currentTotalAccel, 2);  // "g" value as string
  crashDoc["roll"]           = currentRoll;
  crashDoc["leanAngle"]      = currentLeanAngle;
  crashDoc["acceleration"]   = currentTotalAccel;
  crashDoc["speed"]          = currentSpeed;
  crashDoc["engineRunning"]  = engineRunning;
  crashDoc["severity"]       = (currentLeanAngle > 50.0 || currentTotalAccel > 20.0) ? "High" : "Medium";

  // ── Also keep nested location for emergency dashboard map ─────────────
  crashDoc["location"]["lat"] = gpsValid ? currentLat : 0.0;
  crashDoc["location"]["lng"] = gpsValid ? currentLng : 0.0;

  String crashPayload;
  serializeJson(crashDoc, crashPayload);

  // Write to a unique key under /crashes/
  String path = "/helmet_public/" + userUID + "/crashes/" + String(millis()) + ".json";

  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(5000);

  int httpCode = http.PUT(crashPayload);

  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("[CRASH] ✅ Crash data sent to Firebase");
    Serial.printf("[CRASH]   Timestamp: %lu | GPS: %s | Lat: %.6f | Lng: %.6f\n",
                  crashTimestamp, gpsValid ? "YES" : "NO", currentLat, currentLng);
    Serial.printf("[CRASH]   Impact: %.2fg | Lean: %.1f° | Severity: %s\n",
                  currentTotalAccel, currentLeanAngle,
                  (currentLeanAngle > 50.0 || currentTotalAccel > 20.0) ? "High" : "Medium");
  } else {
    Serial.printf("[CRASH] ❌ Failed to send crash data: HTTP %d\n", httpCode);
  }

  http.end();

  // Also update the live data crashDetected flag immediately
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(firebaseHost + livePath);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(2000);
    StaticJsonDocument<64> liveUpdate;
    liveUpdate["crashDetected"] = true;
    String livePayload;
    serializeJson(liveUpdate, livePayload);
    http.PATCH(livePayload);
    http.end();
  }
}

// ═══════════════════════════════════════════════════════════════════════════
// ANTI-THEFT SYSTEM WITH SMS ALERTS
// ═══════════════════════════════════════════════════════════════════════════

void processAntiTheft() {
  if (!engineRunning && !antiTheftArmed) {
    static unsigned long lastVibrationCheck = 0;
    if (millis() - lastVibrationCheck > 2000) { // Check every 2 seconds
      if (vibrationCount > 3) { // Movement detected
        antiTheftArmed = true;
        Serial.println("\n🛡️ ANTI-THEFT ARMED - Movement detected while engine off");
        
        playAlertSound(5, 150);
        sendDashboardAlert("🛡️ Anti-theft system armed - movement detected", "warning");
        
        // ✅ SEND SMS FOR ANTI-THEFT ACTIVATION
        String theftSMSMessage = "VIGILERT ALERT: Anti-theft system activated. Movement detected while engine off. ";
        if (gpsValid) {
          theftSMSMessage += "Location: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
        } else {
          theftSMSMessage += "GPS location unavailable.";
        }
        sendSMS(theftSMSMessage, "ANTI_THEFT");
        
        vibrationCount = 0;
      }
      lastVibrationCheck = millis();
    }
  }
  
  if (antiTheftArmed && engineRunning) {
    antiTheftArmed = false;
    Serial.println("\n✅ ANTI-THEFT DISARMED - Engine started");
    sendDashboardAlert("✅ Anti-theft system disarmed", "success");
  }
}

// ═══════════════════════════════════════════════════════════════════════════
// TRIP TRACKING FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void processTripTracking() {
  if (engineRunning && gpsValid && !tripActive) {
    startTrip();
  } else if (!engineRunning && tripActive) {
    endTrip();
  }
  
  // Update trip data while active
  if (tripActive && gpsValid) {
    if (currentSpeed > tripMaxSpeed) {
      tripMaxSpeed = currentSpeed;
    }
    
    // Calculate distance (simplified)
    static double lastTripLat = 0.0;
    static double lastTripLng = 0.0;
    static unsigned long lastDistanceUpdate = 0;
    
    if (millis() - lastDistanceUpdate > 5000 && lastTripLat != 0.0) { // Every 5 seconds
      double deltaLat = currentLat - lastTripLat;
      double deltaLng = currentLng - lastTripLng;
      double distance = sqrt(deltaLat * deltaLat + deltaLng * deltaLng) * 111.0; // Rough km conversion
      tripDistance += distance;
      
      lastTripLat = currentLat;
      lastTripLng = currentLng;
      lastDistanceUpdate = millis();
    } else if (lastTripLat == 0.0) {
      lastTripLat = currentLat;
      lastTripLng = currentLng;
      lastDistanceUpdate = millis();
    }
  }
}

void startTrip() {
  tripActive = true;
  tripStartTime = millis();
  tripStartLat = currentLat;
  tripStartLng = currentLng;
  tripDistance = 0.0;
  tripMaxSpeed = 0.0;
  
  Serial.println("\n🗺️ TRIP STARTED");
  Serial.printf("Start location: %.6f, %.6f\n", tripStartLat, tripStartLng);
}

void endTrip() {
  if (!tripActive) return;
  
  unsigned long tripDuration = millis() - tripStartTime;
  
  Serial.println("\n🏁 TRIP ENDED");
  Serial.printf("Duration: %lu minutes\n", tripDuration / 60000);
  Serial.printf("Distance: %.2f km\n", tripDistance);
  Serial.printf("Max speed: %.1f km/h\n", tripMaxSpeed);
  
  saveTripToFirebase(tripDuration);
  
  tripActive = false;
}

void saveTripToFirebase(unsigned long duration) {
  if (WiFi.status() != WL_CONNECTED) return;

  StaticJsonDocument<500> tripDoc;
  tripDoc["startTime"] = (uint64_t)(1700000000000ULL + tripStartTime);
  tripDoc["endTime"] = (uint64_t)(1700000000000ULL + millis());
  tripDoc["duration"] = duration;
  tripDoc["distance"] = tripDistance;
  tripDoc["maxSpeed"] = tripMaxSpeed;
  tripDoc["startLocation"]["lat"] = tripStartLat;
  tripDoc["startLocation"]["lng"] = tripStartLng;
  tripDoc["endLocation"]["lat"] = currentLat;
  tripDoc["endLocation"]["lng"] = currentLng;
  tripDoc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  tripDoc["time"] = getCurrentTimeString();
  
  String tripPayload;
  serializeJson(tripDoc, tripPayload);
  
  String tripPath = "/helmet_public/" + userUID + "/trips/" + String(millis()) + ".json";
  
  http.begin(firebaseHost + tripPath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);
  
  int httpCode = http.PUT(tripPayload);
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("[TRIP] ✅ Trip data saved to Firebase");
  } else {
    Serial.printf("[TRIP] ❌ Failed to save trip data: HTTP %d\n", httpCode);
  }
  
  http.end();
}

// ═══════════════════════════════════════════════════════════════════════════
// SPEED MONITORING WITH SMS ALERTS
// ═══════════════════════════════════════════════════════════════════════════

void processSpeedMonitoring() {
  static unsigned long lastSpeedCheck = 0;
  if (millis() - lastSpeedCheck < 1000) return; // Check every second
  
  if (gpsValid && engineRunning) {
    bool currentOverSpeed = (currentSpeed > speedLimit);
    
    if (currentOverSpeed && !isOverSpeed) {
      isOverSpeed = true;
      Serial.printf("\n⚠️ SPEED LIMIT EXCEEDED! %.1f km/h > %d km/h\n", currentSpeed, speedLimit);
      sendDashboardAlert("⚠️ Speed limit exceeded: " + String(currentSpeed, 1) + " km/h", "warning");
      
      // ✅ SEND SMS FOR SPEED VIOLATION
      String speedSMSMessage = "VIGILERT ALERT: Speed limit exceeded (" + String(currentSpeed, 1) + " km/h > " + String(speedLimit) + " km/h). ";
      if (gpsValid) {
        speedSMSMessage += "Location: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
      }
      sendSMS(speedSMSMessage, "SPEED_VIOLATION");
      
    } else if (!currentOverSpeed && isOverSpeed) {
      isOverSpeed = false;
      Serial.printf("\n✅ SPEED NORMALIZED! %.1f km/h\n", currentSpeed);
      sendDashboardAlert("✅ Speed normalized: " + String(currentSpeed, 1) + " km/h", "success");
    }
  }
  
  lastSpeedCheck = millis();
}

void loadSpeedLimit() {
  if (WiFi.status() != WL_CONNECTED) return;

  http.begin(firebaseHost + speedLimitPath);
  http.setTimeout(3000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    int limit = response.toInt();
    if (limit > 0 && limit <= 200) {
      speedLimit = limit;
      Serial.printf("[SPEED] Speed limit loaded: %d km/h\n", speedLimit);
    }
  } else {
    Serial.printf("[SPEED] Failed to load speed limit: HTTP %d\n", httpCode);
  }
  
  http.end();
}

// ═══════════════════════════════════════════════════════════════════════════
// UTILITY FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void sendDashboardAlert(String message, String type) {
  if (WiFi.status() != WL_CONNECTED) return;

  StaticJsonDocument<300> alertDoc;
  alertDoc["message"] = message;
  alertDoc["type"] = type;
  alertDoc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  alertDoc["time"] = getCurrentTimeString();
  alertDoc["deviceType"] = "motorcycle";
  alertDoc["read"] = false;
  
  String alertPayload;
  serializeJson(alertDoc, alertPayload);
  
  String alertPath = "/helmet_public/" + userUID + "/alerts/" + String(millis()) + ".json";
  
  http.begin(firebaseHost + alertPath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000);
  
  int httpCode = http.PUT(alertPayload);
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.printf("[ALERT] ✅ %s alert sent: %s\n", type.c_str(), message.c_str());
  }
  
  http.end();
}

void playAlertSound(int beeps, int duration) {
  for (int i = 0; i < beeps; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(duration);
    digitalWrite(buzzerPin, LOW);
    delay(duration);
  }
}

String getCurrentTimeString() {
  unsigned long currentTime = millis();
  unsigned long seconds = currentTime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  
  return String(hours % 24) + ":" + String(minutes % 60) + ":" + String(seconds % 60);
}

// ═══════════════════════════════════════════════════════════════════════════
// SERIAL COMMAND HANDLER WITH SMS TESTING
// ═══════════════════════════════════════════════════════════════════════════

void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    Serial.print("\n📋 Command received: ");
    Serial.println(cmd);
    
    if (cmd == "START") {
      if (validateSafetyConditions()) {
        startEngine();
      } else {
        Serial.println("🚫 ENGINE START BLOCKED - Safety conditions not met");
        Serial.print("Reasons: ");
        Serial.println(getSafetyBlockingReasons());
      }
      
    } else if (cmd == "STOP") {
      stopEngine();
      
    } else if (cmd == "STATUS") {
      printDetailedStatus();
      
    } else if (cmd == "TEST SMS") {
      Serial.println("\n📱 Testing SMS functionality...");
      if (gsmInitialized && !emergencyPhoneNumber.isEmpty()) {
        String testMessage = "VIGILERT TEST: SMS system is working correctly. ";
        if (gpsValid) {
          testMessage += "Current location: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
        } else {
          testMessage += "GPS location unavailable.";
        }
        
        if (sendSMS(testMessage, "TEST")) {
          Serial.println("✅ Test SMS sent successfully!");
        } else {
          Serial.println("❌ Test SMS failed!");
        }
      } else {
        Serial.print("❌ SMS not available - GSM:");
        Serial.print(gsmInitialized ? "OK" : "FAIL");
        Serial.print(", Phone:");
        Serial.println(emergencyPhoneNumber.isEmpty() ? "EMPTY" : "SET");
      }
      
    } else if (cmd.startsWith("SET PHONE ")) {
      String phoneNumber = cmd.substring(10);
      phoneNumber.trim();
      
      if (phoneNumber.length() >= 10) {
        emergencyPhoneNumber = phoneNumber;
        Serial.print("✅ Emergency phone number set: ");
        Serial.println(emergencyPhoneNumber);
        
        // Test SMS to new number
        if (gsmInitialized) {
          sendSMS("VIGILERT: Emergency contact number updated successfully.", "CONTACT_UPDATE");
        }
      } else {
        Serial.println("❌ Invalid phone number format. Use: SET PHONE +1234567890");
      }
      
    } else if (cmd == "TEST CRASH") {
      Serial.println("\n🧪 SIMULATING CRASH DETECTION...");
      crashDetected = true;
      lastCrashTime = millis();
      sendCrashDataToDashboards();
      
      String testCrashSMS = "VIGILERT TEST: Crash detection system test. This is a simulated emergency. ";
      if (gpsValid) {
        testCrashSMS += "Location: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
      }
      sendSMS(testCrashSMS, "TEST_CRASH");
      
      Serial.println("✅ Crash simulation complete");
      
    } else if (cmd == "TEST LEAN") {
      testLeanAngleReading();
      
    } else if (cmd == "CALIBRATE MPU") {
      if (mpuInitialized) {
        calibrateMPU6050();
      } else {
        Serial.println("❌ MPU6050 not initialized. Cannot calibrate.");
      }
      
    } else if (cmd == "RESET SAFETY") {
      Serial.println("\n🔄 RESETTING SAFETY SYSTEMS...");
      crashDetected = false;
      alcoholDetected = false;
      leanAngleExceeded = false;
      leanAngleWarning = false;
      safetyShutdownActive = false;
      antiTheftArmed = false;
      shutdownReason = "";
      
      Serial.println("✅ All safety flags cleared");
      sendDashboardAlert("✅ Safety systems reset", "success");
      
    } else if (cmd == "GSM STATUS") {
      Serial.println("\n📱 GSM MODULE STATUS:");
      Serial.print("Initialized: ");
      Serial.println(gsmInitialized ? "YES" : "NO");
      Serial.print("Emergency Contact: ");
      Serial.println(emergencyPhoneNumber.isEmpty() ? "NOT SET" : emergencyPhoneNumber);
      Serial.print("SMS Enabled: ");
      Serial.println(!emergencyPhoneNumber.isEmpty() ? "YES" : "NO");
      
      if (gsmInitialized) {
        // Test AT command
        gsmSerial.println("AT");
        delay(1000);
        String response = "";
        while (gsmSerial.available()) {
          response += gsmSerial.readString();
        }
        Serial.print("AT Response: ");
        Serial.println(response.indexOf("OK") != -1 ? "OK" : "FAIL");
      }
      
    } else {
      Serial.print("❌ Unknown command: ");
      Serial.println(cmd);
      Serial.println("\n📋 Available commands:");
      Serial.println("   START - Start engine (with safety validation)");
      Serial.println("   STOP - Stop engine");
      Serial.println("   STATUS - Show detailed system status");
      Serial.println("   TEST SMS - Send test SMS message");
      Serial.println("   SET PHONE +1234567890 - Set emergency contact number");
      Serial.println("   TEST CRASH - Simulate crash detection");
      Serial.println("   TEST LEAN - Test lean angle readings");
      Serial.println("   CALIBRATE MPU - Calibrate MPU6050 sensor");
      Serial.println("   RESET SAFETY - Clear all safety flags");
      Serial.println("   GSM STATUS - Check GSM module status");
    }
  }
}

void testLeanAngleReading() {
  if (!mpuInitialized) {
    Serial.println("❌ MPU6050 not initialized. Cannot test lean angle.");
    return;
  }
  
  Serial.println("\n📐 TESTING LEAN ANGLE READINGS (10 seconds)...");
  Serial.println("Tilt the motorcycle to test lean angle detection:");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  unsigned long testStart = millis();
  float maxLean = 0.0;
  float minLean = 0.0;
  
  while (millis() - testStart < 10000) { // 10 seconds
    // Read MPU6050
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    // Calculate angles
    roll = atan2(ay, az) * 180.0 / PI - rollOffset;
    pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI - pitchOffset;
    
    float testLeanAngle = abs(roll);
    
    if (testLeanAngle > maxLean) maxLean = testLeanAngle;
    if (roll < minLean) minLean = roll;
    
    // Print every 500ms
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 500) {
      Serial.printf("Roll: %6.1f° | Pitch: %6.1f° | Lean: %6.1f° | Accel: %6.1fg | Raw: %d,%d,%d\n",
                    roll, pitch, testLeanAngle, 
                    sqrt((ax/16384.0)*(ax/16384.0) + (ay/16384.0)*(ay/16384.0) + (az/16384.0)*(az/16384.0)) * 9.8,
                    ax, ay, az);
      lastPrint = millis();
    }
    
    delay(50);
  }
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.printf("📊 TEST RESULTS:\n");
  Serial.printf("   Max lean angle: %.1f°\n", maxLean);
  Serial.printf("   Roll range: %.1f° to %.1f°\n", minLean, -minLean);
  Serial.printf("   Threshold check: %s (Warning: %.1f°, Critical: %.1f°)\n", 
                maxLean > LEAN_ANGLE_THRESHOLD ? "EXCEEDED" : "OK", 
                LEAN_ANGLE_WARNING, LEAN_ANGLE_THRESHOLD);
  Serial.println("✅ Lean angle test complete");
}

void printDetailedStatus() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📊 VIGILERT MOTORCYCLE MODULE - COMPLETE WITH SMS ALERTS STATUS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // ✅ ENGINE STATUS
  Serial.println("🚗 ENGINE STATUS:");
  Serial.printf("   Running: %s\n", engineRunning ? "✅ YES" : "❌ NO");
  Serial.printf("   Allowed: %s\n", engineAllowed ? "✅ YES" : "❌ NO");
  Serial.printf("   Starter Relay (GPIO %d): %s\n", relayPin, digitalRead(relayPin) ? "🔓 ENABLED" : "🔒 DISABLED");
  Serial.printf("   Ignition Relay (GPIO %d): %s\n", ignitionRelayPin, digitalRead(ignitionRelayPin) ? "🔓 ENABLED" : "🔒 DISABLED");
  Serial.printf("   Physical Key: %s\n", digitalRead(ignitionRelayPin) ? "🔓 WORKS" : "🔒 BLOCKED");
  
  // ✅ ENHANCED SAFETY STATUS
  Serial.println("\n🛡️ ENHANCED SAFETY SYSTEM:");
  Serial.printf("   Safety Shutdown: %s\n", safetyShutdownActive ? "❌ ACTIVE" : "✅ INACTIVE");
  if (safetyShutdownActive) {
    Serial.print("   Shutdown Reason: ");
    Serial.println(shutdownReason);
  }
  Serial.printf("   Alcohol Detected: %s\n", alcoholDetected ? "❌ YES" : "✅ NO");
  if (alcoholDetected) {
    Serial.printf("   Alcohol Value: %d\n", alcoholSensorValue);
  }
  Serial.printf("   Lean Angle Exceeded: %s\n", leanAngleExceeded ? "❌ YES" : "✅ NO");
  if (leanAngleExceeded) {
    Serial.printf("   Current Lean Angle: %.1f°\n", currentLeanAngle);
  }
  Serial.printf("   Lean Angle Warning: %s\n", leanAngleWarning ? "⚠️ YES" : "✅ NO");
  if (leanAngleWarning) {
    Serial.printf("   Warning Lean Angle: %.1f°\n", currentLeanAngle);
  }
  Serial.printf("   Crash Detected: %s\n", crashDetected ? "❌ YES" : "✅ NO");
  Serial.printf("   Anti-theft Armed: %s\n", antiTheftArmed ? "🛡️ YES" : "❌ NO");
  
  // ✅ SMS SYSTEM STATUS
  Serial.println("\n📱 SMS ALERT SYSTEM:");
  Serial.printf("   GSM Module: %s\n", gsmInitialized ? "✅ CONNECTED" : "❌ DISCONNECTED");
  Serial.printf("   Emergency Contact: %s\n", emergencyPhoneNumber.isEmpty() ? "❌ NOT SET" : "✅ SET");
  if (!emergencyPhoneNumber.isEmpty()) {
    Serial.print("   Phone Number: ");
    Serial.println(emergencyPhoneNumber);
  }
  Serial.printf("   SMS Enabled: %s\n", (!emergencyPhoneNumber.isEmpty() && gsmInitialized) ? "✅ YES" : "❌ NO");
  Serial.printf("   Last SMS: %s ago\n", lastSMSTime > 0 ? "Recent" : "Never");
  if (lastSMSTime > 0) {
    Serial.printf("   SMS Time: %lu seconds ago\n", (millis() - lastSMSTime) / 1000);
  }
  
  // ✅ REAL MPU6050 SENSOR STATUS
  Serial.println("\n📐 MPU6050 LEAN ANGLE SENSOR:");
  Serial.printf("   Initialized: %s\n", mpuInitialized ? "✅ YES" : "❌ NO");
  if (mpuInitialized) {
    Serial.printf("   Current Roll: %.1f°\n", currentRoll);
    Serial.printf("   Current Pitch: %.1f°\n", currentPitch);
    Serial.printf("   Current Lean Angle: %.1f°\n", currentLeanAngle);
    Serial.printf("   Total Acceleration: %.1fg\n", currentTotalAccel);
    Serial.printf("   Roll Offset: %.2f°\n", rollOffset);
    Serial.printf("   Pitch Offset: %.2f°\n", pitchOffset);
    Serial.printf("   Raw Accel: X=%d, Y=%d, Z=%d\n", ax, ay, az);
    Serial.printf("   Raw Gyro: X=%d, Y=%d, Z=%d\n", gx, gy, gz);
  } else {
    Serial.println("   ⚠️ Using simulated lean angle data");
  }
  
  // ✅ GPS STATUS
  Serial.println("\n🛰️ GPS STATUS:");
  Serial.printf("   Valid Fix: %s\n", gpsValid ? "✅ YES" : "❌ NO");
  Serial.printf("   Satellites: %d\n", satelliteCount);
  Serial.printf("   Latitude: %.6f\n", currentLat);
  Serial.printf("   Longitude: %.6f\n", currentLng);
  Serial.printf("   Speed: %.1f km/h\n", currentSpeed);
  Serial.printf("   Over Speed: %s (Limit: %d km/h)\n", isOverSpeed ? "⚠️ YES" : "✅ NO", speedLimit);
  
  // ✅ CONNECTION STATUS
  Serial.println("\n🌐 CONNECTION STATUS:");
  Serial.printf("   WiFi: %s (%s, %d dBm)\n", 
                WiFi.status() == WL_CONNECTED ? "✅ CONNECTED" : "❌ DISCONNECTED",
                WiFi.localIP().toString().c_str(), WiFi.RSSI());
  Serial.printf("   Helmet Connected: %s\n", helmetConnected ? "✅ YES" : "❌ NO");
  
  // ✅ TRIP STATUS
  Serial.println("\n🗺️ TRIP STATUS:");
  Serial.printf("   Active Trip: %s\n", tripActive ? "✅ YES" : "❌ NO");
  if (tripActive) {
    unsigned long tripDuration = millis() - tripStartTime;
    Serial.printf("   Duration: %lu minutes\n", tripDuration / 60000);
    Serial.printf("   Distance: %.2f km\n", tripDistance);
    Serial.printf("   Max Speed: %.1f km/h\n", tripMaxSpeed);
  }
  
  // ✅ SYSTEM HEALTH
  Serial.println("\n💻 SYSTEM HEALTH:");
  Serial.printf("   Free Heap: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("   Uptime: %lu minutes\n", millis() / 60000);
  Serial.printf("   Version: complete_with_sms_v1.0\n");
  
  // ✅ SAFETY THRESHOLDS
  Serial.println("\n⚙️ SAFETY THRESHOLDS:");
  Serial.printf("   Lean Angle Warning: %.1f°\n", LEAN_ANGLE_WARNING);
  Serial.printf("   Lean Angle Critical: %.1f°\n", LEAN_ANGLE_THRESHOLD);
  Serial.printf("   Crash Acceleration: %.1fg\n", CRASH_ACCEL_THRESHOLD);
  Serial.printf("   Alcohol Threshold: %d\n", ALCOHOL_THRESHOLD);
  Serial.printf("   Engine Restart Cooldown: %lu seconds\n", ENGINE_RESTART_COOLDOWN / 1000);
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📋 Commands: START, STOP, STATUS, TEST SMS, SET PHONE, TEST CRASH, CALIBRATE MPU, RESET SAFETY");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}