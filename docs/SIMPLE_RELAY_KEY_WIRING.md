# Simple Relay + Physical Key Wiring

## Your Current Problem

You said: "I can switch on and off the engine but it does not reflect on the relay"

This means your physical key is wired DIRECTLY to the ignition, bypassing the relay completely.

## The Solution

Wire the physical key IN SERIES with the relay so BOTH must be ON for engine to start.

---

## Step-by-Step Wiring

### Current Motorcycle Wiring (Before Modification)

```
Battery (+12V)
    │
    ├─→ Physical Key Switch
    │        │
    │        └─→ Ignition Coil / Starter
    │
Ground (-)
```

### Modified Wiring (With Relay Control)

```
Battery (+12V)
    │
    ├─→ Physical Key Switch
    │        │
    │        └─→ Relay COM (Common terminal)
    │                 │
    │                 └─→ Relay NO (Normally Open)
    │                          │
    │                          └─→ Ignition Coil / Starter
    │
Ground (-)

Relay Control:
- S (Signal) → GPIO 13 (ESP32)
- - (GND)    → GND
- + (VCC)    → 5V External
```

---

## Installation Instructions

### Step 1: Identify the Wires

Find these two wires on your motorcycle:
1. **Hot wire** - From battery to key switch (usually RED or has +12V)
2. **Ignition wire** - From key switch to ignition coil (has +12V only when key is ON)

Use a multimeter:
- Key OFF: Hot wire = 12V, Ignition wire = 0V
- Key ON: Hot wire = 12V, Ignition wire = 12V

### Step 2: Disconnect the Ignition Wire

1. Turn key OFF
2. Disconnect battery (safety first!)
3. Find the wire coming OUT of the key switch that goes to ignition
4. Disconnect or cut this wire

### Step 3: Connect to Relay

Now you have:
- Wire A: From key switch (has power when key is ON)
- Wire B: To ignition coil (needs power to start engine)

Connect them through the relay:

```
Wire A (from key) → Relay COM
Relay NO → Wire B (to ignition)
Relay NC → Leave empty (not used)
```

### Step 4: Test

1. Reconnect battery
2. Turn physical key ON
3. Use dashboard button or Serial command "START ENGINE"
4. Engine should start!

---

## How It Works

| Physical Key | Relay (GPIO 13) | Relay State | Engine Can Start? |
|--------------|-----------------|-------------|-------------------|
| OFF          | HIGH (OFF)      | COM-NC      | ❌ NO (no power from key) |
| OFF          | LOW (ON)        | COM-NO      | ❌ NO (no power from key) |
| ON           | HIGH (OFF)      | COM-NC      | ❌ NO (relay blocks) |
| ON           | LOW (ON)        | COM-NO      | ✅ YES (both allow power) |

**Both must be ON for engine to start!**

---

## Visual Diagram

```
┌─────────────────────────────────────────────────────────┐
│                                                         │
│  Battery +12V                                           │
│      │                                                  │
│      │                                                  │
│      ▼                                                  │
│  ┌─────────────┐                                        │
│  │  Physical   │                                        │
│  │  Key Switch │  ← You turn this ON/OFF manually      │
│  └─────────────┘                                        │
│      │                                                  │
│      │ (Power flows only when key is ON)               │
│      ▼                                                  │
│  ┌─────────────┐                                        │
│  │   Relay     │                                        │
│  │   COM       │  ← Connect key output here            │
│  └─────────────┘                                        │
│      │                                                  │
│      │ (Power flows only when relay is ON)             │
│      ▼                                                  │
│  ┌─────────────┐                                        │
│  │   Relay     │                                        │
│  │   NO        │  ← Connect ignition wire here         │
│  └─────────────┘                                        │
│      │                                                  │
│      │                                                  │
│      ▼                                                  │
│  Ignition Coil / Starter                                │
│      │                                                  │
│      ▼                                                  │
│  Ground (-)                                             │
│                                                         │
└─────────────────────────────────────────────────────────┘

Relay Module Control:
┌─────────────────┐
│  S  → GPIO 13   │  (ESP32 controls relay)
│  -  → GND       │
│  +  → 5V Ext    │
└─────────────────┘
```

---

## Testing Procedure

### Test 1: Physical Key OFF, Dashboard Button ON
1. Turn physical key OFF
2. Click "Turn On Engine" on dashboard
3. **Expected:** Engine does NOT start (key blocks power)
4. **Serial Monitor:** "✅ Starting engine..." but relay clicks, no ignition

### Test 2: Physical Key ON, Dashboard Button ON
1. Turn physical key ON
2. Click "Turn On Engine" on dashboard
3. **Expected:** Engine STARTS (both allow power)
4. **Serial Monitor:** "✅ Starting engine..." and "✅ Relay GPIO 13 = 0 (ON)"

### Test 3: Physical Key ON, Dashboard Button OFF
1. Turn physical key ON
2. Dashboard shows "Turn On Engine" (engine is OFF)
3. **Expected:** Engine does NOT start (relay blocks)
4. **Serial Monitor:** "[STATUS] Engine:STOP | Relay:OFF"

### Test 4: Engine Running, Turn Key OFF
1. Engine is running (key ON, relay ON)
2. Turn physical key OFF
3. **Expected:** Engine STOPS immediately (key cuts power)
4. This is your emergency kill switch!

---

## Advantages of This Setup

✅ **Physical key is master control**
- Even if ESP32 fails, key controls everything
- Cannot bypass with dashboard if key is OFF

✅ **Dashboard control still works**
- When key is ON, dashboard can start/stop engine
- Remote control capability maintained

✅ **Emergency stop**
- Turn key OFF → Engine stops immediately
- No need to wait for dashboard/WiFi

✅ **Security**
- Thief cannot start engine with dashboard (needs physical key)
- Anti-theft system still works

✅ **Simple wiring**
- No additional GPIO needed
- No voltage divider required
- Just reroute one wire through relay

---

## Troubleshooting

### Engine starts even when key is OFF
**Problem:** Key is not in series with relay
**Solution:** Check wiring - key output must go to Relay COM, not directly to ignition

### Engine doesn't start even when key is ON and dashboard button pressed
**Problem:** Relay not switching or wrong connection
**Solution:** 
1. Check relay clicks when dashboard button pressed
2. Verify COM-NO connection (not COM-NC)
3. Test relay with multimeter

### Relay clicks but engine doesn't start
**Problem:** Wiring to ignition is incorrect
**Solution:**
1. Verify Relay NO connects to ignition coil
2. Check ignition wire is the correct one (test with multimeter)
3. Ensure good connections (no loose wires)

---

## Safety Notes

⚠️ **IMPORTANT:**
1. **Always disconnect battery before wiring**
2. **Use proper wire gauge** (14-16 AWG for ignition circuits)
3. **Insulate all connections** with heat shrink tubing
4. **Test with multimeter** before connecting
5. **Double-check polarity** before powering on
6. **Keep relay away from heat/water**

---

## Summary

**What you need to do:**

1. Find the wire from key switch to ignition
2. Cut/disconnect it
3. Connect: Key output → Relay COM
4. Connect: Relay NO → Ignition
5. Test!

**Result:**
- Physical key controls power flow
- Relay controls circuit completion
- Both must be ON for engine to start
- Dashboard button works when key is ON
- Emergency stop by turning key OFF

**No code changes needed** - the relay control code is already perfect!

The issue is just the wiring - once you wire the key in series with the relay, everything will work as expected.
