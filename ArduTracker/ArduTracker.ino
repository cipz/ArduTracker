
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

#include "at_nrf24l01.h"
#include "at_sd.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void setup() {

     pinMode(CS_PIN, OUTPUT);
     
  Serial.begin(9600);
  Serial.println("Booting device");

  SPI.begin();
  
  Serial.print("Setting up SD card: ");
  if (init_sd())
    Serial.println("OK");
  else {
    Serial.println("ERROR");
    // Rebooot Device
  }

  Serial.println("Creating log files");
  init_log_files();
  Serial.println("Log files correctly created!");

  Serial.print("Acquiring parameters from SD card: ");
  if (load_sd_params(params))
    Serial.println("OK");
  else {
    Serial.println("ERROR");
    // Rebooot Device
  }



  listFiles();

  delay(100);

  radio.begin();
  radio.setDataRate(RF24_250KBPS);

  //this is a mesh so we don't want ACKs!
  // radio.setAutoAck(false);

  radio.openWritingPipe(broadcastAddress);
  radio.openReadingPipe(1, broadcastAddress);

  radio.setRetries(3, 5); // delay, count
  radio.startListening();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void loop() {

  // Random delay
  delay(1000);

  radio.stopListening();

  // Send
  sendData();

  radio.startListening();

  // Random delay
  delay(1000);

  Serial.println(SD.exists("/params.json"));

  for (int i = 0; i < 10; i++)
    // Receive
    getData();
}

void sendData() {

  radio.write(&myID, sizeof(myID));
  Serial.println("Sent my ID");

}

void getData() {

  if ( radio.available() ) {
    radio.read(&dataReceived, sizeof(dataReceived));
    Serial.print("Data received: ");
    Serial.println(dataReceived);
  }
}
