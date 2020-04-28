// inject.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "inject.h"
#include < windows.h >
#include < winsock.h >
#include < stdlib.h >
#define BUFFERSIZE 6144      //  Define the buffer size for the Shared memory

// declare the structures for the parametersof the called functions
struct connStruct {
  __in  SOCKET s;
    int nameoffset;
  __in  int namelen;
};

struct sendStruct {
  __in  SOCKET s;
  __in  int bufoffet;   // we store the offset frm the start of MMF to the String than  the Ptr to
                        //  String. I couldn't use strings as had problems with the managed code.
  __in  int len;
  __in  int flags;
};

struct recvStruct {
  __in  SOCKET s;
  __out  int bufoffet;
  __in   int len;
  __in   int flags;
};

struct closStruct {
  __in  SOCKET s;
};

connStruct* connstruct;  // Ptr to the structures containing the parameters
sendStruct* sendstruct;
recvStruct* recvstruct;
closStruct* closstruct;
int* strt;              // Ptr to the start address of memory mapped file

// Function types for the called functions
typedef int (WINAPI Pconnect)(__in  SOCKET s, __in  const struct sockaddr *name,  __in  int namelen);
typedef int (WINAPI Psend)(__in  SOCKET s, __in  const char *buf, __in  int len, __in  int flags);
typedef int (WINAPI Precv)(__in   SOCKET s,  __out  char *buf,  __in   int len,  __in   int flags);
typedef int (WINAPI Pclosesocket)( __in  SOCKET s);

Pconnect* pconnect; // Ptr to the hooking functions of the respective method
Psend* psend;
Precv* precv;
Pclosesocket* pclosesocket;
HANDLE mmfMutex, mmfSemaphore; // mutexes and semaphores
HANDLE mapFile;  // Memory mapped file

// Hook function for Connect call
int WINAPI connectHook (__in  SOCKET s, __in  const struct sockaddr *name,  __in  int namelen) {

	// wait to get the semaphore and mutex
   HANDLE hld[2];
   hld[0] = mmfSemaphore;
   hld[1] = mmfMutex;
   DWORD result = WaitForMultipleObjects( 2, hld,   // handle to semaphore
                                       TRUE, INFINITE);           // zero-second time-out interval

   if(result == WAIT_OBJECT_0 && strt != NULL) {
     *strt = 1;  // set the start integer of mmf to 1 for connect call
	 // get all the parameter values in the correspondng structure
     connstruct = (connStruct*)(strt + sizeof(int));
    (*connstruct).s = s;

	if(namelen != 0) {

	  struct sockaddr *tname = (sockaddr*)(connstruct + sizeof(connStruct));
	  CopyMemory(tname, name, sizeof(sockaddr));
	  (*connstruct).nameoffset = sizeof(connStruct);
	}
	else
	  (*connstruct).nameoffset = -1;

    (*connstruct).namelen = namelen;

    if (! ReleaseMutex(mmfMutex)) {   // release mutex.
	   MessageBox(NULL,(LPCWSTR)L"Release MMFMutex error",NULL,NULL);
    }
   }

   return pconnect(s, name, namelen);
}

// Hook function for Send call
int WINAPI sendHook (__in  SOCKET s, __in  const char *buf, __in  int len, __in  int flags) {

   HANDLE hld[2];
   // NOTE : Semaphores are acquired and not released here inorder to confirm that
   // each call is listened and fetched by the Monitor appln which in turn releases the semaphore.
   hld[0] = mmfSemaphore;
   hld[1] = mmfMutex;
   DWORD result = WaitForMultipleObjects( 2, hld,   // handle to semaphore
                                       TRUE, INFINITE);           // zero-second time-out interval

   if(result == WAIT_OBJECT_0 && strt != NULL) {
     *strt = 2;     // set the start integer of mmf to 2 for send call
	 // get all the parameter values in the correspondng structure
     sendstruct = (sendStruct*)(strt + sizeof(int));
    (*sendstruct).s = s;

    // check if the string can fit into the memory mapped file, if not truncate it.
	if(strlen(buf) != 0) {
	  char *tbuf = (char*)(sendstruct + sizeof(sendStruct));
 	  size_t size = sizeof(char)*(strlen(buf)+1);
	  size_t bufsize = BUFFERSIZE - (sizeof(int) + sizeof(sendStruct) + 1);
	  
	  if(bufsize > size)
        strcpy_s(tbuf, size, buf);
	  else {
		  int count = (bufsize / sizeof(char)) - 1;
          strncpy_s(tbuf, size, buf, count);
	  }

	  (*sendstruct).bufoffet = sizeof(sendStruct);
	}
	else
	  (*sendstruct).bufoffet = -1;

    (*sendstruct).len = len;
    (*sendstruct).flags = flags;

     if (! ReleaseMutex(mmfMutex)) {   // release mutex.
	     MessageBox(NULL,(LPCWSTR)L"Release MMFMutex error",NULL,NULL);
     }
   }

  return psend(s, buf, len, flags);
}

// Hook function for Recv call
int WINAPI recvHook(__in   SOCKET s,  __out  char *buf,  __in   int len,  __in   int flags) {

   HANDLE hld[2];
   hld[0] = mmfSemaphore;
   hld[1] = mmfMutex;
   DWORD result = WaitForMultipleObjects( 2, hld,   // handle to semaphore
                                       TRUE, INFINITE);           // zero-second time-out interval

   if(result == WAIT_OBJECT_0 && strt != NULL) {
     *strt = 3;    // set the start integer of mmf to 3 for recv call
	 // get all the parameter values in the correspondng structure
     recvstruct = (recvStruct*)(strt + sizeof(int));
    (*recvstruct).s = s;

    // check if the string can fit into the memory mapped file, if not truncate it.
	if(strlen(buf) != 0) {
	  char *tbuf = (char*)(recvstruct + sizeof(recvStruct));
 	  size_t size = sizeof(char)*(strlen(buf)+1);
	  size_t bufsize = BUFFERSIZE - (sizeof(int) + sizeof(recvStruct) + 1);
	  
	  if(bufsize > size)
        strcpy_s(tbuf, size, buf);
	  else {
		  int count = (bufsize / sizeof(char)) - 1;
          strncpy_s(tbuf, size, buf, count);
	  }

	  (*recvstruct).bufoffet = sizeof(recvStruct);
	}
	else
	  (*recvstruct).bufoffet = -1;

    (*recvstruct).len = len;
    (*recvstruct).flags = flags;

	 if (! ReleaseMutex(mmfMutex)) {   // release mutex.
	     MessageBox(NULL,(LPCWSTR)L"Release MMFMutex error",NULL,NULL);
     }
   }
  return precv(s, buf, len, flags);
}

// Hook function for CloseSocket call
int WINAPI closesocketHook( __in  SOCKET s) {

   HANDLE hld[2];
   hld[0] = mmfSemaphore;
   hld[1] = mmfMutex;
   DWORD result = WaitForMultipleObjects( 2, hld,   // handle to semaphore
                                       TRUE, INFINITE);           // zero-second time-out interval

   if(result == WAIT_OBJECT_0 && strt != NULL) {
     *strt = 4;    // set the start integer of mmf to 4 for closesocket call
	 // get all the parameter values in the correspondng structure
     closstruct = (closStruct*)(strt + sizeof(int));
    (*closstruct).s = s;

    if (! ReleaseMutex(mmfMutex)) {   // release mutex.
	    MessageBox(NULL,(LPCWSTR)L"Release MMFMutex error",NULL,NULL);
    }
   }

   return pclosesocket(s);
}

// Function to trampoline the function calls
bool Trampoline(void *oldProc, void *newProc, void**ppOrigFn)
{
    bool bRet = false;
    DWORD    oldProtect = NULL;

    WORD* pJumpBack = (WORD*)oldProc;     // address to the 'mov edi edi' instruction
    BYTE* pLongJump = ((BYTE*)oldProc-5); // address to the top 'nop' no instruction bytes
    DWORD* pLongJumpAdr = ((DWORD*)oldProc-1); // same as above, pts to a 'nop'

	// Set protection allowing page of memory to execute and read while trampoline
    VirtualProtect(pLongJump, 7, PAGE_WRITECOPY, &oldProtect);

    // don’t hook functions which have already been hooked
    if ((0xff8b == *pJumpBack) && 
        (0x90 == *pLongJump) &&
        (0x90909090 == *pLongJumpAdr))
    {
        *pLongJump = 0xE9;    // long jmp
        *pLongJumpAdr = ((DWORD)newProc)-((DWORD)oldProc);    // offset to the address of new fn

		// IMPORTANT: We prefer to do a short jump first then the long jump as
		// two-byte short jump can be written atomically.
        *pJumpBack = 0xF9EB;        // short jump back -7 (back 5, plus two for this jump)

        if (ppOrigFn)
            *ppOrigFn = ((BYTE*)oldProc)+2;
        bRet = true;
    }
    VirtualProtect(pLongJump, 7, PAGE_EXECUTE, &oldProtect);
    return bRet;
}

// IMPORTANT: Untrampoline a function may result in access violation, as some Functions 
// are on the call stack of the thread are trampolined resulting in a blocking call
bool Untrampoline(void* oldProc)    // the original fn ptr, not "ppOrigFn" from HotPatch
{
    bool bRet = false;
    DWORD    oldProtect = NULL;

    WORD* pJumpBack = (WORD*)oldProc;
    BYTE* pLongJump = ((BYTE*)oldProc-5);
    DWORD* pLongJumpAdr = ((DWORD*)oldProc-1);

	VirtualProtect(pLongJump, 7, PAGE_WRITECOPY, &oldProtect);

	// We put back the values we replaced previously
    if (0xF9EB == *pJumpBack)
    {
		*pJumpBack = 0xff8b;        // mov edi, edi = nop

		if(0xE9 == *pLongJump) {
          *pLongJump = 0x90;   
          *pLongJumpAdr = 0x90909090; 
		}

		bRet = true;
    }

    VirtualProtect(pLongJump, 7, PAGE_EXECUTE, &oldProtect);
    return bRet;
}

// hooks the winsock functions and set up MMFile
BOOL WINAPI HookAllApps() {

    HINSTANCE hinstLib; 
	Pconnect* connectfn;
	Psend* sendfn;
	Precv* recvfn;
	Pclosesocket* closesocketfn;
    BOOL fRunTimeLinkSuccess = TRUE; 

	TCHAR MemoryName[]=TEXT("GlobalSharedMemory");

	// Open the Memory Mapped File if it already exists
	mapFile = OpenFileMapping( FILE_MAP_ALL_ACCESS,   // read/write access
                               FALSE,                 // do not inherit the name
                               MemoryName);               // name of mapping object 
 
    if(mapFile == NULL)  { 

	  mapFile=CreateFileMapping( INVALID_HANDLE_VALUE,    // use paging file
	  	                         NULL, PAGE_READWRITE, 0,
							     BUFFERSIZE, MemoryName);
	}

    if (mapFile == NULL)  { 
      return FALSE;
    } 
 
	// Map the Memory Map File to a View inorder to access the Memory
	strt = (int*) MapViewOfFile(mapFile, // handle to map object
                                  FILE_MAP_ALL_ACCESS,  // read/write permission
                                  0, 0, BUFFERSIZE);
 
    if (strt == NULL)  { 
      MessageBox(NULL,(LPCWSTR)L"Could not map view of file",NULL,NULL);
      CloseHandle(mapFile);
      return FALSE;
    }

	 mmfMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"MMFMutex"); // Open Mutex

	if (mmfMutex == NULL) 
    {
	  mmfMutex = CreateMutex(NULL, FALSE, L"MMFMutex"); // Create Mutex, calling thread being not the
                                              // owner of the mutex.
	}

	if (mmfMutex == NULL) 
    {
 	    MessageBox(NULL,(LPCWSTR)L"Create MMFMutex error",NULL,NULL);
        return FALSE;
    }

    mmfSemaphore = OpenSemaphore( 
		           SEMAPHORE_ALL_ACCESS,           // default security attributes
                   FALSE,  // maximum count
                   TEXT("MMFSemaphore"));          // named semaphore

    if (mmfSemaphore == NULL) 
    {
        mmfSemaphore = CreateSemaphore( 
           NULL,           // default security attributes
           1,  // initial count
           1,  // maximum count
           TEXT("MMFSemaphore"));          // named semaphore
	}

    if (mmfSemaphore == NULL) 
    {
 	    MessageBox(NULL,(LPCWSTR)L"Create MMFSemaphore error",NULL,NULL);
        return FALSE;
    }

	// Get a handle to the DLL module.
 
	hinstLib = GetModuleHandle(TEXT("Ws2_32"));
 
    // If the handle is valid, try to get the function address.
 
    if (hinstLib != NULL) 
    { 
  	    connectfn = (Pconnect*) GetProcAddress(hinstLib, "connect");
	    sendfn = (Psend*) GetProcAddress(hinstLib, "send"); 
	    recvfn = (Precv*) GetProcAddress(hinstLib, "recv"); 
	    closesocketfn = (Pclosesocket*) GetProcAddress(hinstLib, "closesocket"); 
 
        // If the function address is valid, hook the function.
 
        if (connectfn != NULL) {
			Trampoline(connectfn, (void*)connectHook, (void**) &pconnect);
		}
		else
			return FALSE;

        if (sendfn != NULL) {
			Trampoline(sendfn, (void*)sendHook, (void**) &psend);
		}
		else
			return FALSE;

        if (recvfn != NULL) {
			Trampoline(recvfn, (void*)recvHook, (void**) &precv);
		}
		else
			return FALSE;

        if (connectfn != NULL) {
			Trampoline(closesocketfn, (void*)closesocketHook, (void**) &pclosesocket);
		}
		else
			return FALSE;

    } 
	else {
      fRunTimeLinkSuccess = FALSE;
	}

	return fRunTimeLinkSuccess;
}

// unhooks the winsock functions and releases resources
BOOL WINAPI UnHookAllApps() {

   HINSTANCE hinstLib; 
   Pconnect* connectfn;
   Psend* sendfn;
   Precv* recvfn;
   Pclosesocket* closesocketfn;
   BOOL fRunTimeLinkSuccess = TRUE; 

   // Get a handle to the DLL module.
 
   hinstLib = GetModuleHandle(TEXT("Ws2_32"));
 
   // wait to acquire the mutex
   DWORD result = WaitForSingleObject( mmfMutex, INFINITE);           // zero-second time-out interval

   if(result == WAIT_OBJECT_0) {

      // If the handle is valid, try to get the function address.
     if (hinstLib != NULL)  { 
  	  connectfn = (Pconnect*) GetProcAddress(hinstLib, "connect");
	  sendfn = (Psend*) GetProcAddress(hinstLib, "send"); 
	  recvfn = (Precv*) GetProcAddress(hinstLib, "recv"); 
	  closesocketfn = (Pclosesocket*) GetProcAddress(hinstLib, "closesocket"); 
 
      // If the function address is valid, unhook the function.

      if (connectfn != NULL) {
		Untrampoline(connectfn);
	  }
	  else
		return FALSE;


      if (sendfn != NULL) {
		Untrampoline(sendfn);
	  }
	  else
		return FALSE;

      if (recvfn != NULL) {
		Untrampoline(recvfn);
	  }
	  else
		return FALSE;

      if (connectfn != NULL) {
		Untrampoline(closesocketfn);
	  }
	  else
		return FALSE;
     } 
     else {
       fRunTimeLinkSuccess = FALSE;
     }

	 // release all the resources
	 strt = NULL;
	 UnmapViewOfFile(strt);
     CloseHandle(mapFile);
   }

   if (! ReleaseMutex(mmfMutex)) {   // release mutex.
     MessageBox(NULL,(LPCWSTR)L"Release MMFMutex error",NULL,NULL);
   }
   // tried if this helps to finish the previous function calls successfully, didn't worked
   Sleep(100);
   CloseHandle(mmfSemaphore);
   CloseHandle(mmfMutex);

   return fRunTimeLinkSuccess;
}