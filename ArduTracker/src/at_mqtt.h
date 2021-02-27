/*
 *  MQTT class
 *  --------
 *  This file contains the controller for the MQTT client
*/

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
                Serial.printf("\nFailed, rc= %d", mqttConnAttempts);
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