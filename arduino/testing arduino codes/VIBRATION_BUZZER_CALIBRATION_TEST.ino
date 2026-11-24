// ═══════════════════════════════════════════════════════════════════════════
// VigiLERT — VIBRATION SENSOR & POLICE BUZZER CALIBRATION & TESTING TOOL
// ═══════════════════════════════════════════════════════════════════════════
// Designed specifically to diagnose false alarms, tune sensitivity thresholds,
// and eliminate acoustic/vibrational feedback between buzzer & vibration sensor.
//
// Hardware Pins (Identical to MotorcycleToWebApplication_FINAL.ino):
//   - Vibration Sensor (SW-420 DO) : GPIO 15
//   - Buzzer (Active / Passive)     : GPIO 14
//   - Status LED / Indicator       : GPIO 2
//
// Baud Rate: 115200 baud (set Serial Monitor to "Both NL & CR" or "Newline")
// ═══════════════════════════════════════════════════════════════════════════

const int vibrationSensorPin = 15; // SW-420 digital output
const int buzzerPin          = 14; // Alert buzzer
const int lightIndicatorPin  = 2;  // On-board LED

// ── Pulse accumulator & threshold settings ──────────────────────────────────
int pulseThreshold               = 5;    // Required pulses within window to trigger alarm (default: 5)
const unsigned long WINDOW_MS    = 1500; // Moving window (1.5 seconds)
const unsigned long DEBOUNCE_MS  = 35;   // Minimum debounce between pulses
const unsigned long ALARM_TIME   = 4000; // Siren test duration (4 seconds)
const unsigned long SETTLE_MS    = 2500; // Silence cooldown after buzzer stops

// ── State tracking ─────────────────────────────────────────────────────────
volatile int isrPulseCount = 0;
int windowPulses = 0;
unsigned long windowStart = 0;
unsigned long lastPulseTime = 0;
unsigned long lastSirenStop = 0;
bool isAlarmSounding = false;
unsigned long alarmStartTime = 0;

int totalTriggers = 0;
int lastRawPinState = -1;

// ── Hardware interrupt ─────────────────────────────────────────────────────
void IRAM_ATTR onVibrationISR() {
  isrPulseCount++;
}

// ── Police Siren Generator (Non-blocking two-phase tone) ───────────────────
void startAlarm() {
  if (isAlarmSounding) return; // Prevent recursive re-trigger
  isAlarmSounding = true;
  alarmStartTime  = millis();
  digitalWrite(lightIndicatorPin, HIGH);
  Serial.println("\r\n🚨🚨🚨 [ALARM TRIGGERED!] Threshold reached — sounding police siren... 🚨🚨🚨");
}

void stopAlarm() {
  isAlarmSounding = false;
  lastSirenStop   = millis();
  noTone(buzzerPin);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  Serial.printf("[ALARM STOPPED] Entering %lu ms acoustic settle cooldown (buzzer reverberation suppressed)\r\n\n", SETTLE_MS);
}

void updateAlarmSiren() {
  if (!isAlarmSounding) return;

  unsigned long elapsed = millis() - alarmStartTime;
  if (elapsed >= ALARM_TIME) {
    stopAlarm();
    return;
  }

  // Multi-tone police siren sound
  unsigned long cycle = elapsed % 600; // 600ms cycle
  if (cycle < 300) {
    // Pitch rises 800 Hz -> 1750 Hz
    int f = 800 + (cycle * 950 / 300);
    tone(buzzerPin, f);
  } else {
    // Hi-Lo alternate
    if (cycle < 450) {
      tone(buzzerPin, 1600);
    } else {
      tone(buzzerPin, 900);
    }
  }
}

// ── Visual ASCII Pulse Meter ───────────────────────────────────────────────
void printLiveMeter(int pulses, int rawState) {
  int barLength = map(constrain(pulses, 0, 15), 0, 15, 0, 15);
  String bar = "[";
  for (int i = 0; i < 15; i++) {
    if (i < barLength) bar += "█";
    else bar += "░";
  }
  bar += "]";

  String statusStr = "QUIET (Still)";
  if (isAlarmSounding) {
    statusStr = "🚨 ALARM SOUNDING (Sensors Suppressed)";
  } else if (millis() - lastSirenStop < SETTLE_MS) {
    statusStr = "⏳ COOLDOWN (Chassis settling)";
  } else if (pulses >= pulseThreshold) {
    statusStr = "⚠️ THRESHOLD MET! ALARM!";
  } else if (pulses > 0) {
    statusStr = "⚡ BUMP/TAP DETECTED";
  }

  Serial.printf("GPIO%d=%-4s | Pulses/1.5s: %2d (Req: %2d) %s | %s\r\n",
                vibrationSensorPin,
                rawState ? "HIGH" : "LOW ",
                pulses,
                pulseThreshold,
                bar.c_str(),
                statusStr.c_str());
}

// ── Print Hardware Tuning Guide ────────────────────────────────────────────
void printHelp() {
  Serial.println("\r\n═══════════════════════════════════════════════════════════════════════════");
  Serial.println("📖 SW-420 SENSITIVITY CALIBRATION & TEST COMMANDS:");
  Serial.println("═══════════════════════════════════════════════════════════════════════════");
  Serial.println("  +       : Increase threshold by 1 (requires MORE vibration, LESS sensitive)");
  Serial.println("  -       : Decrease threshold by 1 (requires LESS vibration, MORE sensitive)");
  Serial.println("  B       : Sound BUZZER for 3 seconds (verify acoustic feedback suppression)");
  Serial.println("  T       : Force trigger an alarm test cycle");
  Serial.println("  R       : Reset pulse counter & trigger stats");
  Serial.println("  H       : Show this help menu");
  Serial.println("\r\n🔧 HARDWARE POTENTIOMETER TUNING (Blue box on SW-420):");
  Serial.println("  1. Look at the two small LEDs on the SW-420 sensor module:");
  Serial.println("     - PWR LED : Stays ON continuously (Red).");
  Serial.println("     - DO LED  : Indicates vibration detection (Green/Blue).");
  Serial.println("  2. When the motorcycle is COMPLETELY STILL on its stand:");
  Serial.println("     - The DO LED MUST BE OFF.");
  Serial.println("     - If the DO LED is constantly ON or flickering, it is TOO SENSITIVE!");
  Serial.println("     - Use a tiny screwdriver to gently turn the brass screw:");
  Serial.println("       * CLOCKWISE or COUNTER-CLOCKWISE until the DO LED JUST TURNS OFF.");
  Serial.println("  3. Tap the seat or shake the handlebars:");
  Serial.println("     - The DO LED should flash briefly only when shaken.");
  Serial.println("═══════════════════════════════════════════════════════════════════════════\r\n");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(vibrationSensorPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);

  noTone(buzzerPin);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);

  // Attach interrupt
  attachInterrupt(digitalPinToInterrupt(vibrationSensorPin), onVibrationISR, CHANGE);

  // Startup chirp
  tone(buzzerPin, 1000, 100); delay(150);
  tone(buzzerPin, 1500, 100); delay(150);
  noTone(buzzerPin);

  Serial.println("\r\n===========================================================================");
  Serial.println("🛡️  VigiLERT: VIBRATION SENSOR & POLICE BUZZER CALIBRATION TEST");
  Serial.println("===========================================================================");
  Serial.printf("📍 Vibration Sensor : GPIO %d (SW-420 with INPUT_PULLUP)\r\n", vibrationSensorPin);
  Serial.printf("📍 Alert Buzzer     : GPIO %d\r\n", buzzerPin);
  Serial.printf("📍 Status LED       : GPIO %d\r\n", lightIndicatorPin);
  Serial.printf("⚙️ Current Threshold : %d pulses within %lu ms window\r\n", pulseThreshold, WINDOW_MS);
  Serial.println("===========================================================================");

  printHelp();
  windowStart = millis();
}

void loop() {
  unsigned long now = millis();

  // 1. Process Siren Audio
  updateAlarmSiren();

  // 2. Read new ISR pulses
  int newPulses = isrPulseCount;
  isrPulseCount = 0;

  int rawPin = digitalRead(vibrationSensorPin);

  // 3. Sliding Window Reset
  if (now - windowStart >= WINDOW_MS) {
    windowStart = now;
    windowPulses = 0;
  }

  // 4. Acoustic Feedback & Settle Suppression Filter
  // If the alarm is sounding, or if we just stopped the buzzer within SETTLE_MS:
  // completely ignore pulses generated by the buzzer's own acoustic vibrations!
  bool suppressed = isAlarmSounding || (now - lastSirenStop < SETTLE_MS);

  if (!suppressed && newPulses > 0) {
    if (now - lastPulseTime >= DEBOUNCE_MS) {
      lastPulseTime = now;
      windowPulses += newPulses;

      if (windowPulses >= pulseThreshold) {
        totalTriggers++;
        startAlarm();
      }
    }
  }

  // 5. Periodic Live Meter Print (every 250ms)
  static unsigned long lastMeterPrint = 0;
  if (now - lastMeterPrint >= 250) {
    lastMeterPrint = now;
    printLiveMeter(windowPulses, rawPin);
  }

  // 6. Interactive Serial Commands
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '+' || c == '=') {
      pulseThreshold = min(30, pulseThreshold + 1);
      Serial.printf("\r\n>>> 🔼 Threshold INCREASED to %d pulses (LESS sensitive) <<<\r\n\n", pulseThreshold);
    } else if (c == '-' || c == '_') {
      pulseThreshold = max(1, pulseThreshold - 1);
      Serial.printf("\r\n>>> 🔽 Threshold DECREASED to %d pulses (MORE sensitive) <<<\r\n\n", pulseThreshold);
    } else if (c == 'b' || c == 'B') {
      Serial.println("\r\n>>> 🔊 Testing Buzzer for 3 seconds (Testing acoustic isolation)... <<<");
      startAlarm();
    } else if (c == 't' || c == 'T') {
      Serial.println("\r\n>>> 🚨 Triggering Alarm Cycle Simulation... <<<");
      startAlarm();
    } else if (c == 'r' || c == 'R') {
      windowPulses = 0;
      totalTriggers = 0;
      Serial.println("\r\n>>> 🔄 Counters reset to 0 <<<\r\n");
    } else if (c == 'h' || c == 'H') {
      printHelp();
    }
  }
}
