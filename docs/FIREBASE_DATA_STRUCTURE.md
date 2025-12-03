# VIGILERT Firebase Data Structure

This document outlines the expected Firebase Realtime Database structure for the VIGILERT system.

## Database Path: `helmet_public/{userId}/`

### 1. Device Pairing Status
```json
{
  "devices": {
    "helmet": {
      "status": "On",  // "On" | "Off" | "connected" | "disconnected"
      "lastSeen": 1234567890
    },
    "motorcycle": {
      "status": "On",  // "On" | "Off" | "connected" | "disconnected"
      "lastSeen": 1234567890
    }
  }
}
```

### 2. Device Health Monitoring
```json
{
  "deviceHealth": {
    "battery": 85,        // Battery percentage (0-100)
    "gsm": true,          // GSM connection status
    "gps": true,          // GPS connection status
    "lastUpdate": 1234567890
  }
}
```

### 3. Electrical Diagnostics
```json
{
  "electrical": {
    "headlight": true,      // true = OK, false = FAULT
    "taillight": true,
    "brakeLight": true,
    "signalLights": true,
    "lastCheck": 1234567890
  }
}
```

### 4. Live Data (Existing)
```json
{
  "live": {
    "speed": 45.5,
    "locationLat": 14.5995,
    "locationLng": 120.9842,
    "timestamp": 1234567890
  }
}
```

### 5. Helmet Status (Existing)
```json
{
  "helmetStatus": {
    "status": {
      "helmetConnected": true,
      "alertnessStatus": "Normal",  // "Normal" | "Drowsy"
      "alcoholLevel": 0.0
    }
  }
}
```

### 6. Alcohol Detection (Existing)
```json
{
  "alcohol": {
    "status": {
      "status": "Safe",      // "Safe" | "Danger"
      "sensorValue": 0,
      "timestamp": 1234567890
    }
  }
}
```

### 7. Speed Settings (Existing)
```json
{
  "settings": {
    "speedLimit": 80
  }
}
```

### 8. Trips (Existing)
```json
{
  "trips": {
    "trip_id_1": {
      "startLat": 14.5995,
      "startLng": 120.9842,
      "endLat": 14.6000,
      "endLng": 120.9850,
      "startLocationName": "Location A",
      "endLocationName": "Location B",
      "maxSpeed": 65,
      "distance": 5.2,
      "startTime": 1234567890,
      "endTime": 1234567900
    }
  }
}
```

### 9. Crashes (Existing)
```json
{
  "crashes": {
    "crash_id_1": {
      "timestamp": 1234567890,
      "roll": -45,
      "pitch": 30,
      "lat": 14.5995,
      "lng": 120.9842,
      "hasGPS": true,
      "impactStrength": "High"
    }
  }
}
```

### 10. SOS Alerts (New)
```json
{
  "sos": {
    "sos_timestamp": {
      "timestamp": 1234567890,
      "location": {
        "lat": 14.5995,
        "lng": 120.9842
      },
      "triggeredBy": "manual",  // "manual" | "automatic"
      "status": "active"        // "active" | "resolved"
    }
  }
}
```

## Testing Data

To test the dashboard features, you can manually add this data to Firebase:

```javascript
// Example: Set device pairing status
firebase.database().ref('helmet_public/MnzBjTBslZNijOkq732PE91hHa23/devices').set({
  helmet: { status: "On", lastSeen: Date.now() },
  motorcycle: { status: "On", lastSeen: Date.now() }
});

// Example: Set device health
firebase.database().ref('helmet_public/MnzBjTBslZNijOkq732PE91hHa23/deviceHealth').set({
  battery: 85,
  gsm: true,
  gps: true,
  lastUpdate: Date.now()
});

// Example: Set electrical diagnostics
firebase.database().ref('helmet_public/MnzBjTBslZNijOkq732PE91hHa23/electrical').set({
  headlight: true,
  taillight: true,
  brakeLight: false,  // This will trigger a fault alert
  signalLights: true,
  lastCheck: Date.now()
});
```

## Key Features Implemented

✅ **Helmet-Motorcycle Pairing Status** - Shows connection status of both devices
✅ **Device Health Monitoring** - Battery level, GSM, and GPS status
✅ **Manual SOS Button** - Emergency alert trigger with location
✅ **Electrical Diagnostics** - Real-time monitoring of headlight, taillight, brake light, and signal lights
✅ **Premium UI Design** - Glassmorphism, gradients, and smooth animations
✅ **Real-time Alerts** - Automatic notifications for faults and critical events

## Next Steps for Full Implementation

1. **ESP32 Integration** - Configure ESP32 modules to push data to these Firebase paths
2. **SMS/GSM Integration** - Implement SMS sending for emergency alerts
3. **Firmware Updates** - Add OTA update capability
4. **Remote Lockout** - Add admin controls for remote ignition lockout
5. **Alert History** - Implement comprehensive alert logging and export
6. **Trip Distance Calculation** - Add distance tracking to trip logs
