# 🏍️ Motorcycle Lean Detection Fix

## 🎯 Understanding Motorcycle Orientation:

### MPU6050 Axes:
```
        Forward (Motorcycle direction)
               ↑ X-axis
               |
               |
Left ←---------+--------→ Right
    Y-axis     |
               |
               ↓ Z-axis (Down/Gravity)
```

### Motorcycle Crash Scenarios:

**1. Falling Left/Right (CRASH):**
```
Normal:     Lean Left:    Lean Right:
   |           /              \
   |          /                \
   |         /                  \
  ===       ===                ===
  0°        60°                60°
```

**2. Forward/Backward Tilt (NOT CRASH):**
```
Normal:     Wheelie:      Stoppie:
   |           |              |
   |          /              \
   |         /                \
  ===       ===                ===
```

---

## ✅ CORRECT LEAN ANGLE CALCULATION:

### For Motorcycle (mounted flat on bike):

```cpp
// ✅ ROLL = Left/Right lean (what we want!)
float roll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;

// Positive = Leaning RIGHT
// Negative = Leaning LEFT
// 0° = Upright

// ✅ Use absolute value for crash detection
float leanAngle = abs(roll);

// ✅ Crash if leaning >60° either direction
if (leanAngle > 60) {
  // Motorcycle falling over!
}
```

### Visualization:

```
Lean Angle:  -90°  -60°  -30°   0°   30°   60°   90°
Direction:   LEFT  LEFT  LEFT  UP  RIGHT RIGHT RIGHT
Status:      CRASH CRASH  OK   OK   OK   CRASH CRASH
```

---

## 🔧 IMPLEMENTATION:

### Step 1: Calculate Lean Angle
```cpp
// In loop(), after reading MPU6050:
mpu.getEvent(&accel, &gyro, &temp);

// Calculate total acceleration
currentTotalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                         accel.acceleration.y * accel.acceleration.y +
                         accel.acceleration.z * accel.acceleration.z);

// ✅ Calculate ROLL (left/right lean)
float roll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
float leanAngle = abs(roll); // Absolute value (0-90°)

// Optional: Calculate pitch (forward/backward tilt)
float pitch = atan2(-accel.acceleration.x, 
                    sqrt(accel.acceleration.y * accel.acceleration.y + 
                         accel.acceleration.z * accel.acceleration.z)) * 180.0 / PI;
```

### Step 2: Crash Detection Logic
```cpp
// ✅ Detect crash based on LEFT/RIGHT LEAN
bool suddenImpact = (accelChange > 5.0);        // High impact
bool severeLean = (leanAngle > 60);             // Leaning >60° (falling over)
bool moderateImpact = (accelChange > 3.0);      // Moderate impact

// ✅ Crash if:
// - High impact (>5g) alone, OR
// - Moderate impact (>3g) + Severe lean (>60°)
if ((suddenImpact || (moderateImpact && severeLean)) && !crashDetected) {
  // CRASH DETECTED!
  sendCrashEvent();
}
```

### Step 3: Enhanced Serial Output
```cpp
Serial.printf("[SENSOR] Accel: %.2f | Lean: %.1f° | Pitch: %.1f°\n",
              currentTotalAccel, leanAngle, pitch);

// When crash detected:
Serial.printf("CRASH! Lean: %.1f° | Impact: %.2f g\n", leanAngle, accelChange);
```

---

## 📊 DETECTION SCENARIOS:

### Scenario 1: High-Speed Crash (Impact-Based)
```
Speed: 50 km/h
Impact: 12.0 g change
Lean: 25° (still upright)
Result: CRASH ✓ (suddenImpact = true)
Reason: High impact alone triggers crash
```

### Scenario 2: Low-Speed Tip-Over (Lean-Based)
```
Speed: 5 km/h
Impact: 4.0 g change
Lean: 75° (fallen over)
Result: CRASH ✓ (moderateImpact + severeLean = true)
Reason: Moderate impact + severe lean
```

### Scenario 3: Normal Cornering
```
Speed: 30 km/h
Impact: 1.5 g change
Lean: 35° (normal cornering)
Result: NO CRASH ✓
Reason: Lean angle within safe range
```

### Scenario 4: Wheelie/Stoppie
```
Speed: 20 km/h
Impact: 2.0 g change
Lean: 5° (upright)
Pitch: 45° (wheelie)
Result: NO CRASH ✓
Reason: Pitch doesn't trigger crash (only lean does)
```

### Scenario 5: Parking Stand
```
Speed: 0 km/h
Impact: 0.5 g change
Lean: 20° (on kickstand)
Result: NO CRASH ✓
Reason: Lean angle within safe range
```

---

## 🧪 TESTING:

### Test 1: Manual Crash Command
```
1. Open Serial Monitor
2. Type: CRASH
3. Should see:
   🧪 MANUAL CRASH TEST TRIGGERED!
   📍 Sending test crash WITH GPS
   ✓ Check dashboard for crash marker!
```

### Test 2: Physical Lean Test
```
1. Tilt motorcycle >60° to the left
2. Should trigger crash detection
3. Serial: "CRASH! Lean: 75.0° | Impact: 4.2 g"
4. Dashboard should show crash marker
```

### Test 3: Normal Operation
```
1. Keep motorcycle upright (0-30° lean)
2. Should NOT trigger crash
3. Serial: "Lean: 15.0° | Status: OK"
```

---

## 🐛 TROUBLESHOOTING:

### Issue 1: Crash not appearing on dashboard

**Possible Causes:**
1. Firebase not receiving crash event
2. GPS coordinates invalid
3. Dashboard not listening for crashes

**Debug Steps:**
```cpp
// Check Serial output:
Serial.println("[FIREBASE] Sending crash event...");
Serial.println("[FIREBASE] Crash payload: " + payload);
Serial.printf("[FIREBASE] HTTP Response: %d\n", code);

// Should see:
[FIREBASE] ✓ Crash sent successfully! HTTP: 200
✓ Crash location will appear on dashboard map!
```

**Fix:**
```cpp
// Ensure crash is sent to correct path:
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;

// Verify payload includes:
doc["lat"] = gps.location.lat();
doc["lng"] = gps.location.lng();
doc["hasGPS"] = true;
doc["impactStrength"] = accelChange;
doc["roll"] = leanAngle;
doc["type"] = "crash";
```

### Issue 2: False alarms from normal riding

**Cause:** Lean threshold too low

**Fix:**
```cpp
// Increase lean threshold
bool severeLean = (leanAngle > 70); // Was 60°

// Or add speed check
bool isMoving = (gps.speed.kmph() > 5.0);
if ((suddenImpact || (moderateImpact && severeLean)) && isMoving) {
  // Only detect when moving
}
```

### Issue 3: Missing real crashes

**Cause:** Thresholds too high

**Fix:**
```cpp
// Decrease thresholds
bool suddenImpact = (accelChange > 3.0);      // Was 5.0
bool severeLean = (leanAngle > 50);           // Was 60
bool moderateImpact = (accelChange > 2.0);    // Was 3.0
```

---

## 📈 RECOMMENDED SETTINGS:

### Conservative (Fewer False Alarms):
```cpp
bool suddenImpact = (accelChange > 7.0);
bool severeLean = (leanAngle > 70);
bool moderateImpact = (accelChange > 5.0);
```

### Balanced (Current):
```cpp
bool suddenImpact = (accelChange > 5.0);
bool severeLean = (leanAngle > 60);
bool moderateImpact = (accelChange > 3.0);
```

### Sensitive (Catches More Crashes):
```cpp
bool suddenImpact = (accelChange > 3.0);
bool severeLean = (leanAngle > 50);
bool moderateImpact = (accelChange > 2.0);
```

---

## 🎯 VERIFICATION CHECKLIST:

- [ ] Lean angle calculated correctly (atan2(y, z))
- [ ] Absolute value used (abs(roll))
- [ ] Threshold set appropriately (>60°)
- [ ] Serial output shows lean angle
- [ ] Manual test works ("CRASH" command)
- [ ] Crash sent to Firebase (/crashes path)
- [ ] Dashboard receives crash event
- [ ] Map marker appears at GPS location
- [ ] Alert notification shows
- [ ] No false alarms during normal riding

---

## 🚀 EXPECTED RESULTS:

✅ **Motorcycle falls left (75°)** → CRASH DETECTED → Dashboard shows marker  
✅ **Motorcycle falls right (80°)** → CRASH DETECTED → Dashboard shows marker  
✅ **High-speed impact (12g)** → CRASH DETECTED → Dashboard shows marker  
✅ **Normal cornering (35°)** → No crash → Dashboard normal  
✅ **Parking on kickstand (20°)** → No crash → Dashboard normal  
✅ **Manual test ("CRASH")** → Test crash → Dashboard shows marker  

The system will now accurately detect motorcycle crashes based on left/right lean!
