<?php
 require_once 'res/resources.php';
 $_CURRENT_PAGE_NAME = 'Tracking list';
 require_once 'template/header.php';
 require_once 'controller/tracking-list.php';
?>

<div class="row">
    <div class="col-lg-3">
        <div class="card">
            <div class="card-body">
                <h5 class="card-title text-muted mb-4">Stats</h5>
                <strong>Total tracking logs:</strong> <?=Board::getTotalRecords();?>
            </div>
        </div>
    </div>
    <div class="col-lg-9">
        <table class="table table-striped">
            <thead>
                <tr>
                    <th scope="col">IDs</th>
                </tr>
            </thead>
            <tbody>
                <?php 
                   echo printTrackingList($data, $num);
                ?>
            </tbody> 
        </table>
    </div>
</div>

<?php
 require_once 'template/footer.php';
?>