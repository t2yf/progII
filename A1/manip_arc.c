#include "manip_arc.h"

//Contar a quantidade de membros
int contar_membros(char **membros){
    int i = 0;
    while(membros[i] != NULL){
        i++;
    }
    return i;
}

//Extrair as informações com o membro fechado (stat)
void extrair_info_membro(Archiver *archiver, const char *nome_arq, Membro *membro){
    struct stat info;

    //Verifica se o membro existe
    if(stat(nome_arq, &info) == -1){
        perror("Erro: Não foi possível obter informações do membro\n");
        return;
    }

    //Puxar os atributos
    strncpy(membro->nome, nome_arq, MAX_NOME);
    membro->nome[MAX_NOME -1] = '\0';

    //!!!Tirar os espaços do nome do membro????

    membro->UID = info.st_uid;
    membro->tamanho_original = info.st_size;
    membro->tamanho_comprimido = info.st_size;
    membro->data_modificacao = info.st_mtime;

    ///!!!! O que colocar?
    membro->ordem = archiver->dir.qtde_membros;
    membro->offset = 0; //Mudar depois
}