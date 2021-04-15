/*
 *  nrf24l01 Module functions
 *  --------
 *  This file contains all the functions relative to the radio controls in order to
 *  send and receive messages between multiple prototypes
*/

#pragma once
#include "abstract_radio.h"
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  4 // for nRF24
#define CSN_PIN 5 // for nRF24


// --------------------------------------- RF24 Module functions

class RadioRF24 {
  private: 
    RF24 * radio;
  
  public: 
    RadioRF24() {
      radio = new RF24(CE_PIN, CSN_PIN); // Create a Radio
    }

    // Note: safe deletions, you never know :)
    ~RadioRF24() { 
      if(radio) {
        delete radio;
      }
    }

    RF24 * getRadio() {
      return radio;
    }

    void init_radio() {

      radio->begin();
      switch (params.wifi_kbps) {
      case 250:
        radio->setDataRate(RF24_250KBPS);
        break;
      case 1000:
        radio->setDataRate(RF24_1MBPS);
        break;
      case 2000:
        radio->setDataRate(RF24_2MBPS);
        break;  
      default:
        radio->setDataRate(RF24_250KBPS);
        break;
      }
      // From 250kbps to 2MB, to reduce the signal length

      //this is a mesh so we don't want ACKs!
      // radio.setAutoAck(false);

      radio->openWritingPipe(params.broadcast_io_addr);
      radio->openReadingPipe(1, params.broadcast_io_addr);

      radio->setRetries(3, 5); // delay, count

    }

    int tx_data(int tx_time) {
      int start_tx_time = millis();
      int msg_count = 0;
      while (millis() - start_tx_time < tx_time) {
        radio->write(&params.my_id, sizeof(params.my_id));
        msg_count++;

        delay(random(50, params.scan_duration));
      }
      return msg_count;
    }

    int rx_data(int rx_time, LinkedList<Log> * tmp_friend_list) {

      char data_received[15];

      int start_rx_time = millis();
      int msg_count = 0;

      while (millis() - start_rx_time < rx_time) {
        if (radio->available()) {
          radio->read(&data_received, sizeof(data_received));

          // Avoiding duplicates: a * O(n) where a = RXpackets [before was O(n²)]
          int index = 0;
          for( ; index < tmp_friend_list->size(); ++index) {  
            if(strcmp(tmp_friend_list->get(index).friend_id, data_received) == 0)
              break;
          }

          if(index == tmp_friend_list->size()) { // Non listed friend are added
            tmp_friend_list->add(Log(data_received));
          }

          msg_count++;
        }
        delay(75);
      }

      return msg_count;

    }
};

// ------------------ Debugging

void printTxData(char * data) {
  Serial.print(" =====> Data sent: ");
  Serial.println(data);
}

void printRxData(char * data) {
  Serial.print(" >===== Data received: ");
  Serial.println(data);
}


// --------------------------------------- RF24 Controller

class RadioController : public AbsRadioController{
    int randomRxTime = random(RANDOM_TX_MILLS_MIN, RANDOM_TX_MILLS_MAX);
    int randomTxTime = random(RANDOM_RX_MILLS_MIN, RANDOM_RX_MILLS_MAX);

    int statsTx = 0;
    int statsRx = 0;

    RadioRF24 radioModule;

    public: 
    /**
     * Initialize radio parameters
    */
    void init() {
        Serial.println("Initializing radio functions");
        radioModule.init_radio();
    }

    /**
     * Receive messages from other devices
     * @returns tmpFriendList
    */  
    LinkedList<Log>* scan() {
        radioModule.getRadio()->startListening();
        LinkedList<Log>* tmpFriendList = new LinkedList<Log>();
        int rxCount = radioModule.rx_data(randomRxTime, tmpFriendList);

        Serial.printf("\nReceived %d messages.", rxCount);
        this->statsRx = rxCount;
        return tmpFriendList;
    }

    /**
     * Send params.my_id
    */
    void send() {
        radioModule.getRadio()->stopListening();
        Serial.printf("\nSending id: %s", params.my_id);

        int txCount = radioModule.tx_data(randomTxTime);
        this->statsTx = txCount;
        Serial.printf("\nSent %d messages.", txCount);
    }
    int getStatsTx() {
        return this->statsTx;
    }

    int getStatsRx() {
        return this->statsRx;
    }
};