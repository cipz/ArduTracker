<?php

// -----------------------------------
// Boards - Class Functions

class Board {

    static function getTotalRecords($table="tracking_log") {
        global $_DBC;
        $db = new database();
        $db->query('SELECT * FROM '.$table);
        return $db->num();
    }

    static function getRecentBoards($table="tracking_log") {
        global $_DBC;
        $db = new database();
        $db->query('SELECT my_id,created_at FROM '.$table.' WHERE created_at > (NOW() - INTERVAL 100 DAY) ORDER BY created_at DESC');
        return $db->get();
    }

    static function printBoards($boards) {

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

}