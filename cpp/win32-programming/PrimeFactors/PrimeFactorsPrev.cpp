// PrimeFactors.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include < iostream >
#include < math.h >
#include < vector >
using namespace std;

int getNextPrime(int num) {
bool check_prime = false;
int primeno = -1;

 int i = num + 1;

 while(!check_prime) {
  check_prime = true;

  for ( int j = 2; j <= i/2; j=j+2)
  {
	  //cout << endl << i << " % " << j << endl; 
	  if ( i % j == 0 ) {
      check_prime = false;
	  break;
	  }

	  if(j==2) 
		  j--;
  }

  if ( check_prime == true ) {
    //cout << i << endl;
    primeno = i;
  }
  else
	i++;
 }
 return primeno;
}


int _tmain(int argc, _TCHAR* argv[])
{
unsigned int N = 455555555555555;
/*const int SQR_ROOT_N = (int) (sqrt ((double) N));
bool prime[N + 1];
 
prime[0] = false; // 0 is not prime
prime[1] = false; // 1 is not prime
 
for (int i = 2; i <= N; i++)
prime[i] = true; // flag all numbers as prime initially
 
for (int i = 2; i <= SQR_ROOT_N; i++) {
	cout << endl << "val of i = "  << i;
  if (prime[i]) { // no need for inner loop if i is not prime
	  for (int j = 2 * i; j <= N; j+=i) {
		cout << endl << "val of j = "  << j;
		prime[j] = false; }// j is a multiple of i, thus not prime.
  }
}

// display prime numbers
cout << "Here are the prime numbers\n\n";

for (int i = 2; i <= N; i++) {
  if (prime[i])
   cout << i << endl;
}
*/
 vector<int> factors;
 int prime = 2;

 while(N != 1) { 

   if(N % prime == 0) {
      N = N / prime;
      factors.push_back(prime);
   }
   else {
     prime = getNextPrime(prime);
     //cout << "prime = " << prime;
   }
 }

 int ii;

 for(ii=0; ii < (int)factors.size(); ii++)  {

	cout << endl << " factors : " << factors[ii];
    // pause after printing 20 lines
 }
 cout << endl;
 factors.clear();

 return 0;
}