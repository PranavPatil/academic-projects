#include < winsock.h >

// structures to store the parameters for the function calls
struct connStruct {
  __in  SOCKET s;
    int nameoffset;
  __in  int namelen;
};

struct sendStruct {
  __in  SOCKET s;
  __in  int bufoffet;
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
int* strt;               // Ptr to the start address of memory mapped file

// Returns the process id of the specified process
unsigned long GetTargetProcessIdFromProcname(wchar_t *);
BOOL InjectDLL(DWORD);   // injects the dll in the process
BOOL EjectDLL(DWORD);    // ejects the dll in the process

