<?php  
  $msg = isset($_GET['msg']) ? $_GET['msg']: NULL;
?>

<html>
<head>
<title> Message </title>
<link rel= "stylesheet" type= "text/css" href= "css/clothing.css">
</head>
<body class="abc2" bgcolor="#FFFF8A">
  <table width= "100%" border= "1">
    <tr> <td height="95" colspan= "2" bgcolor="#E6E6E6">
 <div align="left" style="background-image:url(images/clothing_header.jpg); height:174px; width:1041px;"></div>
         </td> 
    </tr>
       <tr style="border-left-color:#FFF; border-bottom-width:thick"> <td width= "19%" valign= "top" height="100">
<div style="display:block;"><h3 style="background-color:#8383E7; color:#FFF; font-size:23px;">Menu</h3>
  <p style="padding-left:5px;">
  <a href= "index.html" class= "abc"> Home </a><br/>
  <a href= "menmenu.html" class= "abc">Men's Clothing. </a><br/>
  <a href= "womenmenu.html" class= "abc"> Women's Clothing. </a><br/>
  <a href= "aboutus.html" class= "abc"> About Us.</a></p>
  </div>
  </td>
        <td width="78%" class= "abc3">
        <br />
        <div id="container" style="width:900px;">

        <div class="info" style="color:#FFF;">
          <h2 style="padding-left:10px;"><?= $msg; ?></h2>
        </div>
		
        </div>
        <p>&nbsp;</p>
<br />
         <p><br/>
        </P></td>
        </tr>
  </table>
</body>
</html>
