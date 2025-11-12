#ifndef __CHARACTER__ 																												
#define __CHARACTER__		

#include "basic_element.h"

#define LEFT    1
#define RIGHT   2
#define UP      3
#define DOWN    4

//criar
//andar
//abaixar
//pular 
//destroy

typedef struct character {
    element *basics;

    //int hp;


    void (*walk)(struct character *actor, char direction);
    //void jump
    //void crouch
    //void fall
    //void idle


} character;

character *character_create(int x, int y, int width, int height, ALLEGRO_BITMAP *sprite);
void *character_move_x(character *actor, char direction);
void *character_move_y(character *actor, char direction);
character *character_jump();
character *character_fall();
character *character_idle();

character *character_destroy(character *actor);

#endif	
