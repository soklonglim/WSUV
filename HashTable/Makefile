#GET_WORD=/encs_share/cs/class/cs360
CC=gcc
COPTS= -g -c -std=c99 -Wall -pedantic
LOPTS= -g -std=c99 -Wall
LIBDIRS= -lget -L$(GET_WORD)/lib
INCDIRS= -I$(GET_WORD)/include
SRCDIRS= $(GET_WORD)/src
DEPS= ht.h
	
wordpairs: main.o ht.o getWord.o crc64.o
	$(CC) $(LOPTS) -o wordpairs main.o ht.o getWord.o crc64.o 
	
main.o: main.c $(SRCDIRS)/getWord.c $(DEPS)
	$(CC) $(COPTS) $(INCDIRS) $(LIBDIRS) main.c $(SRCDIRS)/getWord.c 
	
ht.o: ht.c $(SRCDIRS)/crc64.c $(DEPS)
	$(CC) $(COPTS) $(INCDIRS) $(LIBDIRS) ht.c $(SRCDIRS)/crc64.c 

clean:
	rm wordpairs *.o
