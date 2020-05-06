#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  4// for nRF24
#define CSN_PIN 5 // for nRF24

const byte broadcastAddress[5] = {'R', 'x', 'T', 'x', '0'};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

int tx_data(int random_tx_time) {

  int start_tx_time = millis();
  int count = 0;
  while (millis() - start_tx_time < random_tx_time) {
    radio.write(&myID, sizeof(myID));
    count++;
    delay(100);
  }
  return count;

}

int rx_data(int random_rx_time) {

  int start_rx_time = millis();
  int count = 0;
  while (millis() - start_rx_time < random_rx_time) {
    if ( radio.available() ) {
      radio.read(&dataReceived, sizeof(dataReceived));
      Serial.print("Data received: ");
      Serial.println(dataReceived);
      count++;
    }
    delay(100);
  }
  return count;

}
