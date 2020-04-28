
/***********************************************************************************************/
//
//        Network Programming: Milestone 5
//
//        Compile: sh compile.sh (set path in /srv/www/cgi-bin for Apache2 in openSuse 11.2)
//        Run: On the browser type: localhost//cgi-bin/cscript.cgi
//
/***********************************************************************************************/

#include "list.h"

void addData(List* lst, char* id, char* value) {  // add element to linked list
  Data *newData;
  newData = (Data *)malloc(sizeof(Data));       // create node or element
  newData->id = id;
  newData->value = value;
  newData->next = NULL;

  if(lst->headData == NULL) {        // if there is no headnode then create a headnode for the list.
     lst->headData = newData;
  }
  else {
     (*(lst->lastData)).next = newData;  // append the element to the end of the list.
  }

  lst->lastData = newData;            // assign the current node as the last node.
  lst->size++;
}

Data* getData(List* lst, char* id) {  // get node at a given position
  Data *curData = NULL;
  curData = lst->headData;
  int found = 0;

  while(curData != NULL && found == 0) {
    if(strcasecmp(curData->id, id) == 0)
      found = 1;
    else
      curData = curData->next;
  }

  return curData;
}

int updateData(List* lst, char* id, char* newvalue) { // updates the element at a particular position

   int ret = 0;
   Data *curData = NULL;
   curData = (Data*) getData(lst, id);   // get the node at the given position

   if(curData != NULL) {
     curData->value = newvalue;       // update the data in the node
     ret = 1;
   }
   else
     ret = 0;

   return ret;
}

void showList(List* lst) {  // display linked list
  Data *curData;

  curData = lst->headData;

  if(curData == NULL)
    printf("\t");

  while(curData != NULL) {
    printf(" [%s] =>  %s", curData->id, curData->value);  // display list from start
    curData = curData->next;
  }
}

void dumpList(List* lst) {  // displays linked list
  Data *curData;

  curData = lst->headData;

  while(curData != NULL) {
    printf("%s:%s\n", curData->id, curData->value);  // display list from start
    curData = curData->next;
  }
}

void freeList(List* lst)  { // deletes the element in the given position

  Data *curData;
  curData = lst->headData;

  while(curData != NULL) {
    curData = curData->next;
    free(lst->headData);
    lst->headData = curData;
  }

  lst->headData = NULL;
  lst->lastData = NULL;
  lst->size = 0;
}
