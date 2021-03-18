<?php

// -----------------------------------
// Boards - Class Functions

class Board {

    public $id;
    public $idMac;
    public $config;
    public $new_config_sent;
    public $created_at;
    public $updated_at;

    private function getBoardInfoFromId($id) {
        $db = new database();
        $db->query('SELECT id_board,id_mac,configuration,new_config_sent,created_at,updated_at FROM tracking_board WHERE id_board = "'.$id.'" LIMIT 1');
        return $db->get();
    }

    public function __construct($id) {

        if($this->checkIdBoard($id)) {
            list($this->id, 
                $this->idMac,
                $this->config,
                $this->new_config_sent,
                $this->created_at,
                $this->updated_at) = $this->getBoardInfoFromId($id)[0];
        }
        else
        {
            $this->idBoard = 'undefined';
        }
    }

    static public function checkIdBoard($id){
        $db = new database();
        $db->query('SELECT * FROM tracking_board WHERE id_board = "'.$id.'"');
        return $db->exists();
    }

    static public function checkMacBoard($mac){
        $db = new database();
        $db->query('SELECT * FROM tracking_board WHERE id_mac = "'.$mac.'"');
        return $db->exists();
    }

    static public function getRegisteredBoards() {
        $db = new database();
        $db->query('SELECT id_board as my_id FROM tracking_board ORDER BY created_at DESC');
        return $db->get();
    }


    static public function addNewBoard($id, $mac) {
        global $_DEFAULT_BOARD_CONFIG;
        $db = new database();
        $id = $db->clean($id);
        $mac = $db->clean($mac);
        $defaultConfig = json_encode($_DEFAULT_BOARD_CONFIG);
        $db->query("INSERT INTO tracking_board (id_board, id_mac, configuration) VALUES ('$id', '$mac', '$defaultConfig')");
        return $db->status();
    }


    // Boards from Tracking logs


    static function getTotalRecords($table="tracking_log") {
        $db = new database();
        $db->query('SELECT * FROM '.$table);
        return $db->num();
    }

    static function getRecentBoards($table="tracking_log") {
        $db = new database();
        $db->query('SELECT my_id,created_at FROM '.$table.' WHERE created_at > (NOW() - INTERVAL 365 DAY) ORDER BY created_at DESC');
        return $db->get();
    }

    static function printBoards($boards, $printTime=true) {

        if(empty($boards)) {
            echo "<p class='my-3'>No boards available.</p>";
            return array();
        }

        $names = array();
        echo '<div class="row">';
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
                        <h5 class="card-title"><i class="fas fa-microchip text-muted"></i> <a class="" href="tracking-detail.php?id='.$board->my_id.'">'.$board->my_id.'</a></h5>';
            if($printTime)
                echo '  <p class="card-text">'.$label.'</p>
                        <small><i class="fas fa-history"></i> '.time2String($board->created_at).'</small>';
            echo '
                    </div>
                </div>
            </div>';
        }
        echo '</div>';
        
        return $names;	
    }

}