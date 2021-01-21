<?php

$cmd = escapeshellcmd("python ../res/python/mqtt_publisher.py");
$out = shell_exec($cmd);
echo $out;


echo "<br><br><font color=green>Done!</font> <a href='../index.php'>Go Back</a>";

// header("Location: ../index.php")

?>