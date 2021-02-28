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

#define DEBUG_MODE 0
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

struct Params {
  // WiFi
  char ssid[50];
  char password[50];
  int wifi_send_interval;
  // nRF24L01
  char my_id[15];
  byte broadcast_io_addr[15];
  // MQTT
  char in_topic[25];
  char out_topic[25];
  char mqtt_server[32]; //FIX: Expanded to allow hostnames
  // Other
  int friendly_freshness;
};
Params params;

WiFiClient espClient;
PubSubClient client(espClient);

#include "at_log.h"
#include "at_utils.h"
#include "at_mqtt.h"
// #include "at_nrf24l01.h"
#include "at_ble.h"
#include "at_wifi.h"
#include "at_sd.h"

SDController* sdCrtl;
RadioController* radioCtrl;
WiFiContoller* wifiCtrl;
MQTTController* mqttCtrl;

LinkedList<Log> * friendList;

void setup() {

    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Booting device...");

    pinMode(ONBOARD_LED_PIN,OUTPUT);

    pinMode(CS_PIN, OUTPUT);
    // pinMode(CE_PIN, OUTPUT);
    friendList = new LinkedList<Log>();

    sdCrtl = new SDController();
    radioCtrl = new RadioController();
    wifiCtrl = new WiFiContoller();
    mqttCtrl = new MQTTController();

    sdCrtl->init();
    sdCrtl->listContent();
    sdCrtl->acquireParams();
    sdCrtl->initLog();
    delay(100);
    radioCtrl->init();
    wifiCtrl->init();
    mqttCtrl->init();

    Serial.println("Boot sequence finished correctly!");
    delay(100);
}

// --------------------------------------- Loop operations

void loop() {

    Serial.println("\n\n-- -- -- NEW LOOP CYCLE -- -- --\n");

    if(SD_DEBUG_MODE) 
        sdCrtl->listContent();

    // -------------------- Receive radio message

/*
    // PRE: the list contains all the contacts within a short period of time, without duplicates
    LinkedList<Log>* tmpFriendList = radioCtrl->receive();
    ListUtils list = ListUtils(friendList, tmpFriendList);
    if(DEBUG_MODE)
        list.printList("[DEBUG-BEFORE]");
    
    list.appendList();
    list.compactList();

    if(DEBUG_MODE)
        list.printList("[DEBUG-AFTER]");
    // POST: The list contains all the contacts above the threshold called "friendly_freshness"
   

    // -------------------- Save list to SD

    for(int i = 0; i < tmpFriendList->size(); ++i){

        Log tmpFriend = tmpFriendList->get(i);

        if(WiFi.status() != WL_CONNECTED)
            tmpFriend.seen_millis = time(nullptr);
        
        String msg = tmpFriend.serialize();

        Serial.print("Saving to SD . . . ");
        if(DEBUG_MODE)
            Serial.print(msg);
        sdCrtl->saveInLog(msg);
        Serial.print("Saved!");

    }

    tmpFriendList->clear();


    //-------------------- Send radio message

*/

    radioCtrl->send();

    if(STATS_DEBUG_MODE)
        sdCrtl->saveInStats(radioCtrl->getStatsTx(), radioCtrl->getStatsRx());


    //-------------------- Connect to WiFi
    
    wifiCtrl->connect();
    mqttCtrl->connect();
    
    Serial.printf(
        "\nSending from cache to %s with topic %s", 
        params.mqtt_server, 
        params.out_topic);

    int strIndex = 0;
    char inputStr[200];
    int inputChar;


    //-------------------- Send data to MQTT
    
    File cacheLogFile = SD.open(CACHE_FILE);
    inputChar = cacheLogFile.read();
    
    Serial.println("Sending:");

    while(inputChar != EOF) {
        if(inputChar != '\n') {
            inputStr[strIndex] = inputChar;
            strIndex++;
            inputStr[strIndex] = '\0';
        }
        else {
            Serial.println(inputStr);
            mqttCtrl->publish(params.out_topic, inputStr);
            delay(50);
            strIndex = 0;
        }
        inputChar = cacheLogFile.read();
    }

    Serial.printf("\nAll records have been sent to %s", params.mqtt_server);
    cacheLogFile.close();

    lastWifiSendTime = millis();

    //-------------------- Blink ;)

    digitalWrite(ONBOARD_LED_PIN,HIGH);
    delay(1000);
    digitalWrite(ONBOARD_LED_PIN,LOW);
}