
// ═══════════════════════════════════════════════════════════════════════════
// VIGILERT MOTORCYCLE MODULE — PROVISIONED (reads UID from flash)
// ═══════════════════════════════════════════════════════════════════════════
// Key difference from MotorcycleModule_COMPLETE_WITH_SMS.ino:
//   • WiFi SSID/password and userUID are read from Preferences (flash)
//     instead of being hardcoded.
//   • If no UID is stored the device falls back to provisioning AP mode.
//   • Serial commands "RESET DEVICE" and "SHOW CONFIG" are added.
//   • All Firebase paths are built dynamically using the variable userUID.
// ═══════════════════════════════════════════════════════════════════════════

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <MPU6050.h>

// ─── Firebase host (same for all users) ─────────────────────────────────────
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";

// ─── Runtime credentials (loaded from Preferences in setup()) ───────────────
String userUID   = "";   // set from Preferences — NOT hardcoded
String wifiSSID  = "";
String wifiPass  = "";

// ─── Firebase paths — built dynamically after userUID is loaded ─────────────
// (declared as String variables, assigned in setup() once userUID is known)
String devicePath        = "";
String helmetPath        = "";
String buttonPath        = "";
String livePath          = "";
String crashPath         = "";
String alcoholPath       = "";
String speedLimitPath    = "";
String alertsPath        = "";
String tripsPath         = "";
String emergencyContactPath = "";

// ─── Pin Assignments ─────────────────────────────────────────────────────────
const int relayPin           = 13;
const int ignitionRelayPin   = 27;
const int buzzerPin          = 12;
const int vibrationSensorPin = 15;

// ─── MPU6050 I2C ─────────────────────────────────────────────────────────────
const int SDA_PIN = 21;
const int SCL_PIN = 22;

// ─── SIM800L GSM ─────────────────────────────────────────────────────────────
const int GSM_RX_PIN    = 25;
const int GSM_TX_PIN    = 26;
const int GSM_POWER_PIN = 4;

// ─── Safety thresholds ───────────────────────────────────────────────────────
const float LEAN_ANGLE_THRESHOLD  = 40.0;
const float LEAN_ANGLE_WARNING    = 30.0;
const float LEAN_ANGLE_CRITICAL   = 50.0;
const float CRASH_ACCEL_THRESHOLD = 15.0;
const int   ALCOHOL_THRESHOLD     = 3000;

// ─── Timing ──────────────────────────────────────────────────────────────────
const unsigned long ALCOHOL_SHUTDOWN_DELAY  = 2000;
const unsigned long LEAN_ANGLE_TIMEOUT      = 3000;
const unsigned long SAFETY_CHECK_INTERVAL   = 100;
const unsigned long ENGINE_RESTART_COOLDOWN = 5000;
const unsigned long HEARTBEAT_INTERVAL      = 500;
const unsigned long BUTTON_CHECK_INTERVAL   = 200;
const unsigned long GPS_UPDATE_INTERVAL     = 1000;
const unsigned long SENSOR_READ_INTERVAL    = 100;
const unsigned long LIVE_DATA_INTERVAL      = 500;
const unsigned long CRASH_COOLDOWN          = 3000;
const unsigned long SMS_COOLDOWN            = 30000;
const unsigned long SMS_TIMEOUT             = 10000;

// ─── Hardware serial objects ──────────────────────────────────────────────────
HardwareSerial gpsSerial(1);
HardwareSerial gsmSerial(2);
TinyGPSPlus    gps;
MPU6050        mpu;

// ─── Core states ─────────────────────────────────────────────────────────────
bool engineRunning    = false;
bool engineAllowed    = false;
bool crashDetected    = false;
bool antiTheftArmed   = false;
bool helmetConnected  = false;

// ─── Safety states ───────────────────────────────────────────────────────────
bool   alcoholDetected      = false;
bool   leanAngleExceeded    = false;
bool   leanAngleWarning     = false;
bool   safetyShutdownActive = false;
String shutdownReason       = "";
unsigned long lastSafetyCheck      = 0;
unsigned long alcoholDetectedTime  = 0;
unsigned long leanAngleStartTime   = 0;
unsigned long lastEngineShutdown   = 0;

// ─── MPU6050 variables ───────────────────────────────────────────────────────
int16_t ax, ay, az, gx, gy, gz;
float roll = 0.0, pitch = 0.0;
float rollOffset = 0.0, pitchOffset = 0.0;
bool  mpuInitialized = false;

float currentAccel      = 9.8;
float currentTotalAccel = 9.8;
float currentRoll       = 0.0;
float currentPitch      = 0.0;
float currentLeanAngle  = 0.0;
int   vibrationCount    = 0;
int   alcoholSensorValue = 0;

// ─── GPS variables ───────────────────────────────────────────────────────────
double currentLat  = 0.0;
double currentLng  = 0.0;
float  currentSpeed = 0.0;
bool   gpsValid    = false;
int    satelliteCount = 0;

// ─── Dashboard variables ─────────────────────────────────────────────────────
int    speedLimit      = 60;
bool   isOverSpeed     = false;
String riderStatus     = "Inactive";
String alertnessStatus = "Normal";

// ─── Timing variables ────────────────────────────────────────────────────────
unsigned long lastHeartbeat      = 0;
unsigned long lastButtonCheck    = 0;
unsigned long lastGPSUpdate      = 0;
unsigned long lastSensorRead     = 0;
unsigned long lastLiveDataUpdate = 0;
unsigned long lastCrashTime      = 0;

// ─── Trip tracking ───────────────────────────────────────────────────────────
bool   tripActive    = false;
unsigned long tripStartTime = 0;
double tripStartLat  = 0.0;
double tripStartLng  = 0.0;
float  tripDistance  = 0.0;
float  tripMaxSpeed  = 0.0;

// ─── SMS variables ───────────────────────────────────────────────────────────
bool   gsmInitialized      = false;
String emergencyPhoneNumber = "";
unsigned long lastSMSTime   = 0;

// ─── HTTP client ─────────────────────────────────────────────────────────────
HTTPClient http;

// ─── Provisioning AP (fallback) ──────────────────────────────────────────────
WebServer  provServer(80);
DNSServer  provDNS;
const IPAddress PROV_IP(192, 168, 4, 1);
const IPAddress PROV_SUBNET(255, 255, 255, 0);

// ═══════════════════════════════════════════════════════════════════════════
// FORWARD DECLARATIONS
// ═══════════════════════════════════════════════════════════════════════════
void initializeMPU6050();
void configureMPU6050();
void calibrateMPU6050();
void performSafetyChecks();
void checkLeanAngleSafety();
void checkAlcoholSafety();
void checkHelmetConnection();
void checkSafetyRecovery();
void forceEngineShutdown(String reason);
void connectToWiFi();
void testFirebaseConnection();
void sendDashboardHeartbeat();
void updateLiveDataForDashboards();
void checkDashboardButtonWithSafety();
bool validateSafetyConditions();
String getSafetyBlockingReasons();
void startEngine();
void stopEngine();
void readAllSensors();
void updateGPSData();
void processCrashDetection();
void sendCrashDataToDashboards();
void processAntiTheft();
void processTripTracking();
void startTrip();
void endTrip();
void saveTripToFirebase(unsigned long duration);
void processSpeedMonitoring();
void loadSpeedLimit();
void sendDashboardAlert(String message, String type);
void playAlertSound(int beeps, int duration);
String getCurrentTimeString();
void handleSerialCommands();
void testLeanAngleReading();
void printDetailedStatus();
void initializeGSM();
bool testGSMConnection();
void loadEmergencyContact();
bool sendSMS(String message, String alertType);
void logSMSToFirebase(String message, String alertType);
void startProvisioningAP();

// ═══════════════════════════════════════════════════════════════════════════
// PROVISIONING FALLBACK — served when no UID is stored
// ═══════════════════════════════════════════════════════════════════════════
const char PROV_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html lang="en"><head>
<meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>VIGILERT Setup</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;
     background:linear-gradient(135deg,#1a2a6c,#3D52A0,#7091E6);
     min-height:100vh;display:flex;align-items:center;justify-content:center;padding:16px}
.card{background:#fff;border-radius:20px;box-shadow:0 20px 60px rgba(0,0,0,.35);
      max-width:460px;width:100%;overflow:hidden}
.hdr{background:linear-gradient(135deg,#3D52A0,#7091E6);padding:24px;text-align:center;color:#fff}
.hdr h1{font-size:1.5rem;font-weight:800}
.hdr p{font-size:.85rem;opacity:.85;margin-top:4px}
.body{padding:24px}
.fg{margin-bottom:16px}
label{display:block;font-size:.82rem;font-weight:600;color:#374151;margin-bottom:5px}
input,select{width:100%;padding:10px 13px;border:1.5px solid #d1d5db;border-radius:9px;font-size:.95rem}
input:focus,select:focus{outline:none;border-color:#3D52A0}
.btn{width:100%;padding:13px;background:linear-gradient(135deg,#3D52A0,#7091E6);
     color:#fff;border:none;border-radius:11px;font-size:1rem;font-weight:700;cursor:pointer;margin-top:6px}
</style></head><body>
<div class="card">
<div class="hdr"><h1>VIGILERT Setup</h1><p>Device not configured — please enter your details</p></div>
<div class="body">
<form method="POST" action="/save">
<div class="fg"><label>WiFi Network (SSID)</label>
<input type="text" name="ssid" required autocomplete="off" autocorrect="off" autocapitalize="none"></div>
<div class="fg"><label>WiFi Password</label>
<input type="password" name="pass" autocomplete="new-password"></div>
<div class="fg"><label>User ID (from VIGILERT app)</label>
<input type="text" name="uid" required autocomplete="off" autocorrect="off" autocapitalize="none"></div>
<div class="fg"><label>Device Type</label>
<select name="devtype" required>
<option value="" disabled selected>Select…</option>
<option value="motorcycle">Motorcycle Module</option>
<option value="helmet">Helmet Module</option>
</select></div>
<button type="submit" class="btn">Save &amp; Connect</button>
</form></div></div></body></html>
)rawliteral";

void handleProvRoot() {
  provServer.send_P(200, "text/html", PROV_HTML);
}

void handleProvSave() {
  String newSSID    = provServer.arg("ssid");
  String newPass    = provServer.arg("pass");
  String newUID     = provServer.arg("uid");
  String newDevType = provServer.arg("devtype");
  newSSID.trim(); newUID.trim(); newDevType.trim();

  if (newSSID.length() == 0 || newUID.length() == 0) {
    provServer.send(400, "text/plain", "Missing ssid or uid");
    return;
  }

  Preferences p;
  p.begin("vigilert", false);
  p.putString("wifi_ssid",   newSSID);
  p.putString("wifi_pass",   newPass);
  p.putString("user_uid",    newUID);
  p.putString("device_type", newDevType);
  p.end();

  provServer.send(200, "text/html",
    "<html><body style='font-family:sans-serif;text-align:center;padding:40px'>"
    "<h2 style='color:#22c55e'>&#9989; Saved!</h2>"
    "<p>Device will reboot and connect to your WiFi.</p></body></html>");

  delay(2000);
  ESP.restart();
}

void handleProvRedirect() {
  provServer.sendHeader("Location", "http://192.168.4.1/", true);
  provServer.send(302, "text/plain", "");
}

void startProvisioningAP() {
  Serial.println("\n[PROV] ⚠️  No User ID configured — entering provisioning mode");

  WiFi.mode(WIFI_AP);
  delay(100);

  String mac = WiFi.macAddress();
  mac.replace(":", "");
  String apSSID = "VIGILERT-SETUP-" + mac.substring(mac.length() - 4);
  apSSID.toUpperCase();

  WiFi.softAPConfig(PROV_IP, PROV_IP, PROV_SUBNET);
  WiFi.softAP(apSSID.c_str(), "vigilert123");

  Serial.printf("[PROV] AP SSID:     %s\n", apSSID.c_str());
  Serial.printf("[PROV] AP Password: vigilert123\n");
  Serial.printf("[PROV] Portal URL:  http://192.168.4.1/\n");

  provDNS.start(53, "*", PROV_IP);

  provServer.on("/",                     HTTP_GET,  handleProvRoot);
  provServer.on("/save",                 HTTP_POST, handleProvSave);
  provServer.on("/hotspot-detect.html",  HTTP_GET,  handleProvRedirect);
  provServer.on("/generate_204",         HTTP_GET,  handleProvRedirect);
  provServer.on("/connecttest.txt",      HTTP_GET,  handleProvRedirect);
  provServer.onNotFound(handleProvRedirect);
  provServer.begin();

  Serial.println("[PROV] Connect your phone to the VIGILERT-SETUP network to configure.");

  // Loop forever in provisioning mode
  while (true) {
    provDNS.processNextRequest();
    provServer.handleClient();
  }
}

// ═══════════════════════════════════════════════════════════════════════════
// SETUP
// ═══════════════════════════════════════════════════════════════════════════
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n╔══════════════════════════════════════════════════════════════╗");
  Serial.println("║   VIGILERT MOTORCYCLE MODULE — PROVISIONED FIRMWARE          ║");
  Serial.println("╚══════════════════════════════════════════════════════════════╝");

  // ── Read credentials from Preferences ──────────────────────────────────
  {
    Preferences prefs;
    prefs.begin("vigilert", true);  // read-only
    userUID  = prefs.getString("user_uid",  "");
    wifiSSID = prefs.getString("wifi_ssid", "");
    wifiPass = prefs.getString("wifi_pass", "");
    prefs.end();
  }

  // ── Validate — fall back to provisioning AP if UID is missing ──────────
  if (userUID.length() == 0) {
    Serial.println("[SETUP] ❌ No User ID in Preferences — starting provisioning AP");
    startProvisioningAP();  // never returns
  }

  Serial.printf("[SETUP] ✅ User UID:  %s\n", userUID.c_str());
  Serial.printf("[SETUP] ✅ WiFi SSID: %s\n", wifiSSID.c_str());

  // ── Build Firebase paths dynamically ───────────────────────────────────
  devicePath           = "/helmet_public/" + userUID + "/devices/motorcycle.json";
  helmetPath           = "/helmet_public/" + userUID + "/devices/helmet.json";
  buttonPath           = "/" + userUID + "/engineControl/startButton.json";
  livePath             = "/helmet_public/" + userUID + "/live.json";
  crashPath            = "/helmet_public/" + userUID + "/crashes.json";
  alcoholPath          = "/helmet_public/" + userUID + "/alcohol/status.json";
  speedLimitPath       = "/" + userUID + "/speedLimit.json";
  alertsPath           = "/helmet_public/" + userUID + "/alerts.json";
  tripsPath            = "/helmet_public/" + userUID + "/trips.json";
  emergencyContactPath = "/helmet_public/" + userUID + "/emergencyContact.json";

  // ── Initialize pins ─────────────────────────────────────────────────────
  pinMode(relayPin,           OUTPUT);
  pinMode(ignitionRelayPin,   OUTPUT);
  pinMode(buzzerPin,          OUTPUT);
  pinMode(vibrationSensorPin, INPUT);
  pinMode(GSM_POWER_PIN,      OUTPUT);

  digitalWrite(relayPin,         LOW);
  digitalWrite(ignitionRelayPin, LOW);

  engineRunning       = false;
  engineAllowed       = false;
  safetyShutdownActive = false;

  Serial.println("[SETUP] 🔒 Relays initialized — engine BLOCKED");

  // ── Initialize MPU6050 ──────────────────────────────────────────────────
  initializeMPU6050();

  // ── Initialize GPS ──────────────────────────────────────────────────────
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("[GPS] Initialized on RX:16, TX:17");

  // ── Initialize GSM ──────────────────────────────────────────────────────
  initializeGSM();

  // ── Connect to WiFi ─────────────────────────────────────────────────────
  connectToWiFi();

  if (WiFi.status() == WL_CONNECTED) {
    testFirebaseConnection();
    loadSpeedLimit();
    loadEmergencyContact();

    Serial.println("[SETUP] 🔄 Establishing dashboard connection...");
    for (int i = 0; i < 3; i++) {
      Serial.printf("  Heartbeat %d/3...\n", i + 1);
      sendDashboardHeartbeat();
      delay(500);
    }
    Serial.println("[SETUP] ✅ Dashboard connection established!");
  }

  Serial.println("\n🚀 VIGILERT MOTORCYCLE MODULE READY (PROVISIONED)");
  Serial.printf("   UID:  %s\n", userUID.c_str());
  Serial.printf("   SSID: %s\n", wifiSSID.c_str());
  Serial.printf("   MAC:  %s\n", WiFi.macAddress().c_str());
  Serial.println("📋 Commands: START, STOP, STATUS, SHOW CONFIG, RESET DEVICE");
  Serial.println("             TEST SMS, SET PHONE, TEST CRASH, TEST LEAN,");
  Serial.println("             CALIBRATE MPU, RESET SAFETY, GSM STATUS\n");
}

// ═══════════════════════════════════════════════════════════════════════════
// MAIN LOOP
// ═══════════════════════════════════════════════════════════════════════════
void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastSafetyCheck >= SAFETY_CHECK_INTERVAL) {
    performSafetyChecks();
    lastSafetyCheck = currentTime;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi disconnected — reconnecting...");
    connectToWiFi();
    return;
  }

  if (currentTime - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    sendDashboardHeartbeat();
    lastHeartbeat = currentTime;
  }

  if (currentTime - lastButtonCheck >= BUTTON_CHECK_INTERVAL) {
    checkDashboardButtonWithSafety();
    lastButtonCheck = currentTime;
  }

  // GPS — drain serial buffer every loop iteration (critical for reliable parsing)
  updateGPSData();
  if (currentTime - lastGPSUpdate >= GPS_UPDATE_INTERVAL) {
    lastGPSUpdate = currentTime;
  }

  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    readAllSensors();
    lastSensorRead = currentTime;
  }

  if (currentTime - lastLiveDataUpdate >= LIVE_DATA_INTERVAL) {
    updateLiveDataForDashboards();
    lastLiveDataUpdate = currentTime;
  }

  processCrashDetection();
  processAntiTheft();
  processTripTracking();
  processSpeedMonitoring();
  handleSerialCommands();

  yield();
}
// ---------------------------------------------------------------------------
// GSM / SMS FUNCTIONS
// ---------------------------------------------------------------------------

void initializeGSM() {
  Serial.println("[GSM] Initializing SIM800L module...");
  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX_PIN, GSM_TX_PIN);
  digitalWrite(GSM_POWER_PIN, LOW);  delay(1000);
  digitalWrite(GSM_POWER_PIN, HIGH); delay(2000);
  digitalWrite(GSM_POWER_PIN, LOW);
  Serial.println("[GSM] Waiting for module to initialize...");
  delay(5000);
  if (testGSMConnection()) {
    gsmInitialized = true;
    Serial.println("[GSM] SIM800L initialized successfully!");
    if (!emergencyPhoneNumber.isEmpty()) {
      sendSMS("VIGILERT System Initialized. Motorcycle security system is now active.", "SYSTEM_INIT");
    }
  } else {
    gsmInitialized = false;
    Serial.println("[GSM] SIM800L initialization failed!");
  }
}

bool testGSMConnection() {
  while (gsmSerial.available()) gsmSerial.read();
  gsmSerial.println("AT");
  unsigned long t = millis();
  String resp = "";
  while (millis() - t < 5000) {
    if (gsmSerial.available()) {
      resp += gsmSerial.readString();
      if (resp.indexOf("OK") != -1) {
        gsmSerial.println("AT+CPIN?");
        delay(1000);
        resp = ""; t = millis();
        while (millis() - t < 3000) {
          if (gsmSerial.available()) { resp += gsmSerial.readString(); break; }
        }
        if (resp.indexOf("READY") != -1) {
          gsmSerial.println("AT+CMGF=1");
          delay(1000);
          return true;
        }
        return false;
      }
    }
  }
  return false;
}

void loadEmergencyContact() {
  if (WiFi.status() != WL_CONNECTED) return;
  http.begin(firebaseHost + emergencyContactPath);
  http.setTimeout(3000);
  int code = http.GET();
  if (code == 200) {
    String resp = http.getString();
    int s = resp.indexOf("\"phoneNumber\":\"");
    if (s != -1) {
      s += 15;
      int e = resp.indexOf("\"", s);
      if (e != -1) emergencyPhoneNumber = resp.substring(s, e);
    }
  }
  http.end();
}

bool sendSMS(String message, String alertType) {
  if (!gsmInitialized || emergencyPhoneNumber.isEmpty()) return false;
  if (millis() - lastSMSTime < SMS_COOLDOWN) return false;
  while (gsmSerial.available()) gsmSerial.read();
  gsmSerial.println("AT+CMGF=1"); delay(1000);
  gsmSerial.println("AT+CMGS=\"" + emergencyPhoneNumber + "\""); delay(1000);
  gsmSerial.print(message); delay(100);
  gsmSerial.write(26);
  unsigned long t = millis();
  String resp = "";
  bool sent = false;
  while (millis() - t < SMS_TIMEOUT) {
    if (gsmSerial.available()) {
      resp += gsmSerial.readString();
      if (resp.indexOf("+CMGS:") != -1 || resp.indexOf("OK") != -1) { sent = true; break; }
      if (resp.indexOf("ERROR") != -1) break;
    }
  }
  if (sent) { lastSMSTime = millis(); logSMSToFirebase(message, alertType); }
  return sent;
}

void logSMSToFirebase(String message, String alertType) {
  if (WiFi.status() != WL_CONNECTED) return;
  StaticJsonDocument<300> d;
  d["message"]   = message;
  d["alertType"] = alertType;
  d["phoneNumber"] = emergencyPhoneNumber;
  d["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  d["time"]      = getCurrentTimeString();
  d["location"]["lat"] = currentLat;
  d["location"]["lng"] = currentLng;
  d["gpsValid"]  = gpsValid;
  String payload; serializeJson(d, payload);
  String path = "/helmet_public/" + userUID + "/smsLogs/" + String(millis()) + ".json";
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000);
  http.PUT(payload);
  http.end();
}

// ---------------------------------------------------------------------------
// MPU6050 FUNCTIONS
// ---------------------------------------------------------------------------

void initializeMPU6050() {
  Wire.begin(SDA_PIN, SCL_PIN);
  mpu.initialize();
  if (mpu.testConnection()) {
    configureMPU6050();
    calibrateMPU6050();
    mpuInitialized = true;
    Serial.println("[MPU6050] Real lean angle detection ACTIVE");
  } else {
    mpuInitialized = false;
    Serial.println("[MPU6050] Connection failed � using simulated data");
  }
}

void configureMPU6050() {
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  mpu.setDLPFMode(MPU6050_DLPF_BW_20);
  mpu.setRate(9);
}

void calibrateMPU6050() {
  Serial.println("[MPU6050] Calibrating � keep motorcycle upright and still...");
  delay(3000);
  float rs = 0, ps = 0; int n = 0;
  unsigned long t = millis();
  while (millis() - t < 5000) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    rs += atan2(ay, az) * 180.0 / PI;
    ps += atan2(-ax, sqrt((float)ay*ay + (float)az*az)) * 180.0 / PI;
    n++; delay(50);
  }
  rollOffset  = rs / n;
  pitchOffset = ps / n;
  Serial.printf("[MPU6050] Calibration done. Roll offset: %.2f  Pitch offset: %.2f\n", rollOffset, pitchOffset);
}

// ---------------------------------------------------------------------------
// SAFETY SYSTEM
// ---------------------------------------------------------------------------

void performSafetyChecks() {
  checkAlcoholSafety();
  checkLeanAngleSafety();
  checkHelmetConnection();
  checkSafetyRecovery();
}

void checkLeanAngleSafety() {
  if (mpuInitialized) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    roll  = atan2(ay, az) * 180.0 / PI - rollOffset;
    pitch = atan2(-ax, sqrt((float)ay*ay + (float)az*az)) * 180.0 / PI - pitchOffset;
    currentLeanAngle  = abs(roll);
    currentRoll       = roll;
    currentPitch      = pitch;
    currentTotalAccel = sqrt((ax/16384.0)*(ax/16384.0)+(ay/16384.0)*(ay/16384.0)+(az/16384.0)*(az/16384.0))*9.8;
  } else {
    static unsigned long lu = 0;
    if (millis() - lu > 100) {
      currentLeanAngle = engineRunning ? abs(random(0,30) + sin(millis()/1000.0)*5) : random(0,5);
      currentRoll  = currentLeanAngle;
      currentPitch = random(-5,5);
      lu = millis();
    }
  }

  if (currentLeanAngle >= LEAN_ANGLE_THRESHOLD && !leanAngleExceeded) {
    leanAngleExceeded  = true;
    leanAngleStartTime = millis();
    playAlertSound(3, 150);
    sendDashboardAlert("Excessive lean angle: " + String(currentLeanAngle,1) + "deg", "danger");
    String msg = "VIGILERT ALERT: Excessive lean angle (" + String(currentLeanAngle,1) + "deg). ";
    if (gpsValid) msg += "Location: https://maps.google.com/?q=" + String(currentLat,6) + "," + String(currentLng,6);
    else msg += "GPS unavailable.";
    sendSMS(msg, "LEAN_ANGLE");
  }

  if (currentLeanAngle >= LEAN_ANGLE_WARNING && !leanAngleWarning && !leanAngleExceeded) {
    leanAngleWarning = true;
    sendDashboardAlert("Lean angle warning: " + String(currentLeanAngle,1) + "deg", "warning");
  }

  if (leanAngleExceeded && engineRunning) {
    if (millis() - leanAngleStartTime >= LEAN_ANGLE_TIMEOUT && currentLeanAngle >= LEAN_ANGLE_THRESHOLD) {
      safetyShutdownActive = true;
      shutdownReason = "Excessive Lean Angle";
      lastEngineShutdown = millis();
      forceEngineShutdown("LEAN_ANGLE_EXCEEDED");
      crashDetected = true;
      sendCrashDataToDashboards();
      String msg = "VIGILERT EMERGENCY: Auto shutdown � excessive lean (" + String(currentLeanAngle,1) + "deg). ";
      if (gpsValid) msg += "Location: https://maps.google.com/?q=" + String(currentLat,6) + "," + String(currentLng,6);
      sendSMS(msg, "EMERGENCY_SHUTDOWN");
    }
  }

  if (leanAngleExceeded && currentLeanAngle < (LEAN_ANGLE_THRESHOLD - 5.0)) {
    leanAngleExceeded = false;
    sendDashboardAlert("Lean angle normalized: " + String(currentLeanAngle,1) + "deg", "success");
  }
  if (leanAngleWarning && currentLeanAngle < (LEAN_ANGLE_WARNING - 5.0)) {
    leanAngleWarning = false;
  }
}

void checkAlcoholSafety() {
  if (WiFi.status() != WL_CONNECTED) return;
  http.begin(firebaseHost + alcoholPath);
  http.setTimeout(2000);
  int code = http.GET();
  if (code == 200) {
    String resp = http.getString();
    bool detected = (resp.indexOf("\"Danger\"") != -1);
    int s = resp.indexOf("\"sensorValue\":");
    if (s != -1) {
      s += 14;
      int e = resp.indexOf(",", s); if (e == -1) e = resp.indexOf("}", s);
      if (e != -1) alcoholSensorValue = resp.substring(s, e).toInt();
    }
    if (detected && !alcoholDetected) {
      alcoholDetected = true; alcoholDetectedTime = millis();
      playAlertSound(5, 200);
      sendDashboardAlert("ALCOHOL DETECTED � Engine shutdown imminent", "danger");
      String msg = "VIGILERT ALERT: Alcohol detected (Level: " + String(alcoholSensorValue) + "). Shutdown in 2s. ";
      if (gpsValid) msg += "Location: https://maps.google.com/?q=" + String(currentLat,6) + "," + String(currentLng,6);
      sendSMS(msg, "ALCOHOL_DETECTED");
    }
    if (alcoholDetected && engineRunning && millis() - alcoholDetectedTime >= ALCOHOL_SHUTDOWN_DELAY) {
      safetyShutdownActive = true; shutdownReason = "Alcohol Detection"; lastEngineShutdown = millis();
      forceEngineShutdown("ALCOHOL_DETECTED");
      String msg = "VIGILERT EMERGENCY: Engine shut down � alcohol (Level: " + String(alcoholSensorValue) + "). ";
      if (gpsValid) msg += "Location: https://maps.google.com/?q=" + String(currentLat,6) + "," + String(currentLng,6);
      sendSMS(msg, "ALCOHOL_SHUTDOWN");
    }
    if (!detected && alcoholDetected) {
      alcoholDetected = false;
      sendDashboardAlert("Alcohol levels normalized", "success");
    }
  }
  http.end();
}

void checkHelmetConnection() {
  if (WiFi.status() != WL_CONNECTED) return;
  http.begin(firebaseHost + helmetPath);
  http.setTimeout(1000);
  int code = http.GET();
  if (code == 200) {
    String resp = http.getString();
    bool connected = (resp.indexOf("\"On\"") != -1);
    if (connected != helmetConnected) {
      helmetConnected = connected;
      if (helmetConnected) {
        sendDashboardAlert("Helmet connected", "success");
      } else {
        sendDashboardAlert("Helmet disconnected � safety monitoring limited", "danger");
        if (engineRunning) {
          String msg = "VIGILERT ALERT: Helmet disconnected while riding. ";
          if (gpsValid) msg += "Location: https://maps.google.com/?q=" + String(currentLat,6) + "," + String(currentLng,6);
          sendSMS(msg, "HELMET_DISCONNECT");
        }
      }
    }
  }
  http.end();
}

void checkSafetyRecovery() {
  bool canRestart = true;
  if (alcoholDetected)    canRestart = false;
  if (leanAngleExceeded)  canRestart = false;
  if (crashDetected) {
    unsigned long t = millis() - lastCrashTime;
    bool stable = (currentLeanAngle < 15.0) && (currentTotalAccel < 12.0);
    if (t > 10000 && stable) {
      crashDetected = false;
      sendDashboardAlert("Crash recovery � motorcycle stable, restart available", "success");
    } else { canRestart = false; }
  }
  if (safetyShutdownActive && (millis() - lastEngineShutdown < ENGINE_RESTART_COOLDOWN)) canRestart = false;
  if (safetyShutdownActive && canRestart) {
    safetyShutdownActive = false; shutdownReason = "";
    sendDashboardAlert("Safety conditions normalized � engine restart available", "success");
  }
  engineAllowed = canRestart;
}

void forceEngineShutdown(String reason) {
  digitalWrite(relayPin, LOW);
  digitalWrite(ignitionRelayPin, LOW);
  engineRunning = false; engineAllowed = false;
  riderStatus = "Safety Shutdown";
  for (int i = 0; i < 10; i++) { digitalWrite(buzzerPin,HIGH); delay(100); digitalWrite(buzzerPin,LOW); delay(100); }
  sendDashboardHeartbeat();
  updateLiveDataForDashboards();
  Serial.printf("[SAFETY] Engine shutdown: %s\n", reason.c_str());
}

// ---------------------------------------------------------------------------
// WIFI & FIREBASE
// ---------------------------------------------------------------------------

void connectToWiFi() {
  Serial.printf("[WIFI] Connecting to: %s\n", wifiSSID.c_str());
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(wifiSSID.c_str(), wifiPass.c_str());
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) { delay(500); Serial.print("."); attempts++; }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\n[WIFI] Connected! IP: %s  Signal: %d dBm\n", WiFi.localIP().toString().c_str(), WiFi.RSSI());
  } else {
    Serial.println("\n[WIFI] Connection failed!");
  }
}

void testFirebaseConnection() {
  http.begin(firebaseHost + devicePath);
  http.setTimeout(5000);
  int code = http.GET();
  Serial.printf("[FIREBASE] Connection test: HTTP %d\n", code);
  http.end();
}

// ---------------------------------------------------------------------------
// DASHBOARD INTEGRATION
// ---------------------------------------------------------------------------

void sendDashboardHeartbeat() {
  if (WiFi.status() != WL_CONNECTED) return;
  StaticJsonDocument<800> d;
  d["status"] = "On";
  d["lastHeartbeat"] = (uint64_t)(1700000000000ULL + millis());
  d["timestamp"]     = (uint64_t)(1700000000000ULL + millis());
  d["engineRunning"] = engineRunning;
  d["engineAllowed"] = engineAllowed;
  d["relayState"]    = digitalRead(relayPin);
  d["ignitionState"] = digitalRead(ignitionRelayPin);
  d["safetyShutdownActive"] = safetyShutdownActive;
  d["shutdownReason"]       = shutdownReason;
  d["alcoholDetected"]      = alcoholDetected;
  d["alcoholSensorValue"]   = alcoholSensorValue;
  d["leanAngleExceeded"]    = leanAngleExceeded;
  d["leanAngleWarning"]     = leanAngleWarning;
  d["currentLeanAngle"]     = currentLeanAngle;
  d["mpuInitialized"]       = mpuInitialized;
  d["gsmInitialized"]       = gsmInitialized;
  d["emergencyContact"]     = emergencyPhoneNumber;
  d["smsEnabled"]           = !emergencyPhoneNumber.isEmpty();
  d["gps"]["valid"]         = gpsValid;
  d["gps"]["latitude"]      = currentLat;
  d["gps"]["longitude"]     = currentLng;
  d["gps"]["speed"]         = currentSpeed;
  d["gps"]["satellites"]    = satelliteCount;
  d["sensors"]["acceleration"] = currentTotalAccel;
  d["sensors"]["roll"]         = currentRoll;
  d["sensors"]["pitch"]        = currentPitch;
  d["sensors"]["leanAngle"]    = currentLeanAngle;
  d["sensors"]["mpuConnected"] = mpuInitialized;
  d["crashDetected"]    = crashDetected;
  d["helmetConnected"]  = helmetConnected;
  d["antiTheftArmed"]   = antiTheftArmed;
  d["vibrationSensor"]  = digitalRead(vibrationSensorPin);
  d["freeHeap"]         = ESP.getFreeHeap();
  d["wifiSignal"]       = WiFi.RSSI();
  d["uptime"]           = millis();
  d["connected"]        = true;
  d["deviceType"]       = "motorcycle";
  d["version"]          = "provisioned_v1.0";
  String payload; serializeJson(d, payload);
  http.begin(firebaseHost + devicePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);
  http.PUT(payload);
  http.end();
}

void updateLiveDataForDashboards() {
  if (WiFi.status() != WL_CONNECTED) return;
  StaticJsonDocument<700> d;
  d["engineRunning"]        = engineRunning;
  d["engineAllowed"]        = engineAllowed;
  d["crashDetected"]        = crashDetected;
  d["antiTheftArmed"]       = antiTheftArmed;
  d["alcoholDetected"]      = alcoholDetected;
  d["helmetConnected"]      = helmetConnected;
  d["safetyShutdownActive"] = safetyShutdownActive;
  d["shutdownReason"]       = shutdownReason;
  d["leanAngleExceeded"]    = leanAngleExceeded;
  d["leanAngleWarning"]     = leanAngleWarning;
  d["currentLeanAngle"]     = currentLeanAngle;
  d["alcoholSensorValue"]   = alcoholSensorValue;
  d["mpuInitialized"]       = mpuInitialized;
  d["gsmInitialized"]       = gsmInitialized;
  d["smsEnabled"]           = !emergencyPhoneNumber.isEmpty();
  d["sensorData"]["roll"]         = currentRoll;
  d["sensorData"]["pitch"]        = currentPitch;
  d["sensorData"]["leanAngle"]    = currentLeanAngle;
  d["sensorData"]["acceleration"] = currentTotalAccel;
  d["locationLat"] = currentLat;
  d["locationLng"] = currentLng;
  d["speed"]       = currentSpeed;
  d["gpsValid"]    = gpsValid;
  d["isOverSpeed"] = isOverSpeed;
  if (safetyShutdownActive)    d["riderStatus"] = "Safety Shutdown";
  else if (alcoholDetected)    d["riderStatus"] = "Alcohol Detected";
  else if (leanAngleExceeded)  d["riderStatus"] = "Excessive Lean";
  else if (leanAngleWarning)   d["riderStatus"] = "Lean Warning";
  else                         d["riderStatus"] = riderStatus;
  d["alertnessStatus"] = alertnessStatus;
  d["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  String payload; serializeJson(d, payload);
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000);
  http.PUT(payload);
  http.end();
}

void checkDashboardButtonWithSafety() {
  if (WiFi.status() != WL_CONNECTED) return;
  http.begin(firebaseHost + buttonPath);
  http.setTimeout(3000);
  int code = http.GET();
  if (code == 200) {
    String resp = http.getString();
    bool pressed = (resp.indexOf("true") != -1 || resp.indexOf(":true") != -1);
    static bool lastState = false;
    if (pressed && !lastState) {
      if (!engineRunning) {
        if (validateSafetyConditions()) {
          startEngine();
          sendDashboardAlert("Engine started via dashboard", "success");
        } else {
          String reason = getSafetyBlockingReasons();
          sendDashboardAlert("Engine start blocked: " + reason, "danger");
          String msg = "VIGILERT ALERT: Engine start blocked. Reason: " + reason + ". ";
          if (gpsValid) msg += "Location: https://maps.google.com/?q=" + String(currentLat,6) + "," + String(currentLng,6);
          sendSMS(msg, "ENGINE_BLOCKED");
        }
      } else {
        stopEngine();
        sendDashboardAlert("Engine stopped via dashboard", "info");
      }
      for (int r = 0; r < 3; r++) {
        http.end();
        http.begin(firebaseHost + buttonPath);
        http.addHeader("Content-Type", "application/json");
        http.setTimeout(3000);
        if (http.PUT("false") == 200) break;
        delay(100);
      }
      sendDashboardHeartbeat();
      updateLiveDataForDashboards();
      lastState = true;
    } else if (!pressed) { lastState = false; }
  }
  http.end();
}

bool validateSafetyConditions() {
  if (alcoholDetected)    return false;
  if (leanAngleExceeded)  return false;
  if (crashDetected)      return false;
  if (safetyShutdownActive) return false;
  if (millis() - lastEngineShutdown < ENGINE_RESTART_COOLDOWN) return false;
  return true;
}

String getSafetyBlockingReasons() {
  String r = "";
  if (alcoholDetected)   r += "Alcohol detected (" + String(alcoholSensorValue) + "); ";
  if (leanAngleExceeded) r += "Excessive lean (" + String(currentLeanAngle,1) + "deg); ";
  if (crashDetected) {
    unsigned long t = millis() - lastCrashTime;
    if (t < 10000) r += "Crash recovery (" + String((10000-t)/1000) + "s); ";
    else r += "Crash detected; ";
  }
  if (safetyShutdownActive) r += "Safety shutdown (" + shutdownReason + "); ";
  if (millis() - lastEngineShutdown < ENGINE_RESTART_COOLDOWN)
    r += "Cooldown (" + String((ENGINE_RESTART_COOLDOWN-(millis()-lastEngineShutdown))/1000) + "s); ";
  if (r.length() > 2) r = r.substring(0, r.length() - 2);
  return r;
}

// ---------------------------------------------------------------------------
// ENGINE CONTROL
// ---------------------------------------------------------------------------

void startEngine() {
  digitalWrite(relayPin, HIGH);
  digitalWrite(ignitionRelayPin, HIGH);
  engineRunning = true; riderStatus = "Active";
  playAlertSound(2, 100);
  Serial.println("[ENGINE] Started");
  String msg = "VIGILERT: Engine started. ";
  if (gpsValid) msg += "Location: https://maps.google.com/?q=" + String(currentLat,6) + "," + String(currentLng,6);
  sendSMS(msg, "ENGINE_START");
}

void stopEngine() {
  digitalWrite(relayPin, LOW);
  digitalWrite(ignitionRelayPin, LOW);
  engineRunning = false; riderStatus = "Inactive";
  if (tripActive) endTrip();
  playAlertSound(1, 200);
  Serial.println("[ENGINE] Stopped");
  String msg = "VIGILERT: Engine stopped. ";
  if (gpsValid) msg += "Location: https://maps.google.com/?q=" + String(currentLat,6) + "," + String(currentLng,6);
  sendSMS(msg, "ENGINE_STOP");
}

// ---------------------------------------------------------------------------
// SENSOR READING
// ---------------------------------------------------------------------------

void readAllSensors() {
  if (digitalRead(vibrationSensorPin) == HIGH) vibrationCount++;
}

void updateGPSData() {
  unsigned long start = millis();
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
    if (millis() - start > 50) break;
  }

  if (gps.location.isUpdated() && gps.location.isValid()) {
    currentLat = gps.location.lat();
    currentLng = gps.location.lng();
    gpsValid   = true;
  }
  if (gps.speed.isUpdated() && gps.speed.isValid())           currentSpeed   = gps.speed.kmph();
  if (gps.satellites.isUpdated() && gps.satellites.isValid()) satelliteCount = gps.satellites.value();

  if (gpsValid && gps.location.age() > 10000) gpsValid = false;

  static unsigned long lastGPSDebug = 0;
  if (millis() - lastGPSDebug > 5000) {
    Serial.printf("[GPS] Valid: %s | Sats: %d | Speed: %.1f km/h | Lat: %.6f | Lng: %.6f | Chars: %lu | Sentences: %lu | Fails: %lu\n",
                  gpsValid ? "YES" : "NO",
                  satelliteCount, currentSpeed, currentLat, currentLng,
                  gps.charsProcessed(), gps.sentencesWithFix(), gps.failedChecksum());
    lastGPSDebug = millis();
  }
}

// ---------------------------------------------------------------------------
// CRASH DETECTION
// ---------------------------------------------------------------------------

void processCrashDetection() {
  if (crashDetected && (millis() - lastCrashTime < CRASH_COOLDOWN)) return;
  bool cond = false; String reason = "";
  if (currentLeanAngle >= LEAN_ANGLE_CRITICAL) { cond = true; reason = "Excessive lean (" + String(currentLeanAngle,1) + "deg)"; }
  if (currentTotalAccel >= CRASH_ACCEL_THRESHOLD) {
    cond = true;
    if (reason.length()) reason += " + ";
    reason += "High impact (" + String(currentTotalAccel,1) + "g)";
  }
  static unsigned long lvr = 0;
  if (millis() - lvr > 1000) {
    if (vibrationCount > 10 && engineRunning) {
      cond = true;
      if (reason.length()) reason += " + ";
      reason += "Abnormal vibration";
    }
    vibrationCount = 0; lvr = millis();
  }
  if (cond && !crashDetected) {
    crashDetected = true; lastCrashTime = millis();
    playAlertSound(10, 100);
    sendCrashDataToDashboards();
    String msg = "VIGILERT EMERGENCY: CRASH DETECTED! " + reason + ". ";
    if (gpsValid) msg += "Location: https://maps.google.com/?q=" + String(currentLat,6) + "," + String(currentLng,6) + " Speed: " + String(currentSpeed,1) + " km/h. ";
    else msg += "GPS unavailable. ";
    msg += "Please check on rider immediately!";
    sendSMS(msg, "CRASH_EMERGENCY");
    if (engineRunning) forceEngineShutdown("CRASH_DETECTED");
  }
}

void sendCrashDataToDashboards() {
  if (WiFi.status() != WL_CONNECTED) return;
  StaticJsonDocument<400> d;
  d["detected"]  = true;
  d["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  d["time"]      = getCurrentTimeString();
  d["location"]["lat"] = currentLat;
  d["location"]["lng"] = currentLng;
  d["gpsValid"]  = gpsValid;
  d["speed"]     = currentSpeed;
  d["leanAngle"] = currentLeanAngle;
  d["acceleration"] = currentTotalAccel;
  d["engineRunning"] = engineRunning;
  d["severity"]  = (currentLeanAngle > 50.0 || currentTotalAccel > 20.0) ? "High" : "Medium";
  String payload; serializeJson(d, payload);
  String path = "/helmet_public/" + userUID + "/crashes/" + String(millis()) + ".json";
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);
  http.PUT(payload);
  http.end();
}

// ---------------------------------------------------------------------------
// ANTI-THEFT
// ---------------------------------------------------------------------------

void processAntiTheft() {
  if (!engineRunning && !antiTheftArmed) {
    static unsigned long lvc = 0;
    if (millis() - lvc > 2000) {
      if (vibrationCount > 3) {
        antiTheftArmed = true;
        playAlertSound(5, 150);
        sendDashboardAlert("Anti-theft armed � movement detected", "warning");
        String msg = "VIGILERT ALERT: Anti-theft activated. Movement detected while engine off. ";
        if (gpsValid) msg += "Location: https://maps.google.com/?q=" + String(currentLat,6) + "," + String(currentLng,6);
        sendSMS(msg, "ANTI_THEFT");
        vibrationCount = 0;
      }
      lvc = millis();
    }
  }
  if (antiTheftArmed && engineRunning) {
    antiTheftArmed = false;
    sendDashboardAlert("Anti-theft disarmed", "success");
  }
}

// ---------------------------------------------------------------------------
// TRIP TRACKING
// ---------------------------------------------------------------------------

void processTripTracking() {
  if (engineRunning && gpsValid && !tripActive) startTrip();
  else if (!engineRunning && tripActive) endTrip();
  if (tripActive && gpsValid) {
    if (currentSpeed > tripMaxSpeed) tripMaxSpeed = currentSpeed;
    static double ltLat = 0, ltLng = 0;
    static unsigned long ldu = 0;
    if (millis() - ldu > 5000 && ltLat != 0.0) {
      double dLat = currentLat - ltLat, dLng = currentLng - ltLng;
      tripDistance += sqrt(dLat*dLat + dLng*dLng) * 111.0;
      ltLat = currentLat; ltLng = currentLng; ldu = millis();
    } else if (ltLat == 0.0) { ltLat = currentLat; ltLng = currentLng; ldu = millis(); }
  }
}

void startTrip() {
  tripActive = true; tripStartTime = millis();
  tripStartLat = currentLat; tripStartLng = currentLng;
  tripDistance = 0; tripMaxSpeed = 0;
  Serial.println("[TRIP] Started");
}

void endTrip() {
  if (!tripActive) return;
  unsigned long dur = millis() - tripStartTime;
  Serial.printf("[TRIP] Ended � Duration: %lu min, Distance: %.2f km, Max: %.1f km/h\n",
                dur/60000, tripDistance, tripMaxSpeed);
  saveTripToFirebase(dur);
  tripActive = false;
}

void saveTripToFirebase(unsigned long duration) {
  if (WiFi.status() != WL_CONNECTED) return;
  StaticJsonDocument<500> d;
  d["startTime"]  = (uint64_t)(1700000000000ULL + tripStartTime);
  d["endTime"]    = (uint64_t)(1700000000000ULL + millis());
  d["duration"]   = duration;
  d["distance"]   = tripDistance;
  d["maxSpeed"]   = tripMaxSpeed;
  d["startLocation"]["lat"] = tripStartLat;
  d["startLocation"]["lng"] = tripStartLng;
  d["endLocation"]["lat"]   = currentLat;
  d["endLocation"]["lng"]   = currentLng;
  d["timestamp"]  = (uint64_t)(1700000000000ULL + millis());
  d["time"]       = getCurrentTimeString();
  String payload; serializeJson(d, payload);
  String path = "/helmet_public/" + userUID + "/trips/" + String(millis()) + ".json";
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);
  http.PUT(payload);
  http.end();
}

// ---------------------------------------------------------------------------
// SPEED MONITORING
// ---------------------------------------------------------------------------

void processSpeedMonitoring() {
  static unsigned long lsc = 0;
  if (millis() - lsc < 1000) return;
  if (gpsValid && engineRunning) {
    bool over = (currentSpeed > speedLimit);
    if (over && !isOverSpeed) {
      isOverSpeed = true;
      sendDashboardAlert("Speed limit exceeded: " + String(currentSpeed,1) + " km/h", "warning");
      String msg = "VIGILERT ALERT: Speed limit exceeded (" + String(currentSpeed,1) + " km/h > " + String(speedLimit) + " km/h). ";
      if (gpsValid) msg += "Location: https://maps.google.com/?q=" + String(currentLat,6) + "," + String(currentLng,6);
      sendSMS(msg, "SPEED_VIOLATION");
    } else if (!over && isOverSpeed) {
      isOverSpeed = false;
      sendDashboardAlert("Speed normalized: " + String(currentSpeed,1) + " km/h", "success");
    }
  }
  lsc = millis();
}

void loadSpeedLimit() {
  if (WiFi.status() != WL_CONNECTED) return;
  http.begin(firebaseHost + speedLimitPath);
  http.setTimeout(3000);
  int code = http.GET();
  if (code == 200) {
    int lim = http.getString().toInt();
    if (lim > 0 && lim <= 200) speedLimit = lim;
  }
  http.end();
}

// ---------------------------------------------------------------------------
// UTILITY FUNCTIONS
// ---------------------------------------------------------------------------

void sendDashboardAlert(String message, String type) {
  if (WiFi.status() != WL_CONNECTED) return;
  StaticJsonDocument<300> d;
  d["message"]    = message;
  d["type"]       = type;
  d["timestamp"]  = (uint64_t)(1700000000000ULL + millis());
  d["time"]       = getCurrentTimeString();
  d["deviceType"] = "motorcycle";
  d["read"]       = false;
  String payload; serializeJson(d, payload);
  String path = "/helmet_public/" + userUID + "/alerts/" + String(millis()) + ".json";
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000);
  http.PUT(payload);
  http.end();
}

void playAlertSound(int beeps, int duration) {
  for (int i = 0; i < beeps; i++) {
    digitalWrite(buzzerPin, HIGH); delay(duration);
    digitalWrite(buzzerPin, LOW);  delay(duration);
  }
}

String getCurrentTimeString() {
  unsigned long s = millis() / 1000;
  return String((s/3600)%24) + ":" + String((s/60)%60) + ":" + String(s%60);
}

// ---------------------------------------------------------------------------
// SERIAL COMMAND HANDLER
// ---------------------------------------------------------------------------

void handleSerialCommands() {
  if (!Serial.available()) return;
  String cmd = Serial.readStringUntil('\n');
  cmd.trim(); cmd.toUpperCase();
  Serial.printf("\n[CMD] Received: %s\n", cmd.c_str());

  if (cmd == "START") {
    if (validateSafetyConditions()) startEngine();
    else { Serial.println("[CMD] Blocked: " + getSafetyBlockingReasons()); }

  } else if (cmd == "STOP") {
    stopEngine();

  } else if (cmd == "STATUS") {
    printDetailedStatus();

  } else if (cmd == "SHOW CONFIG") {
    // -- SHOW CONFIG: print UID, SSID (masked password), MAC --------------
    Serial.println("\n[CONFIG] --------------------------------------");
    Serial.printf("[CONFIG]   User UID:  %s\n", userUID.c_str());
    Serial.printf("[CONFIG]   WiFi SSID: %s\n", wifiSSID.c_str());
    String masked = "";
    for (unsigned int i = 0; i < wifiPass.length(); i++) masked += "*";
    Serial.printf("[CONFIG]   WiFi Pass: %s\n", masked.c_str());
    Serial.printf("[CONFIG]   Device MAC: %s\n", WiFi.macAddress().c_str());
    Serial.println("[CONFIG] --------------------------------------");

  } else if (cmd == "RESET DEVICE") {
    // -- RESET DEVICE: clear Preferences and reboot into provisioning -----
    Serial.println("[CMD] Clearing all Preferences and rebooting into provisioning mode...");
    Preferences p;
    p.begin("vigilert", false);
    p.clear();
    p.end();
    Serial.println("[CMD] Preferences cleared. Rebooting in 2 seconds...");
    delay(2000);
    ESP.restart();

  } else if (cmd == "TEST SMS") {
    if (gsmInitialized && !emergencyPhoneNumber.isEmpty()) {
      String msg = "VIGILERT TEST: SMS system working. ";
      if (gpsValid) msg += "Location: https://maps.google.com/?q=" + String(currentLat,6) + "," + String(currentLng,6);
      sendSMS(msg, "TEST") ? Serial.println("[SMS] Test sent!") : Serial.println("[SMS] Test failed!");
    } else {
      Serial.printf("[SMS] Not available � GSM:%s Phone:%s\n",
                    gsmInitialized ? "OK" : "FAIL",
                    emergencyPhoneNumber.isEmpty() ? "EMPTY" : "SET");
    }

  } else if (cmd.startsWith("SET PHONE ")) {
    String num = cmd.substring(10); num.trim();
    if (num.length() >= 10) {
      emergencyPhoneNumber = num;
      Serial.printf("[SMS] Emergency contact set: %s\n", num.c_str());
    } else { Serial.println("[SMS] Invalid number. Use: SET PHONE +1234567890"); }

  } else if (cmd == "TEST CRASH") {
    crashDetected = true; lastCrashTime = millis();
    sendCrashDataToDashboards();
    String msg = "VIGILERT TEST: Crash detection test. ";
    if (gpsValid) msg += "Location: https://maps.google.com/?q=" + String(currentLat,6) + "," + String(currentLng,6);
    sendSMS(msg, "TEST_CRASH");
    Serial.println("[CMD] Crash simulation complete");

  } else if (cmd == "TEST LEAN") {
    testLeanAngleReading();

  } else if (cmd == "CALIBRATE MPU") {
    if (mpuInitialized) calibrateMPU6050();
    else Serial.println("[MPU] Not initialized");

  } else if (cmd == "RESET SAFETY") {
    crashDetected = alcoholDetected = leanAngleExceeded = leanAngleWarning = safetyShutdownActive = antiTheftArmed = false;
    shutdownReason = "";
    sendDashboardAlert("Safety systems reset", "success");
    Serial.println("[SAFETY] All flags cleared");

  } else if (cmd == "GSM STATUS") {
    Serial.printf("[GSM] Initialized: %s\n", gsmInitialized ? "YES" : "NO");
    Serial.printf("[GSM] Contact: %s\n", emergencyPhoneNumber.isEmpty() ? "NOT SET" : emergencyPhoneNumber.c_str());

  } else {
    Serial.println("[CMD] Unknown command. Available:");
    Serial.println("  START, STOP, STATUS, SHOW CONFIG, RESET DEVICE");
    Serial.println("  TEST SMS, SET PHONE +XXXXXXXXXX, TEST CRASH, TEST LEAN");
    Serial.println("  CALIBRATE MPU, RESET SAFETY, GSM STATUS");
  }
}

void testLeanAngleReading() {
  if (!mpuInitialized) { Serial.println("[MPU] Not initialized"); return; }
  Serial.println("[MPU] Testing lean angle for 10 seconds � tilt the device...");
  unsigned long t = millis(); float maxL = 0;
  while (millis() - t < 10000) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    float r = atan2(ay, az) * 180.0 / PI - rollOffset;
    float l = abs(r);
    if (l > maxL) maxL = l;
    static unsigned long lp = 0;
    if (millis() - lp >= 500) {
      Serial.printf("Roll: %6.1f  Lean: %6.1f  Accel: %6.2fg\n", r, l,
                    sqrt((ax/16384.0)*(ax/16384.0)+(ay/16384.0)*(ay/16384.0)+(az/16384.0)*(az/16384.0))*9.8);
      lp = millis();
    }
    delay(50);
  }
  Serial.printf("[MPU] Max lean: %.1f deg (threshold: %.1f deg)\n", maxL, LEAN_ANGLE_THRESHOLD);
}

void printDetailedStatus() {
  Serial.println("\n----------------------------------------------------------");
  Serial.println("  VIGILERT MOTORCYCLE MODULE � PROVISIONED � STATUS");
  Serial.println("----------------------------------------------------------");
  Serial.printf("  User UID:    %s\n", userUID.c_str());
  Serial.printf("  WiFi SSID:   %s\n", wifiSSID.c_str());
  Serial.printf("  Device MAC:  %s\n", WiFi.macAddress().c_str());
  Serial.println("----------------------------------------------------------");
  Serial.printf("  Engine:      %s\n", engineRunning ? "RUNNING" : "STOPPED");
  Serial.printf("  Allowed:     %s\n", engineAllowed ? "YES" : "NO");
  Serial.printf("  Safety:      %s\n", safetyShutdownActive ? ("SHUTDOWN � " + shutdownReason).c_str() : "OK");
  Serial.printf("  Alcohol:     %s (value: %d)\n", alcoholDetected ? "DETECTED" : "Clear", alcoholSensorValue);
  Serial.printf("  Lean angle:  %.1f deg (%s)\n", currentLeanAngle, leanAngleExceeded ? "EXCEEDED" : leanAngleWarning ? "WARNING" : "OK");
  Serial.printf("  Crash:       %s\n", crashDetected ? "DETECTED" : "None");
  Serial.printf("  Anti-theft:  %s\n", antiTheftArmed ? "ARMED" : "Disarmed");
  Serial.printf("  Helmet:      %s\n", helmetConnected ? "Connected" : "Disconnected");
  Serial.println("----------------------------------------------------------");
  Serial.printf("  GPS:         %s | Sats: %d | %.6f, %.6f | %.1f km/h\n",
                gpsValid ? "Valid" : "No fix", satelliteCount, currentLat, currentLng, currentSpeed);
  Serial.printf("  Speed limit: %d km/h (%s)\n", speedLimit, isOverSpeed ? "EXCEEDED" : "OK");
  Serial.println("----------------------------------------------------------");
  Serial.printf("  GSM:         %s\n", gsmInitialized ? "Connected" : "Disconnected");
  Serial.printf("  Contact:     %s\n", emergencyPhoneNumber.isEmpty() ? "Not set" : emergencyPhoneNumber.c_str());
  Serial.printf("  MPU6050:     %s\n", mpuInitialized ? "Connected" : "Simulated");
  Serial.printf("  WiFi:        %s (%d dBm)\n", WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected", WiFi.RSSI());
  Serial.printf("  Free heap:   %d bytes\n", ESP.getFreeHeap());
  Serial.printf("  Uptime:      %lu min\n", millis() / 60000);
  Serial.println("----------------------------------------------------------");
}
