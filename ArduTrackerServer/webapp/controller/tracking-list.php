<?php
require_once 'res/database.php';
$db = new database();

$db->query(
    'SELECT DISTINCT my_id FROM tracking_log 
    UNION
    SELECT DISTINCT friend_id FROM tracking_log');

$num = $db->num();
$data = $db->get();

for($i=0; $i<$num; ++$i) {
    $id = $data[$i][0];
    echo("<tr>");
    echo("<td><a href='tracking-detail.php?id=$id'>$id</a></td>");
    echo("</tr>");
}

?>