
#include < stdio.h >
#include < stdlib.h >
#include < string.h >
#include < arpa/inet.h >
#include < unistd.h >
#include < sys/types.h >
#include < errno.h >

//#define LINUX /* define this if you are on linux   */

# include < netdb.h >         /* gethostbyname  */
# include < netinet/in.h >    /* htons          */
# include < sys/socket.h >
# include "base64.h"

# define BUFFERSIZE 4096

static char* substring(const char* str, size_t begin, size_t len)
{
  if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
    return 0;

  return strndup(str + begin, len);
}

char* writeMessage(unsigned char* result,
                   const char *str,
                   const char *arg)
{
    char buf[BUFFERSIZE];
    char* newresult;

    if (arg != NULL)
        snprintf(buf, sizeof(buf), str, arg);
    else
        snprintf(buf, sizeof(buf), str);

    int count = 0;

    if(result != NULL)
      count = strlen(result) + BUFFERSIZE + 1;
    else
      count = BUFFERSIZE + 1;

//    result = (char*) realloc (result, count * sizeof(char));
    newresult = (char*) calloc (count, sizeof(char));

    if(result != NULL) {
      strcpy(newresult, result);
      strcat(newresult, buf);
    }
    else {
      strcpy(newresult, buf);
    }

    free(result);
    return newresult;
}

static char* writeData(unsigned char* result, const char *str, const size_t size) {

    char buf[size];
    char* newresult;
    int count = 0;

    snprintf(buf, sizeof(buf), str);

    if(result != NULL)
      count = strlen(result) + size + 1;
    else
      count = size + 1;

    newresult = (char*) calloc (count, sizeof(char));

    if(result != NULL) {
      strcpy(newresult, result);
      strcat(newresult, buf);
    }
    else {
      strcpy(newresult, buf);
    }

    free(result);
    return newresult;
}

void recvmessage(int sock) {

  int len = 0;
  char buffer[BUFFERSIZE];
  len = recv(sock, buffer, BUFFERSIZE-1, 0);

  if(len !=0)
  {
   /* We have to null terminate the received data ourselves */
   buffer[len] = '\0';
   printf("Received: \n %s\n",buffer);
  }
}

int composemail(unsigned char* result,
		int*result_size,
                char *to,
		char *from,
		char *charset,
                char *subject,
                unsigned char *body,
		int attachments_nb,
		unsigned char **attachments,
		int *attachments_sizes,
		char **attachnames)
{
  char* newresult = NULL;
  newresult = writeMessage(newresult, "HELO %s\n",       from);    // greeting
  newresult = writeMessage(newresult, "MAIL FROM: %s\n", from);    // from
  newresult = writeMessage(newresult, "RCPT TO: %s\n",   to);      // to
  newresult = writeMessage(newresult, "DATA\n",          NULL);    // begin data

    // next comes mail headers
  newresult = writeMessage(newresult, "From: %s\n",      from);
  newresult = writeMessage(newresult, "To: %s\n",        to);
  newresult = writeMessage(newresult, "Subject: %s.\n",   subject);
  newresult = writeMessage(newresult, "Sender: Pranav Patil\n", NULL);
  newresult = writeMessage(newresult, "MIME-Version: 1.0\n", NULL);
  newresult = writeMessage(newresult, "Content-Type: multipart/mixed; boundary=000e0cd1855061660204800424b2\n", NULL);
  newresult = writeMessage(newresult, "Content-Description: multipart-1\n", NULL);

  newresult = writeMessage(newresult, "--000e0cd1855061660204800424b2\n", NULL);
  newresult = writeMessage(newresult, "Content-Type: text/plain; charset=%s\n", charset);
  newresult = writeMessage(newresult, "%s\n",            body);    // data

  int attachno = 0;

  while(attachno < attachments_nb) {

    newresult = writeMessage(newresult, "\n--000e0cd1855061660204800424b2\n", NULL);

    char * pch;
    pch=strrchr(attachnames[attachno],'.');
    pch = pch + 1;

    if (strncmp (pch,"gif",3) == 0)  {
      newresult = writeMessage(newresult, "Content-Type: image/gif\n", NULL);
    }
    else if (strncmp (pch,"jpg",3) == 0)  {
      newresult = writeMessage(newresult, "Content-Type: image/jpg\n", NULL);
    }
    else if (strncmp (pch,"jpeg",4) == 0)  {
      newresult = writeMessage(newresult, "Content-Type: image/jpeg\n", NULL);
    }
    else if (strncmp (pch,"png",3) == 0)  {
      newresult = writeMessage(newresult, "Content-Type: image/png\n", NULL);
    }
    else if (strncmp (pch,"txt",3) == 0)  {
      newresult = writeMessage(newresult, "Content-Type: text/plain\n", NULL);
    }

    newresult = writeMessage(newresult, "Content-Description: \"Base64 encode of data by Wincode 2.7.3\"\n", NULL);
    newresult = writeMessage(newresult, "Content-Disposition: attachment; filename=\"%s\"\n", attachnames[attachno]);
    newresult = writeMessage(newresult, "Content-Transfer-Encoding: Base64\n", NULL);
    newresult = writeData(newresult, attachments[attachno], strlen(attachments[attachno]));
    //writeMessage("\n",              NULL);
    attachno++;
  }

  newresult = writeMessage(newresult, "\n--000e0cd1855061660204800424b2\n\n", NULL);
  newresult = writeMessage(newresult, ".\n",             NULL);    // end data
  newresult = writeMessage(newresult, "QUIT\n",          NULL);    // terminate

  strcpy(result, newresult);
  return 0;
}

int sendmail(unsigned char* buffer,
	     int* size, char* smtp_server,
	     int port, char* password,
	     char* login,
	     char*to,
	     char*from)
{
   struct hostent *host;
   struct sockaddr_in saddr_in;
   int sock = 0;

   sock = socket(AF_INET, SOCK_STREAM, 0);
   host = gethostbyname(smtp_server);

   saddr_in.sin_family      = AF_INET;
   saddr_in.sin_port        = htons((u_short)port);
   saddr_in.sin_addr.s_addr = 0;

   memcpy((char*)&(saddr_in.sin_addr), host->h_addr, host->h_length);

   if (connect(sock, (struct sockaddr*)&saddr_in, sizeof(saddr_in)) == -1) {
     return -2;
   }

   int retval = 0;
   retval = send(sock, buffer, strlen(buffer), 0);     // strlen(buf), 0);

   if(retval == -1)
    strerror( errno );

   recvmessage(sock);
   close(sock);
}

int flength(FILE *f)
{
   int pos;
   int end;

   pos = ftell (f);
   fseek (f, 0, SEEK_END);
   end = ftell (f);
   fseek (f, pos, SEEK_SET);

   return end;
}

/* --------------------------------------------------------------------------------- */
/* EMBEDED CODE FOR BASE 64 ENCODING TAKEN FROM OUTSIDE SOURCE

MODULE NAME:    b64.c

AUTHOR:         Bob Trower 08/04/01

PROJECT:        Crypt Data Packaging

COPYRIGHT:      Copyright (c) Trantor Standard Systems Inc., 2001

NOTE:           This source code may be used as you wish, subject to
                the MIT license.  See the LICENCE section below.

*/

static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
** Translation Table to decode (created by author)
*/
static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

/*
** encodeblock
**
** encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
void encodeblock( unsigned char in[3], unsigned char out[4], int len )
{
    out[0] = cb64[ in[0] >> 2 ];
    out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}

/*
** encode
**
** base64 encode a stream adding padding and line breaks as per spec.
*/
void encode( FILE *infile, FILE *outfile, int linesize )
{
    unsigned char in[3], out[4];
    int i, len, blocksout = 0;

    while( !feof( infile ) ) {
        len = 0;
        for( i = 0; i < 3; i++ ) {
            in[i] = (unsigned char) getc( infile );
            if( !feof( infile ) ) {
                len++;
            }
            else {
                in[i] = 0;
            }
        }
        if( len ) {
            encodeblock( in, out, len );
            for( i = 0; i < 4; i++ ) {
                putc( out[i], outfile );
            }
            blocksout++;
        }
        if( blocksout >= (linesize/4) || feof( infile ) ) {
            if( blocksout ) {
                fprintf( outfile, "\r\n" );
            }
            blocksout = 0;
        }
    }
}

/*
** decodeblock
**
** decode 4 '6-bit' characters into 3 8-bit binary bytes
*/
void decodeblock( unsigned char in[4], unsigned char out[3] )
{
    out[ 0 ] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
    out[ 1 ] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
    out[ 2 ] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}

/*
** decode
**
** decode a base64 encoded stream discarding padding, line breaks and noise
*/
void decode( FILE *infile, FILE *outfile )
{
    unsigned char in[4], out[3], v;
    int i, len;

    while( !feof( infile ) ) {
        for( len = 0, i = 0; i < 4 && !feof( infile ); i++ ) {
            v = 0;
            while( !feof( infile ) && v == 0 ) {
                v = (unsigned char) getc( infile );
                v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
                if( v ) {
                    v = (unsigned char) ((v == '$') ? 0 : v - 61);
                }
            }
            if( !feof( infile ) ) {
                len++;
                if( v ) {
                    in[ i ] = (unsigned char) (v - 1);
                }
            }
            else {
                in[i] = 0;
            }
        }
        if( len ) {
            decodeblock( in, out );
            for( i = 0; i < len - 1; i++ ) {
                putc( out[i], outfile );
            }
        }
    }
}

/*
** returnable errors
**
** Error codes returned to the operating system.
**
*/
#define B64_SYNTAX_ERROR        1
#define B64_FILE_ERROR          2
#define B64_FILE_IO_ERROR       3
#define B64_ERROR_OUT_CLOSE     4
#define B64_LINE_SIZE_TO_MIN    5

/*
** b64_message
**
** Gather text messages in one place.
**
*/
char *b64_message( int errcode )
{
    #define B64_MAX_MESSAGES 6
    char *msgs[ B64_MAX_MESSAGES ] = {
            "b64:000:Invalid Message Code.",
            "b64:001:Syntax Error -- check help for usage.",
            "b64:002:File Error Opening/Creating Files.",
            "b64:003:File I/O Error -- Note: output file not removed.",
            "b64:004:Error on output file close.",
            "b64:004:linesize set to minimum."
    };
    char *msg = msgs[ 0 ];

    if( errcode > 0 && errcode < B64_MAX_MESSAGES ) {
        msg = msgs[ errcode ];
    }

    return( msg );
}

/*
** b64
**
** 'engine' that opens streams and calls encode/decode
*/

int b64( int opt, char *infilename, char *outfilename, int linesize )
{
    FILE *infile;
    int retcode = B64_FILE_ERROR;

    if( !infilename ) {
        infile = stdin;
    }
    else {
        infile = fopen( infilename, "rb" );
    }
    if( !infile ) {
        perror( infilename );
    }
    else {
        FILE *outfile;
        if( !outfilename ) {
            outfile = stdout;
        }
        else {
            outfile = fopen( outfilename, "wb" );
        }
        if( !outfile ) {
            perror( outfilename );
        }
        else {
            if( opt == 'e' ) {
                encode( infile, outfile, linesize );
            }
            else {
                decode( infile, outfile );
            }
            if (ferror( infile ) || ferror( outfile )) {
                retcode = B64_FILE_IO_ERROR;
            }
            else {
                 retcode = 0;
            }
            if( outfile != stdout ) {
                if( fclose( outfile ) != 0 ) {
                    perror( b64_message( B64_ERROR_OUT_CLOSE ) );
                    retcode = B64_FILE_IO_ERROR;
                }
            }
        }
        if( infile != stdin ) {
            fclose( infile );
        }
    }

    return( retcode );
}

/*
** showuse
**
** display usage information, help, version info
*/
void showuse( int morehelp )
{
    {
        printf( "\n" );
        printf( "  b64      (Base64 Encode/Decode)      Bob Trower 08/03/01 \n" );
        printf( "           (C) Copr Bob Trower 1986-01.      Version 0.00B \n" );
        printf( "  Usage:   b64 -option  [ -l num ] [<FileIn> [<FileOut>]]  \n" );
        printf( "  Purpose: This program is a simple utility that implements\n" );
        printf( "           Base64 Content-Transfer-Encoding (RFC1113).     \n" );
    }
    if( !morehelp ) {
        printf( "           Use -h option for additional help.              \n" );
    }
    else {
        printf( "  Options: -e  encode to Base64   -h  This help text.      \n" );
        printf( "           -d  decode from Base64 -?  This help text.      \n" );
        printf( "  Note:    -l  use to change line size (from 72 characters)\n" );
        printf( "  Returns: 0 = success.  Non-zero is an error code.        \n" );
        printf( "  ErrCode: 1 = Bad Syntax, 2 = File Open, 3 = File I/O     \n" );
        printf( "  Example: b64 -e binfile b64file     <- Encode to b64     \n" );
        printf( "           b64 -d b64file binfile     <- Decode from b64   \n" );
        printf( "           b64 -e -l40 infile outfile <- Line Length of 40 \n" );
        printf( "  Note:    Will act as a filter, but this should only be   \n" );
        printf( "           used on text files due to translations made by  \n" );
        printf( "           operating systems.                              \n" );
        printf( "  Release: 0.00.00, Tue Aug 7   2:00:00 2001, ANSI-SOURCE C\n" );
    }
}

//#define B64_DEF_LINE_SIZE   72
//#define B64_MIN_LINE_SIZE    4

//#define THIS_OPT(ac, av) (ac > 1 ? av[1][0] == '-' ? av[1][1] : 0 : 0)

// END OF EMBEDED CODE FOR BASE 64
/* --------------------------------------------------------------------------------- */

int main(int argc, char *argv[]) {

  argc = 2;
  argv[0] = "globe3t.gif";
  argv[1] = "pirates1.jpg";

  char *tempfilename = "temp.txt";
  char *buffer = NULL;
  unsigned char **farray;
  int *fsize;
  FILE *tempfile;
  int retcode = 0, count = 0;
  int len = 0;

  if(argc > 0)
    farray = malloc(argc * sizeof(char*));

  while(count < argc) {

   retcode = b64( 'e', argv[count], tempfilename, B64_DEF_LINE_SIZE );

   if(retcode == 0) {
     tempfile = fopen( tempfilename, "rb" );

     if (tempfile == NULL) {
      perror ("Error opening file");
      break;
     }
     else {
      // Get file size
      fseek (tempfile , 0 , SEEK_END);
      len = ftell (tempfile);
      rewind (tempfile);

      /* allocate memory to contain the whole header file.*/
      buffer = (char*) malloc (len);

      /* read the file into the buffer. */
      fread (buffer,1,len,tempfile);

      fsize = malloc(1*sizeof(int));
      fsize[count] = len;
//      printf("Length = %d \n\n", len);

      int nrows = 1;
  //  fcontent = malloc(len*sizeof(char));
  //  fgets(fcontent, len, outfile);

      farray[count] = buffer;
      fclose(tempfile);
     }
   }
   count++;
  }

  /* Wait till threads are complete before main continues. Unless we  */
  /* wait we run the risk of executing an exit which will terminate   */
  /* the process and all threads before the threads have completed.   */

  unsigned char* result = NULL;
  int result_size = 0, ret=0;
  result = (char*) calloc(100000, sizeof(char));

  composemail( result, &result_size,
       "pranav@my.it.edu",       /* to       */
       "pranav@gmail.com",   /* from     */
       "ISO-8859-1",               /* charset */
       "Hello",              /* subject  */
       "How are you ?",                 /* body     */
       argc,                     /* no of attachments */
       farray, /* array of attachments */
       fsize,  /* attachments sizes */
       (char**)argv
  );

//  printf("\n\n\n%s\n", result);
//  exit(0);

  if (ret == 0) {
    ret = sendmail(result, &result_size,
              "mailhost.it.edu",             /* hostname */
	      25,                      /* port     */
              "password",
	      "login",
              "pranav@my.it.edu",       /* to       */
              "pranav@gmail.com");   /* from     */

    if (ret != 0)
      fprintf(stderr, "Failed to send mail (code: %i).\n", ret);
    else
      fprintf(stdout, "Mail successfully sent.\n");
  }
  else
     fprintf(stderr, "Failed to send mail (code: %i).\n", ret);

  return ret;
}
