// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "inject.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{

	BOOL val = FALSE;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
                              val = HookAllApps(); // Hook winsock fns when loaded

							  if(!val)
								  MessageBox(NULL,(LPCWSTR)L"Failure in Hooking !!",NULL,NULL);

							 break;
	case DLL_THREAD_ATTACH:
		                     break;
	case DLL_THREAD_DETACH:
		                     break;
	case DLL_PROCESS_DETACH:
                              val = UnHookAllApps();  // UnHook winsock fns when unloading

							  if(!val)
								  MessageBox(NULL,(LPCWSTR)L"Failure in UnHooking !!",NULL,NULL);
 	                         break;
	}
	return TRUE;
}

