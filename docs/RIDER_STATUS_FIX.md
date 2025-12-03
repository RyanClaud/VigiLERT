# Rider Status Fix - Both Devices Required ✅

## Problem

The Rider Status card was showing "Active" when only the motorcycle module was turned on, even though the helmet module was off. This is incorrect because the rider should only be considered "Active" when **BOTH** devices are on.

### Before (Incorrect Behavior):
```
Helmet Module: OFF
Motorcycle Module: ON
↓
Rider Status: Active ✗ (WRONG!)
```

## Root Cause

The `helmetRef` listener was only checking if the helmet was connected (`helmetConnected` field), not considering whether both devices were actually paired and sending heartbeats.

### Old Logic:
```javascript
riderStatus.value = isConnected ? 'Active' : 'Inactive';
```

This only checked the helmet connection, ignoring the motorcycle status.

## Solution

Updated the logic to check **BOTH** `helmetPaired` and `motorcyclePaired` values, which are determined by recent heartbeats from both devices.

### New Logic:
```javascript
const bothDevicesOn = helmetPaired.value && motorcyclePaired.value;
riderStatus.value = bothDevicesOn ? 'Active' : 'Inactive';
```

## Implementation

### 1. Dashboard.vue Changes

**File:** `src/views/Dashboard.vue`

```javascript
// ✅ FIX: Rider status should be Active ONLY when BOTH devices are on
const bothDevicesOn = helmetPaired.value && motorcyclePaired.value;
riderStatus.value = bothDevicesOn ? 'Active' : 'Inactive';

if (bothDevicesOn) {
  riderSubtitle.value = 'Helmet connected';
} else if (!helmetPaired.value && !motorcyclePaired.value) {
  riderSubtitle.value = 'Both devices disconnected';
} else if (!helmetPaired.value) {
  riderSubtitle.value = 'Helmet not connected';
} else {
  riderSubtitle.value = 'Motorcycle not connected';
}

console.log(`[DEBUG] Helmet: ${helmetPaired.value}, Motorcycle: ${motorcyclePaired.value}, Rider Status: ${riderStatus.value}`);
```

### 2. Motorcycle Module Changes

**File:** `MotorcycleToWebApplication_COMPLETE.ino`

Added periodic heartbeat updates (every 2 seconds):

```cpp
// ✅ NEW: Periodic motorcycle heartbeat update (every 2 seconds)
static unsigned long lastMotorcycleHeartbeat = 0;
const unsigned long MOTORCYCLE_HEARTBEAT_INTERVAL = 2000;
if (millis() - lastMotorcycleHeartbeat > MOTORCYCLE_HEARTBEAT_INTERVAL) {
  updateMotorcycleDeviceStatus(true);
  lastMotorcycleHeartbeat = millis();
}
```

## How It Works Now

### Heartbeat System

Both devices send heartbeats to Firebase:

**Helmet Module** → `/devices/helmet`
```json
{
  "status": "On",
  "lastHeartbeat": 3488820531,
  "timestamp": 3488820531
}
```

**Motorcycle Module** → `/devices/motorcycle`
```json
{
  "status": "On",
  "lastHeartbeat": 1763789850345,
  "timestamp": 1763789850345
}
```

### Dashboard Listeners

The dashboard checks heartbeats every time they update:

```javascript
// Helmet listener
onValue(helmetStatusRef, (snapshot) => {
  const data = snapshot.val();
  if (data && data.status === 'On' && data.lastHeartbeat) {
    const now = Date.now();
    const timeSinceLastBeat = now - data.lastHeartbeat;
    
    // Connected if heartbeat within 10 seconds
    helmetPaired.value = timeSinceLastBeat < 10000;
  } else {
    helmetPaired.value = false;
  }
});

// Motorcycle listener (same logic)
onValue(motorcycleStatusRef, (snapshot) => {
  // ... same heartbeat check
  motorcyclePaired.value = timeSinceLastBeat < 10000;
});
```

### Rider Status Determination

```
helmetPaired = true  + motorcyclePaired = true  → Active
helmetPaired = true  + motorcyclePaired = false → Inactive
helmetPaired = false + motorcyclePaired = true  → Inactive
helmetPaired = false + motorcyclePaired = false → Inactive
```

## Visual States

### State 1: Both Devices OFF
```
┌─────────────────────────────────────┐
│ 🔗 PAIRING STATUS                   │
│                                     │
│ Disconnected                        │
│                                     │
│ 🔴 Helmet: OFF                      │
│ 🔴 Motorcycle: OFF                  │
└─────────────────────────────────────┘

┌─────────────────────────────────────┐
│ 👤 Rider Status                     │
│                                     │
│ Inactive                            │
│ Both devices disconnected           │
│                                     │
│ Background: RED                     │
└─────────────────────────────────────┘
```

### State 2: Only Motorcycle ON
```
┌─────────────────────────────────────┐
│ 🔗 PAIRING STATUS                   │
│                                     │
│ Disconnected                        │
│                                     │
│ 🔴 Helmet: OFF                      │
│ 🟢 Motorcycle: ON                   │
└─────────────────────────────────────┘

┌─────────────────────────────────────┐
│ 👤 Rider Status                     │
│                                     │
│ Inactive                            │
│ Helmet not connected                │
│                                     │
│ Background: RED                     │
└─────────────────────────────────────┘
```

### State 3: Only Helmet ON
```
┌─────────────────────────────────────┐
│ 🔗 PAIRING STATUS                   │
│                                     │
│ Disconnected                        │
│                                     │
│ 🟢 Helmet: ON                       │
│ 🔴 Motorcycle: OFF                  │
└─────────────────────────────────────┘

┌─────────────────────────────────────┐
│ 👤 Rider Status                     │
│                                     │
│ Inactive                            │
│ Motorcycle not connected            │
│                                     │
│ Background: RED                     │
└─────────────────────────────────────┘
```

### State 4: Both Devices ON ✅
```
┌─────────────────────────────────────┐
│ 🔗 PAIRING STATUS                   │
│                                     │
│ Connected                           │
│                                     │
│ 🟢 Helmet: ON                       │
│ 🟢 Motorcycle: ON                   │
└─────────────────────────────────────┘

┌─────────────────────────────────────┐
│ 👤 Rider Status                     │
│                                     │
│ Active                              │
│ Helmet connected                    │
│                                     │
│ Background: BLUE                    │
└─────────────────────────────────────┘
```

## Console Logging

### When Only Motorcycle is ON:
```
[DEBUG] Helmet device data from Firebase: {status: "Off"}
[DEBUG] Helmet: DISCONNECTED (no data or status Off)
[DEBUG] Motorcycle device data from Firebase: {status: "On", lastHeartbeat: 1763789850345}
[DEBUG] Motorcycle: CONNECTED (last beat: 2345ms ago)
[DEBUG] Helmet: false, Motorcycle: true, Rider Status: Inactive
```

### When Both Devices are ON:
```
[DEBUG] Helmet device data from Firebase: {status: "On", lastHeartbeat: 3488820531}
[DEBUG] Helmet: CONNECTED (last beat: 1234ms ago)
[DEBUG] Motorcycle device data from Firebase: {status: "On", lastHeartbeat: 1763789850345}
[DEBUG] Motorcycle: CONNECTED (last beat: 2345ms ago)
[DEBUG] Helmet: true, Motorcycle: true, Rider Status: Active
```

## Testing Scenarios

### Test 1: Turn on Motorcycle Only
```
1. Turn on motorcycle ESP32
2. Wait for heartbeat to send
3. Check dashboard

Expected:
  - Pairing Status: Disconnected
  - Helmet: 🔴 OFF
  - Motorcycle: 🟢 ON
  - Rider Status: Inactive (RED)
  - Subtitle: "Helmet not connected"
```

### Test 2: Turn on Helmet Only
```
1. Turn on helmet ESP32
2. Wait for heartbeat to send
3. Check dashboard

Expected:
  - Pairing Status: Disconnected
  - Helmet: 🟢 ON
  - Motorcycle: 🔴 OFF
  - Rider Status: Inactive (RED)
  - Subtitle: "Motorcycle not connected"
```

### Test 3: Turn on Both Devices
```
1. Turn on motorcycle ESP32
2. Turn on helmet ESP32
3. Wait for both heartbeats
4. Check dashboard

Expected:
  - Pairing Status: Connected
  - Helmet: 🟢 ON
  - Motorcycle: 🟢 ON
  - Rider Status: Active (BLUE)
  - Subtitle: "Helmet connected"
```

### Test 4: Turn off Helmet While Riding
```
1. Both devices ON (Active)
2. Turn off helmet ESP32
3. Wait 10 seconds for heartbeat timeout
4. Check dashboard

Expected:
  - Pairing Status: Disconnected
  - Helmet: 🔴 OFF
  - Motorcycle: 🟢 ON
  - Rider Status: Inactive (RED)
  - Subtitle: "Helmet not connected"
  - Warning banner appears
```

## Heartbeat Timing

| Device | Interval | Timeout | Firebase Path |
|--------|----------|---------|---------------|
| Helmet | 2 seconds | 10 seconds | `/devices/helmet` |
| Motorcycle | 2 seconds | 10 seconds | `/devices/motorcycle` |

**Timeout Logic:** If no heartbeat received within 10 seconds, device is considered disconnected.

## Benefits

1. **Accurate Status:** Rider status only shows "Active" when truly ready to ride
2. **Safety:** Prevents false "Active" status when only one device is on
3. **Clear Feedback:** Subtitle shows exactly which device is missing
4. **Real-time Updates:** Status changes immediately when devices turn on/off
5. **Reliable:** Uses heartbeat system to detect actual device status

## Summary

The rider status now correctly requires **BOTH** devices to be on before showing "Active". This ensures accurate safety monitoring and prevents confusion about the system state.

### Key Changes:
- ✅ Dashboard checks both `helmetPaired` AND `motorcyclePaired`
- ✅ Motorcycle sends periodic heartbeats every 2 seconds
- ✅ Subtitle shows which device is missing
- ✅ Console logs show both device states

The system now provides accurate, real-time rider status based on both devices! 🎉
