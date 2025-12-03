# Firebase Listener Computed Ref Fix

## Problem
Dashboard was showing this critical error in console:
```
TypeError: Cannot read properties of undefined (reading 'pieceNum_')
at onChildAdded (firebase_database.js)
at initializeCrashListener (Dashboard.vue:2002:3)
```

This error prevented ALL Firebase listeners from working properly, causing:
- No crash detection alerts
- No alcohol status updates
- No live data updates
- No trip history
- Dashboard showing stale/empty data

## Root Cause

The dashboard was using computed refs incorrectly with Firebase listeners:

**WRONG:**
```javascript
const crashRef = computed(() => userId.value ? dbRef(database, `...`) : null);

// ❌ ERROR: Passing computed ref directly to onChildAdded
onChildAdded(crashRef, (snapshot) => {
  // This fails because crashRef is a computed ref, not a database ref
});
```

**The Problem:**
- `crashRef` is a **computed property** that returns a Firebase database reference
- Firebase's `onChildAdded()` and `onValue()` expect the **actual database reference**, not a computed wrapper
- Need to use `.value` to extract the actual ref from the computed property

## Solution

Add `.value` to all computed refs when passing to Firebase listeners:

**CORRECT:**
```javascript
const crashRef = computed(() => userId.value ? dbRef(database, `...`) : null);

// ✅ FIX: Check if ref exists and use .value
if (!crashRef.value) {
  console.error('[CRASH] crashRef is null, cannot set up listener');
  return;
}

onChildAdded(crashRef.value, (snapshot) => {
  // Now it works! crashRef.value is the actual database reference
});
```

## Fixed Listeners

### 1. Crash Listener
```javascript
// Before: onChildAdded(crashRef, ...)
// After:  onChildAdded(crashRef.value, ...)
```

### 2. Alcohol Listener
```javascript
// Before: onValue(alcoholRef, ...)
// After:  onValue(alcoholRef.value, ...)
```

### 3. Helmet Public Listener
```javascript
// Before: onValue(helmetPublicRef, ...)
// After:  onValue(helmetPublicRef.value, ...)
```

### 4. Helmet Status Listener
```javascript
// Before: onValue(helmetRef, ...)
// After:  onValue(helmetRef.value, ...)
```

### 5. Trips Listener
```javascript
// Before: onValue(tripsRef, ...)
// After:  onValue(tripsRef.value, ...)
```

## Why This Matters

### Computed Refs in Vue 3
```javascript
const myRef = computed(() => someValue);

// myRef is a ComputedRef object with a .value property
console.log(myRef);        // ComputedRef { ... }
console.log(myRef.value);  // The actual value
```

### Firebase Listeners
```javascript
// Firebase expects a DatabaseReference object
onValue(databaseRef, callback);

// NOT a ComputedRef wrapper
onValue(computedRef, callback);  // ❌ ERROR
onValue(computedRef.value, callback);  // ✅ WORKS
```

## Added Safety Checks

Each listener now checks if the ref exists before setting up:

```javascript
if (!crashRef.value) {
  console.error('[CRASH] crashRef is null, cannot set up listener');
  return;
}
```

This prevents errors when:
- User is not authenticated yet
- userId is not available
- Computed ref returns null

## Expected Behavior After Fix

### Console Output (Success)
```
[INIT] Setting up crash listener on path: /helmet_public/.../crashes
[CRASH] New crash event received from Firebase: {...}
[ALCOHOL] ✓ Alcohol data received
[HELMET_PUBLIC] Live data updated
[TRIPS] Trip history loaded
```

### Console Output (Before Fix)
```
TypeError: Cannot read properties of undefined (reading 'pieceNum_')
[Vue warn]: Unhandled error during execution of mounted hook
```

## Testing

1. **Refresh Dashboard** - Should load without errors
2. **Check Console** - No TypeError about 'pieceNum_'
3. **Trigger Crash** - Alert should appear
4. **Check Alcohol** - Status should update
5. **View Live Data** - Speed, location should update
6. **Check Trips** - Recent trips should load

## Files Modified
- `src/views/Dashboard.vue` - Lines 1292, 1409, 1506, 1590, 2002

## Technical Details

### Vue 3 Computed Refs
- `computed()` returns a `ComputedRef<T>` object
- Access the value with `.value` property
- Reactive and automatically updates

### Firebase Database Refs
- `dbRef()` returns a `DatabaseReference` object
- Used with `onValue()`, `onChildAdded()`, etc.
- Not compatible with Vue's reactive wrappers

### The Fix
- Extract the actual `DatabaseReference` from `ComputedRef`
- Use `.value` to unwrap the computed property
- Add null checks for safety
