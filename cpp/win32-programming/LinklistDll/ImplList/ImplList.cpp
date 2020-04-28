// ImplList.cpp : Defines the entry point for the console application.
//

// Assignment : 8
// PRANAV PATIL

// Place the LinkList.dll and LinkList.lib files from the compiled LinkList.cpp code in LinkList folder into the
// ImplList\Debug folder of this project.

#include "stdafx.h"
#include "LinkList.h "
#include < iostream >
#include < limits >     //for numeric_limits
#include < time.h >     // for getting system time

#pragma comment(lib, "LinkList.lib")

using namespace std;

int random(int Min, int Max) {  // Generate random values within min and max range
  int random_integer = -1;
  int range=(Max-Min)+1;
  
  random_integer = Min+int(range*rand()/(RAND_MAX + 1.0)); // generate random value using rand()
  return random_integer;
}

int getData(void) {

   int data = -1;
   cin >> data;

   if(!cin) {                     // clearing the garbage values in inputstream
       cin.clear();                 // reset the stream status flag
       cin.ignore(numeric_limits<streamsize>::max(),'\n'); // skip the rest of the stream data
   }

   return data;
}

int _tmain(int argc, _TCHAR* argv[])
{
   CLinkList list;
   int min=-1, max=-1;

   if(argc == 3) {
     min = _ttoi(argv[1]);        // get min and max values from the command line
     max = _ttoi(argv[2]);        // convert tchar to integer
   }

   if(min > -1 && max > -1 && min < max && (max-min)<1001 )  {
      cout << endl << "Welcome to My Dll Program.." << endl;
	  cout << " Min = " << min << ", Max = " << max << endl;
      int range = max-min;
      int no = -1;

      time_t seconds;              //Declare variable to hold seconds on clock.
      time(&seconds);              //Get value from system clock and place in seconds variable.
      srand((unsigned int) seconds);  //Convert seconds to a unsigned integer.

      for(int index=0; index<range; index++) {
        no = random(0,max);                     // get a random no within the range
	    list.addNode(no);                             // add the no to linked list
      }

      list.show(true);  // display linked list were isForward is True
      list.show(false); // display linked list were isForward is False ie in reverse order

      while(true) {

        int option = -1; 
        int data = -1, pos = -1;
		getchar();
		system("cls");
		cout << endl;
        cout << endl << "-----------------------------------------------------------";
        cout << endl << "  Welcome to Linked List Operations Menu !";
        cout << endl << "-----------------------------------------------------------" << endl;

        cout << endl << " 1) Add Nodes";
        cout << endl << " 2) Update Nodes";
        cout << endl << " 3) Insert Nodes";
        cout << endl << " 4) Delete Nodes";
        cout << endl << " 5) Show Linked List";
        cout << endl << " Enter you choice <Enter 0 to EXIT> : ";
        cin >> option;
        cout << endl;

		if(!cin) {                     // clearing the garbage values in inputstream
          cin.clear();                 // reset the stream status flag
          cin.ignore(numeric_limits<streamsize>::max(),'\n'); // skip the rest of the stream data
        }

        switch (option) {

    	    case 1:
		    	      cout << endl << " Enter the data of the node to add : ";
			    	  data = getData();

   	                  if(data > -1) { 
                        list.addNode(data);     // add node in the linked list.
		              }
		              else { 
                        cerr << " Invalid input values.";
						getchar();
		              }

			          break;
	        case 2:
		    	      cout << endl << " Enter the data of the node to update : ";
			    	  data = getData();
                      cout << endl << " Enter the position of the node to update : ";
		    		  pos = getData();

   	                  if(data > -1 && pos > 0 && pos <= list.getSize()) { 
						  list.updateNode(data,pos);     // update node at the given position in linked list.
		              }
		              else { 
                        cerr << " Invalid input values.";
						getchar();
		              }

					  break;
	        case 3:
			          cout << endl << " Enter the data of the node to insert : ";
    				  data = getData();
                      cout << endl << " Enter the position of the node to insert : ";
		    		  pos = getData();

   	                  if(data > -1 && pos > 0 && pos <= (list.getSize()+1)) { 
			            // allow to specify position greater than size to add elements to the end
                        list.insertNode(data,pos);  // insert node to the given position in the linked list.
		              }
		              else { 
                        cerr << " Invalid input values.";
						getchar();
		              }

		              break;
	        case 4:
                      cout << endl << " Enter the position of the node to delete : ";
		    		  pos = getData();

   	                  if(pos > 0 && pos <= (list.getSize())) { 
						  list.deleteNode(pos);  // delete node to the given position in the linked list.
		              }
		              else { 
                        cerr << " Invalid input values.";
						getchar();
		              }

					  break;
	        case 5:
                      list.show(true);  // display linked list were isForward is True
                      list.show(false); // display linked list were isForward is False ie in reverse order
					  getchar();
    			      break;
	        case 0:
		    	      exit (0);
	        default:
			          break;
        }
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

