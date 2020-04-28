
#include "stdafx.h"
#include < windows.h >
#include < tchar.h >
#include < tlhelp32.h >

using namespace System::Windows::Forms;

// Filer for exception handler
int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {

    MessageBox::Show(TEXT("Exception Code =  ") + code, TEXT("Message"), 
    MessageBoxButtons::OK, MessageBoxIcon::Information);

	 return EXCEPTION_CONTINUE_SEARCH;
} 

// Gets the process id from the process name
unsigned long GetTargetProcessIdFromProcname(wchar_t *procName)
{
   PROCESSENTRY32 pe;
   HANDLE thSnapshot;
   BOOL retval, ProcFound = false;

   // gets process snapshot
   thSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

   if(thSnapshot == INVALID_HANDLE_VALUE)
   {
	    MessageBox::Show(TEXT("Error: unable to create toolhelp snapshot"), TEXT("Loader"), 
			MessageBoxButtons::OK, MessageBoxIcon::Error);
      return false;
   }

   pe.dwSize = sizeof(PROCESSENTRY32);

    retval = Process32First(thSnapshot, &pe);

	// searches through the process name to get the matching process
   while(retval)
   {
	   if(wcsstr(pe.szExeFile, procName) )
      {
         ProcFound = true;
         break;
      }

      retval    = Process32Next(thSnapshot,&pe);
      pe.dwSize = sizeof(PROCESSENTRY32);
   }

   if(ProcFound)
     return pe.th32ProcessID;
   else
     return -1;
} 

// injects the dll in remote processes address space
BOOL InjectDLL(DWORD ProcessID)
{
   HANDLE Proc = NULL, hThread = NULL;
   PWSTR RemoteString = NULL;
   BOOL fOk = FALSE; // Assume that the function fails

   // gets the full path of the dll to be injected
   TCHAR DLL_NAME[MAX_PATH];
   GetModuleFileName(NULL, DLL_NAME, sizeof(DLL_NAME));
   TCHAR * temp = _tcsrchr(DLL_NAME, TEXT('\\')) + 1;
   int len = _tcslen(temp) * sizeof(TCHAR);
   _tcscpy_s(temp,  len, TEXT("inject.dll"));

   __try {

     __try {

		 // checks if process id is valid
     if(!ProcessID) {
		 MessageBox::Show(TEXT("Invalid Process ID: ") + ProcessID, TEXT("Error"), 
			 MessageBoxButtons::OK, MessageBoxIcon::Error);
        return false;
     }

	 //get the handle to remote process
	 Proc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
	                    PROCESS_VM_OPERATION | PROCESS_VM_READ |
						PROCESS_VM_WRITE,
	                    FALSE, ProcessID);

     if(Proc == NULL) {
	    MessageBox::Show(TEXT("Error in OpenProcess : ") + GetLastError(), TEXT("Error"), 
		                 MessageBoxButtons::OK, MessageBoxIcon::Error);
        __leave;
     }


     MessageBox::Show(TEXT("DLL Injection Running : ") + ProcessID, TEXT("Message"), 
 	                MessageBoxButtons::OK, MessageBoxIcon::Information);

     int cch = 1 + lstrlenW(DLL_NAME);
     int cb  = cch * sizeof(wchar_t);
   
	 // Allocated memory to store the DLL name in remote process address space.
     RemoteString = (PWSTR) VirtualAllocEx(Proc, NULL, cb, MEM_COMMIT, PAGE_READWRITE);

     if(RemoteString == NULL) {
	    MessageBox::Show(TEXT("Error in RemoteString : ") + GetLastError(), TEXT("Error"), 
		               MessageBoxButtons::OK, MessageBoxIcon::Error);
        __leave;
     }

	 // copy the dll name in the memory allocated above
     BOOL val = WriteProcessMemory(Proc, RemoteString, (PVOID)DLL_NAME,cb, NULL);

     if(!val) {
	   MessageBox::Show(TEXT("Error in WriteProcess : ") + GetLastError(), TEXT("Error"), 
	 	                MessageBoxButtons::OK, MessageBoxIcon::Error);
        __leave;
     }

	 // get address of loadlibrary function from kernel32.dll
     PTHREAD_START_ROUTINE LoadLibAddy = (PTHREAD_START_ROUTINE)
		                     GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");

     if(LoadLibAddy == NULL) {
	    MessageBox::Show(TEXT("Error in LoadLib : ") + GetLastError(), TEXT("Error"), 
	  	                       MessageBoxButtons::OK, MessageBoxIcon::Error);
        __leave;
     }

	 // fork a remote thread which launches the Loadlibrary function taking the dll
	 // name to be injected as the parameter thus injecting the dll.
	 hThread = CreateRemoteThread(Proc, NULL, 0, LoadLibAddy, RemoteString, 0, NULL);   

	 if(hThread == NULL) {
	    MessageBox::Show(TEXT("Error in CreateRemoteThread : ") + GetLastError(), TEXT("Error"), 
	 	                 MessageBoxButtons::OK, MessageBoxIcon::Error);
        __leave;
     }

     // wait for the dll to be injected
     WaitForSingleObject(hThread, INFINITE);
	 fOk = TRUE;
	 }
	 __except (filter(GetExceptionCode(), GetExceptionInformation())) {

	 }
   }
   __finally {

     // Free the remote memory that contained the DLL's pathname
      if (RemoteString != NULL) 
		  // free the remotely allocated memory
         VirtualFreeEx(Proc, RemoteString, 0, MEM_RELEASE);

      if (hThread  != NULL) 
         CloseHandle(hThread);

      if (Proc != NULL) 
         CloseHandle(Proc);
   }

   return fOk;
}

// ejects the dll from remote process address space
BOOL EjectDLL(DWORD ProcessID)
{
   HANDLE Proc = NULL, hThread = NULL;
   HANDLE hthSnapshot = NULL;
   BOOL fOk = FALSE; // Assume that the function fails

   // Get the full path name of the dll injected
   TCHAR DLL_NAME[MAX_PATH];
   GetModuleFileName(NULL, DLL_NAME, sizeof(DLL_NAME));
   TCHAR * temp = _tcsrchr(DLL_NAME, TEXT('\\')) + 1;
   int len = _tcslen(temp) * sizeof(TCHAR);
   _tcscpy_s(temp,  len, TEXT("inject.dll"));

   __try {

     __try {
          // Get a process snapshot
      hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID);
	  if (hthSnapshot == NULL) {
	    MessageBox::Show(TEXT("Error in Snapshot Tool : ") + GetLastError(), TEXT("Error"), 
		                 MessageBoxButtons::OK, MessageBoxIcon::Error);
        __leave;
	  }

      // Get the HMODULE of the desired library
      MODULEENTRY32W me = { sizeof(me) };
      BOOL fFound = FALSE;
      BOOL fMoreMods = Module32FirstW(hthSnapshot, &me);
      for (; fMoreMods; fMoreMods = Module32NextW(hthSnapshot, &me)) {
		  // check to see if the dll name matches with the one loaded by the process,
		  // if so get the module entry for the dll.
         fFound = (lstrcmpiW(me.szModule,  DLL_NAME) == 0) || 
                  (lstrcmpiW(me.szExePath, DLL_NAME) == 0);
         if (fFound) break;
      }
	  if (!fFound) {
	    MessageBox::Show(TEXT("Could not Find Module"), TEXT("Error"), 
		                 MessageBoxButtons::OK, MessageBoxIcon::Error);
        __leave;
	  }

		 // checks if process id is valid
	  if(!ProcessID) {
		 MessageBox::Show(TEXT("Invalid Process ID: ") + ProcessID, TEXT("Error"), 
			 MessageBoxButtons::OK, MessageBoxIcon::Error);
        return FALSE;
     }

	  // Open the process and get its handle
	 Proc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
	                    PROCESS_VM_OPERATION | PROCESS_VM_READ |
						PROCESS_VM_WRITE,
	                    FALSE, ProcessID);

     if(Proc == NULL) {
	    MessageBox::Show(TEXT("Error in OpenProcess : ") + GetLastError(), TEXT("Error"), 
		                 MessageBoxButtons::OK, MessageBoxIcon::Error);
        __leave;
     }

     MessageBox::Show(TEXT("DLL Ejection Running : ") + ProcessID, TEXT("Message"), 
                    MessageBoxButtons::OK, MessageBoxIcon::Information);

   // get the address of the FreeLibrary function in Kernel32.dll
     PTHREAD_START_ROUTINE FreeLibAdd = (PTHREAD_START_ROUTINE)
		                     GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "FreeLibrary");

     if(FreeLibAdd == NULL) {
	    MessageBox::Show(TEXT("Error in FreeLib : ") + GetLastError(), TEXT("Error"), 
	  	                       MessageBoxButtons::OK, MessageBoxIcon::Error);
        __leave;
     }

	 // fork a remotethread into another process inorder to unload the specified dll
	 hThread = CreateRemoteThread(Proc, NULL, 0, FreeLibAdd, me.modBaseAddr, 0, NULL);   

	 if(hThread == NULL) {
	    MessageBox::Show(TEXT("Error in CreateRemoteThread : ") + GetLastError(), TEXT("Error"), 
	 	                 MessageBoxButtons::OK, MessageBoxIcon::Error);
        __leave;
     }
       // wait for  the dll to unload
     WaitForSingleObject(hThread, INFINITE);
	 fOk = TRUE;
	 }
	 __except (filter(GetExceptionCode(), GetExceptionInformation())) {
         fOk = FALSE;
	 }
   }
   __finally {

      if (hThread  != NULL) 
         CloseHandle(hThread);

      if (Proc != NULL) 
         CloseHandle(Proc);
   }


   return fOk;
}