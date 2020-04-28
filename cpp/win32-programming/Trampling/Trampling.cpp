// Trampling.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include < iostream >
#include < windows.h >

using namespace std;
//typedef int (*Pfntest)(void);
typedef int (WINAPI Pconnect)(__in  SOCKET s, __in  const struct sockaddr *name,  __in  int namelen);

//------------
struct connStruct {
  __in  SOCKET s;
  const struct sockaddr *name;
  __in  int namelen;
}connstruct;

UINT WM_MYMSG = WM_USER + 100;
HWND monitor;
//COPYDATASTRUCT cpystruct;
//------------
//typedef int (__cdecl *MYPROC)(LPWSTR); 
Pconnect* pconn;

int WINAPI myhook(__in  SOCKET s, __in  const struct sockaddr *name,  __in  int namelen) {
  cout << "You are hacked !!!" << endl; 

  connstruct.s = s;
  connstruct.name = NULL;
  connstruct.namelen = namelen;

  //Initalize the data member of MyCDS 

  PostMessage(monitor,WM_MYMSG,NULL , (LPARAM)&connstruct);

  return pconn(s, name, namelen);
}

bool HotPatch(void *oldProc, void *newProc, void**ppOrigFn)
{
    bool bRet = false;
    DWORD    oldProtect = NULL;

    WORD* pJumpBack = (WORD*)oldProc;
    BYTE* pLongJump = ((BYTE*)oldProc-5);
    DWORD* pLongJumpAdr = ((DWORD*)oldProc-1);

    VirtualProtect(pLongJump, 7, PAGE_EXECUTE_WRITECOPY, &oldProtect);

    // don’t hook functions which have already been hooked
    if ((0xff8b == *pJumpBack) && 
        (0x90 == *pLongJump) &&
        (0x90909090 == *pLongJumpAdr))
    {
        *pLongJump = 0xE9;    // long jmp
        *pLongJumpAdr = ((DWORD)newProc)-((DWORD)oldProc);    // 
        *pJumpBack = 0xF9EB;        // short jump back -7 (back 5, plus two for this jump)

        if (ppOrigFn)
            *ppOrigFn = ((BYTE*)oldProc)+2;
        bRet = true;
    }
    VirtualProtect(pLongJump, 7, oldProtect, &oldProtect);
    return bRet;
}

bool HotUnpatch(void*oldProc)    // the original fn ptr, not "ppOrigFn" from HotPatch
{
    bool bRet = false;
    DWORD    oldProtect = NULL;

    WORD* pJumpBack = (WORD*)oldProc;

    VirtualProtect(pJumpBack, 2, PAGE_EXECUTE_WRITECOPY, &oldProtect);

    if (0xF9EB == *pJumpBack)
    {
        *pJumpBack = 0xff8b;        // mov edi, edi = nop
        bRet = true;
    }

    VirtualProtect(pJumpBack, 2, oldProtect, &oldProtect);
    return bRet;
}

int _tmain(int argc, _TCHAR* argv[])
{
    HINSTANCE hinstLib; 
    //Pfntest ProcAdd; 
	Pconnect* ProcAdd;
    BOOL fFreeResult, fRunTimeLinkSuccess = FALSE; 
 
    // Get a handle to the DLL module.
 
    //hinstLib = LoadLibrary(TEXT("D:\\Pranav Docs\\Projects\\test\\Debug\\test.dll")); 
		//monitor = ::FindWindow(NULL, L"Monitor");

	//if(monitor != NULL)
	hinstLib = LoadLibrary(TEXT("Ws2_32.dll")); 
	//else {
	//	cout << endl << "Good morning, still stuck on the gig ?";
	//	return -1;
	//}
 
    // If the handle is valid, try to get the function address.
 
    if (hinstLib != NULL) 
    { 
        //ProcAdd = (Pfntest) GetProcAddress(hinstLib, "fntest"); 
		ProcAdd = (Pconnect*) GetProcAddress(hinstLib, "connect"); 
 
        // If the function address is valid, call the function.
 
        if (NULL != ProcAdd) 
        {
            fRunTimeLinkSuccess = TRUE;
            //int val = (ProcAdd)(); 
			//cout << endl << "Got the value from the function : " << val << endl;

			HotPatch(ProcAdd, (void*)myhook, (void**) &pconn);

            //val = (ProcAdd)(); 
			//cout << endl << "Got the value from the function : " << val << endl;

            if ( (ProcAdd)( 0, NULL, 10) == NULL) {
              cout << "Unable to connect to server!" << endl;
              return 1;
            }
        }
        // Free the DLL module.
 
        fFreeResult = FreeLibrary(hinstLib); 
    } 

    // If unable to call the DLL function, use an alternative.
    if (! fRunTimeLinkSuccess) 
        cout << "Message printed from executable" << endl; 

	return 0;
}

