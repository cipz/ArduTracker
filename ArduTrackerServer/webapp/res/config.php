<?php


// -----------------------------------
// Static 

define("DEBUG", true);
define("VERSION", "0.1");
define("REPO", "https://github.com/cipz/ArduTracker");


// -----------------------------------
// Debug mode

$_DEBUG = DEBUG;

if($_DEBUG)
    error_reporting(E_ALL & ~E_NOTICE);


// -----------------------------------
// Database 

$_DBC = array();
$_DBC['host'] = 'ip1.debug.ovh';
$_DBC['user'] = 'admin_ardutrack';
$_DBC['password'] = '1q9p7lETVi';
$_DBC['database'] = 'admin_ardutrackerdb';


// -----------------------------------
// Global useful variables 

$id = isset($_GET['id']) ? $_GET['id'] : 0;


