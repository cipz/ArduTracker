/*
 *  Utils functions
 *  --------
 *  This file contains all some utilities to manage certain classes in the embed software
*/

#pragma once

void restart(int delay_seconds) {
    if (!DEBUG_MODE) {
        delay(delay_seconds * 1000);
        ESP.restart();
    }
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
                "\n%s {FriendID = %s, SeenMillis = %d, SeenTime = %d}\n",
                label,
                friendList->get(i).friend_id,
                friendList->get(i).seen_millis,
                friendList->get(i).seen_time);
        }
    }
};


// --------------------------------------- MQTT Controller

class MQTTController {
    public:
    /**
     * Initialize MQTT parameters
    */
    void init() {
        Serial.println("Initializing MQTT");
        client.setServer(params.mqtt_server, 21883);
    }

    /**
     * Connect to MQTT server
    */
    void connect() {
        if(WiFi.status() != WL_CONNECTED)
            return;
        
        int mqttConnAttempts = 0;
        
        while(!client.connected()) {
            if(mqttConnAttempts > 3) {
                Serial.println("Exceed MQTT connection tries");
                return;
            }
            Serial.println("Attempting MQTT connection...");
            if(client.connect("ESP8266Client"))
                Serial.println("MQTT connected.");
            else {
                Serial.printf("\nFailed, rc= %d");
                Serial.println("Retrying in 3s");
                delay(3000);
            }
            mqttConnAttempts++;
        }

        client.loop();
    }

    /**
     * Publish message with given topic
    */
    void publish(char* out_topic, char* message) {
        client.publish(out_topic, message);
    }

};