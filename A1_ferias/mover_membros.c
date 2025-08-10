#include <stdio.h>
#include <stdlib.h>


void mover(FILE *arq, unsigned long ini, unsigned long fim, unsigned long ref){
    //Ler elemento
    char buffer[1024];

    unsigned long tam = fim - ini;
    fseek(arq, ini, SEEK_SET);
    fread(buffer, tam, 1, arq);

    //Colocar na referencia 
    fseek(arq, ref, SEEK_SET);
    fwrite(buffer, tam, 1, arq);
}

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

void mover_membros_tras(FILE *arq, unsigned int fim, unsigned int ini,  unsigned long referencia){

    unsigned int ini_membro = fim;
    unsigned int ini_prim = ini;

    while(ini_membro != ini_prim){

        mover(arq, ini_membro, ini_membro, referencia);

        ini_membro--;
    }

    //Mover o primeiro membro
    mover(arq, ini_prim, ini_membro -1 , referencia);

}


//.dat
//cada letra eh um byte

int main(){
    FILE *arq, *arq2;

    arq = fopen("a.dat", "w+");
    arq2 = fopen("b.dat", "w+");

    for(char i = 0; i<3; i++){
        fprintf(arq, "%d", i);
    }

    for(char i = 6; i<9; i++){  
        fprintf(arq2, "%d", i);    
    }

    //mover(arq, 0, 1, 3);

    mover_membros_tras(arq, 1, 0, 1);

    //ler_escreve(arq2, arq, 0, 3, 4);

    fclose(arq);

    return 0;
}