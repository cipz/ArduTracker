
#include <SPI.h>
#include <ArduinoJson.h>

#include "at_utils.h"

struct Params {
  // WiFi
  const char *ssid;
  const char *password;
  // nRF24L01
  const char *my_id;
  const char *broadcast_io_addr;
  byte broadcastAddress[5];
  // MQTT
  const char *in_topic;
  const char *out_topic;
};

struct SeenRecently {

  char friendID[10];
  int seenMoment;

};

Params params;

char myID[10] = "Message 3"; // the two values to be sent to the master
char dataReceived[10]; // must match dataToSend in master

#include "at_nrf24l01.h"
#include "at_sd.h"

int random_rx_time;
int random_tx_time;
int start_rx_time;
int start_tx_time;
int rx_count;
int tx_count;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void setup() {

  Serial.begin(9600);
  Serial.println("Booting device");

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
  if (load_sd_params(params))
    Serial.println("OK");
  else {
    Serial.println("ERROR");
    // Rebooot Device
  }

  Serial.println("Creating log files");
  init_log_files();
  Serial.println("Log files correctly created!");

  delay(100);

  // Radio setup

  radio.begin();
  radio.setDataRate(RF24_250KBPS);

  //this is a mesh so we don't want ACKs!
  // radio.setAutoAck(false);

  radio.openWritingPipe(broadcastAddress);
  radio.openReadingPipe(1, broadcastAddress);

  radio.setRetries(3, 5); // delay, count

  Serial.println("Boot sequence finished correctly!");

  delay(100);

}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void loop() {

  // Generating random times
  random_tx_time = random(3000, 6000);
  random_rx_time = random(3000, 6000);

  Serial.println("\n -- -- -- -- -- -- -- -- \n");

  // Radio receiving
  start_rx_time = millis();
  radio.startListening();
  rx_count = rx_data(random_rx_time);
  Serial.print("Received ");
  Serial.print(rx_count);
  Serial.println(" messages");

  delay(100);

  Serial.println("\n -- -- -- -- -- -- -- -- \n");

  // Radio sending
  start_tx_time = millis();
  radio.stopListening();
  tx_count = tx_data(random_tx_time);
  Serial.print("Sent ");
  Serial.print(tx_count);
  Serial.println(" messages");

}
