#include <stdio.h>
#include "pelucia.h"
#include <time.h>
#include <stdlib.h>


int main () {
        

	unsigned int n;
	int opcao, ret;
	struct loja *loja;
	printf("=======================================\n");
	printf("     LOJA DAS MAQUINAS DE PELÚCIA     \n");
	printf("=======================================\n");
	printf("\n");
	printf("        .-''''''-.      .-''''''-.    \n");
	printf("       /          \\    /          \\   \n");
	printf("      /   O    O   \\  /   O    O   \\  \n");
	printf("     |     ||      |  |     ||      | \n");
	printf("     |     ||      |  |     ||      | \n");
	printf("     |   .--'--.   |  |   .--'--.   | \n");
	printf("     |  |      |   |  |  |      |   |  \n");
	printf("     |  |      |   |  |  |      |   |  \n");
	printf("     |  |      |   |  |  |      |   |  \n");
	printf("     |  |______|   |  |  |______|   |  \n");
	printf("      \\          /    \\          /   \n");
	printf("       `--------'      `--------'    \n");
	printf("\n");
	printf("       Venha e ganhe a sua pelúcia!   \n");
	printf("=======================================\n");

	printf ("\nDefina o número de maquinas da loja: ");
	scanf ("%d", &n);

        //srand(time(NULL));
	loja = criar_loja (n);
    encerrar_dia (loja);

	while (1) {
		printf ("Escolha uma das opcoes:\n");
		printf ("(1) Jogar\n");
		printf ("(2) Encerrar dia\n");
		scanf ("%d", &opcao);

		ret = 0;
		if (opcao == 1){
			ret = jogar (loja);
			//encerrar_dia (loja);
			if (ret < 0) {
				encerrar_dia (loja);
				destruir_loja (loja);
				break;
			}
			else if (ret > 0)
				printf ("Ganhou!\n");
			else
				printf ("Perdeu!\n");

		}

		if (opcao == 2){
			encerrar_dia (loja);
			destruir_loja (loja);
			break;
		}
	}
	return 0;
}

