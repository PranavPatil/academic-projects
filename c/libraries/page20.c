#include <stdio.h>
#include <string.h>
#include <ctype.h>
int is_pal(char *arr,int n)
{
	if(n<1)
		return 1;

	if(!isalpha(arr[0]))
		return(is_pal(arr+1,n-1));
	if(!isalpha(arr[n-1]))
		return(is_pal(arr,n-1));
	if(arr[0]==arr[n-1])
		return(is_pal(arr+1,n-2));
	return 0;
}
int main()
{
	printf("%d\n",is_pal("o .. f  t ma,am;t . fk o",24));
	return 0;
}
