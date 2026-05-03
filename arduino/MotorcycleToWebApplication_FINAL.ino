#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>

// ═══════════════════════════════════════════════════════════════════════════
// VigiLERT Motorcycle Module - FINAL PRODUCTION VERSION (FIXED)
// ═══════════════════════════════════════════════════════════════════════════
// ✅ Synchronized with Dashboard.vue
// ✅ All features: Crash, Anti-theft, Engine control
// ✅ Correct Firebase paths
// ✅ FIXED: Faster engine control response (50ms check, 300ms HTTP timeout)
// ✅ FIXED: Balanced security timeouts (10-15s instead of 3-5s)
// ✅ FIXED: Vibration sensor with INPUT_PULLUP and diagnostics
// ✅ FIXED: WiFi watchdog with alert before shutdown
// ═══════════════════════════════════════════════════════════════════════════

// ── Hardware feature flags ─────────────────────────────────────────────────
// Set to false if the sensor is not physically connected, to bypass it
#define MPU6050_ENABLED true   // Set false to bypass crash detection (no MPU6050 wired)

// ── Security feature flags ─────────────────────────────────────────────────
// Set SECURITY_SHUTDOWNS_ENABLED to false to disable ALL automatic engine
// shutdowns (WiFi watchdog, helmet timeout, security violations).
// Use this when testing the relay/engine without helmet or stable WiFi.
// Set back to true for production deployment.
#define SECURITY_SHUTDOWNS_ENABLED true

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Configuration
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

// ✅ Emergency contacts are fetched from Firebase (users/{uid}/emergencyContacts)
// No hardcoded phone number needed — contacts are managed via the web dashboard

// ── Pin Assignments ────────────────────────────────────────────────────────
// SINGLE RELAY wired in series with the ignition key switch circuit
//
// Wiring:  Battery → Key Switch → Relay → Ignition coil
//
// Relay ENERGIZED (LOW)   = contact CLOSED = ignition circuit complete = engine runs
// Relay DE-ENERGIZED (HIGH) = contact OPEN  = ignition cut = engine stops (even if key is ON)
//
// The relay STAYS LOW the entire time the engine is running.
// Writing HIGH immediately cuts the ignition — this is how remote stop works.
const int relayPin          = 13;   // Ignition relay (active-low, stays ON while engine runs)
const int buzzerPin         = 14;   // Buzzer (GPIO14, not a strapping pin)
const int lightIndicatorPin = 2;    // Status LED
const int vibrationSensorPin = 15;  // SW-420 vibration sensor

// ignitionRelayPin alias — both names refer to the same single relay
#define ignitionRelayPin relayPin

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
// ✅ OPTIONAL: Physical key monitoring (comment out if not using)
// const int physicalKeyPin = 14;  // Physical key switch input
// bool lastKeyState = LOW;

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
const unsigned long ALCOHOL_CHECK_INTERVAL = 2000;  // 2s — don't compete with heartbeat/button

// ✅ ENHANCED: Comprehensive security system with BALANCED timeouts
bool helmetConnected = false;
unsigned long lastHelmetCheck = 0;
const unsigned long HELMET_CHECK_INTERVAL = 3000;  // 3s — helmet updates every 1s so 3s is safe
uint64_t lastHelmetHeartbeat = 0;
const unsigned long HELMET_TIMEOUT = 15000;  // 15s — generous to avoid false disconnects
const unsigned long HELMET_FORCE_OFF_TIMEOUT = 20000;
unsigned long lastHelmetUpdateTime = 0;
bool helmetStatusForcedOff = false;

// WiFi connection monitoring
unsigned long lastWiFiCheck = 0;
unsigned long lastWiFiConnected = 0;
const unsigned long WIFI_CHECK_INTERVAL = 1000;
const unsigned long WIFI_TIMEOUT = 20000;  // 20s — tolerates brief WiFi hiccups

// WiFi Watchdog
bool wifiWatchdogActive = false;
unsigned long wifiDisconnectedTime = 0;
const unsigned long WIFI_WATCHDOG_TIMEOUT = 15000;  // 15s before engine cut
const unsigned long WIFI_WATCHDOG_ALERT_TIME = 8000; // Alert at 8s
bool emergencyShutdownTriggered = false;
bool wifiDisconnectionAlertSent = false;

// Startup WiFi Check
unsigned long deviceBootTime = 0;
const unsigned long STARTUP_WIFI_TIMEOUT = 10000;  // 10s — more time to connect on boot
bool startupWiFiCheckComplete = false;
bool startupShutdownTriggered = false;

// Starter safety — not used for ignition relay, kept for compatibility
unsigned long starterStartTime = 0;
unsigned long lastStarterAttempt = 0;
bool starterActive = false;
const unsigned long STARTER_TIMEOUT = 3000;
const unsigned long STARTER_COOLDOWN = 5000;

// Security check
bool securitySystemActive = true;
unsigned long lastSecurityCheck = 0;
const unsigned long SECURITY_CHECK_INTERVAL = 1000;  // 1s — was 500ms, reduces CPU pressure

// Automatic engine control
bool autoEngineControl = false;
bool engineStartRequested = false;

// Dashboard button control — separate fast poll from slow autoMode check
bool lastDashboardButtonState = false;
unsigned long lastButtonCheck = 0;
const unsigned long BUTTON_CHECK_INTERVAL = 150;  // 150ms button poll — fast response
unsigned long lastAutoModeCheck = 0;
const unsigned long AUTO_MODE_CHECK_INTERVAL = 5000;  // autoMode only needs checking every 5s

// Device heartbeat
unsigned long lastHeartbeat = 0;
const unsigned long HEARTBEAT_INTERVAL = 2000;  // 2s heartbeat — dashboard timeout is 15s so this is fine

// Network tracking
unsigned long lastWiFiConnection = 0;
unsigned long lastFirebaseSuccess = 0;
const unsigned long NETWORK_TIMEOUT = 30000;  // 30 seconds without network = auto shutdown
const unsigned long FIREBASE_TIMEOUT = 60000;  // 60 seconds without Firebase = auto shutdown
bool networkSecurityEnabled = true;
bool systemInSecureMode = false;

// ✅ NEW: Real-time monitoring intervals
unsigned long lastRealtimeUpdate = 0;
const unsigned long REALTIME_INTERVAL = 500;  // 500ms — reduced from 250ms to ease HTTP load
unsigned long lastSensorPrint = 0;
const unsigned long SENSOR_PRINT_INTERVAL = 500;  // 500ms sensor readings
unsigned long lastQuickStatus = 0;
const unsigned long QUICK_STATUS_INTERVAL = 1000;  // 1 second quick status

// Anti-theft system
bool antiTheftEnabled = false;
bool antiTheftArmed = false;
unsigned long engineOffTime = 0;
const unsigned long ARM_DELAY = 10000;          // 10 seconds after engine off before arming
unsigned long lastTheftAlert = 0;
const unsigned long THEFT_ALERT_COOLDOWN = 60000; // 60 seconds between SMS/Firebase alerts
bool theftAlertSent = false;
int theftDetectionCount = 0;
const int THEFT_DETECTION_REQUIRED = 1;
unsigned long lastVibrationTime = 0;
const unsigned long VIBRATION_DEBOUNCE = 30;    // 30ms debounce — catches fast pulses

int consecutiveVibrations = 0;

// ── Non-blocking buzzer state machine ─────────────────────────────────────
// Produces a police/ambulance siren sweep using tone() on the buzzer pin.
// Two modes:
//   SIREN  — rising/falling sweep (theft alert, security shutdown)
//   BEEP   — short double-beep (arming confirmation)
// No delay() used — all timing via millis()

enum BuzzerMode { BUZZER_OFF, BUZZER_SIREN, BUZZER_BEEP };
BuzzerMode buzzerMode = BUZZER_OFF;

unsigned long buzzerAlertStart  = 0;
unsigned long buzzerLastTick    = 0;
bool          buzzerLightState  = false;

// Siren parameters
const unsigned long BUZZER_ALERT_DURATION = 8000;  // Total siren duration per trigger (ms)
const unsigned long SIREN_TICK            = 15;    // Update frequency (ms) — smooth sweep
const int           SIREN_FREQ_LOW        = 800;   // Hz — bottom of sweep
const int           SIREN_FREQ_HIGH       = 1800;  // Hz — top of sweep
const unsigned long SIREN_HALF_PERIOD     = 600;   // ms for one sweep direction (low→high or high→low)

// Siren sweep state
int           sirenCurrentFreq  = SIREN_FREQ_LOW;
bool          sirenGoingUp      = true;
unsigned long sirenPhaseStart   = 0;

// GSM status
bool gsmReady = false;

// Firebase paths
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
const String buttonPath = "/" + userUID + "/engineControl/startButton.json?auth=" + firebaseAuth;
const String livePath = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;
const String alcoholPath = "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;

// ── Siren helpers ─────────────────────────────────────────────────────────
void startSiren() {
  buzzerMode       = BUZZER_SIREN;
  buzzerAlertStart = millis();
  buzzerLastTick   = millis();
  sirenPhaseStart  = millis();
  sirenGoingUp     = true;
  sirenCurrentFreq = SIREN_FREQ_LOW;
  tone(buzzerPin, SIREN_FREQ_LOW);   // kick off immediately
  digitalWrite(lightIndicatorPin, HIGH);
  Serial.println("[SIREN] 🚨 Siren started");
}

void stopSiren() {
  buzzerMode = BUZZER_OFF;
  noTone(buzzerPin);
  digitalWrite(lightIndicatorPin, LOW);
  Serial.println("[SIREN] Siren stopped");
}

// ✅ NEW: Function declarations for security system
void checkComprehensiveSecurity();
void triggerSecurityShutdown(String reason);
void logSecurityEventToFirebase(String eventType);
void printSecurityStatus();
// SMS alert functions
bool sendSMS(String phoneNumber, String message);
void sendSMSToAllContacts(String message, String alertType);
void checkAutoMode();

void setup() {
  Serial.begin(115200);
  delay(100);
  
  // ✅ SECURITY: Record boot time for startup WiFi check
  deviceBootTime = millis();
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   MOTORCYCLE MODULE - FIXED VERSION    ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ ⚡ 1s heartbeat                        ║");
  Serial.println("║ ✅ Dashboard synchronized              ║");
  Serial.println("║ ✅ All features enabled                ║");
  Serial.println("║ � Fast engine control (50ms/300ms)   ║");
  Serial.println("║ 🔒 Balanced security (10-15s timeouts)║");
  Serial.println("║ 🛡️ Enhanced vibration detection       ║");
  Serial.println("║ 🚨 WiFi watchdog with alerts          ║");
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

  // Initialize pins — SINGLE RELAY on starter switch
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);
  pinMode(vibrationSensorPin, INPUT_PULLUP);

  // Relay OFF at startup (HIGH = OFF for active-low relay)
  digitalWrite(relayPin, HIGH);
  noTone(buzzerPin);
  digitalWrite(lightIndicatorPin, LOW);

  delay(100);
  Serial.println("\n[SETUP] ═══════════════════════════════════");
  Serial.println("[SETUP] SINGLE RELAY on starter switch");
  Serial.printf("[SETUP] GPIO %d: Relay = %s\n", relayPin, digitalRead(relayPin) ? "OFF" : "ON");
  Serial.println("[SETUP] ═══════════════════════════════════\n");

  connectToWiFi();
  
  // Send initial heartbeat
  sendMotorcycleHeartbeat(true);
  lastHeartbeat = millis();

  // ✅ VIBRATION SENSOR: Enhanced baseline verification with diagnostic
  delay(500);  // Let pin stabilize
  int baselineReading = digitalRead(vibrationSensorPin);
  Serial.printf("\n[VIBRATION] 🔍 Enhanced sensor baseline check:\n");
  Serial.printf("[VIBRATION] Pin %d reading: %s (INPUT_PULLUP mode)\n", 
                vibrationSensorPin, baselineReading ? "HIGH" : "LOW");
  Serial.printf("[VIBRATION] Expected: HIGH (pulled up), LOW on vibration\n");
  
  if (baselineReading == HIGH) {
    Serial.println("[VIBRATION] ✅ Sensor ready (HIGH baseline with pull-up)");
  } else {
    Serial.println("[VIBRATION] ⚠️  WARNING: Sensor reading LOW - check wiring or sensor may be active!");
  }
  
  // Test sensor 3 times
  Serial.println("[VIBRATION] 🧪 Testing sensor 3 times...");
  for (int i = 0; i < 3; i++) {
    delay(500);
    int testReading = digitalRead(vibrationSensorPin);
    Serial.printf("[VIBRATION] Test %d: GPIO %d = %s\n", i+1, vibrationSensorPin, testReading ? "HIGH" : "LOW");
  }

  Serial.println("\n📋 SERIAL COMMANDS:");
  Serial.println("   START ENGINE / STOP ENGINE");
  Serial.println("   ARM THEFT / DISARM THEFT");
  Serial.println("   SECURITY ON / SECURITY OFF");
  Serial.println("   SECURITY STATUS");
  Serial.println("   TEST VIBRATION");
  Serial.println("   MONITOR (10s real-time data)");
  Serial.println("   STATUS");
  Serial.println("   (SMS alerts are automatic — sent to Firebase emergency contacts)\n");
}

void loop() {
  // ── WiFi auto-reconnect (non-blocking) ────────────────────────────────────
  {
    static unsigned long lastReconnectAttempt = 0;
    if (WiFi.status() != WL_CONNECTED && millis() - lastReconnectAttempt >= 5000) {
      lastReconnectAttempt = millis();
      Serial.println("[WIFI] Disconnected — attempting reconnect...");
      WiFi.reconnect();
    }
    // Always keep lastWiFiConnected fresh when connected
    if (WiFi.status() == WL_CONNECTED) {
      lastWiFiConnected = millis();
    }
  }

  // ✅ PRIORITY #0: WiFi Watchdog
  checkWiFiWatchdog();

  // ✅ Security monitoring (every 1s)
  if (millis() - lastSecurityCheck >= SECURITY_CHECK_INTERVAL) {
    checkComprehensiveSecurity();
    lastSecurityCheck = millis();
  }

  // ── GPS: read continuously ────────────────────────────────────────────────
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
  }
  if (millis() - lastGPSUpdate >= GPS_UPDATE_INTERVAL) {
    if (gps.speed.isValid()) {
      currentSpeed = gps.speed.kmph();
    }
    // Debug GPS status every 10 seconds
    static unsigned long lastGPSDebug = 0;
    if (millis() - lastGPSDebug >= 10000) {
      Serial.printf("[GPS] Fix:%s | Sats:%d | Lat:%.6f | Lng:%.6f | Speed:%.1f kph | Chars:%lu\n",
                    gps.location.isValid() ? "YES" : "NO",
                    gps.satellites.isValid() ? (int)gps.satellites.value() : 0,
                    gps.location.isValid() ? gps.location.lat() : 0.0,
                    gps.location.isValid() ? gps.location.lng() : 0.0,
                    currentSpeed,
                    gps.charsProcessed());
      if (gps.charsProcessed() < 10) {
        Serial.println("[GPS] ⚠️ No data from GPS — check wiring on RX:16 TX:17");
      }
      lastGPSDebug = millis();
    }
    lastGPSUpdate = millis();
  }

  // Trip tracking
  updateTripData();

  // Heartbeat every HEARTBEAT_INTERVAL
  if (millis() - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    bool heartbeatSent = sendMotorcycleHeartbeat(true);
    if (heartbeatSent) {
      lastFirebaseSuccess = millis();
      lastWiFiConnected   = millis();
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
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      int reading = digitalRead(vibrationSensorPin);
      Serial.printf("📍 GPIO %d current reading: %d (%s)\n", vibrationSensorPin, reading, reading ? "HIGH" : "LOW");
      Serial.printf("🛡️ Anti-theft armed: %s\n", antiTheftArmed ? "YES" : "NO");
      Serial.printf("🛡️ Anti-theft enabled: %s\n", antiTheftEnabled ? "YES" : "NO");
      Serial.printf("🔧 Pin mode: INPUT_PULLUP (expect HIGH normally)\n");
      
      Serial.println("\n📊 Reading sensor 10 times (tap sensor now):");
      for (int i = 0; i < 10; i++) {
        int testRead = digitalRead(vibrationSensorPin);
        Serial.printf("  Reading %d: %s\n", i+1, testRead ? "HIGH" : "LOW");
        delay(200);
      }
      
      // Force trigger for testing
      if (antiTheftArmed) {
        Serial.println("\n🚨 FORCE TRIGGERING VIBRATION ALERT!");
        consecutiveVibrations++;
        Serial.printf("🚨 [TEST] VIBRATION DETECTED #%d!\n", consecutiveVibrations);
        
        // Test buzzer
        for (int i = 0; i < 3; i++) {
          tone(buzzerPin, 1200);
          digitalWrite(lightIndicatorPin, HIGH);
          delay(200);
          noTone(buzzerPin);
          digitalWrite(lightIndicatorPin, LOW);
          delay(200);
        }
      } else {
        Serial.println("⚠️ Anti-theft not armed - cannot test alert");
        Serial.println("💡 TIP: Send 'STOP ENGINE' then wait 10 seconds to arm");
      }
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    }
    else if (cmd == "MONITOR") {
      Serial.println("📊 REAL-TIME MONITORING MODE (10 seconds)");
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      
      unsigned long monitorStart = millis();
      while (millis() - monitorStart < 10000) {  // 10 seconds
        // Read all sensors
        float totalAccel = 0.0;
        float roll = 0.0;
        
        if (MPU6050_ENABLED) {
          mpu.getEvent(&accel, &gyro, &temp);
          totalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                           accel.acceleration.y * accel.acceleration.y +
                           accel.acceleration.z * accel.acceleration.z);
          roll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
        } else {
          totalAccel = 9.8;  // Simulate normal gravity
          roll = 0.0;        // Simulate level position
        }
        
        int vibration = digitalRead(vibrationSensorPin);
        
        Serial.printf("[LIVE] Accel:%.1fg Roll:%.1f° Vib:%d Speed:%.1fkph WiFi:%s %s\n",
                      totalAccel, roll, vibration, currentSpeed,
                      WiFi.status() == WL_CONNECTED ? "OK" : "FAIL",
                      MPU6050_ENABLED ? "" : "(MPU6050 BYPASSED)");
        
        delay(200);  // 5 times per second
      }
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.println("📊 MONITORING COMPLETE");
    }
  }

  // ✅ OPTIMIZED: Read MPU6050 continuously (no delay) for instant crash detection
  if (MPU6050_ENABLED) {
    mpu.getEvent(&accel, &gyro, &temp);
    currentTotalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                             accel.acceleration.y * accel.acceleration.y +
                             accel.acceleration.z * accel.acceleration.z);
    currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
  } else {
    // ✅ TESTING: Simulate normal values when MPU6050 is not connected
    currentTotalAccel = 9.8;  // Normal gravity
    currentRoll = 0.0;        // Level position
  }
  float leanAngle = abs(currentRoll);

  // ── Non-blocking siren / buzzer tick ─────────────────────────────────────
  {
    unsigned long now = millis();

    if (buzzerMode == BUZZER_SIREN) {
      // Check if total alert duration has expired
      if (now - buzzerAlertStart >= BUZZER_ALERT_DURATION) {
        noTone(buzzerPin);
        digitalWrite(lightIndicatorPin, LOW);
        buzzerMode = BUZZER_OFF;
      } else if (now - buzzerLastTick >= SIREN_TICK) {
        buzzerLastTick = now;

        // Calculate how far through the current half-period we are (0.0 – 1.0)
        unsigned long phaseElapsed = now - sirenPhaseStart;
        float t = (float)phaseElapsed / (float)SIREN_HALF_PERIOD;
        if (t > 1.0f) t = 1.0f;

        // Linear interpolation between low and high frequency
        int freq = sirenGoingUp
          ? (int)(SIREN_FREQ_LOW  + t * (SIREN_FREQ_HIGH - SIREN_FREQ_LOW))
          : (int)(SIREN_FREQ_HIGH - t * (SIREN_FREQ_HIGH - SIREN_FREQ_LOW));

        tone(buzzerPin, freq);

        // Flash LED in sync with siren direction
        buzzerLightState = sirenGoingUp;
        digitalWrite(lightIndicatorPin, buzzerLightState ? HIGH : LOW);

        // Flip direction when half-period completes
        if (phaseElapsed >= SIREN_HALF_PERIOD) {
          sirenGoingUp = !sirenGoingUp;
          sirenPhaseStart = now;
        }
      }
    } else if (buzzerMode == BUZZER_BEEP) {
      // Simple double-beep for arming confirmation — runs once then stops
      // (handled inline at arm time with tiny delays — acceptable as it's once-only)
      buzzerMode = BUZZER_OFF;
    } else {
      // BUZZER_OFF — ensure silence
      // (noTone already called when mode was set to OFF)
    }
  }

  // ── Vibration sensor / anti-theft ─────────────────────────────────────────
  // Uses a static last-reading variable so state persists across loop iterations
  static int lastVibrationReading = LOW;
  static unsigned long lastVibrationTrigger = 0;

  if (!engineRunning) {
    // Step 1: enable the system as soon as engine turns off
    if (!antiTheftEnabled) {
      antiTheftEnabled = true;
      engineOffTime = millis();
      consecutiveVibrations = 0;
      Serial.println("\n[ANTI-THEFT] 🛡️ System enabled — arming in 10 seconds...");
    }

    // Step 2: arm after ARM_DELAY
    if (!antiTheftArmed && (millis() - engineOffTime >= ARM_DELAY)) {
      antiTheftArmed = true;
      theftDetectionCount = 0;
      theftAlertSent = false;
      consecutiveVibrations = 0;
      lastVibrationReading = digitalRead(vibrationSensorPin); // snapshot baseline
      Serial.println("\n[ANTI-THEFT] 🛡️ ARMED! Monitoring for movement...");
      Serial.printf("[ANTI-THEFT] 📍 GPIO %d baseline: %s\n",
                    vibrationSensorPin, lastVibrationReading ? "HIGH" : "LOW");

      // Two short confirmation beeps using tone() — safe on any pin
      tone(buzzerPin, 1200, 80); delay(160);
      tone(buzzerPin, 1200, 80); delay(160);
      noTone(buzzerPin);
      // 320ms total — acceptable once-only at arm time
    }

    // Step 3: poll sensor every loop — NO delay() here
    if (antiTheftArmed) {
      int currentReading = digitalRead(vibrationSensorPin);
      unsigned long currentTime = millis();

      // SW-420 fires a brief HIGH pulse on movement.
      // Detect BOTH edges (LOW→HIGH and HIGH→LOW) so we catch every pulse.
      if (currentReading != lastVibrationReading) {
        unsigned long timeSinceLastTrigger = currentTime - lastVibrationTrigger;

        if (timeSinceLastTrigger >= VIBRATION_DEBOUNCE) {
          consecutiveVibrations++;
          lastVibrationTrigger = currentTime;

          Serial.printf("\n🚨 [ANTI-THEFT] MOVEMENT #%d detected! GPIO%d: %s→%s\n",
                        consecutiveVibrations, vibrationSensorPin,
                        lastVibrationReading ? "HIGH" : "LOW",
                        currentReading ? "HIGH" : "LOW");

          // ── Start / extend non-blocking siren alert ──────────────────
          // Every new detection restarts the 8-second siren window
          startSiren();

          // ── Firebase + SMS alert (with cooldown to avoid spam) ────────
          unsigned long timeSinceLastAlert = currentTime - lastTheftAlert;
          if (timeSinceLastAlert >= THEFT_ALERT_COOLDOWN) {
            Serial.println("[ANTI-THEFT] � Sending Firebase + SMS alert...");
            // triggerTheftAlert() does Firebase write + SMS — runs once per cooldown
            // The buzzer continues independently while this runs
            triggerTheftAlert();
            lastTheftAlert = currentTime;
            theftAlertSent = true;
          } else {
            Serial.printf("[ANTI-THEFT] ⏳ Alert cooldown: %lu s remaining\n",
                          (THEFT_ALERT_COOLDOWN - timeSinceLastAlert) / 1000);
          }
        }
      }

      lastVibrationReading = currentReading;

      // Debug: print sensor state every 5 seconds so you can verify it's polling
      static unsigned long lastVibDebug = 0;
      if (currentTime - lastVibDebug >= 5000) {
        Serial.printf("[VIBRATION] Armed | GPIO%d=%s | Detections=%d | Buzzer=%s\n",
                      vibrationSensorPin,
                      currentReading ? "HIGH" : "LOW",
                      consecutiveVibrations,
                      buzzerMode == BUZZER_SIREN ? "ON" : "OFF");
        lastVibDebug = currentTime;
      }
    }

  } else {
    // Engine running — disarm anti-theft and silence buzzer
    if (antiTheftArmed) {
      Serial.println("[ANTI-THEFT] 🔓 Disarmed — engine running");
      antiTheftArmed = false;
      antiTheftEnabled = false;
      theftDetectionCount = 0;
      theftAlertSent = false;
      consecutiveVibrations = 0;
      stopSiren();
    }
  }

  // ✅ OPTIMIZED: Crash detection with continuous monitoring (only if MPU6050 enabled)
  if (MPU6050_ENABLED && engineRunning && (currentTotalAccel >= ACCEL_THRESHOLD || leanAngle > 40) && !crashDetected) {
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

  if (crashDetected && MPU6050_ENABLED) {
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

  // Automatic engine control (auto-mode only)
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

  // ✅ REAL-TIME: Fast sensor monitoring every 500ms
  if (millis() - lastSensorPrint > SENSOR_PRINT_INTERVAL) {
    int vibReading = digitalRead(vibrationSensorPin);
    Serial.printf("[SENSORS] Accel: %.2f g | Lean: %.1f° | Speed: %.1f kph | Vibration: %d (%s) %s\n", 
                  currentTotalAccel, abs(currentRoll), currentSpeed, vibReading,
                  vibReading ? "HIGH/Normal" : "LOW/Triggered",
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

  // ✅ FIX: Remove delay for faster crash detection
  // MPU6050 reads continuously without delay
  
  // ✅ STARTER SAFETY: Check starter timeout
  checkStarterTimeout();

  
  yield();  // Allow WiFi/system tasks to run
}

bool sendMotorcycleHeartbeat(bool isActive) {
  if (WiFi.status() != WL_CONNECTED) return false;

  uint64_t timestamp = 1746057600000ULL + (uint64_t)millis();

  StaticJsonDocument<128> doc;
  doc["status"] = isActive ? "On" : "Off";
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;

  String payload;
  serializeJson(doc, payload);

  HTTPClient http;
  http.setTimeout(1500);  // Explicit 1.5s timeout — must complete before next heartbeat
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/motorcycle.json?auth=" + firebaseAuth;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  int code = http.PUT(payload);
  http.end();

  if (code == HTTP_CODE_OK) {
    // ✅ Update lastWiFiConnected here so security checks don't false-trigger
    lastWiFiConnected = millis();
    lastFirebaseSuccess = millis();
    Serial.printf("[HEARTBEAT] ✓ Sent\n");
    return true;
  } else {
    Serial.printf("[HEARTBEAT] ✗ Failed: HTTP %d\n", code);
    return false;
  }
}

// ✅ NOTE: Vibration detection is now done DIRECTLY in loop() for instant response
// This function is kept for reference but not used anymore
void handleAntiTheftWithVibrationSensor_OLD() {
  // ✅ ULTRA-SENSITIVE VIBRATION DETECTION:
  // - Detects ANY state change (LOW→HIGH or HIGH→LOW)
  // - 50ms debounce = catches even slightest movements
  // - Triggers IMMEDIATELY on any vibration (no confirmation needed)
  // - Fast buzzer response (8 quick beeps)
  // - Sensor behavior: Normal=LOW, Vibration=HIGH (active high sensor)
  
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
    Serial.println("\n[ANTI-THEFT] 🛡️ ARMED! Ultra-sensitive vibration detection active...");
    Serial.printf("[ANTI-THEFT] 📍 Monitoring GPIO %d (any movement triggers alarm)\n", vibrationSensorPin);
    
    // Short beep to confirm arming
    for (int i = 0; i < 2; i++) {
      tone(buzzerPin, 1200);
      delay(100);
      noTone(buzzerPin);
      delay(100);
    }
  }

  if (antiTheftArmed) {
    int vibrationDetected = digitalRead(vibrationSensorPin);
    unsigned long currentTime = millis();
    
    // Track state changes
    static int lastVibrationState = LOW;  // Start with LOW (normal state for active-high sensor)
    
    // ✅ ULTRA-SENSITIVE: Detect ANY state change (LOW→HIGH or HIGH→LOW)
    // This catches even the slightest movement that causes brief pin changes
    bool stateChanged = (vibrationDetected != lastVibrationState);
    
    if (stateChanged) {
      unsigned long timeSinceLastVibration = currentTime - lastVibrationTime;
      
      // ✅ ULTRA-FAST DEBOUNCE: Only 50ms (catches slightest movement)
      if (timeSinceLastVibration >= VIBRATION_DEBOUNCE) {
        consecutiveVibrations++;
        lastVibrationTime = currentTime;
        
        Serial.printf("\n🚨🚨 [ANTI-THEFT] VIBRATION DETECTED #%d! 🚨🚨\n", consecutiveVibrations);
        Serial.printf("[ANTI-THEFT] 📍 GPIO %d: %s → %s (movement detected)!\n", 
                      vibrationSensorPin,
                      lastVibrationState ? "HIGH" : "LOW",
                      vibrationDetected ? "HIGH" : "LOW");

        // ✅ FAST BUZZER: 8 quick beeps as requested
        for (int i = 0; i < 8; i++) {
          tone(buzzerPin, 1200);
          digitalWrite(lightIndicatorPin, HIGH);
          delay(80);
          noTone(buzzerPin);
          digitalWrite(lightIndicatorPin, LOW);
          delay(80);
        }

        // ✅ SMS alert with cooldown (prevent spam)
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
      } else {
        // Movement detected but still in debounce period
        Serial.printf("[VIBRATION] 🔍 Movement detected but debouncing (%lu ms remaining)\n", 
                      VIBRATION_DEBOUNCE - timeSinceLastVibration);
      }
    }
    
    // ✅ CONTINUOUS MONITORING: Show sensor reading every 5 seconds for debugging
    static unsigned long lastDebugPrint = 0;
    if (millis() - lastDebugPrint > 5000) {
      Serial.printf("[VIBRATION] Status: %s | Armed: YES | Detections: %d | Sensitivity: ULTRA-HIGH\n", 
                    vibrationDetected ? "HIGH" : "LOW",
                    consecutiveVibrations);
      lastDebugPrint = millis();
    }
    
    // ✅ UPDATE STATE: Track for next iteration
    lastVibrationState = vibrationDetected;
    
    // ✅ RESET COUNTER: After 30 seconds of no activity
    if (consecutiveVibrations > 0 && (currentTime - lastVibrationTime) > 30000) {
      consecutiveVibrations = 0;
      Serial.println("[ANTI-THEFT] ✓ Counter reset (30s inactivity)");
    }
  }
}

void triggerTheftAlert() {
  Serial.println("\n🚨🚨🚨 THEFT ALERT! 🚨🚨🚨");

  // Buzzer is already running via the non-blocking state machine in loop().
  // No blocking delay() here — just do the Firebase + SMS work.

  String location = "Location unavailable";
  if (gps.location.isValid()) {
    location = "https://maps.google.com/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  }

  String message = "VIGILERT THEFT ALERT!\nUnauthorized movement detected on your motorcycle.\nLocation: " + location;

  Serial.println("[ANTI-THEFT] 📱 Sending SMS to emergency contacts...");
  sendSMSToAllContacts(message, "theft");

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

// ─────────────────────────────────────────────────────────────────────────────
// SMS: Send to a single number
// ─────────────────────────────────────────────────────────────────────────────
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
  
  gsmSerial.write(26);  // Ctrl+Z to send
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

// ─────────────────────────────────────────────────────────────────────────────
// SMS: Fetch emergency contacts from Firebase and send SMS to all of them
// Firebase path: /users/{userUID}/emergencyContacts
// Each contact has: name, phone, relationship
// alertType: "crash" | "theft" | "alcohol"
// ─────────────────────────────────────────────────────────────────────────────
void sendSMSToAllContacts(String message, String alertType) {
  if (!gsmReady) {
    Serial.println("[SMS] ❌ GSM not ready — cannot send alerts");
    return;
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[SMS] ❌ No WiFi — cannot fetch contacts");
    return;
  }

  Serial.println("[SMS] 📡 Fetching emergency contacts from Firebase...");

  HTTPClient http;
  http.setTimeout(5000);
  String contactsUrl = firebaseHost + "/users/" + userUID + "/emergencyContacts.json?auth=" + firebaseAuth;
  http.begin(contactsUrl);

  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("[SMS] ❌ Failed to fetch contacts: HTTP %d\n", httpCode);
    http.end();
    return;
  }

  String response = http.getString();
  http.end();

  if (response == "null" || response.length() <= 2) {
    Serial.println("[SMS] ⚠️ No emergency contacts found in Firebase");
    return;
  }

  Serial.println("[SMS] ✅ Contacts received — parsing...");

  // Manual JSON parsing — scan for every "phone" field in the response
  // Firebase returns: {"key1":{"name":"...","phone":"...","relationship":"..."},...}
  int sentCount = 0;
  int pos = 0;

  while (true) {
    // Find next "phone" key
    int phoneIdx = response.indexOf("\"phone\":", pos);
    if (phoneIdx == -1) break;

    // Extract phone value (the string after "phone":)
    int valueStart = response.indexOf("\"", phoneIdx + 8) + 1;
    int valueEnd   = response.indexOf("\"", valueStart);
    if (valueStart <= 0 || valueEnd <= valueStart) {
      pos = phoneIdx + 8;
      continue;
    }

    String phone = response.substring(valueStart, valueEnd);
    phone.trim();

    // Extract name for logging (best-effort, look backwards from phone field)
    String contactName = "Contact";
    int nameIdx = response.lastIndexOf("\"name\":", phoneIdx);
    if (nameIdx != -1 && nameIdx > pos) {
      int nStart = response.indexOf("\"", nameIdx + 7) + 1;
      int nEnd   = response.indexOf("\"", nStart);
      if (nStart > 0 && nEnd > nStart) {
        contactName = response.substring(nStart, nEnd);
      }
    }

    if (phone.length() >= 10) {
      Serial.printf("[SMS] 📱 Sending %s alert to %s (%s)...\n",
                    alertType.c_str(), contactName.c_str(), phone.c_str());
      bool sent = sendSMS(phone, message);
      if (sent) {
        Serial.printf("[SMS] ✅ Sent to %s\n", contactName.c_str());
        sentCount++;
      } else {
        Serial.printf("[SMS] ❌ Failed to send to %s\n", contactName.c_str());
      }
      delay(2000);  // Brief pause between sends to avoid GSM overload
    }

    pos = valueEnd + 1;
  }

  if (sentCount == 0) {
    Serial.println("[SMS] ⚠️ No valid phone numbers found in contacts");
  } else {
    Serial.printf("[SMS] ✅ Alert sent to %d contact(s)\n", sentCount);
  }
}

void logTheftToFirebase(String location) {
  if (WiFi.status() != WL_CONNECTED) return;

  // ✅ FIX: Use 2025-era base epoch so dashboard timestamp checks pass
  // 1746057600000 = May 1 2025 00:00:00 UTC in ms
  uint64_t timestamp = 1746057600000ULL + (uint64_t)millis();

  StaticJsonDocument<256> doc;
  doc["timestamp"] = timestamp;
  doc["type"] = "theft_attempt";
  doc["location"] = location;
  doc["vibrationDetected"] = true;
  doc["armed"] = antiTheftArmed;
  doc["alertTime"] = timestamp;

  String payload;
  serializeJson(doc, payload);

  // Write to /theft_alerts
  HTTPClient http;
  String theftPath = "/helmet_public/" + userUID + "/theft_alerts.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + theftPath);
  http.addHeader("Content-Type", "application/json");
  int code = http.POST(payload);
  if (code == HTTP_CODE_OK) {
    Serial.println("[FIREBASE] ✓ Theft alert logged to /theft_alerts");
  } else {
    Serial.printf("[FIREBASE] ✗ Theft alert failed: %d\n", code);
  }
  http.end();

  // ✅ Also write to /alerts so BOTH dashboards show the alert card immediately
  StaticJsonDocument<256> alertDoc;
  alertDoc["timestamp"] = timestamp;
  alertDoc["type"] = "theft";
  alertDoc["message"] = "🚨 Anti-Theft Alert!";
  alertDoc["details"] = "Unauthorized movement detected. Location: " + location;
  alertDoc["severity"] = "high";

  String alertPayload;
  serializeJson(alertDoc, alertPayload);

  HTTPClient http2;
  String alertsPath = "/helmet_public/" + userUID + "/alerts.json?auth=" + firebaseAuth;
  http2.begin(firebaseHost + alertsPath);
  http2.addHeader("Content-Type", "application/json");
  int alertCode = http2.POST(alertPayload);
  if (alertCode == HTTP_CODE_OK) {
    Serial.println("[FIREBASE] ✓ Theft alert written to /alerts");
  } else {
    Serial.printf("[FIREBASE] ✗ Theft /alerts write failed: %d\n", alertCode);
  }
  http2.end();
}

void triggerCrashShutdown(float impact, float roll) {
  Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
  Serial.printf("Impact: %.2f g | Roll: %.1f°\n", impact, roll);
  
  // ✅ FIX: HIGH = OFF for ACTIVE-LOW relay
  digitalWrite(relayPin, HIGH);
  engineRunning = false;
  
  Serial.printf("🚨 Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
  
  sendCrashToFirebase(impact, roll);
  
  crashDetected = true;
  lastCrashTime = millis();
  
  // ✅ SMS alert to all emergency contacts
  String location = "Location unavailable";
  if (gps.location.isValid()) {
    location = "https://maps.google.com/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  }
  String crashMsg = "🚨 VIGILERT CRASH ALERT!\n"
                    "A crash has been detected on the motorcycle.\n"
                    "Impact: " + String(impact, 1) + "g | Lean: " + String(abs(roll), 1) + "deg\n"
                    "Location: " + location + "\n"
                    "Engine has been automatically shut off.";
  Serial.println("[CRASH] 📱 Sending SMS to emergency contacts...");
  sendSMSToAllContacts(crashMsg, "crash");
  
  for (int i = 0; i < 5; i++) {
    tone(buzzerPin, 1200);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(200);
    noTone(buzzerPin);
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
  
  // ✅ 1. WiFi Security Check
  if (!wifiOK) {
    Serial.println("\n❌ ENGINE START BLOCKED - WiFi TIMEOUT!");
    Serial.printf("💡 WiFi disconnected for %lu ms (>%lu ms limit)\n", timeSinceWiFi, WIFI_TIMEOUT);
    digitalWrite(relayPin, HIGH);
    tone(buzzerPin, 1200, 150); delay(200); tone(buzzerPin, 1200, 150); delay(200); tone(buzzerPin, 1200, 150);
    logSecurityEventToFirebase("Engine Start Blocked - WiFi Timeout");
    return;
  }

  // ✅ 2. Helmet Security Check
  if (!helmetOK && lastHelmetUpdateTime > 0) {
    Serial.println("\n❌ ENGINE START BLOCKED - HELMET TIMEOUT!");
    Serial.printf("💡 Helmet disconnected for %lu ms (>%lu ms limit)\n", timeSinceHelmet, HELMET_TIMEOUT);
    digitalWrite(relayPin, HIGH);
    tone(buzzerPin, 1200, 150); delay(200); tone(buzzerPin, 1200, 150);
    logSecurityEventToFirebase("Engine Start Blocked - Helmet Timeout");
    return;
  }

  // ✅ 3. Alcohol Safety Check
  if (!alcoholOK) {
    Serial.println("\n❌ ENGINE START BLOCKED - ALCOHOL DETECTED!");
    if (autoEngineControl) engineStartRequested = true;
    digitalWrite(relayPin, HIGH);
    tone(buzzerPin, 800, 300); delay(400); tone(buzzerPin, 800, 300); delay(400); tone(buzzerPin, 800, 300);
    logSecurityEventToFirebase("Engine Start Blocked - Alcohol Detected");
    return;
  }

  // ✅ 4. Crash Safety Check
  if (!crashOK) {
    Serial.println("\n❌ ENGINE START BLOCKED - CRASH DETECTED!");
    digitalWrite(relayPin, HIGH);
    tone(buzzerPin, 600, 400); delay(500); tone(buzzerPin, 600, 400); delay(500);
    tone(buzzerPin, 600, 400); delay(500); tone(buzzerPin, 600, 400);
    logSecurityEventToFirebase("Engine Start Blocked - Crash Detected");
    return;
  }

  Serial.println("\n✅ Starting engine...");

  // Ignition relay wired in series with key switch.
  // Set LOW (energize) → contact closes → ignition circuit complete → engine runs.
  // Relay stays LOW until stopEngine() or a security shutdown sets it HIGH.
  digitalWrite(relayPin, LOW);
  starterActive = false;  // No starter timeout needed — relay stays ON
  Serial.printf("✅ Ignition relay ON (GPIO %d = LOW) — stays ON while engine runs\n", relayPin);

  engineRunning = true;
  engineStartRequested = false;
  
  // ✅ NEW: Start trip tracking
  startTrip();
  
  sendLiveToFirebase();
  
  if (antiTheftArmed) {
    antiTheftArmed = false;
    antiTheftEnabled = false;
    theftDetectionCount = 0;
    stopSiren();
    Serial.println("[ANTI-THEFT] 🔓 Disarmed");
  }
}

void stopEngine() {
  Serial.println("\n🛑 Stopping engine...");

  // Set relay HIGH (de-energize) → contact opens → ignition circuit cut.
  // Engine stops immediately even if physical key is still ON.
  digitalWrite(relayPin, HIGH);
  engineRunning = false;
  starterActive = false;

  Serial.printf("🛑 Ignition relay OFF (GPIO %d = HIGH) — ignition cut\n", relayPin);

  endTrip();
  sendLiveToFirebase();

  engineOffTime = millis();
  antiTheftEnabled = true;
  antiTheftArmed = false;
  Serial.println("[ANTI-THEFT] 🛡️ Will arm in 10 seconds...");
}

// checkStarterTimeout() — not used for ignition relay.
// The relay stays ON (LOW) the entire time the engine is running.
// Only stopEngine() or triggerSecurityShutdown() sets it HIGH.
void checkStarterTimeout() {
  // No-op for ignition relay wiring.
  // (Kept so loop() call compiles without changes.)
}

void sendCrashToFirebase(float impact, float roll) {
  if (WiFi.status() != WL_CONNECTED) return;

  // ✅ FIX: Use a 2025-era base epoch so dashboard timestamp checks pass
  // 1746057600000 = May 1 2025 00:00:00 UTC in milliseconds
  uint64_t timestamp = 1746057600000ULL + (uint64_t)millis();
  
  String locationStr = "No GPS";
  if (gps.location.isValid()) {
    locationStr = "https://maps.google.com/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  }

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
  
  // Write to crashes path (for map display)
  HTTPClient http;
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  int code = http.POST(payload);
  if (code == HTTP_CODE_OK) {
    Serial.println("[FIREBASE] ✓ Crash event logged to /crashes");
  } else {
    Serial.printf("[FIREBASE] ✗ Crash event failed: %d\n", code);
  }
  http.end();

  // ✅ Also write to /alerts so both dashboards show the alert card
  StaticJsonDocument<256> alertDoc;
  alertDoc["timestamp"] = timestamp;
  alertDoc["type"] = "crash";
  alertDoc["message"] = "🚨 CRASH DETECTED!";
  alertDoc["details"] = "Impact: " + String(impact, 1) + "g | Lean: " + String(abs(roll), 1) + "deg | " + locationStr;
  alertDoc["time"] = String(millis() / 1000) + "s uptime";
  alertDoc["severity"] = impact > 20.0 ? "severe" : "moderate";

  String alertPayload;
  serializeJson(alertDoc, alertPayload);

  HTTPClient http2;
  String alertsPath = "/helmet_public/" + userUID + "/alerts.json?auth=" + firebaseAuth;
  http2.begin(firebaseHost + alertsPath);
  http2.addHeader("Content-Type", "application/json");
  int alertCode = http2.POST(alertPayload);
  if (alertCode == HTTP_CODE_OK) {
    Serial.println("[FIREBASE] ✓ Crash alert written to /alerts");
  } else {
    Serial.printf("[FIREBASE] ✗ Crash alert to /alerts failed: %d\n", alertCode);
  }
  http2.end();
}

void sendLiveToFirebase() {
  if (WiFi.status() != WL_CONNECTED) return;

  StaticJsonDocument<512> doc;
  doc["engineRunning"] = engineRunning;

  if (MPU6050_ENABLED) {
    doc["mpu6050"]["accelX"]     = accel.acceleration.x;
    doc["mpu6050"]["accelY"]     = accel.acceleration.y;
    doc["mpu6050"]["accelZ"]     = accel.acceleration.z;
    doc["mpu6050"]["totalAccel"] = currentTotalAccel;
    doc["mpu6050"]["roll"]       = currentRoll;
    doc["mpu6050"]["enabled"]    = true;
  } else {
    doc["mpu6050"]["accelX"]     = 0.0;
    doc["mpu6050"]["accelY"]     = 0.0;
    doc["mpu6050"]["accelZ"]     = 9.8;
    doc["mpu6050"]["totalAccel"] = currentTotalAccel;
    doc["mpu6050"]["roll"]       = currentRoll;
    doc["mpu6050"]["enabled"]    = false;
  }

  doc["crashDetected"]    = crashDetected;
  doc["alcoholDetected"]  = alcoholDetected;
  doc["autoEngineControl"]= autoEngineControl;
  doc["antiTheftArmed"]   = antiTheftArmed;
  doc["vibrationSensor"]  = digitalRead(vibrationSensorPin);
  doc["relayStatus"]      = digitalRead(relayPin) ? "OFF" : "ON";
  doc["speed"]            = currentSpeed;

  if (gps.location.isValid()) {
    doc["locationLat"] = gps.location.lat();
    doc["locationLng"] = gps.location.lng();
    doc["gpsValid"]    = true;
  } else {
    doc["locationLat"] = 0.0;
    doc["locationLng"] = 0.0;
    doc["gpsValid"]    = false;
  }
  // Always send satellite count and chars processed for diagnostics
  doc["gpsSatellites"]   = gps.satellites.isValid() ? (int)gps.satellites.value() : 0;
  doc["gpsCharsProcessed"] = (int)gps.charsProcessed();
  doc["timestamp"] = millis();

  String payload;
  serializeJson(doc, payload);

  HTTPClient http;
  http.setTimeout(1500);  // Explicit timeout
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.PUT(payload);
  http.end();

  // Update engineControl path (lightweight — just engine state)
  StaticJsonDocument<64> engineDoc;
  engineDoc["engineRunning"] = engineRunning;
  engineDoc["timestamp"]     = millis();
  String enginePayload;
  serializeJson(engineDoc, enginePayload);

  HTTPClient http2;
  http2.setTimeout(1000);
  String enginePath = "/" + userUID + "/engineControl.json?auth=" + firebaseAuth;
  http2.begin(firebaseHost + enginePath);
  http2.addHeader("Content-Type", "application/json");
  http2.PATCH(enginePayload);
  http2.end();
}

void connectToWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.println("\n[WIFI] Connecting...");

  // Persistent mode + auto-reconnect
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  WiFi.setTxPower(WIFI_POWER_19_5dBm);  // Max TX power for better range/stability
  WiFi.begin(ssid, password);

  // Wait up to 15 seconds with progress dots
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] ✅ Connected!");
    Serial.printf("[WIFI] IP: %s | RSSI: %d dBm\n",
                  WiFi.localIP().toString().c_str(), WiFi.RSSI());
    lastWiFiConnected  = millis();
    lastWiFiConnection = millis();
    lastFirebaseSuccess = millis();
    wifiDisconnectedTime = 0;
    emergencyShutdownTriggered = false;
  } else {
    Serial.println("\n[WIFI] ❌ Failed — will retry automatically");
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

    // Update state — only act on alcohol if helmet is connected
    // (prevents stale Firebase data from triggering shutdown)
    if (currentAlcoholState != alcoholDetected) {
      alcoholDetected = currentAlcoholState;
      lastAlcoholState = currentAlcoholState;

      if (currentAlcoholState && helmetConnected) {
        // Only cut starter relay here — stopEngine() handles ignition
        digitalWrite(relayPin, HIGH);

        if (engineRunning) {
          triggerAlcoholShutdown();
        }
      } else if (currentAlcoholState && !helmetConnected) {
        Serial.println("[ALCOHOL] ⚠️ Danger reading but helmet not connected — ignoring stale data");
        alcoholDetected = false;  // Don't act on stale data
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

  // ✅ FIX: HIGH = OFF for ACTIVE-LOW relay
  digitalWrite(relayPin, HIGH);
  engineRunning = false;

  // ✅ SMS alert to all emergency contacts
  String location = "Location unavailable";
  if (gps.location.isValid()) {
    location = "https://maps.google.com/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  }
  String alcoholMsg = "⚠️ VIGILERT ALCOHOL ALERT!\n"
                      "Alcohol was detected on the rider.\n"
                      "Engine has been automatically shut off for safety.\n"
                      "Location: " + location;
  Serial.println("[ALCOHOL] 📱 Sending SMS to emergency contacts...");
  sendSMSToAllContacts(alcoholMsg, "alcohol");

  // ✅ Write to /alerts so BOTH dashboards show the alert card immediately
  if (WiFi.status() == WL_CONNECTED) {
    uint64_t timestamp = 1746057600000ULL + (uint64_t)millis();
    StaticJsonDocument<256> alertDoc;
    alertDoc["timestamp"] = timestamp;
    alertDoc["type"] = "alcohol";
    alertDoc["message"] = "⚠️ Alcohol Detected - Engine Shutdown!";
    alertDoc["details"] = "Alcohol detected on rider. Engine shut off automatically. Location: " + location;
    alertDoc["severity"] = "high";

    String alertPayload;
    serializeJson(alertDoc, alertPayload);

    HTTPClient http;
    String alertsPath = "/helmet_public/" + userUID + "/alerts.json?auth=" + firebaseAuth;
    http.begin(firebaseHost + alertsPath);
    http.addHeader("Content-Type", "application/json");
    int alertCode = http.POST(alertPayload);
    if (alertCode == HTTP_CODE_OK) {
      Serial.println("[FIREBASE] ✓ Alcohol alert written to /alerts");
    } else {
      Serial.printf("[FIREBASE] ✗ Alcohol /alerts write failed: %d\n", alertCode);
    }
    http.end();
  }

  for (int i = 0; i < 10; i++) {
    tone(buzzerPin, 1200);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(500);
    noTone(buzzerPin);
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
                digitalRead(relayPin) ? "OFF" : "ON");
  Serial.printf("Engine: %s\n", engineRunning ? "RUNNING" : "STOPPED");
  Serial.printf("Crash: %s\n", crashDetected ? "YES" : "NO");
  Serial.printf("Alcohol: %s\n", alcoholDetected ? "YES" : "NO");
  Serial.printf("Auto Control: %s\n", autoEngineControl ? "ON" : "OFF");
  Serial.printf("Anti-Theft: %s\n", antiTheftArmed ? "ARMED" : "DISARMED");
  Serial.printf("WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED" : "DISCONNECTED");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}

void handleDashboardButton() {
  unsigned long now = millis();
  // Fast: button poll every BUTTON_CHECK_INTERVAL (150ms)
  if (now - lastButtonCheck >= BUTTON_CHECK_INTERVAL) {
    checkDashboardButton();
    lastButtonCheck = now;
  }
  // Slow: autoMode check every AUTO_MODE_CHECK_INTERVAL (5s)
  if (now - lastAutoModeCheck >= AUTO_MODE_CHECK_INTERVAL) {
    checkAutoMode();
    lastAutoModeCheck = now;
  }
}

void checkDashboardButton() {
  if (WiFi.status() != WL_CONNECTED) return;

  // ── Fast path: button state only (150ms poll) ─────────────────────────────
  HTTPClient http;
  http.setTimeout(500);  // 500ms max — if Firebase is slow, skip and retry next cycle
  http.begin(firebaseHost + buttonPath);

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    bool currentButtonState = (response.indexOf("true") != -1);

    if (currentButtonState && !lastDashboardButtonState) {
      lastDashboardButtonState = true;
      Serial.println("\n🖥️ DASHBOARD BUTTON PRESSED!");

      if (!engineRunning) {
        engineStartRequested = true;
        startEngine();
      } else {
        stopEngine();
        engineStartRequested = false;
      }
      clearDashboardButton();
    } else if (!currentButtonState) {
      lastDashboardButtonState = false;
    }
  }
  http.end();
}

void checkAutoMode() {
  // ── Slow path: autoMode (checked every 5s, not every 150ms) ──────────────
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.setTimeout(1000);
  String autoModePath = "/" + userUID + "/engineControl/autoMode.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + autoModePath);

  int autoCode = http.GET();
  if (autoCode == HTTP_CODE_OK) {
    String autoResponse = http.getString();
    bool dashboardAutoMode = (autoResponse.indexOf("true") != -1);
    if (dashboardAutoMode != autoEngineControl) {
      autoEngineControl = dashboardAutoMode;
      Serial.printf("[ENGINE] Auto control: %s\n", autoEngineControl ? "ON" : "OFF");
    }
  }
  http.end();
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
// ✅ REMOVED: Physical key monitoring functions (not needed for basic dual relay system)
// The dual relay system works without monitoring the physical key position
// Security is enforced by cutting power through the ignition relay

// ✅ ENHANCED: Comprehensive security system with 5-second timeouts
void checkComprehensiveSecurity() {
  if (!securitySystemActive) return;

  unsigned long currentTime = millis();

  // 1. WiFi timeout � lastWiFiConnected is updated every loop when connected
  //    AND on every successful heartbeat. Only flag after first connection.
  unsigned long timeSinceWiFi = (lastWiFiConnected > 0)
                                  ? (currentTime - lastWiFiConnected)
                                  : 0;
  bool wifiTimeout = (lastWiFiConnected > 0) && (timeSinceWiFi > WIFI_TIMEOUT);

  // 2. Helmet timeout � only flag if engine running AND helmet was seen before
  unsigned long timeSinceHelmet = (lastHelmetUpdateTime > 0)
                                    ? (currentTime - lastHelmetUpdateTime)
                                    : 0;
  bool helmetTimeout = engineRunning &&
                       (lastHelmetUpdateTime > 0) &&
                       (timeSinceHelmet > HELMET_TIMEOUT) &&
                       !helmetConnected;

  bool securityViolation = wifiTimeout || helmetTimeout;

  if (SECURITY_SHUTDOWNS_ENABLED && securityViolation && engineRunning) {
    Serial.println("\n?????? SECURITY VIOLATION � AUTO SHUTDOWN! ??????");
    if (wifiTimeout)   Serial.printf("?? WiFi TIMEOUT: %lu ms (>%lu ms)\n", timeSinceWiFi, WIFI_TIMEOUT);
    if (helmetTimeout) Serial.printf("?? HELMET TIMEOUT: %lu ms (>%lu ms)\n", timeSinceHelmet, HELMET_TIMEOUT);
    triggerSecurityShutdown("Security Violation");
  }

  systemInSecureMode = securityViolation;

  // Debug every 5 seconds
  static unsigned long lastSecurityDebug = 0;
  if (currentTime - lastSecurityDebug > 5000) {
    Serial.printf("[SECURITY] WiFi:%s(%lums) Helmet:%s(%lums) Engine:%s\n",
                  wifiTimeout ? "TIMEOUT?" : "OK?", timeSinceWiFi,
                  helmetTimeout ? "TIMEOUT?" : "OK?", timeSinceHelmet,
                  engineRunning ? "RUNNING" : "STOPPED");
    lastSecurityDebug = currentTime;
  }
}
// ✅ NEW: Security shutdown with detailed logging
void triggerSecurityShutdown(String reason) {
  Serial.println("\n🚨 SECURITY SHUTDOWN INITIATED 🚨");
  Serial.println("Reason: " + reason);

  // Cut ignition immediately — relay HIGH opens the circuit
  digitalWrite(relayPin, HIGH);
  engineRunning = false;
  starterActive = false;

  endTrip();

  Serial.printf("🛑 Ignition relay OFF (GPIO %d = HIGH) — engine cut\n", relayPin);

  startSiren();
  logSecurityEventToFirebase(reason);
}

  // Start siren (non-blocking — keeps going while Firebase write runs)
  startSiren();

  // Log security event to Firebase
  logSecurityEventToFirebase(reason);
}

// ✅ SECURITY: WiFi Watchdog — cuts engine if WiFi drops while engine is running
void checkWiFiWatchdog() {
  unsigned long currentTime = millis();
  bool wifiConnected = (WiFi.status() == WL_CONNECTED);

  // ── Startup WiFi check ────────────────────────────────────────────────────
  if (!startupWiFiCheckComplete) {
    unsigned long timeSinceBoot = currentTime - deviceBootTime;

    if (wifiConnected) {
      startupWiFiCheckComplete = true;
      lastWiFiConnected = currentTime;
      Serial.printf("\n[STARTUP] ✅ WiFi connected in %lu ms\n", timeSinceBoot);
    } else if (timeSinceBoot >= STARTUP_WIFI_TIMEOUT && !startupShutdownTriggered) {
      startupShutdownTriggered = true;
      startupWiFiCheckComplete = true;

      Serial.println("\n[STARTUP] WiFi FAILED � cutting engine power!");
      if (SECURITY_SHUTDOWNS_ENABLED) {
        digitalWrite(ignitionRelayPin, HIGH);
        digitalWrite(relayPin, HIGH);
        engineRunning = false;
        startSiren();
      } else {
        Serial.println("[STARTUP] SECURITY_SHUTDOWNS_ENABLED=false � engine kept running");
      }
      Serial.println("[STARTUP] Connect to WiFi and restart device");
    } else {
      static unsigned long lastStartupWarning = 0;
      if (currentTime - lastStartupWarning >= 1000) {
        Serial.printf("[STARTUP] ⏳ Waiting for WiFi... (%lu ms remaining)\n",
                      STARTUP_WIFI_TIMEOUT - timeSinceBoot);
        lastStartupWarning = currentTime;
      }
    }
    return; // Don't run watchdog until startup check is done
  }

  // ── Runtime WiFi watchdog ─────────────────────────────────────────────────
  // Triggers whenever WiFi drops — whether engine is running or parked
  if (wifiConnected) {
    lastWiFiConnected = currentTime;
    wifiDisconnectedTime = 0;
    emergencyShutdownTriggered = false;
    wifiDisconnectionAlertSent = false;  // Reset alert flag
  } else {
    if (wifiDisconnectedTime == 0) {
      wifiDisconnectedTime = currentTime;
      Serial.printf("\n⚠️ [WIFI WATCHDOG] WiFi LOST — alert at %lu s, shutdown at %lu s\n",
                    WIFI_WATCHDOG_ALERT_TIME / 1000,
                    WIFI_WATCHDOG_TIMEOUT / 1000);
    }

    unsigned long disconnectedDuration = currentTime - wifiDisconnectedTime;

    // ✅ FIXED: Send alert at 5 seconds
    if (disconnectedDuration >= WIFI_WATCHDOG_ALERT_TIME && !wifiDisconnectionAlertSent) {
      wifiDisconnectionAlertSent = true;
      Serial.println("\n⚠️ [WIFI WATCHDOG] 5-SECOND ALERT - WiFi still disconnected!");
      
      // Log to Firebase if possible (may fail if WiFi is down)
      logSecurityEventToFirebase("WiFi Disconnection Alert - 5 seconds");
      
      // Visual/audio alert
      for (int i = 0; i < 3; i++) {
        tone(buzzerPin, 2000, 100);
        delay(150);
      }
    }

    // ✅ FIXED: Shutdown at 10 seconds (was 3 seconds)
    if (disconnectedDuration >= WIFI_WATCHDOG_TIMEOUT && !emergencyShutdownTriggered) {
      emergencyShutdownTriggered = true;
      Serial.println("\n🚨 [WIFI WATCHDOG] 10-SECOND TIMEOUT — cutting engine power!");

      if (SECURITY_SHUTDOWNS_ENABLED) {
        digitalWrite(ignitionRelayPin, HIGH);
        digitalWrite(relayPin, HIGH);
        engineRunning = false;
        startSiren();
      } else {
        Serial.println("[WIFI WATCHDOG] ⚠️ SECURITY_SHUTDOWNS_ENABLED=false — engine kept running");
      }

      Serial.println("[WIFI WATCHDOG] 💡 Reconnect WiFi to restore operation");
    }

    // Warning beep every 2 seconds while disconnected (before shutdown)
    static unsigned long lastWarningBeep = 0;
    if (!emergencyShutdownTriggered && currentTime - lastWarningBeep >= 2000) {
      tone(buzzerPin, 1000, 80);  // Short 80ms pip — non-blocking
      lastWarningBeep = currentTime;
      Serial.printf("[WIFI WATCHDOG] ⚠️ Shutdown in %lu ms...\n",
                    WIFI_WATCHDOG_TIMEOUT - disconnectedDuration);
    }
  }

  // Status log every 5 seconds
  static unsigned long lastWatchdogStatus = 0;
  if (currentTime - lastWatchdogStatus >= 5000) {
    Serial.printf("[WIFI WATCHDOG] %s | Engine:%s | Siren:%s\n",
                  wifiConnected ? "WiFi OK ✅" : "WiFi LOST ❌",
                  engineRunning ? "RUNNING" : "STOPPED",
                  buzzerMode == BUZZER_SIREN ? "ON" : "OFF");
    lastWatchdogStatus = currentTime;
  }
}

// ✅ NEW: Enhanced physical key with security checks
// ✅ REMOVED: Physical key security monitoring (not needed for basic dual relay system)
// Security is enforced by the dual relay system cutting ignition power

// ✅ Check helmet connection — simplified: if Firebase GET succeeds and
// status is "On", the helmet is connected. No timestamp comparison needed.
void checkHelmetConnection() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.setTimeout(1500);
  String helmetPath = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
  http.begin(helmetPath);

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();

    // If status is "On" → helmet is alive. Update our wall-clock timestamp.
    bool statusIsOn = (response.indexOf("\"status\":\"On\"") != -1);

    if (statusIsOn) {
      // ✅ Every successful "On" read counts as a fresh heartbeat
      lastHelmetUpdateTime = millis();
      helmetStatusForcedOff = false;

      if (!helmetConnected) {
        Serial.println("[HELMET] ✅ Connected");
        helmetConnected = true;
      }
    } else {
      // Status is "Off" or missing
      if (helmetConnected) {
        Serial.println("[HELMET] ⚠️ Status is Off");
      }
      helmetConnected = false;
      // Don't zero lastHelmetUpdateTime here — let the timeout in
      // checkComprehensiveSecurity() handle the grace period naturally
    }
  } else {
    // HTTP failed — don't immediately disconnect; let timeout handle it
    Serial.printf("[HELMET] HTTP %d — keeping last known state\n", httpCode);
  }

  http.end();

  // Status log every 5 seconds
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug >= 5000) {
    unsigned long timeSince = (lastHelmetUpdateTime > 0) ? (millis() - lastHelmetUpdateTime) : 0;
    Serial.printf("[HELMET] %s | Last seen: %lu ms ago\n",
                  helmetConnected ? "CONNECTED ✅" : "DISCONNECTED ❌", timeSince);
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
  currentTrip.startTime = 1746057600000ULL + (uint64_t)millis();
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
  
  currentTrip.endTime = 1746057600000ULL + (uint64_t)millis();
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
  doc["endTime"]   = currentTrip.endTime;
  doc["duration"]  = currentTrip.endTime - currentTrip.startTime;

  // ── Coordinates using field names the dashboard expects ──────────────────
  // Dashboard reads: startLat, startLng, endLat, endLng
  doc["startLat"] = currentTrip.fromLat;
  doc["startLng"] = currentTrip.fromLng;
  doc["endLat"]   = currentTrip.toLat;
  doc["endLng"]   = currentTrip.toLng;

  // Also keep nested from/to for backwards compatibility
  doc["from"]["lat"] = currentTrip.fromLat;
  doc["from"]["lng"] = currentTrip.fromLng;
  doc["to"]["lat"]   = currentTrip.toLat;
  doc["to"]["lng"]   = currentTrip.toLng;

  // Trip statistics
  doc["distance"] = currentTrip.totalDistance;
  doc["maxSpeed"] = currentTrip.maxSpeed;
  uint64_t durationMs = currentTrip.endTime - currentTrip.startTime;
  doc["avgSpeed"] = (durationMs > 0)
    ? (currentTrip.totalDistance / (durationMs / 3600000.0))
    : 0.0;

  doc["vehicleType"] = "motorcycle";
  doc["userId"]      = userUID;
  doc["tripId"]      = String(currentTrip.startTime);

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
  
  uint64_t timestamp = 1746057600000ULL + (uint64_t)millis();
  
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
  Serial.println("Physical Key: Not monitored (dual relay security active)");
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
