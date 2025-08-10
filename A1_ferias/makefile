CC     = gcc
CFLAGS = -Wall -g 
# -Wextra

#gera o executável
all: main.o archiver.o manip_arc.o inserir.o lz.o# arq.o $(CC) -o vinac main.o #  arq.
	$(CC) -o vinac main.o archiver.o manip_arc.o inserir.o lz.o

archiver.o: archiver.c archiver.h
	$(CC) -c archiver.c

manip_arc.o: manip_arc.c manip_arc.h
	$(CC) -c manip_arc.c

inserir.o: inserir.c inserir.h
	$(CC) -c inserir.c

lz.o: lz.c lz.h
	$(CC) -c lz.c

clean:
	rm -f *.o *.vc *~ vinac 