<?php
 require_once 'res/resources.php';
 $_CURRENT_PAGE_NAME = 'Homepage';
 require_once 'template/header.php';
?>

<div class="row">
    <div class="col-lg-12">
        <div class="card">
            <div class="card-body">
                <img src="assets/images/logo.png" alt="ArduTracker" width=120 height=120 class="my-3">
                
                <h5 class="card-title text-muted mb-4">Welcome to the ArduTrack Project</h5>
                <p class="card-text">This web application is a proof of concept used to visualize data tracking from the ArduTracker components.</p>
                <p class="card-text">Please, follow the links above to navigate inside this site.</p>
                <a href="controller/publish.php" class="btn btn-primary">Generate test contact</a>
            </div>
        </div>
    </div>
</div>

<?php
 require_once 'template/footer.php';
?>