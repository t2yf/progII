#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

//!!!Se for no formato ./tyf24 vinac -ip archive.vc texto.txt, ele estará lendo como argv[0] = ./tyf24


/*Funções auxiliares*/
int verifica_ext(const char *arq){
    size_t tam = strlen(arq);

    printf("%s\n", arq);
    printf("%ld\n", tam);
    //se o nome do arquivo tem pelo menos 3 caracteres e se termina com .vc
    return (tam >= 3 && strcmp(arq + tam -3, ".vc") == 0);
}

/*Provisório*/
void inserir(){
    printf("inseriu\n");
}
void insere_comprimido(){
    printf("inseriu comprimido\n");
}

void extrai(){
    printf("extraiu\n");
}

void retirar(){
    printf("retirou\n");
}

void comprimir(){
    printf("comprimiu\n");
}

void mover(){
    printf("moveu\n");
}




int main(int argc, char *argv[]){

    /*Verificar se tem argumentos o suficiente*/
    //argv[0] = vinac
    //argv[1] = op
    //argv[2] = archive / membro no -m 
    //argv[3] = membros / archive no -m 
    //argv[4] = target no -m 

    //mínimo para -c
    if(argc < 3){
        printf("Quantidade de argumentos insuficiente!");
        return 1;
    }

    /*argv*/
    char *op = argv[1];

    /*Opções: -m*/
    if(strcmp(op, "-m") == 0){
        if (argc < 5) {
            printf("Uso correto: vinac -m membro archive target\n");
            return 1;
        }
        char *membro = argv[2];
        char *arq = argv[3];
        char *target = argv[4];

        /*Verificar se foi colocado a extensão*/
        if(!verifica_ext(arq)){
            printf("ERRO: o nome do arquivo deve terminar com .vc\n");
            return 1;
        }

        //inicializar archiver

        mover();
        return 0; 
    }

    /*Opções*/

    /*argv*/
    char *arq = argv[2];
    char **membros = &argv[3];

    /*Verificar se foi colocado a extensão*/
    if(!verifica_ext(arq)){
        printf("ERRO: o nome do arquivo deve terminar com .vc\n");
        return 1;
    }

    //inicializar archiver

    if(strcmp(op, "-ip") == 0)
        inserir();

    else if(strcmp(op, "-ic") == 0)
        insere_comprimido();

    else if(strcmp(op, "-x") == 0)
        extrai();

    else if(strcmp(op, "-r") == 0)
        retirar();

    else if(strcmp(op, "-c") == 0)
        comprimir();

    return 0;
}