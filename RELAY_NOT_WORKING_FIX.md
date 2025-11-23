# 🔧 Relay Not Working - Diagnostic & Fix Guide

## 🎯 Problem
Crash is detected in code, but relay doesn't physically respond (LED doesn't change, no clicking sound).

---

## 🔍 Step 1: Verify Relay Module Type

### Check Your Relay Module:

**Most relay modules are ACTIVE-LOW**, meaning:
- **LOW signal (0V)** = Relay ON (LED ON, clicking sound)
- **HIGH signal (3.3V)** = Relay OFF (LED OFF, no sound)

**This is OPPOSITE of what you might expect!**

### Quick Test:
1. **Disconnect relay from motorcycle**
2. **Upload `RELAY_TEST_SIMPLE.ino`**
3. **Watch the relay LED (DS1)**:
   - If LED turns ON when code says "OFF" → **Active-LOW relay**
   - If LED turns ON when code says "ON" → **Active-HIGH relay**

---

## 🔧 Step 2: Fix for Active-LOW Relay

If your relay is **active-LOW** (most common), we need to **invert the logic**:

### Change in Code:
```cpp
// OLD (Active-HIGH logic):
digitalWrite(relayPin, HIGH);  // Turn relay ON
digitalWrite(relayPin, LOW);   // Turn relay OFF

// NEW (Active-LOW logic):
digitalWrite(relayPin, LOW);   // Turn relay ON
digitalWrite(relayPin, HIGH);  // Turn relay OFF
```

### Apply Fix:
I'll create a version with inverted logic for you.

---

## 🔍 Step 3: Verify Wiring

### ESP32 to Relay Module:
```
ESP32 Pin    →    Relay Module
─────────────────────────────────
5V           →    VCC
GND          →    GND
GPIO 27      →    IN1
```

### Check:
1. **VCC**: Should be connected to ESP32 **5V** pin (not 3.3V)
2. **GND**: Should be connected to ESP32 **GND**
3. **IN1**: Should be connected to ESP32 **GPIO 27**

### Verify with Multimeter:
1. **Measure voltage** at IN1 pin:
   - Should be **0V** when relay should be ON
   - Should be **3.3V** when relay should be OFF
2. If no voltage change → **Wiring issue** or **GPIO 27 not working**

---

## 🔍 Step 4: Test Relay Module Directly

### Manual Test (Bypass ESP32):
1. **Disconnect IN1** from ESP32
2. **Connect IN1 to GND** (using jumper wire)
3. **Relay should turn ON** (LED ON, clicking sound)
4. **Disconnect IN1 from GND**
5. **Relay should turn OFF** (LED OFF)

### Results:
- **If relay responds** → ESP32 GPIO issue or code issue
- **If relay doesn't respond** → Relay module is faulty

---

## 🔍 Step 5: Check Relay Module Power

### Symptoms of Power Issue:
- Relay module LED is dim or off
- No clicking sound even when manually grounded
- Relay feels cold (no power)

### Solutions:
1. **Check 5V supply**: Measure voltage at VCC pin (should be 4.8-5.2V)
2. **Check current**: Relay needs 50-100mA, ensure ESP32 can supply it
3. **Use external 5V**: If ESP32 can't supply enough current, use external 5V source

---

## ✅ Solution: Inverted Logic Code

Since most relay modules are **active-LOW**, here's the fix:

### In `MotorcycleToWebApplication_OPTIMIZED.ino`:

**Change 1: Crash Detection**
```cpp
// OLD:
digitalWrite(relayPin, LOW);  // Force relay OFF immediately

// NEW (Active-LOW):
digitalWrite(relayPin, HIGH);  // Force relay OFF immediately (active-LOW)
```

**Change 2: Safety Check**
```cpp
// OLD:
digitalWrite(relayPin, LOW);  // Force relay OFF

// NEW (Active-LOW):
digitalWrite(relayPin, HIGH);  // Force relay OFF (active-LOW)
```

**Change 3: Setup**
```cpp
// OLD:
digitalWrite(relayPin, LOW);

// NEW (Active-LOW):
digitalWrite(relayPin, HIGH);  // Start with relay OFF (active-LOW)
```

**Change 4: Start Engine**
```cpp
// OLD:
digitalWrite(relayPin, HIGH);  // Turn relay ON

// NEW (Active-LOW):
digitalWrite(relayPin, LOW);  // Turn relay ON (active-LOW)
```

**Change 5: Stop Engine**
```cpp
// OLD:
digitalWrite(relayPin, LOW);  // Turn relay OFF

// NEW (Active-LOW):
digitalWrite(relayPin, HIGH);  // Turn relay OFF (active-LOW)
```

---

## 🧪 Testing Procedure

### Test 1: Upload Simple Test
1. **Upload `RELAY_TEST_SIMPLE.ino`**
2. **Watch relay LED (DS1)**
3. **Listen for clicking sound**
4. **Expected**: LED should toggle every 2 seconds

### Test 2: Verify Logic
1. **If LED is ON when code says "OFF"** → Active-LOW (need to invert)
2. **If LED is ON when code says "ON"** → Active-HIGH (current code is correct)

### Test 3: Apply Fix
1. **If active-LOW**: Apply inverted logic changes above
2. **Re-upload code**
3. **Test crash detection**
4. **Expected**: Relay should turn OFF (LED OFF) during crash

---

## 📊 Troubleshooting Matrix

| Symptom | Cause | Solution |
|---------|-------|----------|
| LED never changes | Wiring issue | Check GPIO 27 → IN1 connection |
| LED opposite of expected | Active-LOW relay | Invert logic (HIGH/LOW swap) |
| LED dim or flickering | Power issue | Check 5V supply, use external power |
| No clicking sound | Relay faulty | Replace relay module |
| Relay clicks but no effect | Motorcycle wiring | Check COM/NO connections |

---

## 🎯 Quick Diagnostic Commands

### In Serial Monitor, type:
- `STATUS` - Check system status
- `CRASH` - Trigger test crash
- Watch for relay response

### Expected Output:
```
🚨 EMERGENCY SHUTDOWN - Cutting relay power!
✓ Relay OFF - Engine cannot start
```

### Check Relay:
- **LED (DS1)**: Should change state
- **Sound**: Should hear click
- **Multimeter**: COM-NO should be OPEN (∞Ω)

---

## ✅ Success Criteria

Your relay is working correctly if:
1. ✅ LED (DS1) changes when crash detected
2. ✅ You hear clicking sound
3. ✅ Multimeter shows COM-NO opens (∞Ω)
4. ✅ Engine cannot start after crash
5. ✅ LED returns to normal after crash cleared

---

## 🚀 Next Steps

1. **Upload `RELAY_TEST_SIMPLE.ino`** to verify relay works
2. **Determine if active-LOW or active-HIGH**
3. **Apply appropriate fix** (invert logic if needed)
4. **Re-test crash detection**
5. **Verify engine cannot start during crash**

Let me know the results and I'll help you fix it! 🔧
