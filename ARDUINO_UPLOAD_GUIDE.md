# 🚀 Arduino Upload Guide - Complete Version

## ✅ File Ready to Upload

**File**: `MotorcycleToWebApplication_COMPLETE.ino`

This file now contains:
- ✅ All original functions
- ✅ Enhanced sensor data transmission
- ✅ MPU6050 real-time data
- ✅ GPS quality metrics
- ✅ GSM signal strength
- ✅ Engine status
- ✅ Helmet switch state
- ✅ Complete and compilable

---

## 📋 Pre-Upload Checklist

### 1. Required Libraries
Make sure these libraries are installed in Arduino IDE:

```
✅ WiFi (built-in)
✅ HTTPClient (built-in)
✅ TinyGPSPlus
✅ HardwareSerial (built-in)
✅ Wire (built-in)
✅ Adafruit_MPU6050
✅ Adafruit_Sensor
✅ ArduinoOTA (built-in)
✅ ArduinoJson (v6.x)
✅ TimeLib
```

### 2. Board Configuration
```
Board: ESP32 Dev Module
Upload Speed: 921600
CPU Frequency: 240MHz
Flash Frequency: 80MHz
Flash Mode: QIO
Flash Size: 4MB
Partition Scheme: Default 4MB with spiffs
```

### 3. Update Your Credentials
Open the file and update these lines:

```cpp
// Line 18-19: WiFi Credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Line 21-23: Firebase Config
const String firebaseHost = "https://YOUR-PROJECT.firebaseio.com";
const String userUID = "YOUR_USER_UID";
const String firebaseAuth = ""; // Optional

// Line 35-37: Authorized Phone Numbers
const String authorizedNumber1 = "+639XXXXXXXXX";
const String authorizedNumber2 = "+639XXXXXXXXX";
```

---

## 🔧 Upload Steps

### Step 1: Open Arduino IDE
1. Launch Arduino IDE
2. Go to **File → Open**
3. Select `MotorcycleToWebApplication_COMPLETE.ino`

### Step 2: Select Board & Port
1. **Tools → Board → ESP32 Arduino → ESP32 Dev Module**
2. **Tools → Port → COM X** (your ESP32 port)

### Step 3: Verify Code
1. Click **Verify** button (✓) or press **Ctrl+R**
2. Wait for compilation
3. Check for errors

**Expected Output:**
```
Sketch uses XXXXX bytes (XX%) of program storage space.
Global variables use XXXXX bytes (XX%) of dynamic memory.
```

### Step 4: Upload
1. Click **Upload** button (→) or press **Ctrl+U**
2. Wait for upload to complete
3. Watch for "Done uploading" message

**Expected Output:**
```
Connecting........_____....._____
Writing at 0x00001000... (X %)
...
Hash of data verified.
Leaving...
Hard resetting via RTS pin...
```

---

## 📊 Serial Monitor Testing

### Step 1: Open Serial Monitor
1. **Tools → Serial Monitor** or press **Ctrl+Shift+M**
2. Set baud rate to **115200**
3. Set line ending to **Both NL & CR**

### Step 2: Expected Output
```
[SETUP] Starting VigiLERT System - COMPLETE VERSION...
[SUCCESS] MPU6050 initialized.
[SETUP] GPS Serial started.
[SETUP] GSM Serial started.
Connecting WiFi.........
Connected IP: 192.168.1.XXX
[FIREBASE] Motorcycle status: On (HTTP: 200)
[SUCCESS] Setup complete. All sensors active.

[SENSOR] Accel: 9.81 | Roll: 0.5° | Battery: 12.4V | Helmet: OFF
         GPS: N/A | Speed: 0.0 km/h | GSM: 0%
         Engine: STOPPED
[FIREBASE] Live update: HTTP 200
```

### Step 3: Verify Data Flow
Watch for these messages every 200ms:
```
[SENSOR] Accel: X.XX | Roll: X.X° | Battery: XX.XV | Helmet: ON/OFF
         GPS: OK/N/A | Speed: XX.X km/h | GSM: XX%
         Engine: RUNNING/STOPPED
```

---

## 🐛 Troubleshooting

### Error: "Library not found"
**Solution**: Install missing library
1. **Sketch → Include Library → Manage Libraries**
2. Search for library name
3. Click **Install**

### Error: "Port not found"
**Solution**: Install CH340/CP2102 driver
1. Download driver for your OS
2. Install driver
3. Restart Arduino IDE
4. Reconnect ESP32

### Error: "Compilation error"
**Solution**: Check library versions
1. Ensure ArduinoJson is v6.x (not v5.x)
2. Update all libraries to latest
3. Restart Arduino IDE

### Error: "Upload failed"
**Solution**: Put ESP32 in boot mode
1. Hold **BOOT** button
2. Press **RESET** button
3. Release **RESET**
4. Release **BOOT**
5. Try upload again

### Error: "WiFi not connecting"
**Solution**: Check credentials
1. Verify SSID and password
2. Check WiFi is 2.4GHz (not 5GHz)
3. Move closer to router
4. Check router settings

### Error: "Firebase HTTP 401"
**Solution**: Check Firebase rules
1. Open Firebase Console
2. Go to Realtime Database → Rules
3. Set to:
```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

---

## ✅ Verification Checklist

After upload, verify:

- [ ] Serial monitor shows startup messages
- [ ] WiFi connects successfully
- [ ] MPU6050 initializes
- [ ] GPS serial starts
- [ ] GSM serial starts
- [ ] Firebase updates (HTTP 200)
- [ ] Sensor readings appear every 200ms
- [ ] No error messages

---

## 📊 Expected Firebase Data

After successful upload, check Firebase Console:

### `/helmet_public/{userUID}/live`
```json
{
  "locationLat": 14.5995,
  "locationLng": 120.9842,
  "speed": 0,
  "batteryVoltage": 12.4,
  "headlight": false,
  "taillight": false,
  "leftSignal": false,
  "rightSignal": false,
  "brakeLight": false,
  "mpu6050": {
    "accelX": 0.12,
    "accelY": -0.05,
    "accelZ": 9.81,
    "gyro": 0.5,
    "totalAccel": 9.82
  },
  "gps": {
    "accuracy": "5m",
    "satellites": 8
  },
  "gsm": {
    "signal": "85%",
    "network": "4G"
  },
  "engineRunning": false,
  "helmetSwitch": false,
  "timestamp": 1234567890,
  "source": "gps_module"
}
```

### `/helmet_public/{userUID}/devices/motorcycle`
```json
{
  "status": "On",
  "lastHeartbeat": 1700000000000,
  "timestamp": 1700000000000
}
```

---

## 🎯 Next Steps

After successful upload:

1. **Test GPS**: Move to open area, wait for GPS lock
2. **Test GSM**: Check signal strength in serial monitor
3. **Test Sensors**: Move device, check MPU6050 readings
4. **Test Dashboard**: Refresh web dashboard, verify all data appears
5. **Test Engine**: Send SMS command "1234 START"
6. **Test Crash**: Tilt device sharply, check crash detection

---

## 📞 Support

If you encounter issues:

1. **Check Serial Monitor** for error messages
2. **Check Firebase Console** for data
3. **Check Dashboard Console** (F12) for errors
4. **Verify all connections** (GPS, GSM, MPU6050)
5. **Check power supply** (stable 5V)

---

## 🎉 Success Indicators

You'll know it's working when:

✅ Serial monitor shows sensor readings every 200ms  
✅ Firebase shows live data updating  
✅ Dashboard displays all sensor data  
✅ GPS shows location (after lock)  
✅ GSM shows signal strength  
✅ MPU6050 shows acceleration data  
✅ No error messages in serial monitor  

**Your system is now fully operational!** 🚀
