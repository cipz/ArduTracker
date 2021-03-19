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
                Serial.println(tmpFriendList->get(i).friend_id);
                Serial.println(friendList->get(j).friend_id);
                if(strcmp(tmpFriendList->get(i).friend_id, friendList->get(j).friend_id) == 0) {
                    Serial.println("ahah TROVATO!!!!!!"); // TODO: Accesso alla risorsa non funzionante
                    friendList->get(j).updateExposureSession(tmpFriendList->get(i).rssi);
                    break;
                }
            }
            // Add a new friendList
            if(j == friendList->size()) {
                friendList->add(tmpFriendList->get(i));
                Serial.println("Add a new friendList");
            }
        }

    }

    // Pre: FriendList che contiene i contatti avvenuti precedentemente (non duplicati) e i contatti nuovi avvenuti nell'ultima scansione

    // void compactList() {
    //     for (int i = 0; i < friendList->size(); ++i) {
    //         if (millis() - friendList->get(i).seen_millis > params.friendly_freshness) {
    //             friendList->remove(i);
    //         }
    //     }
    // }

    // Post: friendList che contiene i contatti aggiornati se sotto la soglia "friendly_freshness" (ossia, un contatto ha concluso la sessione di esposizione), altrimenti rimossi friendList e salvati dentro la SD Card (cache.txt) e poi inviati al server mqtt

    void static printList(LinkedList<Log>* friendList, const char * label = "[DEBUG]") {
        // DEBUG print
        // for (int i = 0; i < friendList->size(); ++i) {
        //     Serial.printf(
        //         "\n%s {FriendID = %s, SeenMillis = %d, SeenTime = %jd}\n",
        //         label,
        //         friendList->get(i).friend_id,
        //         friendList->get(i).start_millis,
        //         (intmax_t) friendList->get(i).seen_time);
        //         // intmax_t ref. https://en.cppreference.com/w/c/chrono/time_t
        // }
        
        for (int i = 0; i < friendList->size(); ++i)
            Serial.println(friendList->get(i).serialize());
        
    }
};


