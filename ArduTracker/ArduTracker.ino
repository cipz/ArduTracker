
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

int last_wifi_send_time;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void setup() {

  Serial.begin(9600);
  Serial.println("Booting device");

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  // SD setup
  Serial.print("Setting up SD card: ");
  if (init_sd())
    Serial.println("OK");
  else {
    Serial.println("ERROR");
    // Rebooot Device
  }

  Serial.println("SD card contents: ");
  listFiles();

  Serial.print("Acquiring parameters from SD card: ");
  if (load_sd_params())
    Serial.println("OK");
  else {
    Serial.println("ERROR");
    // Rebooot Device
  }

  Serial.println("Creating log files");
  init_log_files();
  Serial.println("Log files correctly created!");

  delay(100);

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  // Radio setup
  radio.begin();
  radio.setDataRate(RF24_250KBPS);

  //this is a mesh so we don't want ACKs!
  // radio.setAutoAck(false);

  radio.openWritingPipe(params.broadcast_io_addr);
  radio.openReadingPipe(1, params.broadcast_io_addr);

  radio.setRetries(3, 5); // delay, count

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  // Received data list setup
  friend_list = new List();

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  // WiFi
  last_wifi_send_time = millis();

  //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

  Serial.println("Boot sequence finished correctly!");
  delay(100);

}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void loop() {

  // Generating random times
  int random_tx_time = random(3000, 6000);
  int random_rx_time = random(3000, 6000);

  Serial.println("\n -- -- -- -- -- -- -- -- \n");

  // Radio receiving
  int start_rx_time = millis();
  radio.startListening();
  int rx_count = rx_data(random_rx_time);
  Serial.print("Received ");
  Serial.print(rx_count);
  Serial.println(" messages");

  delay(100);

  Serial.println("\n -- -- -- -- -- -- -- -- \n");

  // Radio sending
  int start_tx_time = millis();
  radio.stopListening();
  int tx_count = tx_data(random_tx_time);
  Serial.print("Sent ");
  Serial.print(tx_count);
  Serial.println(" messages");

  Serial.println("\n -- -- -- -- -- -- -- -- \n");

  if (friend_list->getTotalNodes()) {
    friend_list->printNodes();
    friend_list->compactList(params.friendly_freshness);
  }

  //  // Sending messages over WiFi
  //  if (millis() - last_wifi_send_time > params.wifi_send_time) {
  //
  //    Serial.println("Scanning available networks...");
  //    wifi_scan(params.ssid);
  //
  //    Serial.println("Setting up WiFi connection.");
  //    if (WiFi.status() != WL_CONNECTED){
  //      Serial.println("Connection attempt: ");
  //      if (!connect_wifi(params.ssid, params.password))
  //        Serial.print("UN");
  //      Serial.println("SUCCESSFULL");
  //    }
  //
  //    Serial.println("WiFi connected successfully");
  //    Serial.println("IP address: ");
  //    Serial.println(WiFi.localIP());
  //  }
}
