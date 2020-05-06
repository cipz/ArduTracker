#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  4// for nRF24
#define CSN_PIN 5 // for nRF24

const byte broadcastAddress[5] = {'R', 'x', 'T', 'x', '0'};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
