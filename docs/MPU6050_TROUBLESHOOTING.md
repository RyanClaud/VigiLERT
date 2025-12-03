# MPU6050 Crash Detection Troubleshooting

## Problem
Roll angle shows 0.00° even when tilting the sensor left and right.

## Root Cause
The MPU6050 is not reading data correctly. This can be caused by:
1. **I2C wiring issues** - Wrong pins or loose connections
2. **Power issues** - MPU6050 not getting proper 3.3V
3. **I2C address conflict** - Wrong address or multiple devices
4. **Faulty module** - Damaged MPU6050

## Diagnostic Steps

### Step 1: Check Serial Monitor Output

Upload the updated `MotorcycleToWebApplication_FIXED.ino` and look for:

```
[SETUP] Scanning I2C bus...
✓ I2C device found at address 0x68
  → This is likely the MPU6050!
✓ Found 1 I2C device(s)

[SUCCESS] MPU6050 initialized.
[SETUP] Testing MPU6050 readings...
[MPU6050 Test 1] X:0.12 Y:-0.34 Z:9.78 | Temp:25.3°C
[SUCCESS] MPU6050 is working! Total acceleration: 9.81 m/s²
```

### Step 2: Identify the Issue

#### Issue A: No I2C Devices Found
```
✗ No I2C devices found!
  Check wiring: SDA=GPIO21, SCL=GPIO22
```

**Solution:**
1. Check wiring connections:
   - MPU6050 VCC → ESP32 3.3V
   - MPU6050 GND → ESP32 GND
   - MPU6050 SDA → ESP32 GPIO 21
   - MPU6050 SCL → ESP32 GPIO 22
2. Make sure connections are secure (not loose)
3. Try different jumper wires
4. Check if MPU6050 LED is on (if it has one)

#### Issue B: MPU6050 Found But Not Initializing
```
✓ I2C device found at address 0x68
[ERROR] Failed to find MPU6050 chip (attempt 1)
```

**Solution:**
1. Try different I2C address:
   ```cpp
   // In setup(), try:
   mpu.begin(0x69); // Instead of default 0x68
   ```
2. Check if AD0 pin is connected:
   - AD0 = GND → Address 0x68
   - AD0 = VCC → Address 0x69
3. Add pull-up resistors (4.7kΩ) on SDA and SCL lines

#### Issue C: MPU6050 Initialized But Reading Zeros
```
[SUCCESS] MPU6050 initialized.
[MPU6050 Test 1] X:0.00 Y:0.00 Z:0.00 | Temp:0.0°C
⚠️ WARNING: Total acceleration too low! MPU6050 may not be working!
```

**Solution:**
1. MPU6050 is in sleep mode - Add this after `mpu.begin()`:
   ```cpp
   mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
   mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
   mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
   delay(100); // Give it time to wake up
   ```
2. Power cycle the MPU6050
3. Try a different MPU6050 module (might be faulty)

#### Issue D: MPU6050 Reading But Roll Stays 0.00°
```
[MPU6050] Raw: X:0.12 Y:0.00 Z:9.78
Roll: 0.00° (Limits: -47° to 40°)
```

**Solution:**
This means Y-axis is stuck at 0. The calculation is:
```cpp
roll = atan2(Y, Z) * 180 / PI
```

If Y=0, then roll will always be 0° or 180°.

**Fixes:**
1. Check if MPU6050 is mounted correctly
2. Try reading gyroscope data instead:
   ```cpp
   float rollRate = gyro.gyro.x * 180.0 / PI;
   ```
3. Calibrate the MPU6050
4. Replace the MPU6050 module

### Step 3: Hardware Verification

#### Test 1: Visual Inspection
- [ ] MPU6050 VCC connected to ESP32 3.3V
- [ ] MPU6050 GND connected to ESP32 GND
- [ ] MPU6050 SDA connected to ESP32 GPIO 21
- [ ] MPU6050 SCL connected to ESP32 GPIO 22
- [ ] All connections are secure (not loose)
- [ ] No visible damage on MPU6050 module

#### Test 2: Voltage Check
Use a multimeter to check:
- [ ] VCC pin on MPU6050 = 3.3V
- [ ] GND pin on MPU6050 = 0V
- [ ] SDA and SCL pins = 3.3V (when idle, due to pull-ups)

#### Test 3: Continuity Check
Use multimeter in continuity mode:
- [ ] ESP32 GPIO 21 → MPU6050 SDA (should beep)
- [ ] ESP32 GPIO 22 → MPU6050 SCL (should beep)
- [ ] ESP32 3.3V → MPU6050 VCC (should beep)
- [ ] ESP32 GND → MPU6050 GND (should beep)

### Step 4: Software Verification

#### Test 1: I2C Scanner
The updated code includes an I2C scanner. Check output:
```
Scanning I2C bus (addresses 0x01 to 0x7F)...
✓ I2C device found at address 0x68
  → This is likely the MPU6050!
✓ Found 1 I2C device(s)
```

#### Test 2: Raw Data Test
Look for this in Serial Monitor every 2 seconds:
```
========== MPU6050 STATUS ==========
Raw Accel: X=0.12 Y=-0.34 Z=9.78 m/s²
Total Accel: 9.81 m/s² (Threshold: 15.00)
Roll: -1.99° (Limits: -47° to 40°)
Pitch: 0.70°
Temperature: 25.3°C
✓ MPU6050 is working normally
====================================
```

**What to look for:**
- Total Accel should be around 9.8 m/s² (gravity)
- Roll should change when you tilt the board
- Pitch should change when you tilt the board
- Temperature should be reasonable (20-40°C)

#### Test 3: Tilt Test
1. Keep board flat → Roll should be near 0°
2. Tilt left → Roll should go negative (-10° to -90°)
3. Tilt right → Roll should go positive (10° to 90°)
4. Tilt forward → Pitch should go negative
5. Tilt backward → Pitch should go positive

### Step 5: Common Issues & Solutions

#### Issue: "All readings are 0.00"
**Cause:** MPU6050 not powered or not communicating
**Solution:**
1. Check power connections (VCC and GND)
2. Check I2C connections (SDA and SCL)
3. Try different GPIO pins
4. Add pull-up resistors (4.7kΩ)

#### Issue: "Readings don't change when tilting"
**Cause:** MPU6050 in wrong mode or faulty
**Solution:**
1. Reset MPU6050 by power cycling
2. Check if accelerometer range is set correctly
3. Try different MPU6050 module
4. Check for mechanical damage

#### Issue: "Roll stuck at 0° or 180°"
**Cause:** Y-axis not working
**Solution:**
1. This is likely a hardware fault
2. Try different MPU6050 module
3. Use gyroscope instead of accelerometer

#### Issue: "Crash detection triggers randomly"
**Cause:** Threshold too low or vibrations
**Solution:**
1. Increase threshold:
   ```cpp
   const float ACCEL_THRESHOLD = 20.0; // Instead of 15.0
   ```
2. Add filtering:
   ```cpp
   mpu.setFilterBandwidth(MPU6050_BAND_5_HZ); // More filtering
   ```
3. Add debounce logic

#### Issue: "Crash detection never triggers"
**Cause:** Threshold too high
**Solution:**
1. Lower threshold:
   ```cpp
   const float ACCEL_THRESHOLD = 12.0; // Instead of 15.0
   ```
2. Adjust roll limits:
   ```cpp
   if (roll < -30 || roll > 30) // Instead of -47 and 40
   ```

## Quick Test Code

If you want to test MPU6050 independently, use this minimal code:

```cpp
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050!");
    while (1) delay(10);
  }
  
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  float roll = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI;
  
  Serial.printf("X:%.2f Y:%.2f Z:%.2f | Roll:%.2f°\n",
    a.acceleration.x, a.acceleration.y, a.acceleration.z, roll);
  
  delay(500);
}
```

Upload this and check if you see changing values when tilting.

## Expected Output (Working MPU6050)

### When Flat:
```
X:0.12 Y:-0.34 Z:9.78 | Roll:-1.99°
```

### When Tilted Left:
```
X:0.15 Y:-5.23 Z:8.12 | Roll:-32.75°
```

### When Tilted Right:
```
X:0.18 Y:4.89 Z:8.45 | Roll:30.12°
```

### When Upside Down:
```
X:-0.21 Y:0.45 Z:-9.65 | Roll:177.32°
```

## Wiring Diagram

```
ESP32          MPU6050
-----          -------
3.3V    →      VCC
GND     →      GND
GPIO21  →      SDA
GPIO22  →      SCL
        →      XDA (leave unconnected)
        →      XCL (leave unconnected)
        →      AD0 (connect to GND for address 0x68)
        →      INT (leave unconnected)
```

## Alternative: Use Different Pins

If GPIO 21/22 don't work, try different pins:

```cpp
Wire.begin(23, 19); // SDA=23, SCL=19
```

Available I2C pins on ESP32:
- Any GPIO can be used for I2C
- Recommended: GPIO 21 (SDA), GPIO 22 (SCL)
- Alternative: GPIO 23 (SDA), GPIO 19 (SCL)
- Alternative: GPIO 25 (SDA), GPIO 26 (SCL)

## Still Not Working?

If none of the above works:
1. **Try a different MPU6050 module** - Your current one might be faulty
2. **Try a different ESP32 board** - I2C pins might be damaged
3. **Use an oscilloscope** - Check if SDA/SCL lines have activity
4. **Check for shorts** - Use multimeter to check for shorts between pins
5. **Update Adafruit library** - Make sure you have the latest version

## Summary

The updated code now includes:
- ✅ I2C bus scanner
- ✅ MPU6050 initialization retry with detailed errors
- ✅ Multiple test readings on startup
- ✅ Continuous health monitoring
- ✅ Detailed debug output every 2 seconds
- ✅ Warning messages for common issues

Upload the updated code and check Serial Monitor for diagnostic information!
