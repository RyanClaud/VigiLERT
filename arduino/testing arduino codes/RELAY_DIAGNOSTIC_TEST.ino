// ═══════════════════════════════════════════════════════════════════════════
// RELAY DIAGNOSTIC TEST - Physical Key Blocking Issue
// ═══════════════════════════════════════════════════════════════════════════
// This test helps identify why physical key still works when system says "blocked"
// ═══════════════════════════════════════════════════════════════════════════

// Pin Assignments (same as your main code)
const int relayPin = 13;              // Starter relay (temporary, 3-second max)
const int ignitionRelayPin = 27;      // Ignition relay (continuous power control)

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║     RELAY DIAGNOSTIC TEST              ║");
  Serial.println("║   Physical Key Blocking Issue          ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(ignitionRelayPin, OUTPUT);
  
  Serial.println("🔧 RELAY WIRING DIAGNOSIS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("Your setup:");
  Serial.println("• GPIO 13 (Starter Relay) → Connected to starter key ignition wiring");
  Serial.println("• GPIO 27 (Ignition Relay) → Connected DIRECTLY to ignition coil");
  Serial.println("• Using 2-channel relay module (ACTIVE-LOW)");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
  
  Serial.println("🧪 STARTING RELAY TESTS...\n");
  
  runRelayDiagnostic();
}

void loop() {
  // Handle serial commands for manual testing
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "TEST" || cmd == "RUN TEST") {
      runRelayDiagnostic();
    }
    else if (cmd == "BLOCK" || cmd == "BLOCK ENGINE") {
      Serial.println("\n🔒 BLOCKING ENGINE (Both relays OFF)");
      digitalWrite(relayPin, HIGH);        // Starter OFF
      digitalWrite(ignitionRelayPin, HIGH); // Ignition OFF
      Serial.printf("Starter Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
      Serial.printf("Ignition Relay GPIO %d = %d (HIGH/OFF)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
      Serial.println("🔑 Try physical key now - should NOT start");
    }
    else if (cmd == "ALLOW" || cmd == "ALLOW ENGINE") {
      Serial.println("\n✅ ALLOWING ENGINE (Ignition relay ON)");
      digitalWrite(ignitionRelayPin, LOW);  // Ignition ON
      Serial.printf("Ignition Relay GPIO %d = %d (LOW/ON)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
      Serial.println("🔑 Try physical key now - should start");
    }
    else if (cmd == "STARTER ON") {
      Serial.println("\n⚡ STARTER ON (temporary)");
      digitalWrite(relayPin, LOW);  // Starter ON
      Serial.printf("Starter Relay GPIO %d = %d (LOW/ON)\n", relayPin, digitalRead(relayPin));
      Serial.println("⏰ Will turn off in 3 seconds...");
      delay(3000);
      digitalWrite(relayPin, HIGH);  // Starter OFF
      Serial.printf("Starter Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
      Serial.println("✅ Starter turned off");
    }
    else if (cmd == "STATUS") {
      printRelayStatus();
    }
    else if (cmd == "HELP") {
      printCommands();
    }
  }
  
  delay(100);
}

void runRelayDiagnostic() {
  Serial.println("═══════════════════════════════════════════════════════════════");
  Serial.println("🧪 RELAY DIAGNOSTIC TEST SEQUENCE");
  Serial.println("═══════════════════════════════════════════════════════════════");
  
  // Test 1: Both relays OFF (should block engine completely)
  Serial.println("\n📋 TEST 1: COMPLETE ENGINE BLOCK");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  digitalWrite(relayPin, HIGH);        // Starter OFF
  digitalWrite(ignitionRelayPin, HIGH); // Ignition OFF
  
  Serial.printf("✓ Starter Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
  Serial.printf("✓ Ignition Relay GPIO %d = %d (HIGH/OFF)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
  Serial.println("\n🔑 PHYSICAL KEY TEST:");
  Serial.println("   → Try turning physical key now");
  Serial.println("   → Engine should NOT start (completely blocked)");
  Serial.println("   → If engine starts, there's a wiring problem!");
  Serial.println("\n⏳ Waiting 15 seconds for you to test...");
  
  for (int i = 15; i > 0; i--) {
    Serial.printf("   %d seconds remaining...\n", i);
    delay(1000);
  }
  
  // Test 2: Ignition relay ON (should allow engine start)
  Serial.println("\n📋 TEST 2: IGNITION RELAY ON (ENGINE ALLOWED)");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  digitalWrite(ignitionRelayPin, LOW);  // Ignition ON
  
  Serial.printf("✓ Starter Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
  Serial.printf("✓ Ignition Relay GPIO %d = %d (LOW/ON)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
  Serial.println("\n🔑 PHYSICAL KEY TEST:");
  Serial.println("   → Try turning physical key now");
  Serial.println("   → Engine SHOULD start (ignition allowed)");
  Serial.println("   → If engine doesn't start, check ignition relay wiring!");
  Serial.println("\n⏳ Waiting 15 seconds for you to test...");
  
  for (int i = 15; i > 0; i--) {
    Serial.printf("   %d seconds remaining...\n", i);
    delay(1000);
  }
  
  // Test 3: Return to blocked state
  Serial.println("\n📋 TEST 3: RETURN TO BLOCKED STATE");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  digitalWrite(ignitionRelayPin, HIGH); // Ignition OFF
  
  Serial.printf("✓ Starter Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
  Serial.printf("✓ Ignition Relay GPIO %d = %d (HIGH/OFF)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
  Serial.println("\n🔑 PHYSICAL KEY TEST:");
  Serial.println("   → Try turning physical key now");
  Serial.println("   → Engine should NOT start (blocked again)");
  Serial.println("   → If engine still starts, ignition relay is not wired correctly!");
  Serial.println("\n⏳ Waiting 10 seconds for you to test...");
  
  for (int i = 10; i > 0; i--) {
    Serial.printf("   %d seconds remaining...\n", i);
    delay(1000);
  }
  
  Serial.println("\n═══════════════════════════════════════════════════════════════");
  Serial.println("🔍 DIAGNOSTIC RESULTS ANALYSIS");
  Serial.println("═══════════════════════════════════════════════════════════════");
  Serial.println("If physical key STILL works in TEST 1 and TEST 3:");
  Serial.println("❌ PROBLEM: Ignition relay is NOT properly wired to ignition coil");
  Serial.println("");
  Serial.println("✅ SOLUTION: Check these connections:");
  Serial.println("   1. Ignition relay COM → Ignition coil positive wire");
  Serial.println("   2. Ignition relay NO → Battery positive (12V)");
  Serial.println("   3. Ignition relay NC → Leave disconnected");
  Serial.println("   4. When relay OFF: No power to ignition coil");
  Serial.println("   5. When relay ON: Power flows to ignition coil");
  Serial.println("");
  Serial.println("🔧 WIRING CHECK:");
  Serial.println("   • Remove ignition coil fuse temporarily");
  Serial.println("   • Use multimeter to check voltage at ignition coil");
  Serial.println("   • Relay OFF: Should read 0V");
  Serial.println("   • Relay ON: Should read 12V");
  Serial.println("═══════════════════════════════════════════════════════════════");
  
  printCommands();
}

void printRelayStatus() {
  Serial.println("\n📊 CURRENT RELAY STATUS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.printf("Starter Relay (GPIO %d): %d (%s)\n", 
                relayPin, digitalRead(relayPin), 
                digitalRead(relayPin) ? "OFF" : "ON");
  Serial.printf("Ignition Relay (GPIO %d): %d (%s)\n", 
                ignitionRelayPin, digitalRead(ignitionRelayPin), 
                digitalRead(ignitionRelayPin) ? "OFF" : "ON");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  if (digitalRead(ignitionRelayPin) == HIGH) {
    Serial.println("🔒 Physical key should be BLOCKED");
  } else {
    Serial.println("✅ Physical key should be ALLOWED");
  }
}

void printCommands() {
  Serial.println("\n📋 AVAILABLE COMMANDS:");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("TEST         - Run complete diagnostic test");
  Serial.println("BLOCK        - Block engine (both relays OFF)");
  Serial.println("ALLOW        - Allow engine (ignition relay ON)");
  Serial.println("STARTER ON   - Test starter relay (3 seconds)");
  Serial.println("STATUS       - Show current relay status");
  Serial.println("HELP         - Show this help");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}