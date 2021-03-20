<?php
require_once 'res/resources.php';

$db = new database();

$db->query(
    'SELECT DISTINCT my_id FROM tracking_log
    UNION
    SELECT DISTINCT friend_id FROM tracking_log');

$num = $db->num();
$data = $db->get();



$out = "";
 foreach($data as $board) {
    $out .= "<tr>";
    $out .= "<td><i class='fas fa-microchip text-primary'></i> <a href='tracking-detail.php?id=".$board[0]."'>".$board[0]."</a></td>";
    $out .= "</tr>";
}
