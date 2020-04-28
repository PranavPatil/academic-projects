
// ASSIGNMENT NO 10: STRUCTURED EXCEPTION HANDLING
// PRANAV A PATIL

#include "stdafx.h"
#include "exfunction.h"
#include < iostream >
#include < vector >
#include < list >
#include < windows.h >

using namespace std;

 vector<void*> funs;  // vector to store the function pointers.
 vector<wchar_t*> output;  // vector to store the exception output text.
 vector<int> vecpriority;  // vector to store the vector exception handling firsthandlers.
 list<PVOID> vecexptrs;   // list of all vectored exception handlers to remove.
 list<PVOID> veccnptrs;   // list of all vectored continue handlers to remove.

 int priority_idx = 0;    // Index for traversal of vecpriority vector.
 
 void  ExceptionFn(int v) {   // Function to execute

  if(v > -1 && v < (int)funs.size())  // check if v lies in the limits of the funs vector
    ((MyFunctionType)funs[v])(v-1);   // call function using fn ptr specifying parameter
                                      // as the next function pointer in the vector

  int a = 1, b = 0;
  output.push_back(L"Exception Block");
  a = a / b;   // Generate an Divide by Zero Exception
}

 // This is done to see that Try Catch block is in fact __try except block
void  TryCatchFn(int v) {   // Function to execute Try Catch Block
	try {
		if(v > -1 && v < (int)funs.size())
	      ((MyFunctionType)funs[v])(v-1);
	}
	catch(System::Exception ^) {
      output.push_back(L"Catch Block");
	}
}

void  _TryFinallyFn(int v) {   // Function to execute Try Finally
	__try {
		if(v > -1 && v < (int)funs.size())
	      ((MyFunctionType)funs[v])(v-1);
	}
	__finally {
      output.push_back(L"__finally Block");
	}
}

void  _TryExceptEHFn(int v) {   // Function to execute Try Except Returning
	__try {                     // EXCEPTION_EXECUTE_HANDLER
		if(v > -1 && v < (int)funs.size())
	      ((MyFunctionType)funs[v])(v-1);
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
      output.push_back(L"__except Block (EXCEPTION_EXECUTE_HANDLER)");
	}
}

void  _TryExceptCSFn(int v) {   // Function to execute Try Except Returning
	__try {                     // EXCEPTION_CONTINUE_SEARCH
		if(v > -1 && v < (int)funs.size())
	      ((MyFunctionType)funs[v])(v-1);
	}
	__except(EXCEPTION_CONTINUE_SEARCH) {
      output.push_back(L"__except Block (EXCEPTION_CONTINUE_SEARCH)");
	}
}

void  _TryExceptCEFn(int v) {   // Function to execute Try Except Returning
	__try {                     // EXCEPTION_CONTINUE_EXECUTION
		if(v > -1 && v < (int)funs.size())
	      ((MyFunctionType)funs[v])(v-1);
	}
	__except(EXCEPTION_CONTINUE_EXECUTION) {
      output.push_back(L"__except Block (EXCEPTION_CONTINUE_EXECUTION)");
	}
}

   // Filter for UnhandledExceptionFilter returning EXCEPTION_EXECUTE_HANDLER
LONG WINAPI ExHandleFilter (PEXCEPTION_POINTERS lpExceptionInfo) {
   output.push_back(L"UnhandledExceptionFilter Filter");
   MessageBox(NULL,L"UnhandledExceptionFilter  (EX HANDLER)",NULL,NULL);
   return EXCEPTION_EXECUTE_HANDLER;
}

// Function to set UnhandledExceptionFilter returning EXCEPTION_EXECUTE_HANDLER
void EEH_UnHdlExptflfn(int v) {

    SetUnhandledExceptionFilter(ExHandleFilter);

	if(v > -1 && v < (int)funs.size())
	   ((MyFunctionType)funs[v])(v-1);

    SetUnhandledExceptionFilter(NULL);
}

   // Filter for UnhandledExceptionFilter returning EXCEPTION_CONTINUE_SEARCH 
LONG WINAPI ConSrchHandleFilter (PEXCEPTION_POINTERS lpExceptionInfo) {
   output.push_back(L"UnhandledExceptionFilter Filter");
   MessageBox(NULL,L"UnhandledExceptionFilter  (CONTINUE SRCH)",NULL,NULL);
   return EXCEPTION_CONTINUE_SEARCH;
}

// Function to set UnhandledExceptionFilter returning EXCEPTION_CONTINUE_SEARCH
void ECS_UnHdlExptflfn(int v) {

    SetUnhandledExceptionFilter(ConSrchHandleFilter);

	if(v > -1 && v < (int)funs.size())
	   ((MyFunctionType)funs[v])(v-1);

    SetUnhandledExceptionFilter(NULL);
}

   // Filter for UnhandledExceptionFilter returning EXCEPTION_CONTINUE_EXECUTION 
LONG WINAPI ConExHandleFilter (PEXCEPTION_POINTERS lpExceptionInfo) {
   output.push_back(L"UnhandledExceptionFilter Filter");
   MessageBox(NULL,L"UnhandledExceptionFilter (CONTINUE EXEC)",NULL,NULL);
   // WARNING : This might end up in an infinite loop. In order to demonstrate the
   // same, have left it unhandled.
   return EXCEPTION_CONTINUE_EXECUTION;
}

// Function to set UnhandledExceptionFilter returning EXCEPTION_CONTINUE_EXECUTION
void ECE_UnHdlExptflfn(int v) {

    SetUnhandledExceptionFilter(ConExHandleFilter);

	if(v > -1 && v < (int)funs.size())
	   ((MyFunctionType)funs[v])(v-1);

    SetUnhandledExceptionFilter(NULL);
}

   // Filter for VectoredExceptionHandler returning EXCEPTION_CONTINUE_SEARCH 
LONG CALLBACK VectoredHandler1(PEXCEPTION_POINTERS lpExceptionInfo) {

//  It was difficult to determine the priority value inside the filter.
//  will have to create 0 to 5 ie 6 x 2 it 12 methods for each vectored handler.
//  As the 0 value indicates execution atlast rather than first for all non
//  zero values, parsing the vecpriority buffer backwards makes no sense too.

	output.push_back(L"Vectored Exception Handling");
    MessageBox(NULL,L"Vectored Exception Handling",NULL,NULL);
	return EXCEPTION_CONTINUE_SEARCH;
}

// Function to add VectoredExceptionHandler
void VectoredExceptionHandler(int v) {

	PVOID h1;
	ULONG firsthandler = 1;

	if(priority_idx > -1) {
	  firsthandler = vecpriority[priority_idx];
	  priority_idx--;
	}

	h1 = AddVectoredExceptionHandler(firsthandler,VectoredHandler1);
	vecexptrs.push_back(h1);

	if(v > -1 && v < (int)funs.size())
	   ((MyFunctionType)funs[v])(v-1);

	RemoveVectoredExceptionHandler(h1);
}

   // Filter for VectoredExceptionHandler returning EXCEPTION_CONTINUE_SEARCH 
LONG CALLBACK VectoredHandler2(PEXCEPTION_POINTERS lpExceptionInfo) {
  
	output.push_back(L"Vectored Continue Handling");
	MessageBox(NULL,L"Vectored Continue Handling",NULL,NULL);
    return EXCEPTION_CONTINUE_SEARCH;
}

// Function to add VectoredContinueHandler
void VectoredContinueHandler(int v) {

	PVOID h2;
	ULONG firsthandler = 1;

	if(priority_idx > -1) {
	  firsthandler = vecpriority[priority_idx];
	  priority_idx--;
	}

	h2 = AddVectoredContinueHandler(firsthandler, VectoredHandler2);
	veccnptrs.push_back(h2);

	if(v > -1 && v < (int)funs.size())
	   ((MyFunctionType)funs[v])(v-1);

	RemoveVectoredContinueHandler(h2);
}

// function adds function pointers to the array of func pointers
void AddFunction(int val, int priority) {

   MyFunctionType exceptionfn = ExceptionFn;
   MyFunctionType trycatchfn  = TryCatchFn;
   MyFunctionType _tryfinallyfn  = _TryFinallyFn;
   MyFunctionType _tryexceptehfn = _TryExceptEHFn;
   MyFunctionType _tryexceptcsfn = _TryExceptCSFn;
   MyFunctionType _tryexceptcefn = _TryExceptCEFn;
   MyFunctionType eeh_unhdlexptflfn = EEH_UnHdlExptflfn;
   MyFunctionType ecs_unhdlexptflfn = ECS_UnHdlExptflfn;
   MyFunctionType ece_unhdlexptflfn = ECE_UnHdlExptflfn;
   MyFunctionType vectoredexceptionhandler = VectoredExceptionHandler;
   MyFunctionType vectoredcontinuehandler = VectoredContinueHandler;

   switch(val) {

	 case 1:
               funs.push_back(exceptionfn);
		       break;
	 case 2:
               funs.push_back(trycatchfn);
		       break;
	 case 3:
               funs.push_back(_tryexceptehfn);
		       break;
	 case 4:
               funs.push_back(_tryexceptcsfn);
		       break;
	 case 5:
               funs.push_back(_tryexceptcefn);
		       break;
	 case 6:
               funs.push_back(_tryfinallyfn);
		       break;
	 case 7:
               funs.push_back(eeh_unhdlexptflfn);
		       break;
	 case 8:
               funs.push_back(ecs_unhdlexptflfn);
		       break;
	 case 9:
               funs.push_back(ece_unhdlexptflfn);
		       break;
	 case 10:
		       vecpriority.push_back(priority);
               funs.push_back(vectoredexceptionhandler);
		       break;
	 case 11:
		       vecpriority.push_back(priority);
               funs.push_back(vectoredcontinuehandler);
		       break;
	 default:
		       break;
  } 

   return;
 }

// Executes the functions stored in the array
int ExeFunction() {
	output.clear();	
	priority_idx = (int)vecpriority.size() - 1;
	int i = (int)funs.size() - 1;

    ((MyFunctionType)funs[i])(i-1);

   return (int)output.size();
}

// Get the text from the output list
wchar_t* getOutputText(int i) {

	wchar_t* optxt = NULL;

	if(i > -1 && i < (int)output.size()) {
       optxt = output[i];
	}
	
	return optxt;
}

// Removes all the previously added VectoredExceptionHandler and VectoredContinueHandlers
void removeVectorHandlers() {

 PVOID var;

   while(!vecexptrs.empty()) {
	   var = vecexptrs.back();
       RemoveVectoredExceptionHandler(var);
	   vecexptrs.pop_back();
   }

   while(!veccnptrs.empty()) {
	   var = veccnptrs.back();
       RemoveVectoredContinueHandler(var);
	   veccnptrs.pop_back();
   }
}

// flush all the buffers and remove all the set exception handlers/filters
void flushBuffer() {

    SetUnhandledExceptionFilter(NULL);
    removeVectorHandlers();
	funs.clear();	
	vecpriority.clear();
	output.clear();	
}

