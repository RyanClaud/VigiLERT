# Sensor Health Device Pairing Fix

## 🐛 Problem Identified

**Issue**: Sensor Health Monitor showed all sensors as "Active" even when devices (helmet/motorcycle) were completely offline/disconnected.

**Root Cause**: Sensors were only checking their last update timestamp, not the actual device connection status.

## ✅ Solution Implemented

### Now Sensors Check Device Pairing First

Before checking sensor data, the system now verifies if the parent device is connected:

- **MPU6050** → Requires motorcycle paired
- **Alcohol Sensor** → Requires helmet paired  
- **GPS Module** → Requires motorcycle paired
- **GSM Module** → Requires motorcycle paired
- **Engine Status** → Requires motorcycle paired
- **Battery** → Independent (always shows data)

## 🎨 Visual States

### When Devices Are Connected

**All Sensors Active** (Green):
```
┌─────────────────────────────┐
│ 🟢 MPU6050                  │
│ Status: Active              │
│ Last Update: 2s ago         │
└─────────────────────────────┘

┌─────────────────────────────┐
│ 🟢 GPS Module               │
│ Status: Active              │
│ Satellites: 8               │
└─────────────────────────────┘
```

### When Devices Are Disconnected

**All Sensors Offline** (Gray):
```
┌─────────────────────────────┐
│ ⚪ MPU6050                  │
│ Status: Offline             │
│ Last Update: Device         │
│              disconnected   │
└─────────────────────────────┘

┌─────────────────────────────┐
│ ⚪ GPS Module               │
│ Status: Device Offline      │
│ Satellites: 0               │
└─────────────────────────────┘
```

## 🔧 Implementation Details

### SensorHealthPanel Changes

#### 1. Added Device Pairing Props

```javascript
const props = defineProps({
  gpsConnected: Boolean,
  gsmConnected: Boolean,
  deviceBattery: Number,
  alcoholStatus: String,
  helmetPaired: Boolean,      // ✅ NEW
  motorcyclePaired: Boolean,  // ✅ NEW
  sensorData: {
    type: Object,
    default: () => ({})
  }
});
```

#### 2. Updated getSensorStatus Function

```javascript
const getSensorStatus = (sensor) => {
  // ✅ Check device pairing first
  if (sensor === 'mpu6050') {
    if (!props.motorcyclePaired) {
      return {
        isActive: false,
        status: 'Offline',
        lastUpdate: 'Device disconnected'
      };
    }
  }
  
  if (sensor === 'alcohol') {
    if (!props.helmetPaired) {
      return {
        isActive: false,
        status: 'Offline',
        lastUpdate: 'Device disconnected'
      };
    }
  }
  
  // Then check last update time
  const now = Date.now();
  const lastUpdate = props.sensorData[sensor]?.lastUpdate || 0;
  const timeDiff = now - lastUpdate;
  
  const isActive = timeDiff < 10000;
  
  return {
    isActive,
    status: isActive ? 'Active' : 'Offline',
    lastUpdate: isActive ? `${Math.floor(timeDiff / 1000)}s ago` : 'No data'
  };
};
```

#### 3. GPS Status Functions

```javascript
const getGPSStatusColor = () => {
  if (!props.motorcyclePaired) {
    return 'bg-gray-50 border-gray-300'; // Device offline
  }
  return props.gpsConnected ? 'bg-green-50 border-green-500' : 'bg-red-50 border-red-500';
};

const getGPSStatusText = () => {
  if (!props.motorcyclePaired) {
    return 'Device Offline';
  }
  return props.gpsConnected ? 'Active' : 'Offline';
};
```

#### 4. GSM Status Functions

```javascript
const getGSMStatusColor = () => {
  if (!props.motorcyclePaired) {
    return 'bg-gray-50 border-gray-300'; // Device offline
  }
  return props.gsmConnected ? 'bg-green-50 border-green-500' : 'bg-red-50 border-red-500';
};

const getGSMStatusText = () => {
  if (!props.motorcyclePaired) {
    return 'Device Offline';
  }
  return props.gsmConnected ? 'Connected' : 'Offline';
};
```

### Dashboard Changes

#### Updated SensorHealthPanel Props

```vue
<SensorHealthPanel 
  :gps-connected="gpsConnected"
  :gsm-connected="gsmConnected"
  :device-battery="deviceBattery"
  :alcohol-status="alcoholStatus"
  :helmet-paired="helmetPaired"        <!-- ✅ NEW -->
  :motorcycle-paired="motorcyclePaired" <!-- ✅ NEW -->
  :sensor-data="sensorData"
  class="mb-8"
/>
```

## 📊 Sensor-Device Mapping

| Sensor | Parent Device | Shows Offline When |
|--------|--------------|-------------------|
| **MPU6050** | Motorcycle | Motorcycle disconnected |
| **GPS Module** | Motorcycle | Motorcycle disconnected |
| **GSM Module** | Motorcycle | Motorcycle disconnected |
| **Engine Status** | Motorcycle | Motorcycle disconnected |
| **Alcohol Sensor** | Helmet | Helmet disconnected |
| **Battery** | Independent | Never (always shows data) |

## 🧪 Testing Scenarios

### Test 1: Both Devices Connected

**Steps**:
1. Turn on helmet module
2. Turn on motorcycle module
3. Wait for pairing (green indicators)
4. Check Sensor Health Monitor

**Expected**:
- ✅ All sensors show green/active
- ✅ MPU6050: Active
- ✅ GPS: Active
- ✅ GSM: Connected/Offline (based on signal)
- ✅ Alcohol: Active
- ✅ Engine: Shows current state

### Test 2: Motorcycle Disconnected

**Steps**:
1. Turn off motorcycle module
2. Wait 10 seconds for timeout
3. Check Sensor Health Monitor

**Expected**:
- ✅ MPU6050: Gray, "Device disconnected"
- ✅ GPS: Gray, "Device Offline"
- ✅ GSM: Gray, "Device Offline"
- ✅ Engine: Gray, "Inactive"
- ✅ Alcohol: Still active (helmet connected)
- ✅ Battery: Still shows data

### Test 3: Helmet Disconnected

**Steps**:
1. Turn off helmet module
2. Wait 10 seconds for timeout
3. Check Sensor Health Monitor

**Expected**:
- ✅ Alcohol: Gray, "Device disconnected"
- ✅ MPU6050: Still active (motorcycle connected)
- ✅ GPS: Still active (motorcycle connected)
- ✅ GSM: Still active (motorcycle connected)
- ✅ Engine: Still shows state (motorcycle connected)

### Test 4: Both Devices Disconnected

**Steps**:
1. Turn off both modules
2. Wait 10 seconds for timeout
3. Check Sensor Health Monitor

**Expected**:
- ✅ ALL sensors show gray/offline
- ✅ MPU6050: "Device disconnected"
- ✅ GPS: "Device Offline"
- ✅ GSM: "Device Offline"
- ✅ Alcohol: "Device disconnected"
- ✅ Engine: "Inactive"
- ✅ Battery: Still shows last known data

### Test 5: Reconnection

**Steps**:
1. Start with devices off
2. Turn on motorcycle module
3. Wait 2 seconds
4. Check sensors

**Expected**:
- ✅ Motorcycle sensors turn green
- ✅ MPU6050: Active
- ✅ GPS: Active
- ✅ GSM: Active
- ✅ Engine: Shows state
- ✅ Alcohol: Still offline (helmet not connected)

## 🎯 Status Priority

Each sensor checks in this order:

1. **Device Pairing** (highest priority)
   - If parent device offline → Show "Device Offline"
   
2. **Sensor Connection** (medium priority)
   - If sensor not responding → Show "Offline"
   
3. **Last Update Time** (lowest priority)
   - If data stale (>10s) → Show "No data"

## 🔍 Debugging

### Check Device Pairing Status

Open browser console:
```javascript
console.log('Helmet Paired:', helmetPaired.value);
console.log('Motorcycle Paired:', motorcyclePaired.value);
```

### Check Sensor Status

```javascript
console.log('Sensor Data:', sensorData.value);
```

### Verify Firebase Heartbeats

Check Firebase Console:
```
/helmet_public/{userId}/devices/
├── helmet/
│   ├── status: "On"
│   └── lastHeartbeat: 1700000123456
└── motorcycle/
    ├── status: "On"
    └── lastHeartbeat: 1700000123456
```

## 📱 Visual Feedback

### Color Coding

- **🟢 Green**: Device connected, sensor active
- **🔴 Red**: Device connected, sensor error
- **⚪ Gray**: Device disconnected
- **🟡 Yellow**: Device connected, sensor warning

### Animation

- **Pulsing**: Active and receiving data
- **Static**: Offline or no data
- **Blinking**: Warning state

## ✅ Benefits

### Accurate Status Display
- ✅ Shows real device connection state
- ✅ No false "Active" when offline
- ✅ Clear "Device Offline" message
- ✅ Proper gray color for disconnected

### Better User Experience
- ✅ Users know if devices are connected
- ✅ Clear distinction between device offline vs sensor offline
- ✅ Easier troubleshooting
- ✅ Prevents confusion

### Debugging
- ✅ Quickly identify connection issues
- ✅ See which device is offline
- ✅ Verify pairing status
- ✅ Monitor sensor health accurately

## 🚀 Future Enhancements

Potential improvements:

1. **Connection History** - Show when device last connected
2. **Reconnection Timer** - Countdown to next connection attempt
3. **Signal Strength** - Show WiFi/Bluetooth signal quality
4. **Battery Per Device** - Separate battery for helmet/motorcycle
5. **Sensor Diagnostics** - Detailed sensor health metrics
6. **Auto-Reconnect** - Automatic reconnection attempts

## 📝 Summary

The Sensor Health Monitor now:

✅ **Checks device pairing first** before showing sensor status  
✅ **Shows "Device Offline"** when parent device disconnected  
✅ **Gray color** for offline devices  
✅ **Accurate status** reflecting real connection state  
✅ **Clear messaging** - "Device disconnected" vs "No data"  
✅ **Proper visual feedback** - No false "Active" states  

Sensors now accurately reflect the real scenario - if devices are off, sensors show offline!
