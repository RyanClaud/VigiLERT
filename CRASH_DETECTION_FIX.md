# Crash Detection Fix - Left/Right Lean Detection

## Problems Fixed

### Problem 1: Slow Response
**Before:** `delay(100)` = 10 Hz loop rate
**After:** `delay(50)` = 20 Hz loop rate
**Result:** 2x faster crash detection!

### Problem 2: Wrong Axis Detection
**Before:** Detected forward/backward tilt (roll)
**After:** Detects left/right lean (pitch)
**Result:** Proper motorcycle crash detection!

## Understanding Motorcycle Orientation

### Roll vs Pitch
```
        PITCH (Left/Right Lean)
              ↑
              |
    Left ←----+----→ Right
              |
              ↓

        ROLL (Forward/Backward Tilt)
              ↑
              |
  Forward ←---+---→ Backward
              |
              ↓
```

### For Motorcycles:
- **Pitch** = Leaning left or right (CRASH!)
- **Roll** = Tilting forward or backward (normal riding)

## New Crash Detection Logic

### Detection Criteria:
```cpp
bool leftLean = pitch < -45;   // Leaning left more than 45°
bool rightLean = pitch > 45;   // Leaning right more than 45°
bool highImpact = totalAcceleration >= 15.0; // High impact force
```

### Triggers Crash When:
1. **Lean left** > 45° → Motorcycle falling to left side
2. **Lean right** > 45° → Motorcycle falling to right side
3. **High impact** > 15 m/s² → Collision or sudden stop

### Clears Crash When:
- Acceleration < 15 m/s²
- Pitch between -30° and 30° (bike upright)

## Serial Monitor Output

### Normal Riding:
```
[MPU6050] Accel:9.8 | Pitch:5.2° (Lean) | Roll:2.1° | Temp:32.2°C
Accel Total: 9.81 | Roll: 2.13 | Battery: 3.7V | Helmet: ON | GPS: OK
```

### Crash Detected (Lean Left):
```
⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️
Acceleration: 12.3 m/s² | Pitch: -52.4° (Lean) | Roll: 15.2°
→ LEANING LEFT!
Location: 12.747488, 121.482131
[FIREBASE] Crash event sent: 200
```

### Crash Detected (Lean Right):
```
⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️
Acceleration: 11.8 m/s² | Pitch: 58.3° (Lean) | Roll: -8.1°
→ LEANING RIGHT!
Location: 12.747488, 121.482131
[FIREBASE] Crash event sent: 200
```

### Crash Detected (High Impact):
```
⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️
Acceleration: 18.5 m/s² | Pitch: 12.3° (Lean) | Roll: 5.2°
→ HIGH IMPACT!
Location: 12.747488, 121.482131
[FIREBASE] Crash event sent: 200
```

## Dashboard Integration

### Firebase Data Structure:
```json
{
  "crashes": {
    "-ORRTpFsQcpGXqIlvj_0": {
      "timestamp": 1732248246000,
      "lat": 12.747488,
      "lng": 121.482131,
      "hasGPS": true,
      "impactStrength": 12.3,
      "roll": -52.4,
      "leanAngle": -52.4
    }
  }
}
```

### Dashboard Display:
- Crash alert appears immediately
- Shows location on map
- Displays lean angle
- Plays alert sound
- Sends to emergency contacts

## Testing

### Test 1: Lean Left
1. Hold ESP32 flat
2. Tilt left more than 45°
3. Should see: "⚠️ CRASH DETECTED! → LEANING LEFT!"
4. Check Dashboard for crash alert

### Test 2: Lean Right
1. Hold ESP32 flat
2. Tilt right more than 45°
3. Should see: "⚠️ CRASH DETECTED! → LEANING RIGHT!"
4. Check Dashboard for crash alert

### Test 3: High Impact
1. Shake ESP32 vigorously
2. Should see: "⚠️ CRASH DETECTED! → HIGH IMPACT!"
3. Check Dashboard for crash alert

### Test 4: Normal Riding
1. Tilt ESP32 gently (< 45°)
2. Should NOT trigger crash detection
3. Dashboard should show normal status

## Adjusting Sensitivity

### More Sensitive (Detect smaller leans):
```cpp
bool leftLean = pitch < -35;   // 35° instead of 45°
bool rightLean = pitch > 35;
```

### Less Sensitive (Only detect severe crashes):
```cpp
bool leftLean = pitch < -60;   // 60° instead of 45°
bool rightLean = pitch > 60;
```

### Impact Threshold:
```cpp
const float ACCEL_THRESHOLD = 12.0; // Lower = more sensitive
const float ACCEL_THRESHOLD = 20.0; // Higher = less sensitive
```

## Performance Improvements

### Loop Rate:
- **Before:** 10 Hz (100ms delay)
- **After:** 20 Hz (50ms delay)
- **Result:** Detects crashes 2x faster!

### Response Time:
- **Before:** Up to 100ms delay
- **After:** Up to 50ms delay
- **Real-world:** Crash detected within 50-100ms

### Serial Output:
- **Before:** Verbose debug every 2 seconds
- **After:** Compact status every 5 seconds
- **Result:** Cleaner Serial Monitor, easier to read

## Troubleshooting

### Issue: False Positives (Triggers Too Often)
**Solution:** Increase lean angle threshold
```cpp
bool leftLean = pitch < -55;   // More strict
bool rightLean = pitch > 55;
```

### Issue: Not Detecting Crashes
**Solution:** Decrease lean angle threshold
```cpp
bool leftLean = pitch < -40;   // More sensitive
bool rightLean = pitch > 40;
```

### Issue: Crash Doesn't Appear on Dashboard
**Check 1:** Serial Monitor shows "Crash event sent: 200"?
- If yes → Arduino is working
- If no → Check WiFi connection

**Check 2:** Firebase Console shows crash data?
- Go to `/helmet_public/{userId}/crashes`
- Should see new entry with timestamp

**Check 3:** Dashboard listening to crashes?
- Open browser console
- Look for crash event logs

### Issue: Crash Clears Too Quickly
**Solution:** Increase clear threshold
```cpp
if (pitch > -20 && pitch < 20 && crashDetected) {
  // More strict - bike must be more upright
}
```

## Summary

✅ **Fixed:** Now detects left/right lean (pitch) instead of forward/backward tilt (roll)
✅ **Faster:** 20 Hz loop rate (2x faster response)
✅ **Clearer:** Better Serial Monitor output
✅ **Accurate:** Proper motorcycle crash detection
✅ **Real-time:** Crashes appear on Dashboard immediately
✅ **Adjustable:** Easy to tune sensitivity

Upload the updated code and test by tilting the ESP32 left/right!
