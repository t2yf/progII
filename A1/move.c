#include <string.h>
#include "archiver.h"
#include "manip_arc.h"


void mover_membros(FILE *arq, unsigned long inicio, unsigned long final, unsigned long referencia){
    //Colocar o "membro" no buffer
    size_t tam = final - inicio;
    char *buffer = malloc(tam);

    if(buffer == NULL){
        perror("Erro de alocação do buffer\n");
        return;
    }

    fseek(arq, inicio, SEEK_SET);
    fread(buffer, 1, tam, arq);

    //Procurar a referência com o fseek
    fseek(arq, referencia, SEEK_SET);

    //Escrever o "membro" do buffer onde o fseek aponta
    fwrite(buffer, 1, tam, arq);

    free(buffer);
}

