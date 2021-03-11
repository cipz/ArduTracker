/*
 *  nrf24l01 Module functions
 *  --------
 *  This file contains all the functions relative to the radio controls in order to
 *  send and receive messages between multiple prototypes
*/

#pragma once
#include "abstract_radio.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
// --------------------------------------- RF24 Controller

#define BLE_NAME "ArduTracker_ESP32"
#define SERV_UUID "e96bf93a-79b0-11eb-9439-0242ac130002" //random-generated

int newFriendsCount;

/**
 *  Util: converts std::string to String
*/
String stdToString(std::string str) {
    String res = "";
    for(int i=0; i<str.size(); i++)
        res += str[i];
    return res;
}

class BLEController : public AbsRadioController{
    int randomRxTime = random(RANDOM_TX_MILLS_MIN, RANDOM_TX_MILLS_MAX);
    int randomTxTime = random(RANDOM_RX_MILLS_MIN, RANDOM_RX_MILLS_MAX);

    int statsTx = 0;
    int statsRx = 0;

    BLEScan* pBLEScan;  

    public: 
    /**
     * Initialize radio parameters
    */
    void init() override {
        Serial.println("Initializing BLE functions");

        //Start BLE Server
        BLEDevice::init(BLE_NAME);
        BLEServer *pServer = BLEDevice::createServer();

        // Setup Payload attached to the advertisement with my_id
        BLEAdvertisementData pAdvertisementData = BLEAdvertisementData();
        pAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED (to hide Standard Bluetooth Connection)
        pAdvertisementData.setServiceData(BLEUUID(SERV_UUID), params.my_id);

        // Advertise BLE Server readiness
        BLEAdvertising *pAdvertising = pServer->getAdvertising();
        pAdvertising->setAdvertisementData(pAdvertisementData);
        pAdvertising->start();
        // Initialize BLE scanning
        pBLEScan = BLEDevice::getScan();
        pBLEScan->setActiveScan(true);
    }

    /**
     * Scan for other BLE devices
     * @return the number of new friends found
    */
    LinkedList<Log>* scan() override {
        LinkedList<Log>* newFriends = new LinkedList<Log>();

        int scanDuration = 5;
        BLEScanResults devices = pBLEScan->start(scanDuration); 

        for(int i=0; i<devices.getCount(); ++i){
            BLEAdvertisedDevice advertisedDevice = devices.getDevice(i);

            Serial.printf(
                "Found: %s >> %s [%s] RSSI:%d\n",
                advertisedDevice.getName().c_str(),
                advertisedDevice.getServiceData().c_str(),
                advertisedDevice.getAddress().toString().c_str(),
                advertisedDevice.getRSSI());

            if(advertisedDevice.getName() == BLE_NAME){
                newFriends->add(Log(advertisedDevice.getServiceData().c_str()));
            }
        }

        Serial.printf("\nNum. of BLE devices in range: %d\n", devices.getCount());
        // pBLEScan->clearResults();
        // delay(2000);
        return newFriends;
    }

    void send(){
        return;
    }

    int getStatsTx() override{
        return this->statsTx;
    }

    int getStatsRx() override{
        return this->statsRx;
    }
};