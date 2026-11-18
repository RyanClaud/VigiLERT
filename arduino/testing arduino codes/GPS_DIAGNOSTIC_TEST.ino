// ═══════════════════════════════════════════════════════════════════════════
// VIGILERT GPS DIAGNOSTIC TEST
// ═══════════════════════════════════════════════════════════════════════════
// Upload this sketch ALONE to test your GPS module.
// Open Serial Monitor at 115200 baud.
// Take the ESP32 OUTSIDE with a clear sky view.
// ═══════════════════════════════════════════════════════════════════════════

#include <HardwareSerial.h>
#include <TinyGPSPlus.h>

// ── GPS pins — match your wiring ──────────────────────────────────────────
// GPS TX → ESP32 GPIO 16 (RX)
// GPS RX → ESP32 GPIO 17 (TX)  ← optional, GPS doesn't need commands
// GPS VCC → 3.3V
// GPS GND → GND
const int GPS_RX_PIN = 16;
const int GPS_TX_PIN = 17;
const int GPS_BAUD   = 9600;   // Most NEO-6M/7M/8M modules default to 9600

HardwareSerial gpsSerial(1);   // UART1
TinyGPSPlus    gps;

unsigned long lastPrint    = 0;
unsigned long startTime    = 0;
unsigned long totalChars   = 0;
bool          everHadFix   = false;

// ─────────────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("╔══════════════════════════════════════════════════════════╗");
  Serial.println("║          VIGILERT GPS DIAGNOSTIC TEST                   ║");
  Serial.println("╠══════════════════════════════════════════════════════════╣");
  Serial.println("║  GPS TX → GPIO 16  |  GPS VCC → 3.3V                   ║");
  Serial.println("║  GPS RX → GPIO 17  |  GPS GND → GND                    ║");
  Serial.println("╠══════════════════════════════════════════════════════════╣");
  Serial.println("║  TAKE THE MODULE OUTSIDE — GPS CANNOT FIX INDOORS      ║");
  Serial.println("║  Cold start can take 1–3 minutes outdoors               ║");
  Serial.println("╚══════════════════════════════════════════════════════════╝");
  Serial.println();

  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  startTime = millis();

  Serial.println("[GPS] Serial initialized on UART1");
  Serial.println("[GPS] Waiting for data...");
  Serial.println();
}

// ─────────────────────────────────────────────────────────────────────────
void loop() {
  // Feed every available byte to TinyGPS++
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
    totalChars++;
  }

  // Print status every 2 seconds
  if (millis() - lastPrint >= 2000) {
    lastPrint = millis();
    unsigned long elapsed = (millis() - startTime) / 1000;

    Serial.println("──────────────────────────────────────────────────────────");
    Serial.printf("⏱  Elapsed: %lu seconds\n", elapsed);
    Serial.printf("📡 Chars received from GPS: %lu\n", totalChars);
    Serial.printf("📝 NMEA sentences parsed:   %lu\n", gps.sentencesWithFix());
    Serial.printf("❌ Checksum failures:        %lu\n", gps.failedChecksum());
    Serial.println();

    // ── Diagnosis based on char count ──────────────────────────────────
    if (totalChars == 0) {
      Serial.println("🔴 NO DATA — GPS module is not sending anything.");
      Serial.println("   Check: VCC=3.3V, GND connected, TX→GPIO16 correct.");
      Serial.println("   Try swapping RX/TX pins if wired backwards.");
    } else if (gps.failedChecksum() > 10 && gps.sentencesWithFix() == 0) {
      Serial.println("🟡 DATA ARRIVING but checksum errors — possible baud rate mismatch.");
      Serial.println("   Try changing GPS_BAUD to 4800 or 38400 in this sketch.");
    } else if (totalChars > 0 && gps.sentencesWithFix() == 0) {
      Serial.println("🟡 DATA ARRIVING — parsing OK — waiting for satellite fix.");
      Serial.println("   Make sure you are OUTSIDE with clear sky view.");
      Serial.println("   Cold start: 1–3 min outdoors. Warm start: <30 sec.");
    }

    // ── Satellite count ─────────────────────────────────────────────────
    if (gps.satellites.isValid()) {
      Serial.printf("🛰  Satellites in view: %d\n", gps.satellites.value());
      if (gps.satellites.value() == 0) {
        Serial.println("   No satellites yet — go outside, away from buildings.");
      } else if (gps.satellites.value() < 4) {
        Serial.println("   Need at least 4 satellites for a 3D fix.");
      } else {
        Serial.println("   ✅ Enough satellites for a fix!");
      }
    } else {
      Serial.println("🛰  Satellites: not yet available");
    }

    // ── HDOP (accuracy indicator) ───────────────────────────────────────
    if (gps.hdop.isValid()) {
      float hdop = gps.hdop.hdop();
      Serial.printf("📐 HDOP (accuracy): %.2f  ", hdop);
      if      (hdop < 1.0)  Serial.println("(Excellent)");
      else if (hdop < 2.0)  Serial.println("(Good)");
      else if (hdop < 5.0)  Serial.println("(Moderate)");
      else                  Serial.println("(Poor — move to open sky)");
    }

    // ── Location ────────────────────────────────────────────────────────
    Serial.println();
    if (gps.location.isValid()) {
      everHadFix = true;
      Serial.println("✅ ✅ ✅  GPS FIX ACQUIRED!  ✅ ✅ ✅");
      Serial.printf("📍 Latitude:  %.6f\n", gps.location.lat());
      Serial.printf("📍 Longitude: %.6f\n", gps.location.lng());
      Serial.printf("📍 Fix age:   %lu ms\n", gps.location.age());

      // Google Maps link
      Serial.printf("🗺  Maps: https://maps.google.com/?q=%.6f,%.6f\n",
                    gps.location.lat(), gps.location.lng());
    } else {
      if (everHadFix) {
        Serial.println("⚠️  Had a fix but lost it — fix age: " + String(gps.location.age()) + " ms");
      } else {
        Serial.println("⏳ No GPS fix yet — keep waiting outdoors...");
      }
    }

    // ── Speed & Altitude ────────────────────────────────────────────────
    if (gps.speed.isValid()) {
      Serial.printf("🏎  Speed:    %.1f km/h\n", gps.speed.kmph());
    }
    if (gps.altitude.isValid()) {
      Serial.printf("⛰  Altitude: %.1f m\n", gps.altitude.meters());
    }

    // ── Date & Time (UTC) ───────────────────────────────────────────────
    if (gps.date.isValid() && gps.time.isValid()) {
      Serial.printf("🕐 UTC Time: %04d-%02d-%02d %02d:%02d:%02d\n",
                    gps.date.year(), gps.date.month(), gps.date.day(),
                    gps.time.hour(), gps.time.minute(), gps.time.second());
    }

    Serial.println();
  }
}
