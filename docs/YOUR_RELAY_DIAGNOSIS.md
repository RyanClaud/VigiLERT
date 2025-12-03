# 🔧 Your Relay Module Diagnosis & Fix

## 📸 What I See in Your Photo

**Relay Module:** SRD-05VDC-SL-C (5V Single Channel Relay)
- This is a standard active-HIGH relay module
- Has built-in LED indicator (should light up when activated)
- Rated for 10A at 250VAC or 10A at 30VDC

**Wiring I Can See:**
- Yellow wire → VCC (5V power)
- White wire → GND (ground)
- Purple wire → IN (control signal from GPIO 27)
- Red/Black wires → Relay contacts (COM, NO, NC)

---

## 🧪 STEP 1: Test If Relay Module Works

### Upload Test Code

1. **Upload `RELAY_TEST_SIMPLE.ino`** to your ESP32
2. **Open Serial Monitor** (115200 baud)
3. **Watch and Listen:**
   - You should hear **clicking sound** every 2 seconds
   - Blue LED on relay should turn ON/OFF
   - Serial Monitor shows what should happen

### What to Check:

✅ **If relay clicks and LED blinks:**
- Relay module is working correctly
- Problem is in the main code logic
- Skip to STEP 3

❌ **If relay does NOT click:**
- Continue to STEP 2

---

## 🔍 STEP 2: Hardware Troubleshooting

### Check 1: Verify Wiring

```
ESP32 Pin          Wire Color     Relay Module Pin
─────────────────────────────────────────────────
5V        ────→    Yellow    ────→    VCC
GND       ────→    White     ────→    GND
GPIO 27   ────→    Purple    ────→    IN
```

**Test with Multimeter:**
1. Set multimeter to voltage mode (DC)
2. Measure between VCC and GND on relay: Should be **5V**
3. Measure between IN and GND when relay should be ON: Should be **3.3V**
4. Measure between IN and GND when relay should be OFF: Should be **0V**

### Check 2: Test Relay Manually

**Bypass ESP32 to test relay:**
1. Disconnect purple wire (IN) from ESP32
2. Touch purple wire directly to 5V (yellow wire)
3. Relay should **CLICK ON** immediately
4. Remove purple wire from 5V
5. Relay should **CLICK OFF**

**Result:**
- ✅ If relay clicks: ESP32 GPIO 27 is not outputting voltage
- ❌ If relay doesn't click: Relay module is faulty

### Check 3: Test ESP32 GPIO 27

**Test if GPIO 27 is working:**
1. Upload `RELAY_TEST_SIMPLE.ino`
2. Disconnect purple wire from relay IN pin
3. Set multimeter to voltage mode
4. Measure voltage on purple wire (GPIO 27 output):
   - Should alternate between **0V** and **3.3V** every 2 seconds
5. If no voltage change: GPIO 27 might be damaged or wrong pin

### Check 4: Power Supply

**Relay needs stable 5V:**
1. Measure voltage at relay VCC pin: Should be **5.0V ±0.2V**
2. If voltage is low (< 4.8V):
   - ESP32 USB power might not be enough
   - Use external 5V power supply
   - Or power ESP32 from VIN pin with 7-12V

---

## 🔧 STEP 3: Fix Main Code Logic

If relay works in test code but not in main code, the issue is the safety logic.

### Problem: Relay Stays LOW Even When It Should Be HIGH

**Root Cause:** The safety interlock code I added forces relay LOW continuously when conditions aren't met, but it might be checking conditions incorrectly.

### Debug the Safety Check

Add this debug code to see what's happening:

```cpp
// Add to loop(), right before the safety interlock code
Serial.println("\n=== SAFETY DEBUG ===");
Serial.printf("isHelmetModuleActive = %d\n", isHelmetModuleActive);
Serial.printf("helmetSwitchState = %d\n", digitalRead(helmetSwitchPin) == LOW);
Serial.printf("engineRunning = %d\n", engineRunning);
Serial.printf("canStartEngine() = %d\n", canStartEngine());
Serial.printf("Relay Pin (GPIO 27) = %d\n", digitalRead(relayPin));
Serial.println("===================\n");
```

### Expected Output When Helmet is OFF:

```
=== SAFETY DEBUG ===
isHelmetModuleActive = 0    ← Should be 0 (false)
helmetSwitchState = 0       ← Depends on physical switch
engineRunning = 0           ← Should be 0 (not running)
canStartEngine() = 0        ← Should be 0 (cannot start)
Relay Pin (GPIO 27) = 0     ← Should be 0 (LOW)
===================
```

### Expected Output When All Conditions Met:

```
=== SAFETY DEBUG ===
isHelmetModuleActive = 1    ← Should be 1 (true)
helmetSwitchState = 1       ← Should be 1 (switch ON)
engineRunning = 0           ← Should be 0 (waiting for start)
canStartEngine() = 1        ← Should be 1 (can start)
Relay Pin (GPIO 27) = 0     ← Still 0 until SMS start command
===================
```

---

## 🎯 STEP 4: Common Issues & Solutions

### Issue 1: Relay Clicks But Engine Doesn't Start

**Cause:** Relay contacts not connected to correct motorcycle wire

**Solution:**
1. Find the motorcycle's ignition enable wire (NOT the starter motor wire)
2. This wire should have:
   - 0V when key is OFF
   - 12V when key is ON (but engine not started)
3. Cut this wire
4. Connect one end to relay COM (common)
5. Connect other end to relay NO (normally open)

**Relay Contact Wiring:**
```
Motorcycle Ignition Circuit:

Battery +12V ──→ Ignition Switch ──→ [CUT HERE]
                                      ↓
                                   Relay COM
                                      ↓
                                   Relay NO
                                      ↓
                                   Engine ECU/Start Circuit
```

### Issue 2: Relay LED is Always ON

**Cause:** Active-LOW relay module (opposite logic)

**Solution:** Invert all relay commands in code:
```cpp
// Change:
digitalWrite(relayPin, HIGH); // to LOW
digitalWrite(relayPin, LOW);  // to HIGH
```

### Issue 3: Relay Doesn't Click at All

**Possible Causes:**
1. **No power to relay:** Check 5V connection
2. **Wrong GPIO pin:** Verify purple wire is on GPIO 27
3. **Faulty relay module:** Test with manual connection (touch IN to VCC)
4. **GPIO 27 damaged:** Try different GPIO pin (change in code)

### Issue 4: Relay Clicks But Immediately Turns OFF

**Cause:** Safety interlock code forcing it OFF

**Solution:** Temporarily disable safety interlock for testing:
```cpp
// Comment out these lines in loop():
/*
if (!engineRunning && !canStartEngine()) {
    digitalWrite(relayPin, LOW);
}
*/
```

Then test if relay stays ON when you send SMS start command.

---

## 📋 Systematic Testing Procedure

### Test 1: Basic Relay Function
```
1. Upload RELAY_TEST_SIMPLE.ino
2. Listen for clicking every 2 seconds
3. Watch blue LED on relay
4. Result: ✅ Clicks = Relay OK | ❌ No click = Hardware issue
```

### Test 2: GPIO 27 Output
```
1. Run RELAY_TEST_SIMPLE.ino
2. Measure GPIO 27 with multimeter
3. Should see 0V → 3.3V → 0V → 3.3V
4. Result: ✅ Voltage changes = GPIO OK | ❌ No change = GPIO issue
```

### Test 3: Manual Relay Activation
```
1. Disconnect IN wire from GPIO 27
2. Touch IN wire to 5V
3. Relay should click ON
4. Result: ✅ Clicks = Relay OK | ❌ No click = Relay faulty
```

### Test 4: Safety Logic
```
1. Upload main code with debug output
2. Check Serial Monitor for safety status
3. Verify canStartEngine() returns correct value
4. Result: ✅ Logic correct = Ready | ❌ Logic wrong = Fix code
```

### Test 5: SMS Control
```
1. Ensure all conditions met (helmet ON, switch ON, no alcohol)
2. Send SMS: "1234 START"
3. Check Serial Monitor for response
4. Relay should click ON
5. Result: ✅ Works = System ready | ❌ Blocked = Check conditions
```

---

## 🔌 Correct Relay Contact Wiring

Your relay has 3 contacts on the blue terminal block:

```
Looking at relay from contact side:
┌─────────────────────┐
│                     │
│   [NC] [COM] [NO]   │  ← 3 screw terminals
│                     │
└─────────────────────┘

NC  = Normally Closed (connected when relay OFF)
COM = Common (always connected to one side)
NO  = Normally Open (connected when relay ON)
```

**For Safety System, Use:**
- **COM** ← Connect to ignition +12V
- **NO** ← Connect to engine circuit
- **NC** ← Leave empty (not used)

This way:
- Relay OFF (LOW) = Circuit OPEN = Engine cannot start ✅
- Relay ON (HIGH) = Circuit CLOSED = Engine can start ✅

---

## ⚠️ Safety Warnings

1. **Never connect relay directly to starter motor** (draws too much current)
2. **Always use fuse** on battery connection (5A recommended)
3. **Test with multimeter first** before connecting to motorcycle
4. **Have backup start method** in case relay fails
5. **Insulate all connections** to prevent shorts

---

## 📊 Quick Reference: Relay States

| Condition | GPIO 27 | Relay LED | Relay State | COM-NO | Engine |
|-----------|---------|-----------|-------------|--------|--------|
| Helmet OFF | LOW (0V) | OFF | Open | ∞ Ω | Cannot Start |
| Ready to Start | LOW (0V) | OFF | Open | ∞ Ω | Cannot Start |
| SMS "START" Sent | HIGH (3.3V) | ON | Closed | 0 Ω | Can Start |
| Engine Running | HIGH (3.3V) | ON | Closed | 0 Ω | Running |
| Crash Detected | LOW (0V) | OFF | Open | ∞ Ω | Auto Shutdown |

---

## 🎯 Next Steps

1. **Upload `RELAY_TEST_SIMPLE.ino`** and verify relay clicks
2. **If relay doesn't click:** Check wiring and power supply
3. **If relay clicks:** Problem is in main code logic
4. **Add debug output** to main code to see safety status
5. **Test with helmet module ON** and verify all conditions met
6. **Send SMS start command** and verify relay activates

---

## 📸 What to Check in Your Photo

Looking at your setup:
1. ✅ Relay module looks correct (SRD-05VDC-SL-C)
2. ✅ Wiring appears connected (yellow, white, purple)
3. ⚠️ Cannot see if blue LED is lit (check this when testing)
4. ⚠️ Cannot see relay contact wiring (red/black wires)
5. ⚠️ Cannot see ESP32 connection (verify GPIO 27)

**Take additional photos of:**
- ESP32 showing which pin purple wire connects to
- Relay contact side showing red/black wire connections
- Blue LED on relay when it should be ON

---

## 💡 Most Likely Issue

Based on your description "relay does not have an open, light indicator not working":

**Diagnosis:** The relay is probably working, but:
1. The safety interlock code is keeping it LOW (correct behavior when helmet OFF)
2. The LED indicator (GPIO 13) might not be wired correctly

**Solution:**
1. Test relay with `RELAY_TEST_SIMPLE.ino` first
2. Verify LED wiring: GPIO 13 → 220Ω resistor → LED+ → GND
3. Check Serial Monitor for safety status
4. Turn ON helmet module and verify conditions are met

The system is designed to keep the relay OFF (open circuit) when safety conditions aren't met. This is **correct behavior** for safety!
