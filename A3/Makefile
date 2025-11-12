# Compilador 
CC = gcc

# Nome do executável
TARGET = shadow

# Arquivos fonte
SRC = main.c 

# Diretório de saída para os objetos
OBJ_DIR = obj

# Arquivos objeto
OBJ = $(OBJ_DIR)/main.o # Adicionar outros

# Flags do compilador
CFLAGS = -Wall -Werror -o  $(shell pkg-config --cflags allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5) 

# Flags do linker
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5) -lm

# Regra padrão
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Regra para compilar os arquivos objeto
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos compilados
clean:
	rm -rf $(OBJ_DIR) $(TARGET)