#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "archiver.h"
#include "manip_arc.h"
#include "inserir.h"


/*Funções auxiliares*/
int verifica_ext(const char *arq){
    size_t tam = strlen(arq);

    //Se o nome do arquivo tem pelo menos 4 caracteres e se termina com .vc
    return (tam >= 4 && strcmp(arq + tam -3, ".vc") == 0);
}



int main(int argc, char *argv[]){
    /*Argumentos*/
    //argv[0] = ./vinac
    //argv[1] = op
    //argv[2] = archive / membro no -m 
    //argv[3] = membros / archive no -m 
    //argv[4] = target no -m 

    //A operação com o mínimo de argumentos é o -c, que não necessita dos membros
    if(argc < 3){
        printf("Quantidade de argumentos insuficiente!");
        return 1;
    }

    char *op = argv[1];

    /*Opções: -m*/
    if(strcmp(op, "-m") == 0){
        // //Extrapolar limites do -m
        // if (argc != 5) {
        //     printf("Uso correto: ./vinac -m membro archive.vc target\n");
        //     return 1;
        // }
        // char *membro = argv[2];
        // char *arq = argv[3];
        // char *target = argv[4];

        // /*Verificar se foi colocado a extensão*/
        // if(!verifica_ext(arq)){
        //     printf("ERRO: o nome do arquivo deve terminar com .vc e ter 4 caracteres, ex: a.vc\n");
        //     printf("Uso correto: ./vinac -m membro archive.vc target\n");
        //     return 1;
        // }

        // //Criar ou pegar archiver
        // Archiver *archiver = verifica_existe_archiver(arq);

        // //TODO Mover

        // return 0; 
    }

    //TODO caso -c, que tem 3 argumentos e não 4

    /*Opções*/

    char *arq = argv[2];
    char **membros = &argv[3];

    /*Verificar se foi colocado a extensão*/
    if(!verifica_ext(arq)){
        printf("ERRO: o nome do arquivo deve terminar com .vc\n");
        return 1;
    }

    //Se arquivador, ou seja, o .vc não existir, ele cria um vazio
    FILE *file =  verifica_existe_archiver(arq);
    Diretorio *dir = abre_archiver(file); 

    if(strcmp(op, "-ip") == 0){
        int idx_membros = 0;
        for(int i = 4; i <= argc; i++){
            insere(file, dir, membros[idx_membros]);
            idx_membros++;
        }   
    }
    
    //TODO for generico para inserir/extrair/remover > for para pegar os membros arg3 até menor argc
    //TODO por exemplo, se tiver que inserir 4 membros, chamar a função insere 4 vezes
  

    return 0;
}