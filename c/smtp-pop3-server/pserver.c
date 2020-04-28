
/***********************************************************************************************/

//        Network Programming: Milestone 3

//        Implementation of POP3 Server
//        compile as: gcc -o pserver pserver.c -lpthread -lsqlite3
//        run as    : ./pserver
//        client connect as: telnet localhost 9990
/***********************************************************************************************/

#include < stdio.h >
#include < errno.h >
#include < sys/socket.h >
#include < resolv.h >
#include < arpa/inet.h >
#include < errno.h >
#include < pthread.h >
#include < stdlib.h >
#include < unistd.h >
#include < string.h >
#include < fcntl.h >
#include < sqlite3.h >
#include < ctype.h >
#include < time.h >

#define MY_PORT		9990
#define MAXBUF		1024
#define MAX_CONNECTIONS  10

typedef struct {
  pthread_t tid;
  int clientfd;
  struct sockaddr_in client_addr;
} Connthread;

int initserver() {

    int retval = 0; // store return code
    sqlite3 *handle;  // handle for database connection

    // The number of queries to be handled,size of each query and pointer
    int q_cnt = 2,q_size = 150,ind = 0;
    char **queries = malloc(sizeof(char) * q_cnt * q_size);

    sqlite3_stmt *stmt;  // prepered statement for fetching tables

    // try to create the database. If it doesnt exist, it would be created
    // pass a pointer to the pointer to sqlite3, in short sqlite3**
    retval = sqlite3_open("emaildb.sqlite3",&handle);
    // If connection failed, handle returns NULL
    if(retval)
    {
        printf("Database connection failed\n");
        return -1;
    }
    printf("Connection successful\n");

    // Creating pair table
    char* ctable = "CREATE TABLE IF NOT EXISTS pairs (user TEXT PRIMARY KEY,pass TEXT NOT NULL,activated INTEGER)";

    // Execute the query for creating the table
    retval = sqlite3_exec(handle,ctable,0,0,0);

    // Insert first row and second row
    queries[ind++] = "INSERT INTO pairs VALUES('pranav','die',1)";
    retval = sqlite3_exec(handle,queries[ind-1],0,0,0);
    queries[ind++] = "INSERT INTO pairs VALUES('mehul','pulsar',0)";
    retval = sqlite3_exec(handle,queries[ind-1],0,0,0);

    ctable = "CREATE TABLE IF NOT EXISTS tuples (date TEXT,sender TEXT NOT NULL, receiver TEXT NOT NULL, \
                                            subject TEXT, body TEXT)";
    retval = sqlite3_exec(handle,ctable,0,0,0);  // Execute create table query

    // Close the handle to free memory
    sqlite3_close(handle);

    return retval;
}

char* trim(char *source)
{
  int i,j=0;
  char* dest = (char*) calloc(strlen(source), sizeof(char));
  memset(dest, 0, sizeof(char)*strlen(source));

  for(i=0;source[i]!='\0';i++)
  {
    if (source[i] != ' ' && source[i] != '\t')
      dest[j++]=source[i];
  }
  dest[j]='\0';
  return dest;
}

void touppercase(char *str) {

  int i = 0;
  while(i <= strlen(str)) {
      str[i] = toupper(str[i]);
      i++;
  }
}

void tolowercase(char *str) {

  int i = 0;
  while(i <= strlen(str)) {
      str[i] = tolower(str[i]);
      i++;
  }
}

char* substring(char* str, size_t begin, size_t len)
{
  if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
    return 0;

  return strndup(str + begin, len);
}

int isauthentic(sqlite3 *handle, char* userid, char* passwd) {

  int retval;
  int isauth = 0;
  sqlite3_stmt *stmt;
  char* query = NULL;
  query = "SELECT user,pass from pairs";

  retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
  if(retval == 1)
  {
     printf("Selecting data from DB Failed\n");
     return -1;
  }

  retval = sqlite3_step(stmt);
  //tolowercase(userid);
  char* val = NULL;
  char* pval = NULL;

  while(retval == SQLITE_ROW && isauth == 0){
     val = (char*)sqlite3_column_text(stmt,0);
     //tolowercase(val);

     if(val != NULL && strcmp(val, userid) == 0) {
       pval = (char*)sqlite3_column_text(stmt,1);

       if(pval != NULL && strcmp(pval, passwd) == 0)
         isauth = 1;
     }

     retval = sqlite3_step(stmt);  // fetch a row's status
  }

  if(retval != SQLITE_DONE && isauth == 0)  {
    printf("Some error encountered in Authentication\n");
    isauth = 0;
  }

  return isauth;
}

int delmail(sqlite3 *handle, int mailno) {
  int retval = 0;
  sqlite3_stmt *stmt;  // prepared statement for fetching tables
  char* query = NULL;
  mailno--;
  char* smailno = NULL;
  smailno = (char*) calloc(15, sizeof(char));
  sprintf(smailno, "%d", mailno);
  int total = strlen("select date from tuples limit 1 offset ") + strlen(smailno);
  query = (char*) calloc(total, sizeof(char));
  strcpy(query, "select date from tuples limit 1 offset ");
  strcat(query, smailno);

  retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
  if(retval == 1)
  {
    printf("Selecting data from DB Failed\n");
    return 1;
  }

  retval = sqlite3_step(stmt);
  char* data = NULL;
  free(smailno);
  free(query);

  if(retval == SQLITE_ROW) {
    data = (char*)sqlite3_column_text(stmt,0);

    if(data == NULL) {
      return 1;
    }
  }
  else
    return 1;

  total = strlen("delete from tuples where date='") + strlen(data) + 3;
  query = (char*) calloc(total, sizeof(char));
  strcpy(query, "delete from tuples where date='");
  strcat(query, data);
  strcat(query, "'");

  retval = sqlite3_exec(handle,query,0,0,0);
  if(retval == 1)  {
    printf("Deleting data from DB Failed\n");
    return 1;
  }

  return retval;
}

char* getmailstat(sqlite3 *handle) {

  int retval = 0;
  sqlite3_stmt *stmt;  // prepared statement for fetching tables
  char* query = NULL;

  query = "Select * from tuples";

  // select those rows from the table
  retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
  if(retval == 1)
  {
    printf("Selecting data from DB Failed\n");
    return NULL;
  }

  char* data = NULL;
  char* recvbuffer = NULL;
  int tsize = 0;
  int tcount = 0;

  recvbuffer = (char*) calloc(20, sizeof(char));
  strcpy(recvbuffer, "+OK ");

  int cols = sqlite3_column_count(stmt);

  // fetch a row's status
  retval = sqlite3_step(stmt);

  while(retval == SQLITE_ROW)
  {
    // SQLITE_ROW means fetched a row
   int col=0;
   tcount++;

   for(col=0 ; col<cols;col++)  {
     data = (char*)sqlite3_column_text(stmt, col);

     if(data != NULL)
       tsize = tsize + strlen(data);
   }
   retval = sqlite3_step(stmt);
  }

  if(retval != SQLITE_DONE) {
    printf("Some error encountered\n");
    return NULL;
  }

   char* str = NULL;
   str = (char*) calloc(10, sizeof(char));
   sprintf(str, "%d", tcount);
   strcat(recvbuffer, str);
   strcat(recvbuffer, " ");
   sprintf(str, "%d", tsize);
   strcat(recvbuffer, str);
   strcat(recvbuffer, "\n.\n");

  // Close the handle to free memory
  sqlite3_close(handle);

  return recvbuffer;
}

char* listmail(sqlite3 *handle) {

  int retval = 0;
  sqlite3_stmt *stmt;  // prepared statement for fetching tables
  char* query = NULL;

  query = "Select * from tuples";

  // select those rows from the table
  retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
  if(retval == 1)
  {
    printf("Selecting data from DB Failed\n");
    return NULL;
  }

  char* data = NULL;
  char* recvbuffer = NULL;
  char* sint = NULL;
  int tsize = 0, size = 0, counter = 0;

  recvbuffer = (char*) calloc (1024,sizeof(char));
  strcpy(recvbuffer, "\n");
  sint = (char*) calloc(10, sizeof(char));

  int cols = sqlite3_column_count(stmt);
  retval = sqlite3_step(stmt);   // fetch a row's status

  while(retval == SQLITE_ROW)  {
   int col=0;
   size = 0;

   for(col=0 ; col<cols;col++)  {
     data = (char*)sqlite3_column_text(stmt,col);

     if(data != NULL)
       size = size + strlen(data);
   }

   tsize = tsize + size;
   counter++;

   memset(sint, 0, sizeof(char)*10);
   sprintf(sint, "%d", counter);
   strcat(recvbuffer, sint);
   strcat(recvbuffer, " ");
   sprintf(sint, "%d", size);
   strcat(recvbuffer, sint);
   strcat(recvbuffer, "\n");

   retval = sqlite3_step(stmt);
  }

  if(retval != SQLITE_DONE) {
    printf("Some error encountered\n");
    return NULL;
  }

  char* tempbuffer = NULL;
  tempbuffer = (char*) calloc (1072,sizeof(char));
  strcpy(tempbuffer, "+OK ");
  memset(sint, 0, sizeof(char)*10);
  sprintf(sint, "%d", counter);
  strcat(tempbuffer, sint);
  strcat(tempbuffer, " messages (");
  sprintf(sint, "%d", tsize);
  strcat(tempbuffer, sint);
  strcat(tempbuffer, " bytes)");
  strcat(tempbuffer, recvbuffer);
  strcat(tempbuffer, ".\n");
  free(recvbuffer);
  recvbuffer = tempbuffer;
  sqlite3_close(handle);  // Close the handle to free memory

  return recvbuffer;
}

char* listamail(sqlite3 *handle, int mesg) {

  int retval = 0;
  sqlite3_stmt *stmt;  // prepared statement for fetching tables
  char* query = NULL;
  mesg--;

  int total = strlen("Select * from tuples limit 1 offset ");
  char* smarker = NULL;
  smarker = (char*) calloc(15, sizeof(char));
  sprintf(smarker, "%d", mesg);
  smarker[strlen(smarker) + 1] = '\0';
  total = total + strlen(smarker);
  query = (char*) calloc(total, sizeof(char));
  strcpy(query, "select * from tuples limit 1 offset ");
  strcat(query, smarker);

  retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
  if(retval == 1)
  {
    printf("Selecting data from DB Failed\n");
    return NULL;
  }

  char* data = NULL;
  char* recvbuffer = NULL;
  free(smarker);
  free(query);

  int size = 0;
  retval = sqlite3_step(stmt);   // fetch a row's status
  int cols = sqlite3_column_count(stmt);

  if(retval == SQLITE_ROW)  {
    int col=0;

    for(col=0 ; col<cols;col++)  {
      data = (char*)sqlite3_column_text(stmt,col);

      if(data != NULL)
        size = size + strlen(data);
    }
  }

  char* sint = NULL;
  sint = (char*) calloc(10, sizeof(char));

  recvbuffer = (char*) calloc (48,sizeof(char));
  strcpy(recvbuffer, "+OK ");
  memset(sint, 0, sizeof(char)*10);
  sprintf(sint, "%d", (mesg+1));
  strcat(recvbuffer, sint);
  strcat(recvbuffer, " ");
  sprintf(sint, "%d", size);
  strcat(recvbuffer, sint);
  strcat(recvbuffer, "\n.\n");
  sqlite3_close(handle);  // Close the handle to free memory

  return recvbuffer;
}

char* retrmail(sqlite3 *handle, int marker) {

  int retval = 0;
  sqlite3_stmt *stmt;  // prepared statement for fetching tables
  char* query = NULL;
  marker--;

  int total = strlen("Select * from tuples limit 1 offset ");
  char* smarker = NULL;
  smarker = (char*) calloc(15, sizeof(char));
  sprintf(smarker, "%d", marker);
//  itoa(marker, smarker, 10);
  smarker[strlen(smarker) + 1] = '\0';
  total = total + strlen(smarker);
  query = (char*) calloc(total, sizeof(char));
//select date from tuples limit 1 offset 0;
  strcpy(query, "select * from tuples limit 1 offset ");
  strcat(query, smarker);

  // select those rows from the table
  retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
  if(retval == 1)
  {
    printf("Selecting data from DB Failed\n");
    return NULL;
  }

  retval = sqlite3_step(stmt);
  char* date = NULL;
  char* send = NULL;
  char* receive = NULL;
  char* subject = NULL;
  char* data = NULL;
  char* recvbuffer = NULL;
  free(smarker);
  free(query);

  if(retval == SQLITE_ROW) {
    // sqlite3_column_text returns a const void* , typecast it to const char*
    date = (char*)sqlite3_column_text(stmt,0);
    send = (char*)sqlite3_column_text(stmt,1);
    receive = (char*)sqlite3_column_text(stmt,2);
    subject = (char*)sqlite3_column_text(stmt,3);
    data = (char*)sqlite3_column_text(stmt,4);

    if(date != NULL && send != NULL && receive != NULL
                    && subject != NULL && data != NULL)
    {
      total = strlen(date) + strlen(send) + strlen(receive)  + strlen(subject)  + strlen(data)  + 80;
      recvbuffer = (char*) calloc(total, sizeof(char));

      strcpy(recvbuffer, "\n+OK message follows\nX-POP3-Received: fbcf172\nDate: ");
      strncat(recvbuffer, date, (strlen(date)-1));
      strcat(recvbuffer, "\nFrom: ");
      strcat(recvbuffer, send);
      strcat(recvbuffer, "\nTo: ");
      strcat(recvbuffer, receive);
      strcat(recvbuffer, "\nSubject: ");
      strcat(recvbuffer, subject);
      strcat(recvbuffer, "\n");
      strcat(recvbuffer, data);
      strcat(recvbuffer, "\n.\n");
    }
    else
      return NULL;
  }
  else
    return NULL;

  return recvbuffer;
}

char* processcmds(char* buffer, int* isauth, sqlite3 *handle, char** userid) {

  int val = 0;
  char* pch = NULL;

  if(buffer != NULL && strlen(buffer) > 2)
    buffer = substring(buffer, 0, (strlen(buffer)-2));
  else
    return NULL;

/*------------ Start of Block for truncating the initial whitespaces ------------------ */
  int i = 0;

  while(buffer[i] == ' ' || buffer[i] == '\t')
    i++;

  char* source = buffer;
  buffer = substring(source, i, (strlen(source) - i));

/*------------ End of Block for truncating the initial whitespaces ------------------ */

  if((*isauth) == 0) {
    if(strncasecmp(buffer, "USER ", strlen("USER ")) == 0) {
      pch = buffer;
      pch = pch + strlen("USER ");
      pch = trim(pch);

      if(pch != NULL && strlen(pch) > 0) {
         userid[0] = pch;
         buffer = "+OK send PASS\n";
      }
      else
        buffer = "-ERR bad command \nread:errno=0\n";
    }
    else if(strncasecmp(buffer, "PASS ", strlen("PASS ")) == 0) {
     if(userid[0] != NULL) {
       pch = buffer;
       pch = pch + strlen("PASS ");

       if(pch != NULL && strlen(pch) > 0 && isauthentic(handle, userid[0], pch) == 1) {
          (*isauth) = 1;
          buffer = "+OK Welcome.\n";
       }
       else
          buffer = "-ERR permission denied\n";
     }
     else
       buffer = "+ERR send USER first\n";                  //####### check this
    }
    else
       buffer = "-ERR Command not accepted\n";
  }
  else if(strncasecmp(buffer, "LIST", strlen("LIST")) == 0) {

     pch = buffer;
     pch = pch + strlen("LIST");
     pch = trim(pch);

     if(pch != NULL && strlen(pch) == 0) {
        buffer = listmail(handle);
	if(buffer == NULL)
          buffer = "-ERR no messages\n";
     }
     else if(pch != NULL && strlen(pch) > 0) {
       int count = 0;
       count = atoi (pch);

       if(count > 0) {
         buffer = listamail(handle, count);
         if(buffer == NULL)
           buffer = "-ERR no such message\n";
       }
       else {
         buffer = "-ERR bad command \nread:errno=0\n";
       }
     }
     else
       buffer = "-ERR bad command \nread:errno=0\n";
  }
  else if(strncasecmp(buffer, "RETR ", strlen("RETR ")) == 0) {

     pch = buffer;
     pch = pch + strlen("RETR ");
     pch = trim(pch);

     if(pch != NULL && strlen(pch) > 0) {
       int count = 0;
       count = atoi (pch);

       if(count > 0) {
         buffer = retrmail(handle, count);
         if(buffer == NULL)
           buffer = "-ERR server error \nread:errno=0\n";
       }
       else {
         buffer = "-ERR bad command \nread:errno=0\n";
       }
     }
     else
         buffer = "-ERR bad command \nread:errno=0\n";
  }
  else if(strncasecmp(buffer, "DELE ", strlen("DELE ")) == 0) {
    pch = buffer;
    pch = pch + strlen("DELE ");
    pch = trim(pch);

    if(pch != NULL && strlen(pch) > 0) {
      int count = 0;
      count = atoi (pch);

      if(count > 0) {
	if(delmail(handle, count) == 0) {
	  char* out = (char*) calloc(32, sizeof(char));
	  strcpy(out, "+OK message ");
	  strcat(out, " deleted\n");
          buffer = out;
	}
	else
          buffer = "-ERR server error \nread:errno=0\n";
      }
      else {
        buffer = "-ERR bad command \nread:errno=0\n";
      }
    }
    else
      buffer = "-ERR bad command \nread:errno=0\n";
  }
  else if(strncasecmp(buffer, "STAT", strlen("STAT")) == 0) {

     pch = buffer;
     pch = pch + strlen("STAT");
     pch = trim(pch);

     if(pch != NULL && strlen(pch) == 0) {
        buffer = getmailstat(handle);
	if(buffer == NULL)
          buffer = "-ERR server error \nread:errno=0\n";
     }
     else
          buffer = "-ERR bad command \nread:errno=0\n";
  }
  else if(strncasecmp(buffer, "RSET", strlen("RSET")) == 0) {
    buffer = "+OK\n";
  }
  else if(strncasecmp(buffer, "NOOP", strlen("NOOP")) == 0) {
    buffer = "+OK\n";
  }
  else if(strncasecmp(buffer, "QUIT", strlen("QUIT")) == 0) {
    buffer = "+OK Farewell.\nread:errno=0\n";
  }
  else {
    buffer = "-ERR bad command \nread:errno=0\n";
  }

  return buffer;
}

void *clientthread(void *threadarg) {

  char buffer[MAXBUF];
  int clientfdes;
  struct sockaddr_in client_addrs;
  sqlite3 *handle;  // handle for database connection

  if(sqlite3_open("emaildb.sqlite3",&handle)) {
    printf("Database connection failed\n");
    return NULL;
  }

  Connthread *cthd;
  cthd = (Connthread *) threadarg;
  clientfdes = cthd->clientfd;
  client_addrs = cthd->client_addr;
  ssize_t buffsize = 0;
  char *output = NULL;
  char* userid = NULL;
  int isauth = 0;

  do {
    memset(buffer, 0, sizeof(buffer));
    buffsize = recv(clientfdes, buffer, MAXBUF-1, 0);
    output = processcmds(buffer, &isauth, handle, &userid);

    if(output != NULL)
      send(clientfdes, output, strlen(output), 0);

  }while(strncmp(buffer, "quit", strlen("quit")) != 0);

  close(clientfdes);  //---Close data connection---
  sqlite3_close(handle);  // Close the handle to free memory

  // pthread_exit(NULL);
  return NULL;
}

void *serverthread(void *closeset) {
  int* isclosed = NULL;
  isclosed = (int*) closeset;
  char status[25];

  while(fgets (status , 20 , stdin) != NULL && strncmp(status, "quit", strlen("quit")) != 0) {
    sleep(1);
  }

  printf("\nending session %s", status);
  *isclosed = 1;
  pthread_exit(NULL);
  return NULL;
}

int main(int count, char *strings[])
{
    int sockfd;
    int yes = 1;
    struct sockaddr_in servadr;

    if(initserver() != 0) {
       perror("Error in initializing POP3 Server");
       exit(errno);
    }

    /*---Create streaming socket---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
       perror("Socket");
       exit(errno);
    }

    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
       perror("setsockopt:REUSEADDR");
       exit(1);
    }

    /*---Initialize address/port structure---*/
    bzero(&servadr, sizeof(servadr));
    servadr.sin_family = AF_INET;
    servadr.sin_port = htons(MY_PORT);
    servadr.sin_addr.s_addr = INADDR_ANY;

    /*---Assign a port number to the socket---*/
    if ( bind(sockfd, (struct sockaddr*)&servadr, sizeof(servadr)) != 0 )
    {
      perror("socket--bind");
      exit(errno);
    }

    /*---Make it a "listening socket"---*/
    if ( listen(sockfd, 20) != 0 )
    {
      perror("socket--listen");
      exit(errno);
    }

    int oldfl;
    oldfl = fcntl(sockfd, F_GETFL);
    if (oldfl == -1) {
      perror("non blocking flag set error");
      exit(errno);
    }

    fcntl(sockfd, F_SETFL, oldfl | O_NONBLOCK);

    Connthread cthd[MAX_CONNECTIONS];
    int client_no = 0;
    int isclosed = 0;
    pthread_t serverthd;

    if ( pthread_create( &serverthd, NULL, serverthread, (void *) &isclosed) ) {
      printf("error creating serverthread.");
      abort();
    }

    /*---Forever... ---*/
    while (client_no <= MAX_CONNECTIONS && isclosed == 0)
    {
      int addrlen=sizeof(cthd[client_no].client_addr);

      /*---accept a connection (creating a data pipe)---*/
      cthd[client_no].clientfd = accept(sockfd, (struct sockaddr*)&cthd[client_no].client_addr, &addrlen);

      if(cthd[client_no].clientfd != -1) {
        printf("%s:%d connected\n", inet_ntoa(cthd[client_no].client_addr.sin_addr),
                                    ntohs(cthd[client_no].client_addr.sin_port));

        if ( pthread_create( &(cthd[client_no].tid), NULL, clientthread, (void *) &cthd[client_no]) ) {
           printf("error creating clientthread.");
           abort();
        }
      }
    }

    /*---Clean up */
    close(sockfd);

    return 0;
}
