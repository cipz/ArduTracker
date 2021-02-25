<?php
require_once 'res/database.php';
$db = new database();
if(isset($_GET["id"])) {
    $id = $_GET["id"];
    $db->query("SELECT * FROM tracking_log WHERE my_id='$id' OR friend_id='$id' ORDER BY created_at DESC");
} else {
    $db->query("SELECT * FROM tracking_log ORDER BY created_at DESC");
}

$num = $db->num();
$data = $db->get();

for($i=0; $i<$num; ++$i) {
    echo("<tr>");
    echo("<th scope='row'>".$data[$i]["my_id"]."</th>");
    echo("<th scope='row'>".$data[$i]["friend_id"]."</th>");
    echo("<td>".time2string($data[$i]["seen_millis"], true, false)."</td>");
    echo("<td>".time2string($data[$i]["seen_time"], true, true)."</td>");
    echo("<td>".$data[$i]["created_at"]."</td>");
    echo("</tr>");
}

?>