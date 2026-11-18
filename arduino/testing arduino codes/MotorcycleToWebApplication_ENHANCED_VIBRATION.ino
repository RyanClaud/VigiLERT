// ═══════════════════════════════════════════════════════════════════════════
// VigiLERT Motorcycle Module - ENHANCED VIBRATION DETECTION
// ═══════════════════════════════════════════════════════════════════════════
// ✅ Based on MotorcycleToWebApplication_FINAL.ino
// ✅ Enhanced with accurate vibration detection from reference code
// ✅ Maintains all existing functionalities
// ✅ Improved edge detection and immediate alert response
// ═══════════════════════════════════════════════════════════════════════════

// NOTE: This file contains ONLY the modified vibration detection logic.
// Copy the entire MotorcycleToWebApplication_FINAL.ino and replace the
// vibration detection section in loop() with the code below.

// ═══════════════════════════════════════════════════════════════════════════
// ENHANCED VIBRATION DETECTION - REPLACE IN loop() FUNCTION
// ═══════════════════════════════════════════════════════════════════════════

/*
  ── ENHANCED Vibration sensor / anti-theft ────────────────────────────────────
  
  ✅ ACCURATE DETECTION from reference code:
  - Detects BOTH edges (LOW→HIGH and HIGH→LOW) for maximum sensitivity
  - 30ms debounce catches even slightest movements
  - Triggers IMMEDIATELY on any state change
  - Non-blocking siren continues independently
  - Sensor behavior: Normal=LOW, Vibration=HIGH (active-high sensor)
  
  ✅ IMPROVEMENTS over previous version:
  - More reliable edge detection
  - Immediate response to movement
  - Better state tracking
  - Continuous monitoring without blocking
*/

// Place this code in loop() where the vibration detection currently is:

  // ── Enhanced Vibration sensor / anti-theft ────────────────────────────────
  // Uses static variables to persist state across loop iterations
  static int lastVibrationReading = LOW;
  static unsigned long lastVibrationTrigger = 0;

  if (!engineRunning) {
    // Step 1: Enable the system as soon as engine turns off
    if (!antiTheftEnabled) {
      antiTheftEnabled = true;
      engineOffTime = millis();
      consecutiveVibrations = 0;
      Serial.println("\n[ANTI-THEFT] 🛡️ System enabled — arming in 10 seconds...");
    }

    // Step 2: Arm after ARM_DELAY (10 seconds)
    if (!antiTheftArmed && (millis() - engineOffTime >= ARM_DELAY)) {
      antiTheftArmed = true;
      theftDetectionCount = 0;
      theftAlertSent = false;
      consecutiveVibrations = 0;
      lastVibrationReading = digitalRead(vibrationSensorPin); // Snapshot baseline
      Serial.println("\n[ANTI-THEFT] 🛡️ ARMED! Ultra-sensitive vibration detection active...");
      Serial.printf("[ANTI-THEFT] 📍 GPIO %d baseline: %s\n",
                    vibrationSensorPin, lastVibrationReading ? "HIGH" : "LOW");

      // Two short confirmation beeps (acceptable delay only at arm time)
      digitalWrite(buzzerPin, HIGH);
      delay(80);
      digitalWrite(buzzerPin, LOW);
      delay(80);
      digitalWrite(buzzerPin, HIGH);
      delay(80);
      digitalWrite(buzzerPin, LOW);
    }

    // Step 3: Poll sensor every loop — NO delay() here for instant detection
    if (antiTheftArmed) {
      int currentReading = digitalRead(vibrationSensorPin);
      unsigned long currentTime = millis();

      // ✅ ENHANCED: Detect ANY state change (LOW→HIGH or HIGH→LOW)
      // This is the key improvement from reference code - catches every pulse
      bool stateChanged = (currentReading != lastVibrationReading);
      
      if (stateChanged) {
        unsigned long timeSinceLastTrigger = currentTime - lastVibrationTrigger;
        
        // ✅ FAST DEBOUNCE: 30ms (from reference code)
        if (timeSinceLastTrigger >= VIBRATION_DEBOUNCE) {
          consecutiveVibrations++;
          lastVibrationTrigger = currentTime;
          
          Serial.printf("\n🚨🚨 [ANTI-THEFT] MOVEMENT #%d DETECTED! 🚨🚨\n", consecutiveVibrations);
          Serial.printf("[ANTI-THEFT] 📍 GPIO %d: %s → %s (state change detected)!\n", 
                        vibrationSensorPin,
                        lastVibrationReading ? "HIGH" : "LOW",
                        currentReading ? "HIGH" : "LOW");

          // ── Start / extend non-blocking siren alert ──────────────────
          // Every new detection restarts the 8-second siren window
          startSiren();

          // ── Firebase + SMS alert (with cooldown to avoid spam) ────────
          unsigned long timeSinceLastAlert = currentTime - lastTheftAlert;
          if (timeSinceLastAlert >= THEFT_ALERT_COOLDOWN) {
            Serial.println("[ANTI-THEFT] 📱 Sending Firebase + SMS alert...");
            // triggerTheftAlert() does Firebase write + SMS
            // The buzzer continues independently while this runs
            triggerTheftAlert();
            lastTheftAlert = currentTime;
            theftAlertSent = true;
          } else {
            Serial.printf("[ANTI-THEFT] ⏳ Alert cooldown: %lu s remaining\n",
                          (THEFT_ALERT_COOLDOWN - timeSinceLastAlert) / 1000);
          }
        } else {
          // Movement detected but still in debounce period
          Serial.printf("[VIBRATION] 🔍 Movement detected but debouncing (%lu ms remaining)\n", 
                        VIBRATION_DEBOUNCE - timeSinceLastTrigger);
        }
      }
      
      // ✅ UPDATE STATE: Track for next iteration (critical for edge detection)
      lastVibrationReading = currentReading;
      
      // ✅ CONTINUOUS MONITORING: Show sensor reading every 5 seconds
      static unsigned long lastVibDebug = 0;
      if (currentTime - lastVibDebug >= 5000) {
        Serial.printf("[VIBRATION] Armed | GPIO%d=%s | Detections=%d | Buzzer=%s\n",
                      vibrationSensorPin,
                      currentReading ? "HIGH" : "LOW",
                      consecutiveVibrations,
                      buzzerMode == BUZZER_SIREN ? "ON" : "OFF");
        lastVibDebug = currentTime;
      }
      
      // ✅ RESET COUNTER: After 30 seconds of no activity
      if (consecutiveVibrations > 0 && (currentTime - lastVibrationTrigger) > 30000) {
        consecutiveVibrations = 0;
        Serial.println("[ANTI-THEFT] ✓ Counter reset (30s inactivity)");
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

// ═══════════════════════════════════════════════════════════════════════════
// END OF ENHANCED VIBRATION DETECTION
// ═══════════════════════════════════════════════════════════════════════════

/*
  ═══════════════════════════════════════════════════════════════════════════
  IMPLEMENTATION INSTRUCTIONS:
  ═══════════════════════════════════════════════════════════════════════════
  
  1. Open MotorcycleToWebApplication_FINAL.ino
  
  2. Find the vibration detection section in loop() (around line 600-700)
     Look for: "// ── Vibration sensor / anti-theft ─────"
  
  3. Replace the entire vibration detection block with the code above
     (from "// ── Enhanced Vibration sensor" to "stopSiren();")
  
  4. Ensure these constants are defined at the top of your file:
     - const unsigned long VIBRATION_DEBOUNCE = 30;
     - const unsigned long ARM_DELAY = 10000;
     - const unsigned long THEFT_ALERT_COOLDOWN = 60000;
  
  5. Ensure these variables are declared globally:
     - int consecutiveVibrations = 0;
     - unsigned long lastTheftAlert = 0;
     - bool theftAlertSent = false;
     - int theftDetectionCount = 0;
  
  6. Upload to your ESP32 and test!
  
  ═══════════════════════════════════════════════════════════════════════════
  TESTING PROCEDURE:
  ═══════════════════════════════════════════════════════════════════════════
  
  1. Upload code to ESP32
  2. Open Serial Monitor (115200 baud)
  3. Send command: "STOP ENGINE" (to turn off engine)
  4. Wait 10 seconds for arming (you'll hear 2 short beeps)
  5. Tap or shake the vibration sensor
  6. You should see:
     - Serial output: "🚨🚨 [ANTI-THEFT] MOVEMENT #1 DETECTED! 🚨🚨"
     - Siren starts immediately (police/ambulance sweep sound)
     - Light indicator blinks
     - After 60 seconds, SMS alert sent (if GSM connected)
  
  7. Send command: "START ENGINE" to disarm
  
  ═══════════════════════════════════════════════════════════════════════════
  KEY IMPROVEMENTS FROM REFERENCE CODE:
  ═══════════════════════════════════════════════════════════════════════════
  
  ✅ Edge Detection: Detects BOTH LOW→HIGH and HIGH→LOW transitions
  ✅ Immediate Response: Triggers alert on ANY state change
  ✅ Fast Debounce: 30ms catches even slightest movements
  ✅ State Tracking: Maintains last reading for accurate edge detection
  ✅ Non-Blocking: Siren runs independently, system stays responsive
  ✅ Continuous Monitoring: Polls sensor every loop iteration
  ✅ Debug Output: Shows state changes and detection count
  ✅ Auto-Reset: Clears counter after 30 seconds of inactivity
  
  ═══════════════════════════════════════════════════════════════════════════
  TROUBLESHOOTING:
  ═══════════════════════════════════════════════════════════════════════════
  
  Problem: Sensor always reads HIGH
  Solution: Check wiring - sensor may be inverted or needs pull-down resistor
  
  Problem: No detection when tapping sensor
  Solution: 
  - Verify GPIO 15 is connected to sensor DO (Digital Output) pin
  - Check sensor sensitivity adjustment (potentiometer on sensor)
  - Use "TEST VIBRATION" command to force trigger
  
  Problem: Too many false alarms
  Solution: 
  - Increase VIBRATION_DEBOUNCE from 30ms to 50ms or 100ms
  - Adjust sensor sensitivity potentiometer (turn clockwise = less sensitive)
  
  Problem: Siren doesn't sound
  Solution:
  - Check buzzer connection to GPIO 12
  - Verify buzzer is active buzzer (not passive)
  - Check buzzerMode variable in Serial Monitor
  
  ═══════════════════════════════════════════════════════════════════════════
*/
