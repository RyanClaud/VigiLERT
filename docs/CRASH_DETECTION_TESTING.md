# 🚨 Crash Detection Testing Guide

## ⚠️ Current Issue

Your serial monitor shows:
```
[SENSOR] Accel: 0.00 | Roll: 0.0° | Battery: 0.80V | Helmet: ON
```

**Problem**: `Accel: 0.00` means MPU6050 is not reading properly!

---

## 🔧 What Was Fixed

### 1. **MPU6050 Diagnostics**
- Automatic warning every 10 seconds if sensor reads 0.00
- Checks if sensor is working
- Shows troubleshooting tips

### 2. **Enhanced Crash Detection Logging**
- Shows detailed crash information
- Logs Firebase send status
- Shows HTTP response codes

### 3. **Manual Testing Commands**
You can now test crash detection via Serial Monitor:
```
CRASH      - Trigger test crash event
MPU        - Test MPU6050 sensor
GPS        - Show GPS diagnostics
HELP       - Show all commands
```

---

## 🧪 Testing Steps

### Step 1: Check MPU6050

**In Serial Monitor, type:**
```
MPU
```

**Expected Output (Working):**
```
🧪 MPU6050 TEST:
Accel X: 0.12 | Y: -0.05 | Z: 9.81
Total Accel: 9.82 g
Roll: 0.5°
Crash Threshold: 15.00 g
✓ MPU6050 is working!
```

**Expected Output (Not Working):**
```
🧪 MPU6050 TEST:
Accel X: 0.00 | Y: 0.00 | Z: 0.00
Total Accel: 0.00 g
Roll: 0.0°
Crash Threshold: 15.00 g
⚠️ MPU6050 NOT WORKING! Check wiring!
```

---

### Step 2: Test Crash Detection

**In Serial Monitor, type:**
```
CRASH
```

**Expected Output:**
```
🧪 MANUAL CRASH TEST TRIGGERED!
Simulating crash event...
Sending test crash with GPS: 12.747442, 121.482063

[FIREBASE] Sending crash event WITH GPS...
[FIREBASE] Crash payload: {"timestamp":1700000000000,"lat":12.747442,"lng":121.482063,"hasGPS":true,"impactStrength":20.0,"roll":0.5}
[FIREBASE] ✓ Crash sent successfully! HTTP: 200

✓ Test crash sent to Firebase! Check dashboard.
```

**Then check your dashboard:**
- Crash should appear in "Crash Site Locations" section
- Red marker should appear on map
- Alert notification should show

---

## 🔍 MPU6050 Troubleshooting

### Issue: Accel reads 0.00

**Possible Causes:**
1. MPU6050 not wired correctly
2. MPU6050 not powered
3. Wrong I2C address
4. Faulty MPU6050 module

### Solution 1: Check Wiring

```
MPU6050    →    ESP32
─────────────────────
VCC        →    3.3V
GND        →    GND
SDA        →    GPIO 21
SCL        →    GPIO 22
```

### Solution 2: Check I2C Address

Most MPU6050 modules use address `0x68`, but some use `0x69`.

**Test I2C Scanner:**
```cpp
// Add to setup() temporarily:
Wire.begin(21, 22);
Serial.println("Scanning I2C...");
for (byte i = 0; i < 127; i++) {
  Wire.beginTransmission(i);
  if (Wire.endTransmission() == 0) {
    Serial.printf("Found device at 0x%02X\n", i);
  }
}
```

**Expected Output:**
```
Scanning I2C...
Found device at 0x68  ← MPU6050
```

If you see `0x69` instead, you need to change the address:
```cpp
// In Adafruit_MPU6050 library, change:
mpu.begin(0x69); // Instead of default 0x68
```

### Solution 3: Test MPU6050 Separately

Upload this simple test code:
```cpp
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1) delay(10);
  }
  
  Serial.println("MPU6050 found!");
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  Serial.printf("Accel X:%.2f Y:%.2f Z:%.2f\n", 
                a.acceleration.x, a.acceleration.y, a.acceleration.z);
  delay(500);
}
```

**Expected Output:**
```
MPU6050 found!
Accel X:0.12 Y:-0.05 Z:9.81
Accel X:0.15 Y:-0.03 Z:9.79
```

If still showing 0.00, MPU6050 module is likely faulty.

---

## 🎯 Crash Detection Thresholds

### Current Settings:
```cpp
const float ACCEL_THRESHOLD = 15.0;  // 15g acceleration
Roll angle: < -47° or > 40°
```

### What Triggers Crash:
1. **High Impact**: Total acceleration ≥ 15g
2. **Severe Tilt**: Roll angle < -47° or > 40°

### Normal Values:
- **Stationary**: ~9.8g (gravity)
- **Normal riding**: 9-12g
- **Hard braking**: 12-15g
- **Crash**: >15g

### Testing Thresholds:

**Gentle Test (Lower threshold temporarily):**
```cpp
// Change in code:
const float ACCEL_THRESHOLD = 12.0;  // Lower for testing
```

**Tilt Test:**
```
1. Tilt device sharply left (>40°)
2. Or tilt sharply right (<-47°)
3. Should trigger crash detection
```

---

## 📊 Expected Serial Output

### Normal Operation:
```
[SENSOR] Accel: 9.81 | Roll: 0.5° | Battery: 12.4V | Helmet: ON
[GPS] Lat: 12.747442 | Lng: 121.482063 | Speed: 0.0 km/h
      Sats: 9 | HDOP: 1.3 | Time: 14:30:45
```

### Crash Detected:
```
⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️
Impact: 18.50 g | Roll: 0.5° | Threshold: 15.00 g
Sending crash to Firebase with GPS: 12.747442, 121.482063

[FIREBASE] Sending crash event WITH GPS...
[FIREBASE] Crash payload: {"timestamp":1700000000000,"lat":12.747442,"lng":121.482063,"hasGPS":true,"impactStrength":18.5,"roll":0.5}
[FIREBASE] ✓ Crash sent successfully! HTTP: 200

✓ Crash event sent to Firebase!
```

### Dashboard Updates:
```
1. Vehicle Status card turns RED
2. Shows "Crash Detected"
3. Alert notification appears
4. Warning sound plays
5. Crash marker appears on map
6. Crash listed in "Crash Site Locations"
```

---

## 🐛 Common Issues

### Issue 1: MPU6050 reads 0.00
**Solution**: Check wiring, power, I2C address

### Issue 2: Crash detected but not on dashboard
**Solution**: 
1. Check Firebase connection
2. Check HTTP response code
3. Refresh dashboard
4. Check browser console (F12)

### Issue 3: False crash detections
**Solution**: Increase threshold:
```cpp
const float ACCEL_THRESHOLD = 18.0; // Higher threshold
```

### Issue 4: Crash not detected
**Solution**: Lower threshold for testing:
```cpp
const float ACCEL_THRESHOLD = 12.0; // Lower threshold
```

---

## 📋 Quick Checklist

Before testing:
- [ ] MPU6050 wired correctly (SDA:21, SCL:22)
- [ ] MPU6050 powered (3.3V)
- [ ] Serial Monitor open (115200 baud)
- [ ] WiFi connected
- [ ] Firebase accessible
- [ ] Dashboard open in browser

Test sequence:
- [ ] Type `MPU` - Check sensor working
- [ ] Type `CRASH` - Test crash detection
- [ ] Check Serial Monitor for success message
- [ ] Check dashboard for crash marker
- [ ] Check Firebase console for crash data

---

## 🚀 Next Steps

1. **Upload enhanced code**
2. **Open Serial Monitor**
3. **Type `MPU`** to test sensor
4. **If sensor works**: Type `CRASH` to test detection
5. **If sensor doesn't work**: Check wiring and power
6. **Check dashboard** for crash marker

---

## 📞 Still Not Working?

If MPU6050 still reads 0.00:
1. Try different MPU6050 module (may be faulty)
2. Try different I2C pins
3. Check for loose connections
4. Verify 3.3V power supply
5. Test with simple MPU6050 example code

If crash sends but doesn't show on dashboard:
1. Check Firebase Console → Realtime Database
2. Look for `/helmet_public/{userUID}/crashes`
3. Verify data is there
4. Refresh dashboard (Ctrl+R)
5. Check browser console for errors (F12)

**Your crash detection system should now work!** 🚨
