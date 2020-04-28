<?php
  session_start();  
  
  if (!isset($_SESSION[cart])) {
     $_SESSION[cart]=array();
  } 
  
  if (!isset($_SESSION[prices])) {
     $_SESSION[prices]=array();
  } 
?>

<html>
<head>
<title> Men's CLothing </title>
<link rel= "stylesheet" type= "text/css" href= "css/clothing.css">
<!-- CSS -->
<link rel="stylesheet" href="css/structure.css" type="text/css" />
<link rel="stylesheet" href="css/form.css" type="text/css" />

<!-- JavaScript -->
<script type="text/javascript" src="scripts/wufoo.js"></script>
<script language="javascript" type="text/javascript">

function updateMenu()
{
   type = document.getElementById('clothestype').value;
   document.getElementById('formals').style.display="none";
   document.getElementById('casuals').style.display="none";
   document.getElementById(type).style.display="block";
}

function myPopup(image) {
  window.open( "imgviewer.php?imgfile=" +  image, "mywindow","menubar=1,resizable=1,width=650,height=550")
}


</script>
</head>
<body class="abc2" bgcolor="#FFFF8A">
  <table width= "100%" border= "1">
    <tr> <td height="95" colspan= "2" bgcolor="#E6E6E6">
    <div align="left" style="background-image:url(images/clothing_header.jpg); height:174px; width:1041px;"></div>
         </td> 
    </tr>
    <tr style="border-left-color:#FFF; border-bottom-width:thick"> <td width= "19%" valign= "top">
  <div style="display:block;"><h3 style="background-color:#8383E7; color:#FFF; font-size:23px;">Menu</h3>
  <p style="padding-left:5px;">
  <a href= "index.html" class= "abc"> Home </a><br/>
  <a href= "menmenu.html" class= "abc">Men's Clothing. </a><br/>
  <a href= "womenmenu.html" class= "abc"> Women's Clothing. </a><br/>
  <a href= "aboutus.html" class= "abc"> About Us.</a></p>
  </div>
  </td>
        <td width="81%" class= "abc3">        
         <table width="24">
          <tr>
            <td width="14">&nbsp;
            
<div id="container" style="width:900px;" class="DATA">

		<!-- START CART HEADER -->
        <form id="cartform" name="cartform" class="wufoo topLabel page" autocomplete="off" method="post" action="/php/register.php">
		<input type="hidden" name="page" value="index.htm">
        <div class="info">
          <h2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;View Cart</h2>
        </div>

		<div align="center">
		<center>
		<center>
		</center><table border="0" cellpadding="5" cellspacing="0" width="90%">
		<tbody><tr>
	
			<td valign="top" colspan="5">
		
		<hr>
		</td>
		
		</tr>
		<tr>
	
			<td valign="top" class="header_cell">&nbsp;</td>
		
		<td valign="top" class="header_cell">QTY</td>
		<td width="100%" valign="top" class="header_cell">Product Name</td>
		<td valign="top" class="header_cell" align="right">Price</td>
		<td valign="top" nowrap="" class="header_cell">Ext. Amount</td>
		</tr>
		<!-- END CART HEADER -->
        
        <?php
		
           $cartarry = $_SESSION[cart];
		   $pricearry = $_SESSION[prices];
		   $total = count($cartarry);
		   $totalprice = 0;
			 
           for($i=0; $i<$total; $i=$i+1) {
			   
			   if($cartarry[$i] == NULL && $pricearry[$i] == NULL)
			     $total = $total + 1; 
			   else
			   {
				   $totalprice = $totalprice + $pricearry[$i] + ($pricearry[$i] * 0.06);
		?>
	
			<!-- START CART 101 -->
			<tr>
		
				<td valign="top" class="delete_link">
				<a href="php/delete.php?item=<?= $cartarry[$i] ?>">DELETE</a>
				</td>
			
			<td valign="top" class="cart_quantity">
		
				<input type="text" name="qty-101" size="4" value="1" id="acpro_inp1" readonly>
			
			</td>

			<td width="100%" valign="top" class="cart_name"><?= $cartarry[$i] ?></td>
			<td valign="top" align="right" class="cart_price">$ <?= $pricearry[$i] ?></td>
			<td valign="top" align="right" class="cart_extamount">$ <?= ($pricearry[$i] + ($pricearry[$i] * 0.06)) ?></td>
			</tr>
<?php	       }
           } ?>
			<!-- END CART 101 -->
		
		<!-- START CART FOOTER -->
		<tr>
	
			<td valign="top" colspan="5">
		
		<hr>
		</td>
		</tr>
		<tr>
	
			<td valign="top"></td>
		
		<td valign="top"></td>
		<td width="100%" valign="top"></td>
		<td valign="top" nowrap="" align="right" class="header_cell">Sub Total:</td>
		<td valign="top" align="right" class="value_cell">$ <?= $totalprice ?></td>
		</tr>
	
		</tbody></table>
		</center></div>
		<div align="center">
		<table border="0" cellpadding="0" cellspacing="0" width="90%">

	
			</table>
		</div>
		</form>
	
			<div align="center">
			<center>
			<table border="0" cellpadding="10" cellspacing="0">
			<tbody><tr>
			<td>
			<form method="POST" action="index.html">
			<p align="center"><input type="submit" value="Continue Shopping" name="continue_shopping"></p>
			</form>
			</td>
			<td>
			<form method="POST" action="php/clearcart.php">
			<p align="center"><input type="submit" value="    Clear Cart    " name="clear_cart"></p>
			</form>
			</td>
			<td>
			<form method="POST" action="order.html">
			<p align="center"><input type="submit" value="  Place Order  " name="order_form"></p>
			</form>
			</td>
			</tr>
			</tbody></table>
			</center>
			</div>
		
        </div>            
            </td>
           </tr>
         </table>
         <br />
         <div style="display:none; height:125;" id="formals"> 
         <table border="1" cellspacing="0" cellpadding="3" align="left" style="color:#9CF">
          <tbody>
            <tr bgcolor="#F0F0F0" style="color:#000; font-weight:bold;">
              <td width="68"><font size="-1" face="arial">Item number</font></td>
              <td width="316"><font size="-1" face="arial">Item Description</font></td>
              <td width="103"><font size="-1" face="arial">Price</font></td>
              <td width="160"><font size="-1" face="arial">click to add to cart</font></td>
              <td width="160"><font size="-1" face="arial">click to add to cart</font></td>
            </tr>
            <tr>
              <form action="php/addcart.php" method="post">
              <input type="hidden" name="item" value="Business Suit" />
              <input type="hidden" name="page" value="menmenu.html" />
              <td><font face="Arial">1</font></td>
              <td><font face="Arial">Business Suit</font></td>
              <td background="images/price2.png" ><font face="Arial" color="#FF00CC" size="+1">&nbsp;&nbsp;&nbsp;&nbsp;$ 430</font><input name="price" type="hidden" value="430" size="4" readonly="readonly"/></td>
              <td align="center"><input type="image" border="0" name="add" src="images/addtocart.png" width="100" height="34" /></td>
              <td align="center"><input type="image" border="0" name="add" src="images/view.jpg" width="100" height="34" /></td>
              </form>
            </tr>
            <tr>
              <form action="php/addcart.php" method="post">
              <input type="hidden" name="item" value="Cotton Shirt" />
              <input type="hidden" name="page" value="menmenu.html" />
              <td><font face="Arial">2</font></td>
              <td><font face="Arial">Cotton Shirt</font></td>
              <td background="images/price2.png" ><font face="Arial" color="#FF00CC" size="+1">&nbsp;&nbsp;&nbsp;&nbsp;$ 80</font><input name="price" type="hidden" value="80" size="4" readonly="readonly"/></td>
              <td align="center"><input type="image" border="0" name="add" src="images/addtocart.png" width="100" height="34" /></td>
              <td align="center"><input type="image" border="0" name="add" src="images/view.jpg" width="100" height="34" /></td>
              </form>
            </tr>
          </tbody>
         </table>          
         </div>
        
        <p><br/>
        </p></td>
        </tr>
  </table>
</body>
</html>
