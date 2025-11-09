#include <stdlib.h>
#include "Square.h"

square* square_create(unsigned char side, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){						//Implementação da função "square_create"

	if ((x - side/2 < 0) || (x + side/2 > max_x) || (y - side/2 < 0) || (y + side/2 > max_y)) return NULL;										//Verifica se a posição inicial é válida; caso não seja, retorna NULL

	square *new_square = (square*) malloc(sizeof(square));																						//Aloca memória na heap para um novo quadrado
	new_square->side = side;																													//Insere o tamanho do lado de um quadrado
	new_square->x = x;																															//Insere a posição inicial central de X
	new_square->y = y;																															//Insere a posição inicial central de Y
	new_square->control = joystick_create();																									//Insere o elemento de controle do quadrado (!)
	new_square->walk = square_move_x;
	return new_square;																															//Retorna o novo quadrado
}


void square_move_x(square *element, char steps, unsigned char direction){
	if(direction){
		if((element->x - steps*SQUARE_STEP) - element->side/2 >= 0) element->x = element->x - steps*SQUARE_STEP; //mover para esquerda
	} else{
		if((element->x + steps*SQUARE_STEP) + element->side/2 <= X_SCREEN) element->x = element->x + steps*SQUARE_STEP; //mover para direita
	}
}

void square_move_y(square *element, char steps, unsigned char direction){
	if(direction){
		if((element->y - steps*SQUARE_STEP) - element->side/2 >= 0) element->y = element->y - steps*SQUARE_STEP; //mover para cima
	} else{
		if((element->y + steps*SQUARE_STEP) + element->side/2 <= Y_SCREEN) element->y = element->y + steps*SQUARE_STEP; //mover para baixo
	}
}


void square_destroy(square *element){																											//Implementação da função "square_destroy"
	joystick_destroy(element->control);																											//Destrói controle do quadrado (!)
	free(element);																																//Libera a memória do quadrado na heap
}