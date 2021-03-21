/*
 *  Log class
 *  --------
 *  This file contains the class that describes a single log record between 2 subjects
*/
#pragma once
#include <time.h>

// --------------------------------------- Log class data structure

class Log {
public:
    unsigned int cycle_counter;
    char friend_id[15];
    double rssi;
    time_t first_exposure_time;
    time_t last_exposure_time;

    Log(const char * fi = "Default", double rssi = 0.0, time_t fet = time(nullptr)-1, time_t let = time(nullptr)) {
        strlcpy(this->friend_id, fi, sizeof(this->friend_id));
        this->rssi = rssi;
        this->first_exposure_time = fet;
        this->last_exposure_time = let;
        this->cycle_counter = 1;
    }

    Log(StaticJsonDocument<1024> jsonData) {
        this->unserializeLocal(jsonData);
    }

    long getExposureMillis() const {
        return (difftime(last_exposure_time, first_exposure_time)) * 1000; 
    }

    void updateExposureSession(double rssi = 0.0){

        /* Note: time without a proper config counts the seconds since the start of the board
         * Note 2: 1year = 31546000, aleatory value for sanity check
         *
         * Critical case: the exposure session start before time alignment and the new  time is much more greater.
         * 1. Se lastExposureTime < time(nullptr) - 31546000
         * newLastExposureTime = lastExposureTime + time(nullptr) 
         * 
         * Critical case: the exposure session resumes after a restart and the time is not correct yet due to wifi disconnection
         * 2. Se firstExposureTime < time(nullptr) - 31546000 
         * newFirstExposureTime = firstExposureTime - time(nullptr)
        */

        // if(this->last_exposure_time < time(nullptr) - 31546000) { 
        //     this->last_exposure_time = this->last_exposure_time + time(nullptr);
        // }
        // else if(this->first_exposure_time + 31546000 < time(nullptr)) {
        //     this->first_exposure_time = this->first_exposure_time - time(nullptr);
        // }
        // else{
            this->last_exposure_time = time(nullptr);
        // }
        
        this->cycle_counter++;
        this->rssi = this->rssi + ((rssi - this->rssi) / this->cycle_counter); // rssi average calculation
    }

    void unserializeLocal(StaticJsonDocument<1024> jsonData) {
        strlcpy(this->friend_id, jsonData["friend_id"], sizeof(this->friend_id));
        this->rssi = jsonData["rssi"];
        char tmpFet[32];
        char tmpLet[32];
        strlcpy(tmpFet, jsonData["first_exposure_time"], sizeof(tmpFet));
        strlcpy(tmpLet, jsonData["last_exposure_time"], sizeof(tmpLet));
        this->first_exposure_time = (time_t) strtol(tmpFet, nullptr, 10); 
        this->last_exposure_time = (time_t) strtol(tmpLet, nullptr, 10); 
        this->cycle_counter = jsonData["cycle_counter"];
    }

    String serializeLocal() const {
        String msg = "{";
        msg += "\"friend_id\": \""    + (String)this->friend_id + "\",";
        msg += "\"first_exposure_time\": \"" + (String)this->first_exposure_time + "\",";
        msg += "\"last_exposure_time\": \"" + (String)this->last_exposure_time + "\",";
        msg += "\"rssi\": "         + (String)this->rssi + ",";
        msg += "\"cycle_counter\": "   + (String)this->cycle_counter;
        msg += "}";

        return msg;
    }

    String serializeMqtt(const char * my_id = params.my_id) const {
        String msg = "{";
        msg += "\"my_id\": \""        + (String)my_id   + "\",";
        msg += "\"friend_id\": \""    + (String)this->friend_id + "\",";
        msg += "\"seen_millis\": \""  + (String)this->getExposureMillis() + "\",";
        msg += "\"seen_time\": \""    + (String)this->last_exposure_time + "\",";
        msg += "\"rssi\": \""         + (String)this->rssi + "\",";
        msg += "\"scan_count\": \""   + (String)this->cycle_counter + "\"";
        msg += "}";

        return msg;
    }
};

