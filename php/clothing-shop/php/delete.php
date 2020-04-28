<?php
  session_start();  
  if (!isset($_SESSION[cart])) {
     $_SESSION[cart]=array();
  } 
  
  if (!isset($_SESSION[prices])) {
     $_SESSION[prices]=array();
  } 

  $cartarry = $_SESSION[cart];
  $pricearry = $_SESSION[prices];

  $item = isset($_GET['item']) ? $_GET['item']: NULL;

  $total = count($cartarry);
			 
  for($i=0; $i<$total; $i=$i+1) {
			   
    if($cartarry[$i] == NULL && $pricearry[$i] == NULL)
	  $total = $total + 1; 
	else
	{
      if($cartarry[$i] == $item)  {
	    unset($cartarry[$i]);
	    unset($pricearry[$i]);
		break;
	  }
	}
  }
  $_SESSION[cart] = $cartarry;
  $_SESSION[prices] = $pricearry;
  
   $host = $_SERVER['HTTP_HOST'];	
   $url = "Location: ../viewcart.php";
   header($url);
   exit;
?>