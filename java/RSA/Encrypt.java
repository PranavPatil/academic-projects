/*
 * Cryptology Assignment no: 3
 * RSA Implmentation
 *
 *  Creates encrypted file ciphertext.txt
 */

import java.io.*;
import java.math.*;
import java.util.Random;

public class Encrypt {
  /**
  * Default Class Constructor.
  * Uses predefined public keys.
  */

  public Encrypt(String inputfile) {
	//set the public keys based on input
	this(inputfile, "publickey");
  }

  public Encrypt(String inputfile, String pubkeyfile){

    File publicfile = new File(pubkeyfile);

	if(!publicfile.exists()) {
	   System.out.println("Public Key File not found.");
	   System.exit(1);
	}

	try {
	 	 FileReader pubreader = new FileReader(publicfile);
		 BufferedReader buffer = new BufferedReader(pubreader);

		 String buf = buffer.readLine();
		 publickey_n = new BigInteger(buf.substring(buf.indexOf(':')+2));

		 buf = buffer.readLine();
		 publickey_e = new BigInteger(buf.substring(buf.indexOf(':')+2));

	     buffer.close();
		 pubreader.close();
	} catch (IOException e) {
			// TODO Auto-generated catch block
		e.printStackTrace();
	}

	doEncryption(inputfile);
	System.out.println("Message Encrypted Successfully !!");
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
      buffer = new BufferedInputStream(new FileInputStream(input));
      writer = new OutputStreamWriter(new FileOutputStream(output));
    } catch (FileNotFoundException fnfe) {
      System.out.println("File not found: " + fnfe.getMessage());
	  return false;
    }

	return true;
  }

  /**
  * This method performs the main actions of calling the IO class,
  * ascii encoding of the chunk of 16 chars and encrypting it as plaintext
  */
  public void doEncryption(String plaintxt){

	loadFiles(plaintxt,ciphertxt);
    byte[] tempbuf = new byte[MSG_LEN];

    try {
      //read each line of plaintext and encode it with the keys provided
      while( buffer.read(tempbuf) != -1){

        BigInteger val = new BigInteger(tempbuf);
        val = padding(val);
        //write encoded text to file "encrypt.txt"
        writer.write(encryptText(val,publickey_e,publickey_n));

        if(buffer.available() > 0) {
          writer.write("\r\n");

          if(buffer.available() < MSG_LEN) {
        	  for(int i=0; i< MSG_LEN; i++)
        		  tempbuf[i] = 0;
          }
        }
      }
      writer.close(); //close output file
    }
    catch(IOException ex) {
    	ex.printStackTrace();
    }
  } //end doEncryption

  /**
  * Encrypt the text using E(p) = p^e mod n.
  * @param ascii_val Plaintext value in bytes
  * @param e Private key e
  * @param n Public Key n
  */
  public String encryptText(BigInteger ascii_val, BigInteger e, BigInteger n){

    return (ascii_val.modPow(e,n)).toString();
  } //end encryptText

  public BigInteger padding(BigInteger message)
  {
     byte[] padding=new byte[11];
     padding[0]=0x00;
     padding[1]=0x02;

     for(int i=2;i<padding.length-1;i++)
     {
      padding[i]=(byte)new Random().nextInt();
     }
     padding[padding.length-1]=0x00;
     BigInteger paddingBigInteger=new BigInteger(padding);
     message=message.shiftLeft(11*8);
     message=message.add(paddingBigInteger);
     return message;
  }

   /*
  * Data Members
  */
  private BigInteger publickey_e;
  private BigInteger publickey_n;
  private String ciphertxt = "ciphertext.txt";

  private File input,output; //input & output files
  private OutputStreamWriter writer; //output buffer
  private BufferedInputStream buffer; //input buffer
  private static final int MSG_LEN = 16;

} //end class E
