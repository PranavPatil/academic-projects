//recursive round matrix
#include <stdio.h>
#include <math.h>
#define N 16
typedef int MATRIX[N][N];
void print_mat(MATRIX a,int n)
{
	if (n==4){
		printf("%d %d %d %d\n",a[0][0],a[0][1],a[1][1],a[1][0]);
		return;
		}
	print_mat(a,n/4);
	print_mat(&a[0][sqrt(n)/2],n/4);
	print_mat(&a[sqrt(n)/2][sqrt(n)/2],n/4);
	print_mat(&a[sqrt(n)/2][0],n/4);
}
