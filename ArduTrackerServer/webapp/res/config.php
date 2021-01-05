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
$_DBC['host'] = 'localhost';
$_DBC['user'] = 'root';
$_DBC['password'] = '';
$_DBC['database'] = 'db';


// -----------------------------------
// Global usefull variables 

$id = isset($_GET['id']) ? $_GET['id'] : 0;


