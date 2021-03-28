<?php

// -----------------------------------
// Boards - Class Functions

class Board {

    public $id;
    public $idMac;
    public $config;
    public $newConfigSent;
    public $createdAt;
    public $updatedAt;

    private function getBoardInfoFromId($id) {
        $db = new database();
        $id = $db->clean($id);
        $db->query('SELECT id_board,id_mac,configuration,new_config_sent,created_at,updated_at FROM tracking_board WHERE id_board = "'.$id.'" LIMIT 1');
        return $db->get();
    }

    public function __construct() {
        $this->idBoard = 'undefined';
    }

    public function constructFromId($id) {
        if($this->checkIdBoard($id)) {
            list($this->id, 
                $this->idMac,
                $this->config,
                $this->newConfigSent,
                $this->createdAt,
                $this->updatedAt) = $this->getBoardInfoFromId($id)[0];
        }
        else
        {
            $this->id = 'undefined';
        }
    }

    public function constructFromSqlArray($array) {
        list($this->id, 
            $this->idMac,
            $this->config,
            $this->newConfigSent,
            $this->createdAt,
            $this->updatedAt) = $array[0];
    }

    public function deleteBoard() {
        $db = new database();
        $db->query('DELETE FROM tracking_board WHERE id_board = "'.$this->id.'" LIMIT 1');
        return $db->status();
    }

    public function updateNewConfigSent($val){
        $db = new database();
        $val = (int) $val;
        $db->query('UPDATE tracking_board SET new_config_sent = "'.$val.'" WHERE id_board = "'.$this->id.'" LIMIT 1');
        $this->newConfigSent = $val;
        return $db->status();
    }

    public function updateConfiguration($newconfig) {
        $db = new database();
        $newconfigEscaped = $db->clean($newconfig);
        $db->query('UPDATE tracking_board SET configuration = "'.$newconfigEscaped.'", new_config_sent="0" WHERE id_board = "'.$this->id.'" LIMIT 1');
        $this->config = $newconfig;
        $this->newConfigSent = 0;
        return $db->status();
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
        $db->query('SELECT * FROM tracking_board ORDER BY created_at DESC');
        return $db->get();
    }


    static public function addNewBoard($idb, $mac) {
        global $_DEFAULT_BOARD_CONFIG;
        $db = new database();
        $id = $db->clean($idb);
        $mac = $db->clean($mac);
        $defaultConfig = $_DEFAULT_BOARD_CONFIG;
        $defaultConfig[BOARD_ID_CONFIG_FIELD] = $idb;
        $defaultConfigEncoded = json_encode($defaultConfig);
        $db->query("INSERT INTO tracking_board (id_board, id_mac, configuration) VALUES ('$id', '$mac', '$defaultConfigEncoded')");
        return $db->status();
    }

    static public function printBoardsInTable($boards) {
        
        if(empty($boards)) {
            echo "<p class='my-3'>No boards available.</p>";
        }
        foreach($boards as $board)
        {
            $board = (object) $board;
            
            $online = (strtotime($board->created_at) > time()-60*10);
            $label = $board->new_config_sent ? "<span class='badge rounded-pill bg-success'><i class='fas fa-spin fa-sync-alt'></i> Synced</span>" : "<span class='badge rounded-pill bg-danger'><i class='fas fa-times'></i> Not synced</span>";
            $config = json_decode($board->configuration, true);
            $type = (isset($config[BOARD_MODE_CONFIG_FIELD])) ? $config[BOARD_MODE_CONFIG_FIELD] : "undefined";

            echo '<tr>
                    <td>
                        <i class="fas fa-edit text-muted"></i> <a class="text-success" href="board-detail.php?id='.$board->id_board.'">'.$board->id_board.'</a>
                    </td>
                    <td><code>'.$board->id_mac.'</code></td>
                    <td>'.$type.'</td>
                    <td>'.$label.'</td>
                    <td>
                        <small><i class="fas fa-history"></i> '.time2String($board->updated_at).'</small>
                    </td>
                    <td><small><i class="fas fa-history"></i> '.$board->created_at.'</small></td>
                    <td class="small">
                        &raquo; <a href="tracking-detail.php?id='.$board->id_board.'">Go to tracking log</a>
                    </td>';        
        }
    }


    // Functions from Tracking logs

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

    static function printBoardsFromTrackingLog($boards, $printTime=true) {

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