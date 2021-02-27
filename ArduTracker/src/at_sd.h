/*
 *  SD Card class
 *  --------
 *  This contains the business logic for the I/O files in the SD card
*/

#pragma once
#include <SD.h>

#define CS_PIN      22            // for SD card
#define CACHE_FILE  "/cache.txt"
#define PERM_FILE   "/perm.txt"
#define STATS_FILE  "/stats.txt"

// --------------------------------------- SD low-level operations

class SDCard {
    File cacheFile;
    File permFile;
    File statsFile;

    public: 

    bool init() { 
        return SD.begin(CS_PIN);
    }

    bool loadParams() {
        
        if(!SD.exists("/params.json")) 
            return false;
        
        File paramsFile = SD.open("/params.json");
        StaticJsonDocument<512> paramsJson;
        
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

        params.wifi_send_interval = paramsJson["wifi_send_interval"];

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
        
        params.friendly_freshness = paramsJson["friendly_freshness"] ? paramsJson["friendly_freshness"] : 20000;

        int size = sizeof(params.broadcast_io_addr);
        char tmp[size];
        strlcpy(tmp, paramsJson["broadcast_io_addr"], size);
        for(int i=0; i < size; i++) {
            params.broadcast_io_addr[i] = tmp[i];
        }
        return true;  
    }

    void initFile(String filename) {

        if(SD.exists(filename))
            SD.remove(filename);

        cacheFile = SD.open(filename, FILE_WRITE);
        if(cacheFile)
            cacheFile.close();
        else {
            Serial.println("ERROR creating "+ filename +" file");
            restart(RESTART_SECONDS);
        }
    }

    void initLogFiles() {
        initFile(CACHE_FILE);
        if (!SD.exists(PERM_FILE)) {
            initFile(PERM_FILE);
        }
        initFile(STATS_FILE);
    }

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

    void saveInLog(String msg) {
        cacheFile = SD.open(CACHE_FILE, FILE_WRITE);
        if(cacheFile) {
            cacheFile.println(msg);
            cacheFile.close();
        }
        else {
            Serial.println("ERROR opening cache");
        }

        permFile = SD.open(PERM_FILE, FILE_APPEND);
        permFile.println(msg);
        permFile.close();
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
     * Initialize cache and perm files
    */
    void initLog() {
        Serial.println("Creating log files");
        sd->initLogFiles();
        Serial.println("Log files correctly created!");
    }

    /**
     * Save msg to cache (override) and perm (append)
    */
    void saveInLog(String msg) {
        sd->saveInLog(msg);
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

};