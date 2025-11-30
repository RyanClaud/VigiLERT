# Dashboard Button Device Connection Check

## Feature

Dashboard engine control button is now disabled when either helmet or motorcycle is disconnected.

## Changes Made

### Button Disabled Conditions

**Before:**
```vue
:disabled="alcoholDetected && !engineRunning"
```
Only disabled when alcohol detected and engine not running.

**After:**
```vue
:disabled="(alcoholDetected && !engineRunning) || !helmetPaired || !motorcyclePaired"
```
Disabled when:
1. Alcohol detected AND engine not running, OR
2. Helmet not paired, OR
3. Motorcycle not paired

### Button Styling

**Before:**
```vue
alcoholDetected 
  ? 'bg-gray-500 cursor-not-allowed text-gray-300'
  : 'bg-white hover:bg-gray-100 text-green-600'
```

**After:**
```vue
(alcoholDetected || !helmetPaired || !motorcyclePaired)
  ? 'bg-gray-500 cursor-not-allowed text-gray-300'
  : 'bg-white hover:bg-gray-100 text-green-600'
```

## Button States

| Helmet | Motorcycle | Alcohol | Engine | Button State | Button Color |
|--------|------------|---------|--------|--------------|--------------|
| ✅ ON  | ✅ ON      | ✅ Safe | OFF    | Enabled ✅   | White/Green |
| ✅ ON  | ✅ ON      | ✅ Safe | ON     | Enabled ✅   | Red |
| ✅ ON  | ✅ ON      | ❌ Danger | OFF  | Disabled ❌  | Gray |
| ❌ OFF | ✅ ON      | ✅ Safe | OFF    | Disabled ❌  | Gray |
| ✅ ON  | ❌ OFF     | ✅ Safe | OFF    | Disabled ❌  | Gray |
| ❌ OFF | ❌ OFF     | ✅ Safe | OFF    | Disabled ❌  | Gray |

## Visual Feedback

### Button Enabled (Both Devices Connected)
```
┌─────────────────────────────┐
│  ▶  Turn On Engine          │  ← White/Green, clickable
└─────────────────────────────┘
```

### Button Disabled (Device Disconnected)
```
┌─────────────────────────────┐
│  ▶  Turn On Engine          │  ← Gray, not clickable
└─────────────────────────────┘
```

### Button Disabled (Alcohol Detected)
```
┌─────────────────────────────┐
│  ▶  Turn On Engine          │  ← Gray, not clickable
└─────────────────────────────┘
```

## User Experience

### Scenario 1: Normal Operation
1. Both devices ON and connected
2. No alcohol detected
3. **Button:** Enabled, white/green
4. **User can:** Start engine ✅

### Scenario 2: Helmet Disconnected
1. Helmet OFF or disconnected
2. Motorcycle ON
3. **Button:** Disabled, gray
4. **User sees:** Pairing Status shows "Disconnected"
5. **User cannot:** Start engine ❌

### Scenario 3: Motorcycle Disconnected
1. Helmet ON
2. Motorcycle OFF or disconnected
3. **Button:** Disabled, gray
4. **User sees:** Pairing Status shows "Disconnected"
5. **User cannot:** Start engine ❌

### Scenario 4: Alcohol Detected
1. Both devices ON
2. Alcohol detected
3. **Button:** Disabled, gray
4. **User sees:** Alcohol Detection card shows "Danger"
5. **User cannot:** Start engine ❌

### Scenario 5: Engine Running, Device Disconnects
1. Engine running
2. Helmet disconnects
3. **Button:** Still enabled (can stop engine)
4. **System:** Auto-shutdown triggered by motorcycle
5. **Result:** Engine stops automatically

## Mobile Floating Button

The floating action button (mobile view) has the same logic:

```vue
:disabled="(alcoholDetected && !engineRunning) || !helmetPaired || !motorcyclePaired"
```

**States:**
- Green: Both devices connected, no alcohol, ready to start
- Red (pulsing): Engine running
- Gray: Disabled (device disconnected or alcohol detected)

## Testing

### Test 1: Both Devices Connected
1. Turn ON helmet
2. Turn ON motorcycle
3. Wait for pairing
4. **Expected:** Button is white/green and clickable
5. **Expected:** Can start engine

### Test 2: Turn OFF Helmet
1. Both devices connected
2. Turn OFF helmet
3. Wait 5 seconds
4. **Expected:** Pairing status shows "Disconnected"
5. **Expected:** Button turns gray and becomes disabled
6. **Expected:** Cannot click button

### Test 3: Turn OFF Motorcycle
1. Both devices connected
2. Turn OFF motorcycle
3. Wait 5 seconds
4. **Expected:** Pairing status shows "Disconnected"
5. **Expected:** Button turns gray and becomes disabled
6. **Expected:** Cannot click button

### Test 4: Reconnect Devices
1. One device disconnected, button disabled
2. Turn ON the disconnected device
3. Wait for pairing (2-10 seconds)
4. **Expected:** Pairing status shows "Connected"
5. **Expected:** Button turns white/green and becomes enabled
6. **Expected:** Can start engine again

### Test 5: Alcohol Detected
1. Both devices connected
2. Alcohol detected
3. **Expected:** Button turns gray and becomes disabled
4. **Expected:** Cannot start engine

## Benefits

### Safety
✅ Prevents starting engine without both devices
✅ Visual feedback shows why button is disabled
✅ Consistent with pairing status display
✅ User cannot bypass safety checks

### User Experience
✅ Clear visual indication (gray = disabled)
✅ Button state matches system state
✅ No confusing error messages
✅ Intuitive behavior

### System Integration
✅ Works with pairing status
✅ Works with alcohol detection
✅ Works on desktop and mobile
✅ Consistent across all views

## Technical Details

### Reactive Variables Used
```javascript
helmetPaired: ref(false)      // Helmet connection status
motorcyclePaired: ref(false)  // Motorcycle connection status
alcoholDetected: ref(false)   // Alcohol detection status
engineRunning: ref(false)     // Engine running status
```

### Disabled Logic
```javascript
// Button is disabled if:
(alcoholDetected && !engineRunning)  // Alcohol detected when trying to start
|| !helmetPaired                     // Helmet not connected
|| !motorcyclePaired                 // Motorcycle not connected
```

### Styling Logic
```javascript
// Button is gray if:
(alcoholDetected || !helmetPaired || !motorcyclePaired)
  ? 'bg-gray-500 cursor-not-allowed text-gray-300'  // Disabled state
  : 'bg-white hover:bg-gray-100 text-green-600'     // Enabled state
```

## Files Modified
- `src/views/Dashboard.vue`
  - Line 277: Desktop button disabled condition
  - Line 281: Desktop button styling
  - Line 723: Mobile button disabled condition
  - Line 727: Mobile button styling

## Summary

**What changed:**
- ✅ Button disabled when helmet disconnected
- ✅ Button disabled when motorcycle disconnected
- ✅ Button turns gray when disabled
- ✅ Works on desktop and mobile views

**Result:**
Users cannot start the engine from the dashboard when either device is disconnected. The button provides clear visual feedback by turning gray and becoming unclickable.

This ensures safety and prevents confusion when devices are not properly connected.
