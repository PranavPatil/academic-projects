// ServerAppln.cpp : Defines the entry point for the console application.
//

// ASSIGNMENT NO : 4
// PRANAV PATIL

#include < winsock2.h >
#include < ws2tcpip.h >
#include < stdlib.h >
#include < iostream >
#include "stdafx.h"
#include < process.h >

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512    // default length of the buffer
#define DEFAULT_PORT "31337"  // default port number
using namespace std;

/*char* getErrorInfo(int errno) {
	char errorStr[64];
    #if defined UNICODE
        wcstombs(errorStr, gai_strerror(errno), sizeof(errorStr));
    #else
        strncpy(errorStr, gai_strerror(errno), sizeof(errorStr));
    #endif
        errorStr[63] = _T('\0');
		return errorStr;
}*/

// used to determine wether the threads should continue execution.
bool status = true;
// used to count the no of threads executing currently.
volatile long threadNo = 0;

// thread function for each individual connection
unsigned __stdcall clientThread(void *ArgList) {

	// initialize all the buffers and sockets
	SOCKET Client = *((SOCKET*)ArgList);
    char recvbuf[DEFAULT_BUFLEN];
	int iResult, iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;
	int id = 0;

	// to increment the no of threads variable atomically
	InterlockedExchangeAdd(&threadNo, 1);
	id = threadNo;

	// Receive until the peer shuts down the connection
     while (status){

		memset(recvbuf, 0 , sizeof(recvbuf)); // reset the buffer to avoid garbage values.
		// receive message from client
cout << endl << "HELLO BROTHER !!!!!!!!!!!";
		iResult = recv(Client, recvbuf, recvbuflen, 0);

		if (iResult > 0) {
			cout << endl << "Received Client" << id << " : " << recvbuf << endl;
        }
        else  {
			wcout << L"Client" << id << L" : recv failed: " << id << " : " <<  gai_strerrorW(WSAGetLastError()) << endl;
            closesocket(Client);
            WSACleanup();
            break;
        }

		// close connection when close connection from the client.
		if(strstr(recvbuf, "close"))
        { 
		  cout << endl << "Client" << id << " : Connection Terminated" << endl;
          Sleep(10);

          // Echo the buffer back to the sender
          iSendResult = send( Client, recvbuf, sizeof(recvbuf), 0 );
          if (iSendResult == SOCKET_ERROR) {
              wcout << L"Client" << id << L"send failed: " <<  gai_strerrorW(WSAGetLastError()) << endl;
              closesocket(Client);
              WSACleanup();
              break;
          }

          // close client socket and end thread
	      closesocket(Client);
		  break;
	    }                                
        else    
        {
		  // echo back the same message to the client.
          Sleep(10);
		  send(Client,recvbuf,sizeof(recvbuf),0);
        }
		cout << endl << "Server >";
	}
	// to decrement the no of threads variable atomically
	InterlockedExchangeAdd(&threadNo, -1);
	cout << endl << "Server >";
	return 0;
}

// function to create the client connection and fork corresponding thread
unsigned __stdcall connectClient(void *arg) {

   // initialize buffers and sockets
   SOCKET ClientSocket = INVALID_SOCKET;
   SOCKET ListenSocket = *((SOCKET*)arg);
   int iResult;

   // execute until server shutdown.
    while(status)
    { 
	  // wait for accepting the connection from the clients.
      ClientSocket = accept(ListenSocket, NULL, NULL);
      if (ClientSocket == INVALID_SOCKET) {
		wcout << L"accept failed: " <<  gai_strerrorW(WSAGetLastError()) << endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
      }
      cout << endl << "Client" << (threadNo+1) << " connected" << endl;
  
      // fork the client connection thread
      HANDLE hThread;
      unsigned threadID;
      hThread = (HANDLE)_beginthreadex(NULL, 0, &clientThread, &ClientSocket, 0, &threadID);
	  Sleep(100);
      cout << endl << "Server >";
    }

	// shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        wcout << L"shutdown failed: " <<  gai_strerrorW(WSAGetLastError()) << endl;
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
    closesocket(ClientSocket);
    cout << endl << "Server >";
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    WSADATA wsaData;
    SOCKET ListenSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
                    hints;
    int iResult;    

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
		cout<<"WSAStartup failed: " << iResult << endl;
        return 1;
    }

	// setup address info structure
	ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

	// get the ip address of the current host
	char strHost[50];
    memset(strHost, 0 , 50);
    gethostname(strHost, 50);

    // Resolve the server address and port
	iResult = getaddrinfo(strHost, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
		wcout << L"getaddrinfo failed: " <<  gai_strerrorW(iResult) << endl;
        WSACleanup();
        return 1;
    }

	// get the ip address on which server is running.
    in_addr SocketAddr;
    SocketAddr.S_un = ((struct sockaddr_in *)(result->ai_addr))->sin_addr.S_un;

	cout<< "Server IP : " << inet_ntoa(SocketAddr) << endl;

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
		wcout << L"socket failed: " <<  gai_strerrorW(WSAGetLastError()) << endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
		wcout << L"bind failed: " <<  gai_strerrorW(WSAGetLastError()) << endl;
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    // set the socket to listen the incoming connections
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
		wcout << L"listen failed: " <<  gai_strerrorW(WSAGetLastError()) << endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

	// fork the thread to accept connections from the clients.
	HANDLE connThread;
    unsigned thdID;
    connThread = (HANDLE)_beginthreadex(NULL, 0, &connectClient, &ListenSocket, 0, &thdID);

	char buff[10];
	int value = -1;

	while(status) {
	  try {
		cout << endl << "Server >";
		cin >> buff;
		cout << endl;

		if(_stricmp(buff, "threads") == 0)
			cout << endl << "No of client connection threads currently : " << threadNo << endl;
		else if(_stricmp(buff, "shutdown") == 0) {
			status = false;

			// IMPORTANT
			// actually here as accept() and recv() functions in the respective threads
			// are blocking functions which cause thread to hang on, have to terminate thread
			// after certain interval.
			WaitForMultipleObjects(1,&connThread,TRUE,1000);
		}
		else
			cout << endl << "type 'shutdown' to shut the server." << endl
			     << "     'threads' to get the no of threads executing." << endl;
		Sleep(100);
	  } 
	  catch(exception ex) {
        cout << endl << "Unexpected Error." << endl;
	  }
	}

    cout << endl << "Shutting down the server.. " << endl;

	// cleanup everything
    closesocket(ListenSocket);
    WSACleanup();

    return 0;
}