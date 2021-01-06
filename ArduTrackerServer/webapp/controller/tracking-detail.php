<?php
require_once 'res/database.php';
$db = new database();
if(isset($_GET["id"])) {
    $id = $_GET["id"];
    $db->query("SELECT * FROM tracking_log WHERE my_id='$id' OR friend_id='$id'");
} else {
    $db->query("SELECT * FROM tracking_log");
}

$num = $db->num();
$data = $db->get();

for($i=0; $i<$num; ++$i) {
    echo("<tr>");
    echo("<th scope='row'>".$data[$i]["my_id"]."</th>");
    echo("<th scope='row'>".$data[$i]["friend_id"]."</th>");
    echo("<td>".$data[$i]["seen_millis"]."</td>");
    echo("<td>".$data[$i]["seen_time"]."</td>");
    echo("<td>".$data[$i]["created_at"]."</td>");
    echo("</tr>");
}

?>