<?php
require_once 'res/resources.php';
$db = new database();

if(!isset($_GET["id"])) 
    goToLocation('tracking-list.php');


$id = $db->clean($_GET["id"]);
$type = isset($_GET['type']) ? $_GET['type'] : '';
$date_from = isset($_GET['date_from']) ? $db->clean($_GET["date_from"]) : '';
$date_to = isset($_GET['date_to']) ? $db->clean($_GET['date_to']) : '';
$risk = isset($_GET['risk']) ? $db->clean($_GET['risk']) : '';

if(isset($_POST['search'])) {
    goToLocation('?id='.$id.'&type='.$type.'&risk='.$_POST['risk'].'&date_from='.$_POST['date_from'].'&date_to='.$_POST['date_to']);
}

$add = "";
if(!empty($date_from)) $add .= " AND seen_time >= ".strtotime($date_from)*1000;
if(!empty($date_to)) $add .= " AND seen_time <= ".strtotime($date_to)*1000;

switch ($risk) {
    case 'high': $add .= ' AND seen_millis > '.(1000*60*MID_RISK_MINUTES); break;
    case 'mid': $add .= ' AND seen_millis > '.(1000*60*LOW_RISK_MINUTES) . ' AND seen_millis <= '.(1000*60*MID_RISK_MINUTES); break;
    case 'low': $add .= ' AND seen_millis <= '.(1000*60*LOW_RISK_MINUTES); break;
    default: break;
}

switch ($type) {
    case 'b2a': $sql = "SELECT * FROM tracking_log WHERE my_id='$id' $add ORDER BY created_at DESC"; break;
    case 'a2b': $sql = "SELECT * FROM tracking_log WHERE friend_id='$id' $add ORDER BY created_at DESC";  break;
    default: $sql = "SELECT * FROM tracking_log WHERE (my_id='$id' OR friend_id='$id') $add ORDER BY created_at DESC"; break;
}


$db->query($sql);
$num = $db->num(); // Total number of rows
$db->query($sql.Pagination::limitQuery());
$data = $db->get(); // Get data for the current limited rows


$out = "";
foreach($data as $record) {
    $out .= "<tr>";
    $out .= "<th scope='row'>".$record["my_id"]."</th>";
    $out .= "<th scope='row'>".$record["friend_id"]."</th>";
    $out .= "<td>".Tracking::printExposureRisk($record['seen_millis'])." ".millis2string($record["seen_millis"])."</td>";
    $out .= "<td>".date('Y-m-d H:i:s', $record["seen_time"]/1000)."</td>";
    $out .= "<td>".time2String($record["created_at"])."</td>";
    $out .= "</tr>";
}


