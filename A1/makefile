CC     = gcc
CFLAGS = -Wall -g 
# -Wextra

#gera o executável
all: main.o archiver.o # arq.o $(CC) -o vinac main.o #  arq.o
	$(CC) -o vinac main.o archiver.o

#arq.o: arq.c arq.h $(CC) -c $(CFLAGS) arq.c
archiver.o: archiver.c archiver.h
	$(CC) -c archiver.c
clean:
	rm -f *.o *.vc *~ vinac 