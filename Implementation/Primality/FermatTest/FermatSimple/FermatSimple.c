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

//function declarations
void usageExit(char* msg);
long fermatTest(long n);

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
 long n = strtol(argv[1],NULL, 10);
 if(n <= 0)
  usageExit("Your n is not a valid positive integer.");
 return 0;
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

long fermatTest(long n)
{
 long a = 2;	//1 should not be tested
 long p = n-1;  //save ourselves repeated subtraction
 while(a*a < n)	//less costly than sqrt computation
 {
  if(powMod(a,p,n) == 1)
   return a;
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

long powMod(long a, long pow, long m)
{
 long prod = 1;
 long a_pow = a*a; //our current 
 if(pow%2 == 1) //odd exponent, shouldn't come up much in 
 {		//our current application but for the sake of code flexibility
   prod *= a;
   prod--;	//pull the non power of two exponent out
 }
 //we now have an even exponent, which can be written as a sum of powers
 //of two with exponents greater than 1.
 while(pow > 0)
 {
  p /= 2; //divide by two
  if(pow%2 ==1)
 }

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
