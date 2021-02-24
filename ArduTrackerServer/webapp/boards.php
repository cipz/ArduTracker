<?php
 require_once 'res/resources.php';
 $_CURRENT_PAGE_NAME = 'Boards';
 require_once 'template/header.php';
?>

<div class="row">
    <div class="col-lg-3">
        <div class="card">
            <div class="card-body">
                <a href="index.php" class="btn btn-danger btn-sm mb-4">&larr; Go back Home</a>
            </div>
        </div>
    </div>
    <div class="col-lg-9">
        <div class="row">
            <?php printBoards(getTotalBoards()); ?>
        </div>
    </div>
</div>

<?php
 require_once 'template/footer.php';
?>