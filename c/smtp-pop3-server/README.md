# Network Programming Project Milestone 3

## Introduction:

  SMTP Server: SMTP server receives emails or messages from the clients. SMTP is a text-based protocol in which the sender
  communicates with the server using commands such as  EHLO, HELO, MAIL FROM, RCPT TO, DATA, RSET, NOOP, QUIT, VRFY etc
  inorder to send messages. The smtp server in our case uses port 9990 inorder to connect to the client.

  POP3: The POP3 Server sends email messages as requested by the client after the authentication. The POP3 protocol is a
  message retriving protocol were the clients establish TCP connection with the server running the POP3 service on TCP port 9990
  and retrieves messages present on the server. The commands used in order to fetch the messages by the clients are USER, PASS for
  authentication, and LIST, RETR, NOOP, STAT, RSET etc.

  Implmentation is done using unix socket API and using the Sqlite3 database. Both the servers,
  namely the sserver i.e. smtp server and pserver i.e. pop3 server are concurrent using multi threaded environment
  and handle upto 10 client connections concurrently. We also use the option "SO_REUSEADDR" inorder to reuse the socket
  address even after the server shuts down.

  The basic structure of the server is to create a socket, bind to a port 9990 and start listening for incoming
  connections. Once a connection is accepted using the client socket, a client thread with the corresponding thread id is spawned
  inorder to handle the client. The thread id, socket id and the client address (for messaging purposes) is stored in form of
  a structure whose array is maintained in order to server 10 clients parallely. Further we have a server thread which
  continously listens for user command basically 'quit' inorder to shutdown the server gracefully, it was also planned before
  to expand with the set of server commands but faced time limitations.

  In both the sserver (smtp) and the pserver (pop3) server we initialize the database by creating tables, pairs and tuples
  respectively. We also added some default entries in the table of pairs representing the table of email ids and passwords.
  It should be noted that all the email ids on the server have the domain at np2010. Hence the email id becomes
  'emailid@np2010' which is used mostly for sending and receiving emails.  

  Setup emaildb.sqlite3 database by installing sqlite3 and creating database emaildb in
current directory.


## SMTP Server:

  Functions used to support the operations include,

  Trim(): trims all the white spaces in the string.

  toUppercase(): Converts the string to uppercase.

  toLowercase(): Converts the string to lowercase.

  substring():   Gets the substring from the string starting from specified starting point, and ending with end point.

  isemailvalid(): THis is a third party function in order to check whether the email is in valid format according to RFC 822 and is
                  used to check whether the mail from email address is valid or not.

  isaddpresent(): Function used to determine whether the corresponding email address on 'rcpt to' or 'vrfy' command is valid
  and present on the server, with the domain name at np2010. It uses the sqlite3 prepare statements to fetch the records from pairs
  and check for the presence of email ids.

  Storemail(): Store mail uses the data stored in thread stack array to load into the database in tuples table. The format of the
  thread stack array is as follows,

  thdstack[0] -> sender's address
  thdstack[1] -> receiver's address
  thdstack[2] -> Subject of email
  thdstack[3] -> Email Data

  This function is mainly called after quit command in order to load all the emails in the database or after the second mail from command
  just after entering data for the first email transaction. This is done inorder to store the emails before overwriting of the thdstack i.e.
  the buffer.

  SMTP SERVER COMMANDS: All the smtp commands are processed using the processcommands functions and with the help of statuscodes.

  HELO: Used to identify the SMTP client to the SMTP server. They are also used to ensure that the
  SMTP client and the SMTP server are in the initial state, and there is no transaction in progress with
  all state tables and buffers cleared. It indicates that the buffers are ready for the email transaction and
  the initial value of status code from 0 to 1.

  MAIL FROM: It is used to initiate the mail transaction in which the mail data is delivered to an SMTP server starting
  with the sender's email address. It is parsed and if valid with the status code being 1 indicating buffer/stack ready,
  then is stored in thdstack.

  RCPT FROM: It is used to identify an individual recipient of the mail data or multiple recipients using multiple use of the
  command.  It is parsed and if valid with the status code being 2 indicating that mail from address is already specified,
  it is stored in thdstack.

  DATA:  It causes the mail data to be appended to the mail data buffer with all the data being the 128 ASCII character codes.
  After the DATA command every text is considered to be as data until the ending chracter sequence "<CRLF>.<CRLF>" occurs.
  The data is taken only if the status code is greater than 3, i.e. 4 or 5. Initially the data command is used to set the status code
  to 4 if it determines receivers addresss is given using the status code equal to 3.

  RSET: It causes all the current mail transaction to be aborted and the mail data to be discarded, and all buffers (thdstack) and
  state tables (in our case status codes) cleared. The reset operation sets the status code back to 1 and resets all the thdstack to null.

  EHLO: It is used to specify the set of services available and provided by the server and also used to initiaze the mail transaction
  by setting the status code value to 1.

  VRFY: Determines whether the user or the email id of the mailbox is valid and exists on the server confirming the receiver. It is confirmed
  by using the previuosly described storemail function.

  NOOP: It implies no operation and has no affect on any parameters or previously entered commands.

  QUIT: It terminates the client server interaction and writes all the contents of the mail buffer in the mailbox, i.e insert the mail data
  into the database using the function Storemail().

  The indicators of status code:
  statuscode = 0 --> Server in initial state
  statuscode = 1 --> Server ready for a new mail transaction
  statuscode = 2 --> Server has got the sender's email address
  statuscode = 3 --> Server has got the receiver's email address
  statuscode = 4 --> Server has started accepting data from the client
  statuscode = 5 --> Server has got the subject while accepting the mail data


## POP3 SERVER:

 Supported functions include:

 Trim(), tolowercase(), substring() as described in Smtp server.

 isauthentic(): The function takes the userid and password from the processcommands function and checks whether there is a corresponding valid
 entry in the database, if so returns true. It is basically used for initial authentication of the client before connecting to the POP3 server.

 delmail(): It is used to delete an email from the server, with the corresponding email no in the tuples table using sequential order specified.
 It is mainly used in order to carry out the processing after dele command.

 getmailstat(): This function scans all the email messages in the tuples table, calculated the total size in terms of bytes, i.e. chracters as
 chracters correspond to 1 byte. It is responsible to carry the processing of stat command and retrive the corresponding info inorder to send it
 to the client.

 listmail(): Similar to the getmailstat function but it retrieves the count no and size info of all the email messages from the tuples table.
 Used to get the response for the 'LIST' command listing all the email messages.

 listamail(): Similar to the listmail function but it retrieves the count no and size info of the specified email message no from the tuples table
 according to the sequential order in the table. Used to get the response for the 'LIST' command listing all the email messages.

 retrmail(): Used to retrive the email message along with the date, sender and recevier email ids, message id, subject and actual message data along
 with attachments if any. It is mainly called in response to the clients 'RETR' command.

## POP3 COMMANDS:

  USER: It is the used in orderto initiate the client authentication process, where the client should provide a valid mailbox id which
  is verified from the server database. In our implemntation the verification process doesn't take place until the user specifies a valid
  password using the PASS command explained below.

  PASS: The POP3 server uses the argument pair from the USER and PASS commands to determine if the client should be given access to the
  appropriate maildrop. Such an authentication process is performed using the function isauthentic() finding the matching pair for emailid
  and password in the tuples table.

  RETR: The command retrieves the email message of the corresponding message number which is mostly multi-line response and is used in the Transaction
  state of the client-server interaction. The message is retrived using the retrmail() function described above.

  DELE: It is used to mark the specified message number from the mailbox as deleted, with the message number is considered to be the sequence number
  of the message in the tuples table.

  QUIT: The quit command is used to terminate the transaction state and to delete the messages marked as deleted in the delete command.

  STAT: It specifies the no of mail messages along with their total size. If no messages are present then no scan listing messages is returned.
  The scan listing consists of the message-number of the message, followed by a single space and the exact size of the message in octets. We use the
  function getmailstat() inorder to create the corresponding response for the client.

  NOOP: The POP3 server does nothing, but merely replies with a positive response.

  RSET: The messages that have been marked as deleted by the POP3 server are unmarked. It is not implemented in our pop3 server.
