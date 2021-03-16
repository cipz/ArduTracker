<?php
require_once 'controller/board-detail.php';
$_TITLE_CURRENT_PAGE_NAME = 'Board detail - '.$id;
$_CURRENT_PAGE_NAME = '<a href="boards.php" class="text-secondary">Boards</a>';
$_EXTRA_CURRENT_PAGE_NAME = '&raquo; <span class="small text-secondary">'.$id.'</span>';
require_once 'template/header.php';
?>

<div class="row">
    <div class="col-lg-3">
        <div class="card">
            <div class="card-header">
                <i class="fas fa-microchip"></i> Board information
            </div>
            <div class="card-body">
                <div class="mb-3">
                    <p class="small mb-0 text-secondary">Board ID</p>
                    <?=$board->id;?>
                </div>
                <div class="mb-3">
                    <p class="small mb-0 text-secondary">Board MAC Address</p>
                    <code><?=$board->idMac;?></code>
                </div>
                <div class="mb-3">
                    <p class="small mb-0 text-secondary">Configuration Synchronized</p>
                    <?=$label;?>
                </div>
                <div class="mb-3">
                    <p class="small mb-0 text-secondary">Updated at</p>
                    <?=time2string($board->updatedAt);?>
                </div>
                <div class="mb-3">
                    <p class="small mb-0 text-secondary">Created at</p>
                    <?=$board->createdAt;?>
                </div>
            </div>
        </div>
    </div>
    <div class="col-lg-9">
        <div class="card">
            <div class="card-header">
                <i class="fas fa-edit"></i> Edit configuration
            </div>
            <div class="card-body">

                <div class='alert alert-secondary my-3'><i class='fas fa-info-circle'></i> The max configuration size for the JSON is <?= DEFAULT_MAX_PACKET_SIZE; ?> bytes</div>

                <form action="?" method="POST" class="form-box form-full">
                    <?php

                    if (!$board->new_config_sent) {
                        echo "<div class='alert alert-warning'><i class='fas fa-exclamation-triangle'></i> The new configuration has <b>not been synchronized</b> with the board yet.</div>";
                    }

                    $mv = json_decode($board->config);

                    if (empty($mv))
                        echo "The configuration file is empty!";
                    else {
                        $i = 1;
                        foreach ($mv as $k => $v) {
                            echo '<div class="row mb-3" id="default_' . $i . '">';
                            echo '  <label class="col-sm-3 col-form-label"><span class="fa fa-pen-square"></span> ' . $k;
                            echo '    <small>(<a href="javascript:void(0)" onclick="deleteDefault(' . $i . ')" class="text-danger"><i class="fas fa-times"></i> delete</a>)</small>';
                            echo '  </label>';
                            echo '   <div class="col-sm-9"> <input type="hidden" name="tkey[]" value="' . $k . '" />';
                            echo '  <textarea required class="form-control" name="tval[]" rows="' . (strlen($v) > 32 ? 6 : 1) . '">' . $v . '</textarea>';
                            echo '</div></div>';
                            $i++;
                        }
                    }
                    ?>
                    <div id="addRow" class="my-4"></div>

                    <button class="btn btn-sm btn-success" type='button' onclick="addRow();"><span class="fas fa-plus-square"></span> Add a new variable</button>
            </div>
        </div>
        <div class="card my-3">
            <div class="card-body">
                <button type="submit" name="SaveAndSend" class="btn btn-primary mr-2"><span class="fas fa-file-export"></span> Save and send</button> &nbsp;
                <button type="submit" name="Save" class="btn btn-secondary mr-2"><span class="fas fa-check"></span> Save without sending</button>
            </div>
        </div>
    </div>
</div>

<script>
    var globalRowNumber = 1;

    function addRow() {
        var x = document.getElementById('addRow');
        var container = document.createElement("div");
        container.setAttribute('id', 'row_' + globalRowNumber);
        container.innerHTML += "<hr><div class='row'>" +
            "<div class='col-sm-2'> <button type='button' onclick='deleteRow(" + globalRowNumber + ");' class='btn btn-danger btn-small'><i class='fa fa-times'></i> Delete</button></div>" +
            "<div class='col-sm-4'>" +
            "" +
            "<input type='text' class='form-control' maxlength='32' name='tkey[]' placeholder='Key' />" +
            "</div><div class='col-sm-6'>" +
            "" +
            "<textarea class='form-control' name='tval[]' placeholder='Value' rows=1></textarea>" +
            "</div></div>";
        x.appendChild(container);
        globalRowNumber++;
    }

    function deleteRow(i) {
        document.getElementById("row_" + i).outerHTML = "";
    }

    function deleteDefault(i) {
        var x = confirm('Are you really sure to delete this key-value?');
        if (x)
            document.getElementById("default_" + i).outerHTML = "";
    }
</script>


<?php
require_once 'template/footer.php';
?>