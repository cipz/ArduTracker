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

    Log(const char * fi = "Default", int sm = millis(), time_t st = time(nullptr)) {
        strlcpy(this->friend_id, fi, sizeof(this->friend_id));
        this->seen_millis = sm;
        this->seen_time = st;
    }

    char friend_id[15];
    int seen_millis;
    time_t seen_time;

    String serialize(const char * my_id = params.my_id) const {
    
        String msg = "{";
        msg += "\"my_id\": \""        + (String)my_id + "\",";
        msg += "\"friend_id\": \""    + (String)this->friend_id + "\",";
        msg += "\"seen_millis\": \""  + (String)this->seen_millis + "\",";
        msg += "\"seen_time\": \""    + (String)this->seen_time + "\"";
        msg += "}";

        return msg;
    }
};

