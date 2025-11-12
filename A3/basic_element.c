#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "basic_element.h"
#include "utils.h"


element *create_element(int x, int y, int widht, int height, ALLEGRO_BITMAP *sprite){
    element *new_element = (element*) malloc (sizeof(element));
    if(!new_element)
        return NULL;

    new_element->x = x;
    new_element->y = y;
    new_element->width = widht;
    new_element->height = height;

    must_init(sprite, "basic element");

    return new_element;

}

int *check_boundaries(int x, int y, int width, int height){
    
}

void destroy_element(element *element){
    if(element)
        free(element);
}
