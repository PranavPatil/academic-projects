
#include < stdio.h >
#include < stdlib.h >
#include < string.h >
#include < arpa/inet.h >
#include < unistd.h >
#include < sys/types.h >
#include < errno.h >

# include < netdb.h >         /* gethostbyname  */
# include < netinet/in.h >    /* htons          */
# include < sys/socket.h >
# include "base64.h"

#define MAX_CH 255
#define MAXLEN 511
#define POP3_PORT 110
# define BUFFERSIZE 4096

static void sendmsge( const int  sock, const char *str, const char *arg ) {
  char buf[BUFFERSIZE];
  int retval = 0;

  if (arg != NULL)
    snprintf(buf, sizeof(buf), str, arg);
  else
    snprintf(buf, sizeof(buf), str);

  //printf("Send    =    %s", buf);
  retval = send(sock, buf, strlen(buf), 0);

  if(retval == -1)
   strerror( errno );
}

static char* receivemsge(int sock) {
   int len = 0;
   char* buffer = (char*) malloc(BUFFERSIZE);

   len = recv(sock, buffer, BUFFERSIZE-1, 0);

   if(len !=0)
   {
     /* We have to null terminate the received data ourselves */
    buffer[len] = '\0';
 //   printf("Received: \n %s\n",buffer);
   }
   return buffer;
}

static char* receivedata(int sock, int buffsize) {
   int len = 0;
   char* buffer = (char*) malloc(buffsize);

   len = recv(sock, buffer, buffsize-1, 0);

   if(len !=0)
   {
     /* We have to null terminate the received data ourselves */
    buffer[len] = '\0';
 //   printf("Received: \n %s\n",buffer);
   }
   return buffer;
}

static char* substring(const char* str, size_t begin, size_t len)
{
  if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
    return 0;

  return strndup(str + begin, len);
}

char* getemailid(int sock, char* buffer, unsigned char* subject_signature, int no) {

  int count = 0, found = 0;
  char* strset = NULL;
  char* scount = NULL;
  scount = (char*) calloc(10, sizeof(char));
  char* pch = NULL;
  char* fch = NULL;
  char final[100];
  int len = 0;

  while(count < no && found != 1) {

    sprintf(scount, "%d", (count+1));
    sendmsge(sock, "TOP  %s  1\n", scount);    // list
    strset = receivemsge(sock);

    pch = strstr (strset,"Subject: ");

    if(pch != NULL) {
      pch = pch + 9; // skip "Subject: " of the string
      fch = strchr(pch,'.');   // parse subject until full stop

      if(fch != NULL) {
        len = strlen(pch) - strlen(fch);
        strncpy(final, pch, len);
        final[len] = '\0';

	printf("data ##################################################### = %s\n", final);

	if (strncmp (final,subject_signature,strlen(subject_signature)) == 0) {
          found = 1;
	  break;
        }
      }
    }
    count++;
  }

  if(found == 1)
    sprintf(scount, "%d", (count));  // count is equalto no of mails
  else
    sprintf(scount, "%d", (0));  // count is equalto no of mails

  return scount;
}

 int pollmail(char* smtp_server, int port, char* password, char* login, unsigned char* subject_signature,
	      unsigned char* mail, int* mail_size)
 {
    struct hostent *host;
    struct sockaddr_in saddr_in;
    int sock = 0;
    char* buffer = NULL;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    host = gethostbyname(smtp_server);

    saddr_in.sin_family      = AF_INET;
    saddr_in.sin_port        = htons((u_short)port);
    saddr_in.sin_addr.s_addr = 0;

    memcpy((char*)&(saddr_in.sin_addr), host->h_addr, host->h_length);

   if (connect(sock, (struct sockaddr*)&saddr_in, sizeof(saddr_in)) == -1) {
	return -2;
   }

    buffer = receivemsge(sock);
    sendmsge(sock, "USER  %s \n",       login);    // login
    buffer = receivemsge(sock);
    sendmsge(sock, "PASS  %s\n", password);    // pass
    buffer = receivemsge(sock);
    sendmsge(sock, "STAT\n", NULL);    // list
    buffer = receivemsge(sock);

    char* pch = NULL;
    int recno = 0;
    pch = strtok (buffer," ");
    pch = strtok (NULL, " ");
    recno = atoi (pch);

    printf ("-------------------------------------------- >  %d\n",recno);

    char* mailno = NULL;
    mailno = getemailid(sock, buffer, subject_signature, recno);
    printf("mailno = %s\n\n", mailno);

    if(strncmp (mailno,"0",1) != 0) {
      sendmsge(sock, "LIST  %s\n", mailno);    // list itemno
      buffer = receivemsge(sock);
      printf("Received: \n %s\n",buffer);
      sendmsge(sock, "retr %s\n", mailno);    // from
      buffer = receivedata(sock, 100000);
      printf("Received: \n %s\n",buffer);
      strcpy(mail, buffer);
    }
    close(sock);
    return 0;
 }

char* getnextparam (char* mydata, char* target)
{
  char* pch = NULL;
  char* fch = NULL;
  int len = 0;
  pch = strstr (mydata,target);
  pch = pch + (strlen(target) + 1); // skip "boundary=" of the string
  fch = strchr(pch,' ');
  printf("\n\nfch = %s", fch);
  len = strlen(pch) - strlen(fch);
  printf("\n\nlength = %d", len);
  char * final = (char*) calloc (len+1, sizeof(char));
  strncpy(final, pch, len);
  final[len+1] = '\0';
  return final;
}


 int parsemail(unsigned char* mail, int* mail_size, char* body, int buf_size, int attachments_nb,
	       unsigned char* attachments, int* attachment_sizes)
 {
   char* boundry = NULL;
   boundry = getnextparam(mail, "boundary=");
   printf("\n\n\ndata ------------------- = %s\n", boundry);
   char* st = "--";
   char* boundid = (char*) calloc ((strlen(boundry) + 3), sizeof(char));
   strncpy(boundid, "--", 2);
   strcpy(boundid+2, boundry);
   printf("\n\n bound = %s", boundid);
   printf("\n\n\ndata ------------------- = %s\n", getnextparam(mail, boundid));

   char* pch = NULL;
   char* fch = NULL;
   int len = 0;
   pch = strstr (mail,boundid);
   pch = pch + (strlen(boundid) + 1); // skip "boundary=" of the string
   printf("\n\nval = %s", pch);
   return 0;
 }

 void saveattachments() {


 }

int main() {
  unsigned char* email;
  int len = strlen(email);
  int* email_size = &len;
  email = (char*) calloc(100000, sizeof(char));
  pollmail("localhost", 11000, "password", "pranav@my.it.edu", "Summer Conference & Housing Job Openings - College Roll",  email, email_size);
  exit(0);
  char* body;
  unsigned char* attachments;
  int buf_size, attachments_nb;
  int * attachment_sizes;
  parsemail(email, email_size, body, buf_size, attachments_nb,
	    attachments, attachment_sizes);
  // save attachments
  saveattachments(attachments, attachments_nb, attachment_sizes);
  return 0;

}
