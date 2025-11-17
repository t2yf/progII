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
#define RUNNER_VEL 30

typedef struct enemie{
    element *basics;
    int vx; //velocidade do inimigo
    int damage; //quanto de dano o inimigo dá
    int type_of_movement; // posição e direção
  //  int dir;

    float sourceX;
    float sourceY;

    //void (*walk)(struct enemie *badnik, char direction);


} enemie;

enemie *enemie_create(int x, int y, int width, int height, int damage, int type_of_movement,float sourceX, float sourceY, ALLEGRO_BITMAP *sprite);
int enemie_move(enemie *badnik, char direction, int sprite_mult_factor, int vel_relative);//void enemie_move_x(enemie *badnik, char direction);
void enemie_destroy(enemie *badnik);
//void show_enemie(enemie *badnik, ALLEGRO_BITMAP *sprite, int fps, float sourceX, float sourceY);


#endif