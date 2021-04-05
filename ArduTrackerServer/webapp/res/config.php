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
    "friendly_freshness" : "10000",
    "in_topic" : "math/wnma/ardutrack/config/",
    "out_topic" : "math/wnma/ardutrack",
    "mqtt_server" : "ardutracker.debug.ovh",
    "endpoint" : "endpoint",
    "radio_mode" : "WIFI",
    "send_data_cycles" : 3,
    "ble_threshold" : -80,
    "wifi_kbps" : 2000,
    "scan_duration" : 1000
}', true);

