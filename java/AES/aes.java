package main;

/*
 * Cryptology Project no: 3
 * AES Implmentation
 */

public class aes {

  public static void main (String[] arg) {

  //String[] args = {"-e", "assgn.docx", "key.p12"};
  String[] args = {"-d", "ciphertext", "key.p12"};
  //key.p12    //plaintext
  //
  int len = args.length;

    if(len == 3) {

	  if(args[0].equals("-e")) {
	    new Encrypt(args[1], args[2]);
	  }
	  else if(args[0].equals("-d")) {
	    new Decrypt(args[1], args[2]);
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
	System.out.println("-e inputfile keyfile :     Encrypt the file using the aes key");
	System.out.println("-d outputfile keyfile :     decrypt the file using the aes key and ciphertext");
  }
 }
