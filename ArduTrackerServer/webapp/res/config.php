<?php

error_reporting(E_ALL);

define("VERSION", "0.1");
define("REPO", "https://github.com/cipz/ArduTracker");

$name = isset($_GET['name']) ? $_GET['name'] : '';