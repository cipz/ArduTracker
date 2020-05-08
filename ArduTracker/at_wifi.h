#include <WiFi.h>

bool wifi_scan(String);
bool connect_wifi(const char *, const char *);

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

bool wifi_scan(String ssid) {
  byte numSsid = WiFi.scanNetworks();
  // Print the list of networks seen:
  // Serial.print("SSID List:");
  // Serial.println(numSsid);

  if (numSsid == 0)
    return false;
    
  // Print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    // Serial.println(WiFi.SSID(thisNet));
    if (WiFi.SSID(thisNet) == ssid)
      return true;
  }
  
  return false;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

bool connect_wifi(const char *ssid, const char *password) {
  if (!wifi_scan(ssid)) {
    Serial.println("Network not found.");
    return false;
  }
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  } //while
  Serial.println("");
  
  return true;

} //connect_wifi
