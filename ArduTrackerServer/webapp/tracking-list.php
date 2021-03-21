<?php
require_once 'controller/tracking-list.php';
$_CURRENT_PAGE_NAME = 'Tracking list';
require_once 'template/header.php';
?>

<script src="res/javascript/utils.js"></script>

<div class="row">
    <div class="col-lg-3">
        <div class="card">
            <div class="card-body">
                <h5 class="card-title text-muted mb-4"><i class="fas fa-chart-bar"></i> Stats</h5>
                <div class="mb-3">
                    <p class="small mb-0 text-secondary">Total tracking logs</p>
                    <?= Board::getTotalRecords(); ?> logs
                </div>
                <div class="mb-0">
                    <p class="small mb-0 text-secondary">Total boards discovered:</p>
                    <?= count($data); ?> boards
                </div>
            </div>
        </div>
        <div class="card mt-3">
            <div class="card-body">
                <h5 class="card-title text-muted mb-4"><i class="fas fa-info-circle"></i> Information</h5>
                Click on the board ID to show the complete tracking list.
            </div>
        </div>
    </div>
    <div class="col-lg-9">

        <div class="input-group mb-3">
            <span class="input-group-text" id="search"><i class='fas fa-search'></i></span>
            <input type="text" autofocus class="form-control" placeholder="Search by ID" aria-label="Search by ID" aria-describedby="search" onkeyup="searchIds()" id="input-search-id">
        </div>

        <table class="table table-striped">
            <thead>
                <tr>
                    <th scope="col">IDs</th>
                </tr>
            </thead>
            <tbody id="ids-list">
                <?php
                echo $out;
                ?>
            </tbody>
        </table>
    </div>
</div>


<?php
require_once 'template/footer.php';
?>