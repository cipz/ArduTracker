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


// --------------------------------------- List Manager

class ListUtils {
    private:
    LinkedList<Log>* friendList;
    LinkedList<Log>* tmpFriendList;

    public:
    ListUtils(LinkedList<Log>* fl, LinkedList<Log>* tmpFl): friendList(fl), tmpFriendList(tmpFl) {}

    void appendList() {
        for (int i = 0; i < tmpFriendList->size(); ++i) {
            friendList->add(tmpFriendList->get(i));
        }
    }

    void compactList() {
        for (int i = 0; i < friendList->size(); ++i) {
            if (millis() - friendList->get(i).seen_millis > params.friendly_freshness) {
                friendList->remove(i);
            }
        }
    }

    void printList(const char * label = "[DEBUG]") {
        // DEBUG print
        for (int i = 0; i < friendList->size(); ++i) {
            Serial.printf(
                "\n%s {FriendID = %s, SeenMillis = %d, SeenTime = %jd}\n",
                label,
                friendList->get(i).friend_id,
                friendList->get(i).seen_millis,
                (intmax_t) friendList->get(i).seen_time);
                // intmax_t ref. https://en.cppreference.com/w/c/chrono/time_t
        }
    }
};


