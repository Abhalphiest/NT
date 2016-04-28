#
# Makefile for implementations of primality testing and
# factorization methods in C
#
# Math 371 Number Theory Spring 2016
# Rochester Institute of Technology
#
# Author(s): Margaret Dorsey, Jodie Miu
#


all:


# Fermat Test

######### Simple Version
fermatsimple: Primality/FermatTest/FermatSimple/FermatSimple.c
	gcc -o FermatSimple Primality/FermatTest/FermatSimple/FermatSimple.c
fermatbig: Primality/FermatTest/FermatBig/FermatBig.c
	gcc -o FermatBig Primality/FermatTest/FermatBig/FermatBig.c -lgmp
fermatparallel: Primality/FermatTest/FermatParallel/FermatParallel.c
	mpicc -o FermatParallel Primality/FermatTest/FermatBig/FermatBig.c -lgmp
######### Big Data Version

######### Parallel Big Data Version


# Strong Test

