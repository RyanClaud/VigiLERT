// ═══════════════════════════════════════════════════════════════════════════
// VIBRATION SENSOR + POLICE SIREN TEST
// ═══════════════════════════════════════════════════════════════════════════
// Plays a smooth police wail siren (700Hz→1700Hz→700Hz sweep) every time
// the SW-420 vibration sensor detects movement. Fully non-blocking.
// ═══════════════════════════════════════════════════════════════════════════

// ── Pins ───────────────────────────────────────────────────────────────────
const int VIBE_PIN = 15;   // SW-420 output
const int BUZZ_PIN = 14;   // Buzzer
const int LED_PIN  = 2;    // Status LED

// ── Vibration detection ────────────────────────────────────────────────────
const unsigned long DEBOUNCE_MS   = 30;    // Ignore bounces shorter than this
int  lastPinState                 = HIGH;  // INPUT_PULLUP baseline = HIGH
int  detectionCount               = 0;
unsigned long lastTriggerTime     = 0;

// ── Police wail siren (non-blocking) ──────────────────────────────────────
// Sweeps smoothly from SIREN_LOW to SIREN_HIGH and back.
// Each direction takes HALF_PERIOD_MS milliseconds.
// Siren runs for SIREN_DURATION_MS total, then stops.
// Every new vibration detection restarts the siren timer.

const int           SIREN_LOW       = 700;   // Hz — bottom of wail
const int           SIREN_HIGH      = 1700;  // Hz — top of wail
const unsigned long HALF_PERIOD_MS  = 700;   // ms per sweep direction (slow wail)
const unsigned long TICK_MS         = 10;    // Update every 10ms (100 steps/direction)
const unsigned long SIREN_DURATION  = 10000; // Total siren time per trigger (ms)

bool          sirenActive    = false;
unsigned long sirenStart     = 0;
unsigned long sirenLastTick  = 0;
unsigned long phaseStart     = 0;
bool          goingUp        = true;

// ── Helpers ────────────────────────────────────────────────────────────────
void startSiren() {
  sirenActive   = true;
  sirenStart    = millis();
  sirenLastTick = millis();
  phaseStart    = millis();
  goingUp       = true;
  tone(BUZZ_PIN, SIREN_LOW);
  digitalWrite(LED_PIN, HIGH);
}

void stopSiren() {
  sirenActive = false;
  noTone(BUZZ_PIN);
  digitalWrite(LED_PIN, LOW);
}

void tickSiren() {
  if (!sirenActive) return;

  unsigned long now = millis();

  // Stop after SIREN_DURATION
  if (now - sirenStart >= SIREN_DURATION) {
    stopSiren();
    Serial.println("[SIREN] Stopped");
    return;
  }

  // Update frequency every TICK_MS
  if (now - sirenLastTick < TICK_MS) return;
  sirenLastTick = now;

  // How far through the current half-period (0.0 to 1.0)
  unsigned long elapsed = now - phaseStart;
  float t = (float)elapsed / (float)HALF_PERIOD_MS;
  if (t > 1.0f) t = 1.0f;

  // Linear interpolation between low and high
  int freq = goingUp
    ? (int)(SIREN_LOW  + t * (SIREN_HIGH - SIREN_LOW))
    : (int)(SIREN_HIGH - t * (SIREN_HIGH - SIREN_LOW));

  tone(BUZZ_PIN, freq);

  // Flash LED in sync with sweep direction
  digitalWrite(LED_PIN, goingUp ? HIGH : LOW);

  // Flip direction when half-period completes
  if (elapsed >= HALF_PERIOD_MS) {
    goingUp    = !goingUp;
    phaseStart = now;
  }
}

// ── Setup ──────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(VIBE_PIN, INPUT_PULLUP);
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(LED_PIN,  OUTPUT);
  noTone(BUZZ_PIN);
  digitalWrite(LED_PIN, LOW);

  lastPinState = digitalRead(VIBE_PIN);

  Serial.println("\n=== VIBRATION + POLICE SIREN TEST ===");
  Serial.printf("Vibration: GPIO %d | Buzzer: GPIO %d\n", VIBE_PIN, BUZZ_PIN);
  Serial.printf("Siren: %dHz to %dHz, %lums sweep, %lus total\n",
                SIREN_LOW, SIREN_HIGH, HALF_PERIOD_MS, SIREN_DURATION / 1000);
  Serial.printf("Baseline: GPIO%d = %s\n\n",
                VIBE_PIN, lastPinState ? "HIGH (normal)" : "LOW (check wiring)");

  // Startup confirmation — two rising chirps
  tone(BUZZ_PIN, 1000, 120); delay(180);
  tone(BUZZ_PIN, 1500, 120); delay(180);
  noTone(BUZZ_PIN);
  Serial.println("Startup beep done. Shake the sensor to trigger siren.\n");
}

// ── Loop ───────────────────────────────────────────────────────────────────
void loop() {
  unsigned long now = millis();

  // ── 1. Tick the siren (runs every loop, no blocking) ─────────────────────
  tickSiren();

  // ── 2. Read vibration sensor ──────────────────────────────────────────────
  int currentState = digitalRead(VIBE_PIN);

  if (currentState != lastPinState) {
    unsigned long timeSinceLast = now - lastTriggerTime;

    if (timeSinceLast >= DEBOUNCE_MS) {
      detectionCount++;
      lastTriggerTime = now;

      Serial.printf("[VIBRATION #%d] GPIO%d: %s -> %s | Time: %lums\n",
                    detectionCount,
                    VIBE_PIN,
                    lastPinState ? "HIGH" : "LOW",
                    currentState ? "HIGH" : "LOW",
                    now);

      // Start (or restart) the police siren
      startSiren();
      Serial.printf("[SIREN] Started — will run for %lu seconds\n",
                    SIREN_DURATION / 1000);
    }

    lastPinState = currentState;
  }

  // ── 3. Periodic status print every 2 seconds ─────────────────────────────
  static unsigned long lastStatus = 0;
  if (now - lastStatus >= 2000) {
    lastStatus = now;
    Serial.printf("[STATUS] GPIO%d=%s | Detections=%d | Siren=%s\n",
                  VIBE_PIN,
                  currentState ? "HIGH" : "LOW",
                  detectionCount,
                  sirenActive ? "ON" : "off");
  }
}
