/*
 * File: 	FermatSimple.c
 * Description:	A simple, illustrative implementation of a primality testing
 *		algorithm based on Fermat's theorem. The focus of this program
 *		is more on the demonstration of the concept than efficiency,
 *		applicability, or handling of large data.
 *
 * Usage:	FermatSimple <n>, with <n> a positive integer to test for
 *		possible primality.
 *		The program will either report that n may be a prime, or report
 *		the first witness found to its compositeness.
 *
 * Author(s):	
 * Last Updated:
 *
 * See gitlog.txt for revision history.
 *
 */

#include<stdio.h>
#include<stdlib.h>

//upper bound for a
const unsigned long UPP_BOUND = 1000000000;

//function declarations
void usageExit(char* msg);
unsigned long fermatTest(unsigned long n);
unsigned long powMod(unsigned long a, unsigned long pow, unsigned long m);
unsigned long gcd(unsigned long a, unsigned long b);

/*
 * main function validates input before passing it along to the test
 *
 * params:	command line arguments
 *
 * Author: 	Margaret Dorsey
 */
int main(int argc, char** argv)
{
 if(argc != 2)
  usageExit("You have an incorrect number of arguments.");
 unsigned long n =(unsigned long) strtol(argv[1],NULL, 10);
 if(n <= 0)
  usageExit("Your n is not a valid positive integer.");
 
 unsigned long result = fermatTest(n);
 if(result == 0)
 {
   fprintf(stdout, "%ld may be a prime number. \n", n);
   return 0;
 }

 fprintf(stdout, "%ld proves %ld to be composite. \n",result, n);
 return result;
}

/*
 * fermatTest tests numbers below the square root of n against fermat's
 * theorem to determine whether n can be proven composite.
 *
 * params:	n - the positive integer to be tested for compositeness
 *
 * return:	either the first witness to compositeness found or 0 if
 *		not proven composite.
 *
 * Author:	Margaret Dorsey
 *
 */

unsigned long fermatTest(unsigned long n)
{
 unsigned long a = 2;	//1 should not be tested
 unsigned long p = n-1;  //save ourselves repeated subtraction
 while(a < UPP_BOUND)	//less costly than sqrt computation
 {
  fprintf(stderr, "%lu %lu \n", a,powMod(a,p,n));
  if(gcd(a,n) == 1 && powMod(a,p,n) != 1)
   return a;
  a++; //will try to come up with a more sophisticated way 
       //to choose reasonable a in the future
 }
 
 //if we get here, there were no witnesses found
 return 0;
}

/*
 * powMod returns the powth power of a modulo mod
 *
 * params: 	a - the base of the expression
 * 		pow - the power to raise a to
 *		m - the modulo to reduce a^b with
 *
 * returns:	a^p (mod m)
 *
 */

unsigned long powMod(unsigned long a,unsigned long pow,unsigned long m)
{
 unsigned long prod = 1;
 unsigned long a_pow = a; //our current
 while(pow >0)
 { 
  if(pow%2 == 1) //odd exponent, shouldn't come up much in 
  {		//our current application but for the sake of code flexibility
    prod *= a_pow % m;
    pow--;	//pull the non power of two exponent out
		//technically this is a reduntant line with how integer division
		//works, but I think it makes it more clear and readable
    
  }
  //we now have an even exponent, which can be written as a sum of powers
  //of two with exponents greater than 1.
   pow /= 2; //divide by two
   a_pow = a_pow*a_pow; //square
   
 }
 return prod % m;
}

/*
 *
 * usageExit prints a usage error to stderr, then exits with
 * a usage error code.
 *
 * params:	msg - the specific usage error to be printed
 *
 * Author: 	Margaret Dorsey
 *
 */
void usageExit(char* msg)
{
 fprintf(stderr, "Usage: FermatSimple <n>, with <n> being an integer ");
 fprintf(stderr, "to test for primality. \n %s \n", msg);
 exit(1);
}
