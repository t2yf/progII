#ifndef ARCHIVER
#define ARCHIVER

#include <string.h> 
#include <stdlib.h>
#include <time.h>

#define MAX_NOME 1025
#define MAX_ARQ 1000

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
    Membro membros[MAX_ARQ]; //Dinâmico!!! -> Vetor de structs -> Função para dar malloc, inicializa_archiver
    int qtde_membros;
} Diretorio;

typedef struct{
    Diretorio dir; //Estático!!
    char nome[MAX_NOME];
} Archiver;


/*Inicializar o archiver*/
Archiver *inicializar_archiver(Archiver *archive, char *nome);

#endif