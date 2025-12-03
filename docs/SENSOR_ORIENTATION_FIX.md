# 🧭 MPU6050 Sensor Orientation Fix

## 🚨 THE PROBLEM:

From your Serial output:
```
Lean: 5.9° | Lean: 5.4° | Lean: 6.5° | Lean: 4.7°
```

**Lean angles are only 4-6°** even when tilting the motorcycle!

This means:
1. The MPU6050 is mounted in a different orientation than expected
2. The lean angle calculation is using the wrong axis
3. We need to determine which axis actually shows left/right tilt

---

## 🔍 DIAGNOSIS:

### Step 1: Run Orientation Test

1. **Upload the updated Arduino code**
2. **Open Serial Monitor** (115200 baud)
3. **Type:** `ORIENT`
4. **Tilt the motorcycle:**
   - **LEFT** - Watch which value changes
   - **RIGHT** - Watch which value changes
   - **FORWARD** - Watch which value changes
   - **BACKWARD** - Watch which value changes

### Step 2: Identify the Correct Axis

The output will show:
```
X: -0.52 | Y:  0.31 | Z:  9.75 | Roll:  1.8° | Pitch: -3.1° | Total: 9.81
X: -0.48 | Y:  2.15 | Z:  9.62 | Roll: 12.6° | Pitch: -2.8° | Total: 9.85
X: -0.51 | Y: -1.82 | Z:  9.58 | Roll:-10.8° | Pitch: -3.0° | Total: 9.82
```

**Look for:**
- Which axis (X, Y, or Z) changes the most when tilting LEFT/RIGHT?
- Which angle (Roll or Pitch) changes the most?

---

## ✅ SOLUTION OPTIONS:

### Option 1: Use Acceleration Change (Current - Recommended)

Instead of relying on lean angle, detect crashes based on **sudden acceleration changes**:

```cpp
// ✅ Works regardless of sensor orientation
float accelChange = abs(currentTotalAccel - previousTotalAccel);

if (accelChange > 5.0) {
  // Sudden impact detected!
}
```

**Benefits:**
- Works with any sensor orientation
- Detects high-speed impacts
- No calibration needed

### Option 2: Use Acceleration Deviation

Detect when total acceleration deviates significantly from gravity:

```cpp
// ✅ Detects when bike is not upright
float accelDeviation = abs(currentTotalAccel - 9.81);

if (accelDeviation > 3.0) {
  // Bike is tilted or impacted!
}
```

**Benefits:**
- Detects tip-overs
- Works with any orientation
- Simple calculation

### Option 3: Fix Lean Angle Calculation

If you want proper lean angle detection, we need to know your sensor orientation:

#### If sensor is mounted FLAT (horizontal):
```cpp
// Use X-axis for lean
float leanAngle = abs(atan2(accel.acceleration.x, accel.acceleration.z) * 180.0 / PI);
```

#### If sensor is mounted VERTICAL (standing up):
```cpp
// Use Y-axis for lean
float leanAngle = abs(atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI);
```

#### If sensor is mounted SIDEWAYS:
```cpp
// Use different axis combination
float leanAngle = abs(atan2(accel.acceleration.x, accel.acceleration.y) * 180.0 / PI);
```

---

## 🔧 CURRENT IMPLEMENTATION:

The code now uses **THREE detection methods**:

### Method 1: Sudden Impact (Primary)
```cpp
bool suddenImpact = (accelChange > 5.0);
// Detects: High-speed crashes, collisions
```

### Method 2: Lean Angle (Secondary)
```cpp
bool severeLean = (leanAngle > 60);
// Detects: Tip-overs (if sensor mounted correctly)
```

### Method 3: Acceleration Deviation (Backup)
```cpp
float accelDeviation = abs(currentTotalAccel - 9.81);
bool severeShock = (accelDeviation > 3.0);
// Detects: Any significant deviation from normal
```

### Combined Logic:
```cpp
if (suddenImpact || (moderateImpact && severeLean) || severeShock) {
  // CRASH DETECTED!
}
```

---

## 🧪 TESTING:

### Test 1: Orientation Test
```
1. Type: ORIENT
2. Tilt motorcycle in all directions
3. Note which axis changes most
4. Report results
```

### Test 2: Manual Crash Test
```
1. Type: CRASH
2. Should trigger regardless of orientation
3. Check dashboard for crash marker
```

### Test 3: Physical Tilt Test
```
1. Tilt motorcycle >60° to side
2. Should trigger if lean angle works
3. Or trigger based on acceleration deviation
```

### Test 4: Shake Test
```
1. Shake motorcycle vigorously
2. Should trigger based on acceleration change
3. Simulates impact scenario
```

---

## 📊 EXPECTED BEHAVIOR:

### With Current Code:

**Scenario 1: High-Speed Impact**
```
Acceleration change: 12.0 g
Result: CRASH ✓ (suddenImpact)
```

**Scenario 2: Tip-Over (if lean works)**
```
Lean angle: 75°
Acceleration change: 4.0 g
Result: CRASH ✓ (moderateImpact + severeLean)
```

**Scenario 3: Tip-Over (if lean doesn't work)**
```
Acceleration deviation: 5.0 g
Result: CRASH ✓ (severeShock)
```

**Scenario 4: Normal Riding**
```
Acceleration change: 1.5 g
Lean angle: 35°
Deviation: 1.0 g
Result: NO CRASH ✓
```

---

## 🎯 RECOMMENDATIONS:

### Immediate (No Changes Needed):
1. **Use current code** - Works with acceleration change
2. **Test with "CRASH" command** - Should work immediately
3. **Physical shake test** - Should trigger on severe shaking

### If You Want Proper Lean Detection:
1. **Run "ORIENT" test** - Identify correct axis
2. **Report which axis changes** - I'll fix the calculation
3. **Re-upload code** - With corrected lean angle formula

### If Crashes Still Don't Trigger:
1. **Lower thresholds:**
   ```cpp
   bool suddenImpact = (accelChange > 3.0);      // Was 5.0
   bool severeShock = (accelDeviation > 2.0);    // Was 3.0
   ```

2. **Add debug logging:**
   ```cpp
   Serial.printf("Change: %.2f | Deviation: %.2f | Lean: %.1f\n",
                 accelChange, accelDeviation, leanAngle);
   ```

---

## 🚀 QUICK FIX:

If you want crashes to trigger immediately, **lower the thresholds**:

```cpp
// In MotorcycleToWebApplication_COMPLETE.ino, find:
bool suddenImpact = (accelChange > 5.0);
bool severeShock = (accelDeviation > 3.0);

// Change to:
bool suddenImpact = (accelChange > 2.0);      // More sensitive
bool severeShock = (accelDeviation > 1.5);    // More sensitive
```

This will make it trigger on smaller impacts, but may cause false alarms.

---

## 📈 NEXT STEPS:

1. **Upload updated code** (with ORIENT command)
2. **Run orientation test** - Type "ORIENT" in Serial Monitor
3. **Report results** - Which axis changes when tilting left/right?
4. **I'll provide exact fix** - Based on your sensor orientation

The current code should work with acceleration-based detection even if lean angle is wrong!
