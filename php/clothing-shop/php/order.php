<?php
    $host = "brain.it.edu";
	$username = "aasali";
	$password = "Abdul1!";
	$dbname = "udb_aasali";

    $fname = isset($_POST['fname']) ? $_POST['fname']: NULL;  // if   isset($_POST['fname'])   then=? $_POST['fname'] else NULL
    $lname = isset($_POST['lname']) ? $_POST['lname']: NULL;
    $address1 = isset($_POST['address1']) ? $_POST['address1']: NULL;
    $address2 = isset($_POST['address2']) ? $_POST['address2']: NULL;
    $city = isset($_POST['city']) ? $_POST['city']: NULL;
    $state = isset($_POST['state']) ? $_POST['state']: NULL;
    $zipcode = isset($_POST['zipcode']) ? $_POST['zipcode']: NULL;
    $country = isset($_POST['country']) ? $_POST['country']: NULL;
    $phone1 = isset($_POST['phone1']) ? $_POST['phone1']: NULL;
    $phone2 = isset($_POST['phone2']) ? $_POST['phone2']: NULL;
    $phone3 = isset($_POST['phone3']) ? $_POST['phone3']: NULL;
    $email = isset($_POST['email']) ? $_POST['email']: NULL;

	$valid = 1;

    $dblink = mysql_connect($host, $username, $password);

    if (!$dblink)
       $msg = 'SQLError: ' . mysql_error();
	   $valid = 0;
    }

    if($valid == 1 && !mysql_select_db($dbname, $dblink))
	{
      $msg = 'SQLError: ' . mysql_error();
      $valid = 0;
    }

    $sql = "insert into orders(fname, lname, email, phone1, phone2, phone3, address1, address2, city, state, country, zipcode ) values (\"$fname\", \"$lname\", \"$email\", \"$phone1\", \"$phone2\", \"$phone3\", \"$address1\", \"$address2\", \"$city\", \"$state\", \"$country\", \"$zipcode\")";

    if($valid == 1 && !mysql_query($sql, $dblink))
	{
      $msg = 'SQLError Insert: ' . mysql_error();
      $valid = 0;
    }

    if($valid == 1)
	{
	   $msg = "Order Submitted Successfully.";
	}

    $url = "Location: ../message.php?msg=$msg";  // when u spesfie the name or the value in the url then its a 'get' request
    header($url);
    exit;
?>
