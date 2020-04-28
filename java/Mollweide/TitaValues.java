
public class TitaValues {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		int counter = 0;
		  for (double x = 0; x < 256; x++) {
		   for (double y = 0; y < 128; y++) {
		    counter++;
		    double x2 = ((-2 * Math.sqrt(2)) + (4 * Math.sqrt(2))
		      * (x / 256));
		    double y2 = ((Math.sqrt(2)) - (2 * Math.sqrt(2)) * (y / 128));
		 System.out.println("x2 = " + x2);
		 System.out.println("y2 = " + y2);
		  
	//double	x1 = 1/((2*(1/Math.sin(y2/Math.sqrt(2))+ Math.sin(2*(1/Math.sin(y2/Math.sqrt(2)))))/Math.PI));
    //double  y1 = ((Math.PI*x2)/(2*Math.sqrt(2)*Math.cos(Math.sin(y2/Math.sqrt(2)))));
	
	//	System.out.println("x1 = "+ x1);
	//	System.out.println("y1 = "+ y1);
               	
		   }
		  }
		  System.out.println("Counter" + counter);
		 }
	

	
	
                
	}



