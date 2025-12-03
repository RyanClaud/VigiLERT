# VigiLERT Arduino Code

This folder contains the final production Arduino code for the VigiLERT motorcycle safety system.

## 🚀 Production Code (Ready to Upload)

### `HelmetToWebApplication_FINAL.ino`
- **Latest helmet module code**
- Features: Alcohol detection, heartbeat monitoring, WiFi connectivity
- Real-time sensor data transmission to Firebase
- 1-second heartbeat interval for reliable connection tracking

### `MotorcycleToWebApplication_FINAL.ino`
- **Latest motorcycle module code with comprehensive security**
- Features: 
  - 🔒 5-second timeout protection (WiFi & helmet)
  - 🛡️ Anti-theft system with vibration sensor
  - 🗺️ Automatic trip tracking with GPS
  - 🚨 Crash detection with MPU6050
  - 🔑 Physical key security with device verification
  - 📱 Dashboard integration with real-time updates

## 🔧 Diagnostic & Testing Tools

### `GPIO_SCANNER_TEST.ino`
- Utility to scan and test GPIO pins
- Useful for hardware troubleshooting

### `RELAY_DIAGNOSTIC_TEST.ino`
- Relay testing and diagnostic tool
- Helps verify relay wiring and functionality

### `FIREBASE_PATH_TESTER.ino`
- Firebase connection and path testing
- Useful for debugging Firebase communication issues

## 📋 Upload Instructions

1. **For Helmet Module**: Upload `HelmetToWebApplication_FINAL.ino`
2. **For Motorcycle Module**: Upload `MotorcycleToWebApplication_FINAL.ino`
3. **Configure WiFi**: Update `ssid` and `password` in both files
4. **Configure Firebase**: Update `userUID` and Firebase settings

## 🔒 Security Features

The final motorcycle code includes:
- **5-second WiFi timeout** - Engine shuts off if WiFi disconnects >5s
- **5-second helmet timeout** - Engine shuts off if helmet disconnects >5s
- **Physical key protection** - Key disabled during security violations
- **Comprehensive logging** - All security events logged to Firebase
- **Real-time monitoring** - Continuous security status checking

## 🛠️ Hardware Requirements

### Helmet Module:
- ESP32 Development Board
- MQ-3 Alcohol Sensor (GPIO 34)
- LED Indicator (GPIO 2)
- Buzzer (GPIO 5)

### Motorcycle Module:
- ESP32 Development Board
- MPU6050 Accelerometer/Gyroscope
- GPS Module (NEO-6M or similar)
- Relay Module (5V with external power)
- Vibration Sensor (GPIO 15)
- Physical Key Switch (GPIO 14)
- Buzzer and LED indicators

## 📞 Support

For technical support and documentation, check the `/docs` folder in the project root.