# 🔌 Relay Connection Options: Key Switch vs Ignition Coil

## 🎯 Two Main Options

### Option 1: Key Ignition Switch (RECOMMENDED ✅)
Connect relay to the wire coming FROM the key switch

### Option 2: Ignition Coil Circuit
Connect relay to the wire going TO the ignition coil

---

## ⚖️ Comparison

| Feature | Key Ignition Switch | Ignition Coil |
|---------|---------------------|---------------|
| **Difficulty** | ✅ Easy | ⚠️ Moderate |
| **Safety** | ✅ Very Safe | ⚠️ Moderate |
| **Effectiveness** | ✅ Cuts all power | ⚠️ Only ignition |
| **Reversibility** | ✅ Easy to undo | ⚠️ Harder to undo |
| **Risk of damage** | ✅ Very low | ⚠️ Higher |
| **Current load** | ✅ Low (safe for relay) | ⚠️ Can be high |
| **Recommended** | ✅ **YES** | ⚠️ Only if necessary |

---

## ✅ OPTION 1: Key Ignition Switch (RECOMMENDED)

### How It Works:
```
Battery +12V → Key Switch → [RELAY HERE] → ECU/Ignition System
```

When relay is OFF, it's like the key is turned OFF - nothing works.

### Advantages:
1. ✅ **Easiest to install** - Key switch is accessible
2. ✅ **Safest** - Low current, won't damage relay
3. ✅ **Most effective** - Cuts power to entire ignition system
4. ✅ **Easy to reverse** - Simple to remove if needed
5. ✅ **No risk to engine** - Just like turning key off
6. ✅ **Works on all motorcycles** - Universal solution

### Disadvantages:
1. ⚠️ Might need to access behind ignition switch
2. ⚠️ Need to identify correct wire (usually red or red/black)

### Wiring Diagram:
```
┌─────────────────────────────────────────────────┐
│                                                 │
│  Battery +12V                                   │
│      │                                          │
│      ↓                                          │
│  Key Switch                                     │
│      │                                          │
│      ├─── Accessories (lights, horn, etc.)     │
│      │                                          │
│      └─── Ignition Wire (RED or RED/BLACK)     │
│              │                                  │
│              ↓                                  │
│          [CUT HERE]                             │
│              │                                  │
│         Relay COM ←─── From key switch         │
│              │                                  │
│         Relay NO  ───→ To ECU/Ignition         │
│                                                 │
│  When Relay OFF: Circuit OPEN (engine blocked) │
│  When Relay ON:  Circuit CLOSED (engine works) │
│                                                 │
└─────────────────────────────────────────────────┘
```

### How to Find the Wire:
1. **Locate key switch** (usually behind the ignition)
2. **Look for wire bundle** coming from key switch
3. **Common wire colors:**
   - Red or Red/Black (ignition power)
   - Black (ground)
   - Brown/Yellow (accessories)
4. **Test with multimeter:**
   - Key OFF: 0V
   - Key ON: 12V
   - This is your ignition wire!

### Installation Steps:
```
1. Turn OFF motorcycle and remove key
2. Disconnect battery (safety first!)
3. Access key switch wiring (remove panels if needed)
4. Identify ignition power wire (red or red/black)
5. Test with multimeter (0V off, 12V on)
6. Cut the ignition wire
7. Connect one end to Relay COM
8. Connect other end to Relay NO
9. Insulate connections with heat shrink
10. Reconnect battery
11. Test!
```

---

## ⚠️ OPTION 2: Ignition Coil Circuit

### How It Works:
```
ECU → Ignition Coil → [RELAY HERE] → Spark Plug
```

When relay is OFF, no spark = engine won't start.

### Advantages:
1. ✅ Direct control of ignition
2. ✅ Works even if key switch is bypassed

### Disadvantages:
1. ❌ **Higher current** - Can damage relay over time
2. ❌ **More complex** - Harder to find correct wire
3. ❌ **Risk of damage** - Wrong wire can damage ECU
4. ❌ **Less effective** - Fuel pump might still run
5. ❌ **Harder to access** - Coil is often buried in engine
6. ❌ **Motorcycle-specific** - Different for each model

### When to Use:
- Only if key switch is not accessible
- If motorcycle has electronic key system
- If you need to allow accessories but block engine

---

## 🎯 RECOMMENDATION: Use Key Ignition Switch

### Why Key Switch is Better:

**1. Safety First**
- Low current (typically 1-2A)
- Your relay is rated for 10A - plenty of headroom
- No risk of damaging relay or motorcycle

**2. Simplicity**
- Easy to find (behind key switch)
- Easy to test (multimeter)
- Easy to install (accessible location)

**3. Effectiveness**
- Cuts power to ENTIRE ignition system
- ECU, fuel pump, ignition coil all OFF
- Just like turning key OFF

**4. Reversibility**
- Easy to remove if needed
- No permanent modifications
- Can restore to original easily

---

## 🔧 Step-by-Step: Key Switch Installation

### Tools Needed:
- Multimeter
- Wire cutters/strippers
- Heat shrink tubing
- Electrical tape
- Screwdrivers (to remove panels)

### Step 1: Locate Key Switch Wiring
```
1. Remove side panels or headlight cowl
2. Follow wires from key switch
3. You should see 3-5 wires:
   - Black (ground)
   - Red or Red/Black (ignition power) ← THIS ONE
   - Brown/Yellow (accessories)
   - Others (lights, signals)
```

### Step 2: Identify Ignition Wire
```
1. Set multimeter to DC voltage (20V range)
2. Turn key OFF
3. Test each wire to ground
4. Turn key ON (ignition position)
5. Find wire that goes from 0V to 12V
6. This is your ignition power wire!
```

### Step 3: Test Before Cutting
```
1. With key ON, note voltage (should be 12V)
2. Temporarily disconnect this wire
3. Try to start motorcycle
4. If engine doesn't start → CORRECT WIRE! ✅
5. If engine still starts → WRONG WIRE, try another
```

### Step 4: Install Relay
```
1. Disconnect battery
2. Cut the ignition wire
3. Strip both ends (5mm)
4. Connect:
   - Wire from key switch → Relay COM
   - Wire to ignition system → Relay NO
5. Use crimp connectors or solder
6. Cover with heat shrink tubing
7. Secure wires with zip ties
```

### Step 5: Test
```
1. Reconnect battery
2. Turn key ON
3. Send SMS: "1234 START"
4. Try to start motorcycle → Should work ✅
5. Send SMS: "1234 STOP"
6. Try to start motorcycle → Should NOT work ✅
```

---

## 🚨 Common Mistakes to Avoid

### ❌ DON'T:
1. **Cut the starter motor wire** - Too much current, will damage relay
2. **Cut the battery main wire** - Dangerous, can cause fire
3. **Cut accessory wires** - Won't stop engine
4. **Forget to disconnect battery** - Risk of short circuit
5. **Use wrong relay contacts** - Use NO (Normally Open), not NC

### ✅ DO:
1. **Test with multimeter first** - Verify correct wire
2. **Disconnect battery** - Safety first
3. **Use proper connectors** - No twisted wires
4. **Insulate all connections** - Prevent shorts
5. **Test thoroughly** - Before riding

---

## 📊 Wire Identification Guide

### Common Motorcycle Wire Colors:

| Wire Color | Function | Use for Relay? |
|------------|----------|----------------|
| Red or Red/Black | Ignition power | ✅ **YES** |
| Black | Ground | ❌ NO |
| Brown/Yellow | Accessories | ❌ NO |
| Green/White | Starter motor | ❌ NO (too much current) |
| Blue/White | Headlight | ❌ NO |
| Orange | Turn signals | ❌ NO |

**Note:** Colors vary by manufacturer! Always test with multimeter.

---

## 🎯 Final Recommendation

### Use Key Ignition Switch Because:

1. ✅ **Safer** - Low current, won't damage relay
2. ✅ **Easier** - More accessible than ignition coil
3. ✅ **More effective** - Cuts all power, not just spark
4. ✅ **Universal** - Works on all motorcycles
5. ✅ **Reversible** - Easy to remove if needed

### Installation Priority:
```
1st Choice: Key ignition switch wire ← RECOMMENDED
2nd Choice: ECU power supply wire
3rd Choice: Fuel pump relay circuit
4th Choice: Ignition coil circuit
Last Resort: Starter motor circuit (NOT RECOMMENDED)
```

---

## 💡 Pro Tips

### Tip 1: Use Relay Tester
Before connecting to motorcycle:
- Test relay with LED and battery
- Verify it switches correctly
- Check for clicking sound

### Tip 2: Add Fuse
```
Battery → [5A Fuse] → Key Switch → Relay → Ignition
```
Protects against shorts and overcurrent.

### Tip 3: Label Everything
- Label wires with tape
- Take photos before cutting
- Document wire colors
- Makes troubleshooting easier

### Tip 4: Keep Original Connectors
- Don't cut off original connectors
- Use crimp connectors to extend
- Makes it reversible

### Tip 5: Test in Safe Location
- Test in garage or driveway
- Have backup transportation
- Don't test on the road

---

## 🎉 Summary

**BEST OPTION: Connect relay to key ignition switch wire**

**Why:**
- ✅ Easiest to install
- ✅ Safest for relay and motorcycle
- ✅ Most effective (cuts all power)
- ✅ Works on all motorcycles
- ✅ Easy to reverse

**How:**
1. Find red/red-black wire from key switch
2. Test: 0V when OFF, 12V when ON
3. Cut wire and connect through relay COM/NO
4. Test with SMS commands
5. Done!

**Result:**
- Relay OFF = Like key is OFF = Engine blocked ✅
- Relay ON = Like key is ON = Engine works ✅

This is the **safest, easiest, and most effective** way to control your motorcycle engine with the relay!
