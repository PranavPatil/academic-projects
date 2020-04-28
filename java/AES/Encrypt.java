package main;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class Encrypt {

 final int Nb = 4; // no of colmns in the state
 int Nr=0;  // no of rounds
 int Nk=0;  // no of keys

 char[][] state = new char[4][4];
 char[] RoundKey = new char[240];
 
 
  public Encrypt(String inputfile, String keyfile){
  
    Nr = 256;		
	Nk = Nr / 32;
	Nr = Nk + 6;

	File kfile = new File(keyfile);
	File ipfile = new File(inputfile);
	char[] tkey = new char[32];
	char[] ptxt = new char[16];
	char[] ctxt;
			
	if(!kfile.exists()) {
        System.out.println("Secret Key File not found.");
        System.exit(1);
	}
	
	if(!ipfile.exists()) {
        System.out.println("Input File not found.");
        System.exit(1);
	}

	try {
      FileInputStream kin = new FileInputStream(kfile);    	
      FileInputStream ptin = new FileInputStream(ipfile);    	
      FileOutputStream ctout = new FileOutputStream("ciphertext");    	

      int i = 0;
      int val = 0;
      
	  while (i < 32 && (val = kin.read()) != -1) {
		tkey[i] = (char)val; 
        i++;
	  }
      
	  if(i != 32) {
        System.out.println("Invalid secret key for AES 256.");
        System.exit(1);
	  }
	  
	  KeyExpansion(tkey);
	  
	  while(ptin.available() > 0) {
		  
		i = 0;
		val = 0;
		  
		while (i < ptxt.length && (val = ptin.read()) != -1) {
		  ptxt[i] = (char)val;
		  i++;
		}		  		 
		
		ctxt = Cipher(ptxt);
		System.out.println();
		
		for(i=0;i<Nb*4;i++)
		{
			System.out.print(Integer.toHexString((int)ctxt[i]));
		}

		i=0;
		while (i < ctxt.length) {
		  ctout.write(ctxt[i]);
		  i++;
        }
	  }
	  
	  kin.close();
	  ptin.close();
	} catch (IOException e) {
			// TODO Auto-generated catch block
	   e.printStackTrace();
	}

	System.out.println("Message Encrypted Successfully !!");
  }

 int getSBoxValue(int num)
 {
 	int[] sbox =   {
 			
 	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, //0
 	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, //1
 	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, //2
 	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, //3
 	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, //4
 	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, //5
 	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, //6
 	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, //7
 	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, //8
 	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, //9
 	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, //A
 	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, //B
 	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, //C
 	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, //D
 	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, //E
 	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }; //F
// 	if(num == 495)
// 		System.exit(1);
 	return sbox[num];
 }

 int[] Rcon = {
		0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 
		0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 
		0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 
		0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 
		0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 
		0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 
		0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 
		0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 
		0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 
		0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 
		0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 
		0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 
		0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 
		0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 
		0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 
		0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb  };

	// This function produces Nb(Nr+1) round keys. The round keys are used in each round to encrypt the states. 
  void KeyExpansion(char[] Key)
  {
	int i,j;
	char[] temp = new char[4];
	char k;
		
	// The first round key is the key itself.
	for(i=0;i<Nk;i++)
	{
		RoundKey[i*4]=Key[i*4];
		RoundKey[i*4+1]=Key[i*4+1];
		RoundKey[i*4+2]=Key[i*4+2];
		RoundKey[i*4+3]=Key[i*4+3];
	}

	// All other round keys are found from the previous round keys.
	while (i < (Nb * (Nr+1)))
	{
		for(j=0;j<4;j++)
		{
		  temp[j]=RoundKey[(i-1) * 4 + j];
		}
		if (i % Nk == 0)
		{
		   // This function rotates the 4 bytes in a word to the left once.
		   // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]
		   // Function RotWord()
		   {
			  k = temp[0];
			  temp[0] = temp[1];
			  temp[1] = temp[2];
			  temp[2] = temp[3];
			  temp[3] = k;
		   }
		   
		   // SubWord() is a function that takes a four-byte input word and 
		   // applies the S-box to each of the four bytes to produce an output word.
		   // Function Subword()
		   {
			 temp[0]=(char) getSBoxValue(temp[0]);
			 temp[1]=(char) getSBoxValue(temp[1]);
			 temp[2]=(char) getSBoxValue(temp[2]);
			 temp[3]=(char) getSBoxValue(temp[3]);
		   }
		   
		   temp[0] =  (char) (temp[0] ^ Rcon[i/Nk]);  /////////////// Rcon
		}
		else if (Nk > 6 && i % Nk == 4)
		{
			// Function Subword()
			{
			  temp[0]=(char) getSBoxValue(temp[0]);
			  temp[1]=(char) getSBoxValue(temp[1]);
			  temp[2]=(char) getSBoxValue(temp[2]);
			  temp[3]=(char) getSBoxValue(temp[3]);
			}
		}
		
		RoundKey[i*4+0] = (char) (RoundKey[(i-Nk)*4+0] ^ temp[0]);
		RoundKey[i*4+1] = (char) (RoundKey[(i-Nk)*4+1] ^ temp[1]);
		RoundKey[i*4+2] = (char) (RoundKey[(i-Nk)*4+2] ^ temp[2]);
		RoundKey[i*4+3] = (char) (RoundKey[(i-Nk)*4+3] ^ temp[3]);
		i++;
	}
  }

void AddRoundKey(int round) 
{
	int i,j;
//	System.out.print("\nkey --->  ");
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
//			state[j][i] ^= RoundKey[round * Nb * 4 + i * Nb + j];
//			state[i][j] = (char)(state[i][j] ^ RoundKey[round * Nb * 4 + i * Nb + j]);
			state[j][i] = ((char)(state[j][i] ^ RoundKey[round * Nb * 4 + i * Nb + j]));
			
//			if(RoundKey[round * Nb * 4 + i * Nb + j] < 16)
// 	        System.out.println((round * Nb * 4 + i * Nb + j) + " = 0" + Integer.toHexString((int)RoundKey[round * Nb * 4 + i * Nb + j]));
//			else
//	 	    System.out.println((round * Nb * 4 + i * Nb + j) + " = " + Integer.toHexString((int)RoundKey[round * Nb * 4 + i * Nb + j]));
		}
	}
	//System.out.println();
//	System.exit(0);
}

//The SubBytes Function Substitutes the values in the
//state matrix with values in an S-box.
void SubBytes()
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			state[i][j] = (char) (getSBoxValue(state[i][j]));

//			if(state[j][i] == 0)
//		       System.out.println("00");
// 	        else
//		       System.out.println(Integer.toHexString((int)state[j][i]));
		}
	}
//	System.exit(0);
}

//The ShiftRows() function shifts the rows in the state to the left.
//Each row is shifted with different offset.
//Offset = Row number. So the first row is not shifted.
void ShiftRows()
{
	char temp;

	// Rotate first row 1 columns to left	
	temp=state[1][0];
	state[1][0]=state[1][1];
	state[1][1]=state[1][2];
	state[1][2]=state[1][3];
	state[1][3]=temp;

	// Rotate second row 2 columns to left	
	temp=state[2][0];
	state[2][0]=state[2][2];
	state[2][2]=temp;

	temp=state[2][1];
	state[2][1]=state[2][3];
	state[2][3]=temp;

	// Rotate third row 3 columns to left
	temp=state[3][0];
	state[3][0]=state[3][3];
	state[3][3]=state[3][2];
	state[3][2]=state[3][1];
	state[3][1]=temp;
}

//xtime is a macro that finds the product of {02} and the argument to xtime modulo {1b}  
//xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))

//MixColumns function mixes the columns of the state matrix
void MixColumns()
{
	int i;
	char Tmp,Tm,t;
	for(i=0;i<4;i++)
	{	
		t=state[0][i];
//		System.out.print("No = " + Integer.toHexString((int)t));
		
		Tmp = (char) (state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i]) ;
//		System.out.print(" " + Integer.toHexString((int)Tmp));

		Tm = (char) (state[0][i] ^ state[1][i]) ; Tm = xtime(Tm); 
		state[0][i] ^= Tm ^ Tmp ; //state[0][i] ^= 0xff;
//		System.out.print(" " + Integer.toHexString((int)state[0][i]));
			  
		Tm = (char) (state[1][i] ^ state[2][i]) ; Tm = xtime(Tm); 
		state[1][i] ^= Tm ^ Tmp ; //state[1][i] ^= 0xff;
//		System.out.print(" " + Integer.toHexString((int)state[1][i]));

		Tm = (char) (state[2][i] ^ state[3][i]) ; Tm = xtime(Tm); 
		state[2][i] ^= Tm ^ Tmp ; //state[2][i] ^= 0xff;
//		System.out.print(" " + Integer.toHexString((int)state[2][i]));

		Tm = (char) (state[3][i] ^ t) ; Tm = xtime(Tm); 
		state[3][i] ^= Tm ^ Tmp ; //state[3][i] ^= 0xff;
//		System.out.println(" " + Integer.toHexString((int)state[3][i]));
	}
}

 char xtime(char x) {
	// TODO Auto-generated method stub
	return (char) ((char)((x<<1) & 0xff) ^ ((((x>>7)& 0xff) & 1) * 0x1b));
}


//Cipher is the main function that encrypts the PlainText.
char[] Cipher(char[] in)
{
	int i,j,round=0;
	char[] ctxt = new char[16];

	//Copy the input PlainText to state array.
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			state[j][i] = in[i*4 + j];
		}
	}

	// Add the First round key to the state before starting the rounds.
	display();
	AddRoundKey(0); 
	
	// There will be Nr rounds.
	// The first Nr-1 rounds are identical.
	// These Nr-1 rounds are executed in the loop below.
	for(round=1;round<Nr;round++)
	{
		display();
		SubBytes();
		display();
		ShiftRows();
		display();
		MixColumns();
		display();
		AddRoundKey(round);
	}
	
	// The last round is given below.
	// The MixColumns function is not here in the last round.
	display();
	SubBytes();
	display();
	ShiftRows();
	display();
	AddRoundKey(Nr);
	display();

	// The encryption process is over.
	// Copy the state array to output array.
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
		  ctxt[i*4+j]=state[j][i];
		}
	}
	
	return ctxt;
}

public void display() {

	System.out.print("value = ");
	int val = 0;
	
	for(int i = 0; i< 4; i++)
		for(int j = 0; j< 4; j++) {
	      val = (int)state[j][i];
	      
	      if(val == 0)
		       System.out.print("00");
			else if(val < 16)
			       System.out.print("0" + Integer.toHexString(val));
	      else
		       System.out.print(Integer.toHexString(val));
		}

	System.out.println();
	System.out.println();
}

}

