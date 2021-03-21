<?php
require_once 'controller/index.php';
$_CURRENT_PAGE_NAME = 'Homepage';
require_once 'template/header.php';
?>

<div class="row">
    <div class="col-lg-4">
        <div class="card">
            <div class="card-body">

                <h5 class="card-title text-muted mb-4">Welcome to the ArduTrack Project</h5>

                <div class="m-auto text-center">
                    <img src="assets/images/logo.png" alt="ArduTracker" width=120 height=120 class="my-3">
                </div>

                <p class="card-text">This web application is a proof of concept used to visualize data tracking from the ArduTracker components.</p>

                <p class="card-text">Please, follow the links above to navigate inside this site.</p>

            </div>
        </div>
    </div>
    <div class="col-lg-8">
        <div class="card">
            <div class="card-body">

                <?=$out;?>

                <h5 class="card-title text-muted mb-4">Recent Tracked Boards</h5>
                <?php
                $recentBoards = Board::getRecentBoards();
                $uboards = Board::printBoardsFromTrackingLog($recentBoards);
                ?>

                <p>There are <b><?= count($uboards); ?> boards</b> active in the last 48 hours.</p>

                <?php if(DEBUG) : ?>
                <hr>

                <h5 class="card-title text-danger mb-4">Debug features</h5>
                <a href="controller/publish.php" class="btn btn-sm btn-secondary"><i class="far fa-paper-plane"></i> Generate tracking logs</a>
                <?php endif; ?>

            </div>
        </div>
    </div>
</div>

<?php
require_once 'template/footer.php';
?>