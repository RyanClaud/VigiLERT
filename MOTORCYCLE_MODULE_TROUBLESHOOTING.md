# Motorcycle Module Troubleshooting Guide 🔧

## Issues Detected from Serial Monitor

Based on your Serial Monitor output, here are the issues and solutions:

### 1. ⚠️ MPU6050 Reading 0.00 (CRITICAL)

**Symptom:**
```
[SENSOR] Accel: 0.00 | Roll: 0.0°
⚠️ WARNING: MPU6050 reading 0.00! Sensor may not be working!
```

**Cause:** MPU6050 sensor not connected or not working

**Solutions:**
1. **Check Wiring:**
   - SDA → GPIO 21
   - SCL → GPIO 22
   - VCC → 3.3V
   - GND → GND

2. **Check I2C Address:**
   ```cpp
   // Try scanning for I2C devices
   Wire.beginTransmission(0x68);
   if (Wire.endTransmission() == 0) {
     Serial.println("MPU6050 found at 0x68");
   }
   ```

3. **Test MPU6050:**
   - Upload I2C scanner sketch
   - Check if device appears at 0x68 or 0x69
   - If not found, check connections

**Impact:** Crash detection won't work without MPU6050!

---

### 2. 🛰️ GPS No Fix

**Symptom:**
```
[GPS] Chars: 10122 | Sentences: 0 | Failed: 27
GPS: ✗ NO FIX
```

**Cause:** GPS receiving data but no valid location

**Why This Happens:**
- GPS needs clear view of sky
- Can take 30-60 seconds for first fix
- Indoor testing won't work

**Solutions:**
1. **Move to Open Area:**
   - Go outside
   - Clear view of sky
   - Away from buildings

2. **Wait for Fix:**
   - First fix: 30-60 seconds
   - Subsequent fixes: 5-10 seconds

3. **Check Antenna:**
   - Ensure antenna is connected
   - Antenna should be facing up

**Current Status:** GPS module is working (receiving data), just needs better location

---

### 3. 🔋 Battery Reading 0.00V

**Symptom:**
```
Battery: 0.00V
```

**Cause:** Battery voltage pin not connected or wrong pin

**Solutions:**
1. **Check Pin Connection:**
   ```cpp
   const int batteryPin = 34;  // GPIO 34
   ```
   - Connect battery voltage divider to GPIO 34
   - Use voltage divider: Battery → 10kΩ → GPIO34 → 10kΩ → GND

2. **Voltage Divider Circuit:**
   ```
   Battery+ (12V) ──┬── 10kΩ ──┬── GPIO34
                    │          │
                    │          └── 10kΩ ── GND
                    │
                  Battery-
   ```

3. **Test Reading:**
   ```cpp
   int rawValue = analogRead(batteryPin);
   float voltage = rawValue * (3.3 / 4095.0) * 4.3;
   Serial.printf("Raw: %d, Voltage: %.2fV\n", rawValue, voltage);
   ```

---

### 4. 📡 GSM 0% Signal

**Symptom:**
```
GSM: 0%
```

**Cause:** GSM module not responding or no SIM card

**Solutions:**
1. **Check GSM Module:**
   - Power: 5V (needs more current than ESP32 can provide)
   - Use external power supply
   - TX → GPIO 4
   - RX → GPIO 5

2. **Check SIM Card:**
   - SIM card inserted correctly
   - SIM card activated
   - Has credit/data plan

3. **Test GSM:**
   ```cpp
   gsmSerial.println("AT");
   delay(1000);
   if (gsmSerial.available()) {
     Serial.println("GSM responding!");
   }
   ```

---

### 5. 🔗 Pairing Status: Disconnected

**Symptom:**
Dashboard shows "Disconnected" even though device is on

**Cause:** Heartbeat not being sent or timestamp issue

**Check Serial Monitor For:**
```
[HEARTBEAT] Sending motorcycle heartbeat to Firebase...
[FIREBASE] Motorcycle heartbeat sent: 1700000006000
```

**If You Don't See These Messages:**
1. **Re-upload the code** - Make sure latest version is uploaded
2. **Check WiFi connection** - Must be connected to send heartbeats
3. **Check Firebase URL** - Verify firebaseHost is correct

**If You See These Messages:**
1. **Check Dashboard console** (F12)
2. **Look for:** `[DEBUG] Motorcycle: CONNECTED`
3. **If still disconnected:** Clear browser cache and hard refresh

---

## Quick Diagnostic Checklist

### ✅ What's Working:
- [x] ESP32 boots successfully
- [x] WiFi connects
- [x] GPS module receives data
- [x] Helmet switch detected (ON)
- [x] Firebase connection works

### ❌ What's Not Working:
- [ ] MPU6050 sensor (0.00 readings)
- [ ] GPS fix (no location yet)
- [ ] Battery voltage reading
- [ ] GSM module
- [ ] Pairing status on dashboard

---

## Priority Fixes

### 🔴 CRITICAL (Fix First):
1. **MPU6050** - Required for crash detection
   - Check wiring to GPIO 21/22
   - Verify 3.3V power
   - Test with I2C scanner

### 🟡 IMPORTANT (Fix Soon):
2. **Battery Voltage** - Required for monitoring
   - Add voltage divider circuit
   - Connect to GPIO 34

3. **Pairing Status** - Required for dashboard
   - Re-upload latest code
   - Check Serial Monitor for heartbeat messages
   - Refresh dashboard

### 🟢 OPTIONAL (Can Wait):
4. **GPS Fix** - Will work once outside
   - Test in open area
   - Wait 60 seconds

5. **GSM Module** - Optional feature
   - Add external power supply
   - Insert activated SIM card

---

## Testing Steps

### Step 1: Fix MPU6050
```
1. Power off ESP32
2. Check wiring: SDA→21, SCL→22, VCC→3.3V, GND→GND
3. Power on ESP32
4. Check Serial Monitor for "MPU6050 initialized"
5. Should see non-zero Accel values
```

### Step 2: Test Heartbeat
```
1. Upload latest code
2. Open Serial Monitor
3. Look for "[HEARTBEAT] Sending motorcycle heartbeat..."
4. Should appear every 2 seconds
5. Check dashboard - should show Connected
```

### Step 3: Test GPS (Outside)
```
1. Take device outside
2. Place in open area
3. Wait 60 seconds
4. Check Serial Monitor for "GPS: ✓ VALID"
5. Should see Lat/Lng coordinates
```

### Step 4: Add Battery Monitoring
```
1. Build voltage divider circuit
2. Connect to GPIO 34
3. Check Serial Monitor for voltage reading
4. Should show ~12V (or your battery voltage)
```

---

## Expected Serial Monitor Output (When Working)

```
[SETUP] Starting VigiLERT System - COMPLETE VERSION...
[SUCCESS] MPU6050 initialized.
[SETUP] GPS Serial started on RX:16, TX:17
Connected IP: 192.168.1.100

[SENSOR] Accel: 9.81 | Roll: 2.3° | Battery: 12.6V | Helmet: ON
[GPS] Lat: 14.599512 | Lng: 120.984222 | Speed: 0.0 km/h | Sats: 8
[GSM: 85% | Engine: STOPPED

[HEARTBEAT] Sending motorcycle heartbeat to Firebase...
[FIREBASE] Motorcycle heartbeat sent: 1700000006000

[SENSOR] Accel: 9.80 | Roll: 2.1° | Battery: 12.6V | Helmet: ON
[GPS] Lat: 14.599512 | Lng: 120.984222 | Speed: 0.0 km/h | Sats: 8
```

---

## Dashboard Expected State (When Working)

```
┌─────────────────────────────────────┐
│ 🔗 PAIRING STATUS                   │
│                                     │
│ Connected ✓                         │
│                                     │
│ 🟢 Helmet: ON                       │
│ 🟢 Motorcycle: ON                   │
└─────────────────────────────────────┘

┌─────────────────────────────────────┐
│ 👤 Rider Status                     │
│                                     │
│ Active                              │
│ Helmet connected                    │
│                                     │
│ Background: BLUE                    │
└─────────────────────────────────────┘
```

---

## Summary

**Immediate Actions:**
1. ✅ Re-upload the latest Arduino code
2. ✅ Fix MPU6050 wiring (most critical)
3. ✅ Check Serial Monitor for heartbeat messages
4. ✅ Hard refresh dashboard (Ctrl+Shift+R)

**After Basic Fixes:**
5. Test GPS outside in open area
6. Add battery voltage divider circuit
7. Set up GSM module with external power

The most critical issue is the **MPU6050 reading 0.00** - this must be fixed for crash detection to work. The pairing status should resolve once you re-upload the code and see the heartbeat messages in Serial Monitor.
