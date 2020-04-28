void write_it(void)
{
int c;
if ((c=getchar()!=’\n’)
	write_it();
putchar(c);
}


int odd_even(int n)
{
if (n<=1)
	return n;
return odd_even(n-2);
}


int digit_sum(int n)
{
if (n==0)
	return 0;
return n%10 +digit_sum(n/10);
}


int f1(int n)
{
if (n<=3 && n>=1)
	return n;
return f1(n-1)*f1(n-2)-f1(n-3);
}
