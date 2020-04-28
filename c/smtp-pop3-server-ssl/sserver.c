
/***********************************************************************************************/

//        Network Programming: Milestone 4

//        compile as: gcc -o pserver pserver.c -lpthread -lsqlite3 -lssl
//        run as    : ./pserver
//        client connect as: telnet localhost 25
/***********************************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sqlite3.h>
#include <ctype.h>
#include <time.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "openssl/bio.h"
#include <openssl/crypto.h>

#define MY_PORT		25
#define MAXBUF		1024
#define MAX_CONNECTIONS  10

typedef struct {
  pthread_t tid;
  int clientfd;
  struct sockaddr_in client_addr;
} Connthread;

typedef struct {
  SSL_CTX *ctx;
  SSL *ssl;
} SSLConn;

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
    char* ctable = "CREATE TABLE IF NOT EXISTS pairs (user TEXT PRIMARY KEY,pass TEXT NOT NULL)";

    // Execute the query for creating the table
    retval = sqlite3_exec(handle,ctable,0,0,0);

    // Insert first row and second row
    queries[ind++] = "INSERT INTO pairs VALUES('pranav','die')";
    retval = sqlite3_exec(handle,queries[ind-1],0,0,0);
    queries[ind++] = "INSERT INTO pairs VALUES('mehul','pulsar')";
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

int isemailvalid(const char *address) {
  int        count = 0;
  const char *c, *domain;
  static char *rfc822_specials = "()<>@,;:\\\"[]";

  /* first we validate the name portion (name@domain) */
  for (c = address;  *c;  c++) {
    if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) ==
        '\"')) {
      while (*++c) {
        if (*c == '\"') break;
        if (*c == '\\' && (*++c == ' ')) continue;
        if (*c <= ' ' || *c >= 127) return 0;
      }
      if (!*c++) return 0;
      if (*c == '@') break;
      if (*c != '.') return 0;
      continue;
    }
    if (*c == '@') break;
    if (*c <= ' ' || *c >= 127) return 0;
    if (strchr(rfc822_specials, *c)) return 0;
  }
  if (c == address || *(c - 1) == '.') return 0;

  /* next we validate the domain portion (name@domain) */
  if (!*(domain = ++c)) return 0;
  do {
    if (*c == '.') {
      if (c == domain || *(c - 1) == '.') return 0;
      count++;
    }
    if (*c <= ' ' || *c >= 127) return 0;
    if (strchr(rfc822_specials, *c)) return 0;
  } while (*++c);

  return (count >= 1);
}

int isaddpresent(sqlite3 *handle, char* address) {

    int retval;
    int ispresent = 0;

    int q_size = 150,ind = 0;
    char *query = malloc(sizeof(char) * q_size);

    sqlite3_stmt *stmt;

    // select those rows from the table
    query = "SELECT user from pairs";
    retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
    if(retval == 1)
    {
        printf("Selecting data from DB Failed\n");
        return -1;
    }

    retval = sqlite3_step(stmt);
    char *val;
    int len = strlen(address) - strlen("@np2010");
    printf("\n\n len = %d \n", len);
    char* add = NULL;

    if(len > 0) {
      add = substring(address, 0, len-1);
      printf("\n\n add = %s \n", add);

      if(add == NULL)
	return 0;
    }
    else
      return 0;

    tolowercase(add);

    while(retval == SQLITE_ROW && ispresent == 0){
       // sqlite3_column_text returns a const void* , typecast it to const char*
      val = (char*)sqlite3_column_text(stmt,0);
        printf("%s = _%s_\t",sqlite3_column_name(stmt,0),val);
      printf("\n");

      tolowercase(val);

      printf("Val1 = _%s_\t",val);
      printf("Add1 = _%s_\t",add);

      if(strcmp(val, add) == 0)
	ispresent = 1;

      retval = sqlite3_step(stmt);  // fetch a row's status
    }

    if(retval != SQLITE_DONE && ispresent == 0)  {
      printf("Some error encountered in VRFY\n");
      ispresent = 0;
    }

  return ispresent;
}

int storemail(sqlite3 *handle, char** thdstack) {

  int retval = 0;
  sqlite3_stmt *stmt;  // prepared statement for fetching tables
  char* query = NULL;

  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  char* stime = asctime (timeinfo);

  int total = 42 + strlen(stime) + strlen(thdstack[0]) + strlen(thdstack[1])
                 + strlen(thdstack[2]) + strlen(thdstack[3]);
  query = (char*) calloc(total, sizeof(char));

  strcpy(query, "INSERT INTO tuples VALUES('");
  strcat(query, stime);
  strcat(query, "','");
  strcat(query, thdstack[0]);
  strcat(query, "','");
  strcat(query, thdstack[1]);
  strcat(query, "','");
  strcat(query, thdstack[2]);
  strcat(query, "','");
  strcat(query, thdstack[3]);
  strcat(query, "')");
  //query[strlen(query)+1] = '\0';
  retval = sqlite3_exec(handle,query,0,0,0);

  return retval;
}

// Creates a Context
SSL_CTX* getSSLCTX(void)
{
  SSL_METHOD *method;
  SSL_CTX *ctx;

//    OpenSSL_add_all_algorithms();		/* load & register all cryptos, etc. */
  SSL_load_error_strings(); /* optional load the error strings for good error reporting */
  ERR_load_BIO_strings();
  SSL_library_init(); /* load encryption & hash algorithms for SSL */

  method = SSLv2_server_method();		/* create new server-method instance */
  ctx = SSL_CTX_new(method);			/* create new context from method */
  if ( ctx == NULL )
  {
    ERR_print_errors_fp(stderr);
    printf("Unable to create Context\n");
    abort();
  }

  return ctx;
}

/*---------------------------------------------------------------------*/
/*--- LoadCertificates - load from files.                           ---*/
/*---------------------------------------------------------------------*/
void loadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
  /* Load trusted CA in PEM/DER file CA_CERT. */
    if (!SSL_CTX_load_verify_locations(ctx,CertFile,NULL)) {
	ERR_print_errors_fp(stderr);
		exit(1);
    }

	/* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
   if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}

char* processcmds(char* buffer, char** thdstack, int* statuscode, sqlite3 *handle, SSLConn *sslcon) {

  int val = 0;
  char* pch = NULL;
  char* lpch = NULL;
  char* hostname = "np2010";

  if(buffer != NULL && strlen(buffer) > 2)
    buffer = substring(buffer, 0, (strlen(buffer)-2));
  else
    return NULL;

  if((*statuscode) < 4) {
    int i = 0;

    while(buffer[i] == ' ' || buffer[i] == '\t')
      i++;

    char* source = buffer;
    buffer = substring(source, i, (strlen(source) - i));
  }

  char* tch = NULL;

  if((*statuscode) > 3 && sslcon->ssl != NULL) {
       // when eq . then foll and statuscode = 1
       if(strcmp(buffer, ".") == 0) {
	 if(thdstack[2] == NULL)
	   thdstack[2] = "(No Subject)";
	 if(thdstack[3] == NULL)
	   thdstack[3] = " ";

	 (*statuscode) = 1;
         buffer = "250 2.0.0 Ok: queued as 9001B5E831F\n";
       }
       else if((*statuscode) == 4 && strncasecmp(buffer, "SUBJECT:", strlen("SUBJECT:")) == 0) {
	 pch = buffer;
         pch = pch + strlen("SUBJECT:");
         thdstack[2] = pch;
	 thdstack[2][strlen(pch)+1] = '\0';
	 (*statuscode) = 5;
         buffer = NULL;
       }
       else if((*statuscode) == 4 && (tch=strrchr(buffer,'.'))!= NULL && tch-buffer+1 == strlen(buffer)) {
	 pch = buffer;
         char* sch = NULL;
         sch = strstr (pch,"Subject:");

         if(sch == NULL)
           sch = strstr (pch,"SUBJECT:");

         if(sch == NULL)
           sch = strstr (pch,"subject:");

         if(sch != NULL) {
          sch = sch + strlen("Subject:");
          pch = strchr(sch,'\n');
          sch = substring(sch, 0, (strlen(sch)-strlen(pch)));
          thdstack[2] = sch;
          printf("\n\n---------------------------------\n");
          printf("_ %s _", thdstack[2]);
          printf("\n---------------------------------\n");
          pch = pch +1;
          sch = substring(pch, 0, strlen(pch)-1);
          thdstack[3] = sch;
  	  thdstack[3][strlen(sch)+1] = '\0';
          printf("\n\n---------------------------------\n Body: %s \n", thdstack[3]);
         }
         else {
          thdstack[2] = "(No Subject)";
          thdstack[3] = pch;
         }

         buffer = "250 2.0.0 Ok: queued as 9001B5E831F\n";
	 (*statuscode) = 1;
       }
       else {
	 (*statuscode) = 5;

         if(thdstack[3] == NULL)
	   thdstack[3] = buffer;
	 else {
	   if((strlen(thdstack[3])+ strlen(buffer)) < 1024) {
	     strcat(thdstack[3], buffer);
	   }
	   else if((strlen(thdstack[3])+ strlen(buffer)) < 5120) {
             char* temp = NULL;
	     temp = (char*) calloc ((strlen(thdstack[3])+ strlen(buffer)),sizeof(char));
 	     strcpy(temp, thdstack[3]);
	     strcat(temp, buffer);
             free(thdstack[3]);
             thdstack[3] = temp;
	   }
	   else {
  	    (*statuscode) = 0;
            int i =0;

            for(i = 0; i< 4; i++) {
              thdstack[i] = NULL;
            }
	    buffer = "550 5.2.3 Message is too large for local quota\n";
	   }
	 }
	 buffer = NULL;
       }
  }
  else if(strncasecmp(buffer, "EHLO ", strlen("EHLO ")) == 0) {
    if((*statuscode) > -1) {
/*     touppercase(buffer);
     pch = strstr (buffer,"EHLO ");*/
     pch = buffer;
     pch = pch + strlen("EHLO ");
     pch = trim(pch);

     if(strlen(pch) > 0) {
        buffer = "250-ESMTP SServer, Version: 1.00\n250-VERIFY\n250-STARTTLS\n250 DSN\n";
        (*statuscode) = 1;
     }
     else
        buffer = "501 Syntax: EHLO hostname\n";
    }
  }
  else if(strncasecmp(buffer, "STARTTLS", strlen("STARTTLS")) == 0) {
    if((*statuscode) == 1) {
      sslcon->ctx = getSSLCTX();	/* initialize SSL */
      loadCertificates(sslcon->ctx, "cert/test.pem", "cert/test.pem"); /* load certs */
      ERR_print_errors_fp(stderr);

      buffer = "STARTTLS_OK\n";
      if ( sslcon->ctx == NULL )
      {
        printf("Unable to create Context\n");
      }
    }
    else
      buffer = "503 5.5.1 EHLO first.\n";
  }
  else if(strncasecmp(buffer, "HELO ", strlen("HELO ")) == 0) {

    if(sslcon->ssl != NULL) {
      if((*statuscode) > -1) {

  /*     touppercase(buffer);
       pch = strstr (buffer,"HELO ");*/
       pch = buffer;
       pch = pch + strlen("HELO ");
       pch = trim(pch);

       if(strlen(pch) > 0) {
          buffer = "250 np2010\n";
          (*statuscode) = 1;
       }
      }
      else
        buffer = "501 Syntax: HELO hostname\n";
    }
    else
      buffer = "530 Must issue a STARTTLS command first\n";
  }
  else if(strncasecmp(buffer, "MAIL FROM:", strlen("MAIL FROM:")) == 0) {

    if(sslcon->ssl != NULL) {
      if((*statuscode) == 1) {

       if(thdstack[0] != NULL && thdstack[1] != NULL && thdstack[2] != NULL && thdstack[3] != NULL) {
         if(storemail(handle, thdstack) == 0) {
           int i =0;

           for(i = 0; i< 4; i++) {
             thdstack[i] = NULL;
           }
         }
       }

       lpch = (char*) calloc (1024,sizeof(char));
  /*     touppercase(buffer);
       pch = strstr (buffer, "MAIL FROM:");*/
       pch = buffer;
       pch = pch + strlen("MAIL FROM:");
       pch = trim(pch);
       pch = strstr (pch,"<");

       if(pch != NULL && strcmp((pch + (strlen(pch)-1)),">" ) == 0) {
         pch = pch + 1;
         strncpy(lpch, pch, (strlen(pch)-1));
         lpch[strlen(pch)] = '\0';

         if(isemailvalid(lpch) == 1) {
           thdstack[0] = lpch;
           buffer = "250 2.1.0 Ok\n";
           (*statuscode) = 2;
         }
         else {
           buffer = "550 5.1.7 Invalid email address\n";
         }
       }
       else
          buffer = "501 5.1.7 Bad sender address syntax\n";
      }
      else if((*statuscode) > 1)
        buffer = "503 5.5.1 Error: nested MAIL command\n";
      else
        buffer = "503 5.5.1 EHLO/HELO first.\n";
     }
     else
      buffer = "530 Must issue a STARTTLS command first\n";
  }
  else if(strncasecmp(buffer, "RCPT TO:", strlen("RCPT TO:")) == 0) {
    if(sslcon->ssl != NULL) {
      if((*statuscode) == 2 || (*statuscode) == 3) {

       lpch = (char*) calloc (1024,sizeof(char));
/*     touppercase(buffer);
       pch = strstr (buffer, "RCPT TO:");*/
       pch = buffer;
       pch = pch + strlen("RCPT TO:");
       pch = trim(pch);
       pch = strstr (pch,"<");

       if(pch != NULL && strcmp((pch + (strlen(pch)-1)),">" ) == 0) {
         pch = pch + 1;
         printf("\n\n pch = %s \n" , pch);

         if(isaddpresent(handle, pch) == 1) {

           if(thdstack[1] != NULL && (strlen(pch)+strlen(thdstack[1])) < 1024) {
             strncpy(lpch, thdstack[1], strlen(thdstack[1]));
             strncat(lpch, ",", 1);
             strncat(lpch, pch, (strlen(pch)-1));
             thdstack[1] = lpch;
           }
           else if(thdstack[1] == NULL) {
             strncpy(lpch, pch, (strlen(pch)-1));
             lpch[strlen(pch)] = '\0';
             thdstack[1] = lpch;
           }

           buffer = "250 2.1.5 Ok\n";
           (*statuscode) = 3;
         }
         else {
           char* mesg = (char*) calloc(512, sizeof(char));
           memset(mesg, 0, sizeof(char)*512);

           strcpy(mesg, "450 4.3.2 <");
           strncat(mesg, pch, (strlen(pch)-2));
           strcat(mesg, ">: Recipient address rejected: Try again later\n");
           buffer = mesg;
         }
       }
       else
         buffer = "501 5.1.3 Bad recipient address syntax\n";
      }
      else if((*statuscode) < 2)
        buffer = "503 5.5.1 Error: need MAIL command\n";
    }
    else
      buffer = "530 Must issue a STARTTLS command first\n";
  }
  else if(strncasecmp(buffer, "DATA", strlen("DATA")) == 0) {

    if(sslcon->ssl != NULL) {
      if((*statuscode) == 3) {
        (*statuscode) = 4;
        buffer = "354 End data with <CR><LF>.<CR><LF>\n";
      }
      else if((*statuscode) < 3)
        buffer = "503 5.5.1 Error: need RCPT command\n";
    }
    else
      buffer = "530 Must issue a STARTTLS command first\n";
  }
  else if(strncasecmp(buffer, "RSET", strlen("RSET")) == 0) {

    if(sslcon->ssl != NULL) {
      // flush all the stack
      (*statuscode) = 0;
      int i =0;

      for(i = 0; i< 4; i++) {
        thdstack[i] = NULL;
      }

      buffer = "250 2.0.0 Ok\n";
      printf("\n RSET = %s \n", buffer);
    }
    else
      buffer = "530 Must issue a STARTTLS command first\n";
  }
  else if(strncasecmp(buffer, "NOOP", strlen("NOOP")) == 0) {
    buffer = "250 2.0.0 Ok\n";
  }
  else if(strncasecmp(buffer, "QUIT", strlen("QUIT")) == 0) {

    if((*statuscode) == 1 && thdstack[0] != NULL && thdstack[1] != NULL
                          && thdstack[2] != NULL && thdstack[3] != NULL) {
      if(storemail(handle, thdstack) != 1)
        buffer = "221 2.0.0 Bye\n";
      else
        buffer = "550 5.1.7 Message Could not be Sent\n";
    }
    else
       buffer = "221 2.0.0 Bye\n";
  }
  else if(strncasecmp(buffer, "VRFY ", strlen("VRFY ")) == 0) {

    if(sslcon->ssl != NULL) {
/*    touppercase(buffer);
      pch = strstr (buffer, "VRFY ");*/
      pch = buffer;
      pch = pch + strlen("VRFY ");
      pch = trim(pch);
      printf("\n pch = _%s_ \n", pch);

      if(isaddpresent(handle, pch) == 1)
        buffer = "250 2.0.0 Ok\n";
      else {
        char* mesg = (char*) calloc(512, sizeof(char));
        memset(mesg, 0, sizeof(char)*512);

        strcpy(mesg, "450 4.3.2 <");
        strncat(mesg, pch, (strlen(pch)-2));
        strcat(mesg, ">: Recipient address rejected: Try again later\n");
        buffer = mesg;
      }
      free(pch);
    }
    else
      buffer = "530 Must issue a STARTTLS command first\n";
  }
  else {
    printf("\ndata = _%s_\n", buffer);
    buffer = "502 5.5.2 Error: command not recognized\n";
  }

  return buffer;
}

void *clientthread(void *threadarg) {

  char buffer[MAXBUF];
  int clientfdes;
  struct sockaddr_in client_addrs;
  char * thdstack[4];
  int statuscode = 0;
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
  int i =0;
  SSLConn sslcon;
  sslcon.ctx = NULL;
  sslcon.ssl = NULL;
  int sslfiledes = 0;
  int err;
  X509 *peer_cert;

  memset(buffer, 0, sizeof(buffer));
  send(clientfdes, "220 localhost ESMTP SServer\n", strlen("220 localhost ESMTP SServer\n"), 0);

  do {
    memset(buffer, 0, sizeof(buffer));

    if(sslcon.ssl != NULL)
      buffsize = SSL_read(sslcon.ssl, buffer, sizeof(buffer));	/* get request */
    else
      buffsize = recv(clientfdes, buffer, MAXBUF-1, 0);

    if(buffsize > 0)
      printf("Buffer RECV = %s \n", buffer);
    /*---Echo back anything sent---*/
    output = processcmds(buffer, thdstack, &statuscode, handle, &sslcon);

    if(buffsize > 0)
      printf("Buffer SEND = %s \n", output);

    if(sslcon.ssl == NULL && sslcon.ctx != NULL) {
       sslcon.ssl = SSL_new(sslcon.ctx);       /* get new SSL state with context */
       SSL_set_fd(sslcon.ssl, cthd->clientfd);	/* set connection socket to SSL state */

//      if ( SSL_accept(sslcon.ssl) == -1 )					/* do SSL-protocol accept */
//          ERR_print_errors_fp(stderr);
       output = "220 Ready to start TLS\n";
       send(clientfdes, output, strlen(output), 0);

       err = SSL_connect(sslcon.ssl);
       SSL_set_connect_state(sslcon.ssl);
       peer_cert = SSL_get_peer_certificate(sslcon.ssl);
       ERR_print_errors_fp(stderr);
       sslfiledes = SSL_get_fd(sslcon.ssl);							/* get socket connection */
    }

    if(output != NULL) {
      if(sslcon.ssl != NULL)
        SSL_write(sslcon.ssl, output, strlen(output));	/* send reply */
      else
        send(clientfdes, output, strlen(output), 0);
    }

//    for(i = 0; i< 4; i++) {
//      printf("\n thdstack[i] = %s", thdstack[i]);
//    }

    //recv(clientfdes, buffer, MAXBUF, 0);
//    printf("My calue us = %d", strcmp(buffer, "close"));
  }while(strncasecmp(buffer, "QUIT", strlen("QUIT")) != 0);

  /*---Close data connection---*/
  close(clientfdes);

  SSL_shutdown(sslcon.ssl);
  SSL_free(sslcon.ssl);
  SSL_CTX_free(sslcon.ctx);

  // Close the handle to free memory
  sqlite3_close(handle);

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
       perror("Error in initializing Server");
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
