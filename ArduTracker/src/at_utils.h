/*
 *  Utils functions
 *  --------
 *  This file contains some utilities to manage certain classes in the core logic of the software
*/

#pragma once

// --------------------------------------- Restart function

void restart(int delay_seconds) {
    delay(delay_seconds * 1000);
    ESP.restart();
}


// --------------------------------------- Utils class

class Utils {
    public:

    void static updateFriendList(LinkedList<Log>* friendList, LinkedList<Log>* tmpFriendList) {

        // O(n²) 
        for (int i = 0; i < tmpFriendList->size(); ++i) {
            int j = 0;
            // Search for an existent exposure session 
            for(; j < friendList->size(); ++j) {
                if(strcmp(tmpFriendList->get(i).friend_id, friendList->get(j).friend_id) == 0) {
                    Log updatedLog = Log(friendList->get(j)); // creates a copy of the existing log
                    updatedLog.updateExposureSession(tmpFriendList->get(i).rssi);
                    friendList->remove(j); // removes the old one
                    friendList->add(updatedLog); // insert the new one
                    break;
                }
            }
            // Add a new friendList
            if(j == friendList->size()) {
                friendList->add(tmpFriendList->get(i));
            }
        }

    }

    void static printList(LinkedList<Log>* friendList, const char * label = "[DEBUG]") {
        if(friendList->size() > 0){
            Serial.println(label);
            for (int i = 0; i < friendList->size(); ++i)
                Serial.println(friendList->get(i).serializeLocal());
        }

        
    }
};


