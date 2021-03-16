<?php

require_once 'res/resources.php';
$db = new database();

if (isset($_POST["AddBoard"])) {
    if (!isset($_POST['id']) || !isset($_POST['mac'])) {
        $out = errorBox("The board name or MAC address provided is empty.");
    } elseif (Board::checkIdBoard($_POST['id'])) {
        $out = errorBox("The board name already exists. Use a different one.");
    } elseif (Board::checkMacBoard($_POST['mac'])) {
        $out = errorBox("The MAC address already exists. Check the registered boards.");
    } else {
        Board::addNewBoard($_POST['id'], $_POST['mac']);       
        goToLocation("board-detail.php?id=".$_POST['id']);
    }
}

if (isset($_GET["Removed"])) {
    $out = successBox("The board <code>$id</code> has been <b>deleted</b> successfully!");
}
