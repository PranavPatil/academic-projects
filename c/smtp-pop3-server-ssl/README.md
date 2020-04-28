
# Network Programming: Milestone 4

Aim: To implement openssl for the smtp and pop3 servers.

Solution:

Setup emaildb.sqlite3 database by installing sqlite3 and creating database emaildb in
current directory.

SMTP: sserver.c
Initially the server works normally using plaintext messages. But
the only valid commands are STARTTLS, NOOP, QUIT and EHLO, while the other
commands require the SSL connection to be setup as mentioned in RFC 2487 of
SMTP over TLS.
After the STARTTLS command is issued then it is parsed and context is initialzed.
loadCertificates function is called to verify certificates locations, certificates
themselves, and privatekeys. If there are any errors they are displayed and we abort.
(mostly these functions never worked even when the certificates were not present in the
right position without)
Then we create SSL_METHOD and then SSL_CTX is created using the method by calling the
function getSSLCTX(). Once we get the context we set in the structure SSLConn which
contains instances of SSL* and SSL_CTX*. We used it mainly to pass and return these parameters
from the processcommand function (which is used to parse and carry out the operations for the
respective commands. After returning to the calling clientthread() function which serves the
client connections, we check if the context is set and the ssl not, which symbolizes that the
context is ready and its time to initialze ssl. We first initialze the ssl using SSL_new using the
initialzed SSL context. Then we set the state of the client socket to SSL state before calling
connect(). Also we send "220 Ready to start TLS" before calling the connect function and complete the
SSL Handshake. Then we set the ssl state and get the peer certificates. Further as soon as the ssl
is initialzed we use SSL_read() and SSL_write() instead of send() and recv() used in plaintext messages.
Once quit commnad is issued we call SSL_shutdown() and SSL_free() to free the ssl connection.

Issues:
-> In order to test the Smtp server for kmail we need the message data to be parsed in a lump rather
than just line by line, seperating the Subject and Body of the message.
-> Both the smtp and pop3 server should respond with starting with 250 and the server name. Also there
need not be any hyphen at the last line which is mostly "250 DSN".

SMTP: altsserver.c (alternate copy)
Inspired from the examples on the website, http://www.rtfm.com/openssl-examples/ we created another copy
of smtp server. Here we used Bio_Sockets created fromthe client socket and use the ssl accept to carry out the
handshake. We checked with the wserver example which carried out the handshake perfectly and found two issues
causing problems. Firstly, as the sockets are set in NON BLOCKING mode the accept fucntion failed, Further
after using the socket for sending and receinving the data, it could not accept the SSL connection ie. complete
the SSL handshake. Here we used password_cb method to set the password of the private key file and also set the
DHFILEfile along with Random.pem to create random values to carry out SSL negoatiation (which we don't have much
idea).

POP3 Server:
POP3 server is implemented same as the SMTP server with similar methods inorder to get the context, verfiy the
certificates, keys and certificate locations. Then once the context is initialzed then the ssl is initialzed similar
to the smtp server. Hence most of the implementation for the smtp and pop3 for the SSL is the same.

Issues:
-> Inorder to test the pop3 server with kmail we need to implement the UIDL command.


## OpenSSL commands

Working commands

    openssl genrsa -des3 -out keys/ca.key 1024

    openssl req -config openssl.conf -new -x509 -days 1001 -key keys/ca.key -out certs/ca.cer


    openssl req -new -x509 -extensions v3_ca -keyout mykey.pem -out mycert.pem -days 365 -config ./openssl.cnf

    openssl verify mycert.pem

    openssl x509 -in mycert.pem -text -noout

    openssl s_server -accept 4433 -cert mycert.pem -key mykey.pem -www -state -debug

    openssl s_client -connect localhost:4433

    openssl s_client -connect smtp.gmail.com:587 -starttls smtp -showcerts
