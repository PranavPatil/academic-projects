/*
*******************************************************************************************
 SWE 5001 Software Engineering 1
 Assignment 1
---------------------------------------------

 Compile code as: gcc -o output tchinn.c -lm
 	     Run : ./output 1.txt

********************************************************************************************
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define NUMARRAYLENGTH 100
#define maxsize 10


float A[maxsize];
float* BubbleSort(float*, int);
float median_function(float[],int);

int  main( int argc, char *argv[] )
{
   int i,n,j;
   int counter = 0;
   float sum = 0.00;
   float max = 0.00;
   float min = 0.00;
   float mean = 0.00;
   float median = 0.00;
   float variance = 0.00;
   float temp = 0.00;
   float sd = 0.00;
   char str[200];

   FILE* fp ;
   float numbers[NUMARRAYLENGTH];

   fp = fopen (argv[1],"r" ) ;

   if( fp == NULL )
   {
      puts ( "Cannot open the file" ) ;
      exit(1) ;
   }
   else
   {
      char str[200];
      counter = 0;
   }

   while(fgets(str,sizeof(str),fp) != NULL && counter < NUMARRAYLENGTH)
   {
      printf("counter = %d \n",counter);
      int len = strlen(str)-1;
      if(str[len] == '\n')
      str[len] = 0;
      numbers[counter] = atof(str);   // convert string to float
      counter++;
   }

     fclose(fp);

     float* nums;
     nums = BubbleSort(numbers, counter);

     printf("\n%d",counter);

     for(i= 0;i<counter;i++)
          {
            sum = sum + nums[i];
            temp = nums[i]*nums[i];
            printf("\n Sorted = %f ",nums[i]);
          }

     min = nums[0];
     max = nums[counter-1];

     printf ("\n\nMinimum element in an array : %f\n", min);
     printf ("Maximum element in an array : %f\n", max);

     mean = sum/counter;
     variance = (temp - (temp)/counter)/(counter - 1);
     sd = sqrt(variance);

     printf("\n mean = %f",mean);
     printf("\n variance = %f", variance);
     printf("\n standard deviation = %f", sd);

     median = median_function(nums, counter);
     printf("\n median = %f", median);

     return 0;
}


float median_function(float a[],int n)
{
   if(n%2 == 0)
   {
          return (a[n/2]+a[n/2-1])/2;
   }
   else
   {
          return a[n/2];
   }
 }



float* BubbleSort(float* a, int n)
{
    int i, j;
    float tmp;
    if (n <= 1) return a;	// check if the array has no elements or has 1 element

    i = 1;
    do
    {


     for (j = n - 1; j >= i; --j)
      	 {
          if (a[j-1] > a[j])
            {
                tmp = a[j-1];
                a[j-1] = a[j];
                a[j] = tmp;
            }
        }
    }

      while (++i < n);
      return a;
}
