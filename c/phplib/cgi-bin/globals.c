
/***********************************************************************************************/
//
//        Network Programming: Milestone 5
//
//        Compile: sh compile.sh (set path in /srv/www/cgi-bin for Apache2 in openSuse 11.2)
//        Run: On the browser type: localhost//cgi-bin/cscript.cgi
//
/***********************************************************************************************/

#include "globals.h"

char envars[SERVAR_NO][80]={"HTTP_HOST", "HTTP_USER_AGENT", "HTTP_ACCEPT",
                           "HTTP_ACCEPT_LANGUAGE", "HTTP_ACCEPT_ENCODING",
                           "HTTP_ACCEPT_CHARSET", "HTTP_KEEP_ALIVE",
                           "HTTP_CONNECTION", "HTTP_CACHE_CONTROL", "PATH",
                           "SERVER_SIGNATURE", "SERVER_SOFTWARE", "SERVER_NAME",
                           "SERVER_ADDR", "SERVER_PORT", "REMOTE_ADDR",
                           "DOCUMENT_ROOT", "SERVER_ADMIN", "SCRIPT_FILENAME",
                           "REMOTE_PORT", "GATEWAY_INTERFACE", "SERVER_PROTOCOL",
                           "REQUEST_METHOD", "QUERY_STRING", "REQUEST_URI",
                           "SCRIPT_NAME", "PHP_SELF", "REQUEST_TIME", "HTTP_COOKIE"};

char* Server[SERVAR_NO];   //  Server and execution environment information
List Get;   //  HTTP GET variables
List Post;   //  HTTP POST variables
List Cookie;   //  HTTP Cookies

/** Convert a two-char hex string into the char it represents **/
char x2c(char *what) {
  register char digit;

  digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
  digit *= 16;
  digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
  return(digit);
}

/** Reduce any %xx escape sequences to the characters they represent **/
char* unescape_url(char *url) {
  register int i,j;

  for(i=0,j=0; url[j]; ++i,++j) {
    if((url[i] = url[j]) == '%') {
      url[i] = x2c(&url[j+1]) ;
      j+= 2 ;
    }
  }
  url[i] = '\0' ;
  return url;
}

char* replacechar( char *str, char curchar, char newchar)
{
  char *sptr;
  int i = 0;
  int len =0;
  str = sptr = strdup (str);
//  printf("\n\n str = %s \n\n", sptr);

  if (!sptr)
    return NULL;
  else
    len = strlen(sptr);

  /* remove the null terminator */
  for (i=0; i < len-1; i++)
  {
    sptr++;
    if (*sptr == curchar)
      *sptr = newchar;
  }
    /* if we get all the way to here, there must not have been a newline! */
   // printf("\n\n str = %s \n\n", str);
   return str;
}

void parseHeader(char* cgiinput, int reqid)
{
  List* plist;
  int paircount ;
  char *nvpair ;
  char** pairlist;
  char *eqpos ;
  char* separator;

  if(reqid == GET) {
    plist = &Get;
    separator = "&";
  }
  else if(reqid == POST) {
    plist = &Post;
    separator = "&";
  }
  else if(reqid == COOKIE) {
    plist = &Cookie;
    separator = ";";
  }
  else
    return;

  // Change all plusses back to spaces
  cgiinput = replacechar(cgiinput, '+', ' ');

  /** First, split on "&" to extract the name-value pairs into pairlist **/
  pairlist= (char **) malloc(256*sizeof(char **)) ;
  paircount= 0 ;
  nvpair= strtok(cgiinput, separator) ;

  while (nvpair) {
    pairlist[paircount++]= strdup(nvpair) ;
    if (!(paircount%256))
      pairlist= (char **) realloc(pairlist,(paircount+256)*sizeof(char **)) ;
    nvpair= strtok(NULL, separator) ;
  }
  pairlist[paircount]= 0 ;    // terminate the list with NULL

    // List of pairs, extract the names and values
  plist->headData = NULL;
  plist->lastData = NULL;

  int i = 0;
  char* id = NULL;
  char* value = NULL;

  for (i= 0; i<paircount; i++) {
    if ((eqpos=strchr(pairlist[i], '=')) != NULL) {
      *eqpos= '\0' ;
      value = unescape_url(strdup(eqpos+1)) ;

    } else {
      value = unescape_url(strdup("")) ;
    }
    id = unescape_url(strdup(pairlist[i])) ;
    addData(plist, id, value);
  }

  // Free anything that needs to be freed
  free(cgiinput) ;
  for (i=0; pairlist[i]; i++) free(pairlist[i]) ;
  free(pairlist) ;
}

int initServ() {
  int i;

  for (i=0;i<SERVAR_NO;i++) {
    Server[i] = getenv(envars[i]);
  }

  return SERVAR_NO;
}

int initGet()
{
   int query_size= 0;
   char* cginput = NULL;
   cginput = getenv("QUERY_STRING");

   if (cginput != NULL) {
     query_size = strlen(cginput);
     parseHeader(cginput, GET);
   }

   return query_size;
}

int initPost()
{
   int query_size= 0;
   char* cginput = NULL;
   cginput = getenv("CONTENT_LENGTH");

   if(cginput != NULL)
     query_size = atoi(cginput);

   if (query_size > 0) {
      cginput = (char*) malloc(query_size);

     if (cginput != NULL) {
       fread(cginput,query_size,1,stdin);
       parseHeader(cginput, POST);
     }
     else {
       query_size = 0;
     }
   }

   return query_size;
}

int initCookie() {
   int query_size= 0;
   char* cginput = NULL;
   cginput = getenv("HTTP_COOKIE");

   if (cginput != NULL) {
     query_size = strlen(cginput);
     parseHeader(cginput, COOKIE);
   }

   return query_size;
}

void initglobalvars() {
  initServ();
  initGet();
  initPost();
  initCookie();
}
