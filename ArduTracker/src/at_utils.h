#pragma once

void restart(int delay_seconds) {
    if (!DEBUG_MODE) {
        delay(delay_seconds * 1000);
        ESP.restart();
    }
}

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

    void printList(String label = "[DEBUG]") {
        // DEBUG print
        for (int i = 0; i < friendList->size(); ++i) {
            Serial.printf(
                "\n%s{FriendID = %s, SeenMillis = %d, SeenTime = %d}\n",
                label,
                friendList->get(i).friend_id,
                friendList->get(i).seen_millis,
                friendList->get(i).seen_time);
        }
    }
};