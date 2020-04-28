Threading is a very useful technique. Write a network server which listens on Port 31337
The server should handle multiple clients, creating a new thread for each client.
The server simply echoes back what you type (but waits for a newline).
A session is closed when the string �close� is typed to the server.
The console should provide a simple output which details the number of threads in use when asked.
Also, the console should remain responsive to a �shutdown� request (i.e. when I type �shutdown� in the console, the server should cleanly shut down.

Don�t use .NET threading for this, but the beginthreadex calls�
Don�t use .NET sockets, but the more native socket functions�
