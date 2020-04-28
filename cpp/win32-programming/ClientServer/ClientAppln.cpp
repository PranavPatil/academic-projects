// ClientAppln.cpp : Defines the entry point for the console application.
//

// ASSIGNMENT NO : 4
// PRANAV PATIL

#include < winsock2.h >
#include < ws2tcpip.h >
#include < stdlib.h >
#include < stdio.h >
#include < iostream >
#include "stdafx.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512   // default length of the buffer
#define DEFAULT_PORT "31337"  // default port number
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Validate the parameters
    if (argc != 2) {
        wcout << L"usage: " << argv[0] << L" server-name" << endl;
        return 1;
    }

	// Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        wcout << L"WSAStartup failed: " << gai_strerrorW(iResult) << endl;
        return 1;
    }
    
	// setup address info structure
	ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

	try {

    // get the server ip address to connect from the user
	char IPStr[64];
    int ret = 0;

    #if defined UNICODE
	wctomb_s(&ret, IPStr, (size_t)sizeof(IPStr), (wchar_t)argv[1]);
    #else
        strncpy(IPStr, argv[1], sizeof(IPStr));
    #endif
    IPStr[63] = _T('\0');

    // Resolve the server address and port
	iResult = getaddrinfo(IPStr, DEFAULT_PORT, &hints, &result);

    if ( iResult != 0 ) {
		wcout << L"getaddrinfo failed: " <<  gai_strerrorW(iResult) << endl;
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
 		    wcout << L"socket failed: " <<  gai_strerrorW(WSAGetLastError()) << endl;
            freeaddrinfo(result);
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Unable to connect to server!" << endl;
        WSACleanup();
        return 1;
    }

    char dData[DEFAULT_BUFLEN] = "";

	// execute until client terminates the server connection
	while(true)
    {
	   memset(dData, 0 , sizeof(dData)); // reset the buffer to avoid garbage values.
       cout << endl << "Client >";
       fgets(dData,sizeof(dData),stdin); //get the user message
       Sleep(5); 

	   // Send an bufferd message
       iResult = send( ConnectSocket, dData, sizeof(dData), 0 );
       if (iResult == SOCKET_ERROR) {
          wcout << L"send failed: " <<  gai_strerrorW(WSAGetLastError()) << endl;
          closesocket(ConnectSocket);
          WSACleanup();
          return 1;
       }

       cout << "Bytes Sent: " << iResult << endl;

	   memset(dData, 0 , sizeof(dData)); // reset the buffer to avoid garbage values.

	   // Receive echoed message from the server
        iResult = recv(ConnectSocket, dData, recvbuflen, 0);

		if ( iResult > 0 ) {
			cout << endl << "Received : " << dData << endl;

			// on receival of echoed 'close' message from the server exit the loop
			// IMPORTANT : here any message starting with 'close' is interpreted
			// as close command to the server. Problem is _stricmp() doesn't 
			// work for equality in below scenario.
			if(strstr(dData, "close"))
 	          break;
		}
        else if ( iResult == 0 )
            cout << "Connection closed" << endl;
        else
            wcout << L"recv failed: " <<  gai_strerrorW(WSAGetLastError()) << endl;
    }

	// shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        wcout << L"shutdown failed: " <<  gai_strerrorW(WSAGetLastError()) << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
	}
    catch(char * str ) { 
		cout << "Unexpected Exception : " << str ;
	}
    return 0;
}
