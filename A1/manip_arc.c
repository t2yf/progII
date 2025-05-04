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
    membro->UID = info.st_uid;
    membro->tamanho_original = info.st_size;
    membro->tamanho_comprimido = info.st_size;
    membro->data_modificacao = info.st_mtime;

    membro->ordem = archiver->dir.qtde_membros;
    membro->offset = 0; //Mudar depois
}


void mover_membros(FILE *arq, unsigned long inicio, unsigned long final, unsigned long referencia){
    //Colocar o "membro" no buffer
    size_t tam = final - inicio;
    char *buffer = malloc(tam);

    if(buffer == NULL){
        perror("Erro de alocação do buffer\n");
        return;
    }

    fseek(arq, inicio, SEEK_SET);
    fread(buffer, 1, tam, arq);

    //Procurar a referência com o fseek
    fseek(arq, referencia, SEEK_SET);

    //Escrever o "membro" do buffer onde o fseek aponta
    fwrite(buffer, 1, tam, arq);

    free(buffer);

}