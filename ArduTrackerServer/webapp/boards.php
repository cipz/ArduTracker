<?php
 require_once 'res/resources.php';
 $_CURRENT_PAGE_NAME = 'Boards';
 require_once 'template/header.php';
?>

<div class="row">
    <div class="col-lg-12">
        <div class="row">
            <?php $nboards = Board::printBoards(Board::getRecentBoards()); ?>
        </div>
        There is a total of <?=count($nboards);?> boards discovered.
    </div>
</div>

<?php
 require_once 'template/footer.php';
?>