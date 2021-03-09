<?php

class database
{

    public $mysqli;
    private $fetchMode = MYSQLI_BOTH;
    private $result;
    

    public function  __construct($db = NULL) 
    {
        if($db == NULL)
        {
            global $_DBC;
            $db = $_DBC;
        }
        
        $this->mysqli = new mysqli($db['host'], $db['user'], $db['password'], $db['database']);

        if(mysqli_connect_errno()) 
        {
            printf("<h1>Ardu Tracker - Webapp - Fatal Error</h1><br><b>Connection to database failed:</b> %s\n", mysqli_connect_error());
            exit;
        }

    }
    

    public function setFetchMode($type)
    {
        switch($type)
        {           
            case 1:
            $this->fetchMode = MYSQLI_NUM;
            break;
            
            case 2:
            $this->fetchMode = MYSQLI_ASSOC;
            break;
            
            default:
            $this->fetchMode = MYSQLI_BOTH;
            break;

        }

    }

    
    public function query($SQL)
    {
        global $_DEBUG;
        
        $this->result = $this->mysqli->query($SQL);

        if ($this->result == true)
        {
            return true;
        }
        else
        {
            if($_DEBUG)
            {
                printf("<b>SQL query invalid:</b> %s\n", $this->SQL);
                print("<br>".$this->mysqli->error);
                exit;
            }
            else
                return false;
        }
    }



    public function Clean($param, $striptags=1)
    {   
        if($striptags) $param = strip_tags($param);
        return $this->mysqli->real_escape_string($param);
    }

    public function num()
    {
        return $this->result->num_rows;
    }

    public function get($field = NULL)
    {
        if ($field == NULL)
        {
            $data = array();

            while ($row = $this->result->fetch_array($this->fetchMode))
            {
                $data[] = $row;
            }
        }
        else
        {
            $row = $this->result->fetch_array($this->fetchMode);
            $data = $row[$field];
        }

        $this->result->close();

        return $data;

    }
    
    
    public function new_id()
    {
        return $this->mysqli->insert_id;
    }

    public function status(){
        return $this->mysqli->affected_rows > 0 ? 1 : 0;
    }
    
    public function __destruct()
    {
        $this->mysqli->close();
    }
}

?>