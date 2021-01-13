<?php
 require_once 'res/resources.php';
 $_CURRENT_PAGE_NAME = 'Tracking detail';
 require_once 'template/header.php';
?>

<div class="row">
    <div class="col-lg-3">
        <div class="card">
            <div class="card-body">
                <a href="tracking-list.php" class="btn btn-danger btn-sm mb-4">&larr; Go back</a>

                <!--<h5 class="card-title text-muted mb-4">Stats</h5>-->
            </div>
        </div>
    </div>
    <div class="col-lg-9">
        <table class="table table-striped">
            <thead>
                <tr>
                <th scope="col">ID-A</th>
                <th scope="col">ID-B</th>
                <th scope="col">Last seen (millis)</th>
                <th scope="col">Last date</th>
                <th scope="col">Created at</th> 
                </tr>
            </thead>
            <tbody>
                <?php 
                    require_once 'controller/tracking-detail.php';
                ?>
            </tbody> 
        </table>
    </div>
</div>

<?php
 require_once 'template/footer.php';
?>