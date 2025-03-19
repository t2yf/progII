#include "fila_jornal.h"

void requisita(char *titulo, char *texto){
    getchar();
    printf("\nDigite o título: ");
    fgets(titulo, 33, stdin);
    printf("Digite o texto: ");
    fgets(texto, 513, stdin);
}

noticia* criar_noticia(){
    noticia *nova = (noticia*)malloc(sizeof(noticia));
    if(!nova){
        return NULL;
    }

    requisitar_noticia(nova->titulo, nova->texto);
    nova->idade = 0;
    nova->prox = NULL;

    return nova;

}

int inserir_noticia(noticia *nova, fila_jornal *fila){
    
    if (fila->final){ //se tem final,não é o único elemento
        fila->final->prox = nova;
        nova->prox = NULL;
        fila->final = nova;
    } else{ //primeiro elemento
        fila->inicio = nova;
        fila->final = nova;
    }



}



int main(){

    return 0;
}