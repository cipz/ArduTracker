/*
 *  nrf24l01 Module functions
 *  --------
 *  This file contains all the functions relative to the radio controls in order to
 *  send and receive messages between multiple prototypes
*/

#pragma once
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  4 // for nRF24
#define CSN_PIN 5 // for nRF24

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

void init_radio();
int tx_data(int);
int rx_data(int, LinkedList<Log> *);
void printTxData(char *);
void printRxData(char *);

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void init_radio() {

  radio.begin();
  radio.setDataRate(RF24_250KBPS);

  //this is a mesh so we don't want ACKs!
  // radio.setAutoAck(false);

  if (!DEBUG_MODE) {
    radio.openWritingPipe(params.broadcast_io_addr);
    radio.openReadingPipe(1, params.broadcast_io_addr);
  }

  radio.setRetries(3, 5); // delay, count

}

int tx_data(int tx_time) {
  int start_tx_time = millis();
  int msg_count = 0;
  while (millis() - start_tx_time < tx_time) {
    radio.write(&params.my_id, sizeof(params.my_id));
    msg_count++;
    
    int randomDelay = random(50, 200);
    delay(randomDelay);

    // delay(100); // TODO: change this delay to random
  }
  return msg_count;
}

int rx_data(int rx_time, LinkedList<Log> * tmp_friend_list) {

  char data_received[15];

  int start_rx_time = millis();
  int msg_count = 0;

  while (millis() - start_rx_time < rx_time) {
    if (radio.available()) {
      radio.read(&data_received, sizeof(data_received));
      // ============

      // Avoiding duplicates: x * O(n) [before was O(n²)]
      int index = 0;
      for( ; index < tmp_friend_list->size(); ++index) {  
        if(strcmp(tmp_friend_list->get(index).friend_id, data_received) == 0)
          break;
      }

      if(index == tmp_friend_list->size()) { // Non listed friend, gets added
        tmp_friend_list->add(Log(data_received));
      }

      // ============
      msg_count++;
    }
    delay(75); // FIXME: change this to be random or not?
  }

  return msg_count;

}

// ------------------ Debugging

void printTxData(char * data) {
  Serial.print(" =====> Data sent: ");
  Serial.println(data);
}

void printRxData(char * data) {
  Serial.print(" >===== Data received: ");
  Serial.println(data);
}
