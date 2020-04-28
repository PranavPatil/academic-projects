//recursion samples
#include<stdio.h>
int sum_sqrt(int m,int n)
{
if (m<n)
	return m*m+sum_sqrt(m+1,n);
return m*m;
}

int add(int a,int b)
{
if (b>0)
	return add(++a,--b);
return a;
}

int mult(int a,int b)
{
if (b>1)
	return add(a,mult(a,--b));
return a;
}

void rev_str(char *s,int m,int n)
{
char c;
if (m<n)
	{
	c=s[m];
	s[m]=s[n];
	s[n]=c;
	rev_str(s,m+1,n-1);
	}
}

void move_towers(int n,int start,int end,int spare)
{
if (n==1)
	printf("move a disk from peg %d to peg %d\n",start,end);
else
	{
	move_towers(n-1,start,spare,end);
	printf("move a disk from peg %d to peg %d\n",start,end);
	move_towers(n-1,spare,end,start);
	}
}
