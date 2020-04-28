// A short program to demonstrate dynamic memory allocation 
// using a structured exception handler. 

// ASSIGNMENT NO 6
// PRANAV PATIL

#include < windows.h >
#include < tchar.h >
#include < iostream >              // For printf
#include < stdlib.h >             // For exit

#define PAGELIMIT 1            // Number of pages to ask for
using namespace std;

VOID ErrorMessage(LPTSTR lpMsg)
{
  DWORD dw = GetLastError();  // get the last error code
  LPWSTR pBuffer = NULL;

	  // use formatmessage to get the error code message
    DWORD ok = FormatMessage( 
          FORMAT_MESSAGE_ALLOCATE_BUFFER | 
          FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
          NULL,
          dw,
          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
          (LPWSTR)&pBuffer, 
          0, NULL );

	if(ok) {
		if(dw == 87) // just to remind that the flags are not supported by virtualalloc
		  wcout << L"Error : Flag not supported by VirtualAlloc " << " : " << pBuffer << endl;
		else
		  wcout << L"Error : " << lpMsg << " : " << pBuffer << endl;
	}
}

 int filter(unsigned int code) {

	 // check if exception code is access_violation
   if (code == EXCEPTION_ACCESS_VIOLATION) {

	  cerr << endl << "ERROR : Memory Access Violation" << endl;
      return EXCEPTION_EXECUTE_HANDLER;
   }

   else {

	  cerr << endl << " ERROR : Unexpected Error" << endl;
      return EXCEPTION_CONTINUE_SEARCH;

   };
} 

 // PREVIOUS CODE FOR EXECUTE
 /*typedef int (__stdcall  MyFunctionType)(int a, int b, int c);  // Function Type

int __stdcall  MyFunction(int a, int b, int c) {   // Function to execute
  return a * b * c;
}

void __stdcall MyFunction_END() {}   // End Function
*/

int _tmain(int argc, _TCHAR* argv[])
{
  DWORD dwPageSize;               // Page size on this computer
  void* lpPtr;                 // Generic character pointer
  SYSTEM_INFO sSysInfo;         // Useful information about the system

  GetSystemInfo(&sSysInfo);     // Initialize the structure.
  dwPageSize = sSysInfo.dwPageSize; // Get total size of memory

  while(true) {

    int option = 0; 
    DWORD Protect;  // variable to set the page access mode.

    cout << endl << "-----------------------------------------------------------";
    cout << endl << "  Welcome to Memory Management Class and the Assignment !";
    cout << endl << "-----------------------------------------------------------" << endl;

    cout << endl << " 1) PAGE_NOACCESS";
    cout << endl << " 2) PAGE_READONLY";
    cout << endl << " 3) PAGE_READWRITE";
    cout << endl << " 4) PAGE_EXECUTE";
    cout << endl << " 5) PAGE_EXECUTE_READ";
    cout << endl << " 6) PAGE_EXECUTE_READWRITE";
    cout << endl << " 7) PAGE_WRITECOPY";
    cout << endl << " 8) PAGE_EXECUTE_WRITECOPY";
    cout << endl << " Enter you choice <Enter 0 to EXIT> : ";
    cin >> option;
    cout << endl;

    switch (option) {

	    case 1:
	  	        Protect = PAGE_NOACCESS;
			    break;
	    case 2:
		        Protect = PAGE_READONLY;
		        break;
	    case 3:
		        Protect = PAGE_READWRITE;
		        break;
	    case 4:
		        Protect = PAGE_EXECUTE;
			    break;
	    case 5:
		        Protect = PAGE_EXECUTE_READ;
			    break;
	    case 6:
		        Protect = PAGE_EXECUTE_READWRITE;
			    break;
	    case 7:
		        Protect = PAGE_WRITECOPY;
			    break;
	    case 8:
		        Protect = PAGE_EXECUTE_WRITECOPY;
			    break;
	    case 0:
			    exit (0);
	    default:
		        Protect = PAGE_NOACCESS;
			    break;
    }

    lpPtr = VirtualAlloc( NULL,                 // System selects address
                          PAGELIMIT*dwPageSize, // Size of allocation
		  			      MEM_COMMIT | MEM_RESERVE,    // Reserve and Commit Pages
                          Protect);       // Protection = no access
   if (lpPtr == NULL )

     ErrorMessage(TEXT("VirtualAlloc commit failed"));

   else {

    option = 0; // reset the option

    while(option != 4 && lpPtr != NULL) {


      cout << endl << " 1) READ";
      cout << endl << " 2) WRITE";
      cout << endl << " 3) EXECUTE";
      cout << endl << " 4) EXIT";
      cout << endl << " Enter you choice : ";
      cin >> option;
      cout << endl;

      switch (option) {

 	    case 1:
		        __try {

                 _tprintf(TEXT("Value in Memory :  %s. \n"), lpPtr); // Try to read the memory
			    }
                __except(filter(GetExceptionCode())){
                }

			   break;
	  case 2:
		       __try {

				 wchar_t buffer[20] = L"Hello World";
 		         memcpy(lpPtr, buffer, 19);   // copy data into memory
				 cout << endl << "Value Written Successfully." << endl;
			   }
               __except(filter(GetExceptionCode())){
               }

			   break;
	  case 3:
		       __try {

			     /* PREVIOUS CODE
                 //Determine size of MyFunction by taking the difference
                 //of addresses of MyFunction and MyFunction_END:
                 size = abs((int)MyFunction_END - (int)MyFunction) + 1;
                 //Allocate memory with read/write and execute permissions
                 lpPtr = (BYTE*)VirtualAlloc(NULL, size, MEM_COMMIT, Protect);
                 //Copy memory from the function into the buffer
				 memcpy(lpPtr, MyFunction, size);    THIS CODE DOESN'T COPIES THE FUNCTION BUT COPIES THE JMP INSTRUCTION
                 //Cast the buffer to a function pointer
                 mf = (MyFunctionType*)lpPtr;
                 //Execute the function
                 result = mf(2, 3, 4);    WHEN TRY TO EXECUTE THIS THIS DOEN'T LOAD THE ACTUAL FUNCTION
				                          LOADS THE JMP INTRUCTIONS AND TRIES EXECUTING THE GARBAGE MEMORY
				 */

				 // Code Dr Ford explained in Class
				 ((char*) lpPtr)[0] = (char) 0xc3;  // loads the opcode for return instruction into the memory 
				 __asm {
					 call [lpPtr];                  // tries to execute the return instruction
				 }
				 cout << endl << "Return executed Successfully." << endl;
			   }
               __except(filter(GetExceptionCode())){
               }

			   break;
	  case 4:
		       // Do nothing.
			   break;
	  default:
			   break;
      }
    }

	if(VirtualFree( lpPtr,         // Base address of block
                    0,            // Bytes of committed pages
                    MEM_RELEASE)  // Release the pages
		  		    == 0) {
	  ErrorMessage(TEXT("VitrualFree Release Memory failed"));
    }
  }
 }
}
