<?php

// -----------------------------------
// Utils - Functions

function time2String($datetime, $full = false, $ago = true)
{
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

function millis2string($millis)
{
    $sec = $millis / 1000;
    if ($sec > 60)
        return floor($sec / 60) . ' min, ' . $sec % 60 . ' sec';
    else
        return $sec % 60 . ' seconds';
}


function goToLocation($url)
{
    echo "<script>location.replace('$url');</script>";
}

function errorBox($txt)
{
    return "<div class='alert alert-danger'><i class='fas fa-times'></i> <b>Error</b> - $txt</div>";
}

function successBox($txt)
{
    return "<div class='alert alert-success'><i class='fas fa-check'></i> <b>Success</b> - $txt</div>";
}

function isValidJson($string)
{
    json_decode($string);
    return (json_last_error() == JSON_ERROR_NONE);
}
