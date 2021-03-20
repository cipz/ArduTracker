<?php
require_once 'res/resources.php';
$_CURRENT_PAGE_NAME = 'Login';
require_once 'template/header.php';
?>

<div class="row">
    <div class="col-lg-4">
        <div class="card">
            <div class="card-body">

                <h5 class="card-title text-muted mb-4">Welcome to the ArduTrack Project</h5>

                <div class="m-auto text-center">
                    <img src="assets/images/logo.png" alt="ArduTracker" width=120 height=120 class="my-3">
                </div>

                <p class="card-text">Please insert the password to continue.</p>

            </div>
        </div>
    </div>
    <div class="col-lg-8">
        <div class="card">
            <div class="card-body">

                <h5 class="card-title text-muted mb-4">Password protection</h5>
                <form action="?" method="post">
                    <div class="row g-2 my-2">
                        <div class="col-md">
                            <div class="form-floating">
                                <input type="password" required class="form-control" id="passwordInput" name="id" placeholder="Insert password to continue">
                                <label for="passwordInput">Password</label>
                            </div>
                        </div>
                    </div>
                    <button type="submit" name="Login" class="btn btn-success btn-sm"><i class="fas fa-check"></i> Login</button>
                </form>
            </div>
        </div>
    </div>
</div>

<?php
require_once 'template/footer.php';
?>