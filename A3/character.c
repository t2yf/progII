#include <stdio.h>
#include <stdlib.h>


#include "character.h"



#define MAX_X 1280 // borda máx em x
#define MAX_Y 720  // borda máx em y

#define STEPS  50


character *character_create(int x, int y, int width, int height, ALLEGRO_BITMAP *sprite){
    character *new_charac = (character *) malloc (sizeof(character));
    if(!new_charac)
        return NULL;

    new_charac->basics = create_element(x, y, width, height, sprite);
    new_charac->control = joystick_create();
    if(!new_charac->control){
        free(new_charac);
        printf("Erro ao criar controle do personagem\n");
        return NULL;
    }

    new_charac->position = RIGHT;
    new_charac->walk = character_move_x;

    return new_charac;
}

void character_move_x(character *actor, char direction){
    //TODO cuidar de se passar da tela
    if(direction == LEFT){
        actor->basics->x -= STEPS;
    }

    if(direction == RIGHT){
        actor->basics->x += STEPS;
        //printf("%d\n", actor->basics->x);
    }
}


void character_destroy(character *actor){
    if(actor){
        destroy_element(actor->basics);
        free(actor);
    }
}


//Acho que em geral todo personagem vai ter um bitmap, altura, largura
//Tanto shadow quanto os badniks andam, então daria para fazer os atributos básicos, incluindo andar para shadow e badniks herdarem então fazer um character.c(?)