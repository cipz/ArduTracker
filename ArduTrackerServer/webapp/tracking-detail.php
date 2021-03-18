<?php
 require_once 'res/resources.php';
 require_once 'controller/tracking-detail.php';
 $_CURRENT_PAGE_NAME = 'Tracking detail';
 require_once 'template/header.php';
?>

<div class="row">
    <div class="col-lg-3">
    <a href="tracking-list.php" class="btn btn-danger btn-sm mb-4">&larr; Go back to List</a>
        
        <div class="card">
            <div class="card-body">
                <h5 class="card-title text-muted mb-4"><i class="fas fa-filter"></i> Filters</h5>
                <form action="?id=<?=$id;?>&type=<?=$type;?>" method="post">

                    <strong> Date range: </strong>

                    <div class="form-floating my-2">
                        <input type="date" name="date_from" class="form-control" id="floatingDateFrom" value="<?=$date_from?>" >
                        <label for="floatingDateFrom">FROM Date</label>
                    </div>
                    <div class="form-floating my-2">
                        <input type="date" name="date_to" class="form-control" id="floatingDateTo" value="<?=$date_to?>" >
                        <label for="floatingDateTo">TO Date</label>
                    </div>

                    <strong> Exposure level: </strong>

                    <div class="form-check">
                        <input class="form-check-input" type="radio" name="risk" id="inlineAll" value="all" <?=(!in_array($risk, array('low','mid','high')))?'checked':''?>>
                        <label class="form-check-label text-primary" for="inlineAll">All</label>
                    </div>
                    <div class="form-check">
                        <input class="form-check-input" type="radio" name="risk" id="inlineLow" value="low" <?=($risk=='low')?'checked':''?>>
                        <label class="form-check-label text-<?=Tracking::printExposureRiskColor(1000*60*LOW_RISK_MINUTES-0.25);?>" for="inlineLow">Low (< <?=LOW_RISK_MINUTES;?> min)</label>
                    </div>
                    <div class="form-check">
                        <input class="form-check-input" type="radio" name="risk" id="inlineMid" value="mid" <?=($risk=='mid')?'checked':''?>>
                        <label class="form-check-label text-<?=Tracking::printExposureRiskColor(1000*60*MID_RISK_MINUTES-0.25);?>" for="inlineMid">Mid (< <?=MID_RISK_MINUTES;?> min)</label>
                    </div>
                    <div class="form-check">
                        <input class="form-check-input" type="radio" name="risk" id="inlineHigh" value="high" <?=($risk=='high')?'checked':''?>>
                        <label class="form-check-label text-<?=Tracking::printExposureRiskColor(1000*60*MID_RISK_MINUTES+0.25);?>" for="inlineHigh">High (> <?=MID_RISK_MINUTES;?> min)</label>
                    </div>

                    <br>

                    <input type="submit" class="btn btn-success btn-sm" name="search" value="Search"> 
                    <?php if($activeFilters) { ?> 
                        <a href="?id=<?=$id;?>&type=<?=$type;?>" class="btn btn-outline-danger btn-sm">&times; Clear filters</a>
                    <?php } ?>
                </form>
            </div>
        </div>

        <div class="card mt-3">
            <div class="card-body">

                <h5 class="card-title text-muted mb-4"><i class="fas fa-history"></i> Longest Exposure time</h5>

                &raquo; <strong>Subject:</strong> <code><?=$id;?></code>

                <ul class="list-group mt-3">
                <?php 
                        $friends = Tracking::calcultateLongestExposureTime($id, $data); 

                        foreach($friends as $friend => $ftime) {
                            echo ' <li class="list-group-item text-'.Tracking::printExposureRiskColor($ftime).' d-flex justify-content-between align-items-center">
                            '.$friend.'
                            <span class="badge bg-'.Tracking::printExposureRiskColor($ftime).' rounded-pill">'.millis2string($ftime).'</span>
                          </li>';
                        }
                ?>
                </ul>
            </div>
        </div>

        <div class="card mt-3">
            <div class="card-body">

                <h5 class="card-title text-muted mb-4"><i class="fas fa-chart-bar"></i> Stats</h5>
                Theres a total of <?=$num;?> tracking logs.

                </ul>
            </div>
        </div>
    </div>
    <div class="col-lg-9">
        
    <ul class="nav nav-tabs">
        <li class="nav-item">
            <a class="nav-link <?=(!in_array($type,array('b2a','a2b')))?'active':'';?>" href="?id=<?=$id;?>&type=all&date_from=<?=$date_from?>&date_to=<?=$date_to?>&risk=<?=$risk?>">A <i class="fas fa-exchange-alt"></i> B</a>
        </li>
        <li class="nav-item">
            <a class="nav-link <?=($type=='b2a')?'active':'';?>" href="?id=<?=$id;?>&type=b2a&date_from=<?=$date_from?>&date_to=<?=$date_to?>&risk=<?=$risk?>">A <i class="fas fa-long-arrow-alt-left"></i> B</a>
        </li>
        <li class="nav-item">
            <a class="nav-link <?=($type=='a2b')?'active':'';?>" href="?id=<?=$id;?>&type=a2b&date_from=<?=$date_from?>&date_to=<?=$date_to?>&risk=<?=$risk?>">A <i class="fas fa-long-arrow-alt-right"></i> B</a>
        </li>
    </ul>

    <table class="table table-striped">
        <thead>
            <tr class="bg-white">
            <th scope="col"><i class="fas fa-microchip text-primary"></i> ID-A</th>
            <th scope="col"><i class="fas fa-microchip text-secondary"></i> ID-B</th>
            <th scope="col"><i class="fas fa-history text-danger"></i> Total exposure</th>
            <th scope="col"><i class="fas fa-history text-secondary"></i> Date and time</th>
            <th scope="col"><i class="fas fa-plus-circle text-secondary"></i> Record Creation</th> 
            </tr>
        </thead>
        <tbody>
            <?php 
                echo $out;
            ?>
        </tbody> 
    </table>
    <?=($num>0) ? Pagination::printMenu($num, '&id='.$id.'&type='.$type.'&date_from='.$date_from.'&date_to='.$date_to.'&risk='.$risk) : 'No record found <i class="far fa-frown"></i>';?>
    </div>
</div>

<?php
 require_once 'template/footer.php';
?>