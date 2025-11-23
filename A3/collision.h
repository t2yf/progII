#ifndef _COLLISION_
#define _COLLISION_


#define TRUE 1
#define FALSE 0

//compara as extremidades e retorna 1 em caso de colisao e 0 caso contrario
int simple_collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

int four_sides_collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

int complex_collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

#endif