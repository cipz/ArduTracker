<?php
require_once 'controller/board-detail.php';
$_TITLE_CURRENT_PAGE_NAME = 'Board detail - ' . $id;
$_CURRENT_PAGE_NAME = '<a href="boards.php" class="text-secondary">Boards</a>';
$_EXTRA_CURRENT_PAGE_NAME = '&raquo; <span class="small text-secondary">' . $id . '</span>';
require_once 'template/header.php';
?>

<script src="res/javascript/board-detail.js"></script>

<?= (isset($out) ? '<div class="row"><div class="col-lg-12">' . $out . '</div></div>' : ''); ?>

<div class="row">
    <div class="col-lg-3">
        <div class="card">
            <div class="card-header">
                <i class="fas fa-microchip"></i> Board information
            </div>
            <div class="card-body">
                <div class="mb-3">
                    <p class="small mb-0 text-secondary">Board ID</p>
                    <?= $board->id; ?>
                </div>
                <div class="mb-3">
                    <p class="small mb-0 text-secondary">Board MAC Address</p>
                    <code><?= $board->idMac; ?></code>
                </div>
                <div class="mb-3">
                    <p class="small mb-0 text-secondary">Configuration Synchronized</p>
                    <?= $label; ?>
                </div>
                <div class="mb-3">
                    <p class="small mb-0 text-secondary">Updated at</p>
                    <?= time2string($board->updatedAt); ?>
                </div>
                <div class="mb-0">
                    <p class="small mb-0 text-secondary">Created at</p>
                    <?= $board->createdAt; ?>
                </div>
            </div>
        </div>

        <div class="card my-3">
            <div class="card-body d-grid gap-3 ">
                <form action="?id=<?= $board->id; ?>" method="post" class="d-grid">
                    <button type="submit" name="Send" class="btn btn-success btn-sm"><i class="fas fa-sync-alt"></i> Sync config now</button>
                </form>
                <a href="tracking-detail.php?id=<?= $board->id; ?>" class="btn btn-primary btn-sm"><i class="fas fa-list-ul"></i> Tracking logs</a>
                <form action="?id=<?= $board->id; ?>" method="post"  class="d-grid">
                    <button type="submit" name="Remove" onclick="return confirm('Are you really sure to delete this board?')" class="btn btn-danger btn-sm"><i class="fas fa-trash"></i> Remove this board</button>
                </form>
            </div>
        </div>
    </div>
    <div class="col-lg-9">
        <div class="card">
            <div class="card-header">
                <i class="fas fa-edit"></i> Edit configuration
            </div>
            <div class="card-body">

                <div class='alert alert-secondary p-1 px-2 my-3'><i class='fas fa-info-circle'></i> The maximum configuration size for the JSON is <?= DEFAULT_MAX_PACKET_SIZE; ?> bytes</div>

                <?php
                    if (!$board->newConfigSent) {
                        echo "<div class='alert alert-warning'><i class='fas fa-exclamation-triangle'></i> The new configuration has <b>not been synchronized</b> with the board yet.</div>";
                    }
                ?>

                <nav>
                    <div class="nav nav-tabs" id="nav-tab" role="tablist">
                        <button class="nav-link active" id="nav-main-tab" data-bs-toggle="tab" data-bs-target="#nav-main" type="button" role="tab" aria-controls="nav-main" aria-selected="true"><i class="fas fa-list-ul"></i> JSON Readable config</button>
                        <button class="nav-link" id="nav-raw-tab" data-bs-toggle="tab" data-bs-target="#nav-raw" type="button" role="tab" aria-controls="nav-raw" aria-selected="false" onclick="updateJsonStatus(isValidJson(document.getElementById('rawConfigInput').value));"><i class="fas fa-code"></i> Raw JSON config</button>
                        <button class="nav-link" id="nav-docs-tab" data-bs-toggle="tab" data-bs-target="#nav-docs" type="button" role="tab" aria-controls="nav-docs" aria-selected="false"><i class="fas fa-info-circle"></i> Documentation</button>
                    </div>
                </nav>
                <div class="tab-content mt-4">
                    <div class="tab-pane fade show active" id="nav-main" role="tabpanel" aria-labelledby="nav-main-tab">
                        <form action="?id=<?= $board->id; ?>" method="post" class="form">
                            <?php

                            $mv = json_decode($board->config);

                            if (empty($mv))
                                echo "The configuration file is empty!";
                            else {
                                $i = 1;
                                foreach ($mv as $k => $v) {
                                    echo '<div class="row mb-3" id="default_' . $i . '">';
                                    echo '  <label class="col-sm-3 col-form-label"><span class="fa fa-pen-square"></span> ' . $k;
                                    if ($k != BOARD_ID_CONFIG_FIELD)
                                        echo '    <small>(<a href="javascript:void(0)" onclick="deleteDefault(' . $i . ')" class="text-danger"><i class="fas fa-times"></i></a>)</small>';
                                    echo '  </label>';
                                    echo '   <div class="col-sm-9"> <input type="hidden" name="tkey[]" value="' . $k . '" />';
                                    if (isset($_BOARD_CONFIG_EXTRA[$k]['select']))
                                        echo '  <select required class="form-select" name="tval[]"><option value="'.$v.'">' . $v . ' (CURRENT)</option>'.arrayToOptionForm($_BOARD_CONFIG_EXTRA[$k]['select']).'</select>';
                                    else if(isset($_BOARD_CONFIG_EXTRA[$k]['range']))
                                        echo '  <input type="number" min="'.$_BOARD_CONFIG_EXTRA[$k]['min_range'].'" max="'.$_BOARD_CONFIG_EXTRA[$k]['max_range'].'" value="'.$v.'" required class="form-control" name="tval[]">';
                                    else
                                        echo '  <textarea required ' . ($k == BOARD_ID_CONFIG_FIELD ? 'readonly' : '') . ' class="form-control" name="tval[]" rows="' . (strlen($v) > 32 ? 6 : 1) . '" placeholder="'.(isset($_DEFAULT_BOARD_CONFIG[$k]) ? "es. ".$_DEFAULT_BOARD_CONFIG[$k] : "").'">' . $v . '</textarea>';
                                    echo ' <p class="mb-0 small text-muted">'.($_BOARD_CONFIG_EXTRA[$k]['info']).'</p>';
                                    if(isset(($_BOARD_CONFIG_EXTRA[$k]['unit'])))
                                        echo ' <p class="mb-0 small text-warning"><strong>Unit of measurement:</strong> '.($_BOARD_CONFIG_EXTRA[$k]['unit']).'</p>';
                                    echo '</div></div>';
                                    $i++;
                                }
                            }
                            ?>
                            <div id="addRow" class="my-4"></div>

                            <a class="btn btn-sm btn-dark" href="javascript:void(0)" onclick="addRow();"><span class="fas fa-plus-square"></span> Add a new variable</a>

                            <div class="card mt-3">
                                <div class="card-body">
                                    <button type="submit" name="SaveAndSend" class="btn btn-success mr-2"><i class="fas fa-file-export"></i> Save and sync</button> &nbsp;
                                    <button type="submit" name="Save" class="btn btn-secondary mr-2"><i class="fas fa-check"></i> Save without synching</button>
                                </div>
                            </div>

                        </form>
                    </div>
                    <div class="tab-pane fade" id="nav-raw" role="tabpanel" aria-labelledby="nav-raw-tab">
                        <form action="?id=<?= $board->id; ?>" method="post" class="form">
                            <?php
                            if (empty($board->config))
                                echo "The configuration file is empty! Delete and recreate this board to reset to the default configuration. ";
                            else {
                            ?>
                                <textarea required class="form-control" id="rawConfigInput" name="rawconfig" onkeyup="updateJsonStatus(isValidJson(this.value));" rows="15"><?= $board->config; ?></textarea>
                                <div class="p small my-2">The JSON is <span id="jsonValid"></span></div>
                            <?php } ?>

                            <div class="card mt-3">
                                <div class="card-body">
                                    <button type="submit" id="saveRaw" name="SaveAndSend" class="btn btn-success mr-2"><i class="fas fa-file-export"></i> Save and sync</button> &nbsp;
                                    <button type="submit" id="saveAndSendRaw" name="Save" class="btn btn-secondary mr-2"><i class="fas fa-check"></i> Save without synching</button>
                                </div>
                            </div>
                        </form>
                    </div>
                    <div class="tab-pane fade" id="nav-docs" role="tabpanel" aria-labelledby="nav-docs-tab">

                    <h4 class="mb-4 h5">Variables documentation</h4>
                    <?php

                        $_bce = $_BOARD_CONFIG_EXTRA;

                        if (empty($_BOARD_CONFIG_EXTRA))
                            echo "No documentation found.";
                        else {
                            
                            foreach ($_BOARD_CONFIG_EXTRA as $_kbce => $_bce) {
                                echo '<div class="row mb-3">';
                                echo '  <div class="col-sm-3">';
                                echo '      <p class="mb-0 small"><code>'.($_kbce).'</code></p>';
                                echo '  </div>';
                                echo '  <div class="col-sm-9">';
                                echo '      <p class="mb-0 small text-muted">'.($_bce['info']).'</p>';
                                if(isset(($_bce['unit'])))
                                    echo ' <p class="mb-0 small text-warning"><strong>Unit of measurement:</strong> '.($_bce['unit']).'</p>';
                                echo '</div></div>';
                            }
                        }
                    ?>
                    </div>
                </div>
            </div>
        </div>

    </div>
</div>

<?php
require_once 'template/footer.php';
?>