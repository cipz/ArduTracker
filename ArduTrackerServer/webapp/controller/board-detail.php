<?php

require_once 'res/resources.php';
$db = new database();

if (!isset($_GET["id"]))
    goToLocation('boards.php');

$board = new Board();
$board->constructFromId($id);

if ($board->id == 'undefined')
    goToLocation('boards.php');


if (isset($_POST['Save']) || isset($_POST['Send']) || isset($_POST['SaveAndSend'])) {

    if (isset($_POST['Save']) || isset($_POST['SaveAndSend'])) {

        $tkey = $_POST['tkey'];
        $tval = $_POST['tval'];

        if (empty($tkey) || empty($tval))
            $out = errorBox("The configuration sent is empty.");
        else {
            $arr = array();
            for ($i = 0; $i < count($tkey); $i++) {
                $arr = array_merge($arr, array($tkey[$i] => $tval[$i]));
            }

            $json = json_encode($arr);
            $board->updateConfiguration($json);

            if (isset($_POST['Save'])) {
                $out = successBox("The configuration has been saved. Remember to sync it after completing all the edits.");
            }
        }
    }

    if (isset($_POST['Send']) || isset($_POST['SaveAndSend'])) {

        // Sync script ...
        //

        if (isset($_POST['SaveAndSend'])) {
            $board->updateNewConfigSent(1);
            $out = successBox("The configuration has been saved and the request for synchronization has been sent.");
        } else {
            $board->updateNewConfigSent(1);
            $out = successBox("The request for synchronization has been sent.");
        }
    }
}

if (isset($_POST['Remove'])) {
    if($board->deleteBoard()) {
       goToLocation("boards.php?id=$id&Removed");
    }
    else {
        $out = errorBox("The board could not be deleted due to an internal error");
    }

}


$label = $board->newConfigSent ? "<span class='badge rounded-pill bg-success'><i class='fas fa-spin fa-sync-alt'></i> Synched</span>" : "<span class='badge rounded-pill bg-danger'><i class='fas fa-times'></i> Not synched</span>";
