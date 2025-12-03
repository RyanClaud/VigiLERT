# Physical Key Switch Wiring Guide

## Your Current Setup (Correct ✅)

```
Relay Module Connections:
┌─────────────────────┐
│   Relay Module      │
│                     │
│  S  ──────→ GPIO 13 (ESP32)
│  -  ──────→ GND (ESP32)
│  +  ──────→ 5V (External power supply)
│                     │
│  NC ──────→ (Not used)
│  COM ─────→ Ignition wire 1
│  NO  ─────→ Ignition wire 2
└─────────────────────┘
```

**How it works:**
- GPIO 13 = HIGH → Relay OFF → COM-NC connected → Engine circuit OPEN
- GPIO 13 = LOW → Relay ON → COM-NO connected → Engine circuit CLOSED

---

## Option 1: Physical Key in Series (RECOMMENDED ⭐)

**Best for safety and reliability**

### Wiring Diagram

```
Battery +12V
    │
    │
    ▼
┌─────────────┐
│ Physical    │
│ Key Switch  │  (Your motorcycle's existing key)
└─────────────┘
    │
    │ (Only when key is ON)
    ▼
┌─────────────┐
│   Relay     │
│   COM       │
└─────────────┘
    │
    │ (Only when relay is ON)
    ▼
┌─────────────┐
│   Relay     │
│   NO        │
└─────────────┘
    │
    │
    ▼
Ignition Coil / Starter
    │
    │
    ▼
Ground (-)
```

### Installation Steps

1. **Locate ignition wires** (usually behind the key switch)
   - Wire 1: From battery (hot wire)
   - Wire 2: To ignition coil/starter

2. **Cut Wire 1** (the hot wire from battery)

3. **Connect:**
   - Battery side → Physical key switch input
   - Physical key switch output → Relay COM
   - Relay NO → Ignition coil/starter (Wire 2)

### Advantages
- ✅ Physical key is master control
- ✅ Even if ESP32 fails, key controls everything
- ✅ Cannot bypass with dashboard if key is OFF
- ✅ Most secure setup
- ✅ No additional wiring to ESP32 needed

### How It Works
| Physical Key | Relay (GPIO 13) | Engine Can Start? |
|--------------|-----------------|-------------------|
| OFF          | OFF (HIGH)      | ❌ NO             |
| OFF          | ON (LOW)        | ❌ NO             |
| ON           | OFF (HIGH)      | ❌ NO             |
| ON           | ON (LOW)        | ✅ YES            |

**This is the safest option!** The physical key must be ON for the relay to work.

---

## Option 2: Monitor Physical Key with ESP32

**For automatic control based on key position**

### Wiring Diagram

```
Physical Key Switch (3 terminals):
┌─────────────────────┐
│  Terminal 1 (Input) │──→ Battery +12V
│  Terminal 2 (Output)│──→ Ignition system
│  Terminal 3 (Sense) │──→ Voltage Divider → GPIO 14
└─────────────────────┘

Voltage Divider Circuit:
Terminal 3 (12V when key ON)
    │
    ├──[ 10kΩ ]──┐
    │            │
    │            ├──→ GPIO 14 (ESP32)
    │            │
    │            └──[ 3.3kΩ ]──→ GND
    │
   GND

Output voltage to GPIO 14:
- Key OFF: 0V (LOW)
- Key ON: ~3.3V (HIGH)
```

### Why Voltage Divider?

Motorcycle key switch outputs **12V** when ON, but ESP32 GPIO can only handle **3.3V max**.

**Voltage divider formula:**
```
Vout = Vin × (R2 / (R1 + R2))
Vout = 12V × (3.3kΩ / (10kΩ + 3.3kΩ))
Vout = 12V × 0.248
Vout = 2.98V ✅ Safe for ESP32
```

### Components Needed
- 1× 10kΩ resistor (1/4W)
- 1× 3.3kΩ resistor (1/4W)
- Wire to connect to GPIO 14

### Installation Steps

1. **Find key switch terminal** that has voltage when key is ON
   - Use multimeter to test
   - Should read ~12V when key ON, 0V when key OFF

2. **Build voltage divider:**
   ```
   Key terminal → 10kΩ resistor → GPIO 14
                                    │
                                    └→ 3.3kΩ resistor → GND
   ```

3. **Test with multimeter:**
   - Key OFF: GPIO 14 should read 0V
   - Key ON: GPIO 14 should read 2.5-3.3V

4. **Upload code** (already done)

### Code Configuration

The code is already set for this option (HIGH = key ON):

```cpp
// Key turned ON (HIGH when key provides voltage)
if (currentKeyState == HIGH && !engineRunning) {
  Serial.println("\n🔑 PHYSICAL KEY TURNED ON!");
  startEngine();
}
```

### Advantages
- ✅ ESP32 knows when key is turned
- ✅ Can auto-start engine when key turned
- ✅ Can log key usage
- ✅ Dashboard shows key status

### Disadvantages
- ⚠️ More complex wiring
- ⚠️ Requires voltage divider
- ⚠️ If ESP32 fails, key monitoring stops (but relay still works)

---

## Option 3: Simple Switch to GND (Easiest)

**For testing or simple setups**

### Wiring Diagram

```
┌─────────────────┐
│ Toggle Switch   │
│                 │
│  Terminal 1 ────→ GPIO 14
│  Terminal 2 ────→ GND
└─────────────────┘

When switch CLOSED: GPIO 14 = LOW (pulled to GND)
When switch OPEN:   GPIO 14 = HIGH (internal pullup)
```

### Installation Steps

1. Connect one wire from GPIO 14 to switch terminal 1
2. Connect another wire from GND to switch terminal 2
3. Change code to use OPTION 2 (uncomment the section)

### Code Configuration

Uncomment this section in the code:

```cpp
// ✅ OPTION 2: If using switch to GND (uncomment below, comment above)
// Key turned ON (LOW because of INPUT_PULLUP)
if (currentKeyState == LOW && !engineRunning) {
  Serial.println("\n🔑 PHYSICAL KEY TURNED ON!");
  startEngine();
}
// Key turned OFF
else if (currentKeyState == HIGH && engineRunning) {
  Serial.println("\n🔑 PHYSICAL KEY TURNED OFF!");
  stopEngine();
}
```

### Advantages
- ✅ Very simple wiring
- ✅ No voltage divider needed
- ✅ Safe voltage levels (3.3V max)

### Disadvantages
- ⚠️ Requires separate switch (not using motorcycle's key)
- ⚠️ Less integrated

---

## Recommended Setup for Your Motorcycle

Based on your current relay wiring, I recommend:

### **Option 1: Physical Key in Series** ⭐

**Why:**
1. You already have the relay wired correctly
2. No additional ESP32 wiring needed
3. Most secure - key is master control
4. Simplest to implement
5. Dashboard button still works when key is ON

**How to do it:**
1. Find the wire from battery to ignition
2. Cut it
3. Connect: Battery → Key → Relay COM → Relay NO → Ignition
4. Done! No code changes needed

**Result:**
- Physical key OFF → Engine cannot start (even with dashboard)
- Physical key ON + Dashboard button → Engine starts
- Physical key ON + Dashboard button → Engine stops
- Physical key OFF → Engine stops immediately

---

## Testing

### Test 1: Relay Control (Current Setup)
1. Upload code
2. Open Serial Monitor
3. Type `START ENGINE`
4. Should see: `✅ Relay GPIO 13 = 0 (ON)`
5. Engine should start

### Test 2: Physical Key (Option 1)
1. Turn physical key OFF
2. Try dashboard button
3. Engine should NOT start (key blocks it)
4. Turn physical key ON
5. Try dashboard button
6. Engine should start

### Test 3: Physical Key Monitoring (Option 2)
1. Turn physical key ON
2. Serial Monitor should show: `🔑 PHYSICAL KEY TURNED ON!`
3. Engine should start automatically
4. Turn physical key OFF
5. Serial Monitor should show: `🔑 PHYSICAL KEY TURNED OFF!`
6. Engine should stop

---

## Troubleshooting

### Relay not clicking
- Check GPIO 13 connection
- Check 5V external power to relay
- Check GND connection
- Measure voltage: GPIO 13 should be 0V when ON, 3.3V when OFF

### Physical key monitoring not working (Option 2)
- Check voltage divider resistor values
- Measure voltage at GPIO 14: should be 0V (key OFF) or 2.5-3.3V (key ON)
- Check for loose connections
- Verify key switch terminal has 12V when ON

### Engine starts even when key is OFF (Option 1)
- Check wiring: Key must be in SERIES before relay
- Verify key switch is actually cutting power
- Check for bypass wires

---

## Safety Notes

⚠️ **IMPORTANT:**
1. Always disconnect battery before wiring
2. Use proper wire gauge (14-16 AWG for ignition)
3. Insulate all connections with heat shrink
4. Test with multimeter before connecting to ESP32
5. Never connect 12V directly to ESP32 GPIO
6. Double-check polarity before powering on

---

## Summary

**Your current relay wiring is perfect!** ✅

**Next step:** Choose your option:
- **Option 1** (Recommended): Wire physical key in series - No ESP32 changes needed
- **Option 2**: Add voltage divider to GPIO 14 - ESP32 monitors key
- **Option 3**: Add simple switch to GPIO 14 - For testing

The code is ready for all options. Just choose your wiring method!
