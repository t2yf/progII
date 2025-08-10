#ifndef MANIP_ARC
#define MANIP_ARC

#include "archiver.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#define TAM_BUFFER 1024

void extrair_info_membro(Diretorio *dir, const char *nome_arq, Membro *membro);
int contar_membros(char **membros);

//movimentar membros
void mover_membros(FILE *arq, unsigned long inicio, unsigned long final, unsigned long referencia);

//Alocar buffer dinâmico
unsigned char *aloca_buffer_dinamico(size_t tamanho);

//leitura e escrita
void ler_escreve(FILE *arq_leitura, FILE *arq_escrita, unsigned long ini_leitura, unsigned long fim_leitura, unsigned long ini_escrita);

//movimentar membros para tras
void mover_membros_tras(FILE *arq, Diretorio *dir, unsigned int ordem_prim_membro, unsigned long referencia);

//atualizar localização dos membros depois de mover ou remover

#endif 