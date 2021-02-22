/*
 *  Controller classes
 *  --------
 *  This file contains all classes for the core business logic of the program
*/
#pragma once
class SDController {
    SDCard* sd;

    public:
    SDController() {
        sd = new SDCard();
    }

    void init() {
        Serial.print("Setting up SD card: ");
        if(sd->init()) 
            Serial.println("OK");
        else {
            Serial.println("ERROR, rebooting in 10s");
            restart(RESTART_SECONDS);
        }
    }

    void listContent() {
        Serial.println("SD card content: ");
        sd->listFiles();
    }

    void acquireParams() {
        Serial.print("Acquiring parameters: ");
        if (sd->loadParams())
            Serial.println("OK");
        else {
            Serial.printf("\nERROR, rebooting in %d sec", RESTART_SECONDS);
            restart(RESTART_SECONDS);
        }
    }

    void initLog() {
        Serial.println("Creating log files");
        sd->initLogFiles();
        Serial.println("Log files correctly created!");
    }

    void saveInLog(String msg) {
        sd->saveInLog(msg);
    }

    void saveInStats(int tx, int rx) {
        String msg = millis() + String("\t") +
                    tx + String("\t") + 
                    rx;
        sd->saveAndAppendInStats(msg);
    }

};

class RadioController {
    int randomRxTime = random(RANDOM_TX_MILLS_MIN, RANDOM_TX_MILLS_MAX);
    int randomTxTime = random(RANDOM_RX_MILLS_MIN, RANDOM_RX_MILLS_MAX);

    int statsTx = 0;
    int statsRx = 0;
    
    public: 
    void init() {
        Serial.println("Initializing radio functions");
        init_radio();
    }

    LinkedList<Log>* receive() {
        int startRxTime = millis();
        radio.startListening();
        LinkedList<Log>* tmpFriendList = new LinkedList<Log>();
        int rxCount = rx_data(randomRxTime, tmpFriendList);

        Serial.printf("\nReceived %d messages.", rxCount);
        this->statsRx = rxCount;
        return tmpFriendList;
    }

    void send() {
        radio.stopListening();
        Serial.printf("\nSending id: %s", params.my_id);

        int startTxTime = millis();
        int txCount = tx_data(randomTxTime);
        this->statsTx = txCount;
        Serial.printf("\nSent %d messages.", txCount);
    }

    int getStatsTx() {
        return this->statsTx;
    }

    int getStatsRx() {
        return this->statsRx;
    }
};

class WiFiContoller {
    public:
    void init() {
        Serial.println("Initializing WiFi");
        WiFi.mode(WIFI_STA);
        lastWifiSendTime = 1000000000;
        wifiTransmission = true;
        // timezone is 2
        configTime(2 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    }

    void send() {
    }

    void connect() {
        if(!wifiTransmission)
            return;
        if(millis()-lastWifiSendTime <= params.wifi_send_interval)
            return;
        
        if(WiFi.status() != WL_CONNECTED) {
            Serial.println("Scanning available networks...");

            if(wifi_scan(params.ssid)) {
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

class MQTTController {
    public:
    void init() {
        Serial.println("Initializing MQTT");
        client.setServer(params.mqtt_server, 21883);
    }

    void connect() {
        if(WiFi.status() != WL_CONNECTED)
            return;
        
        int mqttConnAttempts = 0;
        
        while(!client.connected()) {
            if(mqttConnAttempts > 3) {
                Serial.print("Exceed MQTT connection tries");
                return;
            }
            Serial.print("Attempting MQTT connection...");
            if(client.connect("ESP8266Client"))
                Serial.print("MQTT connected.");
            else {
                Serial.printf("\nFailed, rc= %d");
                Serial.print("Retrying in 3s");
                delay(3000);
            }
            mqttConnAttempts++;
        }

        client.loop();
    }
};