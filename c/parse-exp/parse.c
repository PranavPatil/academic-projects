/*
*******************************************************************************************
 SWE 5001 Software Engineering 1
 September 12, 2010

********************************************************************************************
*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define EXPRMAXSIZE 50
#define VARIABLE 1
#define NUMBER 2
#define ADD 3
#define SUBSTRACT 4
#define MULTIPLY 5
#define DIVIDE 6
#define ASSIGNMENT 7

typedef struct Symbol Symbol;
typedef struct Tree Tree;

struct Symbol {
	int value;
	char *name;
};

struct Tree {
	int op;
	int value ;
	Symbol *symbol ;
	Tree *left;
	Tree *right;
};

void parse(char*, int*, Tree**);
void builtTree(Tree*, Tree **, int);
void inorder(Tree*);
void postorder(Tree*);
int eval (Tree*);

int  main()
{
   char inputExpression[EXPRMAXSIZE];
   int thislen = 0;
   char* beginning;
   char* expression;
   char* quit = "quit";

   printf("\nPlease type the expression starting with let  ");

   while( strcasecmp(fgets(inputExpression,sizeof(inputExpression),stdin),quit) != 0 )     //Reads a line of input from the keyboard
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

        if(strcmp(beginning,"let") != 0)               // validates it as "let"
        {
		    printf("\nExpression must start with let\n");
            printf("\nType expression again\n");

	        continue;
        }

		Tree *parseTree[EXPRMAXSIZE];                      //Tree Structure  object
		int totaltokens = 0;
		parse(expression, &totaltokens, parseTree);                       //parses the expression based on rules given in assignment.
		printf("\n TotalTokens = %d ", totaltokens);
		Tree* bTree;
		builtTree(bTree, parseTree, totaltokens);
        //inorder(bTree);
		exit(1);

    }
    return 0;
}

// This function parses the input expression.
void parse(char* expression, int* totaltoken, Tree** parseTree)
{
        int i = 0;
        int tokenIndex = 0;
        char* next;
		char* characters[EXPRMAXSIZE];
		Symbol* thisCharacter = NULL;             // Symbol structure object
		Tree *t = NULL;
		int token = 0;

		next = strtok(expression," ");

        while(next != NULL)                        // Expression tokenized and stored in characters array.
        {

		   if(strcmp(next,"let")!=0)
		   {
 		     characters[tokenIndex] = next;
  		     tokenIndex++;
		   }

		   next = strtok(NULL," ");
		}
		int totalTokens = tokenIndex;
		*totaltoken = tokenIndex;

		while(i<totalTokens)                               // Parse rules used for insertion of tree elements into the tree structure.
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
                case VARIABLE:
					{
							thisCharacter = (Symbol*) malloc(sizeof(Symbol));
							memset(thisCharacter, 0, sizeof(Symbol));
							t = (Tree*) malloc(sizeof(Tree));
							memset(t, 0, sizeof(Tree));
							thisCharacter -> name = characters[i];
							thisCharacter -> value = 0;
							t -> op = VARIABLE;
							t -> value = 0;
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
							  thisCharacter -> name = characters[i];
							  thisCharacter -> value = atoi(characters[i]);
							  t = (Tree*) malloc(sizeof(Tree));
							  memset(t, 0, sizeof(Tree));
							  t -> op = NUMBER;
							  t -> value = atoi(characters[i]);
							  t -> symbol = thisCharacter;
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
							  thisCharacter -> value = 0;
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
							  thisCharacter -> value = 0;
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
							  thisCharacter -> value = 0;
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
							  thisCharacter -> value = 0;
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
							  thisCharacter -> value = 0;
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
	    	 for(i = 0;i<totalTokens;i++)
			 {
				if(parseTree[i] != NULL)
	        	printf("\n%s \n", (parseTree[i] -> symbol)-> name);
			 }

	    }
		return;
 }

 void builtTree(Tree* bTree, Tree** parseTree, int totalTokens)
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
			   else if(currentHead -> op == MULTIPLY ||
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
				   printf("\n ERRORRRRRRRRRRRRRRRRRRRRRRRRRRR");
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
	 int val = 0;
	 val = eval(bTree);
	 printf("\n Value of Expression = %d \n", val);

	 return;
 }

void inorder(Tree *rtree)
{
 if(rtree != NULL)
 {
   if(rtree->left != NULL)
     inorder(rtree->left);  /*step-1 & step-2*/

   printf(" %s", rtree->symbol->name);   /*step-3*/

   if(rtree->right != NULL)
     inorder(rtree->right);  /*step-4*/
 }
}

void postorder(Tree *rtree)
{
 if(rtree != NULL)
 {
   if(rtree->left != NULL)
     inorder(rtree->left);  /*step-1 & step-2*/

   if(rtree->right != NULL)
     inorder(rtree->right);  /*step-4*/

   printf(" %s", rtree->symbol->name);   /*step-3*/
 }
}

int eval (Tree *t)
{
  int left, right;

  switch (t->op) {

    case NUMBER:
                 return t -> value;
    case VARIABLE:
                 return t -> symbol -> value ;
    case ADD:
                 return eval ( t -> left) + eval ( t -> right) ;
    case SUBSTRACT:
                 return eval ( t -> left) - eval ( t -> right) ;
    case MULTIPLY:
                 left = eval (t -> left);
                 right = eval (t -> right);
                 return left * right;
    case DIVIDE:
                 left = eval (t -> left) ;
                 right = eval (t -> right) ;
                 if (right == 0)
                   printf ("divide %d by zero", left) ;
                 return left / right;
    case ASSIGNMENT:
                 t -> left -> symbol -> value = eval (t -> right) ;
                 return t -> left -> symbol -> value;
  }
}
