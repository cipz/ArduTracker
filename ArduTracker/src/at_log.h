/*
 *  Log class
 *  --------
 *  This file contains the class that describes a single log record between 2 subjects
*/
#pragma once
#include <time.h>

// --------------------------------------- Log class data structure

class Log {
private:
    unsigned int cycle_counter;

public:

    char friend_id[15];
    int start_millis;
    int end_millis;
    double rssi;
    time_t last_exposure_time;

    Log(const char * fi = "Default", double rssi = 0.0, int sm = millis()-1000, int em = millis(), time_t let = time(nullptr)) {
        strlcpy(this->friend_id, fi, sizeof(this->friend_id));
        this->start_millis = sm;
        this->end_millis = em;
        this->rssi = rssi;
        this->last_exposure_time = let;
        this->cycle_counter = 1;
    }

    // Log(const Log&  old, double newRssi = 0.0) {
    //     strlcpy(this->friend_id, old.friend_id, sizeof(old.friend_id));
    //     this->start_millis = old.start_millis;
    //     this->end_millis = millis();
    //     this->rssi = old.rssi + ((newRssi - old.rssi) / cycle_counter);
    //     this->last_exposure_time = time(nullptr);
    //     this->cycle_counter = old.cycle_counter + 1;
    // }

    Log(const JsonDocument& jsonData) {
        strlcpy(this->friend_id, jsonData["friend_id"], sizeof(this->friend_id));
        this->start_millis = jsonData["start_millis"];
        this->end_millis = jsonData["end_millis"];
        this->rssi = jsonData["rssi"];
        this->last_exposure_time = time(jsonData["last_exposure_time"]);
        this->cycle_counter = jsonData["cycle_counter"];
    }

    void updateExposureSession(double rssi = 0.0){
        this->end_millis = millis();
        this->last_exposure_time = time(nullptr);
        this->cycle_counter++;
        // rssi average calculation
        this->rssi = this->rssi + ((rssi - this->rssi) / this->cycle_counter);
    }

    String serializeLocal() const {
        String msg = "{";
        msg += "\"friend_id\": \""    + (String)this->friend_id + "\",";
        msg += "\"start_millis\": " + (String)this->start_millis + ",";
        msg += "\"end_millis\": "    + (String)this->end_millis + ",";
        msg += "\"last_exposure_time\": " + (String)this->last_exposure_time + ",";
        msg += "\"rssi\": "         + (String)this->rssi + ",";
        msg += "\"cycle_counter\": "   + (String)this->cycle_counter;
        msg += "}";

        return msg;
    }

    String serializeMqtt(const char * my_id = params.my_id) const {
        String msg = "{";
        msg += "\"my_id\": \""        + (String)my_id + "\",";
        msg += "\"friend_id\": \""    + (String)this->friend_id + "\",";
        msg += "\"seen_millis\": \""  + (String)(this->end_millis - this->start_millis) + "\",";
        msg += "\"seen_time\": \""    + (String)this->last_exposure_time + "\",";
        msg += "\"rssi\": \""         + (String)this->rssi + "\",";
        msg += "\"scan_count\": \""   + (String)this->cycle_counter + "\"";
        msg += "}";

        return msg;
    }
};

