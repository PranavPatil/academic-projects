/*
 * Cryptology Assignment no: 3
 * RSA Implmentation
 *
 *  Creates files 'privatekey' and 'publickey'
 */

import java.io.*;
import java.math.BigInteger;

public class RSAKeyGen {
	/**
	* Class Constructor.
	* This method randomly generates p and q and corresponding
	* values for exponent e and d.
	*/

	public RSAKeyGen(){
	System.out.println("Please be patient, the program takes a few minutes to generate the keys. . .\n");

	prime_p = RabinMiller.RMPrime(KEY_SIZE);
	prime_q = RabinMiller.RMPrime(KEY_SIZE);
	BigInteger phi = (prime_p.subtract(INT_ONE)).multiply(prime_q.subtract(INT_ONE));

//	BigInteger my_public_key = RabinMiller.myBigRanNum(16);
	/* Make sure that the chose value for e is relatively prime with phi.
	* Otherwise, increment e by 2 till we find
	* a relatively prime number.
	*/
//	while( !my_public_key.gcd(phi).equals(INT_ONE)){
//	my_public_key = my_public_key.add(INT_TWO);
//	}

	//set the public and private keys.
	publickey_e = RabinMiller.RMPrime(16);
	privatekey_d = publickey_e.modInverse(phi);

	System.out.println("Public and Private Keys Successfully Generated. . .\n");
	}
	/**
	* Get method to return Private key d - for decryption routine.
	* @return BigInteger containing the private key
	*/
	public BigInteger getPrivateKey(){
	return this.privatekey_d;
	}
	/**
	* Get method to return Public key e - for encryption routine.
	* @return BigInteger containing the public key
	*/
	public BigInteger getPublicKey(){
	return this.publickey_e;
	}
	/**
	* Get method to return Public key n - for decryption and encryption
	* routines.
	* @return BigInteger containing the public key
	*/
	public BigInteger getN(){
	return (prime_p.multiply(prime_q));
	}

	public void writePublicKey() {

		File publicfile = new File("publickey");

		if(!publicfile.exists()) {
			try {
				publicfile.createNewFile();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

	    try {
			FileWriter pubwriter = new FileWriter(publicfile);
            pubwriter.write("public ( modulus: " + getN());
			pubwriter.write("\r\n");
            pubwriter.write(", publicExponent: " + publickey_e);
			pubwriter.write("\r\n)");
			pubwriter.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void writePrivateKey() {

		File privatefile = new File("privatekey");

		if(!privatefile.exists()) {
			try {
				privatefile.createNewFile();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

	    try {
			FileWriter priwriter = new FileWriter(privatefile);
            priwriter.write("private (0, modulus: " + getN());
			priwriter.write("\r\n");
            priwriter.write(", publicExponent: " + publickey_e);
			priwriter.write("\r\n");
            priwriter.write(", privateExponent: " + privatekey_d);
			priwriter.write("\r\n");
            priwriter.write(", prime1: " + prime_p);
			priwriter.write("\r\n");
            priwriter.write(", prime1: " + prime_q);
			priwriter.write("\r\n");

            priwriter.write(", exponent1: " + privatekey_d.mod(prime_p.subtract(INT_ONE)));
			priwriter.write("\r\n");
            priwriter.write(", exponent2: " + privatekey_d.mod(prime_q.subtract(INT_ONE)));
			priwriter.write("\r\n");
            priwriter.write(", coefficient: "  + prime_q.subtract(INT_ONE).mod(prime_p));
			priwriter.write("\r\n)");

			priwriter.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	/**
	* Data members
	*/
	private final static BigInteger INT_ONE = new BigInteger("1");
//	private final static BigInteger INT_TWO = new BigInteger("2");
	private final static int KEY_SIZE = 1024;
	private BigInteger privatekey_d;
	private BigInteger publickey_e;
	private BigInteger prime_p;
	private BigInteger prime_q;

} //end class RSA
