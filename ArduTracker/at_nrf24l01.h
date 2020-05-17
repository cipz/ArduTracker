#pragma once

#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  4// for nRF24
#define CSN_PIN 5 // for nRF24

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

void init_radio();
int tx_data(int);
int rx_data(int, List *);
void printRxData(char *);

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void init_radio() {

  radio.begin();
  radio.setDataRate(RF24_250KBPS);

  //this is a mesh so we don't want ACKs!
  // radio.setAutoAck(false);

  //  radio.openWritingPipe(params.broadcast_io_addr);
  //  radio.openReadingPipe(1, params.broadcast_io_addr);

  radio.setRetries(3, 5); // delay, count

}

int tx_data(int tx_time) {
  int start_tx_time = millis();
  int msg_count = 0;
  while (millis() - start_tx_time < tx_time) {
    radio.write(&params.my_id, sizeof(params.my_id));
    msg_count++;
    delay(100);
  }
  return msg_count;
}

int rx_data(int rx_time, List * new_friend_list) {
  // must match dataToSend in master
  char data_received[15];

  int start_rx_time = millis();
  int msg_count = 0;

  while (millis() - start_rx_time < rx_time) {
    if ( radio.available() ) {
      radio.read(&data_received, sizeof(data_received));
      printRxData(data_received);
      friend_list->appendNode(data_received);
      msg_count++;
    }
    delay(100);
  }

  return msg_count;

}

void printRxData(char * data_received) {
  Serial.print("Data received: ");
  Serial.println(data_received);
}
