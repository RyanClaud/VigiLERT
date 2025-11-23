/*
 * RELAY VERIFICATION TEST
 * Quick test to verify relay responds to crash detection
 * Upload this to test relay before using full code
 */

const int relayPin = 25;  // GPIO 25 (proven working)
const int ledPin = 13;    // Built-in LED for visual feedback

void setup() {
  Serial.begin(115200);
  delay(100);
  
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // Start with relay OFF
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   RELAY VERIFICATION TEST              ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ Relay Pin: GPIO 25                     ║");
  Serial.println("║ Type: ACTIVE-HIGH (HIGH=ON, LOW=OFF)   ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  Serial.println("Commands:");
  Serial.println("  ON  - Turn relay ON");
  Serial.println("  OFF - Turn relay OFF");
  Serial.println("  TEST - Simulate crash (turn OFF)");
  Serial.println("  BLINK - Blink relay 5 times");
  Serial.println("  STATUS - Show current state\n");
  
  delay(2000);
  
  // Initial test sequence
  Serial.println("Running initial test sequence...\n");
  testSequence();
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "ON") {
      turnRelayOn();
    }
    else if (cmd == "OFF") {
      turnRelayOff();
    }
    else if (cmd == "TEST" || cmd == "CRASH") {
      simulateCrash();
    }
    else if (cmd == "BLINK") {
      blinkRelay();
    }
    else if (cmd == "STATUS") {
      showStatus();
    }
    else {
      Serial.println("Unknown command. Type: ON, OFF, TEST, BLINK, or STATUS");
    }
  }
}

void turnRelayOn() {
  Serial.println("\n[COMMAND] Turning relay ON...");
  digitalWrite(relayPin, HIGH);
  digitalWrite(ledPin, HIGH);
  delay(100);
  showStatus();
}

void turnRelayOff() {
  Serial.println("\n[COMMAND] Turning relay OFF...");
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);
  delay(100);
  showStatus();
}

void simulateCrash() {
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   🚨 CRASH SIMULATION 🚨               ║");
  Serial.println("╚════════════════════════════════════════╝");
  
  Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
  Serial.println("Impact: 18.50 g | Roll: 45.2°");
  
  // Simulate the exact crash detection code
  Serial.println("\n🚨 EMERGENCY SHUTDOWN - Cutting relay power NOW!");
  digitalWrite(relayPin, LOW);  // Force relay OFF
  digitalWrite(ledPin, LOW);
  delay(100);
  
  Serial.printf("🚨 Relay GPIO %d forced to: %d (should be 0 = LOW/OFF)\n", 
                relayPin, digitalRead(relayPin));
  Serial.println("🚨 Engine power CUT - relay should be OFF!");
  
  Serial.println("\n✓ Crash simulation complete!");
  Serial.println("✓ Check if relay LED turned OFF");
  Serial.println("✓ This is exactly what happens in real crash\n");
  
  showStatus();
}

void blinkRelay() {
  Serial.println("\n[TEST] Blinking relay 5 times...");
  for (int i = 0; i < 5; i++) {
    Serial.printf("  Blink %d/5\n", i + 1);
    digitalWrite(relayPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(relayPin, LOW);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
  Serial.println("✓ Blink test complete\n");
  showStatus();
}

void showStatus() {
  int relayState = digitalRead(relayPin);
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   RELAY STATUS                         ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.printf("║ GPIO Pin: %d                           ║\n", relayPin);
  Serial.printf("║ Digital State: %s                       ║\n", 
                relayState ? "HIGH (1)" : "LOW (0) ");
  Serial.printf("║ Relay Status: %s                        ║\n", 
                relayState ? "ON      " : "OFF     ");
  Serial.printf("║ LED Status: %s                          ║\n", 
                digitalRead(ledPin) ? "ON      " : "OFF     ");
  Serial.println("╚════════════════════════════════════════╝\n");
}

void testSequence() {
  Serial.println("1. Testing relay OFF state...");
  digitalWrite(relayPin, LOW);
  delay(1000);
  Serial.printf("   GPIO %d = %d (should be 0)\n", relayPin, digitalRead(relayPin));
  Serial.println("   ✓ Relay should be OFF\n");
  
  Serial.println("2. Testing relay ON state...");
  digitalWrite(relayPin, HIGH);
  delay(1000);
  Serial.printf("   GPIO %d = %d (should be 1)\n", relayPin, digitalRead(relayPin));
  Serial.println("   ✓ Relay should be ON\n");
  
  Serial.println("3. Testing relay OFF state again...");
  digitalWrite(relayPin, LOW);
  delay(1000);
  Serial.printf("   GPIO %d = %d (should be 0)\n", relayPin, digitalRead(relayPin));
  Serial.println("   ✓ Relay should be OFF\n");
  
  Serial.println("╔════════════════════════════════════════╗");
  Serial.println("║   INITIAL TEST COMPLETE                ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ If relay LED changed during test:     ║");
  Serial.println("║   ✅ Relay is working correctly!       ║");
  Serial.println("║                                        ║");
  Serial.println("║ If relay LED did NOT change:          ║");
  Serial.println("║   ❌ Check wiring and power supply     ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  Serial.println("Ready for commands. Type TEST to simulate crash.\n");
}
