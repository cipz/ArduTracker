<?php 

// -----------------------------------
// Functions

function getTotalRecords($table="tracking_log") {
	global $_DBC;
	$db = new database();
	$db->query('SELECT * FROM '.$table);
	return $db->num();
}

function getRecentBoards($table="tracking_log") {
	global $_DBC;
	$db = new database();
	$db->query('SELECT my_id,created_at FROM '.$table.' WHERE created_at > (NOW() - INTERVAL 100 DAY) ORDER BY created_at DESC');
	return $db->get();
}

function printBoards($boards) {

	if(empty($boards)) {
		echo "No boards available.";
		return array();
	}

	$names = array();
	foreach($boards as $board)
	{
		$board = (object) $board;
		if(in_array($board->my_id,$names))
			continue;

		$names[] = $board->my_id;
		$online = (strtotime($board->created_at) > time()-60*10);
		$label = $online ? "<span class='badge rounded-pill bg-success'><i class='fas fa-toggle-on'></i> Active</span> in the last 10 min" : "<span class='badge rounded-pill bg-danger'><i class='fas fa-toggle-off'></i> Offline</span>";

		echo '<div class="col-md-4 mb-4">
			<div class="card">
				<div class="card-body">
					<h5 class="card-title"><i class="fas fa-microchip text-muted"></i> <a class="" href="tracking-detail.php?id='.$board->my_id.'">'.$board->my_id.'</a></h5>
					<p class="card-text">'.$label.'</p>
					<small><i class="fas fa-history"></i> '.time2String($board->created_at).'</small>
				</div>
			</div>
		</div>';
	}
	
	return $names;	
}


// Thanks stack overflow :)
function time2String($datetime, $full = false, $ago = true) {
    $now = new DateTime;
    $ago = new DateTime($datetime);
    $diff = $now->diff($ago);

    $diff->w = floor($diff->d / 7);
    $diff->d -= $diff->w * 7;

    $string = array(
        'y' => 'year',
        'm' => 'month',
        'w' => 'week',
        'd' => 'day',
        'h' => 'hour',
        'i' => 'minute',
        's' => 'second',
    );
    foreach ($string as $k => &$v) {
        if ($diff->$k) {
            $v = $diff->$k . ' ' . $v . ($diff->$k > 1 ? 's' : '');
        } else {
            unset($string[$k]);
        }
    }

    if (!$full) $string = array_slice($string, 0, 1);
    return $string ? implode(', ', $string) . ($ago ? ' ago' : '') : 'just now';
}

function millis2string($millis) {
	$sec = $millis / 1000;
	if($sec > 60)
		return floor($sec/60) . ' min, ' . $sec%60 . ' seconds';
	else
		return $sec%60 . ' seconds';
}

function calcultateLongestExposureTime($subject, $haystack) {

	$friends = array();
	foreach($haystack as $exp) {
		if($exp['my_id'] == $subject) {

			if(!isset($friends[$exp['friend_id']])) 
				$friends[$exp['friend_id']] = $exp['seen_millis'];
			else
				$friends[$exp['friend_id']] = max($friends[$exp['my_friend']], $exp['seen_millis']);
		}
	}
	arsort($friends);
	return $friends;
}