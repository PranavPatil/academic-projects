Program should be able to intercept the following Winsock system calls:
connect
send
recv
closesocket
It should be able to launch an arbitrary monitored process (specified on the Command line)
It should be able to display the calls to connect, send, recv and closesocket along with their parameters
I strongly suggest you read Ch 22 of Windows System Programming.

Here Monitor folder has the application and inject folder has code for
inject.dll which Monitor will inject into the remote process to monitor its function
calls as above.
