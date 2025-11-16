#include <stdio.h>
#include <stdlib.h>


#include "character.h"



#define MAX_X 1280 // borda máx em x
#define MAX_Y 720  // borda máx em y



#define VEL_X  30 //velocidade em X
//[TODO] mudar qtde 

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

    new_charac->vy = 0;
    new_charac->vx = 30;
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
        actor->vx = -VEL_X;
        
         // Fixar câmera caso esteja nos limites
         if (!((actor->basics->x - 1*actor->vx) >=  4 * actor->basics->width)){
            //printf("está no limite\n");
        
            actor->fix_camera = 1;
            return;
        }
    }

    if(direction == RIGHT){
        actor->vx = VEL_X;
        //Se estiver na máxima direita, não ultrapassar
      
        
        // Fixar câmera caso esteja nos limites
        if(!((actor->basics->x + 1*actor->vx)<= MAX_X - 10 *actor->basics->width) ){
           // printf("está no limite\n");
            actor->fix_camera = 1;
            return;
        }
    }


    actor->fix_camera = 0;
    actor->basics->x += actor->vx;
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

