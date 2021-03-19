/*
 *  ARDUTRACKER BOARD (2020 - 2021)
 *  -----------------
 *  Wireless Networks for mobile applications, UniPD
 *  --------
 *  Created by Ciprian Voinea
 *  Edited by Federico Carboni and Mariano Sciacco
*/

// --------------------------------------- Static config

#define RANDOM_TX_MILLS_MIN 2000
#define RANDOM_TX_MILLS_MAX 4000
#define RANDOM_RX_MILLS_MIN 3000
#define RANDOM_RX_MILLS_MAX 6000
#define SERIAL_BAUD_RATE 9600
#define MAX_WIFI_RECON_COUNT 5
#define RESTART_SECONDS 10
#define ONBOARD_LED_PIN 2

// Debug modes

#define DEBUG_MODE 1
#define SD_DEBUG_MODE 0
#define STATS_DEBUG_MODE 1

// --------------------------------------- Initialization

#include <SPI.h>
#include <PubSubClient.h>
#include <time.h>
#include <ArduinoJson.h>
#include <LinkedList.h>
#include <WiFi.h>

bool wifiTransmission;
int lastWifiSendTime;
int sendDataCount;

struct Params {
  // WiFi
  char ssid[50];
  char password[50];
//   int wifi_send_interval;
  // nRF24L01
  char my_id[15];
  byte broadcast_io_addr[15];
  // MQTT
  char in_topic[32];
  char out_topic[32];
  char mqtt_server[32]; //FIX: Expanded to allow hostnames
  // Other
  int friendly_freshness;
  // Radio mode
  char radio_mode[15];
  int send_data_cycles;
};
Params params;

WiFiClient espClient;
PubSubClient client(espClient);

#include "at_log.h"
#include "at_utils.h"
#include "abstract_radio.h"
#include "at_nrf24l01.h"
#include "at_ble.h"
#include "at_wifi.h"
#include "at_sd.h"

SDController* sdCrtl;
#include "at_mqtt.h"

AbsRadioController* radioCtrl;
WiFiContoller* wifiCtrl;
MQTTController* mqttCtrl;

LinkedList<Log>* friendList;

void setup() {

    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Booting device...");

    pinMode(ONBOARD_LED_PIN,OUTPUT);

    pinMode(CS_PIN, OUTPUT);
    pinMode(CE_PIN, OUTPUT);

    friendList = new LinkedList<Log>();

    sdCrtl = new SDController();
    wifiCtrl = new WiFiContoller();
    mqttCtrl = new MQTTController();

    sdCrtl->init();
    sdCrtl->listContent();
    sdCrtl->acquireParams();
    sdCrtl->initLog();
    delay(100);

    // Mode switcher 
    if(strcmp(params.radio_mode, "NRF24") == 0)
        radioCtrl = new RadioController();
    else if(strcmp(params.radio_mode, "BLE") == 0)
        radioCtrl = new BLEController();
    else {
        Serial.print("Radio mode not found! Rebooting");
        restart(10); // FIXME: Restart
    }

    radioCtrl->init();
    wifiCtrl->init();
    mqttCtrl->init();
    sendDataCount = 0;

    Serial.println("Boot sequence finished correctly!");
    delay(100);
}

// --------------------------------------- Loop operations
void loop() {

    Serial.println("\n\n-- -- -- NEW LOOP CYCLE -- -- --\n");

    if(SD_DEBUG_MODE) 
        sdCrtl->listContent();

    //-------------------- Connect to WiFi and reach new Configuration
    
    wifiCtrl->connect();
    mqttCtrl->connect();
    
    if(WiFi.isConnected() && !mqttCtrl->isSubscribed()) {
        // Subscribe to params configuration topic
        String configTopic = params.in_topic + WiFi.macAddress();
        mqttCtrl->subscribe(configTopic.c_str(), sdCrtl);
        Serial.printf(
            "\nSubscribed to: %s [status:%d]", 
            configTopic.c_str(), 
            client.connected());
    }


    //-------------------- Send data to MQTT

    if(WiFi.isConnected() && sendDataCount >= params.send_data_cycles) {
        
        Serial.printf(
            "\nSending from CACHE to %s with topic %s", 
            params.mqtt_server, 
            params.out_topic);

        int strIndex = 0;
        char inputStr[500]; //FIXME !!
        int inputChar;

        File cacheLogFile = SD.open(CACHE_FILE);
        inputChar = cacheLogFile.read();
        
        Serial.println("\nSending:");

        while(inputChar != EOF) {
            if(inputChar != '\n') {
                inputStr[strIndex] = inputChar;
                strIndex++;
                inputStr[strIndex] = '\0';
            }
            else {
                Serial.println(inputStr);
                mqttCtrl->publish(params.out_topic, inputStr);
                sdCrtl->initLog(); // clear cache
                delay(50);
                strIndex = 0;
            }
            inputChar = cacheLogFile.read();
        }

        Serial.printf("\nAll records have been sent to %s", params.mqtt_server);
        cacheLogFile.close();
        sendDataCount = 0;
    }
    else {
        sendDataCount++;
    }

    // lastWifiSendTime = millis();

    // -------------------- Receive radio message
    LinkedList<Log>* tmpFriendList = radioCtrl->scan();

    if(DEBUG_MODE)
        Utils::printList(friendList, "[DEBUG-BEFORE]");
    
    Utils::updateFriendList(friendList, tmpFriendList);

    if(DEBUG_MODE)
        Utils::printList(friendList, "[DEBUG-AFTER]");
   
    // -------------------- Save list to SD // FIXME:OK

    Serial.print("Saving to SD . . . ");

    for(int i = 0; i < friendList->size(); ++i){
        String msg = friendList->get(i).serialize();

        if(DEBUG_MODE)
            Serial.print(msg);

        sdCrtl->saveInLog(msg);
    }
    
    Serial.print("Saved!");

    tmpFriendList->clear();

    //-------------------- Send radio message

    radioCtrl->send();

    if(STATS_DEBUG_MODE)
        sdCrtl->saveInStats(radioCtrl->getStatsTx(), radioCtrl->getStatsRx());

// Yee we reach the end without crashing, yet :D
}