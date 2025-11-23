/*
 * GPIO SCANNER TEST
 * Tests multiple GPIO pins to find one that works with your relay
 * This will help identify if GPIO 25 is the problem
 */

// List of safe GPIO pins to test
const int testPins[] = {23, 32, 33, 19, 18, 5, 17, 16, 4, 2, 15, 25, 26, 27};
const int numPins = sizeof(testPins) / sizeof(testPins[0]);

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   GPIO SCANNER TEST                    ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ Testing multiple GPIO pins             ║");
  Serial.println("║ Watch which one makes relay respond    ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  Serial.println("This will test each GPIO pin:");
  for (int i = 0; i < numPins; i++) {
    Serial.printf("  GPIO %d\n", testPins[i]);
  }
  Serial.println("\nStarting test in 3 seconds...\n");
  delay(3000);
}

void loop() {
  for (int i = 0; i < numPins; i++) {
    int pin = testPins[i];
    
    Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.printf("Testing GPIO %d\n", pin);
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    
    // Setup pin
    pinMode(pin, OUTPUT);
    
    // Test HIGH
    Serial.printf("\n[1/4] Setting GPIO %d to HIGH...\n", pin);
    digitalWrite(pin, HIGH);
    delay(500);
    int readHigh = digitalRead(pin);
    Serial.printf("      Read back: %d\n", readHigh);
    Serial.println("      👀 Check relay LEDs now!");
    delay(2000);
    
    // Test LOW
    Serial.printf("\n[2/4] Setting GPIO %d to LOW...\n", pin);
    digitalWrite(pin, LOW);
    delay(500);
    int readLow = digitalRead(pin);
    Serial.printf("      Read back: %d\n", readLow);
    Serial.println("      👀 Check relay LEDs now!");
    delay(2000);
    
    // Test HIGH again
    Serial.printf("\n[3/4] Setting GPIO %d to HIGH again...\n", pin);
    digitalWrite(pin, HIGH);
    delay(500);
    Serial.println("      👀 Check relay LEDs now!");
    delay(2000);
    
    // Test LOW again
    Serial.printf("\n[4/4] Setting GPIO %d to LOW again...\n", pin);
    digitalWrite(pin, LOW);
    delay(500);
    Serial.println("      👀 Check relay LEDs now!");
    delay(2000);
    
    // Results
    Serial.println("\n📊 RESULTS:");
    if (readHigh == 1 && readLow == 0) {
      Serial.printf("✅ GPIO %d works correctly (can toggle HIGH/LOW)\n", pin);
      Serial.println("   Did the relay LEDs change?");
      Serial.println("   - If YES: This GPIO works with your relay! ✅");
      Serial.println("   - If NO: Try next GPIO or check wiring");
    } else {
      Serial.printf("❌ GPIO %d may not work properly\n", pin);
      Serial.printf("   Expected: HIGH=1, LOW=0\n");
      Serial.printf("   Got: HIGH=%d, LOW=%d\n", readHigh, readLow);
    }
    
    // Reset pin
    digitalWrite(pin, LOW);
    pinMode(pin, INPUT);
    
    Serial.println("\nMoving to next GPIO in 3 seconds...");
    delay(3000);
  }
  
  Serial.println("\n\n╔════════════════════════════════════════╗");
  Serial.println("║   TEST COMPLETE                        ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ Which GPIO made the relay respond?    ║");
  Serial.println("║ Use that GPIO in your main code!      ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  Serial.println("Restarting test in 10 seconds...\n");
  delay(10000);
}
