CC=gcc
COPTS=-std=c99 -Wall -pedantic
COPTSbug=-g $(COPTS)
#GET_WORD=/encs_share/cs/class/cs360
INCLUDE=-I$(GET_WORD)/include
LIB=-L$(GET_WORD)/lib

wordpairs: main.o hash.o
	$(CC) $(COPTS) $(LIB) main.o hash.o -o wordpairs -lget

debug: main.o hash.o
	$(CC) -g $(COPTS) main.o hash.o -o wordpairs

clobber: clean
	-rm -f wordpairs
	-rm -f Assignment1.tar.gz

clean:
	-rm -f core *.o
	-rm -f core *~
	-rm -f core *#

main.o: main.c hash.h 
	$(CC) -g $(COPTS) $(INCLUDE) $(LIB) -c main.c
hash.o: hash.c hash.h 
	$(CC) -g $(COPTS) $(INCLUDE) $(LIB) -c hash.c

tar:
	tar czvf Assignment1.tar.gz Makefile hash.h hash.c main.c