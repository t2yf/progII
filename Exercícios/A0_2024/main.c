#include <stdlib.h>
#include <stdio.h>

/* Structs */

typedef struct teddy_machine {
    unsigned int id;
    unsigned int probability;

    struct teddy_machine *next;
    struct teddy_machine *previous;
} teddy_machine;

/* Auxiliary functions */
int isTherePlush(teddy_machine *list){
    if(list == NULL)
        return 0;
    return 1;
}

/* List functions */

teddy_machine* create_list (unsigned int machines){ 
    struct teddy_machine *first, *new, *aux;

    if(!(first = malloc(sizeof(struct teddy_machine))))
        return NULL;

    first->id = 1;
    first->probability = 5;
    first->next = first;
    first->previous = first;

    aux = first;

    for(int i = 2; i<=machines; i++){
        if(!(new = malloc(sizeof(struct teddy_machine))))
            return NULL;
        
        new->id = i;
        new->probability = 5;

        aux->next = new;
        new->previous = aux;
        new->next = first;
        first->previous = new;

        aux = new;
    }

    return first;
 }

teddy_machine* select_machine (teddy_machine *list, unsigned int place){ 
    struct teddy_machine *aux;

    aux = list;
    
    //o quanto anda a pessoa selecionando a máquina
    for(int i = 1; i <= place; i++){
        aux = aux->next;
    }

    return aux;
 }

teddy_machine* remove_machine (teddy_machine *list, teddy_machine *remove) { 
    struct teddy_machine *aux;

    printf("list: %d\n", list->id);
    printf("remove: %d\n", remove->id);

    if(list == NULL || remove == NULL)
        return NULL;

    //se for primeiro item da 
    if(list == remove){
        aux = list->next;
        list->previous->next = aux;
        aux->previous = list->previous;

        free(remove);
        return list;
    }

    //se for último item da lista
    if(list->previous == remove){
        aux = remove->previous;
        aux->next = list;
        list->previous = aux;

        free(remove);
        return list;
    }

    //se não for primeiro nem último
    remove->previous->next = remove->next;
    remove->next->previous = remove->previous;

    free(remove);
    return list;
 }

void destroy_list (teddy_machine *list){
    struct teddy_machine *aux;
    if(list == NULL)
        return;

    aux = list->next;

    while(aux != list){
        aux = aux->next;
        free(aux->previous);
    }

    free(list);
}

/* Randomization functions */

unsigned int get_place(unsigned int machines){
    return rand() % machines;
}

unsigned int get_attempt(){
    return rand() % 100 + 1;
}

/* Printing functions */

void print_attempt(teddy_machine *machine, unsigned int attempt){

    if (attempt <= machine->probability) printf("O URSO DA MAQUINA %u [FOI] OBTIDO!\n", machine->id);
    else printf("O URSO DA MAQUINA %u [NAO FOI] OBTIDO!\n", machine->id);
}

void print_available_machines(teddy_machine *list){

    if (!list) printf("NAO HA MAQUINAS DISPONIVEIS!\n");
    else{
        teddy_machine *i = list;
        do {
            printf("PROBABILIDADE DA MAQUINA %u: %u\n", i->id, i->probability);
            i = (teddy_machine*) i->next;
        } while((i) && (i != list));
    }

}

/* Main function */

int main(int argc, char *argv[]){
    unsigned int machines, rounds, seed;
    int rv = 0;

    if (argc != 4) rv = -1;
    else {
        machines = atoi(argv[1]);
        if (!machines) rv = -2;
        else {
            rounds = atoi(argv[2]);
            if (!rounds) rv = -3;
            else {
                seed = atoi(argv[3]);
                if (!seed) rv = -4;
            }
        }
    }

    if (rv){
        printf("USO: main [NR. DE MAQUINAS] [NR. DE RODADAS] [SEMENTE DE RAND.]\n");
        return rv;
    }

    teddy_machine *list = create_list(machines);
    teddy_machine *selected;
    srand(seed);

    unsigned int machine_place, machine_attempt;
    for (unsigned int r = 0; r < rounds; r++){
        printf("\n============================ ROUND %u ============================\n", r+1);
        machine_place = get_place(machines); /* Define a localiza��o da m�quina da rodada, n�o considera m�quinas sem urso */
        machine_attempt = get_attempt(); /* Define a tentativa da rodada; se for menor ou igual � probabilidade da m�quina selecionada, o urso foi pego */

        //se existir máquinas ainda, ou seja, se a lista de máquinas não for null
        if(!isTherePlush(list)){
            break;
        }

       selected = select_machine(list, machine_place);
       print_attempt(selected, machine_attempt);
        if(machine_attempt <= selected->probability ){
            //ganhar
            remove_machine(list, selected);
        } else
            selected->probability +=2;

        

        print_available_machines(list);
        printf("==================================================================\n");

        selected = NULL;
    }

    destroy_list(list);

    return 0;
}
