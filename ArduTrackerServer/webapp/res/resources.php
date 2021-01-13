<?php

require_once 'config.php';
require_once 'database.php';

// -----------------------------------
// Functions

function getTotalRecords($table="tracking_log") {
	global $_DBC;
	$db = new database();
	$db->query('SELECT * FROM '.$table);
	return $db->num();
}
