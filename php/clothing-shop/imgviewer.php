<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
  <head>
  <title>Image Viewer</title>
  <script language="javascript" type="text/javascript"> 
         <!--
         var interval = null; // keeps track of the interval
         var speed = 6; // determines the speed of the animation
         var count = 0; // size of the image during the animation

         // called repeatedly to animate the book cover
         function run()
         {
            count += speed;

            // stop the animation when the image is large enough
            if ( count >= 375 ) 
            {
               window.clearInterval( interval );
               interval = null;
            } // end if

            var bigImage = document.getElementById( "imgCover" );
            bigImage.style.width = .7656 * count + "px";
            bigImage.style.height = count + "px";
         } // end function run

         // inserts the proper image into the main image area and
         // begins the animation
         function display( imgfile )
         {
            if ( interval )
               return;
      
            var bigImage = document.getElementById( "imgCover" );
            var newNode = document.createElement( "img" );
            newNode.id = "imgCover";
            newNode.src = imgfile;
            newNode.alt = "Large image";
   			newNode.className = "imgCover";
            newNode.style.width = "0px";
            newNode.style.height = "0px";
            bigImage.parentNode.replaceChild( newNode, bigImage );
            count = 0; // start the image at size 0
            interval = window.setInterval( "run()", 10 ); // animate
         } // end function display
         // -->
   </script>
   </head>
   <?php  
      $imgfile = isset($_GET['imgfile']) ? $_GET['imgfile']: NULL;
      $imgfile = "images/" . $imgfile;
   ?>
   
   <body onload="display( '<?php echo $imgfile; ?>')"> 
         <img id="imgCover" src="./Asali's favorate pictures_files/NX.jpeg" alt="Large image" class="imgCover" style="width: 289.397px;          height: 378px; ">
   </body>
   </html>