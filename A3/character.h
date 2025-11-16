#ifndef __CHARACTER__ 																												
#define __CHARACTER__		

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "element.h"
#include "utils.h"
#include "joystick.h"

#define SPRITE_MULT_FACTOR 3.5
#define LEFT    1
#define RIGHT   2
#define UP      3
#define DOWN    4
#define GRAVITY 5;
//criar
//andar
//abaixar
//pular 
//destroy

typedef struct character {
    element *basics;
    joystick *control;

    //int hp;
    int position; //direção e posição que ele está

    int ground; 
    int vy;     //velocidade em y
    int fix_camera; //para rolling background
   

    void (*walk)(struct character *actor, char direction);
    void (*jump)(struct character *actor, char direction);

    //void crouch
    //void fall
    //void idle


} character;

character *character_create(int x, int y, int width, int height, ALLEGRO_BITMAP *sprite);
void character_move_x(character *actor, char direction);
void character_move_y(character *actor, char direction);
// character *character_jump();
// character *character_fall();
// character *character_idle();

void character_destroy(character *actor);

#endif	
