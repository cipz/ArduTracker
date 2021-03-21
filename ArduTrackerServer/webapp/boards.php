<?php
require_once 'controller/boards.php';
$_CURRENT_PAGE_NAME = 'Boards';
require_once 'template/header.php';
?>

<script src="res/javascript/utils.js"></script>
<script src="res/javascript/board-detail.js"></script>

<div class="row">
    <div class="col-lg-12">
        <div class="card my-3">
            <div class="card-header">
                <i class="fas fa-plus-circle"></i> Add a new board
            </div>
            <div class="card-body">
                <?=(isset($out) ? $out : '');?>
                <form action="?" method="post">
                    <div class="row g-2 my-2">
                        <div class="col-md">
                            <div class="form-floating">
                                <input type="text" required maxlength="15" class="form-control" id="idBoardInput" placeholder="Board ID" name="id">
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
                    <button type="submit" name="AddBoard" class="btn btn-success btn-sm"><i class="fas fa-check"></i> Confirm</button>
                </form>
            </div>
        </div>

        <div class="card my-4">
            <div class="card-header">
                <i class="fas fa-microchip"></i> Registered Boards
            </div>
            <div class="card-body">
                
            <div class="input-group mb-3">
                <span class="input-group-text" id="search"><i class='fas fa-search'></i></span>
                <input type="text" class="form-control" placeholder="Search by ID" aria-label="Search by ID" aria-describedby="search" onkeyup="searchIds()" id="input-search-id">
            </div>

                <div class="table-responsive">
                    <table class="table table-striped">
                        <thead>
                            <tr>
                                <th scope="col">IDs</th>
                                <th scope="col">MAC Address</th>
                                <th scope="col">Mode</th>
                                <th scope="col">Configuration</th>
                                <th scope="col">Last update</th>
                                <th scope="col">Creation date</th>
                                <th scope="col">View logs</th>
                            </tr>
                        </thead>
                        <tbody id="ids-list">
                            <?php
                            $registeredBoards = Board::getRegisteredBoards();
                            $rboards = Board::printBoardsInTable($registeredBoards);
                            ?>
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
    </div>
</div>

<?php
require_once 'template/footer.php';
?>