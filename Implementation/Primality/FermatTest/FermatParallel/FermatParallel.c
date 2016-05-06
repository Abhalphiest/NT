/*
 * File: 	FermatParallel.c
 * Description:	A parallelization of the FermatBig program, using the OpenMPI
 *		library to compute on multiple CPU cores in parallel. This 
 *		implementation focuses heavily on optimization and flexibility
 *		in order to get a true feel for the computational viability of
 *		Fermat's Theorem based algorithms for primality testing.
 *
 * Author(s):	
 * Last Updated:
 *
 * See gitlog.txt for revision history.
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
#include<mpi.h>
//function declarations
void usageExit(char* msg);
void fermatTest(mpz_t n, mpz_t b, mpz_t result, int id, int numThreads);

/*
 * main function validates input before passing it along to the test
 *
 * params:	command line arguments
 *
 * Author: 	Margaret Dorsey
 */
int main(int argc, char** argv)
{
 //Initialize MPI and fix our argc and argv
 int ierr = MPI_Init(&argc, &argv);
 int id = MPI_Comm_rank(MPI_COMM_WORLD, &id);
 int num_threads = MPI_Comm_size(MPI_COMM_WORLD, &numthreads);
 if(id == 0) //original thread
 {
  if(argc != 3)
   usageExit("You have an incorrect number of arguments.");
 }
 //set up our really big integers
 //these need to be freed with mpz_clear() before exiting the program
 mpz_t n;
 mpz_init(n);
 mpz_t b;
 mpz_init(b);

 ierr = mpz_set_str(n, argv[1], 10);
 if(ierr < 0 && id == 0)
 {
  mpz_clear(n);
  mpz_clear(b);
  usageExit("Your n is not a valid positive integer.");
 }
 ierr  = mpz_set_str(b, argv[2], 10);
 if(ierr < 0 && id == 0)
 {
  mpz_clear(n);
  mpz_clear(b);
  usageExit("Your b is not a valid positive integer.");
 }
 if(id != 0)
 { 
  mpz_t result;
  mpz_init(result);
 
  fermatTest(n, b, result,id, num_threads);

  if(mpz_cmp_ui(result, (unsigned long int) 0) == 0)
  {
   int i = 0;
   MPI_Send(&i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
   mpz_clear(n);
   mpz_clear(b);
   mpz_clear(result);
   return 0;
  }
  else //we found a witness
  {
   int i = 1;
   fprintf(stdout, "%s proves %s composite.", mpz_get_str(NULL, 10, result)
					      mpz_get_str(NULL,10, n);
   MPI_Send(&i, 1, MPI_INT, 0,0, MPI_COMM_WORLD); //send message to main thread
   mpz_clear(n);
   mpz_clear(b);
   mpz_clear(result);
   return 0;
  }
 }
 else
 {
  while(num_threads>1) //until we're the last thread left
  {
   int i;
   //blocking call
   MPI_Recv(&i, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
            MPI_COMM_WORLD, NULL);
   if(i != 0) //witness is found
   {
    mpz_clear(n);
    mpz_clear(b);
    MPI_Finalize();
    return 0;
   } 
   num_threads--; //whoever sent us a message also exited
  }
  //no witnesses found
  fprintf(stdout, "%s may be a prime.", mpz_get_str(NULL, 10, n));
  mpz_clear(n);
  mpz_clear(b);
  return 0;
 } 
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

void fermatTest(mpz_t n, mpz_t b, mpz_t result, int id, int numThreads)
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
  if(mpz_mod_ui(NULL, a, (unsigned long) numThreads-1) != id -1)
   continue; //split up the workload
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
 MPI_Finalize(); //kill all our threads
 exit(1);
}
