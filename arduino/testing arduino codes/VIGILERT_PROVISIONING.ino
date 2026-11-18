// ═══════════════════════════════════════════════════════════════════════════
// VIGILERT PROVISIONING SKETCH
// ═══════════════════════════════════════════════════════════════════════════
// First-time device setup via captive portal WiFi AP.
// Flash this sketch to a new device. After provisioning, flash the main
// VIGILERT_MOTORCYCLE_PROVISIONED.ino firmware.
//
// Flow:
//   1. Boot → check Preferences for saved credentials
//   2. If all 4 keys present AND WiFi connects → print PROVISIONED, halt
//   3. Otherwise → start AP "VIGILERT-SETUP-XXXX", serve captive portal
//   4. User fills form → credentials saved to Preferences → reboot
// ═══════════════════════════════════════════════════════════════════════════

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <DNSServer.h>

// ─── AP Configuration ───────────────────────────────────────────────────────
const char* AP_PASSWORD   = "vigilert123";
const IPAddress AP_IP(192, 168, 4, 1);
const IPAddress AP_SUBNET(255, 255, 255, 0);
const byte DNS_PORT = 53;

// ─── Objects ────────────────────────────────────────────────────────────────
WebServer server(80);
DNSServer  dnsServer;
Preferences prefs;

// ─── Saved credentials (loaded on boot) ─────────────────────────────────────
String savedSSID     = "";
String savedPass     = "";
String savedUID      = "";
String savedDevType  = "";

// ─── HTML page stored in PROGMEM to save RAM ────────────────────────────────
const char SETUP_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>VIGILERT Device Setup</title>
<style>
  *{box-sizing:border-box;margin:0;padding:0}
  body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;
       background:linear-gradient(135deg,#1a2a6c 0%,#3D52A0 50%,#7091E6 100%);
       min-height:100vh;display:flex;align-items:center;justify-content:center;padding:16px}
  .card{background:#fff;border-radius:20px;box-shadow:0 20px 60px rgba(0,0,0,.35);
        max-width:480px;width:100%;overflow:hidden}
  .header{background:linear-gradient(135deg,#3D52A0,#7091E6);padding:28px 24px;text-align:center}
  .shield{width:56px;height:56px;margin:0 auto 12px}
  .brand{color:#fff;font-size:1.6rem;font-weight:800;letter-spacing:1px}
  .tagline{color:rgba(255,255,255,.8);font-size:.85rem;margin-top:4px}
  .steps{display:flex;justify-content:center;gap:8px;padding:16px 24px;
         background:#f0f4ff;border-bottom:1px solid #e0e8ff}
  .step{display:flex;flex-direction:column;align-items:center;flex:1;font-size:.7rem;color:#6b7280}
  .step-num{width:28px;height:28px;border-radius:50%;background:#d1d9f0;color:#3D52A0;
            font-weight:700;display:flex;align-items:center;justify-content:center;
            margin-bottom:4px;font-size:.8rem}
  .step.active .step-num{background:#3D52A0;color:#fff}
  .step.done .step-num{background:#22c55e;color:#fff}
  .step-line{flex:1;height:2px;background:#d1d9f0;margin-top:14px;max-width:24px}
  .body{padding:28px 24px}
  .form-group{margin-bottom:18px}
  label{display:block;font-size:.82rem;font-weight:600;color:#374151;margin-bottom:6px}
  input,select{width:100%;padding:11px 14px;border:1.5px solid #d1d5db;border-radius:10px;
               font-size:.95rem;color:#111827;outline:none;transition:border-color .2s}
  input:focus,select:focus{border-color:#3D52A0;box-shadow:0 0 0 3px rgba(61,82,160,.15)}
  .hint-toggle{font-size:.78rem;color:#3D52A0;cursor:pointer;text-decoration:underline;
               margin-top:4px;display:inline-block}
  .hint-box{background:#f0f4ff;border:1px solid #c7d2fe;border-radius:8px;padding:12px;
            margin-top:8px;font-size:.8rem;color:#374151;display:none;line-height:1.6}
  .hint-box.open{display:block}
  .hint-box ol{padding-left:18px;margin-top:6px}
  .hint-box li{margin-bottom:4px}
  .btn{width:100%;padding:14px;background:linear-gradient(135deg,#3D52A0,#7091E6);
       color:#fff;border:none;border-radius:12px;font-size:1rem;font-weight:700;
       cursor:pointer;transition:opacity .2s,transform .1s;margin-top:8px}
  .btn:hover{opacity:.92}
  .btn:active{transform:scale(.98)}
  .divider{border:none;border-top:1px solid #e5e7eb;margin:20px 0}
  .reset-link{text-align:center;font-size:.78rem;color:#9ca3af}
  .reset-link a{color:#ef4444;text-decoration:none;font-weight:600}
  .reset-link a:hover{text-decoration:underline}
  .footer{text-align:center;padding:14px;background:#f9fafb;
          font-size:.72rem;color:#9ca3af;border-top:1px solid #f0f0f0}
</style>
</head>
<body>
<div class="card">
  <!-- Header -->
  <div class="header">
    <svg class="shield" viewBox="0 0 56 56" fill="none" xmlns="http://www.w3.org/2000/svg">
      <path d="M28 4L8 12V28C8 39.05 16.8 49.36 28 52C39.2 49.36 48 39.05 48 28V12L28 4Z"
            fill="rgba(255,255,255,0.25)" stroke="white" stroke-width="2.5" stroke-linejoin="round"/>
      <path d="M20 28L25.5 33.5L36 23" stroke="white" stroke-width="3"
            stroke-linecap="round" stroke-linejoin="round"/>
    </svg>
    <div class="brand">VIGILERT</div>
    <div class="tagline">Motorcycle Safety System — Device Setup</div>
  </div>

  <!-- Step Indicators -->
  <div class="steps">
    <div class="step done">
      <div class="step-num">&#10003;</div>
      <span>Connect</span>
    </div>
    <div class="step-line"></div>
    <div class="step active">
      <div class="step-num">2</div>
      <span>Configure</span>
    </div>
    <div class="step-line"></div>
    <div class="step">
      <div class="step-num">3</div>
      <span>Done</span>
    </div>
  </div>

  <!-- Form -->
  <div class="body">
    <form method="POST" action="/save">
      <div class="form-group">
        <label for="ssid">WiFi Network (SSID)</label>
        <input type="text" id="ssid" name="ssid" placeholder="Your home or office WiFi name"
               required autocomplete="off" autocorrect="off" autocapitalize="none" spellcheck="false">
      </div>

      <div class="form-group">
        <label for="pass">WiFi Password</label>
        <input type="password" id="pass" name="pass" placeholder="WiFi password"
               autocomplete="new-password">
      </div>

      <div class="form-group">
        <label for="uid">User ID</label>
        <input type="text" id="uid" name="uid" placeholder="Paste your User ID from the VIGILERT app"
               required autocomplete="off" autocorrect="off" autocapitalize="none" spellcheck="false">
        <span class="hint-toggle" onclick="toggleHint()">&#9660; Where to find your User ID</span>
        <div class="hint-box" id="hintBox">
          Open the <strong>VIGILERT app</strong> on your phone, then:
          <ol>
            <li>Tap the <strong>Menu</strong> icon (top-left)</li>
            <li>Go to <strong>Device Setup</strong></li>
            <li>Your User ID is shown in the blue box at the top</li>
            <li>Tap it to copy, then paste it here</li>
          </ol>
        </div>
      </div>

      <div class="form-group">
        <label for="devtype">Device Type</label>
        <select id="devtype" name="devtype" required>
          <option value="" disabled selected>Select device type&hellip;</option>
          <option value="motorcycle">Motorcycle Module</option>
          <option value="helmet">Helmet Module</option>
        </select>
      </div>

      <button type="submit" class="btn">Save &amp; Connect</button>
    </form>

    <hr class="divider">
    <div class="reset-link">
      Already configured? <a href="/reset" onclick="return confirm('Clear all saved settings?')">Factory Reset</a>
    </div>
  </div>

  <div class="footer">VIGILERT &copy; 2024 &mdash; Motorcycle Safety System</div>
</div>

<script>
function toggleHint(){
  var b=document.getElementById('hintBox');
  b.classList.toggle('open');
}
</script>
</body>
</html>
)rawliteral";

// ─── Success page ────────────────────────────────────────────────────────────
const char SUCCESS_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>VIGILERT — Setup Complete</title>
<style>
  *{box-sizing:border-box;margin:0;padding:0}
  body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;
       background:linear-gradient(135deg,#1a2a6c,#3D52A0,#7091E6);
       min-height:100vh;display:flex;align-items:center;justify-content:center;padding:16px}
  .card{background:#fff;border-radius:20px;box-shadow:0 20px 60px rgba(0,0,0,.35);
        max-width:420px;width:100%;padding:40px 32px;text-align:center}
  .icon{font-size:4rem;margin-bottom:16px}
  h1{color:#22c55e;font-size:1.5rem;font-weight:800;margin-bottom:8px}
  p{color:#6b7280;font-size:.9rem;line-height:1.6;margin-bottom:12px}
  .steps{background:#f0fdf4;border:1px solid #bbf7d0;border-radius:12px;
         padding:16px;text-align:left;margin:20px 0}
  .steps ol{padding-left:20px;color:#374151;font-size:.85rem;line-height:2}
  .note{background:#fef9c3;border:1px solid #fde68a;border-radius:8px;
        padding:12px;font-size:.8rem;color:#92400e;margin-top:12px}
</style>
</head>
<body>
<div class="card">
  <div class="icon">&#9989;</div>
  <h1>Setup Complete!</h1>
  <p>Your VIGILERT device has been configured successfully.</p>
  <div class="steps">
    <strong style="color:#15803d">Next steps:</strong>
    <ol>
      <li>Disconnect your phone from <em>VIGILERT-SETUP-XXXX</em></li>
      <li>Reconnect to your normal WiFi</li>
      <li>Power-cycle (unplug &amp; replug) the VIGILERT device</li>
      <li>The device will connect to your WiFi automatically</li>
      <li>Open the VIGILERT app — your device will appear online</li>
    </ol>
  </div>
  <div class="note">
    &#9888; If the device does not appear online within 60 seconds, double-check your WiFi password and repeat setup.
  </div>
</div>
</body>
</html>
)rawliteral";

// ─── Reset confirmation page ─────────────────────────────────────────────────
const char RESET_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>VIGILERT — Reset</title>
<style>
  *{box-sizing:border-box;margin:0;padding:0}
  body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;
       background:linear-gradient(135deg,#1a2a6c,#3D52A0,#7091E6);
       min-height:100vh;display:flex;align-items:center;justify-content:center;padding:16px}
  .card{background:#fff;border-radius:20px;box-shadow:0 20px 60px rgba(0,0,0,.35);
        max-width:380px;width:100%;padding:40px 32px;text-align:center}
  .icon{font-size:3.5rem;margin-bottom:16px}
  h1{color:#ef4444;font-size:1.4rem;font-weight:800;margin-bottom:8px}
  p{color:#6b7280;font-size:.9rem;line-height:1.6}
  a{display:inline-block;margin-top:20px;padding:12px 28px;
    background:linear-gradient(135deg,#3D52A0,#7091E6);color:#fff;
    border-radius:10px;text-decoration:none;font-weight:700;font-size:.9rem}
</style>
</head>
<body>
<div class="card">
  <div class="icon">&#128260;</div>
  <h1>Device Reset</h1>
  <p>All saved credentials have been cleared. The device will restart in setup mode.</p>
  <a href="/">Configure Again</a>
</div>
</body>
</html>
)rawliteral";

// ═══════════════════════════════════════════════════════════════════════════
// HELPER: redirect any request to the captive portal
// ═══════════════════════════════════════════════════════════════════════════
void handleCaptiveRedirect() {
  server.sendHeader("Location", "http://192.168.4.1/", true);
  server.send(302, "text/plain", "");
}

// ═══════════════════════════════════════════════════════════════════════════
// ROUTE HANDLERS
// ═══════════════════════════════════════════════════════════════════════════
void handleRoot() {
  server.send_P(200, "text/html", SETUP_HTML);
}

void handleSave() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
    return;
  }

  String newSSID    = server.arg("ssid");
  String newPass    = server.arg("pass");
  String newUID     = server.arg("uid");
  String newDevType = server.arg("devtype");

  newSSID.trim();
  newUID.trim();
  newDevType.trim();

  if (newSSID.length() == 0 || newUID.length() == 0 || newDevType.length() == 0) {
    server.send(400, "text/plain", "Missing required fields: ssid, uid, devtype");
    return;
  }

  // Persist to flash
  prefs.begin("vigilert", false);
  prefs.putString("wifi_ssid",   newSSID);
  prefs.putString("wifi_pass",   newPass);
  prefs.putString("user_uid",    newUID);
  prefs.putString("device_type", newDevType);
  prefs.end();

  Serial.println("\n[PROV] ✅ Credentials saved to Preferences:");
  Serial.printf("[PROV]   SSID:        %s\n", newSSID.c_str());
  Serial.printf("[PROV]   UID:         %s\n", newUID.c_str());
  Serial.printf("[PROV]   Device type: %s\n", newDevType.c_str());
  Serial.println("[PROV] Rebooting in 3 seconds...");

  server.send_P(200, "text/html", SUCCESS_HTML);

  delay(3000);
  ESP.restart();
}

void handleReset() {
  prefs.begin("vigilert", false);
  prefs.clear();
  prefs.end();

  Serial.println("[PROV] 🔄 Preferences cleared — factory reset");

  server.send_P(200, "text/html", RESET_HTML);

  delay(2000);
  ESP.restart();
}

void handleStatus() {
  prefs.begin("vigilert", true);
  String uid  = prefs.getString("user_uid",    "");
  String ssid = prefs.getString("wifi_ssid",   "");
  String dt   = prefs.getString("device_type", "");
  prefs.end();

  String json = "{";
  json += "\"provisioned\":" + String((uid.length() > 0 && ssid.length() > 0) ? "true" : "false") + ",";
  json += "\"has_uid\":"     + String(uid.length()  > 0 ? "true" : "false") + ",";
  json += "\"has_ssid\":"    + String(ssid.length() > 0 ? "true" : "false") + ",";
  json += "\"device_type\":\"" + dt + "\",";
  json += "\"mac\":\"" + WiFi.macAddress() + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

// ═══════════════════════════════════════════════════════════════════════════
// SETUP
// ═══════════════════════════════════════════════════════════════════════════
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n╔══════════════════════════════════════════════╗");
  Serial.println("║   VIGILERT PROVISIONING SKETCH               ║");
  Serial.println("╚══════════════════════════════════════════════╝");

  // ── Load saved credentials ──────────────────────────────────────────────
  prefs.begin("vigilert", true);
  savedSSID    = prefs.getString("wifi_ssid",   "");
  savedPass    = prefs.getString("wifi_pass",   "");
  savedUID     = prefs.getString("user_uid",    "");
  savedDevType = prefs.getString("device_type", "");
  prefs.end();

  bool hasAll = (savedSSID.length() > 0 && savedUID.length() > 0 && savedDevType.length() > 0);

  // ── If credentials exist, try to connect ───────────────────────────────
  if (hasAll) {
    Serial.println("[PROV] Credentials found — attempting WiFi connection...");
    Serial.printf("[PROV]   SSID: %s\n", savedSSID.c_str());
    Serial.printf("[PROV]   UID:  %s\n", savedUID.c_str());

    WiFi.mode(WIFI_STA);
    WiFi.begin(savedSSID.c_str(), savedPass.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n[PROV] ✅ WiFi connected!");
      Serial.printf("[PROV]   IP: %s\n", WiFi.localIP().toString().c_str());
      Serial.println("\n[PROV] ══════════════════════════════════════════");
      Serial.println("[PROV]   PROVISIONED");
      Serial.println("[PROV]   Flash VIGILERT_MOTORCYCLE_PROVISIONED.ino");
      Serial.println("[PROV] ══════════════════════════════════════════");
      // Halt — user should now flash the main firmware
      while (true) { delay(10000); }
    } else {
      Serial.println("\n[PROV] ❌ WiFi connection failed — starting setup AP");
    }
  } else {
    Serial.println("[PROV] No credentials found — starting setup AP");
  }

  // ── Start AP mode ───────────────────────────────────────────────────────
  WiFi.mode(WIFI_AP);
  delay(100);

  // Build AP SSID from last 4 chars of MAC
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  String apSSID = "VIGILERT-SETUP-" + mac.substring(mac.length() - 4);
  apSSID.toUpperCase();

  WiFi.softAPConfig(AP_IP, AP_IP, AP_SUBNET);
  WiFi.softAP(apSSID.c_str(), AP_PASSWORD);

  Serial.printf("\n[AP] SSID:     %s\n", apSSID.c_str());
  Serial.printf("[AP] Password: %s\n", AP_PASSWORD);
  Serial.printf("[AP] IP:       %s\n", WiFi.softAPIP().toString().c_str());

  // ── Start DNS server (redirect all domains → 192.168.4.1) ──────────────
  dnsServer.start(DNS_PORT, "*", AP_IP);
  Serial.println("[DNS] Captive portal DNS started");

  // ── Register routes ─────────────────────────────────────────────────────
  server.on("/",          HTTP_GET,  handleRoot);
  server.on("/save",      HTTP_POST, handleSave);
  server.on("/reset",     HTTP_GET,  handleReset);
  server.on("/status",    HTTP_GET,  handleStatus);

  // Captive portal detection URLs (iOS, Android, Windows)
  server.on("/hotspot-detect.html",  HTTP_GET, handleCaptiveRedirect);
  server.on("/generate_204",         HTTP_GET, handleCaptiveRedirect);
  server.on("/connecttest.txt",      HTTP_GET, handleCaptiveRedirect);
  server.on("/ncsi.txt",             HTTP_GET, handleCaptiveRedirect);
  server.on("/redirect",             HTTP_GET, handleCaptiveRedirect);
  server.on("/canonical.html",       HTTP_GET, handleCaptiveRedirect);
  server.on("/success.txt",          HTTP_GET, handleCaptiveRedirect);

  // Catch-all for any other path → redirect to portal
  server.onNotFound(handleCaptiveRedirect);

  server.begin();
  Serial.println("[HTTP] Web server started on port 80");
  Serial.println("\n[PROV] Ready — connect your phone to the VIGILERT-SETUP WiFi network");
}

// ═══════════════════════════════════════════════════════════════════════════
// LOOP
// ═══════════════════════════════════════════════════════════════════════════
void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
