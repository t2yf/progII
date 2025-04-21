#include <stdio.h>
#include <string.h>
#include "archiver.h"

Archiver *inicializar_archiver(const char *nome){
    //Alocar archive
    Archiver *archive = malloc(sizeof(Archiver));

    if(archive == NULL){
        return NULL;
    }

    //Atribuir nome
    strncpy(archive->nome, nome, MAX_NOME);
    archive->nome[MAX_NOME -1] = '\0';

    //Inicializar diretorio
    archive->dir.qtde_membros = 0;
    archive->dir.cap = 1;
    archive->dir.membros = malloc(sizeof(Membro));

    if(archive->dir.membros == NULL){
        free(archive);
        return NULL;
    }


    return archive;
}

//Retornar como Archiver para poder trabalhar em memória
Archiver *verifica_existe_archiver(const char *nome){
    //Abrir um arquivo binário, verificar se ele existe
    FILE *arq = fopen(nome, "r+b");

    //Se arquivo não existir, criar um novo.
    if(arq == NULL){
        printf("O archiver '%s' não existe. Um novo archiver será criado\n", nome);

        Archiver *new_mem = inicializar_archiver(nome);
        if(new_mem == NULL) return NULL;

        //Criar um novo archiver
        FILE *new_file = fopen(nome, "w+b");

        if(new_file == NULL){
            printf("Erro: Criar um novo archiver, '%s'\n", nome);
            free(new_mem->dir.membros);
            free(new_mem);
            return NULL;
        }
        
        //Salvar diretorio no archiver
        fwrite(&new_mem->dir, sizeof(Diretorio), 1, new_file);
        fclose(new_file);
        return new_mem;
    }
    //Ler o arquivo para devolver um archiver
    Archiver *archiver = malloc(sizeof(Archiver));

    if(archiver == NULL){
        fclose(arq);
        return NULL;
    }

    //Atribuir nome
    strncpy(archiver->nome, nome, MAX_NOME);
    archiver->nome[MAX_NOME -1] = '\0';

    //Ler as informações e atribuir 
        //*Obs: Seguir a mesma ordem do fwrite
    fread(&archiver->dir.qtde_membros, sizeof(int), 1, arq);
    fread(&archiver->dir.cap, sizeof(int), 1, arq);

    //Alocar o vetor de membros conforme a capacidade
    archiver->dir.membros = malloc(sizeof(Membro) * archiver->dir.qtde_membros); 
    if(archiver->dir.membros == NULL){
        fclose(arq);
        free(archiver);
        return NULL;
    }

    //Ler os membros
    fread(archiver->dir.membros, sizeof(Membro), archiver->dir.qtde_membros, arq);

    fclose(arq);
    return archiver;
}