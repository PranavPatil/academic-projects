//solving equation recursivly
#include<stdio.h>
#include<stdlib.h>
int isdigit(int c)
{
	return (c >= '0') && (c <= '9');
}

int isoperator(int c)
{
	return (c == '+') || (c == '-') || (c == '*') || (c == ')') || (c == '(');
}

void getnuop(int *number,int *oper,int *lastop)
{
	int c;
	*lastop=*oper;
	if (isdigit (c=getchar()))
	{
		*number = c - '0';
		while (isdigit(c = getchar()))
			*number = 10 * ( *number ) + c - '0';
	}
	*oper = c;
	return;
}
void calc(int *result,int lastop,int num)
{
		if (lastop=='+')
			*result+=num;
		if (lastop=='-')
			*result-=num;
		if (lastop=='*')
			*result*=num;
}

int solve (void)
{
   int result=0;
   int num=0,op='+',lastop;
   getnuop(&num,&op,&lastop);
   while ((op!=')')||(op!='\n'))
   {
	if (op=='(')
	{
		calc(&result,lastop,solve());
		op='\n';
	}
	else
		calc(&result,lastop,num);
	if (op=='\n')
		break;
	getnuop(&num,&op,&lastop);

    }
    return result;
}
int main(void)
{
	printf("Result is: %d\n",solve());
	return 0;
}
 