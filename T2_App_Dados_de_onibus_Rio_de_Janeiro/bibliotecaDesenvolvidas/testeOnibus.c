#include "dadosOnibus.h"
#include <stdio.h>

//Arquivo para testar a biblioteca dadosOnibus.h
//Esse programa pede uma linha e imprime todas as ordens de onibus circulando naquela linha

int main (void){
	char linha[8];
	float velocidade;
	double latitude;
	double longitude;
	
	Onibus* lista;
	
	printf("Digite linha a ser consultada\n");
	fflush(stdin);
	scanf("%7[^\n]",linha);


	lista = infoLinha(linha);

	imprimeListaOnibusApenasOrdem(lista);
		
}
