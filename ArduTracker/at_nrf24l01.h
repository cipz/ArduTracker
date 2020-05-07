#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  4// for nRF24
#define CSN_PIN 5 // for nRF24

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

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

int rx_data(int rx_time) {

  // must match dataToSend in master
  char data_received[15]; 
  
  int start_rx_time = millis();
  int msg_count = 0;
  
  while (millis() - start_rx_time < rx_time) {
    if ( radio.available() ) {
      radio.read(&data_received, sizeof(data_received));
      Serial.print("Data received: ");
      Serial.println(data_received);
      friend_list->addNode(data_received);
      msg_count++;
    }
    delay(100);
  }

  Serial.println(friend_list->getTotalNodes());
//  friend_list->printNodes();
  
  return msg_count;

}
