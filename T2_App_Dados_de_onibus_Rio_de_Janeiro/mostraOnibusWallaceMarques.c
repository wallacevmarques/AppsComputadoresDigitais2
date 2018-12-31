#include "dadosOnibus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAMANHOORDEM 6

//Programa que pede intervalo de tempo, numero de onibus e as ordens dos onibus a serem testados. 
//Mostra a velocidade desses onibus a cada segundo no intervalo e a linha do onibus de maior velocidade.

int main (void){
	
	int tamanhoStringOrdem = TAMANHOORDEM + 1;
	int tempoAmostragem;
	int quantidadeOnibus;
	int quantidadeOnibusValidos;
	int velocidadeOnibusAtualMedia;
	int velocidadeMaiorMedia;
	int nOnibusMaiorVelocidade;
	char linhaOnibusMaiorVelocidade[8];
	
	Onibus* onibus;
	
	int i;
	int n;
	int t;
	
	//Recebe o número de momentos diferentes a medir
	do {
		printf("Digite o numero de momentos diferentes (ate 200) a medir a velocidade dos onibus: ");
		scanf("%d", &tempoAmostragem);
		if (tempoAmostragem<=0 || tempoAmostragem>200) {
			printf("Numero invalido de momentos! Digite novamente.\n");
		}
		printf("\n");
	} while (tempoAmostragem<=0 || tempoAmostragem>200);
	
	//Recebe o número de ordens de ônibus
	do {
		printf("Digite o numero de onibus (ate 20) a medir a velocidade: ");
		scanf("%d", &quantidadeOnibus);
		if (quantidadeOnibus<=0 || quantidadeOnibus>20) {
			printf("Numero invalido de onibus! Digite novamente.\n");
		}
		printf("\n");
	} while (quantidadeOnibus<=0 || quantidadeOnibus>20);
	printf("\n");
	
	//Recebe as ordens dos ônibus
	printf("Iniciando insercao de numeros de ordem dos onibus...\n\n");
	char **ordensOnibus = (char**) malloc(quantidadeOnibus*sizeof(char*));
	for (i=0; i<quantidadeOnibus; i++) {
		ordensOnibus[i] = (char*) malloc(tamanhoStringOrdem*sizeof(char));
		printf("Digite a ordem do onibus %d: ", i+1);
		fflush(stdin);
		scanf(" %6[^\n]", ordensOnibus[i]);
	}
	printf("\n");
	
	//Testa as ordens dos ônibus
	printf("Testando numeros de ordem dos onibus...\n\n");
	quantidadeOnibusValidos = quantidadeOnibus;
	for (i=0; i<quantidadeOnibus; i++) {
		onibus = infoOnibus(ordensOnibus[i]);
		if (onibus == NULL) {
	    	printf("Erro ao testar o onibus de ordem %s.\n\n", ordensOnibus[i]);
	    	ordensOnibus[i] = NULL;
	    	quantidadeOnibusValidos--;
	   } else {
	   		printf("\n");
	   }
	   free(onibus);
	}
	printf("\n");
	
	//Continua o programa se houver ordens validas. Do contrario, mostra mensagem de problema.
	if (quantidadeOnibusValidos>0) {
		
		//Cria novo vetor de ordens validas
		char **ordensOnibusValidos = (char**) malloc(quantidadeOnibusValidos*sizeof(char*));
		for (i=0, n=0; i<quantidadeOnibus; i++) {
			if (ordensOnibus[i] != NULL) {
	        	ordensOnibusValidos[n] = (char*) malloc(tamanhoStringOrdem*sizeof(char));
	        	strcpy(ordensOnibusValidos[n], ordensOnibus[i]);
		    	n++;
			}
		}
		
		//Aloca matriz de velocidades dos onibus
		float **velocidadesOnibus = (float**) malloc(quantidadeOnibusValidos*sizeof(float*));
		for (i=0; i<quantidadeOnibusValidos; i++) {
			velocidadesOnibus[i] = (float*) malloc(tempoAmostragem*sizeof(float));
		}						
		
		//Faz medição das velocidade dos onibus
		printf("Fazendo medicao das velocidades dos onibus...\n\n");
		for (t=0; t<tempoAmostragem; t++) {
			for (n=0; n<quantidadeOnibusValidos; n++) {
				onibus = infoOnibus(ordensOnibusValidos[n]);
				velocidadesOnibus[n][t] = (*onibus).velocidade;
				free(onibus);
			}
			sleep(1);
		}
		
		//Verifica qual onibus tem a maior velocidade media e obtem sua linha
		velocidadeMaiorMedia = 0;
		for (n=0; n<quantidadeOnibusValidos; n++) {
			velocidadeOnibusAtualMedia = 0;
			for (t=0; t<tempoAmostragem; t++) {
				velocidadeOnibusAtualMedia += velocidadesOnibus[n][t];
			}
			velocidadeOnibusAtualMedia /= tempoAmostragem;
			if(velocidadeOnibusAtualMedia>=velocidadeMaiorMedia) {
				nOnibusMaiorVelocidade = n;
				velocidadeMaiorMedia = velocidadeOnibusAtualMedia;
			}
		}
		onibus = infoOnibus(ordensOnibusValidos[nOnibusMaiorVelocidade]);
		strcpy(linhaOnibusMaiorVelocidade, (*onibus).linha);
		free(onibus);
		printf("\n\n");
	
		//Mostra as medidas de velocidade obtidas para cada ordem de onibus
		printf("Resultado das medicoes:\n\n");
		for (n=0; n<quantidadeOnibusValidos; n++) {
			printf("%s:", ordensOnibusValidos[n]);
			for (t=0; t<tempoAmostragem; t++) {
				if (t < tempoAmostragem-1) {
					printf(" %.0f Km/h;", velocidadesOnibus[n][t]);
				} else {
					printf(" %.0f Km/h.\n", velocidadesOnibus[n][t]);
				}
			}
		}
		printf("\n");
	
		//Mostra linha com maior velocidade
		if (strcmp((*onibus).linha, "") != 0) {
			printf("Linha com maior velocidade: %s\n\n\n", linhaOnibusMaiorVelocidade);
		} else {
			printf("Nao foi possivel determinar a linha do onibus de maior velocidade.\n\n\n");
		}
		
		//Desaloca toda memoria dinamicamente alocada
		for (i=0; i<quantidadeOnibus; i++) {
			free(ordensOnibus[i]);
		}
		free(ordensOnibus);
		for (i=0; i<quantidadeOnibusValidos; i++) {
			free(ordensOnibusValidos[i]);
		}
		free(ordensOnibusValidos);
		for (i=0; i<quantidadeOnibusValidos; i++) {
			free(velocidadesOnibus[i]);
		}
		free(velocidadesOnibus);
		
	} else {
		//Avisa erro em todos os onibus
		printf("Houve problemas ao testar todos os onibus.\n\n\n");
	}
	
	//Encerra o programa
	printf("Encerrando o programa...\n");
	return 0;
}
