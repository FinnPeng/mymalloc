/*
 *
 * mymalloc.c is a library that has a custom malloc and free
 *
 * authors: Sean Maye and Finn Peng
 * date created: 2/21/2022
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "mymalloc.h" //our header file for my malloc

#define msize 4096  //memory size
static char memory[msize];

// return a point to allocated block if OK, null on error

/* Constants and macros */
#define wordSize 4 /* Word size (bytes) which is also the size of header and footer*/
#define doubleWordSize 8 /* Double word size (bytes) */

/* Pack a size and allocated bit into a word */
#define pack(size, alloc) ((size) | (alloc))

/* Read and write a word at address ptr */
#define mget(ptr) (*(unsigned int *)(ptr))
#define mput(ptr, val) (*(unsigned int *)(ptr) = (val))

/* Read the size and allocated fields from address ptr */
#define get_size(ptr) (mget(ptr) & ~0x7)
#define get_alloc(ptr) (mget(ptr) & 0x1)

/* Given block ptr blockPtr, compute address of its header and footer */
#define headerPtr(blockPtr) ((char *)(blockPtr)-wordSize)
#define footerPtr(blockPtr) ((char *)(blockPtr) + get_size(headerPtr(blockPtr)) - doubleWordSize)

/* Given block ptr blockPtr, compute address of next and previous blocks */
#define nextBlockPtr(blockPtr) ((char *)(blockPtr) + get_size(((char *)(blockPtr)-wordSize)))
#define prevBlockPtr(blockPtr) ((char *)(blockPtr) - get_size(((char *)(blockPtr)-doubleWordSize)))

/*coalesce function*/
static void *coalesce(void *blockPtr)
{
    size_t prev_alloc = get_alloc(footerPtr(prevBlockPtr(blockPtr)));
    size_t next_alloc = get_alloc(headerPtr(nextBlockPtr(blockPtr)));
    size_t size = get_size(headerPtr(blockPtr));

    if ((prev_alloc && next_alloc) || (!prev_alloc && !next_alloc))
    { /* Case 1 */
        return blockPtr;
    }
    else if (prev_alloc && !next_alloc)
    { /* Case 2 */
        size += get_size(headerPtr(nextBlockPtr(blockPtr)));
        mput(headerPtr(blockPtr), pack(size, 0));
        mput(footerPtr(blockPtr), pack(size, 0));
    }

    else if (!prev_alloc && next_alloc)
    { /* Case 3 */
        size += get_size(headerPtr(prevBlockPtr(blockPtr)));
        mput(footerPtr(blockPtr), pack(size, 0));
        mput(headerPtr(prevBlockPtr(blockPtr)), pack(size, 0));
        blockPtr = prevBlockPtr(blockPtr);
    }

    else
    { /* Case 4 */
        size += get_size(headerPtr(prevBlockPtr(blockPtr))) +
                get_size(footerPtr(nextBlockPtr(blockPtr)));
        mput(headerPtr(prevBlockPtr(blockPtr)), pack(size, 0));
        mput(footerPtr(nextBlockPtr(blockPtr)), pack(size, 0));
        blockPtr = prevBlockPtr(blockPtr);
    }
    return blockPtr;
}

//helper method that finds a spot in memory that can fit our desired chunk
static void *find_fit(size_t asize)
{
    void *blockPtr;
    blockPtr = (void *)memory;

    for (blockPtr = (void *)memory; get_size(headerPtr(blockPtr)) > 0; blockPtr = nextBlockPtr(blockPtr))
    {
        if (!get_alloc(headerPtr(blockPtr)) && (asize <= get_size(headerPtr(blockPtr))))
            ;
        return blockPtr;
    }
    printf("Memory cannot be placed!\n");
    return NULL; // ERROR!
}

//helper method that places in the spot that find_fit() found
static void place(void *blockPtr, size_t asize)
{
    size_t csize = get_size(headerPtr(blockPtr));

    if ((csize - asize) >= (2 * doubleWordSize))
    {
        mput(headerPtr(blockPtr), pack(asize, 1));
        mput(footerPtr(blockPtr), pack(asize, 1));
        blockPtr = nextBlockPtr(blockPtr);
        mput(headerPtr(blockPtr), pack(csize - asize, 0));
        mput(footerPtr(blockPtr), pack(csize - asize, 0));
    }
    else
    {
        mput(headerPtr(blockPtr), pack(csize, 1));
        mput(footerPtr(blockPtr), pack(csize, 1));
    }
}

/*our malloc function*/
void *mymalloc(size_t size, char *file, int line)
{
    /* According to the C standard library, if a user attempts to allocate 0 bytes using malloc, then null should be returned */
    size_t asize;
    char *blockPtr;
    if (size == 0)
    {
        return NULL;
    }

    if (size <= doubleWordSize)
    {
        asize = 2 * doubleWordSize;
    }
    else
    {
        asize = doubleWordSize * ((size + (doubleWordSize) + (doubleWordSize - 1)) / doubleWordSize);
    }

    blockPtr = (void *)memory;
    if (get_size(headerPtr(blockPtr)) == 0)
    {
        mput(headerPtr(blockPtr), pack(asize, 1));
        mput(footerPtr(blockPtr), pack(asize, 1));
        return blockPtr;
    }
    blockPtr = find_fit(asize);
    if (blockPtr != NULL)
    {
        place(blockPtr, asize);
        return blockPtr;
    }
    return NULL; // could not place the payload
}

//our free function that dereferences pointer and coalesces freed pointers to avoid fragmentation
void myfree(void *blockPtr, char *file, int line)
{
    int hasError = 0;
    
    if (blockPtr == NULL)
    {
        printf("The memory you are trying to free does not exist!\n");
        hasError = 1;
        
    }
    else if (get_size(headerPtr(blockPtr)) != get_size(footerPtr(blockPtr)))
    {
        printf("The memory you are trying to free is not at the start of the chunk!\n");
        hasError = 1;
        
    }
    else if ((get_alloc(headerPtr(blockPtr)) == 0 && get_size(headerPtr(blockPtr)) == 0))
    {
        printf("The memory you are trying to free has already been freed!\n");
        hasError = 1;
        
    }
    
    if (hasError == 0)
    {
        
        size_t size = get_size(headerPtr(blockPtr));
        mput(headerPtr(blockPtr), pack(size, 0));
        mput(footerPtr(blockPtr), pack(size, 0));
        coalesce(blockPtr);
    }
}