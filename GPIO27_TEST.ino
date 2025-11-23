/*
GPIO 27 Test - Verify GPIO 27 can control the relay
Watch the relay LED (DS1) and listen for clicking
*/

const int relayPin = 27;

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  
  Serial.println("\n=== GPIO 27 RELAY TEST ===");
  Serial.println("Relay should toggle every 2 seconds");
  Serial.println("Watch DS1 LED and listen for clicks");
  Serial.println("===========================\n");
}

void loop() {
  // Test 1: Set HIGH
  Serial.println("Setting GPIO 27 = HIGH");
  digitalWrite(relayPin, HIGH);
  delay(2000);
  
  // Test 2: Set LOW  
  Serial.println("Setting GPIO 27 = LOW");
  digitalWrite(relayPin, LOW);
  delay(2000);
  
  Serial.println("---");
}
