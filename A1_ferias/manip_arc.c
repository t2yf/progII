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
void extrair_info_membro(Diretorio *dir, const char *nome_arq, Membro *membro){
    struct stat info;

    //Verifica se o membro existe
    if(stat(nome_arq, &info) == -1){
        perror("Erro: Não foi possível obter informações do membro\n");
        return;
    }

    //Puxar os atributos
    strncpy(membro->nome, nome_arq, MAX_NOME);
    membro->nome[MAX_NOME -1] = '\0';
    membro->UID = info.st_ino;
    membro->tamanho_original = info.st_size;
    membro->tamanho_comprimido = info.st_size;
    membro->data_modificacao = info.st_mtime;

    membro->ordem = dir->qtde_membros;
    membro->offset = 0; //Mudar depois
}

//TODO função privada
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

void mover_membros_frente(FILE *arq){

}

//A ideia saber a partir de qual membro eu devo ir puxando para trás
void mover_membros_tras(FILE *arq, Diretorio *dir, unsigned int ordem_prim_membro, unsigned long referencia){
    //Localizar o primeiro membro
    unsigned long ini_prim = dir->membros[ordem_prim_membro].offset;
    
    //Começar puxando do último membro
    unsigned int idx = dir->qtde_membros;
    unsigned long ini_membro, fim_membro, tam_membro;

    ini_membro = dir->membros[idx].offset;

    while(ini_membro != ini_prim){
        tam_membro = dir->membros[idx].tamanho_comprimido;

        fim_membro = ini_membro + tam_membro;

        mover_membros(arq, ini_membro, fim_membro, referencia);

        idx--;
        ini_membro = dir->membros[idx].offset;
    }

    //Mover o primeiro membro
    tam_membro = dir->membros[ordem_prim_membro].tamanho_comprimido;
    fim_membro = ini_prim + tam_membro;
    mover_membros(arq, ini_prim, fim_membro, referencia);

}

// OBS: Admite-se que os arquivos de leitura e escrita existam e estejam abertos 
void ler_escreve(FILE *arq_leitura, FILE *arq_escrita, unsigned long ini_leitura, unsigned long fim_leitura, unsigned long ini_escrita){
    //Colocar dados no buffer
    size_t tam_conteudo = fim_leitura - ini_leitura;
    char *buffer = malloc(tam_conteudo);

    if(buffer == NULL){
        perror("Erro de alocação do buffer\n");
        return;
    }

    //Ler conteúdo do arquivo de leitura
    fseek(arq_leitura, ini_leitura, SEEK_SET);
    fread(buffer, 1, tam_conteudo, arq_leitura);

    //Escrever conteúdo no arquivo de escrita
    fseek(arq_escrita, ini_escrita, SEEK_SET);
    fwrite(buffer, 1, tam_conteudo, arq_escrita);

    free(buffer);
}



unsigned char *aloca_buffer_dinamico(size_t tamanho){
    unsigned char *buffer = malloc(sizeof(unsigned char) * tamanho);

    if (!buffer) {
        perror("Erro ao alocar buffer");
        return NULL;
    }
    return buffer;
}