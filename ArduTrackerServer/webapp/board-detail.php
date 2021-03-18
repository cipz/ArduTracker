<?php
 require_once 'controller/board-detail.php';
 $_CURRENT_PAGE_NAME = 'Board detail';
 require_once 'template/header.php';
?>

<div class="row">
    <div class="col-lg-12">
        <div class="row">

        <form action="?=" method="POST" class="form-box form-full">

        <?php

            $mv = json_decode($board->config);
            
            if(empty($mv))
                echo "The configuration file is empty!";
            else
            {
                $i = 1;
                foreach($mv as $k => $v)
                {
                    echo '<div class="form-row" id="default_'.$i.'">';
                    echo '  <label class="textarea"><span class="fa fa-pen-square"></span> '.$k;
                    echo '    <br /> <small>(<a href="javascript:void(0)" onclick="deleteDefault('.$i.')">delete</a>)</small>';
                    echo '  </label>';
                    echo '  <input type="hidden" name="tkey[]" value="'.$k.'" />';
                    echo '  <textarea class="form-control textarea-translate" name="tval[]" rows="'.(strlen($v) > 32 ? 6 : 1).'">'.$v.'</textarea>';
                    echo '</div>';
                    $i++;
                }
            }

        ?>
            
                <div class='form-row h4'>New config variable</div>
                <div id="addRow"></div>
            
                <div class="form-row">
                    <button class="btn btn-success" type='button' onclick="addRow();"><span class="fas fa-plus-square"></span> Add a new variable</button>
                </div>

            <div class="form-row mt-3">
                <button type="submit" class="btn btn-primary"><span class="fas fa-check"></span> Save and send</button>
            </div>
        </div>
    </div>
</div>

<script>

var globalRowNumber = 1;

function addRow()
{
    var x = document.getElementById('addRow');
    var container = document.createElement("div");
    container.setAttribute('id', 'row_'+globalRowNumber);
    container.innerHTML += "<div class='form-row'>"
                + "<button type='button' onclick='deleteRow("+globalRowNumber+");' class='btn btn-reverse btn-small'><i class='fa fa-times'></i> Delete</button></div>" 
                + "<div class='form-row'>"
                + "<label> <i class='fa fa-key'></i> Key</label>"
                + "<input type='text' class='form-input' maxlength='32' name='tkey[]' placeholder='Key' />"
                + "</div><div class='form-row'>"
                + "<label class='textarea'><span class='fas fa-pen-square'></span> Value</label>"
                + "<textarea class='form-input textarea-translate' name='tval[]' placeholder='Value'></textarea>"
                + "</div><hr />";
    x.appendChild(container);
    globalRowNumber++;
}

function deleteRow(i)
{
    document.getElementById("row_"+i).outerHTML="";
}

function deleteDefault(i)
{
    var x = confirm('Vuoi veramente cancellare questa chiave?');
    if(x)
    document.getElementById("default_"+ i).outerHTML="";
}

</script>


<?php
 require_once 'template/footer.php';
?>