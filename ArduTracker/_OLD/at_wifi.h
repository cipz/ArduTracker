#pragma once

#include <WiFi.h>

bool wifi_scan(String);

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
