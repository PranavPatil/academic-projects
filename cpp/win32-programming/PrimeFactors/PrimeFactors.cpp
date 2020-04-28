// PrimeFactors.cpp : Defines the entry point for the console application.
//

// ASSIGNMENT NO: 5
// PRANAV PATIL

#include "stdafx.h"
#include < iostream >
#include < math.h >
#include < vector >
#include < queue >
#include < list >
#include < process.h >
#include < windows.h >
#include < limits >     //for numeric_limits
#include < errno.h >    // 'atoi' does not reset 'errno' in the case of no error

using namespace std;

queue<unsigned int>  numbers; // declare a queue to store the input numbers.
volatile bool status = true;  // used to check status of the threads.
HANDLE noSemaphore;  // manages synchronized access to input number queue.
HANDLE opThdMutex;   // avoids overlapping input and outputs on the console.

int getNextPrime(int num) {  // function to get the next prime number after the given number.
bool check_prime = false;
int primeno = -1;

 int i = num + 1;

 while(!check_prime) {
  check_prime = true;

  for ( int j = 2; j <= i/2; j=j+2)
  {
	  if ( i % j == 0 ) {
      check_prime = false;
	  break;
	  }

	  if(j==2) 
		  j--;
  }

  if ( check_prime == true ) {
    primeno = i;
  }
  else
	i++;
 }
 return primeno;
}

unsigned __stdcall outputThread(PVOID pvParam) { // output thread function

 vector<int> factors;  // vector to store the output prime factors.

 while(status) {  // check status of the appln.

   while(numbers.empty()) {  // when no numbers to process in queue sleep.
     Sleep(200);
   }

   DWORD result = WaitForSingleObject( noSemaphore,   // handle to semaphore
                                       INFINITE); // infinite interval

   if(result == WAIT_OBJECT_0 && !numbers.empty()) { // check again if queue is empty.

     unsigned int N = numbers.front();   // get the first number inerted in the queue.
     unsigned int num = N;               // num is used to buffer the number for display.
     numbers.pop();  // pop the first entry from the queue.

     if (!ReleaseSemaphore( noSemaphore,  // handle to semaphore
                            1,            // increase count by one
                            NULL) )       // not interested in previous count
     {
	   char buffer[50];
	   cerr << endl << "Release noSemaphore error: outputThread P1: " 
		    << strerror_s(buffer, 49, GetLastError()) << endl;
	   return 1;
     }

     int prime = 2;

     while(N > 0 && N != 1) {  // check if number is +ve and reduces to 1.

       if(N % prime == 0) {  // check if number divisible by prime no.
          N = N / prime;
	      factors.push_back(prime);  // push next prime no in queue.
       }
       else {
         prime = getNextPrime(prime); // get next prime number.
       }
     }

	 // opThdMutex to avoid input and output overlapping on the Console.
	 result = WaitForSingleObject( opThdMutex,   // handle to semaphore
                                       INFINITE); // zero-second time-out interval

     if(result == WAIT_OBJECT_0) {

	   // Display the resultant prime factors of the Number.
       int ii;
	   cout << endl << " Number : " << num << endl;

       for(ii=0; ii < (int)factors.size(); ii++)  {

         cout << endl << " factors " << (ii+1) << " : " << factors[ii];

	     if(((ii+1) % 20) == 0 ) {
		     cout << endl;
		     getchar();  // pause after printing 20 lines...... Test no : 134217728
		     getchar();
	     }         
       }
       cout << endl;
       factors.clear(); // clear the display buffer.

       if (! ReleaseMutex(opThdMutex)) // Release mutex
       { 
  	     char buffer[50];
	     cerr << endl << "Release opThdMutex error: " 
		      << strerror_s(buffer, 49, GetLastError()) << endl;
	     return 1;
       } 
	 }
   }
   else if(result == WAIT_OBJECT_0 && numbers.empty()) {

     // release the semaphore if in case got hold of it.
	 if (!ReleaseSemaphore( noSemaphore,  // handle to semaphore
                            1,            // increase count by one
                            NULL) )       // not interested in previous count
     {
	   char buffer[50];
	   cerr << endl << "Release noSemaphore error: outputThread P2: " 
		    << strerror_s(buffer, 49, GetLastError()) << endl;
	   return 1;
     }
   }
 }
 return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
 unsigned int temp = 0;

 noSemaphore = CreateSemaphore( 
        NULL,           // default security attributes
        1,  // initial count
        1,  // maximum count
        TEXT("Input"));          // named semaphore

    if (noSemaphore == NULL) 
    {
        cerr << endl << "Create noSemaphore error: " << GetLastError();
        return 1;
    }

 opThdMutex = CreateMutex(NULL, FALSE, NULL); // Create Mutex, calling thread being not the
                                              // owner of the mutex.

	if (opThdMutex == NULL) 
    {
        cerr << endl << "Create opThdMutex error: " << GetLastError();
        return 1;
    }

 // fork the first output thread
 unsigned threadID1;
 HANDLE outputThread1;
 outputThread1 = (HANDLE)_beginthreadex(NULL, 0, &outputThread, NULL, 0, &threadID1);

 Sleep(100);

 // fork the second output thread
 HANDLE outputThread2;
 unsigned threadID2;
 outputThread2 = (HANDLE)_beginthreadex(NULL, 0, &outputThread, NULL, 0, &threadID2);

 bool run = true; 

 while(run) { // check whether to continue getting numbers from the user.

   // Wait until number semaphore is free and no thread is currently holding the mutex.
   HANDLE hld[2];
   hld[0] = noSemaphore;
   hld[1] = opThdMutex;
   DWORD result = WaitForMultipleObjects( 2, hld,   // handle to semaphore
                                       TRUE, INFINITE);           // zero-second time-out interval
   
   if(result == WAIT_OBJECT_0) {

	 char str[100];
     cout << endl << "Please enter the numbers <max 5> <Press 'e' to exit> :";
     cin.getline(str, 99); // get the input numbers

     if(_stricmp(str, "e") == 0) { // check the exit condition.
		run = false;
	 }
	 else {
       int no = 0;
	   char *seps   = " ";
       char *token1, *next_token1;

	   // Parse the string into tokens and get all the integers entered into the number queue.
	   // when max no 20 - > console goes in infinite loop.
	   // when max no 10 - > works good for small inputs, on large inputs skips most of nos.
       while(no == 0 || (token1 != NULL && no < 5)) {  // allow only 5 numbers

	  	if(no == 0)
          token1 = strtok_s( str, seps, &next_token1); // initial setting of string in next token
		else
          token1 = strtok_s( NULL, seps, &next_token1); // get the next token of the remaining.
		  
		if(token1 != NULL) {
		  temp = 0;
          temp = atoi( token1 ); // convert string to integer.

		  if (errno == ERANGE) { // check for out of range entries.
		    cout << endl << endl << "Number: " << token1 << " : Input number out of range !!" << endl;
			errno = 0;
		  }
		  else if(temp < 1)
		    cout << endl << endl << "Number: " << token1 << " : Invalid input !!" << endl;
		  else if(strcspn( token1, "-" ) == 0) // check negative numbers.
		    cout << endl << endl << "Number: " << token1 << " : Negative input !!" << endl;
		  else if(numbers.empty())  // when queue empty push the number
			numbers.push(temp);
		  else if(!numbers.empty() && numbers.back() != temp)
		    numbers.push(temp);  // check for duplicates with most recent number in queue.
	      else if(!numbers.empty() && numbers.back() == temp)
			  cout << endl << endl << "Number: " << temp << " : Duplicate input !!" << endl;
		}
		no++;
       }
	   cout << endl;
	 }

     if (!ReleaseSemaphore( noSemaphore,  // handle to semaphore
                            1,            // increase count by one
                            NULL) )       // not interested in previous count
     {
	   char buffer[50];
	   cerr << endl << "Release noSemaphore error: inputThread: " 
		    << strerror_s(buffer, 49, GetLastError()) << endl;
	   return 1;
     }

     if (! ReleaseMutex(opThdMutex)) {   // release mutex.
  	   char buffer[50];
	   cerr << endl << "Release opThdMutex error: " 
	        << strerror_s(buffer, 49, GetLastError()) << endl;
	   return 1;
     } 
   }
 }

 while(!numbers.empty()) { // allow output threads to process the remaining numbers in the queue.
	 Sleep(1000);
 }
 Sleep(500);
 status = false;  // set running status of the threads to shutdown.
 Sleep(1500);

 CloseHandle(outputThread1); // Closing a thread handle does not terminate 
 CloseHandle(outputThread2); // the associated thread or remove the thread object
 CloseHandle(noSemaphore);   // Close all handles, even when it is taken care by the kernel.
 CloseHandle(opThdMutex);

return 0;
}