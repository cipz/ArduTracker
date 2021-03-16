<?php 

require_once 'res/resources.php';
$db = new database();

if(!isset($_GET["id"])) 
    goToLocation('boards.php');

$id = $db->clean($_GET["id"]);
$board = new Board($id);
$label = $board->newConfigSent ? "<span class='badge rounded-pill bg-success'><i class='fas fa-sync-alt'></i> Synched</span>" : "<span class='badge rounded-pill bg-danger'><i class='fas fa-times'></i> Not synched</span>";

if($board->id == 'undefined')
    goToLocation('boards.php');