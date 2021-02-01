
#define SERIAL_BAUD_RATE 9600
#define DEBUG_MODE 0
#define MAX_WIFI_RECON_COUNT 5
#define RESTART_SECONDS 10
#define RANDOM_TX_MILLS_MIN 2000
#define RANDOM_TX_MILLS_MAX 4000
#define RANDOM_RX_MILLS_MIN 3000
#define RANDOM_RX_MILLS_MAX 6000

#include <SPI.h>
#include <ArduinoJson.h>

#include <PubSubClient.h>
#include <time.h>

#include "at_utils.h"
#include "at_list.h"
List * friend_list;

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
  char mqtt_server[32]; // Expanding from 16 to 32 also for hostnames, not only ipv4 addresses
  // Other
  int friendly_freshness;
};
Params params;

#include "at_nrf24l01.h"
#include "at_sd.h"
#include "at_wifi.h"

bool wifi_transmission;
int last_wifi_send_time;

WiFiClient espClient;
PubSubClient client(espClient);

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void setup() {

  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("Booting device");

  pinMode(CS_PIN, OUTPUT);
  pinMode(CE_PIN, OUTPUT);

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  // SD setup
  Serial.print("Setting up SD card: ");

  if (init_sd())
    Serial.println("OK");
  else {
    Serial.println("ERROR");
    Serial.println("Rebooting in 10 seconds");
    restart(RESTART_SECONDS);
  }

  Serial.println("SD card contents: ");
  listFiles();

  Serial.print("Acquiring parameters from SD card: ");
  if (load_sd_params())
    Serial.println("OK");
  else {
    Serial.println("ERROR");
    Serial.println("Rebooting in 10 seconds");
    restart(RESTART_SECONDS);
  }

  Serial.println("Creating log files");
  init_log_files();
  Serial.println("Log files correctly created!");

  delay(100);

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  // Radio setup
  Serial.println("Initializing radio functions");
  init_radio();

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  // Received data list setup
  friend_list = new List();

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  // WiFi + time
  Serial.println("Initializing WiFi");
  WiFi.mode(WIFI_STA);
  last_wifi_send_time = 1000000000;
  wifi_transmission = true;
  // timezone is 2
  configTime(2 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  // MQTT
  Serial.println("Initializing MQTT");
  client.setServer(params.mqtt_server, 21883); // default 1883, for this project 21883

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  if (DEBUG_MODE) {

    Serial.println("Initializing DEBUG_MODE variables");

    // Radio
    const byte broadcastAddress[5] = {'R', 'x', 'T', 'x', '0'};
    radio.openWritingPipe(broadcastAddress);
    radio.openReadingPipe(1, broadcastAddress);
    strlcpy(params.my_id, "IRON MAN", sizeof(params.my_id));
    params.friendly_freshness = 5000;

    // WiFi
    strlcpy(params.ssid, "NETGEAR66", sizeof(params.ssid));
    strlcpy(params.password, "elatedowl630", sizeof(params.password));
    wifi_transmission = false;

    // MQTT
    strlcpy(params.mqtt_server, "192.168.0.13", sizeof(params.mqtt_server));

  }

  Serial.println("Boot sequence finished correctly!");
  delay(100);

}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void loop() {

  Serial.println("\n\n\n\n\n- - - - - NEW LOOP CYCLE\n");

  if (DEBUG_MODE) {
    Serial.println("SD card contents: ");
    listFiles();
  }

  // Generating random times
  int random_tx_time = random(RANDOM_TX_MILLS_MIN, RANDOM_TX_MILLS_MAX);
  int random_rx_time = random(RANDOM_RX_MILLS_MIN, RANDOM_RX_MILLS_MAX);

  // Serial.println("\n -- -- -- -- -- -- -- -- \n");

  // Radio receiving
  int start_rx_time = millis();
  radio.startListening();
  List * tmp_friend_list = new List();
  int rx_count = rx_data(random_rx_time, tmp_friend_list);

  Serial.print("Received ");
  Serial.print(rx_count);
  Serial.println(" messages");

  // -----------------------------------

  // Saving files to sd card
  Serial.println("friend_list->printNodes();");
  friend_list->printNodes();

  Serial.println("tmp_friend_list->printNodes();");
  tmp_friend_list->printNodes();

  Serial.println("removing duplicates from tmp_friend_list");
  tmp_friend_list->removeDuplicates();
  
  friend_list->appendList(tmp_friend_list);
  friend_list->compactList(params.friendly_freshness);

  Serial.println("tmp_friend_list->printNodes();");
  tmp_friend_list->printNodes();
  Node * tmp_friend = tmp_friend_list->first;
  while (tmp_friend) {

    if (WiFi.status() != WL_CONNECTED)
      tmp_friend->seen_time = time(nullptr);

    // Creating string to save in file
    String current_message = "{\
\"my_id\" : \"" + (String)params.my_id + "\", \
\"friend_id\" : \"" + (String)tmp_friend->friend_id + "\", \
\"seen_millis\" : \"" + (String)tmp_friend->seen_millis + "\", \
\"seen_time\" : \"" + (String)tmp_friend->seen_time + "\" \
}";

    Serial.println("Saving to sd card... ");
    Serial.println(current_message);
    save_in_log(current_message);

    Serial.println(tmp_friend->seen_millis);
    tmp_friend = tmp_friend->next;

  }

  // Removing tmp_list
  tmp_friend_list->deleteList();

  // -----------------------------------

  delay(50);

  Serial.println("\n -- -- -- -- -- -- -- -- \n");

  // Radio sending
  int start_tx_time = millis();
  radio.stopListening();
  Serial.print("Sending : '");
  Serial.print(params.my_id);
  Serial.println("'");
  int tx_count = tx_data(random_tx_time);
  Serial.print("Sent ");
  Serial.print(tx_count);
  Serial.println(" messages");

  Serial.println("\n -- -- -- -- -- -- -- -- \n");

  if (wifi_transmission) {
    // Sending messages over WiFi
    if (millis() - last_wifi_send_time > params.wifi_send_interval) {
      if (WiFi.status() != WL_CONNECTED) {

        Serial.println("Scanning available networks...");
        if (wifi_scan(params.ssid)) {

          Serial.println("Setting up WiFi connection.");
          int wifi_recon_count = 0;
          Serial.print("Attempting connection");
          while ((WiFi.status() != WL_CONNECTED) and (wifi_recon_count < MAX_WIFI_RECON_COUNT)) {
            Serial.print(".");
            WiFi.begin(params.ssid, params.password);
            wifi_recon_count++;
            delay(50);
          }

          if (wifi_recon_count < MAX_WIFI_RECON_COUNT) {
            Serial.println("WiFi connected successfully");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
          } else {
            Serial.println("Exceeded connection tries");
          }

        } else {
          Serial.print(params.ssid);
          Serial.println(" not in range.");
        }
      } else {
        Serial.println("WiFi already connected");
      }

      if (WiFi.status() != WL_CONNECTED) {
        Serial.print("Error connecting to ");
        Serial.println(params.ssid);

      } else {

        Serial.print("Connected to ");
        Serial.println(params.ssid);

        // -----------------------------------

        int mqtt_conn_attempts = 0;
        while ((!client.connected()) and (mqtt_conn_attempts < 3)) {
          Serial.print("Attempting MQTT connection...");
          // Attempt to connect
          if (client.connect("ESP8266Client")) {
            Serial.println("connected");
          } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 3 seconds");
            delay(3000);
          }
          mqtt_conn_attempts++;
        }

        client.loop();

        Serial.print("Sending all the contents of the cache file to ");
        Serial.print(params.mqtt_server);
        Serial.print(" with topic ");
        Serial.println(params.out_topic);

        // yep, its working
        // client.publish(params.out_topic, "TRANSMISSION TEST");

        int stringIndex = 0;
        char inputString[200];
        int inputChar;

        File cache_log_file = SD.open("/cache.txt");
        inputChar = cache_log_file.read();

        while (inputChar != -1) {
          if (inputChar != '\n') {
            inputString[stringIndex] = inputChar;
            stringIndex++;
            inputString[stringIndex] = '\0';
          } else {
            Serial.print("[Sending] : ");
            Serial.println(inputString);
            client.publish(params.out_topic, inputString);
            delay(50);
            stringIndex = 0;
          }
          inputChar = cache_log_file.read();
        }
        Serial.print("All records have been sent to ");
        Serial.println(params.mqtt_server);

        cache_log_file.close();

        // Recreating log file
        SD.remove("/cache.txt");
        cache_log_file = SD.open("/cache.txt", FILE_WRITE);
        cache_log_file.close();


        // -----------------------------------

      }
      last_wifi_send_time = millis();
    }
  }
}
