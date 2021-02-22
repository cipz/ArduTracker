/*
 *  Log class
 *  --------
 *  This file contains the class that describes a single log record between 2 subjects
*/
#pragma once
#include <time.h>

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

    // FIXME: are these useless? 
    void updateSeenMillis(int millis);
    void updateSeenTime(time_t time);
};

