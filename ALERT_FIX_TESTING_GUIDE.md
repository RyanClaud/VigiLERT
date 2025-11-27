# Testing Guide: Alert Persistence Fix

## Quick Test Steps

### Test 1: Old Crash Should NOT Alert on Restart
**Purpose:** Verify old crashes don't trigger alerts when app restarts

1. **Trigger a crash** (or use existing crash in Firebase)
2. **Wait for alert** to appear and sound to play
3. **Close/refresh the browser** (restart the app)
4. **Expected Result:**
   - ✅ No alert sound plays
   - ✅ No red flashing crash card
   - ✅ Crash still visible in "Crash Site Locations" section (history)
   - ✅ Console shows: `[CRASH] ⏭️ Skipping old crash from before app start`

### Test 2: New Crash SHOULD Alert After Restart
**Purpose:** Verify new crashes still trigger alerts properly

1. **Restart the app** (refresh browser)
2. **Trigger a NEW crash** from Arduino
3. **Expected Result:**
   - ✅ Alert sound plays
   - ✅ Red crash card appears and flashes
   - ✅ Alert notification appears in "Recent Alerts"
   - ✅ Console shows: `[CRASH] ✓ New crash detected, triggering animation...`

### Test 3: Clear All Alerts Button
**Purpose:** Verify user can dismiss all alerts

1. **Have multiple alerts** in the "Recent Alerts" section
2. **Click "Clear All" button** (red button next to "Show/Hide")
3. **Expected Result:**
   - ✅ All alerts disappear from list
   - ✅ Alert count badge disappears
   - ✅ Crash animation stops (if running)
   - ✅ Crash card returns to "Vehicle Stable" (green)

### Test 4: Clear Individual Crash Alert
**Purpose:** Verify user can dismiss specific crash alerts

1. **Go to "Crash Site Locations" section**
2. **Click "Clear Alert" button** on a crash event
3. **Expected Result:**
   - ✅ Crash animation stops
   - ✅ Crash card returns to "Vehicle Stable"
   - ✅ Crash event still visible in history
   - ✅ Console shows: `[CRASH] ✓ Alert cleared, status reset to Stable`

## Console Log Verification

### On App Start (with existing crash)
```
[INIT] App started at: [current timestamp]
[CRASH] New crash event received from Firebase: {...}
[CRASH] ✓ Valid crash event: {...}
[CRASH] ⏭️ Skipping old crash from before app start
[CRASH] Crash time: [old timestamp]
[CRASH] App start: [current timestamp]
```

### On New Crash Detection
```
[CRASH] New crash event received from Firebase: {...}
[CRASH] ✓ Valid crash event: {...}
[CRASH] ✓ Showing ONLY latest crash marker
[CRASH] Comparing timestamps - Last: [old], New: [new]
[CRASH] ✓ New crash detected, triggering animation...
[FLASH CRASH] 🚨🚨🚨 Starting crash alert animation...
```

### On Clear Alert
```
[CRASH] Clearing crash alert for event: {...}
[CRASH] ✓ Alert cleared, status reset to Stable
```

## Visual Indicators

### Normal State (No Crash)
- **Crash Card:** Green background, "Vehicle Stable"
- **Alert Count:** No badge or "0"
- **Recent Alerts:** Empty or old alerts

### Active Crash Alert
- **Crash Card:** Red background, pulsing animation, "Crash Detected"
- **Alert Count:** Red badge with number
- **Recent Alerts:** "🚨 CRASH DETECTED!" at top
- **Sound:** Alert sound playing

### After Restart (Old Crash)
- **Crash Card:** Green background, "Vehicle Stable" (NOT red)
- **Alert Count:** No new alerts
- **Recent Alerts:** No new crash alerts
- **Sound:** Silent (no alert sound)
- **History:** Crash visible in "Crash Site Locations"

## Common Issues & Solutions

### Issue: Alert still plays on restart
**Solution:** Check console for timestamp comparison. Ensure `appStartTime` is being set correctly.

### Issue: New crashes not alerting
**Solution:** Check if crash timestamp is after `appStartTime`. Verify Firebase connection.

### Issue: Clear button not working
**Solution:** Check console for errors. Verify `clearCrashAlert()` function is being called.

### Issue: Crash history not showing
**Solution:** Old crashes should still appear in "Crash Site Locations" section, just without alerts.

## Browser Developer Tools

### Check App Start Time
```javascript
// In browser console
console.log('App Start Time:', new Date(appStartTime.value).toLocaleString());
```

### Check Last Crash Timestamp
```javascript
// In browser console
console.log('Last Crash:', localStorage.getItem('lastCrashTimestamp_MnzBjTBslZNijOkq732PE91hHa23'));
```

### Manually Clear localStorage (if needed)
```javascript
// In browser console
localStorage.removeItem('lastCrashTimestamp_MnzBjTBslZNijOkq732PE91hHa23');
```

## Success Criteria

✅ **All tests pass**
✅ **No false alarms on restart**
✅ **New crashes still trigger alerts**
✅ **User can dismiss alerts**
✅ **Console logs show correct filtering**
✅ **No JavaScript errors in console**

## Deployment Checklist

- [x] Code changes applied
- [x] Build successful (no errors)
- [x] Documentation created
- [ ] Test on development environment
- [ ] Test on production environment
- [ ] Verify with real Arduino crash detection
- [ ] User acceptance testing
