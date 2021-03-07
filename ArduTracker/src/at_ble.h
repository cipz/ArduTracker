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
 * Callback called foreach devices found
*/
class BLEScanCallback: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        // TODO: check if the ble device is esp32 and if it has the ID field
        // FIXME: maybe move this in the main class
        Serial.printf("\nDevice: %s \n", advertisedDevice.toString().c_str());
        Serial.printf("Payload: %s \n", advertisedDevice.getServiceData().c_str());
    }
};

class RadioController {
    int randomRxTime = random(RANDOM_TX_MILLS_MIN, RANDOM_TX_MILLS_MAX);
    int randomTxTime = random(RANDOM_RX_MILLS_MIN, RANDOM_RX_MILLS_MAX);

    int statsTx = 0;
    int statsRx = 0;

    BLEScan* pBLEScan;  

    public: 
    /**
     * Initialize radio parameters
    */
    void init() {
        Serial.println("Initializing BLE functions");

        //Start BLE Server
        BLEDevice::init("ArduTracker_ESP32");
        BLEServer *pServer = BLEDevice::createServer();

        // Setup Payload attached to the advertisement with my_id
        BLEAdvertisementData pAdvertisementData = BLEAdvertisementData();
        // pAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED (to hide Standard Bluetooth Connection)
        pAdvertisementData.setServiceData(BLEUUID(SERV_UUID), params.my_id);

        // Advertise BLE Server readiness
        BLEAdvertising *pAdvertising = pServer->getAdvertising();
        pAdvertising->setAdvertisementData(pAdvertisementData);
        pAdvertising->start();

        // Initialize BLE scanning
        pBLEScan = BLEDevice::getScan();
        pBLEScan->setAdvertisedDeviceCallbacks(new BLEScanCallback);
        pBLEScan->setActiveScan(true);
    }

    /**
     * Scan for other BLE devices
    */
    void scan() {
        int scanDuration = 5;
        BLEScanResults found = pBLEScan->start(scanDuration); 
        Serial.printf("\nFound  %d devices", found.getCount());
        pBLEScan->clearResults();
        delay(2000);
    }

    int getStatsTx() {
        return this->statsTx;
    }

    int getStatsRx() {
        return this->statsRx;
    }
};