<?php
 require_once 'controller/boards.php';
 $_CURRENT_PAGE_NAME = 'Boards';
 require_once 'template/header.php';
?>

<div class="row">
    <div class="col-lg-12">
           <h4>Registered Boards</h4>

           <div class="card">
                <div class="card-body"> 
                    <form action="?" method="post">
                        <div class="row g-2 my-2">
                            <div class="col-md">
                                <div class="form-floating">
                                    <input type="text" required maxlength="64" class="form-control" id="idBoardInput" placeholder="Board ID" name="id">
                                    <label for="idBoardInput">Board ID (custom name)</label>
                                </div>
                            </div>
                            <div class="col-md">
                                <div class="form-floating">
                                    <input type="text" required maxlength="17" class="form-control" id="macBoardInput" placeholder="Board MAC address" pattern="([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})" name="mac">
                                    <label for="macBoardInput">Board MAC address (from network card)</label>
                                </div>
                            </div>
                        </div>
                        <input type="submit" name="AddBoard" value="Add a new board" class="btn btn-success btn-block">
                    </form>
                </div>
            </div>
           
           
                <?php 
                    $registeredBoards = Board::getRegisteredBoards();
                    $rboards = Board::printBoards($registeredBoards); 
                ?>

            <h4>Unknown Discovered Boards</h4>
            <?php 
                $recentBoards = Board::getRecentBoards();
                $unknownBoards = array_udiff($recentBoards, $registeredBoards, function($e1, $e2) {
                    return $e1['my_id'] == $e2['my_id'];
                });
                $uboards = Board::printBoards($unknownBoards); 
            ?>

        There is a total of <?=count($rboards + $uboards);?> boards discovered.
    </div>
</div>

<script>
document.getElementById("macBoardInput").addEventListener('keyup', function() { 
  this.value = 
    (this.value.toUpperCase()
    .replace(/[^\d|A-F]/g, '')
    .match(/.{1,2}/g) || [])
    .join(":")
});
</script>

<?php
 require_once 'template/footer.php';
?>