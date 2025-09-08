#include "pelucia.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int maq_aleat(unsigned int max){
    return((rand()%max));
}

int num_aleat(){
    return ((rand()%101));
}

//Se existe máquinas ainda, se não encerrar o dia
int existe_maquinas(struct loja *loja){
    int existe;
    if(loja->numero_maquinas == 0)
        existe = 0;
    else 
        existe = 1;

    return existe;
}


void cria_maquinas(struct loja *loja, int id){
    struct maquina_pelucia *novo, *aux, *auxAnt;

    if(loja == NULL)
        return;

    if(!(novo = malloc(sizeof(struct maquina_pelucia))))
        return;

    loja->numero_maquinas++;
    novo->id = id;
    novo->probabilidade = num_aleat();

    

    //Se é o primeiro elemento
    if(loja->inicio == NULL){
        loja->inicio = novo;
        novo->anterior = novo;
        novo->proximo = novo;

        

        return;
    } else {
        //Fazer prioridade baseado na probabilidade

        

        aux = loja->inicio;

        

        //Se substituir o início 
        //Se for igual, toma o lugar
        if(novo->probabilidade >= loja->inicio->probabilidade){
           
            novo->proximo = loja->inicio;
            novo->anterior = loja->inicio->anterior;
            loja->inicio->anterior->proximo = novo;
            loja->inicio->anterior = novo;
            

            loja->inicio = novo;


            return;
        }

        //Encontrar posição
        while(aux->proximo != loja->inicio){
            if(aux->probabilidade <= novo->probabilidade){
                novo->proximo = aux;
                auxAnt->proximo = novo;
                aux->anterior = novo;
                novo->anterior = auxAnt;

               
                return;
            }
            auxAnt = aux;
            aux = aux->proximo;
        }

        //Menor valor possível

        loja->inicio->anterior->proximo = novo;
        novo->proximo = loja->inicio;
        novo->anterior = loja->inicio->anterior;
        loja->inicio->anterior = novo;

      
    }

    
    
}

struct loja* criar_loja (unsigned int numero_maquinas){
    struct loja *loja;

    if(!(loja = malloc(sizeof(struct loja))))
        return NULL;

    loja->inicio = NULL;
    loja->numero_maquinas = numero_maquinas;

    for(unsigned int i = 0; i < numero_maquinas; i++){
        cria_maquinas(loja, i);
    }

    

    return loja;
}

void retira_maquina(struct loja *loja, unsigned int id){
    struct maquina_pelucia *aux = loja->inicio, *prev = NULL;

    while(aux->id != id){
        //Evitar loop infinito
        if(aux == loja->inicio->anterior)
            return;
        prev = aux;
        aux = aux->proximo;        
    }

    loja->numero_maquinas--;

    //Único nodo
    if(aux->proximo == loja->inicio && prev == NULL){
        loja->inicio = NULL;
        free(aux);
        return;
    }

    //Retirar header
    if(aux == loja->inicio){
        prev = loja->inicio->anterior;
        loja->inicio = aux->proximo;
        prev->proximo = loja->inicio;
        loja->inicio->anterior = prev;
        free(aux);
    } else if(aux->proximo == loja->inicio){
        prev->proximo = loja->inicio;
        loja->inicio = prev;
        free(aux);
    } else{
        prev->proximo = aux->proximo;
        aux->proximo->anterior = prev;
        free(aux);
    }

}

int jogar (struct loja *loja){
    struct maquina_pelucia *atual = loja->inicio;

    // Se não existir máquinas na loja, encerrar o dia
    if(!existe_maquinas(loja) || loja == NULL || loja->inicio == NULL){
        return -1;
    }

    //Jogada 
    unsigned int jogada = num_aleat();
    unsigned int aleat_maq = maq_aleat(loja->numero_maquinas);


    for(unsigned int i = 0; i < aleat_maq; i++){
        atual = atual->proximo;
    }


    //Ganhar: retirar a primeira máquina
    if(jogada < atual->probabilidade){
        retira_maquina(loja, atual->id);
    }

    return 1;

}

void encerrar_dia (struct loja *loja){
    //Imprimir as máquinas que existem
    if(loja == NULL || loja->inicio == NULL)
        return;


    //Se não existe máquinas na loja
    if(!existe_maquinas(loja))
        return;

    struct maquina_pelucia *aux = loja->inicio;

    while(aux->proximo != loja->inicio){
        printf("Maquina: %d , Probabilidade: %d \n", aux->id, aux->probabilidade);
        aux = aux->proximo;
    }

    printf("Maquina: %d , Probabilidade: %d \n", aux->id, aux->probabilidade);

    
}

void destruir_loja (struct loja *loja){
    if(loja == NULL)
        return;

    //Loja existe, mas sem nenhuma máquina
    if(loja->inicio == NULL){
        free(loja);
        return;
    }

    struct maquina_pelucia *aux, *remov;

    aux = loja->inicio->proximo;
    while(aux != loja->inicio){
        remov = aux;
        aux = aux->proximo;
        free(remov);
    }

    free(loja->inicio);
    free(loja);
}


