
/***********************************************************************************************/
//
//        Network Programming: Milestone 5
//
//        Compile: sh compile.sh (set path in /srv/www/cgi-bin for Apache2 in openSuse 11.2)
//        Run: On the browser type: localhost//cgi-bin/cscript.cgi
//
/***********************************************************************************************/
#ifndef PHPLIB_H // header guards

  #include "globals.h"

  #define PHPLIB_H
  #define TRUE  1
  #define FALSE 0

  typedef int bool;
  enum bool {false, true};
  extern List Header;

  void header(char*, bool, int);

  bool setcookie (char*, char*, int, char*, char*, bool, bool);

  void print_r(int);

#endif
