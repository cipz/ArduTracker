<?php

require_once 'res/resources.php';
$pr = new protection(true);

$out = "";

if(isset($_GET['Logout'])) {
    $pr->logout();
    goToLocation(LOGIN_PAGE_URL);
}

if(isset($_GET['LoggedIn'])){
    $out = successBox("You are now logged in! <i class='far fa-smile'></i>");
}