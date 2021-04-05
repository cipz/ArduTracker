<?php

require_once 'res/resources.php';
$pr = new protection(true);
$db = new database();

if (!isset($_GET["id"]))
    goToLocation('boards.php?NotFound&id=%20');

$board = new Board();
$board->constructFromId($id);

if ($board->id == 'undefined')
    goToLocation('boards.php?NotFound&id='.$id);


if (isset($_POST['Save']) || isset($_POST['Send']) || isset($_POST['SaveAndSend'])) {

    $saved = false;

    if (isset($_POST['Save']) || isset($_POST['SaveAndSend'])) {


        // JSON readable form

        if (isset($_POST['tkey'], $_POST['tval'])) {

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

                if (!$board->updateConfiguration($json))
                    $out = errorBox("The configuration could not be saved, due to an internal error with the database, check the syntax.");
                else {

                    $saved = true;

                    if (isset($_POST['Save'])) {
                        $out = successBox("The configuration has been saved. Remember to sync it after completing all the edits.");
                    }
                }
            }

        // Raw JSON form

        } elseif (isset($_POST['rawconfig'])) {

            $rawconfig = $_POST['rawconfig'];

            if (empty($rawconfig) || !isValidJson($rawconfig))
                $out = errorBox("The configuration sent is empty or the syntax is invalid.");
            else {

                if (!$board->updateConfiguration($rawconfig))
                    $out = errorBox("The configuration could not be saved, due to an internal error with the database, check the syntax.");
                else {

                    $saved = true;

                    if (isset($_POST['Save'])) {
                        $out = successBox("The configuration has been saved. Remember to sync it after completing all the edits.");
                    }
                }
            }

        // Invalid form

        } else {
            $out = errorBox("The configuration sent is invalid.");
        }
    }

    if (isset($_POST['Send']) || (isset($_POST['SaveAndSend']) && $saved)) {

        // Sync script
        $cmd = escapeshellcmd("python res/python/mqtt_config.py " . $board->idMac);
        $sres = shell_exec($cmd);

        if (trim($sres) != "done") {
            $board->updateNewConfigSent(0);
            $out = errorBox("Unable to send the new synchronization due to an internal error.");
        } else {
            if (isset($_POST['SaveAndSend'])) {
                $board->updateNewConfigSent(1);
                $out = successBox("The configuration has been saved and the request for synchronization has been sent.");
            } else {
                $board->updateNewConfigSent(1);
                $out = successBox("The request for synchronization has been sent.");
            }
        }
    }
}

if (isset($_POST['Remove'])) {
    if ($board->deleteBoard()) {
        goToLocation("boards.php?id=$id&Removed");
    } else {
        $out = errorBox("The board could not be deleted due to an internal error");
    }
}


$label = $board->newConfigSent ? "<span class='badge rounded-pill bg-success'><i class='fas fa-spin fa-sync-alt'></i> Synced</span>" : "<span class='badge rounded-pill bg-danger'><i class='fas fa-times'></i> Not synced</span>";
