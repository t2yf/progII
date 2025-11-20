
#include "collision.h"
#include <stdio.h>


int simple_collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if(ax1 > bx2) return FALSE;
    if(ax2 < bx1) return FALSE;
    if(ay1 > by2) return FALSE;
    if(ay2 < by1) return FALSE;

    return TRUE;
}


int complex_collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2){
    int collision = FALSE;

    if(ax1 >= bx2) 
        return FALSE;
    
    if(ax2 <= bx1) 
        return FALSE;
    

    if(ax1 <= bx1){
        
        collision = 1; //horizontal left-right collide
    }
    else{ 
        
        collision = 2; //horizontal right-left collide
    }

   // printf("collision a %d\n ", collision);

    //confere as alturas para saber se a colisão aconteceu
    if(ay1 >= by2) 
        return FALSE;
    if(ay2 <= by1) 
        return FALSE;

    if(ay1 == by1)
        return collision;
    else if (ay2 <= by2)
        collision += 2; //retorna 3 se for cima pra baixo pela esquerda e 4 se pela direita
    //else collision +=  4; //de baixo para cima ay2 > by2 5 se for pela esquerda e 6 pela direita

   // printf("collision b %d\n ", collision);
   
    return collision;
}

// int complex_collide(int ax1, int ay1, int ax2, int ay2,
//                     int bx1, int by1, int bx2, int by2){
//     int collision = 0;

//     if (ax1 >= bx2) {
//         collision = 0;
//         return collision;
//     }
//     if (ax2 <= bx1) {
//         collision = 0;
//         return collision;
//     }

//     if (ax1 <= bx1){
//         printf("collision 1\n");
//         collision = 1;
//     } else {
//         printf("collision 2\n");
//         collision = 2;
//     }

//     if (ay1 >= by2) {
//         collision = 0;
//         return collision;
//     }
//     if (ay2 <= by1) {
//         collision = 0;
//         return collision;
//     }

//     if (ay1 == by1) {
//         printf("returning early: ay1 == by1 (%d == %d). collision=%d\n", ay1, by1, collision);
       
//         return collision;
//     } else if (ay2 <= by2) {
//         collision += 2;
//     } else {
//         collision += 4;
//     }

//     printf("Valor collision %d\n", collision);
 
//     return collision;
// }
