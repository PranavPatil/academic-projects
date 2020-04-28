
// Assignment No : 8
// PRANAV PATIL


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LINKLIST_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LINKLIST_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LINKLIST_EXPORTS
#define LINKLIST_API __declspec(dllexport)
#else
#define LINKLIST_API __declspec(dllimport)
#endif

// This class is exported from the LinkList.dll
class LINKLIST_API CLinkList {
public:
	CLinkList(void);
	// TODO: add your methods here.
     void addNode(int);      // add elements to the list.
     bool insertNode(int,int);  // insert elements to a particular position in the list.
	 bool updateNode(int, int); // updates the element at a particular position
	 bool deleteNode(int);  // deletes the element in the given position
     void show(bool);     // display the nodes in linked list both in ascending and descending order.
     int getSize(void);     // get the current size of linked list.

  private:          
         class Node {         // 
           public:
	             int data;           // data of the node.
		         Node *next;         // next element in the linked list.
				 Node *prev;         // previous element in the linked list.
				 friend class List;  // Node is friend class of List.
         };
	  
	     int size;            // no of elements in the linked list.
         Node *headNode;      // first node of the linked list.
         Node *lastNode;      // last node of the linked list.
		 Node* getNode(int);  // get the pointer to the node at a given position in the linked list.
};
