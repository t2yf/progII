CC = gcc
CFLAGS = -Wall -g -Wextra 
LIB = pelucia
BIN = main

$(BIN): $(BIN).o $(LIB).o
$(BIN).o: $(BIN).c $(LIB).h
$(LIB).o: $(LIB).c $(LIB).h

clean:
	rm -f *.o *~$(BIN)