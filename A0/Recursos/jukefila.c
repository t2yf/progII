#include <stdlib.h>
#include <stdio.h>
#include "jukefila.h"
#include "pedido.h"


jukefila* criar_jukefila(){
    jukefila *f;

    f = (jukefila*) malloc (sizeof(jukefila));

    if(!f){
        printf("Fila não alocada\n");
        return NULL;
    }

    f->inicio = NULL;
    f->final = NULL;

    return f;
}
void inserir_jukefila(pedido* elemento, jukefila* fila){
    pedido *aux, *auxAnt;
    
    /*fila vazia*/
    if(!fila){
        printf("Fila não alocada\n");
        return;
    }

    /*primeiro elemento*/
    if(fila->inicio == NULL){
        fila->inicio = elemento;
        fila->final = elemento;
        elemento->proximo = NULL;
        elemento->anterior = NULL;


        return;
    }

    aux = fila->inicio;
    /*inserir no inicio*/
    if(elemento->valor > aux->valor){
        elemento->proximo = aux;
        aux->anterior = elemento;
        elemento->anterior = NULL;
        fila->inicio = elemento;



        return;
    }
    
    /*inserir na posição*/
    /*chegar na posição*/
    aux = fila->inicio;
    while(aux != NULL){
        if(elemento->valor > aux->valor){
            elemento->proximo = aux;
            elemento->anterior = auxAnt;
            aux->anterior = elemento;
            auxAnt->proximo = elemento;


            return;
        }
        auxAnt = aux;
        aux = aux->proximo;
    }

    /*inserir no final*/
    aux = fila->final;

    aux->proximo = elemento;
    elemento->proximo = NULL;
    fila->final = elemento;



}
pedido* consumir_jukefila(jukefila* fila){
    pedido *aux;

    /*fila vazia*/
    if(fila == NULL|| fila->inicio ==NULL){
        return NULL;
    }

    aux = fila->inicio;
    if(aux->proximo  == NULL){
        fila->inicio = fila->final = NULL;
        return aux;
    }

    fila->inicio = aux->proximo;
    fila->inicio->anterior = NULL;

    return aux;  
}
unsigned int contar_jukefila(jukefila* fila){
    pedido *aux;
    int count;

    /*fila vazia*/
    if(!fila){
        return 0;
    }

    aux = fila->inicio;
    count = 0;
    while(aux != NULL){
        count++;
        aux = aux->proximo;
    }

    return count;
}
void destruir_jukefila(jukefila *fila){
    pedido *aux;

    /*fila vazia*/
    if(fila == NULL){
        printf("Fila não alocada\n");
        return;
    }

    aux = fila->inicio;

    while(aux != NULL){
        fila->inicio = fila->inicio->proximo;
        destruir_pedido(aux);
        aux = fila->inicio;
    }

    free(fila);
}