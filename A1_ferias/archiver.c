#include <stdio.h>
#include <string.h>
#include "archiver.h"


int verifica_arquivador_vazio(FILE *arq){
    fseek(arq, 0, SEEK_END);
    unsigned long fim = ftell(arq);
    
    return fim == 0 ;
}

//Essa função deve vir antes da abre_archiver na main
FILE *verifica_existe_archiver(const char *nome){
    FILE *arq = fopen(nome, "r+b");

    if(arq == NULL){
        printf("O archiver '%s' não existe. Um novo archiver será criado\n", nome);

        //Criar um novo archiver
        arq = fopen(nome, "w+b");
    }

    return arq;
}

Diretorio *abre_archiver(FILE *arq){
    //Não precisa testar se existe ou não porque a verifica_existe_archiver já vai ter sido executada

    Diretorio *dir = malloc(sizeof(Diretorio));

    if(dir == NULL){
        fclose(arq);
        return NULL;
    }

    //Ler informações do arquivador para dir na Mem RAM
    fread(&dir->qtde_membros, sizeof(int), 1, arq);
    dir->membros = malloc(sizeof(Membro) * dir->qtde_membros);

    if(dir->membros == NULL){
        fclose(arq);
        free(dir);
        return NULL;
    }

    fread(dir->membros, sizeof(Membro), dir->qtde_membros, arq);

    fclose(arq);

    return dir;
}


