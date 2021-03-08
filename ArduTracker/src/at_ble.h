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

#define BLE_NAME "ArduTracker_ESP32"
#define SERV_UUID "e96bf93a-79b0-11eb-9439-0242ac130002" //random-generated
#define CHAR_UUID "50e88e70-79b1-11eb-9439-0242ac130002" //random-generated

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
        BLEDevice::init(BLE_NAME);
        BLEServer *pServer = BLEDevice::createServer();

        // Setup Payload attached to the advertisement with my_id
        BLEAdvertisementData pAdvertisementData = BLEAdvertisementData();
        // pAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED (to hide Standard Bluetooth Connection)
        pAdvertisementData.setServiceData(BLEUUID(SERV_UUID), params.my_id);
        pAdvertisementData.setName(BLE_NAME);

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
    int scan() {
        newFriendsCount = 0;
        int scanDuration = 5;
        BLEScanResults devices = pBLEScan->start(scanDuration); 

        for(int i=0; i<devices.getCount(); ++i){
            BLEAdvertisedDevice advertisedDevice = devices.getDevice(i);

            Serial.printf(
                "Found: %s >> %s [%s]\n",
                advertisedDevice.getName().c_str(),
                advertisedDevice.getServiceData().c_str(),
                advertisedDevice.getAddress().toString().c_str());

            if(advertisedDevice.getName() != BLE_NAME) // FIXME: the Name field is not always shown!!!
                return;

            newFriendsCount++;

            // Avoiding duplicates: a * O(n) where a = RXpackets [before was O(n²)]
            int i = 0;
            for(; i < tmpFriendList->size(); ++i) {  
                if(strcmp(tmpFriendList->get(i).friend_id, advertisedDevice.getServiceData().c_str()) == 0)
                    break;
            }

            if(i == tmpFriendList->size()) { // Non listed friend are added
                tmpFriendList->add(Log(advertisedDevice.getServiceData().c_str()));
            }
        }

        Serial.printf("\nNum. of BLE devices in range: %d\n", devices.getCount());
        pBLEScan->clearResults();
        // delay(2000);
        return newFriendsCount;
    }

    int getStatsTx() {
        return this->statsTx;
    }

    int getStatsRx() {
        return this->statsRx;
    }
};