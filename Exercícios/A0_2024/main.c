#include <stdlib.h>
#include <stdio.h>

/* Structs */

typedef struct teddy_machine {
    unsigned int id;
    unsigned int probability;

    struct teddy_machine *next;
    struct teddy_machine *previous;
} teddy_machine;

/* List functions */

teddy_machine* create_list (unsigned int machines){ /* INSTRU��O: Implemente a fun��o */ }

teddy_machine* select_machine (teddy_machine *list, unsigned int place){ /* INSTRU��O: Implemente a fun��o */ }

teddy_machine* remove_machine (teddy_machine *list, teddy_machine *remove) { /* INSTRU��O: Implemente a fun��o */ }

void destroy_list (teddy_machine *list){/* INSTRU��O: Implemente a fun��o */ }

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
    srand(seed);

    unsigned int machine_place, machine_attempt;
    for (unsigned int r = 0; r < rounds; r++){
        printf("\n============================ ROUND %u ============================\n", r+1);
        machine_place = get_place(machines); /* Define a localiza��o da m�quina da rodada, n�o considera m�quinas sem urso */
        machine_attempt = get_attempt(); /* Define a tentativa da rodada; se for menor ou igual � probabilidade da m�quina selecionada, o urso foi pego */

        /* INSTRU��O: Implemente a l�gica do seu programa aqui */
        /* INSTRU��O: Utilize a fun��o "print_attempt" antes do "print_available_machines"! */

        print_available_machines(list);
        printf("==================================================================\n");
    }

    return 0;
}
