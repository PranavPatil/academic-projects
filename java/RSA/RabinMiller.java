/*
 * Cryptology Assignment no: 3
 * RSA Implmentation
 *
 */

import java.math.BigInteger;
import java.util.Random;

public class RabinMiller {

	private static final int MAX_BIT = 32;

	/**
	 * @param args
	 */
	public static BigInteger RMPrime(int nbits) {
		// TODO Auto-generated method stub

		boolean found = false;
		BigInteger e = null;

		while(!found) {
	      //e = myBigRanNum(nbits);
		  Random eRand = new Random();
          e = new BigInteger(nbits, eRand);
          e = e.abs();

	      found = MillerRabin(e, 10);
		}
		return e;
	}

	public static boolean MillerRabin(BigInteger n, int k){

        BigInteger a;
        BigInteger m = n.subtract(BigInteger.ONE);
        boolean isPrime = true;

        if (n.compareTo(BigInteger.ONE) == 0) return false;
        if (n.mod(BigInteger.valueOf(2)).intValue() == 0) return false;

        for (int i = 0; i < k; i++) {
            a = myBigRanNum(MAX_BIT);
            if (a.compareTo(m) > 0) a = a.mod(m);
            if (a.compareTo(BigInteger.ZERO) == 0) a = BigInteger.ONE;

            if (isPrime(a, n)) return false;
        }
        return isPrime;
}

   public static boolean isPrime(BigInteger a, BigInteger n) {
        int s;
        BigInteger apow;
        BigInteger firstEqua, secondEqua;
        BigInteger m = n.subtract(BigInteger.ONE);
        BigInteger d;
        s = FactorOfTwo(m);
        d = m.divide(BigInteger.valueOf((int)Math.pow(2, s)));

        firstEqua = ModExpo(a, d, n);
        if (firstEqua.intValue() == 1 || firstEqua.compareTo(m) == 0 ) return false;

        for (int r = 0; r < s; r++) {
            apow = d.multiply(BigInteger.valueOf((int)Math.pow(2, r)));
            secondEqua = ModExpo(a, apow, n);
            if (secondEqua.intValue() == -1 || secondEqua.compareTo(m) == 0) {
                return false;
            }
        }
        return true;
   }

   public static BigInteger ModExpo(BigInteger aBigInt, BigInteger pow, BigInteger n) {
        BigInteger e = pow;
        BigInteger a = aBigInt;
        BigInteger result = BigInteger.ONE;
        while (e.compareTo(BigInteger.ZERO) > 0 ) {
            if (e.and(BigInteger.ONE).compareTo(BigInteger.ONE) == 0 ){
                result = (result.multiply(a)).mod(n);
            }
            e = e.shiftRight(1);
            a = (a.multiply(a)).mod(n);
        }
        return result;
   }

   public static int FactorOfTwo(BigInteger aNumber) {
        int i = 0;
        BigInteger myNumber = aNumber;
        BigInteger myRemainer;
        while (true) {
            myRemainer = myNumber.mod(BigInteger.valueOf(2));
            if (myRemainer.compareTo(BigInteger.ZERO) != 0 ) break;
            myNumber = myNumber.divide(BigInteger.valueOf(2));
            i++;
        }
        return i;
   }

   public static BigInteger myBigRanNum(int numBits) {

        //We only deal with positive number here
        Random aRand = new Random();
        //aRand.nextLong();
        BigInteger aBigInterger = new BigInteger(numBits, aRand);
        aBigInterger = aBigInterger.abs();
        return aBigInterger;
   }
}
