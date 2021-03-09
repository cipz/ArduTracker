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
                    $friends[$exp['friend_id']] = max($friends[$exp['my_friend']], $exp['seen_millis']);
            }
        }
        arsort($friends);
        return $friends;
    }

}