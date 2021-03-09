<?php


// -----------------------------------
// Static 

define("DEBUG", true);
define("VERSION", "0.2.0");
define("REPO", "https://github.com/cipz/ArduTracker");
define("PAGINATION_LIMIT", "20");
define("LOW_RISK_MINUTES", "2");
define("MID_RISK_MINUTES", "4");

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


