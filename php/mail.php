<?php
require("class.phpmailer.php");
require("class.smtp.php");

$mail = new PHPMailer();

$mail->IsSMTP();                                      // set mailer to use SMTP
$mail->Host = "smtp.gmail.com";  // specify main and backup server
$mail->SMTPAuth = true;     // turn on SMTP authentication
$mail->SMTPSecure = "ssl";
$mail->Port = "465";
$mail->Username = "pranav";  // SMTP username
$mail->Password = "secretpassword"; // SMTP password

$mail->From = "pranav@gmail.com";
$mail->FromName = "Pranav";
$mail->AddAddress("john@gmail.com", "John Nance");

$mail->WordWrap = 50;                                 // set word wrap to 50 characters
$mail->IsHTML(true);                                  // set email format to HTML

$mail->Subject = "Hello John";
$mail->Body    = "Lets hope this works, long day today!</b>";
$mail->AltBody = "This is the body in plain text for non-HTML mail clients";

if(!$mail->Send())
{
   echo "Message could not be sent. <p>";
   echo "Mailer Error: " . $mail->ErrorInfo;
   exit;
;}

echo "Message has been sent";
?>
