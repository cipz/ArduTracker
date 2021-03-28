<?php

// -----------------------------------
// Tracking - Functions

class Tracking {


    static function calcultateLongestExposureTime($subject, $haystack) {
        $friends = array();
        foreach($haystack as $exp) {
            if($exp['my_id'] == $subject) {

                if(!isset($friends[$exp['friend_id']])) 
                    $friends[$exp['friend_id']] = $exp['seen_millis'];
                else
                    $friends[$exp['friend_id']] = max($friends[$exp['friend_id']], $exp['seen_millis']);
            }
        }
        arsort($friends);
        return $friends;
    }

    static function printExposureRisk($millis) {
        if($millis/1000 < 60*LOW_RISK_MINUTES)
            return "<span class='badge bg-secondary'>Low</span>";
        elseif($millis/1000 < 60*MID_RISK_MINUTES)
            return "<span class='badge bg-warning'>Mid</span>";
        else 
            return "<span class='badge bg-danger'>High</span>";        
    }

    static function printExposureRiskColor($millis) {
        if($millis/1000 < 60*LOW_RISK_MINUTES)
            return "secondary";
        elseif($millis/1000 < 60*MID_RISK_MINUTES)
            return "warning";
        else 
            return "danger";        
    }

}