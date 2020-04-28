
//  DBMS EXTRA CREDIT PROJECT

// DBConnect.cpp : Defines the entry point for the console application.
//
// Program to execute all the Queries.sql Queries for x no of times randomly such as
// every query is executed each time.


#include "stdafx.h"
#include < windows.h >
#include "database.h"
#include < time.h >     // for getting system time
#include <vector>

# define LOAD_BUFFER_SIZE 82
# define TOTAL_QUERIES 24

using namespace std;

// Buffer to store the Queries
vector<char *> queries;
vector<int> queryno;  // get the queries which are remaining to execute
vector<int>::iterator qry_Iter;
int counter[TOTAL_QUERIES];   // counter to track no of executions
float exectime[TOTAL_QUERIES-2];  // store query execution time

// Database Connection String
char CnnStr[100]= "Driver={SQL Server};Server=MY-PC\\SQLEXPRESS;"
                  "Database=tpch;Uid=pranav;Pwd=password;";
char ErrStr[200];
Database db;

// Variables needed to parse the file and load the queries
bool isQueryComplete = true;
bool isLoopingFactor = false;
bool isQueryRems = false;
int iterations = 0;

float TimeDiff(void)
{
  // Last counter reading
  static LARGE_INTEGER OldCounter = {0, 0};

  LARGE_INTEGER Counter, Frequency;
  if (QueryPerformanceFrequency(&Frequency))
  {
    // Gets current counter reading
    QueryPerformanceCounter(&Counter);

    // Calculates time difference (zero if called first time)
	float TimeDiff = OldCounter.LowPart ? (float) (Counter.LowPart - OldCounter.LowPart) / Frequency.LowPart : 0;

    // Resets last counter reading
    OldCounter = Counter;

    // Returns time difference
    return TimeDiff;
  }
  else
  {
    // No high resolution performance counter; returns zero
    return 0;
  }
}
// function to initialize databse
bool InitializeDatabase()
{
  try {
  	::CoInitialize(NULL);

	cout << endl << " Connection : ";

	if(!db.Open("pranav","patil123",CnnStr)) {
		db.GetErrorErrStr(ErrStr);
		cout << ErrStr << endl;
	}
  }
  catch(...)  {
    cout << "Unknown failure" << endl;
    return false;
  }

  return true;
}

// function to release database connections
bool releaseDatabase() {

  ::CoUninitialize();
  return true;
}

 void ShowError() {
  LPVOID  hlocal = NULL;
  DWORD dw = GetLastError();  // get the last error code
  cout << "Error ------------ " << dw;

  try {
	  // use formatmessage to get the error code message
	  wchar_t buf[1000];
	  hlocal = &buf;
    DWORD ok = FormatMessage(
          FORMAT_MESSAGE_ALLOCATE_BUFFER |
          FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
          NULL,
          dw,
          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
          (LPWSTR) hlocal,
          0, NULL );

    if(ok)
		wcout << L"Error : " << (LPSTR)hlocal;
    else
		cerr << "Unknown Error : " << GetLastError();
  } catch(exception e) {
		cerr << "Unknown Error";
  }
 }

 // function to parse queries from the file
 void getQuery(char * buffer) {

  char * query = NULL;

  if(isQueryComplete == true) {

	  if(isQueryRems) {
         char * remQuery = queries.back();
         queries.pop_back();
         int total = strlen(remQuery) + strlen(buffer) + 2;
         char* tp = (char *) malloc(sizeof(char)* total);
         memset(tp, 0, total);
         strncpy_s(tp, total-2, remQuery, strlen(remQuery));
         strcat_s(tp, total-1, buffer);
	     buffer = tp;
		 isQueryRems = false;
	  }

	 query = strstr(buffer ,"Create");  // get first char

	 if(query == NULL)
	   query = strstr(buffer ,"Select");  // get first char

	 if(query == NULL)
       query = strstr(buffer ,"Drop");  // get first char

	 if(query != NULL) {
	   isQueryComplete = false;

	   if(!isLoopingFactor) {
         char* temp = _strdup(query);
         query = temp;
	   }
	 }
  }
  else {

	 query = queries.back();
     queries.pop_back();

     int total = strlen(query) + strlen(buffer) + 2;
     char* tp = (char *) malloc(sizeof(char)* total);
     memset(tp, 0, total);
     strncpy_s(tp, total-2, query, strlen(query));
     strcat_s(tp, total-1, buffer);
	 query = tp;
  }

  char * qry = NULL;

  if(query != NULL)
    qry = strstr(query, "Go");

  if(qry != NULL) {

	 int no = strlen(query) - strlen(qry);
	 int total = (no) + 2;
     char* final = (char *) malloc(sizeof(char)*(total));
     memset(final, 0, total);
     strncpy_s(final, total-1, query, no);
     queries.push_back(final);
     isQueryComplete = true;
	 isLoopingFactor = true;
     getQuery(qry);
  }
  else {

	 if(query != NULL)
	  queries.push_back(query);
	 else {
	  queries.push_back(buffer);  // case where part of SELECT clause is present in buffer.
	  isQueryRems = true;         // Tell select block to consider contents in queries too.
	 }
  }
}

 int random(int Min, int Max) {  // Generate random values within min and max range
  int random_integer = -1;
  int range=(Max-Min)+1;

  random_integer = Min+int(range*rand()/(RAND_MAX + 1.0)); // generate random value using rand()
  return random_integer;
}

 // function to load the queries from the database
 bool LoadQueries() {

   HANDLE hFile;
   DWORD dwBytesRead = 0;
   char ReadBuffer[LOAD_BUFFER_SIZE] = {0};

   // Code to get the application path and get the full path to load the file
   TCHAR SQL_FILE[MAX_PATH];
   GetModuleFileName(NULL, SQL_FILE, sizeof(SQL_FILE));
   TCHAR * temp = _tcsrchr(SQL_FILE, TEXT('\\')) + 1;
   int len = _tcslen(temp) * sizeof(TCHAR);
   _tcscpy_s(temp,  len, TEXT("Queries.sql"));

   hFile = CreateFile(SQL_FILE,               // file to open
                       GENERIC_READ,          // open for reading
                       FILE_SHARE_READ,       // share for reading
                       NULL,                  // default security
                       OPEN_EXISTING,         // existing file only
                       FILE_ATTRIBUTE_NORMAL, // normal file
                       NULL);                 // no attr. template

    if (hFile == INVALID_HANDLE_VALUE)
    {
        cout << "Could not open file :(error " << GetLastError() << ")" << endl;
        return false;
    }

    // Parse the file
	// Read one character less than the buffer size to save room for
    // the terminating NULL character.

	DWORD filesize =  GetFileSize( hFile, NULL);
    DWORD totalbytes = 0;

	while(totalbytes < filesize) {

      if((filesize-totalbytes) < LOAD_BUFFER_SIZE-1)
		memset(ReadBuffer, 0, LOAD_BUFFER_SIZE);

	  if( FALSE == ReadFile(hFile, ReadBuffer, LOAD_BUFFER_SIZE-1, &dwBytesRead, NULL) )  {
		ShowError();
        CloseHandle(hFile);
        return false;
      }

      if (dwBytesRead > 0)  {
		totalbytes = totalbytes + dwBytesRead;
		isLoopingFactor = false;
		getQuery(ReadBuffer);
      }
      else  {
		cout << "No data read from file : " << endl << totalbytes << " : " << filesize;
      }
	}

    CloseHandle(hFile);
	return true;
 }

int _tmain(int argc, _TCHAR* argv[])
{
   bool status = true;
   char buf[10];

   cout << endl << "  ---------------------------------------------------------------------";
   cout << endl << "      Database Systems : TPCH Specs (Extra Credit Assignment)";
   cout << endl << "  ---------------------------------------------------------------------";
   cout << endl << endl;

   while(iterations == 0) {

     errno = 0;
	 cout << endl << " Enter no of iterations to execute the Queries : ";
     cin >> buf;
	 iterations = atoi(buf);
   }

   status = InitializeDatabase();

   if(status)
    status = LoadQueries();

   if(status) {

	for(int i = 0; i < TOTAL_QUERIES; i++) {
        counter[i] = iterations;

		if(i != 15 && i != 16)                              // special case query 15
	      queryno.push_back(i);
     }

     int loc_no = 0, query_no = 0;
	 float qrytime = 0;

	 time_t seconds;              //Declare variable to hold seconds on clock.
     time(&seconds);              //Get value from system clock and place in seconds variable.
     srand((unsigned int) seconds);  //Convert seconds to a unsigned integer.
	 int no = 1;

	 try {

		 cout << endl << endl << endl << "  Sr No\t\tQuery No" << "\t\tTime in Secs" << endl;

	   while(!queryno.empty()) {
		   loc_no = random(0, queryno.size()-1); // get a random number
		   query_no = queryno[loc_no];

	      if(counter[query_no] > 0) {

			if(query_no > 16)
			  cout << endl << "  " << no++ << "\t\t  " << query_no-1 << "\t\t\t  ";
			else if(query_no < 15)
			  cout << endl << "  " << no++ << "\t\t  " << query_no+1 << "\t\t\t  ";

            // open the dataset
	        TimeDiff();

			if(!db.Execute(queries[query_no]))  {
		      db.GetErrorErrStr(ErrStr);
		      cout<< ErrStr << endl;
	        }

			if(query_no == 14) {

				if(!db.Execute(queries[query_no+1]))  {
		        db.GetErrorErrStr(ErrStr);
		        cout<< ErrStr << endl;
	          }

			  (counter[query_no+1])--;

			  if(!db.Execute(queries[query_no+2]))  {
		        db.GetErrorErrStr(ErrStr);
		        cout<< ErrStr << endl;
	          }

			  (counter[query_no+2])--;
			}

			qrytime = TimeDiff();
			exectime[query_no] = exectime[query_no] + qrytime;

			if(query_no != 15 && query_no != 16)
			  cout << qrytime;
			//cout << endl << "    " << query_no+1 << "\t\t   " << qrytime;

		    (counter[query_no])--;
	      }
		  else {

		    qry_Iter = queryno.begin();

			while(qry_Iter != queryno.end()) {
				if(*qry_Iter == query_no)
					break;
               qry_Iter++;
            }

			queryno.erase(qry_Iter);
		  }
	   }
	 }
	 catch(...) {
       cout << "Unknown failure" << endl;
       return -1;
     }

	 float total = 0, totalavg = 0;

	 cout << endl << endl;

	 for(int j=0; j<TOTAL_QUERIES; j++) {

		 total = total + exectime[j];

		 if(j != 15 && j != 16) {

			 if(j > 16)
		       cout << endl << "Average Time Query " << (j-1) << " : "
		 	        << exectime[j]/ iterations << " secs";
			 else
		       cout << endl << "Average Time Query " << (j+1) << " : "
		 	        << exectime[j]/ iterations << " secs";
		 }
	 }

	 cout << endl << endl << endl << "Total Time : " << total  << " secs" << endl;
	 cout << endl << endl << "Average Total Time : " << total / ((TOTAL_QUERIES-2) * iterations)
		  << " secs" << endl;
   }

   cout << endl << endl << endl << "Press <ENTER> to Exit........";
   gets_s(buf, 10);

   return 0;
}
