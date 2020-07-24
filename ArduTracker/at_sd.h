#pragma once

#include <SD.h>

#define CS_PIN 22 // for SDCard

File cache_file;
File perm_file;

bool init_sd();
bool load_sd_params(Params);
void init_log_files();
void save_in_log(String);
void printDirectory(File, int);
void listFiles();

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

bool init_sd() {
  return SD.begin(CS_PIN);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

bool load_sd_params() {

  if (!SD.exists("/params.json")) return false;

  File params_file;
  StaticJsonDocument<512> params_json;

  params_file = SD.open("/params.json");

  DeserializationError error = deserializeJson(params_json, params_file);
  if (error) Serial.println("Failed to read file, using default configuration");

  params_file.close();

  // strlcpy(destination, source, destination_dimension);

  // WiFi
  strlcpy(params.ssid, params_json["ssid"], sizeof(params.ssid));
  strlcpy(params.password, params_json["password"], sizeof(params.password));

  params.wifi_send_interval = params_json["wifi_send_interval"];

  strlcpy(params.my_id, params_json["my_id"], sizeof(params.my_id));

  char tmp_broadcast_io_addr[sizeof(params.broadcast_io_addr)];
  strlcpy(tmp_broadcast_io_addr, params_json["broadcast_io_addr"], sizeof(tmp_broadcast_io_addr));
  for (int i = 0; i < sizeof(params.broadcast_io_addr); i++)
    params.broadcast_io_addr[i] = tmp_broadcast_io_addr[i];

  // MQTT
  strlcpy(params.in_topic, params_json["in_topic"], sizeof(params.in_topic));
  strlcpy(params.out_topic, params_json["out_topic"], sizeof(params.out_topic));
  strlcpy(params.mqtt_server, params_json["mqtt_server"], sizeof(params.mqtt_server));

  // Other
  params.friendly_freshness = params_json["friendly_freshness"] | 20000;

  return true;

}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void init_log_files() {

  // Remove the cache file to create it from scratch
  if (SD.exists("/cache.txt"))
    SD.remove("/cache.txt");

  // Creating files
  cache_file = SD.open("/cache.txt", FILE_WRITE);
  if (cache_file)
    cache_file.close();
  else {
    Serial.println("Error creating cache file");
    restart(RESTART_SECONDS);
  }

  if (!SD.exists("/perm.txt")) {
    perm_file = SD.open("/perm.txt", FILE_WRITE);
    if (perm_file)
      perm_file.close();
    else {
      Serial.println("Error creating perm file");
      restart(RESTART_SECONDS);
    }
  }
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void print_file(String filename) {
  File dataFile = SD.open(filename);
  if (dataFile) {
    while (dataFile.available())
      Serial.write(dataFile.read());
    dataFile.close();
  } else {
    Serial.print("Error opening ");
    Serial.println(filename);
  }
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void save_in_log(String message) {

  cache_file = SD.open("/cache.txt", FILE_WRITE);
  if (cache_file) {
    cache_file.println(message);
    cache_file.flush();
    cache_file.close();
  } else {
    Serial.println("Error opening cache");
  }

  perm_file = SD.open("/perm.txt", FILE_WRITE);
  perm_file.println(message);
  perm_file.close();

}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void listFiles() {
  File root = SD.open("/");
  printDirectory(root, 0);
  Serial.println("done! ----------");
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
