#ifndef ARCHIVER
#define ARCHIVER

#include <string.h> 
#include <stdlib.h>
#include <time.h>

#define MAX_NOME 1025

typedef struct{
    char nome[MAX_NOME];
    int UID;
    int tamanho_original;
    int tamanho_comprimido;
    time_t data_modificacao;
    int ordem; //em que posicao o arquivo se encontra
    long offset; //"Onde que começa o arquivo"

} Membro;

typedef struct {
    int qtde_membros;
    //int cap; // Para o realloc
    Membro *membros; //Vetor dinâmico de membros(structs) -> Função para dar malloc, inicializa_archiver
} Diretorio;

typedef struct{
    Diretorio dir; //Estático!!
    char nome[MAX_NOME];
} Archiver;


/*Inicializar o archiver*/
//Archiver *inicializar_archiver(Archiver *archive, char *nome);
//void verifica_existe_archiver(Archiver *archiver);

Archiver *verifica_existe_archiver(const char *nome);

#endif