CC = gcc
CFLAGS = -Wall -g
#using c23
all: gbv
# change test2 por main !!!!!!!!!!!!!!! -lm
gbv: test2.o gbv.o util.o
	$(CC) $(CFLAGS) -o gbv test2.o gbv.o util.o -lm

test2.o: test2.c gbv.h
	$(CC) $(CFLAGS) -c test2.c

gbv.o: gbv.c gbv.h
	$(CC) $(CFLAGS) -c gbv.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

clean:
	rm -f *.o *.gbv gbv

