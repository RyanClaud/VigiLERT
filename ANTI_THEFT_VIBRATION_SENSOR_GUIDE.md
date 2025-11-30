# Anti-Theft System with Vibration Sensor - Implementation Guide

## Overview
This system adds anti-theft protection to your motorcycle using a dedicated vibration sensor module and GSM SMS alerts. When the engine is off, the system monitors for unauthorized movement and sends SMS alerts to the owner.

## Hardware Requirements

### Components Needed
1. **ESP32** - Main controller
2. **Vibration Sensor Module** (SW-420 or similar)
   - Digital output (HIGH when vibration detected)
   - Adjustable sensitivity via potentiometer
3. **GSM Module** (SIM800L, SIM900, or similar)
   - For sending SMS alerts
   - Requires active SIM card
4. **MPU6050** - For crash detection (already installed)
5. **GPS Module** - For location tracking (already installed)
6. **Relay Module** - Engine control (already installed)
7. **Buzzer & LED** - Alerts (already installed)

### Pin Connections

```
ESP32 Pin Assignments:
├── GPIO 13 - Relay (Engine Control)
├── GPIO 12 - Buzzer
├── GPIO 2  - LED Indicator
├── GPIO 15 - Vibration Sensor (NEW)
├── GPIO 4  - GSM TX
├── GPIO 5  - GSM RX
├── GPIO 16 - GPS RX
├── GPIO 17 - GPS TX
├── GPIO 21 - I2C SDA (MPU6050)
└── GPIO 22 - I2C SCL (MPU6050)
```

### Vibration Sensor Wiring
```
Vibration Sensor Module:
├── VCC → 3.3V or 5V (check module specs)
├── GND → GND
└── DO (Digital Out) → GPIO 15
```

**Note**: Most vibration sensors have a potentiometer to adjust sensitivity. Start with medium sensitivity and adjust based on testing.

## How It Works

### System States

#### 1. Engine Running
- Anti-theft: **DISARMED**
- Vibration sensor: **INACTIVE**
- MPU6050: **ACTIVE** (crash detection)
- Behavior: Normal operation

#### 2. Engine Stopped (0-30 seconds)
- Anti-theft: **ARMING**
- Vibration sensor: **INACTIVE**
- Status: 30-second grace period
- Behavior: Countdown to arm

#### 3. Engine Stopped (30+ seconds)
- Anti-theft: **ARMED**
- Vibration sensor: **ACTIVE**
- Status: Monitoring for theft
- Behavior: Detects unauthorized movement

### Detection Logic

```
Vibration Detected → Count +1
│
├─ Count < 3 → Continue monitoring
│
└─ Count ≥ 3 → TRIGGER ALERT
   │
   ├─ Sound alarm (buzzer + LED)
   ├─ Send SMS to owner
   ├─ Log to Firebase
   └─ Reset count, continue monitoring
```

### False Alarm Prevention
1. **30-second arming delay** - Time to walk away after parking
2. **3 detection requirement** - Must detect 3 vibrations before alerting
3. **500ms debounce** - Prevents multiple triggers from single event
4. **10-second decay** - Count decreases if no vibration
5. **5-minute cooldown** - Between SMS alerts (prevents spam)

## SMS Alert Format

When theft is detected, owner receives:
```
VIGILERT THEFT ALERT!
Unauthorized movement detected!
https://maps.google.com/?q=14.123456,121.123456
Check your motorcycle NOW!
```

## Configuration

### 1. Set Owner Phone Number
```cpp
const String ownerPhoneNumber = "+639675715673";  // Your number
```

### 2. Adjust Timing (Optional)
```cpp
const unsigned long ARM_DELAY = 30000;  // 30 seconds to arm
const int THEFT_DETECTION_REQUIRED = 3;  // Detections needed
const unsigned long VIBRATION_DEBOUNCE = 500;  // 500ms debounce
const unsigned long THEFT_ALERT_COOLDOWN = 300000;  // 5 min between SMS
```

### 3. Change Vibration Sensor Pin (If Needed)
```cpp
const int vibrationSensorPin = 15;  // Change to your pin
```

## Testing Procedure

### 1. Test Vibration Sensor
```
Serial Command: TEST VIBRATION
```
- Shake the motorcycle
- Watch Serial Monitor for readings
- Adjust sensor sensitivity if needed

### 2. Test SMS Sending
```
Serial Command: TEST SMS
```
- Sends test SMS to owner number
- Verify SMS received
- Check GSM signal strength

### 3. Test Anti-Theft System
```
Steps:
1. Serial Command: STOP ENGINE
2. Wait 30 seconds (or use: ARM THEFT)
3. Shake motorcycle
4. Watch for alarm and SMS
```

### 4. Test Full Cycle
```
1. Start engine (START ENGINE)
2. Stop engine (STOP ENGINE)
3. Wait 30 seconds
4. Shake motorcycle
5. Verify SMS received
6. Start engine to disarm
```

## Serial Commands

### Engine Control
- `START ENGINE` - Start engine
- `STOP ENGINE` - Stop engine

### Anti-Theft Control
- `ARM THEFT` - Manually arm (skip 30s delay)
- `DISARM THEFT` - Manually disarm
- `TEST THEFT` - Trigger theft alert
- `TEST VIBRATION` - Test vibration sensor

### Testing
- `TEST SMS` - Send test SMS
- `STATUS` - Show system status
- `CRASH` - Test crash detection
- `ALCOHOL` - Test alcohol detection

### Auto Control
- `AUTO ON` - Enable automatic engine control
- `AUTO OFF` - Disable automatic control

## Troubleshooting

### Vibration Sensor Not Working
**Problem**: No detection when shaking
**Solutions**:
1. Check wiring (VCC, GND, DO)
2. Adjust sensitivity potentiometer
3. Test with `TEST VIBRATION` command
4. Verify pin number in code

### False Alarms
**Problem**: Alerts triggered by wind/rain
**Solutions**:
1. Reduce sensor sensitivity
2. Increase `THEFT_DETECTION_REQUIRED` to 4 or 5
3. Increase `VIBRATION_DEBOUNCE` to 1000ms
4. Mount sensor away from wind exposure

### SMS Not Sending
**Problem**: No SMS received
**Solutions**:
1. Check SIM card inserted and activated
2. Verify GSM module power (needs 2A)
3. Check phone number format (+639...)
4. Test GSM with `TEST SMS` command
5. Check GSM signal strength

### System Arms Too Quickly
**Problem**: Not enough time to walk away
**Solutions**:
1. Increase `ARM_DELAY` to 60000 (60 seconds)
2. Use `DISARM THEFT` command before parking

### Too Many SMS Alerts
**Problem**: Multiple SMS for same event
**Solutions**:
1. Increase `THEFT_ALERT_COOLDOWN` to 600000 (10 min)
2. Increase `THEFT_DETECTION_REQUIRED`
3. Check sensor sensitivity

## Vibration Sensor Sensitivity Adjustment

### SW-420 Module (Common Type)
1. Locate potentiometer on module
2. Turn clockwise = MORE sensitive
3. Turn counter-clockwise = LESS sensitive

### Recommended Settings
- **High Sensitivity**: Detects light touch (may cause false alarms)
- **Medium Sensitivity**: Detects shaking/moving (recommended)
- **Low Sensitivity**: Only detects strong impacts (may miss theft)

### Testing Sensitivity
```
1. Upload code
2. Run: TEST VIBRATION
3. Try different levels of shaking:
   - Light tap
   - Moderate shake
   - Strong shake
4. Adjust potentiometer until:
   - Detects moderate shake
   - Ignores light vibrations
```

## Power Considerations

### GSM Module Power
- GSM modules need **2A** peak current
- Use separate power supply or large capacitor
- ESP32 USB power may not be enough

### Battery Backup (Optional)
- Add battery backup for anti-theft
- System continues monitoring even if main power cut
- Recommended: 18650 Li-ion battery + charging circuit

## Firebase Integration

### Data Logged
```json
{
  "theft_alerts": {
    "timestamp": 1234567890,
    "type": "theft_attempt",
    "location": "https://maps.google.com/?q=14.123,121.123",
    "vibrationDetected": true,
    "armed": true
  }
}
```

### Dashboard Display
- Anti-theft status shown in live data
- Theft alerts logged to Firebase
- Can view theft history on dashboard

## Best Practices

### 1. Parking Procedure
```
1. Stop engine via dashboard
2. Walk away within 30 seconds
3. System arms automatically
4. Receive confirmation beep (2 short beeps)
```

### 2. Starting After Parking
```
1. Approach motorcycle
2. Start engine via dashboard
3. System disarms automatically
4. No SMS alerts triggered
```

### 3. Maintenance
- Check GSM signal monthly
- Test SMS alerts monthly
- Verify vibration sensor sensitivity
- Keep SIM card credit topped up

### 4. Security Tips
- Park in well-lit areas
- Use additional physical locks
- Don't rely solely on electronic security
- Check theft alerts immediately

## Cost Estimate

| Component | Price (PHP) |
|-----------|-------------|
| Vibration Sensor (SW-420) | ₱50-100 |
| GSM Module (SIM800L) | ₱200-400 |
| SIM Card + Load | ₱50-100 |
| Wires & Connectors | ₱50 |
| **Total** | **₱350-650** |

## Advantages Over MPU6050-Only

| Feature | MPU6050 Only | Vibration Sensor |
|---------|--------------|------------------|
| Sensitivity | Fixed | Adjustable |
| False Alarms | Higher | Lower |
| Power Usage | Higher | Lower |
| Dedicated Purpose | No | Yes |
| Easy Tuning | No | Yes (potentiometer) |

## Future Enhancements

### Possible Additions
1. **Remote Disarm** - SMS command to disarm
2. **Geofencing** - Alert if moved outside area
3. **Photo Capture** - Camera module for evidence
4. **Siren** - Louder alarm than buzzer
5. **Kill Switch** - Prevent engine start remotely

---

**File**: `MotorcycleToWebApplication_ANTI_THEFT_V2.ino`
**Version**: 2.0 with Vibration Sensor
**Date**: November 29, 2025
**Status**: ✅ Ready for Testing
