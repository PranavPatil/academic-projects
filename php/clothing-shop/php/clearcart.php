<?php
  session_start();  
  unset($_SESSION[cart]);
  unset($_SESSION[prices]);
  
  $url = "Location: ../viewcart.php";
  header($url);
  exit;  
?>