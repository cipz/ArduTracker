<?php 

require_once 'res/resources.php';
$db = new database();

if(!isset($_GET["id"])) 
    goToLocation('boards.php');

$id = $db->clean($_GET["id"]);
$board = new Board($id);

if($board->id == 'undefined')
    goToLocation('boards.php');