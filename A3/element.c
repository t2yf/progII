#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "element.h"


element *create_element(int x, int y, int width, int height, ALLEGRO_BITMAP *sprite){

    element *new_element = (element*) malloc (sizeof(element));
    if(!new_element)
        return NULL;

  
    new_element->x = x;
    new_element->y = y;
    new_element->width = width;
    new_element->height = height;



    return new_element;

}


void destroy_element(element *element){
    if(element)
        free(element);
}
