# VigiLERT Diagnostic Mode - Complete Guide

## Overview
The diagnostic version provides comprehensive real-time monitoring of all sensors and modules through the Serial Monitor. Perfect for troubleshooting and verifying hardware functionality.

## File
`MotorcycleToWebApplication_DIAGNOSTIC_FULL.ino`

## Features

### ✅ Automatic Status Display
- Updates every 3 seconds
- Shows all module status
- Real-time sensor readings
- GPS data with satellite count
- MPU6050 accelerometer/gyroscope data
- System status overview

### ✅ Module Detection
- WiFi connection status
- GSM module ready state
- GPS fix status
- MPU6050 sensor detection
- Automatic troubleshooting hints

### ✅ Serial Commands
All commands for testing and control

## Serial Monitor Output Example

```
╔═══════════════════════════════════════════════════════════════════════════╗
║                        SYSTEM DIAGNOSTICS REPORT                          ║
╠═══════════════════════════════════════════════════════════════════════════╣
║ 📡 MODULE STATUS                                                          ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  WiFi:      ✅  IP: 192.168.1.100 | Signal: -45 dBm                       ║
║  GSM:       ✅  Module responding | SMS ready                             ║
║  GPS:       ✅  Satellites: 8 | Fix acquired                              ║
║  MPU6050:   ✅  Crash detection active                                    ║
╠═══════════════════════════════════════════════════════════════════════════╣
║ 🛰️  GPS DATA                                                               ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Latitude:     14.123456°                                                 ║
║  Longitude:   121.123456°                                                 ║
║  Altitude:      125.50 m                                                  ║
║  Speed:          45.20 km/h                                               ║
║  Satellites: 8                                                            ║
╠═══════════════════════════════════════════════════════════════════════════╣
║ 📊 MOTION SENSOR (MPU6050)                                                ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Accel X:        0.12 m/s²                                                ║
║  Accel Y:        0.05 m/s²                                                ║
║  Accel Z:        9.81 m/s²                                                ║
║  Total:          9.82 m/s² (Threshold: 15.0)                              ║
║  Roll:           0.58°                                                    ║
║  Gyro X:         0.02 °/s                                                 ║
║  Gyro Y:        -0.01 °/s                                                 ║
║  Gyro Z:         0.00 °/s                                                 ║
║  Temp:          28.50 °C                                                  ║
╠═══════════════════════════════════════════════════════════════════════════╣
║ 🔌 DIGITAL SENSORS                                                        ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Vibration:  None      (GPIO 15: 0)                                       ║
║  Relay:      OFF  (GPIO 13: 1) - Engine STOPPED                           ║
║  Buzzer:     OFF  (GPIO 12: 0)                                            ║
║  LED:        OFF  (GPIO 2: 0)                                             ║
╠═══════════════════════════════════════════════════════════════════════════╣
║ ⚙️  SYSTEM STATUS                                                          ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Engine:         STOPPED 🛑                                               ║
║  Crash:          None ✅                                                   ║
║  Alcohol:        Safe ✅                                                   ║
║  Anti-Theft:     ARMED 🛡️                                                 ║
║  Auto Control:   Disabled                                                 ║
║  Uptime:         1234 seconds                                             ║
╚═══════════════════════════════════════════════════════════════════════════╝
```

## Serial Commands

### Basic Control
| Command | Description |
|---------|-------------|
| `START ENGINE` | Start engine (relay ON) |
| `STOP ENGINE` | Stop engine (relay OFF) |
| `STATUS` | Show full system diagnostics |

### Module Details
| Command | Description |
|---------|-------------|
| `GPS` | Show detailed GPS information |
| `GSM` | Show detailed GSM information |
| `MPU` | Show detailed MPU6050 information |
| `SENSORS` | Show all sensor readings |

### Testing
| Command | Description |
|---------|-------------|
| `TEST VIBRATION` | Test vibration sensor (10 seconds) |
| `TEST SMS` | Send test SMS to owner |
| `TEST RELAY` | Test relay on/off (5 cycles) |

### Anti-Theft
| Command | Description |
|---------|-------------|
| `ARM THEFT` | Manually arm anti-theft |
| `DISARM THEFT` | Disarm anti-theft |

### Simulation
| Command | Description |
|---------|-------------|
| `CRASH` | Simulate crash detection |

## Troubleshooting Guide

### WiFi Not Connecting
**Symptoms**: `WiFi: ❌ FAILED`

**Check**:
1. SSID and password correct
2. Router in range
3. 2.4GHz network (ESP32 doesn't support 5GHz)
4. Check Serial Monitor for error messages

### GSM Not Ready
**Symptoms**: `GSM: ❌ NOT READY`

**Check**:
1. Power supply (GSM needs 2A peak)
2. SIM card inserted correctly
3. Antenna connected
4. Wiring: RX:4, TX:5
5. Try command: `GSM` for details

**Test Manually**:
```
1. Open Serial Monitor
2. Type: AT
3. Should respond: OK
```

### GPS No Fix
**Symptoms**: `GPS: ❌ NO FIX`

**Check**:
1. Move to open area (outdoors)
2. Clear sky view
3. Wait 30-60 seconds for initial fix
4. Antenna connected
5. Wiring: RX:16, TX:17

**Monitor Progress**:
```
Type: GPS
Check: Satellites count (need 4+ for fix)
```

### MPU6050 Not Detected
**Symptoms**: `MPU6050: ❌ FAILED`

**Check**:
1. I2C connections (SDA:21, SCL:22)
2. Power supply (3.3V or 5V)
3. I2C address (default: 0x68)
4. Try I2C scanner sketch

**Test**:
```
Type: MPU
Should show accelerometer readings
```

### Vibration Sensor Not Working
**Symptoms**: Always shows 0 or always shows 1

**Check**:
1. Wiring: VCC, GND, DO to GPIO 15
2. Adjust sensitivity potentiometer
3. Test with: `TEST VIBRATION`
4. Shake motorcycle while testing

### Relay Not Clicking
**Symptoms**: No sound when starting/stopping engine

**Check**:
1. Relay wiring to GPIO 13
2. Relay power supply
3. Test with: `TEST RELAY`
4. Listen for 5 clicks
5. Check relay LED indicator

## Interpreting Sensor Values

### MPU6050 Accelerometer
- **Normal (stationary)**: X≈0, Y≈0, Z≈9.8 m/s²
- **Crash threshold**: Total > 15.0 m/s²
- **Lean angle**: Calculated from X, Y, Z

### MPU6050 Gyroscope
- **Normal (stationary)**: All values near 0 °/s
- **Moving**: Values change with rotation
- **Range**: ±2000 °/s

### GPS
- **Satellites needed**: 4+ for 2D fix, 6+ for 3D fix
- **Good signal**: 8+ satellites
- **HDOP**: Lower is better (< 2.0 is good)

### Vibration Sensor
- **0**: No vibration
- **1**: Vibration detected
- **Sensitivity**: Adjust potentiometer on module

## Performance Monitoring

### Update Intervals
- **Diagnostic Display**: Every 3 seconds
- **Heartbeat**: Every 2 seconds
- **Firebase Updates**: Every 2 seconds
- **Alcohol Check**: Every 500ms
- **Dashboard Button**: Every 1 second

### Memory Usage
Monitor in Serial output:
- Free heap
- Stack usage
- WiFi buffer

## Best Practices

### 1. Initial Setup
```
1. Upload diagnostic code
2. Open Serial Monitor (115200 baud)
3. Wait for all modules to initialize
4. Check module status (all should be ✅)
5. Test each module individually
```

### 2. GPS Testing
```
1. Go outdoors
2. Wait 60 seconds
3. Type: GPS
4. Check satellite count
5. Verify coordinates on map
```

### 3. GSM Testing
```
1. Type: GSM
2. Check signal strength
3. Type: TEST SMS
4. Verify SMS received
```

### 4. Sensor Testing
```
1. Type: SENSORS
2. Verify all readings
3. Type: TEST VIBRATION
4. Shake motorcycle
5. Check buzzer beeps
```

### 5. Relay Testing
```
1. Type: TEST RELAY
2. Listen for 5 clicks
3. Type: START ENGINE
4. Verify relay clicks
5. Type: STOP ENGINE
```

## Common Issues & Solutions

### Issue: Constant Reboots
**Solution**: Check power supply, add capacitor to GSM

### Issue: GPS Takes Forever
**Solution**: Move outdoors, check antenna, wait longer

### Issue: GSM No Signal
**Solution**: Check SIM card, antenna, operator coverage

### Issue: MPU6050 Wrong Values
**Solution**: Calibrate sensor, check mounting orientation

### Issue: Vibration Too Sensitive
**Solution**: Adjust potentiometer clockwise (less sensitive)

### Issue: Relay Always On/Off
**Solution**: Check wiring, verify ACTIVE-LOW logic

## Serial Monitor Settings

**Baud Rate**: 115200
**Line Ending**: Newline or Both NL & CR
**Timestamp**: Optional (helpful for debugging)

## Data Logging

To log diagnostic data:
1. Enable timestamps in Serial Monitor
2. Copy output to text file
3. Analyze patterns
4. Share with support if needed

## Switching to Production Mode

Once all modules verified:
1. Upload production code
2. Remove diagnostic Serial.print statements
3. Keep essential error logging
4. Monitor via dashboard instead

---

**File**: `MotorcycleToWebApplication_DIAGNOSTIC_FULL.ino`
**Purpose**: Hardware verification and troubleshooting
**Baud Rate**: 115200
**Update Interval**: 3 seconds
**Status**: ✅ Ready for testing
