COMPILER = gcc
CFLAGS = -Wall -g
CFLAGS2 = -g

all: mymalloc 
.PHONY: all
.PHONY: clean

mymalloc.o: mymalloc.c mymalloc.h
	$(COMPILER) $(CFLAGS) -c mymalloc.c

memgrind.o: memgrind.c mymalloc.h
	$(COMPILER) $(CFLAGS2) -c memgrind.c

mymalloc: mymalloc.o memgrind.o
	$(COMPILER) $(CFLAGS) -o mymalloc memgrind.o mymalloc.o

clean:
	rm -f * *.o