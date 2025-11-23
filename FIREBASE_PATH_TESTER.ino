/*
Firebase Path Tester - Find the correct alcohol detection path
Tests multiple possible Firebase paths to find where alcohol data is stored
*/

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "DPWH";
const char* password = "12345678900";
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== FIREBASE PATH TESTER ===");
  Serial.println("Testing multiple Firebase paths to find alcohol data...\n");
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
  
  // Test different possible paths
  String testPaths[] = {
    "/" + userUID + "/alcohol.json",
    "/" + userUID + "/alcohol/status.json", 
    "/" + userUID + "/alcohol/status/status.json",
    "/" + userUID + "/alcohol/sensorValue.json",
    "/helmet_public/" + userUID + "/helmetStatus.json",
    "/helmet_public/" + userUID + "/helmetStatus/alcoholDetected.json",
    "/helmet_public/" + userUID + "/alcohol.json",
    "/" + userUID + ".json"  // Get entire user data
  };
  
  int numPaths = sizeof(testPaths) / sizeof(testPaths[0]);
  
  for (int i = 0; i < numPaths; i++) {
    Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.printf("Testing Path %d/%d: %s\n", i+1, numPaths, testPaths[i].c_str());
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    
    testFirebasePath(testPaths[i]);
    delay(1000);
  }
  
  Serial.println("\n🎯 ANALYSIS COMPLETE!");
  Serial.println("Look for paths that return alcohol-related data.");
  Serial.println("The correct path should show 'Safe', 'Danger', or similar status.");
}

void loop() {
  // Do nothing
}

void testFirebasePath(String path) {
  HTTPClient http;
  String fullURL = firebaseHost + path + "?auth=" + firebaseAuth;
  
  Serial.println("Full URL: " + fullURL);
  
  http.begin(fullURL);
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.GET();
  
  Serial.printf("HTTP Code: %d\n", httpCode);
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    Serial.println("Response Length: " + String(response.length()));
    
    if (response.length() > 0 && response != "null") {
      Serial.println("✅ SUCCESS - Data Found!");
      Serial.println("Raw Response:");
      Serial.println("─────────────────────────────────────────────────────────────");
      Serial.println(response);
      Serial.println("─────────────────────────────────────────────────────────────");
      
      // Check for alcohol-related keywords
      if (response.indexOf("alcohol") != -1 || 
          response.indexOf("Alcohol") != -1 ||
          response.indexOf("Safe") != -1 ||
          response.indexOf("Danger") != -1 ||
          response.indexOf("status") != -1) {
        Serial.println("🎯 POTENTIAL ALCOHOL DATA FOUND!");
      }
    } else {
      Serial.println("❌ Empty or null response");
    }
  } else {
    Serial.printf("❌ HTTP Error: %d\n", httpCode);
    String errorResponse = http.getString();
    if (errorResponse.length() > 0) {
      Serial.println("Error Details: " + errorResponse);
    }
  }
  
  http.end();
}