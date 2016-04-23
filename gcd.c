//Margaret Dorsey
//1/28/2016
//Finds the greatest common divisor of n integer values
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
int getgcd(int a, int b);

int main(int argc, char** argv)
{
    if(argc < 3)
    {
      printf("Usage: requires at least two arguments.");
      return 1;
    }
    int count = argc -1;
    int i;
    int* p_vals = (int *) malloc(sizeof(int)*count);
    bool nonZero = false;
    for(i = 1; i < argc; i++)
    {
     p_vals[i-1]= (int) strtol(argv[i],NULL,10); //will be 0 if failed
     if(p_vals[i-1] != 0)
       nonZero = true;
    }
 
    if(!nonZero)
    {
      printf("At least one argument must be non-zero for the gcd to exist.");
      free(p_vals);
      return 2; //exit failure
    }

    int gcd = 1;
    int j,k;
    int tempgcd;
    bool divides = true;
    for(i = 0; i < count; i++)
    {
      for(j = i+1; j < count; j++)
      {
        tempgcd = getgcd(p_vals[i], p_vals[j]);
        if(tempgcd > gcd)
	{
           //printf("%d \n",tempgcd);
           for(k = 0; k < count; k++)
           {
             if(p_vals[k] %tempgcd != 0)
               divides = false;
           }
           if(divides)
             gcd = tempgcd;
        }
      }
    }



    printf("The gcd is %d \n",gcd);
    free(p_vals);
    return 0;
    

}
//recursive
int getgcd(int a, int b)
{
 int r;
 if(a > b)
 {
   r = a%b;
   if(r == 0)//base case
    return b;
   return getgcd(r,b);
 } 
 else
 {
   r=b%a;
   if(r == 0)
    return a;
   return getgcd(a,r);
   
 }

}
