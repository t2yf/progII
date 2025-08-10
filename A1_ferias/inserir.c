#include "inserir.h"

void insere(FILE *arq, Diretorio *dir, char *membro){
    printf("Inserir\n");

    //Abrir membro
    FILE *novo_membro = fopen(membro, "rb");

    if(!novo_membro){
        printf("Erro ao abrir o arquivo (membro)\n");
        return;
    }

    //Atualizar diretório: Mais um membro para o diretório
    dir->qtde_membros++;

    //Extrair informações do membro
    Membro info_membro;
    extrair_info_membro(dir, membro, &info_membro);


    //!!!!!!! certo?
    dir->membros[dir->qtde_membros] = info_membro;
   
    unsigned long tam_dir = sizeof(int) + dir->qtde_membros*sizeof(Membro);
    unsigned long tam_membro = info_membro.tamanho_original;

    //Posicionar no início do arquivador (.vc)
    rewind(arq);

    //Escrever o diretório (int da quantidade de membros) !!!!!!!!!!!
    fwrite(&dir->qtde_membros, sizeof(int), 1, arq);


    //Caso 1: Arquivador vazio, ou seja, primeiro elemento a ser inserido
    int vazio = verifica_arquivador_vazio(arq);

    if(vazio){
        //Atualizar o offset: int (info da quantidade de membros) + Membros
        info_membro.offset = sizeof(int) + dir->qtde_membros*sizeof(Membro);
        //!!!!!!
        fwrite(dir->membros, sizeof(Membro), dir->qtde_membros, arq);

        //Escrever o membro
        ler_escreve(novo_membro, arq, 0, tam_membro, tam_dir);

        fclose(novo_membro);
        //!!!!!
        fclose(arq);
        return;
    }

    //Caso 2: Arquivador não está vazio
    
    //Mover os membros para frente na quantidade de bytes de uma struct membro (info)
    
    mover_membros_tras(arq, dir, tam_dir, sizeof(Membro));


    //Atualizar o offset: int (info da quantidade de membros) + Membros + tamanho dos membros (bytes)
    unsigned long tam_total_membros = 0;
    for(unsigned int i = 0; i < dir->qtde_membros; i++){
        tam_total_membros += dir->membros[i].tamanho_comprimido;
    }
    info_membro.offset = sizeof(int) + dir->qtde_membros*sizeof(Membro) + tam_total_membros;

    //Escrever o diretório (infos)
    fwrite(dir->membros, sizeof(Membro), dir->qtde_membros, arq);

    //Escrever o membro
    ler_escreve(novo_membro, arq, 0, tam_membro, tam_dir);


    fclose(novo_membro);
    fclose(arq);


    //TODO arquivo muito muito grande (blocos grandes)
    //TODO substituir os arquivos repetidos
}





//TODO lidar com blocos grandes
//TODO inserir vários membros


//TODO move para frente e para trás

//TODO (.vc) existe vazio [conferir se eh vazio], (.vc sem existir),  um elemento, vários
//TODO já envia o arquivo aberto 

