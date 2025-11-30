#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi Config
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Config
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";

// Sensor
const int mq3AnalogPin = 34;
const int alcoholThreshold = 600;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   MINIMAL TEST VERSION                 ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  pinMode(mq3AnalogPin, INPUT);
  
  // Connect WiFi
  WiFi.begin(ssid, password);
  Serial.print("[WIFI] Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[WIFI] ✓ Connected!");
  Serial.printf("[WIFI] IP: %s\n\n", WiFi.localIP().toString().c_str());
}

void loop() {
  // Read sensor
  int alcoholValue = analogRead(mq3AnalogPin);
  bool isDanger = alcoholValue > alcoholThreshold;
  String status = isDanger ? "Danger" : "Safe";
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.printf("[ALCOHOL] Value: %d | Status: %s\n", alcoholValue, status.c_str());
  
  // Send to Firebase - SINGLE PATH TEST
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/alcohol/status.json";
  
  Serial.printf("[FIREBASE] URL: %s\n", url.c_str());
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(5000);
  
  // Create JSON
  StaticJsonDocument<128> doc;
  doc["sensorValue"] = alcoholValue;
  doc["status"] = status;
  doc["timestamp"] = millis();
  
  String payload;
  serializeJson(doc, payload);
  
  Serial.printf("[FIREBASE] Payload: %s\n", payload.c_str());
  
  // Send
  int httpCode = http.PUT(payload);
  
  Serial.printf("[FIREBASE] HTTP Code: %d ", httpCode);
  
  if (httpCode == 200) {
    Serial.println("✓ SUCCESS!");
  } else if (httpCode == 401) {
    Serial.println("✗ AUTH ERROR - Check Firebase rules");
  } else if (httpCode == 404) {
    Serial.println("✗ NOT FOUND - Check URL");
  } else if (httpCode < 0) {
    Serial.println("✗ CONNECTION ERROR - Check WiFi");
  } else {
    Serial.printf("✗ ERROR\n");
  }
  
  // Show response
  String response = http.getString();
  Serial.printf("[FIREBASE] Response: %s\n", response.c_str());
  
  http.end();
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
  
  delay(2000);  // Update every 2 seconds
}
