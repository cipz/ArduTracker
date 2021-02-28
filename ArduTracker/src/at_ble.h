/*
 *  nrf24l01 Module functions
 *  --------
 *  This file contains all the functions relative to the radio controls in order to
 *  send and receive messages between multiple prototypes
*/

#pragma once
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
// --------------------------------------- RF24 Controller

#define SERV_UUID "e96bf93a-79b0-11eb-9439-0242ac130002" //random-generated
#define CHAR_UUID "50e88e70-79b1-11eb-9439-0242ac130002" //random-generated

/**
 *  Util: converts std::string to String
*/
String stdToString(std::string str) {
    String res = "";
    for(int i=0; i<str.size(); i++)
        res += str[i];
    return res;
}

/**
 * Callback class: called on Characteristic value changed
*/
class BLECallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
        String value = stdToString(pCharacteristic->getValue());
        Serial.printf("\nReceived value: %s", value);
    }
};

class RadioController {
    int randomRxTime = random(RANDOM_TX_MILLS_MIN, RANDOM_TX_MILLS_MAX);
    int randomTxTime = random(RANDOM_RX_MILLS_MIN, RANDOM_RX_MILLS_MAX);

    int statsTx = 0;
    int statsRx = 0;

    BLECharacteristic* pCharacteristic;

    public: 
    /**
     * Initialize radio parameters
    */
    void init() {
        Serial.println("Initializing BLE functions");

        //Start BLE Server and set Characteristic
        BLEDevice::init("ArduTracker_ESP32");
        BLEServer *pServer = BLEDevice::createServer();
        BLEService *pService = pServer->createService(SERV_UUID);
        pCharacteristic = pService->createCharacteristic(
                                                CHAR_UUID,
                                                BLECharacteristic::PROPERTY_READ |
                                                BLECharacteristic::PROPERTY_WRITE
                                            );

        pCharacteristic->setCallbacks(new BLECallback());
        pService->start();

        // Advertise BLE Server readiness
        BLEAdvertising *pAdvertising = pServer->getAdvertising();
        pAdvertising->start();
    }

    /**
     * Send params.my_id
    */
    void send() {
        Serial.printf("\nSending id: %s", params.my_id);
        pCharacteristic->setValue(params.my_id);
    }

    int getStatsTx() {
        return this->statsTx;
    }

    int getStatsRx() {
        return this->statsRx;
    }
};