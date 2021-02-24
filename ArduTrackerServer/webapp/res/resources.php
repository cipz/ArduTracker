<?php

require_once 'config.php';
require_once 'database.php';

// -----------------------------------
// Functions

function getTotalRecords($table="tracking_log") {
	global $_DBC;
	$db = new database();
	$db->query('SELECT * FROM '.$table);
	return $db->num();
}

function getTotalBoards($table="tracking_log") {
	global $_DBC;
	$db = new database();
	$db->query('SELECT DISTINCT my_id, created_at FROM '.$table.' ORDER BY created_at DESC');
	return $db->get();
}

function printBoards($boards) {
	if(empty($boards))
		echo "No boards available.";
	else
	{
		$names = array();
		foreach($boards as $board)
		{
			

			$board = (object) $board;
			$online = (strtotime($board->created_at) > time()-60*10);
			$label = $online ? "<span class='badge rounded-pill bg-success'>Active</span>" : "<span class='badge rounded-pill bg-danger'>Offline</span>";

			echo '<div class="col-md-4">
				<div class="card">
					<div class="card-body">
						<h5 class="card-title"><i class="fas fa-microchip"></i> '.$board->my_id.'</h5>
						<p class="card-text">'.$label.'</p>
						<small><em>Last time online:</em> '.$board->created_at.'</small>
					</div>
				</div>
			</div>';
		}
	}
	
}