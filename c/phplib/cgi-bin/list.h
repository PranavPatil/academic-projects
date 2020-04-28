
/***********************************************************************************************/
//
//        Network Programming: Milestone 5
//
//        Compile: sh compile.sh (set path in /srv/www/cgi-bin for Apache2 in openSuse 11.2)
//        Run: On the browser type: localhost//cgi-bin/cscript.cgi
//
/***********************************************************************************************/


#ifndef LIST_H // header guards

  #include < stdio.h >
  #include < stdlib.h >
  #include < string.h >
  #include < ctype.h >

  #define LIST_H

  typedef struct node{
    char *id;
    char* value;
    struct node *next;
  } Data;

  typedef struct list{
    Data* headData;
    Data* lastData;
    int size;
  } List;

  void addData(List* , char*, char*);  // add element to linked list

  Data* getData(List* , char*);  // get node at a given position

  int updateData(List* , char*, char*); // updates the element at a particular position

  void showList(List*);  // display linked list

  void dumpList(List*);  // displays linked list

  void freeList(List*); // deletes the element in the given position

#endif
