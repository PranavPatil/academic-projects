
/***********************************************************************************************/
//
//        Network Programming: Milestone 5
//
//        Compile: sh compile.sh (set path in /srv/www/cgi-bin for Apache2 in openSuse 11.2)
//        Run: On the browser type: localhost//cgi-bin/cscript.cgi
//
/***********************************************************************************************/

#include < string.h >
#include < stdlib.h >
#include < stdio.h >
#include < ctype.h >
#include "phplib.h"

// print the code for the HTML file, making sure to include the content-type

void printHeader(char * title)
{
  // Print the HTML header
  //printf("Content-type: text/html\n");
  dumpList(&Header);
  printf("\n<HTML>\n");
  printf("<HEAD><TITLE>%s</TITLE></HEAD>\n",title);
  printf("<BODY bgcolor=\"#FFFFFF\">\n");
  printf("<H1>%s</H1>\n",title);
  return ;
}

void htmlPre()
{
  // print the commands to send the code to the screen pre-formatted for HTML
  printf("<PRE><BR>\n");
  return ;
}

void htmlPreOff()
{
  // print the commands to send the code to the screen pre-formatted for HTML
  printf("</PRE>\n");
  return ;
}

void printFooter()
{
  // close out the HTML file
  printf("</BODY>\n");
  printf("</HTML>\n");
  return ;
}

int main(int count, char *strings[])
{
    initglobalvars();
    header("Content-type: text/html", TRUE, 0);
//    header("Location: http://www.example.com/", FALSE, 0);
//    header("Location: http://www.google.com/", TRUE, 0);
    setcookie("Name", "Pranav", 0, "/", "localhost", false, false);
    printHeader("Test Page");
    printf("<br>Hello World !!<br><br>");
    print_r(SERVER);
    print_r(GET);
    print_r(POST);
    print_r(COOKIE);
    printFooter();
    return 0;
}
