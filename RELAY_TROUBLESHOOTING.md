# 🔧 Relay & Safety System Troubleshooting Guide

## ⚠️ Issue: Engine Can Start Even When Helmet Module is OFF

---

## 🔍 Problem Description

You reported:
1. Helmet ESP32 is turned OFF
2. Engine can still be turned ON
3. Relay does not have an open circuit
4. Light indicator is not working

This means the **safety interlock is not working** - the relay should be forced LOW (open circuit) when safety conditions aren't met.

---

## ✅ Solution Applied

I've updated `MotorcycleToWebApplication_COMPLETE.ino` with:

### 1. **Continuous Safety Monitoring**
```cpp
// In main loop() - runs every cycle
if (!engineRunning && !canStartEngine()) {
    digitalWrite(relayPin, LOW); // Force relay OFF
    // Blink LED to indicate system is locked
}
```

This ensures the relay is **always forced LOW** when safety conditions aren't met, even if someone tries to manually activate it.

### 2. **Active Safety Enforcement**
```cpp
// If engine is running but safety conditions change
if (engineRunning && !canStartEngine()) {
    Serial.println("SAFETY VIOLATION - shutting down!");
    stopEngine();
}
```

If the helmet module turns OFF while engine is running, it will automatically shut down.

### 3. **Visual Feedback**
- **Blinking LED** = Safety conditions NOT met (system locked)
- **Solid LED** = Safety conditions met (ready to start)
- **LED OFF** = Engine is running

### 4. **Detailed Serial Monitor Output**
Every second, you'll see:
```
[SAFETY STATUS]
  Helmet Module Active: ✗ NO
  Helmet Switch: ✗ OFF
  Relay State: LOW (Engine Blocked)
  Can Start Engine: ✗ NO
```

---

## 🧪 Testing Steps

### Step 1: Upload Updated Code
1. Open `MotorcycleToWebApplication_COMPLETE.ino`
2. Upload to motorcycle ESP32
3. Open Serial Monitor (115200 baud)

### Step 2: Test Without Helmet Module
1. **Turn OFF helmet ESP32** (or don't power it)
2. Power ON motorcycle ESP32
3. Wait 10 seconds for it to check Firebase
4. **Check Serial Monitor** - should show:
   ```
   [SAFETY STATUS]
   Helmet Module Active: ✗ NO
   Relay State: LOW (Engine Blocked)
   Can Start Engine: ✗ NO
   ```
5. **Check LED** - should be **blinking** (system locked)
6. **Check relay with multimeter**:
   - Measure between COM and NO pins
   - Should show **OPEN CIRCUIT** (infinite resistance)
7. **Try to start engine via SMS**: `1234 START`
   - Should see: `❌ ENGINE START BLOCKED!`
   - Relay should stay LOW

### Step 3: Test With Helmet Module
1. **Turn ON helmet ESP32**
2. Wait 5 seconds for heartbeat to reach Firebase
3. **Check Serial Monitor** - should show:
   ```
   [SAFETY STATUS]
   Helmet Module Active: ✓ YES
   Helmet Switch: ✓ ON (if connected)
   Relay State: LOW (Engine Blocked - waiting for start command)
   Can Start Engine: ✓ YES
   ```
4. **Check LED** - should be **solid ON** (ready to start)
5. **Try to start engine via SMS**: `1234 START`
   - Should see: `✅ [ENGINE] Started successfully!`
   - Relay should go HIGH
   - LED should turn OFF

### Step 4: Test Safety Shutdown
1. With engine running, **turn OFF helmet ESP32**
2. Wait 10 seconds (for heartbeat timeout)
3. **Check Serial Monitor** - should show:
   ```
   ⚠️⚠️⚠️ SAFETY VIOLATION DETECTED! ⚠️⚠️⚠️
   Safety conditions no longer met - shutting down engine!
   ```
4. Relay should go LOW
5. Engine should stop

---

## 🔌 Hardware Checks

### Check 1: Relay Module Type
There are two types of relay modules:

**Type A: Active HIGH (Most Common)**
- Relay activates when IN pin is HIGH
- Your code uses this: `digitalWrite(relayPin, HIGH)` = relay ON

**Type B: Active LOW (Less Common)**
- Relay activates when IN pin is LOW
- If you have this type, you need to invert the logic

**How to Test:**
```cpp
// In Serial Monitor, type this command:
digitalWrite(27, HIGH);
delay(1000);
Serial.println("Relay should click ON now");
digitalWrite(27, LOW);
delay(1000);
Serial.println("Relay should click OFF now");
```

If relay clicks when you set LOW, you have an **active-LOW relay** and need to invert the code.

### Check 2: Relay Wiring
```
Relay Module Pins:
┌─────────────────┐
│ VCC  → ESP32 5V │
│ GND  → ESP32 GND│
│ IN   → GPIO 27  │
└─────────────────┘

Relay Contacts:
┌─────────────────────────┐
│ COM → Ignition +12V     │
│ NO  → Engine Circuit    │
│ NC  → (not used)        │
└─────────────────────────┘
```

**Common Mistakes:**
- ❌ Using NC (Normally Closed) instead of NO
- ❌ Relay module not powered (VCC/GND)
- ❌ Wrong GPIO pin (should be 27)

### Check 3: LED Indicator
```
GPIO 13 → [220Ω Resistor] → LED (+) → GND
```

**Test LED:**
```cpp
// In Serial Monitor:
digitalWrite(13, HIGH); // LED should turn ON
delay(1000);
digitalWrite(13, LOW);  // LED should turn OFF
```

If LED doesn't work:
- Check polarity (long leg = +, short leg = -)
- Check resistor value (220Ω)
- Try different LED (might be burned out)

---

## 🐛 Common Issues & Fixes

### Issue 1: Relay Always ON (Engine Always Allowed)

**Cause:** Active-LOW relay module

**Fix:** Invert relay logic in code:
```cpp
// Change this:
digitalWrite(relayPin, HIGH); // Engine ON
digitalWrite(relayPin, LOW);  // Engine OFF

// To this:
digitalWrite(relayPin, LOW);  // Engine ON
digitalWrite(relayPin, HIGH); // Engine OFF
```

---

### Issue 2: Helmet Module Shows "Active" Even When OFF

**Cause:** Old heartbeat data in Firebase

**Fix:** Clear Firebase data:
1. Go to Firebase Console
2. Navigate to `/helmet_public/{userUID}/devices/helmet`
3. Delete the node
4. Restart motorcycle ESP32

---

### Issue 3: Relay Clicks But Engine Doesn't Start

**Cause:** Relay not connected to correct ignition wire

**Fix:**
1. Use motorcycle wiring diagram
2. Find ignition enable wire (NOT starter motor wire)
3. Test with multimeter:
   - Key OFF: 0V
   - Key ON: 12V
4. Cut this wire and connect through relay COM/NO

---

### Issue 4: LED Not Blinking

**Cause:** LED wiring or code issue

**Fix:**
1. Check LED polarity
2. Check resistor (220Ω)
3. Test with manual digitalWrite commands
4. Check GPIO 13 is not used elsewhere

---

### Issue 5: Serial Monitor Shows "Can Start: YES" But Shouldn't

**Cause:** Safety check logic not working

**Debug Steps:**
1. Check Serial Monitor output:
   ```
   [SAFETY CHECK] Engine Start Request:
   ✓ Motorcycle Module: ON
   ✗ Helmet Module: OFF    ← Should be OFF
   ✗ Helmet Switch: OFF
   ✓ Alcohol Test: SAFE
   ```

2. If Helmet Module shows "ON" when it's OFF:
   - Check `isHelmetModuleActive` variable
   - Check `checkHelmetModuleStatus()` function
   - Verify Firebase heartbeat timeout (should be < 5 seconds)

3. Add debug output:
   ```cpp
   Serial.printf("isHelmetModuleActive = %d\n", isHelmetModuleActive);
   Serial.printf("lastHeartbeat = %lu\n", lastHeartbeat);
   Serial.printf("timeSinceHeartbeat = %lu\n", timeSinceHeartbeat);
   ```

---

## 📊 Expected Behavior Summary

| Condition | Relay State | LED State | Engine Can Start? |
|-----------|-------------|-----------|-------------------|
| Helmet OFF | LOW (Open) | Blinking | ❌ NO |
| Helmet ON, Switch OFF | LOW (Open) | Blinking | ❌ NO |
| Helmet ON, Switch ON, Alcohol Detected | LOW (Open) | Blinking | ❌ NO |
| All Conditions Met | LOW (Open) | Solid ON | ✅ YES (waiting for SMS) |
| Engine Running | HIGH (Closed) | OFF | ✅ Running |
| Crash Detected | LOW (Open) | Blinking | ❌ Auto-shutdown |

---

## 🔍 Debug Commands

Add these to Serial Monitor for testing:

```cpp
// In loop(), add:
if (Serial.available()) {
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  cmd.toUpperCase();
  
  if (cmd == "RELAY ON") {
    digitalWrite(relayPin, HIGH);
    Serial.println("Relay forced ON");
  }
  else if (cmd == "RELAY OFF") {
    digitalWrite(relayPin, LOW);
    Serial.println("Relay forced OFF");
  }
  else if (cmd == "CHECK SAFETY") {
    Serial.println("\n=== SAFETY CHECK DEBUG ===");
    Serial.printf("isHelmetModuleActive = %d\n", isHelmetModuleActive);
    Serial.printf("helmetSwitchState = %d\n", digitalRead(helmetSwitchPin) == LOW);
    Serial.printf("canStartEngine() = %d\n", canStartEngine());
    Serial.printf("Relay Pin State = %d\n", digitalRead(relayPin));
  }
  else if (cmd == "LED TEST") {
    for (int i = 0; i < 5; i++) {
      digitalWrite(lightIndicatorPin, HIGH);
      delay(200);
      digitalWrite(lightIndicatorPin, LOW);
      delay(200);
    }
    Serial.println("LED test complete");
  }
}
```

---

## ✅ Final Checklist

Before declaring the system working:

- [ ] Uploaded updated code to motorcycle ESP32
- [ ] Serial Monitor shows safety status every second
- [ ] LED blinks when helmet module is OFF
- [ ] LED solid when all conditions met
- [ ] Relay stays LOW when helmet OFF (verified with multimeter)
- [ ] Relay goes HIGH only after SMS start command
- [ ] Engine blocks when helmet OFF
- [ ] Engine starts when all conditions met
- [ ] Engine auto-shuts down when helmet turns OFF
- [ ] Crash detection triggers auto-shutdown

---

## 📞 Still Not Working?

If the relay still doesn't block the engine:

1. **Take a photo of your wiring** - especially relay connections
2. **Copy Serial Monitor output** - full 10 seconds of output
3. **Check relay type** - active HIGH or LOW?
4. **Measure voltages**:
   - GPIO 27 when should be OFF: should be 0V
   - GPIO 27 when should be ON: should be 3.3V
   - Relay VCC: should be 5V
5. **Test relay manually** - connect IN to VCC, should click

The updated code now **forces** the relay LOW every loop cycle when conditions aren't met, so it should be impossible to bypass.
