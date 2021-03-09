<?php
 require_once 'res/resources.php';
 $_CURRENT_PAGE_NAME = 'Tracking detail';
 require_once 'template/header.php';
 require_once 'controller/tracking-detail.php';
?>

<div class="row">
    <div class="col-lg-3">
    <a href="tracking-list.php" class="btn btn-danger btn-sm mb-4">&larr; Go back to List</a>
        <div class="card">
            <div class="card-body">

                <h5 class="card-title text-muted mb-4">Stats</h5>
                Theres a total of <?=$num;?> records

                <?php if(isset($_GET['id'])) { ?>

                <h5 class="card-title text-muted mt-4 mb-3">Longest Exposure time</h5>

                &raquo; Subject: <code><?=$id;?></code>

                <ul class="list-group mt-3">
                <?php 
                        $friends = Tracking::calcultateLongestExposureTime($id, $data); 

                        foreach($friends as $friend => $ftime) {
                            echo ' <li class="list-group-item d-flex justify-content-between align-items-center">
                            '.$friend.'
                            <span class="badge bg-secondary rounded-pill">'.millis2string($ftime).'</span>
                          </li>';
                        }
                    } 
                ?>

                
                </ul>
            </div>
        </div>
    </div>
    <div class="col-lg-9">
        <table class="table table-striped">
            <thead>
                <tr>
                <th scope="col"><i class="fas fa-microchip text-primary"></i> ID-A</th>
                <th scope="col"><i class="fas fa-microchip text-secondary"></i> ID-B</th>
                <th scope="col"><i class="fas fa-history text-danger"></i> Last seen</th>
                <th scope="col"><i class="fas fa-history text-secondary"></i> Last date</th>
                <th scope="col"><i class="fas fa-plus-circle text-secondary"></i> Record registered</th> 
                </tr>
            </thead>
            <tbody>
                <?php 
                   echo printTrackingDetail($data, $num);
                ?>
            </tbody> 
        </table>
    </div>
</div>

<?php
 require_once 'template/footer.php';
?>