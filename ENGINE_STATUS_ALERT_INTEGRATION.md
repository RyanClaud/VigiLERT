# Engine Status Alert Integration

## ✅ Feature Overview

The Engine Status display now **dynamically responds to safety alerts** and shows why the engine was shut down. The status changes color and displays specific shutdown reasons when alerts are detected.

## 🎯 Problem Solved

**Before**: Engine Status only showed "Running" or "Stopped" without context.

**After**: Engine Status shows:
- ✅ Why the engine stopped (Alcohol/Crash/Anti-Theft)
- ✅ Color-coded alerts (Green/Red/Gray)
- ✅ Specific shutdown reasons
- ✅ Real-time relay state

## 🎨 Visual States

### 1. Engine Running (Normal)
```
┌─────────────────────────────┐
│ 🟢 Engine Status            │
│    Relay Control Monitor    │
│                             │
│ Status:    Running          │ ← Green
│ Relay:     ON               │ ← Green, Bold
│ Condition: Active           │ ← Green
└─────────────────────────────┘
```
- **Color**: Green background
- **Icon**: Pulsing green
- **Indicator**: Pulsing green dot

### 2. Engine Stopped - Alcohol Detected
```
┌─────────────────────────────┐
│ 🔴 Engine Status            │
│    Relay Control Monitor    │
│                             │
│ Status:    Stopped - Alcohol│ ← Red
│ Relay:     OFF              │ ← Red, Bold
│ Condition: Blocked - Alcohol│ ← Red
│ ─────────────────────────── │
│ ⚠️ Shutdown Reason:         │
│ 🍺 Alcohol detected -       │
│    Engine disabled for      │
│    safety                   │
└─────────────────────────────┘
```
- **Color**: Red background
- **Icon**: Pulsing red
- **Indicator**: Pulsing red dot
- **Reason**: Alcohol safety block

### 3. Engine Stopped - Crash Detected
```
┌─────────────────────────────┐
│ 🔴 Engine Status            │
│    Relay Control Monitor    │
│                             │
│ Status:    Stopped - Crash  │ ← Red
│ Relay:     OFF              │ ← Red, Bold
│ Condition: Emergency Stop   │ ← Red
│ ─────────────────────────────│
│ ⚠️ Shutdown Reason:         │
│ 💥 Crash detected -         │
│    Emergency shutdown       │
│    activated                │
└─────────────────────────────┘
```
- **Color**: Red background
- **Icon**: Pulsing red
- **Indicator**: Pulsing red dot
- **Reason**: Emergency crash shutdown

### 4. Engine Stopped - Anti-Theft Armed
```
┌─────────────────────────────┐
│ 🔴 Engine Status            │
│    Relay Control Monitor    │
│                             │
│ Status:    Stopped - Theft  │ ← Red
│ Relay:     OFF              │ ← Red, Bold
│ Condition: Security Lock    │ ← Red
│ ─────────────────────────────│
│ ⚠️ Shutdown Reason:         │
│ 🔒 Anti-theft armed -       │
│    Engine locked            │
└─────────────────────────────┘
```
- **Color**: Red background
- **Icon**: Pulsing red
- **Indicator**: Pulsing red dot
- **Reason**: Security system active

### 5. Engine Stopped - Normal
```
┌─────────────────────────────┐
│ ⚪ Engine Status            │
│    Relay Control Monitor    │
│                             │
│ Status:    Stopped          │ ← Gray
│ Relay:     OFF              │ ← Gray, Bold
│ Condition: Inactive         │ ← Gray
└─────────────────────────────┘
```
- **Color**: Gray background
- **Icon**: Gray (no pulse)
- **Indicator**: Gray dot
- **Reason**: None (normal stop)

## 🔧 Implementation Details

### Dashboard Changes (`src/views/Dashboard.vue`)

#### 1. Added Alert Status to sensorData

```javascript
const sensorData = ref({
  // ... existing fields
  engineRunning: false,
  crashDetected: false,     // ✅ NEW
  antiTheftArmed: false,    // ✅ NEW
  // ... other fields
});
```

#### 2. Real-Time Updates from Firebase

```javascript
// Update crash detection status
if (typeof liveData.crashDetected !== 'undefined') {
  sensorData.value.crashDetected = liveData.crashDetected;
  console.log('[CRASH] Status updated:', liveData.crashDetected ? 'DETECTED' : 'CLEAR');
}

// Update anti-theft armed status
if (typeof liveData.antiTheftArmed !== 'undefined') {
  sensorData.value.antiTheftArmed = liveData.antiTheftArmed;
  console.log('[ANTI-THEFT] Status updated:', liveData.antiTheftArmed ? 'ARMED' : 'DISARMED');
}
```

### SensorHealthPanel Changes (`src/components/SensorHealthPanel.vue`)

#### 1. Dynamic Color Based on Alert State

```javascript
const getEngineStatusColor = () => {
  if (props.sensorData.engineRunning) {
    return 'bg-green-50 border-green-500';  // Running
  }
  // Check if stopped due to alert
  if (props.alcoholStatus === 'Danger' || 
      props.sensorData.crashDetected || 
      props.sensorData.antiTheftArmed) {
    return 'bg-red-50 border-red-500';  // Alert shutdown
  }
  return 'bg-gray-50 border-gray-300';  // Normal stop
};
```

#### 2. Context-Aware Status Text

```javascript
const getEngineStatusText = () => {
  if (props.sensorData.engineRunning) {
    return 'Running';
  }
  if (props.alcoholStatus === 'Danger') {
    return 'Stopped - Alcohol';
  }
  if (props.sensorData.crashDetected) {
    return 'Stopped - Crash';
  }
  if (props.sensorData.antiTheftArmed) {
    return 'Stopped - Theft';
  }
  return 'Stopped';
};
```

#### 3. Shutdown Reason Display

```javascript
const getShutdownReason = () => {
  if (props.sensorData.engineRunning) {
    return null; // No shutdown if running
  }
  
  if (props.alcoholStatus === 'Danger') {
    return '🍺 Alcohol detected - Engine disabled for safety';
  }
  if (props.sensorData.crashDetected) {
    return '💥 Crash detected - Emergency shutdown activated';
  }
  if (props.sensorData.antiTheftArmed) {
    return '🔒 Anti-theft armed - Engine locked';
  }
  return null;
};
```

## 📊 Data Flow

```
Arduino ESP32
    ↓
Firebase /live/
    ├── engineRunning: true/false
    ├── crashDetected: true/false
    └── antiTheftArmed: true/false
    ↓
Dashboard Listener
    ↓
sensorData.value updates
    ↓
SensorHealthPanel
    ↓
Dynamic UI Display
```

## 🧪 Testing Scenarios

### Test 1: Normal Engine Operation

**Steps**:
1. Start engine via dashboard
2. Check Engine Status card

**Expected**:
- ✅ Green background
- ✅ Status: "Running"
- ✅ Relay: "ON" (green)
- ✅ Condition: "Active"
- ✅ No shutdown reason shown

### Test 2: Alcohol Detection Shutdown

**Steps**:
1. Engine is running
2. Trigger alcohol detection (blow into sensor)
3. Watch Engine Status card

**Expected**:
- ✅ Changes to red background
- ✅ Status: "Stopped - Alcohol"
- ✅ Relay: "OFF" (red)
- ✅ Condition: "Blocked - Alcohol"
- ✅ Shutdown reason: "🍺 Alcohol detected - Engine disabled for safety"

### Test 3: Crash Detection Shutdown

**Steps**:
1. Engine is running
2. Trigger crash (shake MPU6050 hard)
3. Watch Engine Status card

**Expected**:
- ✅ Changes to red background
- ✅ Status: "Stopped - Crash"
- ✅ Relay: "OFF" (red)
- ✅ Condition: "Emergency Stop"
- ✅ Shutdown reason: "💥 Crash detected - Emergency shutdown activated"

### Test 4: Anti-Theft Activation

**Steps**:
1. Stop engine
2. Wait 30 seconds (anti-theft arms)
3. Check Engine Status card

**Expected**:
- ✅ Changes to red background
- ✅ Status: "Stopped - Theft"
- ✅ Relay: "OFF" (red)
- ✅ Condition: "Security Lock"
- ✅ Shutdown reason: "🔒 Anti-theft armed - Engine locked"

### Test 5: Normal Stop

**Steps**:
1. Stop engine via dashboard
2. Check Engine Status card

**Expected**:
- ✅ Gray background
- ✅ Status: "Stopped"
- ✅ Relay: "OFF" (gray)
- ✅ Condition: "Inactive"
- ✅ No shutdown reason shown

## 🎯 Alert Priority

When multiple alerts are active, the display shows them in this priority:

1. **Alcohol Detection** (highest priority)
2. **Crash Detection**
3. **Anti-Theft Armed**
4. **Normal Stop** (lowest priority)

Example: If both alcohol and crash are detected, it shows "Stopped - Alcohol".

## 🔍 Debugging

### Check Firebase Data

Open Firebase Console and verify these paths:

```
/helmet_public/{userId}/live/
{
  "engineRunning": false,
  "crashDetected": true,
  "antiTheftArmed": false
}
```

### Check Browser Console

Look for these log messages:

```
[ENGINE] Status updated: STOPPED
[CRASH] Status updated: DETECTED
[ANTI-THEFT] Status updated: ARMED
```

### Verify Sensor Data

Open browser console and run:

```javascript
console.log('Sensor Data:', sensorData.value);
```

Should show:
```javascript
{
  engineRunning: false,
  crashDetected: true,
  antiTheftArmed: false,
  // ... other fields
}
```

## 📱 Responsive Design

The Engine Status card adapts to all screen sizes:

- **Desktop**: Full card with all details
- **Tablet**: Compact card with icons
- **Mobile**: Stacked layout with touch-friendly buttons

## ✅ Benefits

### User Experience
- ✅ **Clear feedback** - Users know exactly why engine stopped
- ✅ **Visual alerts** - Color-coded for quick recognition
- ✅ **Context-aware** - Shows relevant information
- ✅ **Real-time** - Updates instantly

### Safety
- ✅ **Alcohol prevention** - Clear indication of safety block
- ✅ **Crash awareness** - Emergency shutdown visible
- ✅ **Security status** - Anti-theft state displayed
- ✅ **Relay monitoring** - Actual relay state shown

### Debugging
- ✅ **Relay verification** - See if relay is responding
- ✅ **Alert tracking** - Know which alert triggered
- ✅ **System status** - Complete engine state overview
- ✅ **Troubleshooting** - Identify issues quickly

## 🚀 Future Enhancements

Potential improvements:

1. **Alert History** - Show last 5 shutdown events
2. **Time Tracking** - How long engine has been stopped
3. **Override Button** - Manual override for false alarms
4. **Alert Logs** - Detailed log of all shutdowns
5. **Statistics** - Count of each shutdown type
6. **Notifications** - Push alerts when engine stops

## 📝 Summary

The Engine Status display now provides:

✅ **Dynamic visual feedback** based on alert state  
✅ **Specific shutdown reasons** for each alert type  
✅ **Color-coded indicators** (Green/Red/Gray)  
✅ **Real-time relay monitoring**  
✅ **Context-aware status messages**  
✅ **Emergency shutdown visibility**  

Users can now instantly see why the engine stopped and what safety system is active!
