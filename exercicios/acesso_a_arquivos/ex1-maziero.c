/*
    Escreva um programa em C para informar o número de caracteres presentes em um arquivo de texto.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    FILE *arq;
    char c;
    int i = 0;

    //abrir arquivo de leitura
    arq = fopen("arq.txt", "r"); //parametro read

    if(!arq){
        perror("Erro ao abrir o arquivo");
        exit(1);
    }


    while((c = fgetc (arq)) != EOF){ //enquanto não chegar no fim do arquivo, ir contando
        i++;
    }

    printf("Quantidade de caracteres: %d\n", i);

    //fechar um arquivo
    fclose(arq);

    return 0;
}