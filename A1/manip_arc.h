#ifndef MANIP_ARC
#define MANIP_ARC

#include "archiver.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

void extrair_info_membro(Archiver *archiver, const char *nome_arq, Membro *membro);
int contar_membros(char **membros);

//verificar se membro já existe no archive

//verificar quantos membros serão inseridos

//extrair informações dos membros


//atualizar localização dos membros depois de mover ou remover

#endif 