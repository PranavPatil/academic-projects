

# Network Programming Project Milestone 2


## Introduction:

  SMTP: SMTP is a text-based protocol in which the sender communicates with the
  server using commands inorder to send messages. We use the port number
  22.

  POP3: The POP3 protocol is a meesage retriving protocol in which the client host
  establishes a TCP connection with the server running the POP3 service on TCP port 110
  and retrieves the messages on the server.

  Implmentation is done using unix socket API.  

  In the sendmail function The composemail
  method dumps the entire email in the result buffer and , it is
  used by the sendmail to send the emails.

  Command used to create a proxy for the pop server is as follows:


## Functions implemented:

  int compose-mail(unsigned char* result, int*result_size, char* to, char* from,
                   char* charset, char* subject, unsigned char*body, int attachments_nb,
                   unsigned char**attachments, int* attachments_sizes, char**attachnames)

  In the compose-mail we pass a preallocated result char* of 10000 characters in order to
  store the email. Now if we would have allocated the memory in the compose-mail then the
  previous address of the result pointer will no longer be valid and hence we can't access
  the result int main. Further, we could have passed address of char* ie the result but that
  would require further change in the prototype of the given function which is avoided.
  Later we pass the file parameters of attachments in the base 64 encoding which is embeded
  and is a thrid party program. I would have been ideal to create a library by using gcc options
  but not implemented. We also felt to specify an additional parameter to the
  compose mail which is the file name in order to determine the file name while downloading
  the file in parsemail and detect appropriate content type. We felt such a parameter would
  add better functionality for composing the email hence we added that one out.
  Also here we pass the array of attachment containing character data which we presumed would
  be already base 64 encoded, hence base 64 encoding occurs in the main or the calling function.
  We need to specify the subject such that it ends with a full stop or just a dot which is
  useful in scanning the subjects in pop3 implementation.


  int sendmail(unsigned char* buffer, int* size, char* smtp_server, int port, char* password, char* login, char*to, char*from);

  The sendmail function just carries out connection with the server, now the connection is unsecure and without using
  login and password parameters in the function which would result in failure while connecting to
  a secure smtp server. Then we use the snprintf to fromat the message before sending it. Now here we used default
  protocol 0 option while sending the message which could be changed to TCP for further improvement.


  int pollmail(char* smtp_server, int port, char* password, char* login, unsigned char* subject_signature, unsigned char* mail, int* mail_size)

  In the implementation of the pollmail we first needed to establish a connection with the pop server.
  Now the command given on the website is
  cat /tmp/pippe | sock -l :11000 | (openssl s_client -crlf -connect pop.gmail.com:110 -starttls pop3) 2>&1 >/tmp/pippe

  After facing couple of failures, mainly all University's emails located in gmail/exchange server rather than mailhost server
  and the spacing between -l and ':' in the sock command we came up with the foll command to connect the gmail pop server.

  cat /tmp/pippe | sock -l :11000 | (openssl s_client -crlf -ssl3 -connect pop.gmail.com:995 ) 2>&1 > /tmp/pippe


  We established the connection with the pop server and then used the STAT command to get the total
  number of emails present in the mailbox. Using that we call the function getemailid which parses
  the subjects of all the email header retrieved using "TOP" command and retrieves the one with the
  corresponding subject signature. The logic here is find the string "Subject: " and then get the
  substring further until there is a ".", even if the substring contains additional header attached
  we compare the first part with the subject signature to retrieve the email id. Then using the
  "LIST mailno" command we were supposed to get the total size of the email inorder to allocate
  buffers for its storage. But the connection seems to have been broken and the socket command
  proxy server exits, regardless of connecting again or using alternative socket to for the LIST command.
  Hence we have a running implementation until fetching the email id of the corresponding email.
  Further, after allocating the buffer for the email, a "RETR mailid" would fetch the email from the server
  and store it in the mail buffer.


  int parsemail(unsigned char* mail, int* mail_size, char* body, int buf_size, int attachments_nb, unsigned char* attachments, int* attachment_sizes)

  In the parsemail which is incomplete we planned to parse the email using the boundry of the content type,
  by checking for the string "boundary=" and retrieve the boundry id for the multipart/mixed content type.
  Then further after attaching the "--" boundry head to the boundary id we would parse the email to find the corresponding
  message boundaries. The text between two corresponding boundary ids could be fetched into the attachments buffer.
  Finally in the mail or calling function, the attachments could be scanned to detect the contentype at its start,
  parse the content type to get the filename and file type and use the base 64 decoding methods to decode the ASCII
  values to binaries and save them on the local machine.

  We hope that we could complete the library for pop and refine the libraries for easy access in the next milestone.
