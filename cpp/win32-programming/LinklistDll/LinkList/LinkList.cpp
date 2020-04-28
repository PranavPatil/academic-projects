// LinkList.cpp : Defines the exported functions for the DLL application.

// Assignment No : 8
// PRANAV PATIL


#include "stdafx.h"
#include "LinkList.h"
#include < iostream >

using namespace std;

// This is the constructor of a class that has been exported.
// see LinkList.h for the class definition
CLinkList::CLinkList()
{
    headNode = NULL;  // initialize all field to default values
    lastNode = NULL;
    size = 0;
	return;
}

void CLinkList::addNode(int ndata) {  // add element to linked list
  Node *newNode;
  newNode = new Node();       // create node or element
  (*newNode).data = ndata;

  if(headNode == NULL) {        // if there is no headnode then create a headnode for the list.
	 headNode = newNode;
    (*headNode).prev = NULL;
  }
  else {
	(*lastNode).next = newNode;  // append the element to the end of the list.
    (*newNode).prev = lastNode;
  }
  (*newNode).next = NULL;        
  lastNode = newNode;            // assign the current node as the last node.
  size++;                        // increment size
}

bool CLinkList::insertNode(int n,int position) { // insert element at specified position in the list
  Node *newNode;
  newNode = new Node();              // create the node or element
  (*newNode).data = n;

  if(position == 1 && size > 0) {                // insert element at the start of the list
        (*newNode).next = headNode;
        (*headNode).prev = newNode;
        (*newNode).prev = NULL;
        headNode = newNode;
        size++;
  }
  else if(position == 1 && size == 0) {                // insert element at the start of the list
        (*newNode).next = NULL;
        (*newNode).prev = NULL;
        headNode = newNode;
		lastNode = headNode;
        size++;
  }
  else if (position == size+1) {      // append element at the end of the list
	  (*lastNode).next = newNode;
	  (*newNode).prev = lastNode;
	  (*newNode).next = NULL;
      lastNode = newNode;
      size++;
  }
  else if (1 < position && position < size+1) {  // insert element in between the linked list
	  Node *cNode = getNode(position);
	  (*(*cNode).prev).next = newNode;
	  (*newNode).prev = (*cNode).prev;
	  (*newNode).next = cNode;
      (*cNode).prev = newNode;
      size++;
  }
  else
	  return false;
  
  return true;
}

bool CLinkList::updateNode(int n, int position) { // updates the element at a particular position

   Node *cNode = getNode(position);   // get the node at the given position

   if(cNode != NULL) {
     (*cNode).data = n;       // update the data in the node
      return true;
   }
   else 
	   return false;
}

bool CLinkList::deleteNode(int position)  { // deletes the element in the given position

  Node *cNode;

  if(position == 1 && size > 1) {                // delete element at the start of the list
	    cNode = headNode;                        // where list has more than 1 nodes
		headNode = headNode->next;
		headNode->prev = NULL;
		cNode = NULL;
        size--;

		if(size == 1)
		  lastNode = headNode;
  }
  else if(position == 1 && size == 1) {          // delete element at the start of the list
	    cNode = headNode;                        // where list has just a single 1 node
		headNode = NULL;
		lastNode = headNode;
		cNode = NULL;
        size--;
  }
  else if (position == size) {      // delete element at the end of the list

	  cNode = lastNode;
	  lastNode = lastNode->prev;
	  lastNode->next = NULL;
	  cNode = NULL;
      size--;
  }
  else if (1 < position && position < size) {  // delete element in between the linked list
	  Node *cNode = getNode(position);
	  (cNode->prev)->next = cNode->next;
	  (cNode->next)->prev = cNode->prev;
	  cNode = NULL;
      size--;
  }
  else
	  return false;

  return true;
}

CLinkList::Node* CLinkList::getNode(int pos) {  // get node at a given position
  int mid = getSize()/2; 
  Node *currentNode = NULL;
  int counter;

  if(pos > size)
    return NULL;

  if(pos <= mid) {
	currentNode = headNode;
    counter = 1;

	while(counter <= mid) {  // traverse list from start until the position arrives or until middle of the list
		if(counter == pos)
			break;
		else {
            currentNode = (*currentNode).next;
			counter++;
		}
	}
  }
  else {
	currentNode = lastNode;
    counter = getSize();

	while(counter > mid) {  // traverse list from end until the position arrives or until middle of the list
		if(counter == pos)
			break;
		else {
            currentNode = (*currentNode).prev;
			counter--;
		}
	}
  }
	return currentNode;
}

void CLinkList::show(bool isForward) {  // display linked list
  Node *current;

  if(isForward == true) {
    current = headNode;
    cout << endl << " Forward List" << endl << endl << "  ";
    do {
	    cout << (*current).data << "  ";   // display list from start
	    current = (*current).next;
    }while(current != NULL);
  }
  else{
    current = lastNode;
    cout << endl << endl << " Reverse List" << endl << endl <<  "  ";

    do {
	    cout << (*current).data << "  ";      // display list in reverse
	    current = (*current).prev;
    }while(current != NULL);
  }
}

int CLinkList::getSize(void) {     // get the current size of linked list.
  return size;
}