#ifndef __ELEMENT__ 																												
#define __ELEMENT__	

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define MAX_MAP_BOUNDARIE_X 6400 // x máximo do background 
#define MIN_MAP_BOUNDARIE_X 0 // x minimo do background

typedef struct element{
    int x, y; //posição
    int width, height; //tamanho

    ALLEGRO_BITMAP *sprite; //sprite do elemento

}element;

element *create_element(int x, int y, int width, int height, ALLEGRO_BITMAP *sprite);
void destroy_element(element *element);

#endif