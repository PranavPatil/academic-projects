// dblprog.cpp : Defines the entry point for the console application.
// Assisgment 1 : Pranav Patil.

#include "stdafx.h"
#include < iostream >
#include < limits >     //for numeric_limits
#include < time.h >     // for getting system time

using namespace std;
const int Forward = 0;   // const value to show ascending order.
const int Reverse = 1;   // const value to show descending order.


class List {
  public:
          List( );            // constructor
          void add(int);      // add elements to the list.
		  void insert(int,int);  // insert elements to a particular position in the list.
          void show(int);     // display the nodes in linked list both in ascending and descending order.
		  int getSize() {     // get the current size of linked list.
			  return size;
		  };
  private:          
         class Node {         // 
           private:
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

List::List( ) {  // constructor
  headNode = NULL;  // initialize all field to default values
  lastNode = NULL;
  size = 0;
};

void List::add(int ndata) {  // add element to linked list
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
};

void List::insert(int n,int position) { // insert element at specified position in the list
  Node *newNode;
  newNode = new Node();              // create the node or element
  (*newNode).data = n;

  if(position == 1) {                // insert element at the start of the list
        (*newNode).next = headNode;
        (*headNode).prev = newNode;
        (*newNode).prev = NULL;
        headNode = newNode;
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
	  cerr << endl << "Invalid Input";
};

List::Node* List::getNode(int pos) {  // get node at a given position
  int mid = getSize()/2; 
  Node *currentNode = NULL;
  int counter;

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
};

void List::show(int order) {  // display linked list
  Node *current;

  if(order == Forward) {
    current = headNode;
    cout << endl << " Forward List" << endl << endl << "  ";
    do {
	    cout << (*current).data << "  ";   // display list from start
	    current = (*current).next;
    }while(current != NULL);
  }
  else if (order == Reverse) {
    current = lastNode;
    cout << endl << endl << " Reverse List" << endl << endl <<  "  ";

    do {
	    cout << (*current).data << "  ";      // display list in reverse
	    current = (*current).prev;
    }while(current != NULL);
  }
}

int random(int Min, int Max) {  // Generate random values within min and max range
  int random_integer = -1;
  int range=(Max-Min)+1;
  
  random_integer = Min+int(range*rand()/(RAND_MAX + 1.0)); // generate random value using rand()
  return random_integer;
}

int _tmain(int argc, _TCHAR* argv[])
{
   List list;
   int min=-1, max=-1;

   if(argc == 3) {
     min = _ttoi(argv[1]);        // get min and max values from the command line
     max = _ttoi(argv[2]);        // convert tchar to integer
   }

   if(min > -1 && max > -1 && min < max && (max-min)<1001 )  {
      cout << endl << "Welcome to Dr Ford's Windows Progamming Course.." << endl;
      cout << "Executing Assignment 1...." << endl;
	  cout << " Min = " << min << ", Max = " << max << endl;
      int range = max-min;
      int no = -1;
	  char input;

      time_t seconds;              //Declare variable to hold seconds on clock.
      time(&seconds);              //Get value from system clock and place in seconds variable.
      srand((unsigned int) seconds);  //Convert seconds to a unsigned integer.

      for(int index=0; index<range; index++) {
        no = random(0,max);                     // get a random no within the range
	    list.add(no);                             // add the no to linked list
      }

      list.show(Forward);  // display linked list
      list.show(Reverse);

      cout << endl << endl << "Do u want to insert more elements in the linked list ? (hit y for yes): ";
	  cin >> input;
	  
	  while(input == 'Y' || input == 'y'){
	    int data = -1, pos = -1;
		cout << endl << " Enter the data of the node to add : ";
	    cin >> data;

	    if(!cin) {                     // clearing the garbage values in inputstream
          cin.clear();                 // reset the stream status flag
		  cin.ignore(numeric_limits<streamsize>::max(),'\n'); // skip the rest of the stream data
        }

		cout << endl << " Enter the position of the node in the Linked List : ";
	    cin >> pos;

	    if(!cin) {                      // clearing the garbage values in inputstream
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(),'\n');
        }

	    if(data > -1 && pos > 0 && pos <= (list.getSize()+1)) { 
			// allow to specify position greater than size to add elements to the end
          list.insert(data,pos);     // add node to the given position in the linked list.
          list.show(Forward);   // display linked list
          list.show(Reverse);
		}
		else {
          cerr << " Invalid input values.";
		}
		
		cout << endl << endl << "Do u want to insert more elements in the linked list ? (hit y for yes): ";
		cin >> input;
	  }
   }
   else {   // error handling
	   if(argc != 3) 
         cerr << endl << " Invalid number of input parameters." << endl;
	   else if(min < 0) 
         cerr << endl << " Invalid min parameter." << endl;
	   else if(max < 0) 
         cerr << endl << " Invalid max parameter." << endl;
	   else if(min >= max)
         cerr << endl << " Invalid input, max is not greater than min parameter." << endl;
	   else if((max-min) > 1000) 
         cerr << endl << " Invalid input, range between min and max exceeds 1000." << endl;
	   else
	     cerr << endl << " Unknown exception." << endl;
   }

   return 0;
}
