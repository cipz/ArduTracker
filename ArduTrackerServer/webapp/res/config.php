<?php

session_start(); 

// -----------------------------------
// Static 

define("DEBUG", true);
define("PASSWORD_PROTECTION", "ardutrack");
define("LOGIN_PAGE_URL", "login.php");
define("VERSION", "0.4.1");
define("REPO", "https://github.com/cipz/ArduTracker");
define("PAGINATION_LIMIT", "20");
define("LOW_RISK_MINUTES", "2");
define("MID_RISK_MINUTES", "4");
define("DEFAULT_MAX_PACKET_SIZE", "1024");
define("BOARD_ID_CONFIG_FIELD", "my_id");
define("BOARD_MODE_CONFIG_FIELD", "radio_mode");
define("BOARD_STATION_CONFIG_FIELD", "station_mode");
define("GRAPH_MAX_DEPTH_CHILDREN", "3"); // change to 3

// -----------------------------------
// Debug mode

$_DEBUG = DEBUG;

if($_DEBUG)
    error_reporting(E_ALL & ~E_NOTICE);


// -----------------------------------
// Database 

$_DBC = array();
$_DBC['host'] = 'database';
$_DBC['user'] = 'admin_ardutrack';
$_DBC['password'] = '1q9p7lETVi';
$_DBC['database'] = 'ardutrackerdb';


// -----------------------------------
// Global useful variables 

$id = isset($_GET['id']) ? $_GET['id'] : 0;

// -----------------------------------
// Default Board Configuration

$_DEFAULT_BOARD_CONFIG = json_decode('{
    "ssid" : "WIFI-ARDUTRACK",
    "password" : "changeme",
    "my_id" : "DEFAULT",
    "broadcast_io_addr" : "192.168.1.1",
    "in_topic" : "math/wnma/ardutrack/config/",
    "out_topic" : "math/wnma/ardutrack",
    "mqtt_server" : "ardutracker.debug.ovh",
    "radio_mode" : "BLE",
    "station_mode" : 0,
    "friendly_freshness" : 10000,
    "send_data_cycles" : 3,
    "ble_threshold" : -80,
    "wifi_kbps" : 2000,
    "scan_duration" : 1000
}', true);



$_BOARD_CONFIG_EXTRA = json_decode('{
    "ssid" : {
        "info" : "WI-FI SSID to connect as client to the MQTT server"
    },
    "password" : {
        "info" : "WI-FI password for the SSID"
    },
    "my_id" : {
        "info" : "Board Identificative name"
    },
    "broadcast_io_addr" : {
        "info" : "Broadcast addresses for RF24L01 module, separated from a comma"
    },
    "in_topic" : {
        "info" : "MQTT Topic used to send new configuration (always followed by a forward slash!)"
    },
    "out_topic" : {
        "info" : "MQTT Topic used to send concluded exposure sessions (do not add forward slash at the end!)"
    },
    "mqtt_server" : {
        "info" : "MQTT server URL "
    },
    "radio_mode" : {
        "info" : "Set the radio mode to detect exposure sessions",
        "select" : ["WIFI", "BLE"]
    },
    "station_mode" : {
        "info" : "Enable or disable the station mode (0 = disabled)",
        "select" : ["0", "1"]
    },
    "friendly_freshness" : {
        "info" : "Maximum time period to wait before ending the exposure session after no detection",
        "unit" : "ms",
        "range" : true,
        "min_range" : 1000,
        "max_range" : 120000
    },
    "send_data_cycles" : {
        "info" : "Board cycles to wait before sending concluded exposure sessions to the MQTT topic, thus reducing WI-FI traffic",
        "unit" : "cycles",
        "range" : true,
        "min_range" : 1,
        "max_range" : 1000 
    },
    "ble_threshold" : {
        "info" : "Only for Radio Mode BLE - RSSI threshold to ignore in an exposure session ",
        "range" : true,
        "unit" : "dBm",
        "min_range" : -100,
        "max_range" : -20 
    },
    "wifi_kbps" :  {
        "info" : "Only for Radio Mode WIFI - Set the data rate to send packets WIFI for exposure sessions",
        "unit" : "Kbps",
        "select" : ["2000", "1000", "250"]
    },
    "scan_duration" : {
        "info" : "Set the scan duration for the current Radio Mode (lower value increases the number of cycles). In WIFI mode the number of sent packets will be higher/lower, meanwhile in BLE mode scanning will be more/less frequent. ",
        "range" : true,
        "unit" : "ms",
        "min_range" : 1000,
        "max_range" : 20000
    }
}', true);

