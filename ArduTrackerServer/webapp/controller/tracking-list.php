<?php
require_once 'res/resources.php';
$db = new database();

$db->query(
    'SELECT DISTINCT my_id FROM tracking_log
    UNION
    SELECT DISTINCT friend_id FROM tracking_log');

$num = $db->num();
$data = $db->get();


function printTrackingList($data, $num) {

    $out = "";
    for($i=0; $i<$num; ++$i) {
        $id = $data[$i][0];
        $out .= "<tr>";
        $out .= "<td><a href='tracking-detail.php?id=$id'>$id</a></td>";
        $out .= "</tr>";
    }
    return $out;
}
