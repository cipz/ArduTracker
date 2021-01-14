<?php

$cmd = escapeshellcmd("python ../res/mqtt_publisher.py");
$out = shell_exec($cmd);
echo $out;

header("Location: ../index.php")

?>