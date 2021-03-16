<?php


// -----------------------------------
// Static 

define("DEBUG", true);
define("VERSION", "0.3.0");
define("REPO", "https://github.com/cipz/ArduTracker");
define("PAGINATION_LIMIT", "20");
define("LOW_RISK_MINUTES", "2");
define("MID_RISK_MINUTES", "4");
define("DEFAULT_MAX_PACKET_SIZE", "2048");
define("BOARD_MODE_CONFIG_FIELD", "type");

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
	"my_id" : "JohnDoe",
	"broadcast_io_addr" : "RxTx0",
	"in_topic" : "math/wnma/ardutrack",
	"out_topic" : "math/wnma/ardutrack",
	"mqtt_server" : "ardutracker.debug.ovh",
    "friendly_freshness" : 5000
}');


