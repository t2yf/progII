#include <stdio.h>
#include <stdlib.h>

// visualizar, printar no terminal

int main() {
    FILE *arq;

    arq = fopen("a.txt", "r+");
    fseek(arq, 0, SEEK_END);
    size_t tam = ftell(arq);
    char *buffer = malloc(tam);


    rewind(arq);

    fread(buffer, tam, 1, arq);
    fwrite(buffer, 1, tam, stdout); //printar no terminal
    free(buffer);
    fclose(arq);

    return 0;
}