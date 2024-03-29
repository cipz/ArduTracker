<?php
require_once 'controller/tracking-detail.php';
$_TITLE_CURRENT_PAGE_NAME = 'Tracking detail - ' . $id;
$_CURRENT_PAGE_NAME = '<a href="tracking-list.php" class="text-secondary">Tracking list</a>';
$_EXTRA_CURRENT_PAGE_NAME = '&raquo; <span class="small text-secondary">' . $id . '</span>';
require_once 'template/header.php';
?>

<link rel="stylesheet" href="res/javascript/treant-graph/Treant.css" type="text/css">

<script src="res/javascript/treant-graph/vendor/raphael.js"></script>
<script src="res/javascript/treant-graph/Treant.js"></script>

<div class="row">
    <div class="col-lg-3">
        <div class="card">
            <div class="card-body">
                <h5 class="card-title text-muted mb-4"><i class="fas fa-filter"></i> Filters</h5>
                <form action="?id=<?= $id; ?>&type=<?= $type; ?>" method="post">

                    <div class="mb-3">
                        <p class="small mb-0 text-secondary">Graph settings</p>
                        <div class="form-floating my-2">
                            <input type="number" name="gdepth" class="form-control" id="graphDepthForm" value="<?= $gdepth; ?>">
                            <label for="graphDepthForm">Graph nodes depth</label>
                        </div>
                    </div>
                    <div class="mb-3">
                        <p class="small mb-0 text-secondary">Data Range</p>

                        <div class="form-floating my-2">
                            <input type="date" name="date_from" class="form-control" id="floatingDateFrom" value="<?= $date_from ?>">
                            <label for="floatingDateFrom">FROM Date</label>
                        </div>
                        <div class="form-floating my-2">
                            <input type="date" name="date_to" class="form-control" id="floatingDateTo" value="<?= $date_to ?>">
                            <label for="floatingDateTo">TO Date</label>
                        </div>
                    </div>
                    <div class="mb-0">
                        <p class="small mb-1 text-secondary">Exposure Level</p>

                        <div class="form-check">
                            <input class="form-check-input" type="radio" name="risk" id="inlineAll" value="all" <?= (!in_array($risk, array('low', 'mid', 'high'))) ? 'checked' : '' ?>>
                            <label class="form-check-label text-primary fw-bold" for="inlineAll">All</label>
                        </div>
                        <div class="form-check">
                            <input class="form-check-input" type="radio" name="risk" id="inlineLow" value="low" <?= ($risk == 'low') ? 'checked' : '' ?>>
                            <label class="form-check-label text-<?= Tracking::printExposureRiskColor(1000 * 60 * LOW_RISK_MINUTES - 0.25); ?> fw-bold" for="inlineLow">Low <small>(< <?= LOW_RISK_MINUTES; ?> min)</small></label>
                        </div>
                        <div class="form-check">
                            <input class="form-check-input" type="radio" name="risk" id="inlineMid" value="mid" <?= ($risk == 'mid') ? 'checked' : '' ?>>
                            <label class="form-check-label text-<?= Tracking::printExposureRiskColor(1000 * 60 * MID_RISK_MINUTES - 0.25); ?> fw-bold" for="inlineMid">Mid <small>(< <?= MID_RISK_MINUTES; ?> min)</small></label>
                        </div>
                        <div class="form-check">
                            <input class="form-check-input" type="radio" name="risk" id="inlineHigh" value="high" <?= ($risk == 'high') ? 'checked' : '' ?>>
                            <label class="form-check-label text-<?= Tracking::printExposureRiskColor(1000 * 60 * MID_RISK_MINUTES + 0.25); ?> fw-bold" for="inlineHigh">High <small>(> <?= MID_RISK_MINUTES; ?> min)</small></label>
                        </div>
                        <br>
                        <input type="submit" class="btn btn-success btn-sm" name="search" value="Search">
                        <?php if ($activeFilters) { ?>
                            <a href="?id=<?= $id; ?>&type=<?= $type; ?>" class="btn btn-outline-danger btn-sm">&times; Clear filters</a>
                        <?php } ?>
                    </div>
                </form>
            </div>
        </div>

        <div class="card mt-3">
            <div class="card-body">

                <h5 class="card-title text-muted h6 mb-4"><i class="fas fa-history"></i> Longest Exposure Session</h5>

                <div class="mb-3">
                    <p class="small mb-0 text-secondary">Subject</p>
                    <code><?= $id; ?></code>
                </div>

                <div class="mb-0">
                    <p class="small mb-2 text-secondary">Exposures</p>

                    <ul class="list-group">
                        <?php
                        $friends = Tracking::calcultateLongestExposureTime($id, $data);
                        if(empty($friends))
                            echo "<li class='list-group-item small'>No contacts with the subject in the current selection <i class='far fa-smile'></i></li>";
                        else
                        foreach ($friends as $friend => $ftime) {
                            echo ' <li class="list-group-item text-' . Tracking::printExposureRiskColor($ftime) . ' d-flex justify-content-between align-items-center">
                            ' . $friend . '
                            <span class="badge bg-' . Tracking::printExposureRiskColor($ftime) . ' rounded-pill">' . millis2string($ftime) . '</span>
                          </li>';
                        }
                        ?>
                    </ul>
                </div>
            </div>
        </div>

        <div class="card my-3">
            <div class="card-body">

                <h5 class="card-title text-muted mb-4"><i class="fas fa-chart-bar"></i> Stats</h5>

                <div class="mb-0">
                    <p class="small mb-0 text-secondary">Total tracking logs</p>
                    <?= $num; ?> logs
                </div>
            </div>
        </div>
    </div>
    <div class="col-lg-9">
        <ul class="nav nav-pills">
            <li class="nav-item">
                <a class="nav-link <?= (!in_array($type, array('b2a', 'a2b'))) ? 'active' : ''; ?>" href="?id=<?= $id; ?>&type=all&date_from=<?= $date_from ?>&date_to=<?= $date_to ?>&risk=<?= $risk ?>"><i class="fas fa-exchange-alt"></i> All</a>
            </li>
            <li class="nav-item">
                <a class="nav-link <?= ($type == 'b2a') ? 'active' : ''; ?>" href="?id=<?= $id; ?>&type=b2a&date_from=<?= $date_from ?>&date_to=<?= $date_to ?>&risk=<?= $risk ?>"><i class="fas fa-compress-arrows-alt"></i> ID-A as Receiver</a>
            </li>
            <li class="nav-item">
                <a class="nav-link <?= ($type == 'a2b') ? 'active' : ''; ?>" href="?id=<?= $id; ?>&type=a2b&date_from=<?= $date_from ?>&date_to=<?= $date_to ?>&risk=<?= $risk ?>"> <i class="fas fa-expand-arrows-alt"></i> ID-A as Trasmitter</a>
            </li>
        </ul>
        
        <div class="card my-3 ">
            <div class="card-body">
                
                <div id="tree"> </div>
                <script>
                    var jsonTree = <?= $jsonTreeJs; ?>; 
                    var my_chart = new Treant(jsonTree);
                </script>
            </div>
        </div>

        <table class="table table-striped">
            <thead>
                <tr class="bg-white">
                    <th scope="col"><i class="fas fa-microchip text-primary"></i> ID-A</th>
                    <th scope="col"><i class="fas fa-microchip text-info"></i> ID-B</th>
                    <th scope="col"><i class="fas fa-history text-danger"></i> Exposure session</th>
                    <th scope="col" class="small"><i class="fas fa-satellite-dish text-warning"></i> RSSI / Scan count</th>
                    <th scope="col"><i class="fas fa-history text-secondary"></i> Date and time</th>
                    <th scope="col"><i class="fas fa-plus-circle text-success"></i> Register date</th>
                </tr>
            </thead>
            <tbody>
                <?php
                echo $out;
                ?>
            </tbody>
        </table>
        <?= ($num > 0) ? Pagination::printMenu($num, '&id=' . $id . '&type=' . $type . '&date_from=' . $date_from . '&date_to=' . $date_to . '&risk=' . $risk . '&gdepth=' . $gdepth) : 'No record found <i class="far fa-frown"></i>'; ?>

    </div>
</div>




<?php
require_once 'template/footer.php';
?>