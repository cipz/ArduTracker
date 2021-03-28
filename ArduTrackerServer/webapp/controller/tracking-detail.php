<?php
require_once 'res/resources.php';
$pr = new protection(true);
$db = new database();

if (!isset($_GET["id"]))
    goToLocation('tracking-list.php');


$id = $db->clean($_GET["id"]);
$type = isset($_GET['type']) ? $_GET['type'] : '';
$date_from = isset($_GET['date_from']) ? $db->clean($_GET["date_from"]) : '';
$date_to = isset($_GET['date_to']) ? $db->clean($_GET['date_to']) : '';
$risk = isset($_GET['risk']) ? $db->clean($_GET['risk']) : '';

if (isset($_POST['search'])) {
    goToLocation('?id=' . $id . '&type=' . $type . '&risk=' . $_POST['risk'] . '&date_from=' . $_POST['date_from'] . '&date_to=' . $_POST['date_to']);
}

$add_filters = "";
if (!empty($date_from)) $add_filters .= " AND seen_time >= " . strtotime($date_from);
if (!empty($date_to)) $add_filters .= " AND seen_time <= " . strtotime($date_to);

switch ($risk) {
    case 'high':
        $add_filters .= ' AND seen_millis > ' . (1000 * 60 * MID_RISK_MINUTES);
        break;
    case 'mid':
        $add_filters .= ' AND seen_millis > ' . (1000 * 60 * LOW_RISK_MINUTES) . ' AND seen_millis <= ' . (1000 * 60 * MID_RISK_MINUTES);
        break;
    case 'low':
        $add_filters .= ' AND seen_millis <= ' . (1000 * 60 * LOW_RISK_MINUTES);
        break;
    default:
        break;
}

switch ($type) {
    case 'b2a':
        $sql = "SELECT * FROM tracking_log WHERE my_id='$id' $add_filters ORDER BY created_at DESC";
        break;
    case 'a2b':
        $sql = "SELECT * FROM tracking_log WHERE friend_id='$id' $add_filters ORDER BY created_at DESC";
        break;
    default:
        $sql = "SELECT * FROM tracking_log WHERE (my_id='$id' OR friend_id='$id') $add_filters ORDER BY created_at DESC";
        break;
}

$activeFilters = !empty($add_filters);

$db->query($sql);
$num = $db->num(); // Total number of rows
$db->query($sql . Pagination::limitQuery());
$data = $db->get(); // Get data for the current limited rows

$out = "";
foreach ($data as $record) {
    $out .= "<tr>";
    $out .= "<th scope='row' class='small'>" . $record["my_id"] . "</th>";
    $out .= "<th scope='row' class='small'>" . $record["friend_id"] . "</th>";
    $out .= "<td class='small'>" . Tracking::printExposureRisk($record['seen_millis']) . " " . millis2string($record["seen_millis"]) . "</td>";
    $out .= "<td class='small'>" . $record['rssi'] . " dBm / " . $record['scan_count'] . " scan</td>";
    $out .= "<td class='small'>" . date('Y-m-d H:i:s', $record["seen_time"]) . "</td>";
    $out .= "<td class='small'>" . time2String($record["created_at"]) . "</td>";
    $out .= "</tr>";
}


// ----------- Graph ------------


function childFinder(string $current, string $root, string $parent, int $depth)
{

    global $db, $add_filters;

    if ($depth > GRAPH_MAX_DEPTH_CHILDREN - 1)
        return array("text" => array("name" => $current), "link" => array("href" => "tracking-detail.php?id=$current"));

    $gsql = "SELECT DISTINCT friend_id FROM tracking_log WHERE my_id='$current' AND friend_id NOT IN ('$root', '$parent') $add_filters ORDER BY friend_id ASC";
    $db->query($gsql);

    $children_array = array();
    if ($db->num() == 0) {
        return array("text" => array("name" => $current), "link" => array("href" => "tracking-detail.php?id=$current"));
    } else {
        $data = $db->get();
        foreach ($data as $node) {
            array_push($children_array, childFinder($node['friend_id'], $root, $current, $depth + 1));
        }
        return array(
            "text" => array("name" => $current),
            "link" => array("href" => "tracking-detail.php?id=$current"),
            "children" => $children_array
        );
    }
}

$jsonTree = childFinder($id, $id, $id, 0);
$jsonTreeJs = array("chart" => array("container" => "#tree", "connectors" => array("type" => "curve")), "nodeStructure" => $jsonTree);
$jsonTreeJs = json_encode($jsonTreeJs);

// lvl 0: PIPPO
// lvl 1: Paolo e Pluto, paperino
// lvl 2:  | 
//      Paperino, Pluto
//            |
//            Pippo
//
// id node (root) + parent 
            


// INSERT INTO `tracking_log` (`my_id`, `friend_id`, `seen_millis`, `seen_time`, `scan_count`, `rssi`, `created_at`) VALUES
// ('IRON MAN', 'THOR', 69, 1614019056, 0, 0, '2021-01-05 21:10:49'),
// ('IRON MAN', 'THOR', 2147483647, 1614019056, 0, 0, '2021-01-13 12:12:23'),
// ('IRON MAN', 'THOR', 2147483647, 1614019056, 0, 0, '2021-01-13 12:15:41'),
// ('THOR', 'IRON MAN', 2147483647, 1614019056, 0, 0, '2021-01-13 12:23:08'),
// ('IRON MAN', 'HULK', 2147483647, 1614019056, 0, 0, '2021-01-14 18:51:45'),
// ('BOB', 'CHARLIE', 1000, 8, 1, -28, '2021-03-26 16:29:11'),
// ('CHARLIE', 'ALICE', 119056, 1616776254, 19, -36.42, '2021-03-26 16:31:24'),
// ('BOB', 'ALICE', 127260, 1616776264, 22, -41.14, '2021-03-26 16:31:34'),
// ('CHARLIE', 'BOB', 182437, 1616776318, 27, -48.56, '2021-03-26 16:32:09'),
// ('BOB', 'CHARLIE', 167558, 1616776310, 29, -43.62, '2021-03-26 16:36:17'),
// ('BOB', 'CHARLIE', 1000, 1616776570, 1, -39, '2021-03-26 16:36:38'),
// ('ALICE', 'CHARLIE', 115316, 1616776256, 20, -34.15, '2021-03-26 16:37:38'),
// ('ALICE', 'BOB', 115313, 1616776256, 20, -41.1, '2021-03-26 16:37:38'),
// ('CAP', 'SPIDEY', 20082, 1616853216, 3, 0, '2021-03-27 13:54:19'),
// ('CAP', 'SPIDEY', 5187, 1616853425, 2, 0, '2021-03-27 13:57:23'),
// ('SPIDEY', 'SPIDEY', 6781, 1616854011, 2, -36.5, '2021-03-27 14:07:03'),
// ('CAP', 'SPIDEY', 1000, 1616854128, 1, -26, '2021-03-27 14:10:14');
// COMMIT;