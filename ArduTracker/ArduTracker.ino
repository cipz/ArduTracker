
#include <SPI.h>
#include <ArduinoJson.h>

#include "at_utils.h"

#include "at_list.h"
List * friend_list;

struct Params {
  // WiFi
  char ssid[50];
  char password[50];
  int wifi_send_time;
  // nRF24L01
  char my_id[15];
  byte broadcast_io_addr[15];
  // MQTT
  char in_topic[25];
  char out_topic[25];
  // Other
  int friendly_freshness;
};
Params params;

#include "at_nrf24l01.h"
#include "at_sd.h"
#include "at_wifi.h"

#define SERIAL_BAUD_RATE 9600
#define MAX_WIFI_RECON_COUNT 5

bool wifi_transmission;
int last_wifi_send_time;

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
    // delay(10000);
    // ESP.restart();
  }

  Serial.println("SD card contents: ");
  listFiles();

  Serial.print("Acquiring parameters from SD card: ");
  if (load_sd_params())
    Serial.println("OK");
  else {
    Serial.println("ERROR");
    Serial.println("Rebooting in 10 seconds");
    // delay(10000);
    // ESP.restart();
  }

  Serial.println("Creating log files");
  init_log_files();
  Serial.println("Log files correctly created!");

  delay(100);

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  // Radio setup
  init_radio();

  // TODO REMOVE, FOR TESTING WITHOUT SD CARD
  const byte broadcastAddress[5] = {'R', 'x', 'T', 'x', '0'};
  radio.openWritingPipe(broadcastAddress);
  radio.openReadingPipe(1, broadcastAddress);
  strlcpy(params.my_id, "COM3", sizeof(params.my_id));

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  // Received data list setup
  friend_list = new List();

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  // WiFi
  last_wifi_send_time = millis();

  // TODO SHOULD BE SET ACCORDING TO JSON FILE
  wifi_transmission = false;

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  Serial.println("Boot sequence finished correctly!");
  delay(100);

}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void loop() {

  Serial.println("\n\n\n\n\n- - - - - NEW LOOP CYCLE\n");

  // TODO fix code formatting / camelcase or other to all
  // TODO uncomment reboot line when working
  // TODO remove duplicates in list
  // TODO save json to sd card and test
  // TODO connect to WIFI only if variable is set
  // TODO MQTT connection

  // SD card debugging
  // Serial.println("SD card contents: ");
  // listFiles();

  // Generating random times
  int random_tx_time = random(2000, 4000);
  int random_rx_time = random(3000, 6000);

  // Serial.println("\n -- -- -- -- -- -- -- -- \n");

  // Radio receiving
  int start_rx_time = millis();
  radio.startListening();
  int rx_count = rx_data(random_rx_time);
  Serial.print("Received ");
  Serial.print(rx_count);
  Serial.println(" messages");

  if (friend_list->getTotalNodes()) {
    // friend_list->printNodes();
    friend_list->compactList(params.friendly_freshness);
  }

  // -----------------------------------

  // SAVE DATA TO SD HERE
  // Creating string to save in file

  // String current_message = "mesajio";
  //
  //  //  String current_message = "{\
  //  //\"id\" : \"" + (String)current_id + "\", \
  //  //\"air\" : \"" + (String)ppm + "\", \
  //  //" + gps + " \
  //  //\"tpc\" : \"" + out_topic + "\"\
  //  //}";
  //
  //  Serial.println("Saving to sd card... ");
  //  Serial.println(current_message);
  //
  //save_in_log(current_message);

  //  print_file("/params.json");
  //  print_file("/cache.txt");
  //  print_file("/perm.txt");

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
    if (millis() - last_wifi_send_time > params.wifi_send_time) {
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
            Serial.print("Exceeded connection tries");
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

        // SEND DATA SOMEWHERE HERE
        Serial.println("Sending data over the Internet into the future");

        // -----------------------------------

      }

      last_wifi_send_time = millis();
    }
  }
}
