<?php
  session_start();  
  
  if (!isset($_SESSION[cart])) {
     $_SESSION[cart]=array();
  } 
  
  if (!isset($_SESSION[prices])) {
     $_SESSION[prices]=array();
  } 

  $item = isset($_POST['item']) ? $_POST['item']: NULL;
  $price = isset($_POST['price']) ? $_POST['price']: NULL;
  $page = isset($_POST['page']) ? $_POST['page']: NULL;
   
  array_push($_SESSION[cart],$item);
  array_push($_SESSION[prices],$price);
  
  $url = "Location: ../$page";
  header($url);
  exit;  
?>