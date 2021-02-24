/*
 *  Wifi module functions
 *  --------
 *  This file contains all the functions relative to WIFI connection
*/

#pragma once

// --------------------------------------- WIFI Controller class

class WiFiContoller {
    public:
    /** 
     * Initialize parameters
    */
    void init() {
        Serial.println("Initializing WiFi");
        WiFi.mode(WIFI_STA);
        lastWifiSendTime = 1000000000;
        wifiTransmission = true;
        // timezone is 2
        configTime(2 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    }

    /**
     * Scan WiFi networks available 
    */
    bool wifiScan(String ssid) {
      int ssidCount = WiFi.scanNetworks(); //old numSsid

      if (ssidCount == 0)
        return false;

      // For each network found
      for (int i = 0; i < ssidCount; i++) {
        if (WiFi.SSID(i) == ssid)
          return true;
      }
      return false;
    }

    /** 
     * Establish connection 
    */
    void connect() {
        if(!wifiTransmission)
            return;
        if(millis()-lastWifiSendTime <= params.wifi_send_interval)
            return;
        
        if(WiFi.status() != WL_CONNECTED) {
            Serial.println("Scanning available networks...");

            if(wifiScan(params.ssid)) {
                Serial.println("Attempting WiFi connection");
                int reconCount = 0;

                while(WiFi.status() != WL_CONNECTED) {
                    if(reconCount > MAX_WIFI_RECON_COUNT) {
                        Serial.println("Exceeded WiFi connection tries");
                        return;
                    }
                    Serial.print(".");
                    WiFi.begin(params.ssid, params.password);
                                       
                    reconCount++;
                    delay(50);
                }

                Serial.println("WiFi connected successfully");
                Serial.println("IP address " + WiFi.localIP());
            }
            else  //!wifi_scan(params.ssid)
                Serial.printf("\n%s not in range", params.ssid);
        }
        else //WiFi.status() == WL_CONNECTED
            Serial.println("WiFi already connected");
    }
};
