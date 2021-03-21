<?php

require_once 'res/resources.php';
$pr = new protection(false);

if (isset($_POST['Login'], $_POST['password'])) {

    if ($pr->accessWithSinglePassword($_POST['password'])) {
        goToLocation('index.php?LoggedIn');
    } else {
        sleep(2);
        $out = errorBox("Incorrect password, retry...");
    }
}
