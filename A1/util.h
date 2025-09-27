#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include <stdio.h>
#include "gbv.h"


// Converte time_t para string formatada
void format_date(time_t t, char *buffer, int max);

/*Manipula Library ===================================================================================================*/

// Libera o vetor dinâmico de Documents do Library
void free_lib(Library *lib);

// Escreve no arquivo gbv a quantidade de docs
void lib_count_write(FILE *gbv, Library *lib);

// Escreve no arquivo gbv o offset do library no disco
void lib_offset_write(FILE *gbv, Library *lib, unsigned long int offset_lib);

// Escreve no arquivo gbv o vetor de info de documentos
void lib_write(FILE *gbv, Library *lib, unsigned long int offset_lib);

/*Funções auxiliares =================================================================================================*/

// A partir do docname, procura no vetor de docs em qual índice está o docs
// Retorna o índice, caso contrário -1
int docs_name_cmp(const Library *lib, const char *docname);

//Verifica se o filername tem pelo menos 5 caracteres e se termina com .gbv
int gbv_ext_verify(const char *filername);

//Verifica se o .gbv está vazio, ou seja, se o final é igual ao início
int gbv_empty(FILE *gbv);

#endif

