#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "archiver.h"
#include "inserir.h"
#include "manip_arc.h"


/*Funções auxiliares*/
int verifica_ext(const char *arq){
    size_t tam = strlen(arq);

    //Se o nome do arquivo tem pelo menos 4 caracteres e se termina com .vc
    return (tam >= 4 && strcmp(arq + tam -3, ".vc") == 0);
}

/*Provisório*/
void inserir(){
    printf("inseriu\n");
}
void insere_comprimido(){
    printf("inseriu comprimido\n");
}

void extrai(Archiver *arq){
 
    if(arq)
        printf("extraiu\n");
}

void retirar(){
    printf("retirou\n");
}

void comprimir(){
    printf("listou\n");
}

void mover(){
    printf("moveu\n");
}




int main(int argc, char *argv[]){
    /*Verificar se tem argumentos o suficiente*/
    //argv[0] = ./vinac
    //argv[1] = vinac
    //argv[2] = op
    //argv[3] = archive / membro no -m 
    //argv[4] = membros / archive no -m 
    //argv[5] = target no -m 

    //Limites da quantidade de parâmetros
    if(argc < 4){
        printf("Quantidade de argumentos insuficiente!");
        return 1;
    }

    char *op = argv[2];

    /*Opções: -m*/
    if(strcmp(op, "-m") == 0){
        //Extrapolar limites do -m
        if (argc != 6) {
            printf("Uso correto: ./vinac vinac -m membro archive.vc target\n");
            return 1;
        }
        char *membro = argv[3];
        char *arq = argv[4];
        char *target = argv[5];

        /*Verificar se foi colocado a extensão*/
        if(!verifica_ext(arq)){
            printf("ERRO: o nome do arquivo deve terminar com .vc e ter 4 caracteres, ex: a.vc\n");
            printf("Uso correto: ./vinac vinac -m membro archive.vc target\n");
            return 1;
        }

        //Criar ou pegar archiver
        Archiver *archiver = verifica_existe_archiver(arq);

        mover();
        return 0; 
    }

    /*Opções*/

    /*argv*/
    char *arq = argv[3];
    char **membros = &argv[4];

    /*Verificar se foi colocado a extensão*/
    if(!verifica_ext(arq)){
        printf("ERRO: o nome do arquivo deve terminar com .vc\n");
        return 1;
    }

    //Criar ou pegar archiver
    Archiver *archiver = verifica_existe_archiver(arq);
   // exit (0);

    //Contar quantidade de membros
    int num_membros = contar_membros(membros);
    //printf("quantidade de membros: %d \n", num_membros);

    if(strcmp(op, "-ip") == 0)
        inserir_membros(archiver, membros, num_membros);

    else if(strcmp(op, "-ic") == 0)
        inserir_membros_comprimidos(archiver, membros, num_membros);
    else if(strcmp(op, "-x") == 0){
        if(argc == 4){
            printf("Extração de tudo\n");
            return 0;
        }
        //Extração de um elemento ou mais
        extrai(archiver);
    }
        

    else if(strcmp(op, "-r") == 0)
        retirar();

    else if(strcmp(op, "-c") == 0){
        if(argc == 4){
            printf("Listar tudo\n");
            return 0;
        }
        //Listar um elemento ou mais
        comprimir();
    }
        

    return 0;
}