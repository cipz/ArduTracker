#include <SD.h>

#define CS_PIN 22 // for SDCard

File cache_log_file;
File perm_log_file;

bool init_sd();
bool load_sd_params(Params);
void init_log_files();

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

  strlcpy(params.my_id, params_json["my_id"], sizeof(params.my_id));

  char tmp_broadcast_io_addr[sizeof(params.broadcast_io_addr)];
  strlcpy(tmp_broadcast_io_addr, params_json["broadcast_io_addr"], sizeof(tmp_broadcast_io_addr));
  for (int i = 0; i < sizeof(params.broadcast_io_addr); i++)
    params.broadcast_io_addr[i] = tmp_broadcast_io_addr[i];

  // MQTT
  strlcpy(params.in_topic, params_json["in_topic"], sizeof(params.in_topic));
  strlcpy(params.out_topic, params_json["out_topic"], sizeof(params.out_topic));

  return true;

}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void init_log_files() {

  // Remove the cache_log file to crete it from scratch
  if (SD.exists("/cache_log.txt"))
    SD.remove("/cache_log.txt");

  // Creating files
  cache_log_file = SD.open("/cache_log.txt", FILE_WRITE);
  perm_log_file = SD.open("/perm_log.txt", FILE_WRITE);

  cache_log_file.close();
  perm_log_file.close();

}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

void listFiles() {

  File root = SD.open("/");
  // printDirectory(root, 0);
  Serial.println("done! ----------");
  Serial.println();

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
