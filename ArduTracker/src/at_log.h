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
    unsigned int cycle_counter = 1;

public:

    Log(const char * fi = "Default", double rssi = 0.0, int sm = millis()-1000, int em = millis(), time_t let = time(nullptr)) {
        strlcpy(this->friend_id, fi, sizeof(this->friend_id));
        this->start_millis = sm;
        this->end_millis = em;
        this->rssi = rssi;
        this->last_exposure_time = let;
    }

    char friend_id[15];
    int start_millis;
    int end_millis;
    double rssi;
    time_t last_exposure_time;

    void updateExposureSession(double rssi = 0.0){
        this->end_millis = millis();
        this->last_exposure_time = time(nullptr);
        this->cycle_counter++;
        // rssi average calculation
        this->rssi = this->rssi + ((rssi - this->rssi) / this->cycle_counter);
    }

    String serialize(const char * my_id = params.my_id) const {
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

