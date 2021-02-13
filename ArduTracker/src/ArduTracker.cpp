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


#include "at_utils.h"
#include "at_list.h"
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

List* friendList;



void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Booting device");

    pinMode(CS_PIN, OUTPUT);
    pinMode(CE_PIN, OUTPUT);
    friendList = new List();

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

    List* tmpFriendList = radioCtrl->receive();

    friendList->printNodes();
    tmpFriendList->removeDuplicates();
    friendList->appendList(tmpFriendList);
    friendList->compactList(params.friendly_freshness);
    tmpFriendList->printNodes();

    Node* tmpFriend = tmpFriendList->first;
    while(tmpFriend) {
        if(WiFi.status() != WL_CONNECTED)
            tmpFriend->seen_millis = time(nullptr);
        
        String msg = "{\n";
        msg += "  \"my_id\": \""        + (String)params.my_id + "\",\n";
        msg += "  \"friend_id\": \""    + (String)tmpFriend->friend_id + "\",\n";
        msg += "  \"seen_millis\": \""  + (String)tmpFriend->seen_millis + "\",\n";
        msg += "  \"seen_time\": \""    + (String)tmpFriend->seen_time + "\",\n";
        msg += "}";

        Serial.print("Saving to SD . . .");
        Serial.print(msg);

        sdCrtl->saveInLog(msg);

        tmpFriend = tmpFriend->next;
    }
    tmpFriendList->deleteList();

    delay(50);

    radioCtrl->send();
    wifiCtrl->connect();
    mqttCtrl->connect();
    wifiCtrl->send();
    
    Serial.printf(
        "Sending from cache to %s with topic %s", 
        params.mqtt_server, 
        params.out_topic);

    int strIndex = 0;
    char inputStr[200];
    int inputChar;

    File cacheLogFile = SD.open(CACHE_FILE);
    inputChar = cacheLogFile.read();
    
    while(inputChar != EOF) {
        if(inputChar != '\n') {
            inputStr[strIndex] = inputChar;
            strIndex++;
            inputStr[strIndex] = '\0';
        }
        else {
            Serial.printf("Sending:\n%s", inputStr);
            client.publish(params.out_topic, inputStr);
            delay(50);
            strIndex = 0;
        }
        inputChar = cacheLogFile.read();
    }
    Serial.printf("All records have been sent to %s", params.mqtt_server);
    cacheLogFile.close();

    lastWifiSendTime = millis();
    //################# ARRIVATO QUI
}