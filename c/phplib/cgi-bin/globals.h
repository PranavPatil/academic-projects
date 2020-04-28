
/***********************************************************************************************/
//
//        Network Programming: Milestone 5
//
//        Compile: sh compile.sh (set path in /srv/www/cgi-bin for Apache2 in openSuse 11.2)
//        Run: On the browser type: localhost//cgi-bin/cscript.cgi
//
/***********************************************************************************************/

#ifndef GLOBALS_H // header guards

  #define GLOBALS_H

  #include < stdio.h >
  #include < stdlib.h >
  #include < string.h >
  #include < ctype.h >
  #include "list.h"

  #define SERVAR_NO 29
  #define GET 1
  #define POST 2
  #define COOKIE 3
  #define SERVER 4

  extern char envars[SERVAR_NO][80]; // Server environment variable ids
  extern char* Server[SERVAR_NO];   //  Server and execution environment information
  extern List Get;   //  HTTP GET variables
  extern List Post;   //  HTTP POST variables
  extern List Cookie;   //  HTTP Cookies

  char* replacechar( char*, char, char);

  int initServ();

  int initGet();

  int initPost();

  int initCookie();

  void initglobalvars();

#endif
