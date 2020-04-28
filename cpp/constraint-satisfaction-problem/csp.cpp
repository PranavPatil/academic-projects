//  Project 2 : Centralized Algorithm for Constraint Satisfaction Problem
//

//#include "stdafx.h"           Required when using visual studio
#include < fstream >
#include < iostream >
#include < sstream >
#include < string >
#include < vector >

using namespace std;

 vector<int> varvalnos;
 vector < vector <int> > varvalues;
 vector < vector < vector <int> > > varconsts;
 vector <int> solution;
 int varno = 0;

 int getvarvalue(int var1, int var2, int var1val, int var2val) {

  bool flag = false;
  int j;

  if(0 < varconsts[var1][var2].size()) {
	if(var2val == -1)
	   j = ((int)varvalues[var2].size())*var1val;
	else
	   j = var2val+1;

	for(; j < ((int)varvalues[var2].size())*(var1val+1); j++) {

      if(varconsts[var1][var2][j]== 1)
	  {
		if(varvalues[var2][(j % varvalues[var2].size())] != -1) {
	      flag = true;
          break;
		}
	  }
	}
  }
  else {

    if(var2val == -1)
	 j = 0;
	else
	 j = var2val+1;

	while(varvalues[var2][j] == -1 && j < (int)varvalues[var2].size()) j++;  // select first non deleted value of variable

	if(j < (int)varvalues[var2].size())
	  flag = true;
  }

  if(!flag)
   return -1;
  else
   return j;
 }

int backtrack(int var, int index) { // index is var2 values index in varvalues

 bool startagain;

 if(var < varno-1) {

   int i=0;

   do {

    startagain = false;

	if(solution.empty()) {

	 while(varvalues[var][i] == -1 && i < (int)varvalues[var].size()) i++;  // select first non deleted value of variable

	 if(i >= (int)varvalues[var].size())
		 break;
	}
	else {
      i = index % varvalues[var].size();
	}

	int k = var+1;
	int j = -1;
    bool verifyagain;

	do {

	  verifyagain = false;

	  if((j=getvarvalue(var,k,i,j)) != -1) {

	    // check the solution for the variable with other constraints
		int p = j % varvalues[k].size();
		int r = -1;
		int isvalid = true;

		for(int m=0; m<var; m++) {

			if(0 < (int)varconsts[m][k].size()) {

			  r = ((int)varvalues[k].size())*solution[m];

			  if(varconsts[m][k][r+p] != 1) {
				  isvalid = false;
				  break;
			  }
			}
	    }

		if(isvalid) {

		 if(var+1 > (int)solution.size())
	      solution.push_back(i);

		 if(var+1 < varno-1) {
			int bno=backtrack(var+1, j);

			  if(bno > -1) {
				  verifyagain = true;
				  j = bno;
			  }
			  else if(bno == -1)
				  return -1;
	     }
		 else {
           solution.push_back(j%varvalues[k].size());
		   return -1;
		 }
		}
		else {
           verifyagain = true;
		}
	  }
	  else {
        verifyagain = true;
	  }
	}while(verifyagain && j != -1 && j < ((int)varvalues[var].size())*(i+1));

	if(verifyagain) {

	 if(var < (int)solution.size())
      solution.pop_back();

	 if(var > 0)
	   return index;
	 else {
	   startagain = true;
	   i++;
	 }
	}

   }while(startagain && i < (int)varvalues[var].size());

   if(startagain) {
	 cout << " No Solution Exists !!" << endl;
   }

   return -1;
 }
 else
   return -1;
}

int main(int argc, char* argv[])
{
  if(argc < 2) {
	  cout << endl << "  USAGE:  windows: csp inputfile" << endl;
	  cout << endl << "          linux  : ./csp inputfile" << endl;
	  return -1;
  }

  ifstream file;
  file.open(argv[1]);

  if (!file){
   cout << endl << " Error in opening file...";
   return EXIT_FAILURE;
  }

  string lineread;
  bool isVarInit = false;
  bool isConInit = false;

  int varcount = 0;

  int conno = 0;
  int concount = 0;
  int convars = 0;
  int rowiter = 0, coniter = 0;
  int var1 = -1, var2 = -1;

  while(std::getline(file, lineread)) // Read line by line
  {
	if(!isVarInit) {
	  if(varno == 0) {

	    varno = atoi(lineread.c_str());
		varvalnos.resize(varno);
        varvalues.resize(varno);
	  }
	  else if(varcount < varno) {

	    int pos = lineread.find_first_of('v');

	    int var = atoi(lineread.substr(pos+1, 2).c_str());

		varvalnos[var] = atoi(lineread.substr(pos+3, 2).c_str());
		varvalues[var].resize(varvalnos[var]);

	    pos = pos + 2;

	    for(int i =0; i < varvalnos[var]; i++) {

	      pos = pos + 2;
	      varvalues[var][i] = atoi(lineread.substr(pos+1, 2).c_str());
	    }
		varcount++;
	  }
	  else {
			isVarInit = true;
	  }
	}
	else if(isVarInit && !isConInit){

	  if(conno == 0) {
	    conno = atoi(lineread.c_str());

        varconsts.resize(varno);

		for(int i=0; i<varno; i++) {
          varconsts[i].resize(varno);
		}
	  }
	  else if(concount < conno) {

  		if(convars == 0) {    // convars is no of variables for each constaint

			if((convars=atoi(lineread.c_str()))!= 2) {  // no of vars in constraint could be only 2
			  cout << endl << " Invalid number of constraints !!!" << endl;
			  isConInit = true;
		    }
			var1 = -1;

		}
		else if(var1 == -1) { // get variable 1 for constraint
	       var1 = atoi(lineread.substr(lineread.find_first_of('v')+1).c_str());
  		   var2 = -1;
		}
		else if(var2 == -1) { // get variable 2 for constraint
           var2 = atoi(lineread.substr(lineread.find_first_of('v')+1).c_str());
	       varconsts[var1][var2].resize(varvalnos[var1]*varvalnos[var2]);
		   rowiter = 0;
		   coniter = 0;
		}
		else if(rowiter < varvalnos[var1]) {  // rowiter counts the no of rows parsed from constraint matrix

		   istringstream iss(lineread);
		   int coliter = 0;  // coliter counts no of columns parsed from constraint matrix

	       while (!iss.eof() && coliter < varvalnos[var2])  {
             string sub;
             iss >> sub;

			 varconsts[var1][var2][coniter] = atoi(sub.c_str());

			 coliter++;
			 coniter++;
           }
		   rowiter++;
		}
		else {
		   convars = 0;
		   concount++;
		}
	  }
	}
  }

  for(int i=0; i<varno; i++) {

	  for(int j=0; j<varno; j++) {

		if(varconsts[i][j].size() != 0) {

		    int a=0, b=0; // b is the iterator index of vector v
			vector<int> v(varvalues[j].size()); // used to check sum of column values

			for(int k=0; k<(int)varconsts[i][j].size(); k++) {

			  a = a + varconsts[i][j][k];

			  v[b] = v[b] + varconsts[i][j][k];
			  b++;

			  if(((k+1)% varvalues[i].size()) == 0) {

				  if(a == 0) {

				    if(varvalues[i][((int)((k+1)/varvalues[i].size())) - 1] != -1) {
					   varvalues[i][((int)((k+1)/varvalues[i].size())) - 1] = -1; // delete values
					   varvalnos[i]--;
					}

					if(varvalnos[i] < 1) {
						  cout << endl << " No Solution Exits !!" << endl;
						  exit(0);
			        }
				  }
                  a = 0;
				  b = 0;
			  }
			}

			for(int h=0; h<(int)varvalues[j].size(); h++) {
			  if (v[h] == 0) {
				  if(varvalues[j][h] != -1) {
				    varvalnos[j]--;
				    varvalues[j][h] = -1; // delete values
				  }

				  if(varvalnos[i] < 1) {
					  cout << endl << " No Solution Exists !!" << endl;
					  exit(0);
				  }
			  }
			}
		  }
	  }
  }

  backtrack(0,-1);  // start backtracking initial variable is 0 and its value unknown ie -1 default

  cout << endl;

  for(int i=0; i < (int)solution.size(); i++) {
	  cout << "var" << i << "  " << varvalues[i][(solution[i])] << endl;
  }

  return 0;
}
