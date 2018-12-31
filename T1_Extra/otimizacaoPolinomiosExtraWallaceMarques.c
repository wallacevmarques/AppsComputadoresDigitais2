#include <math.h>
#include "otimizacaoPolinomiosExtraWallaceMarques.h"

#define precisao 0.001

float calculaMaximoExtra(float *polinomio, int grau, float *intervalo, float *xmax) {
	
	//Cria variaveis que armazenam o resultado da funcao durante sua execucao
	float xmaxTemp;
	float ymaxTemp;
	
	//Inicializa xmaxTemp e ymaxTemp
	xmaxTemp = intervalo[0];
	ymaxTemp = 0;
	int n;
	for (n=0; n<=grau; n++) {
		ymaxTemp = ymaxTemp + polinomio[n]*pow(xmaxTemp,n);
	}
	
	//Obtem as coordenadas do ponto de maximo
	float x;
	float y;
	for (x=intervalo[0]; x<=intervalo[1]; x=x+precisao) {
		
		//Calcula o valor "y" do polinomio em funcao valor atual de "x"
		y = 0;
		int n;
		for (n=0; n<=grau; n++) {
			y = y + polinomio[n]*pow(x,n);
		}
		
		//Verifica se o valor atual de "y" eh maior que o valor maximo encontrado ate entao
		//Caso seja, atribui os valores atuais de "x" e "y" a xmaxTemp e ymaxTemp respectivamente
		if (y > ymaxTemp) {
			xmaxTemp = x;
			ymaxTemp = y;
		}
		
	}
	
	//Retorna o resultado da funcao
	*xmax = xmaxTemp;
	return  ymaxTemp;
	
}

float calculaMinimoExtra(float *polinomio, int grau, float *intervalo, float *xmin) {
	
	//Cria variaveis que armazenam o resultado da funcao durante sua execucao
	float xminTemp;
	float yminTemp;
	
	//Inicializa xmin e ymin
	xminTemp = intervalo[0];
	yminTemp = 0;
	int n;
	for (n=0; n<=grau; n++) {
		yminTemp = yminTemp + polinomio[n]*pow(xminTemp,n);
	}
	
	//Obtem as coordenadas do ponto de minimo
	float x;
	float y;
	for (x=intervalo[0]; x<=intervalo[1]; x=x+precisao) {
		
		//Calcula o valor "y" do polinomio em funcao valor atual de "x"
		y = 0;
		int n;
		for (n=0; n<=grau; n++) {
			y = y + polinomio[n]*pow(x,n);
		}
		
		//Verifica se o valor atual de "y" eh menor que o valor minimo encontrado ate entao
		//Caso seja, atribui os valores atuais de "x" e "y" a xminTemp e yminTemp respectivamente
		if (y < yminTemp) {
			xminTemp = x;
			yminTemp = y;
		}
		
	}
	
	//Retorna o resultado da funcao
	*xmin = xminTemp;
	return  yminTemp;
	
}
