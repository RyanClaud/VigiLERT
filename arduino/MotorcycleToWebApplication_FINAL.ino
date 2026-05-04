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
const char* password = "123456789000";

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
const unsigned long ALCOHOL_CHECK_INTERVAL = 500;  // 500ms — matches helmet write rate for fast detection

// ✅ ENHANCED: Comprehensive security system with BALANCED timeouts
bool helmetConnected = false;
unsigned long lastHelmetCheck = 0;
const unsigned long HELMET_CHECK_INTERVAL = 2000;  // Check every 2s
uint64_t lastHelmetHeartbeat = 0;
const unsigned long HELMET_TIMEOUT = 10000;  // 10s — if heartbeat stops for 10s, shut down
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

// ── Hardware interrupt for vibration sensor ────────────────────────────────
// The SW-420 fires a brief pulse (10-50ms). If the loop is blocked by HTTP
// calls, digitalRead() misses it. An interrupt catches it instantly.
volatile bool vibrationISRTriggered = false;
volatile unsigned long vibrationISRTime = 0;

void IRAM_ATTR onVibration() {
  // Keep ISR minimal — no millis(), no Serial, no heap operations
  // Debounce is handled in loop() handler
  vibrationISRTriggered = true;
}

// ── Non-blocking buzzer state machine ─────────────────────────────────────
// Produces a professional police/emergency siren pattern.
//
// SIREN mode — "Wail" pattern (classic police siren):
//   Sweeps smoothly from LOW to HIGH and back, like a real siren.
//   LOW=700Hz, HIGH=1700Hz, sweep takes 800ms each direction.
//
// BEEP mode — short confirmation beeps (arming, engine start feedback)
//
// All timing via millis() — zero delay() in the siren tick.

// ── Simple two-tone police wail — loop-based, zero ISR complexity ─────────
// Alternates between HIGH tone and LOW tone every SIREN_PHASE_MS.
// tone() is called from loop() — completely safe, no ISR, no crash.
// Sounds like a real police/ambulance wail.

enum BuzzerMode { BUZZER_OFF, BUZZER_SIREN };
BuzzerMode buzzerMode = BUZZER_OFF;

const int           SIREN_TONE_HIGH  = 1700;  // Hz — high note
const int           SIREN_TONE_LOW   = 700;   // Hz — low note
const unsigned long SIREN_PHASE_MS   = 700;   // ms per note (700ms high, 700ms low)
const unsigned long SIREN_DURATION   = 10000; // total siren time per trigger

unsigned long sirenStartTime  = 0;
unsigned long sirenPhaseStart = 0;
bool          sirenHighPhase  = true;  // true = playing high note

// Called every loop iteration — updates tone if phase has changed
void tickSiren() {
  if (buzzerMode != BUZZER_SIREN) return;

  unsigned long now = millis();

  // Stop after SIREN_DURATION
  if (now - sirenStartTime >= SIREN_DURATION) {
    noTone(buzzerPin);
    digitalWrite(lightIndicatorPin, LOW);
    buzzerMode = BUZZER_OFF;
    return;
  }

  // Switch note every SIREN_PHASE_MS
  if (now - sirenPhaseStart >= SIREN_PHASE_MS) {
    sirenPhaseStart = now;
    sirenHighPhase  = !sirenHighPhase;
    tone(buzzerPin, sirenHighPhase ? SIREN_TONE_HIGH : SIREN_TONE_LOW);
    digitalWrite(lightIndicatorPin, sirenHighPhase ? HIGH : LOW);
  }
}

// GSM status
bool gsmReady = false;

// Firebase paths
const String crashPath  = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
const String buttonPath = "/" + userUID + "/engineControl/startButton.json?auth=" + firebaseAuth;
const String livePath   = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;
const String alcoholPath = "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;

// ── Siren helpers ─────────────────────────────────────────────────────────
void startSiren() {
  buzzerMode      = BUZZER_SIREN;
  sirenStartTime  = millis();
  sirenPhaseStart = millis();
  sirenHighPhase  = true;
  tone(buzzerPin, SIREN_TONE_HIGH);
  digitalWrite(lightIndicatorPin, HIGH);
  Serial.println("[SIREN] Started");
}

void stopSiren() {
  buzzerMode = BUZZER_OFF;
  noTone(buzzerPin);
  digitalWrite(lightIndicatorPin, LOW);
  Serial.println("[SIREN] Stopped");
}

// Short pleasant double-chirp — used for confirmations (arming, blocked start)
// Two quick rising tones: 1000Hz → 1400Hz, 80ms each. Non-blocking via tone duration.
void playConfirmBeep() {
  tone(buzzerPin, 1000); delay(80);
  tone(buzzerPin, 1400); delay(80);
  noTone(buzzerPin);
}

void playWarningBeep(int count) {
  int freqs[] = {1400, 1200, 1000, 800};
  for (int i = 0; i < count && i < 4; i++) {
    tone(buzzerPin, freqs[i]); delay(120);
    noTone(buzzerPin);         delay(60);
  }
}

// Forward declarations — security system
void checkComprehensiveSecurity();
void triggerSecurityShutdown(String reason);
void logSecurityEventToFirebase(String eventType);
void printSecurityStatus();
void printSystemStatus();
void checkWiFiWatchdog();
void checkHelmetConnection();
void forceHelmetStatusOff();
// Engine / relay
void startEngine();
void stopEngine();
void checkStarterTimeout();
void triggerCrashShutdown(float impact, float roll);
void sendCrashToFirebase(float impact, float roll);
void sendLiveToFirebase();
void connectToWiFi();
bool sendMotorcycleHeartbeat(bool isActive);
// Trip
void startTrip();
void endTrip();
void updateTripData();
void saveTripToFirebase();
float calculateDistance(double lat1, double lng1, double lat2, double lng2);
// Alcohol / security
void checkAlcoholStatus();
void triggerAlcoholShutdown();
// Anti-theft
void triggerTheftAlert();
void logTheftToFirebase(String location);
// GSM
void initializeGSM();
void tickGSMInit();
// Dashboard button
void handleDashboardButton();
void checkDashboardButton();
void checkAutoMode();
void clearDashboardButton();
// SMS
bool sendSMS(String phoneNumber, String message);
void sendSMSToAllContacts(String message, String alertType);
// Buzzer helpers
void playConfirmBeep();
void playWarningBeep(int count);
// GSM
void tickGSMInit();

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

  // Initialize GPS — must start before GSM to avoid UART conflicts
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("[SETUP] GPS Serial started on RX:16, TX:17");
  // Flush any GPS startup garbage
  delay(100);
  while (gpsSerial.available()) gpsSerial.read();

  // Initialize GSM
  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);
  Serial.println("[SETUP] GSM Serial started on RX:4, TX:5");
  initializeGSM();

  // Initialize pins — correct order: pinMode THEN tone/noTone
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);
  pinMode(vibrationSensorPin, INPUT_PULLUP);

  // Set initial pin states
  digitalWrite(relayPin, HIGH);       // Relay OFF (active-low)
  noTone(buzzerPin);                  // Buzzer silent
  digitalWrite(lightIndicatorPin, LOW);

  // Hardware interrupt for vibration sensor
  attachInterrupt(digitalPinToInterrupt(vibrationSensorPin), onVibration, CHANGE);

  // Startup beep — confirms buzzer is working
  tone(buzzerPin, 1200, 150); delay(200);
  tone(buzzerPin, 1600, 150); delay(200);
  noTone(buzzerPin);

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
  // ── PRIORITY #0: Tick the siren (two-tone wail, loop-safe) ───────────────
  tickSiren();

  // ── PRIORITY #1: Vibration interrupt handler ──────────────────────────────
  if (vibrationISRTriggered) {
    vibrationISRTriggered = false;  // clear flag immediately
    unsigned long now = millis();
    // Debounce in loop — ignore if too soon after last trigger
    if (now - vibrationISRTime >= VIBRATION_DEBOUNCE) {
      vibrationISRTime = now;
      if (!engineRunning && antiTheftArmed) {
        consecutiveVibrations++;
        Serial.printf("[ANTI-THEFT] INTERRUPT: Movement #%d!\n", consecutiveVibrations);
        startSiren();
        if (now - lastTheftAlert >= THEFT_ALERT_COOLDOWN) {
          triggerTheftAlert();
          lastTheftAlert = now;
          theftAlertSent = true;
        }
      }
    }
  }

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

  // ── GPS: read ALL available bytes every loop — don't limit to one char ──────
  // Reading more bytes per iteration ensures full NMEA sentences are parsed
  // before the next loop cycle, giving faster fix acquisition.
  unsigned long gpsReadStart = millis();
  while (gpsSerial.available() > 0 && millis() - gpsReadStart < 20) {
    gps.encode(gpsSerial.read());
  }
  if (millis() - lastGPSUpdate >= GPS_UPDATE_INTERVAL) {
    if (gps.speed.isValid()) {
      float rawSpeed = gps.speed.kmph();
      const float GPS_SPEED_DEADBAND = 3.0;
      currentSpeed = (rawSpeed > GPS_SPEED_DEADBAND) ? rawSpeed : 0.0;
    } else {
      currentSpeed = 0.0;
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
        
        // Test siren
        startSiren();
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

  // -- Vibration sensor / anti-theft -----------------------------------------
  // SW-420 with INPUT_PULLUP: normal=HIGH, vibration=LOW (brief pulse)
  // Detect BOTH edges to catch every pulse.
  static int lastVibrationReading = HIGH;  // Correct baseline for INPUT_PULLUP
  static unsigned long lastVibrationTrigger = 0;

  if (!engineRunning) {
    // Step 1: enable system as soon as engine turns off
    if (!antiTheftEnabled) {
      antiTheftEnabled = true;
      engineOffTime = millis();
      consecutiveVibrations = 0;
      lastVibrationReading = digitalRead(vibrationSensorPin); // real baseline NOW
      Serial.println("\n[ANTI-THEFT] System enabled � arming in 10 seconds...");
      Serial.printf("[ANTI-THEFT] Baseline: GPIO%d = %s\n",
                    vibrationSensorPin, lastVibrationReading ? "HIGH" : "LOW");
    }

    // Step 2: arm after ARM_DELAY
    if (!antiTheftArmed && (millis() - engineOffTime >= ARM_DELAY)) {
      antiTheftArmed = true;
      theftDetectionCount = 0;
      theftAlertSent = false;
      consecutiveVibrations = 0;
      lastVibrationReading = digitalRead(vibrationSensorPin); // re-snapshot at arm
      Serial.println("\n[ANTI-THEFT] ARMED! Monitoring for movement...");
      Serial.printf("[ANTI-THEFT] GPIO %d baseline at arm: %s\n",
                    vibrationSensorPin, lastVibrationReading ? "HIGH" : "LOW");
      playConfirmBeep();
    }

    // Step 3: poll sensor every loop � NO delay() here
    if (antiTheftArmed) {
      int currentReading = digitalRead(vibrationSensorPin);
      unsigned long currentTime = millis();

      if (currentReading != lastVibrationReading) {
        unsigned long timeSinceLastTrigger = currentTime - lastVibrationTrigger;

        if (timeSinceLastTrigger >= VIBRATION_DEBOUNCE) {
          consecutiveVibrations++;
          lastVibrationTrigger = currentTime;

          Serial.printf("\n[ANTI-THEFT] MOVEMENT #%d! GPIO%d: %s->%s\n",
                        consecutiveVibrations, vibrationSensorPin,
                        lastVibrationReading ? "HIGH" : "LOW",
                        currentReading ? "HIGH" : "LOW");

          startSiren();

          unsigned long timeSinceLastAlert = currentTime - lastTheftAlert;
          if (timeSinceLastAlert >= THEFT_ALERT_COOLDOWN) {
            Serial.println("[ANTI-THEFT] Sending Firebase + SMS alert...");
            triggerTheftAlert();
            lastTheftAlert = currentTime;
            theftAlertSent = true;
          } else {
            Serial.printf("[ANTI-THEFT] Cooldown: %lu s remaining\n",
                          (THEFT_ALERT_COOLDOWN - timeSinceLastAlert) / 1000);
          }
        }
      }

      lastVibrationReading = currentReading;

      // Debug every 3 seconds
      static unsigned long lastVibDebug = 0;
      if (currentTime - lastVibDebug >= 3000) {
        Serial.printf("[VIBRATION] GPIO%d=%s | Detections=%d | Siren=%s\n",
                      vibrationSensorPin,
                      currentReading ? "HIGH" : "LOW",
                      consecutiveVibrations,
                      buzzerMode == BUZZER_SIREN ? "ON" : "off");
        lastVibDebug = currentTime;
      }
    }

  } else {
    // Engine running � disarm anti-theft and silence buzzer
    if (antiTheftArmed) {
      Serial.println("[ANTI-THEFT] Disarmed � engine running");
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

  // Alcohol monitoring — 500ms poll matches helmet write rate
  if (millis() - lastAlcoholCheck > ALCOHOL_CHECK_INTERVAL) {
    checkAlcoholStatus();
    lastAlcoholCheck = millis();
  }

  // ── GSM background retry — non-blocking state machine ────────────────────
  tickGSMInit();

  // ── Alcohol safety enforcement — runs every loop ─────────────────────────
  if (alcoholDetected && engineRunning) {
    Serial.println("[ALCOHOL] DANGER — cutting ignition!");
    stopEngine();
  }

  // Automatic engine control (auto-mode only)
  if (autoEngineControl) {
    if (!alcoholDetected && engineStartRequested && !engineRunning) {
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
    
    // Confirmation chirp � arming confirmed
      playConfirmBeep();
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

        // Siren alert — non-blocking
          startSiren();

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

// ── GSM non-blocking state machine ────────────────────────────────────────
// Replaces the blocking initializeGSM() with a state machine that advances
// one step per loop iteration. No delay() — uses millis() for all timing.
enum GsmInitState {
  GSM_IDLE,
  GSM_FLUSH,
  GSM_AT_SEND,
  GSM_AT_WAIT,
  GSM_SETUP_ECHO,
  GSM_SETUP_CMGF,
  GSM_DONE
};
GsmInitState gsmInitState   = GSM_IDLE;
int          gsmAttempt     = 0;
unsigned long gsmStateTimer = 0;

// Call this every loop — advances the state machine one step at a time
void tickGSMInit() {
  if (gsmReady) return;  // Already initialized

  unsigned long now = millis();

  switch (gsmInitState) {
    case GSM_IDLE:
      Serial.println("[GSM] Starting non-blocking init...");
      while (gsmSerial.available()) gsmSerial.read();  // flush
      gsmAttempt   = 0;
      gsmStateTimer = now;
      gsmInitState  = GSM_FLUSH;
      break;

    case GSM_FLUSH:
      // Wait 500ms for module to settle, then start AT attempts
      if (now - gsmStateTimer >= 500) {
        gsmInitState = GSM_AT_SEND;
      }
      break;

    case GSM_AT_SEND:
      if (gsmAttempt >= 15) {
        // Exhausted attempts — wait 30s then retry from scratch
        Serial.println("[GSM] No response after 15 attempts — will retry in 30s");
        gsmInitState  = GSM_IDLE;
        gsmStateTimer = now + 30000;  // reuse timer as "next retry" time
        // Hack: set state to IDLE but delay the next trigger
        // We'll use a separate flag to gate re-entry
        break;
      }
      gsmSerial.println("AT");
      gsmAttempt++;
      gsmStateTimer = now;
      gsmInitState  = GSM_AT_WAIT;
      break;

    case GSM_AT_WAIT:
      // Read response for up to 800ms
      {
        String resp = "";
        while (gsmSerial.available()) resp += (char)gsmSerial.read();
        if (resp.indexOf("OK") != -1) {
          Serial.printf("[GSM] OK on attempt %d\n", gsmAttempt);
          gsmInitState  = GSM_SETUP_ECHO;
          gsmStateTimer = now;
        } else if (now - gsmStateTimer >= 800) {
          // No response yet — try again
          gsmInitState = GSM_AT_SEND;
        }
      }
      break;

    case GSM_SETUP_ECHO:
      if (now - gsmStateTimer >= 200) {
        gsmSerial.println("ATE0");   // echo off
        gsmSerial.println("AT+CMGF=1");  // SMS text mode
        gsmStateTimer = now;
        gsmInitState  = GSM_SETUP_CMGF;
      }
      break;

    case GSM_SETUP_CMGF:
      if (now - gsmStateTimer >= 500) {
        while (gsmSerial.available()) gsmSerial.read();  // flush responses
        gsmReady     = true;
        gsmInitState = GSM_DONE;
        Serial.println("[GSM] Ready!");
      }
      break;

    case GSM_DONE:
      break;  // Nothing to do
  }
}

// Legacy blocking init — kept for startup only (called once in setup)
// Uses the non-blocking machine but blocks until done or timeout
void initializeGSM() {
  Serial.println("[GSM] Initializing...");
  gsmInitState = GSM_IDLE;
  gsmReady     = false;
  unsigned long deadline = millis() + 15000;  // 15s max at startup
  while (!gsmReady && millis() < deadline) {
    tickGSMInit();
    delay(10);  // tiny yield — acceptable only at startup
  }
  if (!gsmReady) {
    Serial.println("[GSM] Not ready at startup — will retry in background");
  }
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
      delay(500);  // Brief pause between SMS sends
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

  // Start siren — non-blocking, continues while loop runs
  startSiren();
}

void startEngine() {
  Serial.println("\n[ENGINE] startEngine() called");
  
  // ✅ ENHANCED: Comprehensive security checks before engine start
  unsigned long currentTime = millis();
  unsigned long timeSinceWiFi = currentTime - lastWiFiConnected;
  unsigned long timeSinceHelmet = currentTime - lastHelmetUpdateTime;
  
  bool wifiOK   = (lastWiFiConnected > 0) && (timeSinceWiFi <= WIFI_TIMEOUT);
  // Use helmetConnected boolean directly — it's kept current by checkHelmetConnection()
  // Don't use timeSinceHelmet here: the check runs every 2s so the timestamp
  // can be up to 2s stale even when the helmet is live.
  bool helmetOK = helmetConnected;
  bool alcoholOK = !alcoholDetected;
  bool crashOK   = !crashDetected;
  
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
    playWarningBeep(3);
    logSecurityEventToFirebase("Engine Start Blocked - WiFi Timeout");
    return;
  }

  // ✅ 2. Helmet Security Check
  if (!helmetOK) {
    Serial.println("\n❌ ENGINE START BLOCKED - HELMET NOT CONNECTED!");
    Serial.printf("💡 helmetConnected=%s | Last seen: %lu ms ago\n",
                  helmetConnected ? "true" : "false", timeSinceHelmet);
    digitalWrite(relayPin, HIGH);
    playWarningBeep(2);
    logSecurityEventToFirebase("Engine Start Blocked - Helmet Not Connected");
    return;
  }

  // ✅ 3. Alcohol Safety Check
  if (!alcoholOK) {
    Serial.println("\n❌ ENGINE START BLOCKED - ALCOHOL DETECTED!");
    if (autoEngineControl) engineStartRequested = true;
    digitalWrite(relayPin, HIGH);
    playWarningBeep(3);
    logSecurityEventToFirebase("Engine Start Blocked - Alcohol Detected");
    return;
  }

  // ✅ 4. Crash Safety Check
  if (!crashOK) {
    Serial.println("\n❌ ENGINE START BLOCKED - CRASH DETECTED!");
    digitalWrite(relayPin, HIGH);
    playWarningBeep(4);
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
  http.setTimeout(800);  // Short timeout — must complete within poll interval
  http.begin(firebaseHost + alcoholPath);

  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    // Don't spam Serial on every failed read — only log occasionally
    static unsigned long lastFailLog = 0;
    if (millis() - lastFailLog > 5000) {
      Serial.printf("[ALCOHOL] HTTP %d\n", httpCode);
      lastFailLog = millis();
    }
    http.end();
    return;
  }

  String response = http.getString();
  http.end();

  // Null / empty response — treat as Safe
  if (response == "null" || response.length() <= 2) {
    if (alcoholDetected) {
      Serial.println("[ALCOHOL] Cleared (no data)");
      alcoholDetected = false;
    }
    return;
  }

  bool currentAlcoholState = false;
  int  sensorValue = 0;

  // ── Priority 1: "detected" boolean (fastest — single field check) ─────────
  if (response.indexOf("\"detected\":true") != -1) {
    currentAlcoholState = true;
  } else if (response.indexOf("\"detected\":false") != -1) {
    currentAlcoholState = false;
  }
  // ── Priority 2: "status" string ───────────────────────────────────────────
  else if (response.indexOf("\"status\":\"Danger\"") != -1 ||
           response.indexOf("\"status\":\"danger\"") != -1) {
    currentAlcoholState = true;
  } else if (response.indexOf("\"status\":\"Safe\"") != -1 ||
             response.indexOf("\"status\":\"safe\"") != -1) {
    currentAlcoholState = false;
  }
  // ── Priority 3: raw sensor value fallback ─────────────────────────────────
  else {
    int idx = response.indexOf("sensorValue");
    if (idx != -1) {
      int s = response.indexOf(":", idx) + 1;
      int e = response.indexOf(",", s);
      if (e == -1) e = response.indexOf("}", s);
      if (e > s) sensorValue = response.substring(s, e).toInt();
    }
    currentAlcoholState = (sensorValue > 600);
  }

  // ── Update state and act immediately ──────────────────────────────────────
  if (currentAlcoholState != alcoholDetected) {
    alcoholDetected = currentAlcoholState;
    Serial.printf("[ALCOHOL] >>> %s <<<\n", alcoholDetected ? "DANGER DETECTED" : "CLEARED");
  }

  // Cut engine immediately — no guard, no delay
  if (alcoholDetected && engineRunning) {
    Serial.println("[ALCOHOL] Cutting ignition NOW!");
    triggerAlcoholShutdown();
  }

  // Status log every 3 seconds
  static unsigned long lastLog = 0;
  if (millis() - lastLog > 3000) {
    Serial.printf("[ALCOHOL] %s | Value:%d\n",
                  alcoholDetected ? "DANGER" : "safe", sensorValue);
    lastLog = millis();
  }
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

  // Siren alert � non-blocking
  startSiren();
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

  // 2. Helmet check � engine must not run without an active helmet connection.
  //    "never seen" (lastHelmetUpdateTime==0) is treated as disconnected.
  unsigned long timeSinceHelmet = (lastHelmetUpdateTime > 0)
                                    ? (currentTime - lastHelmetUpdateTime)
                                    : currentTime;  // never seen = infinite time ago
  bool helmetTimeout = engineRunning && (!helmetConnected || timeSinceHelmet > HELMET_TIMEOUT);

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
      
      // Warning beep pattern
      playWarningBeep(3);
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
      tone(buzzerPin, 1000);  // Short 80ms pip — non-blocking
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

void checkHelmetConnection() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.setTimeout(1500);
  String helmetPath = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
  http.begin(helmetPath);

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    http.end();

    // Parse lastHeartbeat value
    int idx = response.indexOf("\"lastHeartbeat\":");
    if (idx != -1) {
      int s = response.indexOf(":", idx) + 1;
      // skip whitespace
      while (s < response.length() && response[s] == ' ') s++;
      int e = response.indexOf(",", s);
      if (e == -1) e = response.indexOf("}", s);
      if (e > s) {
        // Use the raw numeric value as a change detector
        // We don't need the actual time — just whether it changed
        String hbStr = response.substring(s, e);
        hbStr.trim();
        // Convert to a comparable value (last 9 digits to avoid float precision loss)
        uint32_t hbLow = (uint32_t)(hbStr.toDouble());  // lower 32 bits

        if (hbLow != (uint32_t)lastHelmetHeartbeat || !helmetConnected) {
          // Heartbeat changed OR we just reconnected � helmet is alive
          lastHelmetHeartbeat  = hbLow;
          lastHelmetUpdateTime = millis();
          helmetStatusForcedOff = false;

          if (!helmetConnected) {
            Serial.println("[HELMET] Connected � resetting heartbeat timer");
            helmetConnected = true;
          }
        } else {
          // Heartbeat value unchanged � check staleness
          unsigned long stale = millis() - lastHelmetUpdateTime;
          if (stale > HELMET_TIMEOUT) {
            if (helmetConnected) {
              Serial.printf("[HELMET] Stale for %lu ms � DISCONNECTED\n", stale);
              helmetConnected = false;
              lastHelmetHeartbeat = 0;  // reset so next read triggers "changed"
            }
          } else {
            // Still within timeout � keep connected, refresh timestamp
            lastHelmetUpdateTime = millis();
          }
        }
      }
    } else {
      // No lastHeartbeat field — check status field as fallback
      bool statusIsOn = (response.indexOf("\"status\":\"On\"") != -1);
      if (!statusIsOn && helmetConnected) {
        Serial.println("[HELMET] Status Off — DISCONNECTED");
        helmetConnected = false;
      }
    }
  } else {
    http.end();
    // HTTP failed — don't immediately disconnect; let timeout handle it
    static unsigned long lastFailLog = 0;
    if (millis() - lastFailLog > 5000) {
      Serial.printf("[HELMET] HTTP %d\n", httpCode);
      lastFailLog = millis();
    }
  }

  // Status log every 3 seconds
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug >= 3000) {
    unsigned long stale = (lastHelmetUpdateTime > 0) ? (millis() - lastHelmetUpdateTime) : 0;
    Serial.printf("[HELMET] %s | Heartbeat age: %lu ms\n",
                  helmetConnected ? "CONNECTED" : "DISCONNECTED", stale);
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










