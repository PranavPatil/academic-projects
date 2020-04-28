//recursive example of filling a sack of rocks
#include<stdio.h>
int knapsack(int total,int candit,int sack[],int size)
{
	if(total==0)
		return 1;
	if(total<0||candit>=size)
		return 0;
	if (knapsack(total-sack[candit],candit+1,sack,size))
	{
		printf("%d\n",sack[candit]);
		return 1;
	}  /*else:*/
	return knapsack(total,candit+1,sack,size);
}
int main(void)
{
	int rocks[5]={5,5,1,3,2},size=5;
	knapsack(7,0,rocks,size);
	return 0;
}
