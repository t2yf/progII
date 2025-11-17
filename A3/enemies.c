#include <stdio.h>
#include <stdlib.h>


#include "enemies.h"



enemie *enemie_create(int x, int y, int width, int height, int damage, int type_of_movement,float sourceX, float sourceY, ALLEGRO_BITMAP *sprite){
    enemie *new_enemie = (enemie *) malloc (sizeof(enemie));
    if(!new_enemie)
        return NULL;

    new_enemie->basics = create_element(x, y, width, height, sprite);
    new_enemie->vx = 0;
    new_enemie->damage = damage;
    new_enemie->type_of_movement = type_of_movement;
    new_enemie->sourceX = sourceX;
    new_enemie->sourceY = sourceY;
   // new_enemie->dir = 1;

    //new_enemie->walk = enemie_move_x;

    return new_enemie;
}

// a cada batida de relógio ele vai chegando ao inicio do mapa
int update_runner_enemie(enemie *badnik, int vel_relative){
    badnik->vx = -RUNNER_VEL;
    //Se ultrapassar o limite esquerdo do mapa, esse badnik pode sumir
    if(badnik->basics->x - badnik->vx < 0){
        return -1;
    }

    badnik->basics->x += badnik->vx - vel_relative;
    return 1;
}
//ficar andando de um lado pra outro
//[TODO] tem q add velocidade relativa ao mapa tando aqui quanto no runner
int update_patrol_enemie(enemie *badnik, char direction, int sprite_mult_factor, int vel_relative){
    if(direction == -1){
        badnik->vx = -PATROL_VEL;
        //Não passar dos limites
        if(badnik->basics->x < MIN_MAP_BOUNDARIE_X){
            badnik->basics->x = MIN_MAP_BOUNDARIE_X;
        }
    }
    if(direction == 1){
        badnik->vx = PATROL_VEL;
        //Não passar dos limites
        if(badnik->basics->x > MAX_MAP_BOUNDARIE_X - badnik->basics->width*sprite_mult_factor){
            badnik->basics->x = MAX_MAP_BOUNDARIE_X - badnik->basics->width*sprite_mult_factor;
        }
    }
   

    badnik->basics->x +=  badnik->vx - vel_relative;
    return 1;
}

int enemie_move(enemie *badnik, char direction, int sprite_mult_factor, int vel_relative){
    if(badnik->type_of_movement == RUNNER){
        return update_runner_enemie(badnik, vel_relative);
    }
    if(badnik->type_of_movement == PATROL){
        return update_patrol_enemie(badnik, direction, sprite_mult_factor, vel_relative);
    }

    return 0;
}


void enemie_destroy(enemie *badnik){
    if(badnik){
        destroy_element(badnik->basics);
        free(badnik);
    }
}