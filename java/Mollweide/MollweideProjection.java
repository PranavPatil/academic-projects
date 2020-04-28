

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Scanner;

import javax.imageio.ImageIO;

/*
 * Created on Sep 7, 2010
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */

/**
 * @author pranav
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class MollweideProjection {

	  public static void main(final String[] args) {
	  	try {
	  	    File file = new File(args[0]);
		    File file_out = new File(args[1]);
		  	BufferedImage bi = ImageIO.read (file);
		  	BufferedImage ob = new BufferedImage (bi.getWidth(), bi.getHeight(), BufferedImage.TYPE_INT_ARGB);

		  	int pixelColor = 0;
		  	double xdash = 0;
		  	double ydash = 0;
		  	double phi = 0;
		  	double lambda = 0;
		  	double theta = 0;
		  	double xpixel = 0;
		  	double ypixel = 0;
		  	int xwidth = bi.getWidth();
		  	int yheight = bi.getHeight();

			for (int x=0; x< xwidth; x++){
		  		for (int y=0;  y<yheight; y++){

		  			//pixelColor = bi.getRGB (x,y);

		  			xdash = (-2 * (Math.sqrt(2))) + ((4 * (Math.sqrt(2))) * (x / (double)xwidth));
		  			ydash = ((Math.sqrt(2))) - ((2 * (Math.sqrt(2))) * (y / (double)yheight));
		  			//ydash = ((2 * (Math.sqrt(2))) * (Math.abs(y - 64) / 64.0));

		  			theta = Math.asin(ydash / Math.sqrt(2));

		  			phi = Math.asin(((2 * theta) + Math.sin(2*theta)) / Math.PI);
		  			lambda = (Math.PI * xdash) / (2 * Math.sqrt(2) * Math.cos(theta));

		  			if(lambda > 0)
		  				xpixel = (xwidth / 2.0) + (((Math.abs(lambda)) / Math.PI) * (xwidth / 2.0));
		  			else
		  				xpixel = (xwidth / 2.0) - (((Math.abs(lambda)) / Math.PI) * (xwidth / 2.0));

		  			if(phi < 0)
		  				ypixel = (yheight / 2.0) + (((Math.abs(phi)) / (Math.PI / 2)) * (yheight / 2.0));
		  			else
		  				ypixel = (yheight / 2.0) - (((Math.abs(phi)) / (Math.PI / 2)) * (yheight / 2.0));

		  			//System.out.println("YDash: " + ydash + ", X: " + xpixel + ", Y: " + ypixel);

		  			try{
		  				ob.setRGB(x,y, bi.getRGB((int)xpixel, (int)ypixel));
		  			} catch (Exception e) {
		  				ob.setRGB(x, y, Color.TRANSLUCENT);
		  			}
		  		}
		  	}

		  	ImageIO.write (ob, "png", file_out);
	  	}
	  	catch(IOException ex) {
	  		System.out.println("IOException" + ex.getMessage());
	  	}

	  }
}
