<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
  <meta name="description" content="ArduTracking WebApp - PoC for Wireless Networks for Mobile Applications">
  <link rel="icon" href="assets/images/logo.png">
  <title><?php if(isset($_CURRENT_PAGE_NAME)) echo "$_CURRENT_PAGE_NAME | "; ?>ArduTracker - Webapp</title>
  <!-- Bootstrap core CSS -->
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-giJF6kkoqNQ00vy+HMDP7azOuL0xtbfIcaT9wjKHr8RbDVddVHyTfAAsrekwKmP1" crossorigin="anonymous">
  <link rel="stylesheet" href="assets/style.css">
  <!-- Fontawesome icons -->
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.12.0/css/all.min.css">
</head>

<body>

  <nav class="navbar navbar-expand-lg bg-dark navbar-dark fixed-top">
    <div class="container">
      <a class="navbar-brand" href="./">
        <img src="assets/images/logo.png" width="28" height="28" class="at-logo" alt="">
        ArduTracker &nbsp;
      </a>
      <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarResponsive" aria-controls="navbarResponsive" aria-expanded="false" aria-label="Toggle navigation">
        <span class="navbar-toggler-icon"></span>
      </button>
      <div class="collapse navbar-collapse" id="navbarResponsive">
        <ul class="navbar-nav ml-auto">
            <li class="nav-item">
                <a class="nav-link" href="tracking-list.php">Tracking List</a>
            </li>
        </ul>
      </div>
    </div>
  </nav>

<div class="container py-3 at-container">
<?php if(isset($_CURRENT_PAGE_NAME)) echo "<h1 class='my-3'>$_CURRENT_PAGE_NAME</h1>"; ?>
