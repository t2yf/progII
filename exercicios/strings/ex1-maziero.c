/* Ler uma string da entrada padrão e escrevê-la na saída padrão ao contrário (do final para o início), de forma similar ao comando rev do shell UNIX. */

#include <stdio.h>
#include <string.h> //biblioteca padrão do C

int main(){
    char string[11];
    scanf("%[^\n]", string);
    getchar(); //limpar o buffer

    for(int i= strlen(string); i>= 0 ; i--){
        putchar(string[i]);
    }

    return 0;
}