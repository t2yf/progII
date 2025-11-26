#ifndef __ENEMIES__ 																												
#define __ENEMIES__	
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "element.h"
#include "utils.h"
#include "joystick.h"
//#include "character.h"


#define IDLE 0
#define RUNNER 1
#define PATROL 2

#define PATROL_VEL  10
#define RUNNER_VEL 20

typedef struct enemie{
    element *basics;
    int vx; //velocidade do inimigo
    int damage; //quanto de dano o inimigo dá
    int type_of_movement; // posição e direção

    float sourceX;
    float sourceY;


} enemie;

enemie *enemie_create(int x, int y, int width, int height, int damage, int type_of_movement,float sourceX, float sourceY, ALLEGRO_BITMAP *sprite);
int enemie_move(enemie *badnik, char direction, int sprite_mult_factor, int vel_relative);
void enemie_destroy(enemie *badnik);



#endif