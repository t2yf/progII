/*Remover de uma string caracteres repetidos em sequência (rr, ss, ee, etc), sem usar string auxiliar.*/

#include <stdio.h>
#include <string.h>

int main(){
    char string[101];

    scanf("%[^\n]", string);
    getchar(); //limpar o buffer

    int i = 0;
    while (string[i] != '\0') {
        if (string[i] == string[i + 1]) {
            // Encontrou início de repetição
            char repetido = string[i];
            int j = i + 1;

            // Avança j enquanto o caractere se repetir
            while (string[j] == repetido) {
                j++;
            }

            // Remove os caracteres repetidos deslocando o resto da string
            int k = i + 1;
            while (string[j] != '\0') {
                string[k++] = string[j++];
            }

            // Finaliza a string com \0
            string[k] = '\0';

            // Não incrementa i, porque pode ter mais repetições depois da remoção
        } else {
            i++; // avança se não houver repetição
        }
    }

    puts(string);

    return 0;
}