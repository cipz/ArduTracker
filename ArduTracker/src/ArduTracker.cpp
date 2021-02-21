#define SERIAL_BAUD_RATE 9600
#define DEBUG_MODE 0
#define MAX_WIFI_RECON_COUNT 5
#define RESTART_SECONDS 10
#define RANDOM_TX_MILLS_MIN 2000
#define RANDOM_TX_MILLS_MAX 4000
#define RANDOM_RX_MILLS_MIN 3000
#define RANDOM_RX_MILLS_MAX 6000

#include <SPI.h>
#include <PubSubClient.h>
#include <time.h>
#include <ArduinoJson.h>
#include <LinkedList.h>

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

#include "at_log.h"
#include "at_utils.h"
#include "at_nrf24l01.h"
#include "at_wifi.h"
#include "at_sd.h"
WiFiClient espClient;
PubSubClient client(espClient);
#include "at_controllers.h"

SDController* sdCrtl;
RadioController* radioCtrl;
WiFiContoller* wifiCtrl;
MQTTController* mqttCtrl;

LinkedList<Log> * friendList;

void setup() {

    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Booting device...");

    pinMode(CS_PIN, OUTPUT);
    pinMode(CE_PIN, OUTPUT);
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

void loop() {

    Serial.println("\n\n-- -- -- NEW LOOP CYCLE -- -- --\n");

    if(DEBUG_MODE) 
        sdCrtl->listContent();

    // -------------------- Receive radio message

    // PRE: the list contains all the contacts within a short period of time, without duplicates

    LinkedList<Log>* tmpFriendList = radioCtrl->receive();

    ListUtils list = ListUtils(friendList, tmpFriendList);
    list.printList("[DEBUG-BEFORE]");
    list.appendList();
    list.compactList();
    list.printList("[DEBUG-AFTER]");

    // tmpFriendList->clear();

    // POST: The list contains all the contacts above the threshold called "friendly_freshness"
   

    // -------------------- Save list to SD
    // TODO: move the logic to SD Controller

    for(int i = 0; i < tmpFriendList->size(); ++i){

        Log tmpFriend = tmpFriendList->get(i);

        if(WiFi.status() != WL_CONNECTED)
            tmpFriend.seen_millis = time(nullptr);
        

        // TODO: create a serializer in the Log class
        String msg = "{\n";
        msg += "  \"my_id\": \""        + (String)params.my_id + "\",\n";
        msg += "  \"friend_id\": \""    + (String)tmpFriend.friend_id + "\",\n";
        msg += "  \"seen_millis\": \""  + (String)tmpFriend.seen_millis + "\",\n";
        msg += "  \"seen_time\": \""    + (String)tmpFriend.seen_time + "\",\n";
        msg += "}";

        Serial.print("Saving to SD . . . ");
        Serial.print(msg);
        sdCrtl->saveInLog(msg);
        Serial.print("Saved!");

    }
    // NOTE: This temporary list will be cleared after being saved
    tmpFriendList->clear();


    //-------------------- Random delay befor sending (?)
    /*int randomDelay = random(RANDOM_TX_MILLS_MIN, RANDOM_TX_MILLS_MAX);
    Serial.printf("\nRandom delay before sending = %d millis", randomDelay);
    delay(randomDelay);
    */
   // FIXME: the delay must be done in the at_nrf24l01 class

    //-------------------- Send radio message

    radioCtrl->send();

    sdCrtl->saveInStats(radioCtrl->getStatsTx(), radioCtrl->getStatsRx()); // Save stats to SD Card

    wifiCtrl->connect();
    mqttCtrl->connect();
    wifiCtrl->send();
    
    Serial.printf(
        "\nSending from cache to %s with topic %s", 
        params.mqtt_server, 
        params.out_topic);

    int strIndex = 0;
    char inputStr[200];
    int inputChar;


    //-------------------- Send to mqtt
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
            client.publish(params.out_topic, inputStr);
            delay(50);
            strIndex = 0;
        }
        inputChar = cacheLogFile.read();
    }
    Serial.printf("\nAll records have been sent to %s", params.mqtt_server);
    cacheLogFile.close();

    lastWifiSendTime = millis();
}