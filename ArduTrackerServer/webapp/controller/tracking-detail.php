<?php
require_once 'res/resources.php';
$db = new database();
if(isset($_GET["id"])) {
    $id = $_GET["id"];
    $sql = "SELECT * FROM tracking_log WHERE my_id='$id' OR friend_id='$id' ORDER BY created_at DESC";
} else {
    $sql = "SELECT * FROM tracking_log ORDER BY created_at DESC";
}

    $db->query($sql);
    $num = $db->num(); // Total number of rows
    $db->query($sql.Pagination::limitQuery());
    $data = $db->get(); // Get data for the current limited rows

function printTrackingDetail($data) {

    $out = "";
    foreach($data as $record) {
        $out .= "<tr>";
        $out .= "<th scope='row'>".$record["my_id"]."</th>";
        $out .= "<th scope='row'>".$record["friend_id"]."</th>";
        $out .= "<td>".millis2string($record["seen_millis"])."</td>";
        $out .= "<td>".date('Y-m-d H:i:s', $record["seen_time"]/1000)."</td>";
        $out .= "<td>".time2String($record["created_at"])."</td>";
        $out .= "</tr>";
    }

    return $out;
}
