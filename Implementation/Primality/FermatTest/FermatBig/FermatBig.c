/*
 * File: 	FermatBig.c
 * Description:	An extension of the FermatSimple program to handle numbers
 *		larger than the native integer data types offered by the C
 *		language, using the GMP library.
 *
 *
 * Author(s):		Margaret Dorsey
 * Last Updated: 	26/4/2016
 *
 * See gitlog.txt for revision history.
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>

//function declarations
void usageExit(char* msg);
void fermatTest(mpz_t n, mpz_t b, mpz_t result);

/*
 * main function validates input before passing it along to the test
 *
 * params:	command line arguments
 *
 * Author: 	Margaret Dorsey
 */
int main(int argc, char** argv)
{
 if(argc != 3)
  usageExit("You have an incorrect number of arguments.");
 
 //set up our really big integers
 //these need to be freed with mpz_clear() before exiting the program
 mpz_t n;
 mpz_init(n);
 mpz_t b;
 mpz_init(b);

 int ierr = mpz_set_str(n, argv[1], 10);
 if(ierr < 0)
 {
  mpz_clear(n);
  mpz_clear(b);
  usageExit("Your n is not a valid positive integer.");
 }
 ierr  = mpz_set_str(b, argv[2], 10);
 if(ierr < 0)
 {
  mpz_clear(n);
  mpz_clear(b);
  usageExit("Your b is not a valid positive integer.");
 }
 
 mpz_t result;
 mpz_init(result);
 fermatTest(n, b, result);
 if(mpz_cmp_ui(result, (unsigned long int) 0) == 0)
 {
   fprintf(stdout, "%s may be a prime number. \n", mpz_get_str(NULL, 10,n));
   return 0;
 }

 fprintf(stdout, "%s proves %s to be composite. \n",mpz_get_str(NULL, 10,result)
						   ,mpz_get_str(NULL, 10, n));
 mpz_clear(n);
 mpz_clear(b);
 mpz_clear(result);
 return 0;
}

/*
 * fermatTest tests numbers below the square root of n against fermat's
 * theorem to determine whether n can be proven composite.
 *
 * params:	n - the positive integer to be tested for compositeness
 *		b - the upper bound to check against n
 * 		result - to pass back to the caller
 *		either the first witness to compositeness found or 0 if
 *		not proven composite.
 *
 * Author:	Margaret Dorsey
 *
 */

void fermatTest(mpz_t n, mpz_t b, mpz_t result)
{
 mpz_t a;
 mpz_init(a);
 mpz_set_ui(a, (unsigned long int) 2);	//1 should not be tested

 mpz_t p;
 mpz_init(p);
 mpz_sub_ui(p,n,(unsigned long int) 1);  //save ourselves repeated subtraction

 mpz_t gcd;
 mpz_init(gcd);
 mpz_t powmod;
 mpz_init(powmod);
 while(mpz_cmp(a,b) < 0)	//havent hit the upper bound yet
 {
  mpz_gcd(gcd,a,n); //get the gcd of a and n and store in gcd
  mpz_powm(powmod,a,p,n);
  if(mpz_cmp_ui(gcd, (unsigned long int) 1) == 0
     && mpz_cmp_ui(powmod, (unsigned long int) 1 != 0))
  {
    mpz_clear(gcd);
    mpz_clear(powmod);
    mpz_clear(p);
    mpz_set(result,a);
    mpz_clear(a);
    return;
  }
  mpz_add_ui(a, a, (unsigned long int) 1);
 }
 
 //if we get here, there were no witnesses found
 mpz_clear(gcd);
 mpz_clear(powmod);
 mpz_clear(a);
 mpz_clear(p); //it would be faster to clear these as a null terminated list
		//but this is more illustrative
 mpz_set_ui(result, (unsigned long int) 0);
 return;
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
    prod *= a_pow;
    prod = prod % m;
    pow--;	//pull the non power of two exponent out
		//technically this is a reduntant line with how integer division
		//works, but I think it makes it more clear and readable
    
  }
  //we now have an even exponent, which can be written as a sum of powers
  //of two with exponents greater than 1.
   pow /= 2; //divide by two
   a_pow = a_pow*a_pow % m; //square
   
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
