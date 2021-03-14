<?php
require_once 'controller/board-detail.php';
$_CURRENT_PAGE_NAME = $id . ' - Board detail';
require_once 'template/header.php';
?>

<div class="row">
    <div class="col-lg-12">
        <div class="card">
            <div class="card-header">
                <i class="fas fa-plus-circle"></i> Edit configuration
            </div>
            <div class="card-body">
            <form action="?" method="POST" class="form-box form-full">

                    <?php

                    $mv = json_decode($board->config);

                    if (empty($mv))
                        echo "The configuration file is empty!";
                    else {
                        $i = 1;
                        foreach ($mv as $k => $v) {
                            echo '<div class="row mb-3" id="default_' . $i . '">';
                            echo '  <label class="col-sm-3 col-form-label"><span class="fa fa-pen-square"></span> ' . $k;
                            echo '    <br /> <small>(<a href="javascript:void(0)" onclick="deleteDefault(' . $i . ')">delete</a>)</small>';
                            echo '  </label>';
                            echo '   <div class="col-sm-9"> <input type="hidden" name="tkey[]" value="' . $k . '" />';
                            echo '  <textarea required class="form-control" name="tval[]" rows="' . (strlen($v) > 32 ? 6 : 1) . '">' . $v . '</textarea>';
                            echo '</div></div>';
                            $i++;
                        }
                    }
                    ?>
                    <div id="addRow"></div>
            </div>
        </div>

        <div class="card my-4">
            <div class="card-header">
                <i class="fas fa-plus-circle"></i> Add a new variable to configuration
            </div>
            <div class="card-body">
                

                <div class="row mb-3">
                    <button class="btn btn-success" type='button' onclick="addRow();"><span class="fas fa-plus-square"></span> Add a new variable</button>
                </div>
            </div>
        </div>

        <button type="submit" class="btn btn-primary"><span class="fas fa-check"></span> Save and send</button>
    </div>
</div>

<script>
    var globalRowNumber = 1;

    function addRow() {
        var x = document.getElementById('addRow');
        var container = document.createElement("div");
        container.setAttribute('id', 'row_' + globalRowNumber);
        container.innerHTML += "<div class='row'>" +
            "<div class='col-sm-2'> <button type='button' onclick='deleteRow(" + globalRowNumber + ");' class='btn btn-danger btn-small'><i class='fa fa-times'></i> Delete</button></div>" +
            "<div class='col-sm-4'>" +
            "" +
            "<input type='text' class='form-control' maxlength='32' name='tkey[]' placeholder='Key' />" +
            "</div><div class='col-sm-6'>" +
            "" +
            "<textarea class='form-control' name='tval[]' placeholder='Value' rows=1></textarea>" +
            "</div></div><hr>";
        x.appendChild(container);
        globalRowNumber++;
    }

    function deleteRow(i) {
        document.getElementById("row_" + i).outerHTML = "";
    }

    function deleteDefault(i) {
        var x = confirm('Vuoi veramente cancellare questa chiave?');
        if (x)
            document.getElementById("default_" + i).outerHTML = "";
    }
</script>


<?php
require_once 'template/footer.php';
?>