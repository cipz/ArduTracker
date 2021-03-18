<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
  <meta name="description" content="ArduTracking WebApp - PoC for Wireless Networks for Mobile Applications">
  <link rel="icon" href="assets/images/logo.png">
  <title><?php if(isset($_TITLE_CURRENT_PAGE_NAME)) echo "$_TITLE_CURRENT_PAGE_NAME | ";
               elseif(isset($_CURRENT_PAGE_NAME))  echo "$_CURRENT_PAGE_NAME | "; ?>ArduTracker - Webapp</title>
  <!-- Bootstrap core CSS -->
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-BmbxuPwQa2lc/FVzBcNJ7UAyJxM6wuqIj61tLrc4wSX0szH/Ev+nYRRuWlolflfl" crossorigin="anonymous">
  <link rel="stylesheet" href="assets/style.css">
  <!-- Fontawesome icons -->
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.12.0/css/all.min.css">
</head>

<body>

  <nav class="navbar navbar-expand-lg bg-dark navbar-dark fixed-top">
    <div class="container">
      <a class="navbar-brand" href="./">
        <img src="assets/images/logo.png" width="24" height="24" class="at-logo" alt="">
        ArduTracker &nbsp;
      </a>
      <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarResponsive" aria-controls="navbarResponsive" aria-expanded="false" aria-label="Toggle navigation">
        <span class="navbar-toggler-icon"></span>
      </button>
      <div class="collapse navbar-collapse" id="navbarResponsive">
        <ul class="navbar-nav ml-auto">
          <li class="nav-item">
              <a class="nav-link" href="index.php"><i class='fas fa-home'></i> Homepage</a>
          </li>
          <li class="nav-item">
              <a class="nav-link" href="boards.php"><i class='fas fa-microchip'></i> Boards</a>
          </li>
          <li class="nav-item">
              <a class="nav-link" href="tracking-list.php"><i class='fas fa-list'></i> Tracking List</a>
          </li>
        </ul>
      </div>
    </div>
  </nav>

<div class="container py-3 at-container">
<?php if(isset($_CURRENT_PAGE_NAME)) echo "<h1 class='fw-light mt-1 mb-3 h2 at-title'>$_CURRENT_PAGE_NAME ".(isset($_EXTRA_CURRENT_PAGE_NAME) ? $_EXTRA_CURRENT_PAGE_NAME : '')."</h1>"; ?>
