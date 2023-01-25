/*
 *
 * memgrind.c is a program where our mymalloc.c library and mymalloc.h header are tested
 *
 * authors: Sean Maye and Finn Peng
 * date created: 2/21/2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include "mymalloc.h" //our header file
#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)

/*malloc() and immediately free() a 1-byte chunk, 120 times*/
void test1()
{
    for (int i = 0; i < 120; i++)
    {
        int *test = malloc(1);
        free(test);
    }
}

/*Use malloc() to get 120 1-byte chunks, storing the pointers in an array, then use free() to
deallocate the chunks*/
void test2()
{
    int *k[120];

    for (int i = 0; i < 120; i++)
    {
        int *test = malloc(1);
        k[i] = test;
    }
    for (int i = 0; i < 120; i++)
    {
        free(k[i]);
    }
}

/*Randomly choose between
•Allocating a 1-byte chunk and storing the pointer in an array
•Deallocating one of the chunks in the array (if any)
Repeat until you have called malloc() 120 times, then free all remaining allocated chunks*/
void test3()
{
    int p = 0;
    int j = 0;
    int *arrayTest[120 * sizeof(int *)];
    while (p < 120)
    {
        int rng = rand() % 2;
        if (rng == 1)
        {
            int *test = malloc(1);
            arrayTest[j] = test;
            p++;
            j++;
        }
        else
        {
            if (arrayTest[j] != NULL)
            {
                free(arrayTest[j]);
                j--;
            }
        }
    }
    for (int i = 0; i < 120; i++)
    {
        if (arrayTest[i] != NULL)
        {
            free(arrayTest[i]);
        }
    }
}

/*Allocate [1] to [120] in the array, then free all remaining allocated chunks*/
void test4()
{
    int *ptr;
    int i, n;
    n = 120;
    ptr = (int *)malloc(n * sizeof(int));

    for (i = 0; i < n; ++i)
    {
        ptr[i] = i + 1;
    }
    free(ptr);
}

/* Allocate in all the odd locations in memory and Allocate in all the even locations in memory, then free all remaining allocated chunks*/
void test5()
{
    int *ptr = (int *)malloc(100);
    for (int i = 0; i < 100; i++)
    {
        if (i % 2 == 0)
        {
            ptr[i] = 2;
        }
    }
    for (int i = 0; i < 100; i++)
    {
        if (i % 2 == 1)
        {
            ptr[i] = 1;
        }
    }
    free(ptr);
}

int main(int argc, char **argv)
{
    double time1avg = 0;

    for (int i = 0; i < 50; i++)
    {
        clock_t start = clock();
        test1();
        clock_t end = clock();
        double time_elapsed_in_seconds = (end - start) / (double)CLOCKS_PER_SEC;
        time1avg += time_elapsed_in_seconds;
    }
    time1avg = time1avg / 50;
    printf("Average time for test1 in seconds: %f\n", time1avg);

    double time2avg = 0;

    for (int i = 0; i < 50; i++)
    {
        clock_t start = clock();
        test2();
        clock_t end = clock();
        double time_elapsed_in_seconds = (end - start) / (double)CLOCKS_PER_SEC;
        time2avg += time_elapsed_in_seconds;
    }
    time2avg = time2avg / 50;
    printf("Average time for test2 in seconds: %f\n", time2avg);

    double time3avg = 0;

    for (int i = 0; i < 1; i++)
    {
        clock_t start = clock();
        test3();
        clock_t end = clock();
        double time_elapsed_in_seconds = (end - start) / (double)CLOCKS_PER_SEC;
        time3avg += time_elapsed_in_seconds;
    }
    time3avg = time3avg;
    printf("Average time for test3 in seconds: %f\n", time3avg);

    double time4avg = 0;

    for (int i = 0; i < 1; i++)
    {
        clock_t start = clock();
        test4();
        clock_t end = clock();
        double time_elapsed_in_seconds = (end - start) / (double)CLOCKS_PER_SEC;
        time1avg += time_elapsed_in_seconds;
    }
    time4avg = time4avg;
    printf("Average time for test4 in seconds: %f\n", time4avg);

    double time5avg = 0;

    for (int i = 0; i < 50; i++)
    {
        clock_t start = clock();
        test5();
        clock_t end = clock();
        double time_elapsed_in_seconds = (end - start) / (double)CLOCKS_PER_SEC;
        time1avg += time_elapsed_in_seconds;
    }
    time5avg = time5avg / 50;
    printf("Average time for test5 in seconds: %f\n", time5avg);

    // Bug Catching Showcase

    // Freeing with an adress not obtained in Malloc
    int *x = NULL;
    free(x);

    int *q = malloc(sizeof(int) * 2);
    free(q + 1);

    /* int *p = malloc(sizeof(int) * 100);
     free(p);
     free(p); */
}