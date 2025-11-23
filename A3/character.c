#include <stdio.h>
#include <stdlib.h>


#include "character.h"



#define MAX_X 1280 // borda máx em x
#define MAX_Y 720  // borda máx em y



#define CHARAC_VEL_X  30 //velocidade em X
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
    new_charac->hp = 10;
    new_charac->vy = 0;
    new_charac->vx = CHARAC_VEL_X;
    new_charac->position = RIGHT;
    new_charac->ground = 1;
    new_charac->fix_camera = 0;
    new_charac->collide = 0;
    new_charac->crounch = 0;
    new_charac->stamina = STAMINA;

    new_charac->walk = character_move_x;
    new_charac->jump = character_move_y;

    return new_charac;
}

void character_move_x(character *actor, char direction){
    //TODO cuidar de se passar da tela
    if(direction == LEFT){
        actor->vx = -CHARAC_VEL_X;
        
         // Fixar câmera caso esteja nos limites
         if (!((actor->basics->x - 1*actor->vx) >=  4 * actor->basics->width)){
            //printf("está no limite\n");
        
            actor->fix_camera = 1;
            return;
        }
    }

    if(direction == RIGHT){
        actor->vx = CHARAC_VEL_X;
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

int character_collide(character *actor, element *element, int map_ajustment){

    int op = complex_collide(actor->basics->x, actor->basics->y, actor->basics->x + actor->basics->width*SPRITE_MULT_FACTOR, actor->basics->y + actor->basics->height*SPRITE_MULT_FACTOR,
                              element->x - map_ajustment, 550, element->x + element->width*2 - map_ajustment, element->y + element->height*2);
    //printf("op %d\n", op);
    switch(op){
        case 1:
        {
            actor->basics->x = element->x - map_ajustment - actor->basics->width*SPRITE_MULT_FACTOR;
            break;
        }
        case 2:
        {                
            actor->basics->x = element->x - map_ajustment + element->width*2;
            break;
        }
        case 3:
        {                 
            actor->basics->y = 550 - element->height*2;
            break;
        }
        case 4:
        {
            actor->basics->y = 550 - element->height*2;
            break;
        }
        case 5:
        {                    
            actor->basics->y = element->y + element->height*2;
            break;
        }
        case 6:
        {                    
            actor->basics->y = element->y + element->height*2;
            break;
        }
        default:                   
            return 0;
        }

        if(actor->basics->x < actor->basics->width) {
            actor->basics->x = actor->basics->width;
        }

        
        return 1;

}


void character_destroy(character *actor){
    if(actor){
        destroy_element(actor->basics);
        free(actor);
    }
}

