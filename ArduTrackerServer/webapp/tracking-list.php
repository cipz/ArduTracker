<?php
 require_once 'res/config.php';
 $_CURRENT_PAGE_NAME = 'Tracking list';
 require_once 'template/header.php';
?>

<div class="row">
    <div class="col-lg-3">
        <div class="card">
            <div class="card-body">
                <h5 class="card-title text-muted mb-4">Something aside</h5>
                Maybe <a href="#">urls</a>?
            </div>
        </div>
    </div>
    <div class="col-lg-9">
    
    <table class="table table-striped">
        <thead>
            <tr>
            <th scope="col">ID-1</th>
            <th scope="col">ID-2</th>
            <th scope="col">Last seen (millis)</th>
            <th scope="col">Last date</th>
            <th scope="col">Created at</th>
            </tr>
        </thead>
        <tbody>
            <tr>
            <th scope="row">1</th>
            <td>Mark</td>
            <td>Otto</td>
            <td>@mdo</td>
            <td>@mdo</td>
            </tr>
            <tr>
            <th scope="row">2</th>
            <td>Jacob</td>
            <td>Thornton</td>
            <td>@fat</td>
            <td>@mdo</td>
            </tr>
            <tr>
            <th scope="row">3</th>
            <td colspan="2">Larry the Bird</td>
            <td>@twitter</td>
            <td>@mdo</td>
            </tr>
        </tbody>
    </table>

    </div>
</div>

<?php
 require_once 'template/footer.php';
?>