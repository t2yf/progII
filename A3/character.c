#include <stdio.h>
#include <stdlib.h>


#include "character.h"



#define MAX_X 1280 // borda máx em x
#define MAX_Y 720  // borda máx em y
#define MAX_MAP_BOUNDARIE_X 1280 // tamanho máximo do background 
#define MIN_MAP_BOUNDARIE_X 0


#define STEPS  30
//[TODO] mudar qtde steps

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
    new_charac->ground = 1;
    new_charac->fix_camera = 0;

    new_charac->walk = character_move_x;
    new_charac->jump = character_move_y;

    return new_charac;
}

void character_move_x(character *actor, char direction){
    //TODO cuidar de se passar da tela
    if(direction == LEFT){
        actor->basics->x -= STEPS;
        //Se estiver na máxima esquerda, não deixar ele ultrapassar
        if(actor->basics->x < MIN_MAP_BOUNDARIE_X){
            actor->basics->x = MIN_MAP_BOUNDARIE_X;
        }
        //[TODO] se ele quiser voltar para a esquerda com rolling background
    }

    if(direction == RIGHT){
        actor->basics->x += STEPS;
        //Se estiver na máxima direita, não ultrapassar
        if(actor->basics->x > MAX_MAP_BOUNDARIE_X  - actor->basics->width*SPRITE_MULT_FACTOR){
            actor->basics->x = MAX_MAP_BOUNDARIE_X  - actor->basics->width*SPRITE_MULT_FACTOR;
        }
    }
}


void character_move_y(character *actor, char direction){
    //[TODO] fazer o rasteijar
    if(direction == DOWN){
        actor->basics->y -= actor->vy;
    }

    if(direction == UP){
        actor->basics->y -= actor->vy;
    }
}

void character_destroy(character *actor){
    if(actor){
        destroy_element(actor->basics);
        free(actor);
    }
}

