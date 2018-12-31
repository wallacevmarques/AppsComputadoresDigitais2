#include <stdio.h>
#include <stdlib.h>
#include "otimizacaoPolinomiosExtraWallaceMarques.c"

int main (void) {
	
	int codigoOperacao;
	float intervalo[2];
	int grau;
	float *polinomio = NULL;
	float xmax;
	float ymax;
	float xmin;
	float ymin;
	
	do {
		
		//Mostra o menu na tela
		printf("Digite o numero da operacao\n");
		printf("1 - Inserir Polinomio\n");
		printf("2 - Calcular Maximo\n");
		printf("3 - Calcular Minimo\n");
		printf("4 - Saida\n");
		printf("\n");
		
		//Recebe o codigo da operacao
		printf("Opcao: ");
		scanf("%d", &codigoOperacao);
		printf("\n");
		
		
		switch (codigoOperacao) {
			
			//Insere o polinomio
			case 1:
				//Recebe os valores maximo e minimo de x no intervalo a ser analisado
				printf("Digite o valor minimo de x para o intervalo em analise: ");
				scanf("%d", &intervalo[0]);
				printf("\n");
				printf("Digite o valor maximo de x para o intervalo em analise: ");
				scanf("%d", &intervalo[1]);
				printf("\n");
				//Recomeca o processo caso o valor seja invalido
				if (intervalo[0] >= intervalo[1]) {
					printf("Intervalo invalido! Escolha uma opcao novamente.\n");
					printf("\n");
					break;
				}
				
				//Recebe o grau do polinomio
				printf("Digite o grau do polinomio: ");
				scanf("%d", &grau);
				printf("\n");
				//Recomeca o processo caso o valor seja invalido
				if (grau<0) {
					printf("Grau de polinomio invalido! Escolha uma opcao novamente.\n");
					printf("\n");
					break;
				}
				
				// Aloca vetor para receber cada coeficiente do polinomio
				polinomio = (float *) malloc(grau*sizeof(float));
				if (polinomio == NULL) {
					printf("Memoria insuficiente\n");
					exit(1);
				}
				
				//Recebe e armazena os dados no vetor
				int i;
				for (i=0; i<=grau; i++) {
					printf("Insira o coeficiente de grau %d do polinomio: ", i);
					scanf("%f", &polinomio[i]);
				}
				printf("\n");
				
				//Calcula as coordenadas do ponto de maximo e de minimo do polinomio
				ymax = calculaMaximoExtra(polinomio, grau, intervalo, &xmax);
				ymin = calculaMinimoExtra(polinomio, grau, intervalo, &xmin);
				
				//Libera o espaco alocado para o vetor de coeficientes do polinomio
				free(polinomio);
			break;
			
			//Mostra o maximo
			case 2:
				//Recomeca o processo caso o valor seja invalido
				if (polinomio == NULL) {
					printf("O polinomio ainda nao foi inserido! Tecle 1 para inserir um polinomio.\n");
					printf("\n");
					break;
				}
				//Mostra as coordenadas do ponto de maximo do polinomio
				printf("As coordenadas do ponto de maximo sao:\n");
				printf("xmax = %f e ymax = %f\n", xmax, ymax);
				printf("\n");
			break;
			
			//Mostra o minimo
			case 3:
				//Recomeca o processo caso o valor seja invalido
				if (polinomio == NULL) {
					printf("O polinomio ainda nao foi inserido! Tecle 1 para inserir um polinomio.\n");
					printf("\n");
					break;
				}
				//Mostra as coordenadas do ponto de minimo do polinomio
				printf("As coordenadas do ponto de minimo sao:\n");
				printf("xmin = %f e ymin = %f\n", xmin, ymin);
				printf("\n");
			break;
			
			//Encerra o programa
			case 4:
				printf("Saindo...\n");
				break;
			break;
			
			//Opcao invalida
			default:
				printf("Opcao invalida! Escolha novamente.\n");
				printf("\n");
			break;
			
		}
		
		
		
		
	} while(codigoOperacao != 4);
	
	return 0;
}
