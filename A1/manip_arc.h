#ifndef MANIP_ARC
#define MANIP_ARC

#include "archiver.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#define TAM_BUFFER 1024

void extrair_info_membro(Archiver *archiver, const char *nome_arq, Membro *membro);
int contar_membros(char **membros);

//movimentar membros
void mover_membros(FILE *arq, unsigned long inicio, unsigned long final, unsigned long referencia);

//Alocar buffer dinâmico
unsigned char *aloca_buffer_dinamico(size_t tamanho);

//verificar se membro já existe no archive

//verificar quantos membros serão inseridos

//extrair informações dos membros


//atualizar localização dos membros depois de mover ou remover

#endif 