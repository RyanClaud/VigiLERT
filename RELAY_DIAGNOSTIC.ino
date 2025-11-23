/*
  RELAY DIAGNOSTIC - Find out why relay isn't blocking engine
  
  This will help determine if:
  1. GPIO 27 is working
  2. Relay is responding to GPIO 27
  3. Relay is wired correctly (active HIGH or LOW)
  4. Relay contacts are working
*/

const int relayPin = 27;
const int ledPin = 13;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.println("\n╔════════════════════════════════════════════════════════╗");
  Serial.println("║          RELAY DIAGNOSTIC TEST                         ║");
  Serial.println("╠════════════════════════════════════════════════════════╣");
  Serial.println("║  This test will help identify relay issues            ║");
  Serial.println("║  Watch the relay and listen for clicking sounds       ║");
  Serial.println("╚════════════════════════════════════════════════════════╝\n");
  
  // Start with relay OFF
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);
  delay(1000);
  
  Serial.println("Starting diagnostic sequence...\n");
}

void loop() {
  // Test 1: Set relay LOW (should be OFF/OPEN)
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("TEST 1: Setting GPIO 27 = LOW");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);
  delay(500);
  
  int pinState = digitalRead(relayPin);
  Serial.printf("✓ GPIO 27 set to LOW\n");
  Serial.printf("✓ Reading back: %d (should be 0)\n", pinState);
  Serial.println("\n🔍 WHAT TO CHECK:");
  Serial.println("   • Relay should be OFF (no click sound)");
  Serial.println("   • Blue LED on relay should be OFF");
  Serial.println("   • Multimeter (COM to NO): OPEN circuit (infinite ohms)");
  Serial.println("   • If engine CAN start now: relay is active-LOW (inverted)");
  Serial.println("   • If engine CANNOT start now: relay is working correctly");
  
  Serial.println("\nWaiting 10 seconds... Check the relay now!");
  for (int i = 10; i > 0; i--) {
    Serial.printf("   %d seconds remaining...\n", i);
    delay(1000);
  }
  
  // Test 2: Set relay HIGH (should be ON/CLOSED)
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("TEST 2: Setting GPIO 27 = HIGH");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  digitalWrite(relayPin, HIGH);
  digitalWrite(ledPin, HIGH);
  delay(500);
  
  pinState = digitalRead(relayPin);
  Serial.printf("✓ GPIO 27 set to HIGH\n");
  Serial.printf("✓ Reading back: %d (should be 1)\n", pinState);
  Serial.println("\n🔍 WHAT TO CHECK:");
  Serial.println("   • Relay should CLICK ON");
  Serial.println("   • Blue LED on relay should be ON");
  Serial.println("   • Multimeter (COM to NO): CLOSED circuit (0 ohms)");
  Serial.println("   • If engine CAN start now: relay is active-HIGH (normal)");
  Serial.println("   • If engine CANNOT start now: relay is active-LOW (inverted)");
  
  Serial.println("\nWaiting 10 seconds... Check the relay now!");
  for (int i = 10; i > 0; i--) {
    Serial.printf("   %d seconds remaining...\n", i);
    delay(1000);
  }
  
  // Test 3: Rapid switching to verify relay is responding
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("TEST 3: Rapid ON/OFF switching (5 times)");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🔍 WHAT TO CHECK:");
  Serial.println("   • You should hear 5 clicks");
  Serial.println("   • Blue LED should blink 5 times");
  Serial.println("   • If no clicking: relay not connected or broken");
  Serial.println("");
  
  for (int i = 1; i <= 5; i++) {
    Serial.printf("   Click %d: ON\n", i);
    digitalWrite(relayPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(500);
    
    Serial.printf("   Click %d: OFF\n", i);
    digitalWrite(relayPin, LOW);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
  
  Serial.println("\n✓ Rapid switching test complete");
  
  // Summary
  Serial.println("\n╔════════════════════════════════════════════════════════╗");
  Serial.println("║                  DIAGNOSTIC SUMMARY                    ║");
  Serial.println("╠════════════════════════════════════════════════════════╣");
  Serial.println("║  Based on the tests above, determine:                 ║");
  Serial.println("║                                                        ║");
  Serial.println("║  1. Does relay click? (YES/NO)                        ║");
  Serial.println("║     NO  → Check wiring or relay is broken             ║");
  Serial.println("║     YES → Continue to step 2                          ║");
  Serial.println("║                                                        ║");
  Serial.println("║  2. When can engine start?                            ║");
  Serial.println("║     When GPIO LOW  → Relay is ACTIVE-LOW (inverted)  ║");
  Serial.println("║     When GPIO HIGH → Relay is ACTIVE-HIGH (normal)    ║");
  Serial.println("║                                                        ║");
  Serial.println("║  3. Check relay contacts with multimeter:             ║");
  Serial.println("║     GPIO LOW:  COM-NO should be OPEN (∞ ohms)        ║");
  Serial.println("║     GPIO HIGH: COM-NO should be CLOSED (0 ohms)      ║");
  Serial.println("║                                                        ║");
  Serial.println("╚════════════════════════════════════════════════════════╝\n");
  
  Serial.println("⏸️  Test paused for 30 seconds before repeating...\n");
  delay(30000);
  
  Serial.println("\n\n🔄 Restarting diagnostic sequence...\n\n");
}
