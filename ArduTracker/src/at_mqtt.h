/*
 *  MQTT class
 *  --------
 *  This file contains the controller for the MQTT client
*/

// --------------------------------------- MQTT Controller

SDController* pSdCtrl;
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("New message on topic: ");
    Serial.println(topic);

    Serial.print("Message:");
    String msg = "";
    for (int i = 0; i < length; i++) {
        msg += (char)payload[i];
    }
    
    Serial.println(msg);
    if(pSdCtrl->updateParams(msg)) {
        restart(RESTART_SECONDS);
    }
}

class MQTTController {
    private:
    bool subscribed = false;

    public:
    /**
     * Initialize MQTT parameters
    */
    void init() {
        Serial.println("Initializing MQTT");
        client.setServer(params.mqtt_server, 21883);
        client.setBufferSize(2048);
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


    /**
     * Subscribe to the topic
    */
    void subscribe(const char* topic, SDController* sdCtrl) {
        pSdCtrl = sdCrtl;
        client.subscribe(topic);
        client.setCallback(callback);
        subscribed = true;
    }

    bool isConnected() {
        return client.connected();
    }

    bool isSubscribed() {
        if(!isConnected())
            subscribed = false;
        return subscribed;
    }
};