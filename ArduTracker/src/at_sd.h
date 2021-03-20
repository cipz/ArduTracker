/*
 *  SD Card class
 *  --------
 *  This contains the business logic for the I/O files in the SD card
*/

#pragma once
#include <SD.h>

#define CS_PIN       22            // for SD card
#define CACHE_FILE   "/cache.txt"
#define PERM_FILE    "/perm.txt"
#define STATS_FILE   "/stats.txt"
#define SESSION_FILE "/session.txt"

// --------------------------------------- SD low-level operations

class SDCard {
    File cacheFile;
    File permFile;
    File sessionFile;
    File statsFile;

    public: 

    bool init() { 
        return SD.begin(CS_PIN);
    }

    bool loadParams() {
        
        if(!SD.exists("/params.json")) 
            return false;
        
        File paramsFile = SD.open("/params.json");
        StaticJsonDocument<1024> paramsJson;
        
        DeserializationError error = deserializeJson(paramsJson, paramsFile);
        if(error) 
            Serial.println("Failed to read file. Using default config");

        paramsFile.close();

        
        strlcpy(
            params.ssid, 
            paramsJson["ssid"], 
            sizeof(params.ssid));
        strlcpy(
            params.password, 
            paramsJson["password"], 
            sizeof(params.password));

        // params.wifi_send_interval = paramsJson["wifi_send_interval"];

        strlcpy(
            params.my_id, 
            paramsJson["my_id"], 
            sizeof(params.my_id));
        strlcpy(
            params.in_topic, 
            paramsJson["in_topic"], 
            sizeof(params.in_topic));
        strlcpy(
            params.out_topic, 
            paramsJson["out_topic"], 
            sizeof(params.out_topic));
        strlcpy(
            params.mqtt_server, 
            paramsJson["mqtt_server"], 

            sizeof(params.mqtt_server));
            
        strlcpy(
            params.radio_mode, 
            paramsJson["radio_mode"], 
            sizeof(params.radio_mode));
        
        params.send_data_cycles = paramsJson["send_data_cycles"] ? paramsJson["send_data_cycles"] : 5;

        params.friendly_freshness = paramsJson["friendly_freshness"] ? paramsJson["friendly_freshness"] : 20000;

        int size = sizeof(params.broadcast_io_addr);
        char tmp[size];
        strlcpy(tmp, paramsJson["broadcast_io_addr"], size);
        for(int i=0; i < size; i++) {
            params.broadcast_io_addr[i] = tmp[i];
        }
        return true;  
    }

    String getParams() {
        File paramsFile = SD.open("/params.json", FILE_READ);
        String paramsText = paramsFile.readString();
        paramsFile.close();
        return paramsText;
    }

    void updateParams(String paramsText) {
        File paramsFile = SD.open("/params.json", FILE_WRITE);
        paramsFile.print(paramsText);
        paramsFile.close();
    }

    void initFile(String filename) {
        if(!SD.exists(filename)) {
            SD.open(filename, FILE_WRITE).close();
        }
    }

    void initLogFiles() {
        initFile(CACHE_FILE);
        initFile(PERM_FILE);
        initFile(SESSION_FILE);
        initFile(STATS_FILE);
    }

    void clearFile(String filename) {
        if(SD.exists(filename)) {
            File f = SD.open(filename, FILE_WRITE);
            f.print("");
            f.close();
        }
    }

    // void initFile(String filename) {
    //     if(SD.exists(filename))
    //         SD.remove(filename);

    //     cacheFile = SD.open(filename, FILE_WRITE);
    //     if(cacheFile)
    //         cacheFile.close();
    //     else {
    //         Serial.println("ERROR creating "+ filename +" file");
    //         restart(RESTART_SECONDS);
    //     }
    // }

    // void initLogFiles() {
    //     initFile(CACHE_FILE);
    //     if (!SD.exists(PERM_FILE)) {
    //         initFile(PERM_FILE);
    //     }
    //     initFile(STATS_FILE);
    // }

    void printFile(String filename) {
        File dataFile = SD.open(filename);
        if(dataFile) {
            while(dataFile.available()) {
                Serial.write(dataFile.read());
            }
            dataFile.close();
        } 
        else
            Serial.println("ERROR opening file " + filename);
    }

    void printFile(File file) {
        int stringIndex = 0;
        char inputString[200];
        int inputChar;

        inputChar = file.read();

        while(inputChar != -1) {
            if (inputChar != '\n') {
                inputString[stringIndex] = inputChar;
                stringIndex++;
                inputString[stringIndex] = '\0';
            } 
            else {
                Serial.println(inputString);
                stringIndex = 0;
            }
            inputChar = file.read();
        }
    }

    void printDirectory(File dir, int numTabs) {
        while(true) {
            File entry = dir.openNextFile();
            if(!entry) 
                break;

            for(uint8_t i=0; i < numTabs; i++) 
                Serial.print('\t');
            Serial.print(entry.name());

            if(entry.isDirectory()) {
                Serial.println("/");
                printDirectory(entry, numTabs+1);
            }
            else {
                Serial.printf("\t\t%d", entry.size());
                Serial.println();
            }
            entry.close();
        }
    }

    void saveInFile(String msg, const char* fileName, const char* mode) {
        File dataFile = SD.open(fileName, mode);
        
        if(dataFile) {
            dataFile.println(msg);
            dataFile.close();
        }
        else {
            Serial.printf("ERROR opening files %s with mode %s \n", fileName, mode);
        }
    }

    void saveAndAppendInStats(String msg) {
        statsFile = SD.open(STATS_FILE, FILE_APPEND);
        if(statsFile) {
            statsFile.println(msg);
            statsFile.close();
        }
        else {
            Serial.println("ERROR opening Stats file");
        }
    }
    void listFiles() {
        File root = SD.open("/");
        printDirectory(root, 0);
        Serial.println("done.");
    }
};


// --------------------------------------- SD Controller class

class SDController {

    SDCard* sd;

    public:
    SDController() {
        sd = new SDCard();
    }

    static void populateFromCache() {
        File cacheFile = SD.open(CACHE_FILE, FILE_READ);
        int inputChar = cacheFile.read();
        int strIndex = 0;
        char inputStr[1024];

        Serial.println("\nSending:");

        while(inputChar != EOF) {
            if(inputChar != '\n') {
                inputStr[strIndex] = inputChar;
                strIndex++;
                inputStr[strIndex] = '\0';
            }
            else { // Each line           
                Serial.println(inputStr);
                StaticJsonDocument<1024> jsonData;
                DeserializationError error = deserializeJson(jsonData, inputStr);
                // Serial.print(jsonData);
    
                if(error == DeserializationError::Ok) {
                    Log log = Log();
                    strlcpy(log.friend_id, jsonData["friend_id"], sizeof(log.friend_id));
                    log.start_millis = jsonData["start_millis"];
                    log.end_millis = jsonData["end_millis"];
                    log.rssi = jsonData["rssi"];
                    log.last_exposure_time = (time_t) atoll(jsonData["last_exposure_time"]);
                    log.cycle_counter = jsonData["cycle_counter"];
                    friendList->add(log);
                    delay(50);
                }
                strIndex = 0;
            }
            inputChar = cacheFile.read();
        }

        Serial.printf("\nAll records have been sent to %s", params.mqtt_server);
        cacheFile.close();
        
    }

    /**
     * Initialize SD class (or restart)
    */
    void init() {
        Serial.print("Setting up SD card: ");
        if(sd->init()) 
            Serial.println("OK");
        else {
            Serial.println("ERROR, rebooting in 10s");
            restart(RESTART_SECONDS);
        }
    }

    void listContent() {
        Serial.println("SD card content: ");
        sd->listFiles();
    }
    
    /**
     * Get parameters from params.json file (or restart)
    */
    void acquireParams() {
        Serial.print("Acquiring parameters: ");
        if (sd->loadParams())
            Serial.println("OK");
        else {
            Serial.printf("\nERROR, rebooting in %d sec", RESTART_SECONDS);
            restart(RESTART_SECONDS);
        }
    }

    /** 
     * Write parameters to params.json
     * @return true if params was updated
    */
    bool updateParams(String newParams) {
        Serial.println("Updating params...");
        
        String oldParams = sd->getParams();

        if(oldParams.equals(newParams)) {
            Serial.println("Nothing to change in params.");
            return false;
        } else {
            sd->updateParams(newParams);
            Serial.println("Params updated!");
            Serial.println(newParams);
            return true;
        }
    }

    /** 
     * Initialize cache and perm files
    */
    void initLog() {
        Serial.println("Creating log files");
        sd->initLogFiles();
        Serial.println("Log files correctly created!");
    }

    /**
     * Save msg in different modes depending on the file
    */
    void saveConcludedSession(String msg) {
        sd->saveInFile(msg, SESSION_FILE, FILE_APPEND);
        sd->saveInFile(msg, PERM_FILE, FILE_APPEND);
    }

    void saveCurrentSessions(String msg) {
        sd->saveInFile(msg, CACHE_FILE, FILE_WRITE);
    }

    /**
     * Save statistics
    */
    void saveInStats(int tx, int rx) {
        String msg = millis() + String("\t") +
                    tx + String("\t") + 
                    rx;
        sd->saveAndAppendInStats(msg);
    }

    void clearFile(String msg) {
        sd->clearFile(msg);
    }

    

};