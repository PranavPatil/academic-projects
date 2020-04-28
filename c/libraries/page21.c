//recursive example of complete numbers
#include <stdio.h>
int upside(int number,int loop)
{
	int i,save;
	save=number;
	if (number==0)
		return 0;
	save=number%10;
	for (i=1;i<loop;i++)
		save*=10;
	save+=upside(number/10,--loop);
	return save;
}
int reverse(int number,int *zeros)
{
	int loop=0,save;
	while (number%10==0)
	{
		++*zeros;
		number/=10;
	}
	save=number;
	do
	{
		number/=10;
		++loop;
	}while(number!=0);
	number=upside(save,loop);
	return number;
}
int main(void)
{
	int number,zeros,i;
	for(i=1;i<=1000;++i)
	{
		if(i==reverse(i,&zeros))
			printf("%4d",i);
	}
	return 0;
}
