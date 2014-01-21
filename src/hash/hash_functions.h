
#ifndef _HASH_FUNCTIONS_H
#define _HASH_FUNCTIONS_H


#include <stdio.h>


typedef unsigned int (*hash_function)(char*, unsigned int len);

unsigned int RSHash  (char* str, unsigned int len);
/**
 * A simple hash function from Robert Sedgwicks Algorithms in C book. 
 */

unsigned int JSHash  (char* str, unsigned int len);
/**
 * A bitwise hash function written by Justin Sobel.
 */
 
unsigned int PJWHash (char* str, unsigned int len);
/**
 * This hash algorithm is based on work by Peter J. Weinberger of AT&T Bell Labs. 
 * The book Compilers (Principles, Techniques and Tools) by Aho, Sethi and Ulman, 
 * recommends the use of hash functions that employ the hashing methodology found
 *  in this particular algorithm.
 */

unsigned int ELFHash (char* str, unsigned int len);
/**
 * Similar to the PJW Hash function, but tweaked for 32-bit processors. 
 * Its the hash function widely used on most UNIX systems.
 */

unsigned int BKDRHash(char* str, unsigned int len);
/**
 * This hash function comes from Brian Kernighan and Dennis Ritchie's book 
 * "The C Programming Language". It is a simple hash function using a strange 
 * set of possible seeds which all constitute a pattern of 31....31...31 etc, 
 * it seems to be very similar to the DJB hash function.
 */

unsigned int SDBMHash(char* str, unsigned int len);
/**
 * This is the algorithm of choice which is used in the open source SDBM project. 
 * The hash function seems to have a good over-all distribution for many different 
 * data sets. It seems to work well in situations where there is a high variance 
 * in the MSBs of the elements in a data set.
 */

unsigned int DJBHash (char* str, unsigned int len);
/**
 * An algorithm produced by Professor Daniel J. Bernstein and shown first to the 
 * world on the usenet newsgroup comp.lang.c. It is one of the most efficient 
 * hash functions ever published.
 */

unsigned int DEKHash (char* str, unsigned int len);
/**
 * An algorithm proposed by Donald E. Knuth in The Art Of Computer Programming 
 * Volume 3, under the topic of sorting and search chapter 6.4.
 */

unsigned int BPHash  (char* str, unsigned int len);
unsigned int FNVHash (char* str, unsigned int len);
unsigned int APHash  (char* str, unsigned int len);

unsigned int murmur_hash (char *str, unsigned int len);

#endif
