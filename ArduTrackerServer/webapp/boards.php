<?php
require_once 'controller/boards.php';
$_CURRENT_PAGE_NAME = 'Boards';
require_once 'template/header.php';
?>

<div class="row">
    <div class="col-lg-12">

        <h4>Boards Management Dashboard</h4>
        <div class="card my-3">
        <div class="card-header">
                <i class="fas fa-plus-circle"></i> Add a new board
            </div>
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
                    <input type="submit" name="AddBoard" value="Confirm" class="btn btn-success btn-block">
                </form>
            </div>
        </div>

        <div class="card my-4">
            <div class="card-header">
                <i class="fas fa-microchip"></i> Registered Boards List
            </div>
            <div class="card-body">
                <div class="table-responsive">
                    <table class="table table-striped">
                        <thead>
                            <tr>
                                <th scope="col">IDs</th>
                                <th scope="col">MAC Address</th>
                                <th scope="col">Configuration Sync</th>
                                <th scope="col">Last configuration update</th>
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


        <h5 class="my-3">Recent Boards</h5>
        <?php
        $recentBoards = Board::getRecentBoards();
        $uboards = Board::printBoardsFromTrackingLog($recentBoards);
        ?>

        <p>There is a total of <?= count($uboards); ?> boards active in the last 48 hours.</p>
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