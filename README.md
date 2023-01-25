# mymalloc
Contributors: Sean Maye and Finn Peng
_____________________________________
Description:
Our goal with this project is to make our own version of manual allocation and deallocation. Make sure you #include "mymalloc.h" when using this library! mymalloc() is a function that replicates the manual allocation function malloc() given in the standard C-library. myfree() is a function that replicates the deallocation function free() given in the standard C-library. Since malloc() and free() are included in stdlib.h, we created a library mymalloc.c that will contain mymalloc() and myfree().
_____________________________________
Descriptions of files:
memgrind.c - this file contains the main function that is used to test our version of malloc and free
mymalloc.h - this is a header file that contains all the function prototypes for mymalloc.c and wraps mymalloc in and myfree into malloc and free respectively
mymalloc.c - this file contains the mymalloc and myfree functions along with the coalesce(), find-fit(), and place() functions which are used to manipulate the array pointers in memory and manually allocate space
Makefile - this file is used to quickly compile our programs with certain flags used for debugging
_____________________________________
Testplan:
We will create a headerfile called mymalloc.h that contains the function prototypes mymalloc and myfree. These functions will be wrapped into the standard malloc and free functions given by the C-library. Each of the functions, along with any helper functions, will be written in the mymalloc.c file, which will include the mymalloc.h headerfile. After that, we will create a memgrind.c file that contains a main function which we can use to test the robustness of our malloc and free.
_____________________________________
What properties our library has:
Our library has 2 functions mymalloc() and myfree() which are wrapped into the standard malloc and free. By defining malloc and free with our own function, the memgrind.c program will output results using the version of malloc and free that we implemented.
_____________________________________
Methods we will use:
Within the mymalloc.c programs, we will use
pack(size, alloc), which will combine the size and alloc bits into a WORD;
mget(ptr), which will read a WORD at ptr;
mput(ptr), which will write a WORD at ptr;
get_size(ptr), which reads the size at ptr; and
get_alloc(ptr), which reads the allocation at ptr.
These methods are declared by #DEFINE so that they can be used freely by all functions in mymalloc.c
_____________________________________
Descriptions of functions in mymalloc.c:
static void *coalesce(void *blockPtr) - to reduce external fragmentation, we take the fragmented parts of memory and let the memory in between the footer and header be overwriten after using the myfree(void *blockPtr, char *file, int line) method
static void *find_fit(size_t asize) - find a segment of memory that is large enough to fit the payload in the heap 
static void place(void *blockPtr, size_t asize) - place the payload along with the header and footer at the segment in memory that we found with find_fit(size_t asize)
void *mymalloc(size_t size, char *file, int line) - manually allocates a segment of memory in an array by calling upon the find_fit(size_t asize) and place(void *blockPtr, size_t asize) methods and returning a pointer to the malloced array
void myfree(void *blockPtr, char *file, int line) - takes in a pointer to an array in memory and changes the footer and header to zero and coalesces the pointers with coalesce(void *blockPtr)
_____________________________________
Stress testing our program:
Testing our program will be done though the memgrind.c file. This file will
1. malloc() and immediately free() a 1-byte chunk, 120 times.
2. Use malloc() to get 120 1-byte chunks, storing the pointers in an array, then use free() to
deallocate the chunks.
3. Randomly choose between
•Allocating a 1-byte chunk and storing the pointer in an array
•Deallocating one of the chunks in the array (if any)
Repeat until you have called malloc() 120 times, then free all remaining allocated chunks.
4. Allocate [1] to [120] in the array, then free all remaining allocated chunks.
5. Allocate in all the odd locations in memory and Allocate in all the even locations in memory, then free all remaining allocated chunks.

Repeat 50 times.
_____________________________________
Description of error messages:
address outside of memory
    "The memory you are trying to free does not exist!" - calling free() with an address not obtained from malloc()
    "The memory you are trying to free is not at the start of the chunk!" - calling free() with an address not at the start of the chunk 
    "The memory you are trying to free has already been freed!" - calling free() a second time on the same pointer
_____________________________________
Design notes:
Using an implicit free list header and footer makes coalesce() more efficient! Because the user is not required to initialize the block when they malloc, we initialize the start of the block in the mymalloc.c.
_____________________________________
Some issues we ran into:
Segmentation fault when we tried to malloc 1 byte then free 1 byte, 120 times, 50 times.
We may have to initialize the array before to avoid a Segmentation Fault.
We had an issue with directly printing memory so we put various print statements throughout our program to print the process of writing into memory and freeing it after. We ended up changing the if/else statements in coalesce to better work with our program.
