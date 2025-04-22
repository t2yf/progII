#include "flags.h"
#include "manip_arc.h"

/*IDEIA: Armazenar em RAM os membros já existentes, atualizar o diretório, escrever os membros após o diretório atualizado e, por fim, inserir o membro novo*/
void inserir_membros(Archiver *archiver, char **membros, int num_arq){
    //Abrir arquivo binário
    FILE *arq = fopen(archiver->nome, "r+b");
    if(!arq){
        perror("Erro ao abrir o arquivo\n");
        return;
    }


    /*IMPORTANTE: Como essa operação vem após o verifica_existe_archiver o *archiver já possui o arquivo.vc carregado*/

    //Carregar os membros já existentes
    char **membros_existentes = malloc(sizeof(char *)*archiver->dir.qtde_membros);
    for(int i=0; i < archiver->dir.qtde_membros; i++){
        //Alocar de acordo com o tamanho comprimido, se não estiver comprimido o tamanho é o mesmo do original
        membros_existentes[i] = malloc(archiver->dir.membros[i].tamanho_comprimido);
        fseek(arq, archiver->dir.membros[i].offset, SEEK_SET);
        fread(membros_existentes[i], 1, archiver->dir.membros[i].tamanho_comprimido, arq);
    }

    //!!!VERIFICAR SE O ARQUIVO JÁ EXISTE
    //Extrair informação dos novos membros
    for(int i=0; i<num_arq; i++){

    }

    //!!!!Fazer um buffer, tenho que pegar membro por membro, não pode tudo, conforme as regras. Logo eu tenho que colocar em um buffer, do tamanho do maior elemento, o membro e fazer algo com ele. Enfim, acho que deixar o diretório no final é melhor
    //!!!Também tenho que pensar se eu faço com uma lista ou com um vetor, por hora vou deixar vetor mesmo, não sei bem qual é melhor


    //Inserir os novos membros no final do arquivo


    //Colocar membros na RAM

    //Colocar diretório na RAM
    //Extrair informações dos membros, realocar diretório e inserir essas informações
    //Atualizar quantidade de membros

    //Escrever o diretório no disco
    //Achar tamanho do diretório
    //Escrever membros no disco
    //Escrever membro novo no disco


}

//Ideia A:
    //Abrir archiver->nome

    //Ver quantos arquivos serão inseridos

    //Achar final diretorio

    //Achar final do arquivo

    //Tentar empurrar de algum modo

    //Add mais uma posição no diretório e escrever

    //Fechar arquivo e liberar malloc

//Ideia B: Copiar tudo em um arquivo temporário