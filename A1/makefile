CC     = gcc
CFLAGS = -Wall -g 
# -Wextra

#gera o executável
all: main.o archiver.o manip_arc.o# arq.o $(CC) -o vinac main.o #  arq.o
	$(CC) -o vinac main.o archiver.o manip_arc.o

archiver.o: archiver.c archiver.h
	$(CC) -c archiver.c

manip_arc.o: manip_arc.h manip_arc.h
	$(CC) -c manip_arc.c

clean:
	rm -f *.o *.vc *~ vinac 