<?php
 require_once 'res/resources.php';
 $_CURRENT_PAGE_NAME = 'Tracking list';
 require_once 'template/header.php';
 require_once 'controller/tracking-list.php';
?>

<div class="row">
    <div class="col-lg-3">
        <div class="card">
            <div class="card-body">
                <h5 class="card-title text-muted mb-4"><i class="fas fa-chart-bar"></i> Stats</h5>
                <strong>Total tracking logs:</strong> <?=Board::getTotalRecords();?> <br>
                <strong>Total boards discovered:</strong> <?=count($data);?>
            </div>
        </div>
        <div class="card mt-3">
            <div class="card-body">
                <h5 class="card-title text-muted mb-4"><i class="fas fa-info-circle"></i> Information</h5>
                Click on the board ID to show the complete tracking list.
            </div>
        </div>
    </div>
    <div class="col-lg-9">

    <div class="input-group mb-3">
        <span class="input-group-text" id="search"><i class='fas fa-search'></i></span>
        <input type="text" class="form-control" placeholder="Search by ID" aria-label="Search by ID" aria-describedby="search" onkeyup="searchIds()" id="input-search-id">
    </div>

        <table class="table table-striped">
            <thead>
                <tr>
                    <th scope="col">IDs</th>
                </tr>
            </thead>
            <tbody id="ids-list">
                <?php 
                   echo $out;
                ?>
            </tbody> 
        </table>
    </div>
</div>

<script>
function searchIds() {
  var input, filter, tbody, td, a, i, txtValue;
  input = document.getElementById('input-search-id');
  filter = input.value.toUpperCase();
  tbody = document.getElementById("ids-list");
  td = tbody.getElementsByTagName('td');
  for (i = 0; i < td.length; i++) {
    a = td[i].getElementsByTagName("a")[0];
    txtValue = a.textContent || a.innerText;
    if (txtValue.toUpperCase().indexOf(filter) > -1) {
      td[i].style.display = "";
    } else {
      td[i].style.display = "none";
    }
  }
}
</script>

<?php
 require_once 'template/footer.php';
?>