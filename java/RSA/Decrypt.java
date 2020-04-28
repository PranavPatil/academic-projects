/*
 * Cryptology Assignment no: 3
 * RSA Implmentation
 *
 * Creates Decrypted file plaintext.bin
 */

import java.io.*;
import java.math.*;

public class Decrypt{
/**
* Default Class Constructor.
* Uses predefined public and private keys.
*/

public Decrypt(String inputfile){
   //set the public keys based on input
   this(inputfile, "privatekey");
}

public Decrypt(String inputfile, String prikeyfile){

	File privatefile = new File(prikeyfile);

	if(!privatefile.exists()) {
        System.out.println("Private Key File not found.");
        System.exit(1);
	}

    try {
		FileReader prireader = new FileReader(privatefile);
	    BufferedReader buffer = new BufferedReader(prireader);

		buffer.readLine();
		buffer.readLine();

		String buf = buffer.readLine();
		publickey_d = new BigInteger(buf.substring(buf.indexOf(':')+2));

		buf = buffer.readLine();
		publickey_p = new BigInteger(buf.substring(buf.indexOf(':')+2));

		buf = buffer.readLine();
		publickey_q = new BigInteger(buf.substring(buf.indexOf(':')+2));

        buffer.close();
		prireader.close();
	} catch (IOException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}

	doDecryption(inputfile, true);
	System.out.println("Message Decrypted Successfully !!");
}

public boolean loadFiles(String plaintext, String ciphertext) {

	input = new File(plaintext);

	if(!input.exists()) {
	 System.out.println("Input File not found !!");
	 System.exit(1);
	}

	output = new File(ciphertext);
	if(!output.exists()) {
	  try {
		  output.createNewFile();
	  } catch (IOException e) {
		  e.printStackTrace();
		  return false;
	  }
    }

    //read in input and prepare output file for writing
    try {
      InputStreamReader reader = new InputStreamReader(new FileInputStream(input));
      bufreader = new BufferedReader(reader);
      writer = new FileOutputStream(output);
    } catch (FileNotFoundException fnfe) {
      System.out.println("File not found: " + fnfe.getMessage());
	  return false;
    }

	return true;
  }

/**
* Decrypt the text using D(c) = c^d mod n.
* @param ciphertext Encoded text
* @param d Private key d
* @param n Public key n   */

public BigInteger decryptText(BigInteger ciphertext, BigInteger d, BigInteger n){

  return (ciphertext.modPow(d,n));
} //end decryptText

public BigInteger fastdecryptText(BigInteger ciphertext, BigInteger d, BigInteger p, BigInteger q){

  BigInteger Cp = ciphertext.modPow(d.mod(p.subtract(INT_ONE)), p);
  BigInteger Cq = ciphertext.modPow(d.mod(q.subtract(INT_ONE)), q);
  BigInteger M = (q.multiply(q.modInverse(p))).multiply(Cp.subtract(Cq)).add(Cq);
  return (M);
} //end decryptText

/**
* This method performs the main actions of calling the IO class
* and decrypting the ciphertext using fast decryption technique
* with the help of chinese remainder theorem.
* It also decodes the chunk of chars from their ascii value
*/
public void doDecryption(String ciphertxt, boolean isFastDecryption){
	loadFiles(ciphertxt, outtxt);
    String temp_str = " ";

    try {
        //read each line of plaintext and encode it with the keys provided
       while((temp_str = bufreader.readLine()) != null){

   	    BigInteger ciphertext = new BigInteger(temp_str);
   	    BigInteger M;

   	    if(isFastDecryption)
   	      M = fastdecryptText(ciphertext, publickey_d, publickey_p, publickey_q);
   	    else
   	      M = decryptText(ciphertext, publickey_d, publickey_p.multiply(publickey_q));

   	    M = removePadding(M);
        writer.write(M.toByteArray());
       }
       bufreader.close();
       writer.close(); //close output file
    }
    catch(IOException ex) {
      ex.printStackTrace();
    }
} //end doDecryption

public BigInteger removePadding(BigInteger message)
{
  message=message.shiftRight(11*8);
  return message;
}

/*
* Data Members
*/
private BigInteger publickey_d;
private BigInteger publickey_p;
private BigInteger publickey_q;
//default filenames for plaintext output
private String outtxt = "plaintext.bin";
private final static BigInteger INT_ONE = new BigInteger("1");

private File input,output; //input & output files
private BufferedReader bufreader; //input buffer
private FileOutputStream writer; //input buffer

} //end class Decrypt
