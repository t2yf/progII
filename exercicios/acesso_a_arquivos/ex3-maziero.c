/* Escreva um programa em C para ler um arquivo minusc.txt e escrever um arquivo maiusc.txt contendo o mesmo texto em maiúsculas.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(){
    FILE *arq;
    FILE *write;
    char c;

    //abre arquivo
    arq = fopen("minusc.txt", "r");
    write = fopen("maiusc.txt", "w");

    while((c = fgetc (arq)) != EOF){ //enquanto não chegar no fim do arquivo
        //transformar de minusculo para maiusculo
        if(c >= 'a' && c<= 'z'){ //dentro da tabela ascii a - z -> A - Z
            c = c - 32;
            fputc(c, write);
            continue;
        }
        fputc(c, write);
    }

    //fecha o arquivo
    fclose(arq);
    fclose(write);

    return 0;
}