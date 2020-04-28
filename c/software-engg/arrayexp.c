/*
*******************************************************************************************
 SWE 5001 Software Engineering 1
 Assignment 4 Part 2
 September 12, 2010

********************************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define EXPRMAXSIZE 100
#define VARIABLE 1
#define NUMBER 2
#define ADD 3
#define SUBSTRACT 4
#define MULTIPLY 5
#define DIVIDE 6
#define ASSIGNMENT 7

typedef struct Symbol Symbol;
typedef struct Tree Tree;
typedef struct DoubleArray DoubleArray;

struct Symbol {
	double *values;
	char *name;
	int novals;
};

struct Tree {
	int op;
	double value ;
	Symbol *symbol ;
	Tree *left;
	Tree *right;
};

struct DoubleArray {
	int size;
	double *values ;
};


typedef struct LNode List ;

struct LNode {
  char* identifier;
  double* values;
  int novals;
  List *next;  // node
};

void parse(char**, int, List*, Tree**);
DoubleArray* builtTree(Tree*, Tree **, int);
void inorder(Tree*);
void postorder(Tree*);
DoubleArray* eval (Tree*);

// create new item from value
List *newitem (char*, double[], int);

// add newp to front of listp
List *addfront (List*, List*);

// add newp to end of listp
List *addend (List* , List *newp);

// sequential search for identifier in the listp
List *lookuplist (List*, char*);

// print values using format in arg
void dprintlist (List*);

// write values using format in arg in a file
void dwritelist (List*, char*);

// free all elements of listp
void freeall (List*);

DoubleArray* operate(DoubleArray*, DoubleArray*, int);
int noofvars = 0;

int  main()
{
   char inputExpression[EXPRMAXSIZE]; /* or other suitable maximum line size */
   int thislen = 0;
   char* beginning;
   char* expression;
   char* quit = "quit\n";

   Tree *parseTree[EXPRMAXSIZE];                      //Tree Structure  object
   int totaltokens = 0;
   Tree* bTree = NULL;
   char* next;
   char* characters[EXPRMAXSIZE];
   List* symbolTable = NULL;
   List* item = NULL;

   char filename[100];
   printf("Enter the Input File name: ");
   scanf ("%s",&filename);
   char outfile[100];
   printf("Enter the Output File name: ");
   scanf ("%s",&outfile);

   FILE *file = fopen ( filename, "r" );

   while(file != NULL && fgets ( inputExpression, sizeof inputExpression, file ) != NULL )
   {
		thislen = strlen(inputExpression);
	    expression = (char*) malloc (thislen);
		memset (expression,'\0',(thislen));

        if (inputExpression[thislen-1] == '\n')
        {
	        inputExpression[thislen-1] = '\0';
        }

		strcpy (expression,inputExpression);    // creates a copy of the expression
		beginning = strtok(inputExpression," ");     // retrieves the first word of the expression

        if(beginning != NULL && strcmp(beginning,"let") != 0)               // validates it as "let"
        {
		    printf("\nExpression must start with let\n");
            fclose ( file );
            return 0;
        }

		next = NULL;
		next = strtok(expression," ");
		totaltokens = 0;

        while(next != NULL)                        // Expression tokenized and stored in characters array.
        {
		   if(strcmp(next,"let")!=0)
		   {
 		     characters[totaltokens] = next;
  		     totaltokens++;
		   }
		   next = strtok(NULL," ");
		}

		if(totaltokens > 3 && strrchr(characters[2], ',') != NULL)
		{
          double* values = (double*) malloc ((totaltokens-2) * sizeof(double));
		  memset(values, 0, (totaltokens-2) * sizeof(double));
		  char* ptr = NULL;
		  int k = 0;

		  for(k=2; k < totaltokens; k++)
		  {
			 ptr = strrchr(characters[k], ',');

			 if(ptr == NULL && strlen(characters[k]) > 0) {           // string is '2.3'
			    values[k-2] = atof(characters[k]);
			 }
			 else if(ptr == NULL && strlen(characters[k]) <= 0) {     // string is ''
			    values[k-2] = 0;
			 }
			 else if(strlen(ptr) == 1 && strlen(characters[k]) == 1) {  // string is ','
			    values[k-2] = 0;
			 }
			 else if(strlen(ptr) == 1  && strlen(characters[k]) > 1) {  // string is '2.3,'
 			    char* temp = (char*) calloc (strlen(characters[k])-1,sizeof(char));
			    strncpy (temp, characters[k], strlen(characters[k])-1);
			    values[k-2] = atof(temp);
			 }
			 else if(strlen(ptr) == strlen(characters[k]) && strlen(characters[k]) > 1) {  // string is ',2.3'
			    values[k-2] = atof(characters[k]+1);
			 }
			 else {
			    values[k-2] = 0;
			 }
		  }

          if(symbolTable == NULL) {                                                  // intialize symbol table
     	       symbolTable = newitem(characters[0], values, totaltokens-2);
		  }
		  else {
	        item = NULL;                                                               // add entries to symbol table
            item = newitem(characters[0], values, totaltokens-2);
	        symbolTable = addend(symbolTable, item);
	      }
		}
		else if(totaltokens > 3 && strrchr(characters[2], ',') == NULL)
		{
		  parse(characters, totaltokens, symbolTable, parseTree);                       //parses the expression based on rules given in assignment.

		  DoubleArray* arry = NULL;
		  arry = builtTree(bTree, parseTree, totaltokens);

          if(symbolTable == NULL) {                                                  // intialize symbol table
     	       symbolTable = newitem(characters[0], arry -> values, arry -> size);
		  }
		  else {
	        item = NULL;                                                               // add entries to symbol table
            item = newitem(characters[0], arry -> values, arry -> size);
	        symbolTable = addend(symbolTable, item);
	      }
		}
		else if(totaltokens == 3)
		{
          double* values = (double*) malloc (sizeof(double));
		  memset(values, 0, sizeof(double));
		  values[0] = atof(characters[2]);

          if((isalpha(*characters[0]) != 0) && (strcmp(characters[1],"=") == 0) && (isdigit(*characters[2]) != 0))  // symbols loaded in symbol table
		  {
			if(symbolTable == NULL) {                                                  // intialize symbol table
     	       symbolTable = newitem(characters[0], values, 1);
		    }
			else {
			   item = NULL;                                                               // add entries to symbol table
       	       item = newitem(characters[0], values, 1);
	           symbolTable = addend(symbolTable, item);
			}
		  }
		  else
			  printf("\n Invalid Expression");
		}
		else
		  printf("\n Invalid Expression");

  	    printf("\nSymbol Table");
		dprintlist(symbolTable);   // print symbol table
		dwritelist(symbolTable, outfile);   // print symbol table

		printf("\n>");
    }

	if(file != NULL)
      fclose ( file );

    return 0;
}

// This function parses the input expression.
void parse(char** characters, int totaltokens, List* symbolTable, Tree** parseTree)
{
        int i = 0;
		Symbol* thisCharacter = NULL;             // Symbol structure object
		Tree *t = NULL;
		int token = 0;
		List* tempNode = NULL;
		double* temp = NULL;

		while(i<totaltokens)                               // Parse rules used for insertion of tree elements into the tree structure.
		{
		    if(isalpha(*characters[i]))
			   token = VARIABLE;
			else if(isdigit(*characters[i]))
			   token = NUMBER;
			else if(strcmp(characters[i],"+")== 0)
			   token = ADD;
			else if(strcmp(characters[i],"-")== 0)
			   token = SUBSTRACT;
			else if(strcmp(characters[i],"*")== 0)
			   token = MULTIPLY;
			else if(strcmp(characters[i],"/") == 0)
			   token = DIVIDE;
			else if(strcmp(characters[i],"=") == 0)
			   token = ASSIGNMENT;

            switch(token)
			{
			    // check operator and perform corresponding operation
                case VARIABLE:
					{
							thisCharacter = (Symbol*) malloc(sizeof(Symbol));
							memset(thisCharacter, 0, sizeof(Symbol));
							t = (Tree*) malloc(sizeof(Tree));
							memset(t, 0, sizeof(Tree));
							thisCharacter -> name = characters[i];
							tempNode = NULL;
							tempNode = lookuplist (symbolTable, characters[i]);
							if(tempNode != NULL) {
							  thisCharacter -> values = tempNode -> values;
							  thisCharacter -> novals = tempNode -> novals;
							  t -> value = 0;
							}
							else {
							  thisCharacter -> values = 0;
							  t -> value = 0;
							}

							t -> op = VARIABLE;
							t -> symbol= thisCharacter;
							t -> left = NULL;
							t -> right = NULL;
							parseTree[i] = t;
							i++;
							break;
					}
				case  NUMBER:
					{
							thisCharacter = (Symbol*) malloc(sizeof(Symbol));
							memset(thisCharacter, 0, sizeof(Symbol));
                            temp = (double*) malloc (sizeof(double));
                            memset(temp, 0, sizeof(double));
                            temp[0] = atof(characters[i]);
            			    thisCharacter -> values = temp;
							thisCharacter -> novals = 1;
							thisCharacter -> name = characters[i];
							t = (Tree*) malloc(sizeof(Tree));
							memset(t, 0, sizeof(Tree));
							t -> op = NUMBER;
							t -> value = atof(characters[i]);
							t -> symbol= thisCharacter;
							t -> left = NULL;
							t -> right = NULL;
							parseTree[i] = t;
							i++;
							break;
				 }
				case ADD:
				 {
							  thisCharacter = (Symbol*) malloc(sizeof(Symbol));
							  memset(thisCharacter, 0, sizeof(Symbol));
							  t = (Tree*) malloc(sizeof(Tree));
							  memset(t, 0, sizeof(Tree));
							  thisCharacter -> name = characters[i];
							  thisCharacter -> values = NULL;
							  thisCharacter -> novals = 0;
							  t -> op = ADD;
							  t -> value = 0;
							  t -> symbol = thisCharacter;
							  t -> left = NULL;
							  t -> right = NULL;
							  parseTree[i] = t;
							  i++;
							  break;

				 }
				case SUBSTRACT:
				 {
							  thisCharacter = (Symbol*) malloc(sizeof(Symbol));
							  memset(thisCharacter, 0, sizeof(Symbol));
							  t = (Tree*) malloc(sizeof(Tree));
							  memset(t, 0, sizeof(Tree));
							  thisCharacter -> name = characters[i];
							  thisCharacter -> values = NULL;
							  thisCharacter -> novals = 0;
							  t -> op = SUBSTRACT;
							  t -> value = 0;
							  t -> symbol = thisCharacter;
							  t -> left = NULL;
							  t -> right = NULL;
							  parseTree[i] = t;
							  i++;
							  break;

				 }
				case MULTIPLY:
				 {
							  thisCharacter = (Symbol*) malloc(sizeof(Symbol));
							  memset(thisCharacter, 0, sizeof(Symbol));
							  thisCharacter -> name = characters[i];
							  thisCharacter -> values = NULL;
							  thisCharacter -> novals = 0;
							  t = (Tree*) malloc(sizeof(Tree));
							  memset(t, 0, sizeof(Tree));
							  t -> op = MULTIPLY;
							  t -> value = 0;
							  t -> symbol = thisCharacter;
							  t -> left = NULL;
							  t -> right = NULL;
							  parseTree[i] = t;
							  i++;
							  break;
				 }
				case DIVIDE:
				 {
							 thisCharacter = (Symbol*) malloc(sizeof(Symbol));
							  memset(thisCharacter, 0, sizeof(Symbol));
							  thisCharacter -> name = characters[i];
							  thisCharacter -> values = NULL;
							  thisCharacter -> novals = 0;
							  t = (Tree*) malloc(sizeof(Tree));
							  memset(t, 0, sizeof(Tree));
							  t -> op = DIVIDE;
							  t -> value = 0;
							  t -> symbol = thisCharacter;
							  t -> left = NULL;
							  t -> right = NULL;
							  parseTree[i] = t;
							  i++;
							  break;
				 }
				case ASSIGNMENT:
				 {
							  thisCharacter = (Symbol*) malloc(sizeof(Symbol));
							  memset(thisCharacter, 0, sizeof(Symbol));
							  t = (Tree*) malloc(sizeof(Tree));
					          memset(t, 0, sizeof(Tree));
							  thisCharacter -> name = characters[i];
							  thisCharacter -> values = NULL;
							  thisCharacter -> novals = 0;
							  t -> op = ASSIGNMENT;
							  t -> value = 0;
							  t -> symbol = thisCharacter;
							  t -> left = NULL;
							  t -> right = NULL;
							  parseTree[i] = t;
							  i++;
							  break;
				 }
				 default:
				 {
					  printf("\nExpression contains illegal characters");
					  break;
				 }
	   	    }
	    }


		if(parseTree[0] != NULL)                // checks if the parse tree has valid elements and displays them as specified.
		{
	    	 for(i = 0;i<totaltokens;i++)
			 {
				if(parseTree[i] != NULL && parseTree[i] -> symbol != NULL)
	        	printf("\n%s \n", (parseTree[i] -> symbol)-> name);
				else if(parseTree[i] != NULL && parseTree[i] -> symbol == NULL)
	        	printf("\n%g \n", parseTree[i] -> value);
			 }
	    }
		return;
 }

 // creates a single tree from the array of tree depending on the precedence of operators
 DoubleArray* builtTree(Tree* bTree, Tree** parseTree, int totalTokens)
 {
	 int i = 0;
	 int opcode = 0;
	 Tree* currentHead = NULL;
	 Tree* prevHead = NULL;
     bTree = NULL;

	 while(i < totalTokens)
	 {
		 opcode = parseTree[i] -> op;

		 if(opcode == VARIABLE || opcode == NUMBER)
		 {
		   if(bTree == NULL)
			   bTree = parseTree[i];
		   else if( i > 1)
		   {
			   if(currentHead -> left == NULL)
				   currentHead -> left = parseTree[i];
			   else if(currentHead -> right == NULL)
				   currentHead -> right = parseTree[i];
			   else
			   {
				   if(opcode == VARIABLE)
				     printf("\n No Operator for the Variable %s", parseTree[i] -> symbol -> name);
				   else
				     printf("\n No Operator for the Number %s", parseTree[i] -> symbol -> name);
			       return;
			   }
		   }
		   else
		   {
			  printf("\n Invalid occurrence of Variable or Number");
			  return;
		   }
		 }
		 else if(opcode == ADD || opcode == SUBSTRACT)
		 {
           if(i < 2)
		   {
			   if(opcode == ADD)
			     printf("\n Invalid occurrence of Addition Operator");
			   else
			     printf("\n Invalid occurrence of Subtraction Operator");

			   return;
		   }
		   else
		   {
			   if(currentHead -> left == NULL)
			   {
				   printf("\n No Variable Occurred Before the Operator");
			       return;
			   }
			   else if(currentHead -> right == NULL)
			   {
				   printf("\n No Variable Occurred After the Operator");
			       return;
			   }
			   else if(currentHead -> op == MULTIPLY ||              /// shift + or - operator on top
			      currentHead -> op == DIVIDE)
			   {
			      if(currentHead -> right -> op == VARIABLE ||
				       currentHead -> right -> op == NUMBER)
			      {
				    parseTree[i] -> left = currentHead;
				    prevHead -> right = parseTree[i];
				    currentHead = parseTree[i];
			      }
			   }
			   else
			   {
			      if(currentHead -> right -> op == VARIABLE ||
				       currentHead -> right -> op == NUMBER)
			      {
				    parseTree[i] -> left = currentHead -> right;
				    currentHead -> right = parseTree[i];
				    prevHead = currentHead;
				    currentHead = parseTree[i];
			      }
			   }
		   }
		 }
		 else if(opcode == MULTIPLY || opcode == DIVIDE)
		 {
           if(i < 2)
		   {
			   if(opcode == MULTIPLY)
			     printf("\n Invalid occurrence of Muliplication Operator");
			   else
			     printf("\n Invalid occurrence of Division Operator");

			   return;
		   }
		   else
		   {
			   if(currentHead -> left == NULL)
			   {
				   printf("\n No Variable Occurred Before the Operator");
			       return;
			   }
			   else if(currentHead -> right == NULL)
			   {
				   printf("\n No Variable Occurred After the Operator");
			       return;
			   }
			   else if(currentHead -> right -> op == VARIABLE ||
				       currentHead -> right -> op == NUMBER)
			   {
				    parseTree[i] -> left = currentHead -> right;
				    currentHead -> right = parseTree[i];
				    prevHead = currentHead;
				    currentHead = parseTree[i];
			   }
			   else
			   {
				   printf("\n Invalid");
			       return;
			   }
		   }
		 }
		 else if(opcode == ASSIGNMENT)
		 {
           if(i == 1)
		   {
			   parseTree[i] -> left = bTree;
			   bTree = parseTree[i];
			   currentHead = bTree;
			   prevHead = bTree;
		   }
		   else
		   {
			   printf("\n Invalid occurence of Assignment Operator");
     	       return;
		   }
		 }

		 //printf("\n OpCOde = %d ", opcode);
		 i++;
	 }

	 printf("\n\n InOrder: ");
	 inorder(bTree);
	 printf("\n\n PostOrder: ");
	 postorder(bTree);
	 printf("\n\n");
	 DoubleArray* val = NULL;
	 val = eval(bTree);
	 int h= 0;

	 for(h=0; h < val -> size; h++) {
	   printf("Value of Expression = %g \n", val->values[h]);
	 }

	 return val;
 }

 // parse tree as inorder
void inorder(Tree *rtree)
{
 if(rtree != NULL)
 {
   if(rtree->left != NULL)
     inorder(rtree->left);  /*step-1 & step-2*/

   if(rtree->symbol != NULL)
     printf(" %s", rtree->symbol->name);   /*step-3*/
   else
     printf(" %g", rtree->value);   /*step-3*/

   if(rtree->right != NULL)
     inorder(rtree->right);  /*step-4*/
 }
}

// parse tree as postorder
void postorder(Tree *rtree)
{
 if(rtree != NULL)
 {
   if(rtree->left != NULL)
     inorder(rtree->left);  /*step-1 & step-2*/

   if(rtree->right != NULL)
     inorder(rtree->right);  /*step-4*/

   if(rtree->symbol != NULL)
     printf(" %s", rtree->symbol->name);   /*step-3*/
   else
     printf(" %g", rtree->value);   /*step-3*/
 }
}

// evaluate the tree by parsing the tree and carrying the operation based on the operators
DoubleArray* eval (Tree *t)
{
  DoubleArray* left, right;
  DoubleArray* lresult = NULL;
  DoubleArray* rresult = NULL;
  DoubleArray* arry = NULL;

  switch (t->op) {

    case NUMBER:
				 arry = (DoubleArray*) malloc (sizeof (DoubleArray)) ;
				 arry -> size = 1;
				 arry -> values = (double*) malloc (sizeof (double)) ;
				 arry -> values[0] = t -> value;
                 return arry;
    case VARIABLE:
				 arry = (DoubleArray*) malloc (sizeof (DoubleArray)) ;
				 arry -> size = t -> symbol -> novals;
				 arry -> values = t -> symbol -> values;
                 return arry;
    case ADD:
	             lresult = eval ( t -> left);
				 rresult = eval ( t -> right);
				 rresult = operate(lresult, rresult, ADD);
                 return rresult;
    case SUBSTRACT:
	             lresult = eval ( t -> left);
				 rresult = eval ( t -> right);
				 rresult = operate(lresult, rresult, SUBSTRACT);
                 return rresult;
    case MULTIPLY:
	             lresult = eval ( t -> left);
				 rresult = eval ( t -> right);
				 rresult = operate(lresult, rresult, MULTIPLY);
                 return rresult;
    case DIVIDE:
	             lresult = eval ( t -> left);
				 rresult = eval ( t -> right);
				 rresult = operate(lresult, rresult, DIVIDE);
                 return rresult;
    case ASSIGNMENT:
				 arry = eval (t -> right);
                 t -> left -> symbol -> values = arry -> values;
                 return arry;
  }
}

/* create new item from name and value */
List *newitem(char* id, double* values, int vals)
{
  List *newp;
  newp = (List*) malloc (sizeof (List)) ;
  newp->identifier = id;
  newp->values = values;
  newp->novals = vals;
  newp->next = NULL;
  return newp;
}

// addfront: add newp to front of listp
List *addfront(List *listp, List *newp)
{
  newp->next = listp;
  return newp;
}

// addend: add newp to end of listp
List *addend(List *listp, List *newp)
{
  List *p;

  if (listp == NULL)
    return newp;
  else if (listp->next == NULL) {
    if (strcmp(newp->identifier, listp->identifier) == 0) {
	  printf("\nDuplicate symbol, value being overwritten. \n");
	  listp->values = newp->values;
      return listp ;
    }
	listp->next = newp;
  }
  else {
    for (p = listp; p->next != NULL; p = p->next)
    {
      if (strcmp(newp->identifier, p->identifier) == 0) {
	    printf("\nDuplicate symbol, value being overwritten. \n");
	    p->values = newp->values;
        return listp ;
	  }
    }
	p->next = newp;
  }

  return listp;
}

/* sequential search for value in the listp */
List *lookuplist (List *listp, char* id)
{
  for ( ; listp != NULL; listp = listp->next)
    if (strcmp(id, listp->identifier) == 0)
      return listp ;
  return NULL; /* no match */
}

void dwritelist (List *listp, char* outfile)
{
  FILE *file;
  file = fopen(outfile,"w");

  if (!file)
  {
		printf("File Open failed!\n");
  }
  else {
   int i = 0;

   for ( ; listp != NULL; listp = listp->next) {

     for (i=0 ; i < listp->novals; i++) {
       fprintf(file,"%g\r\n" , listp->values[i]); /*writes*/
     }

	 //fprintf(file,"\r\n"); /*writes*/
   }

   fclose(file); /*done!*/
  }
}

void dprintlist (List *listp)
{
  printf("\n");
  int i = 0;

  for ( ; listp != NULL; listp = listp->next) {
    printf ("\n Identifier: %s " , listp->identifier) ;
	printf(" : Size = %d \n  Values : \n", listp->novals);

    for (i=0 ; i < listp->novals; i++) {
      printf (" %g \n" , listp->values[i]) ;
    }
  }
  printf("\n");
}


void freeall (List *listp)
{
  List *next ;

  for ( ; listp != NULL; listp = next)
  {
    next = listp->next;
    //assumes identifier is freed elsewhere
    free (listp);
  }
}

DoubleArray* operate(DoubleArray* leftArry, DoubleArray* rightArry, int operation) {

  int i = 0;
  int lsize = leftArry -> size;
  int rsize = rightArry -> size;
  double* left = leftArry -> values;
  double* right = rightArry -> values;

 if(lsize != rsize && lsize != 1 && rsize != 1)
   return NULL;
 if(lsize == rsize && (lsize < 1 || rsize < 1))
   return NULL;

 DoubleArray* darry  = NULL;
 double* temp = NULL;

 if(lsize == rsize) {
   temp = (double*) malloc (rsize*sizeof(double));
   memset(temp, 0, rsize*sizeof(double));
   darry = (DoubleArray*) malloc (sizeof (DoubleArray)) ;
   darry -> values = temp;
   darry -> size = rsize;

   for(i = 0; i < rsize; i++) {
     if(operation == ADD)
       temp[i] = left[i] + right[i];
	 else if(operation == SUBSTRACT)
       temp[i] = left[i] - right[i];
	 else if(operation == MULTIPLY)
       temp[i] = left[i] * right[i];
	 else if(operation == DIVIDE && right[i] != 0)
       temp[i] = left[i] / right[i];
	 else if(operation == DIVIDE && right[i] == 0) {
       printf ("\n Divide %g by zero", left[i]);
	   temp = NULL;
	 }
   }
 }
 else if(lsize == 1) {
   temp = (double*) malloc (rsize*sizeof(double));
   memset(temp, 0, rsize*sizeof(double));
   darry = (DoubleArray*) malloc (sizeof (DoubleArray)) ;
   darry -> values = temp;
   darry -> size = rsize;

   for(i = 0; i < rsize; i++) {
     if(operation == ADD)
       temp[i] = left[0] + right[i];
	 else if(operation == SUBSTRACT)
       temp[i] = left[0] - right[i];
	 else if(operation == MULTIPLY)
       temp[i] = left[0] * right[i];
	 else if(operation == DIVIDE && right[i] != 0)
       temp[i] = left[0] / right[i];
	 else if(operation == DIVIDE && right[i] == 0) {
       printf ("\n Divide %g by zero", left[0]);
	   temp = NULL;
	 }
   }
 }
 else if(rsize == 1) {
   temp = (double*) malloc (lsize*sizeof(double));
   memset(temp, 0, lsize*sizeof(double));
   darry = (DoubleArray*) malloc (sizeof (DoubleArray)) ;
   darry -> values = temp;
   darry -> size = lsize;

   for(i = 0; i < lsize; i++) {
     if(operation == ADD)
       temp[i] = left[i] + right[0];
	 else if(operation == SUBSTRACT)
       temp[i] = left[i] - right[0];
	 else if(operation == MULTIPLY)
       temp[i] = left[i] * right[0];
	 else if(operation == DIVIDE && right[0] != 0)
       temp[i] = left[i] / right[0];
	 else if(operation == DIVIDE && right[0] == 0) {
       printf ("\n Divide %g by zero", left[i]);
	   temp = NULL;
	 }
   }
 }

 return darry;
}
