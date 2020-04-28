/*
 * Cryptology Assignment no: 3
 * RSA Implmentation
 * 
 */

public class rsa {

  public static void main (String args[]) {

  int len = args.length;

   if(len == 1){

	 if(args[0].equals("-g")) {
	   RSAKeyGen rsakeygen = new RSAKeyGen();
	   rsakeygen.writePublicKey();
	   rsakeygen.writePrivateKey();
	 }
     else {
       message();
     }
   }
   else if(len == 2) {

	if(args[0].equals("-e")) {
	  new Encrypt(args[1]);
	}
	else if(args[0].equals("-d")) {
	  new Decrypt(args[1]);
	}
	else
	  message();
    }
   else
     message();
}

  public static void message() {

	System.out.println();
	System.out.println("Usage: java rsa -options [args....]");
	System.out.println();
	System.out.println("where the options include");
	System.out.println("-g :              Generate RSA public and private keys and store in files");
	System.out.println("-e filename :     Encrypt the file using the generated rsa keys");
	System.out.println("-d filename :     decrypt the file using the generated rsa keys");
  }
 }
