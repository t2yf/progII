#ifndef ARCHIVER
#define ARCHIVER

#include <string.h> 
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

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
    Membro *membros; //Vetor dinâmico de membros(structs) -> Função para dar malloc, inicializa_archiver
} Diretorio;



//Retorna 1 se arquivo existir e estiver vazio, 0 se não estiver vazio e existir e -1 se não existir
int verifica_arquivador_vazio(FILE *arq);

//Verifica se o archiver existe, se não cria um archiver e retorna
FILE *verifica_existe_archiver(const char *nome);

// Retorna as informações do archiver (diretório) em RAM 
Diretorio *abre_archiver(FILE *arq);

#endif