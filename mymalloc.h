/*
* mymalloc.h is a headerfile
*
* authors: Sean Maye and Finn Peng
* date created: 2/21/2022
*
*/

#include <stdio.h>
#include <stddef.h>

#ifndef MYMALLOC_H_
#define MYMALLOC_H_

//wrapping our functions into malloc() and free()
#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)

//our 2 function prototypes
void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);

#endif /* MYMALLOC_H_ */
