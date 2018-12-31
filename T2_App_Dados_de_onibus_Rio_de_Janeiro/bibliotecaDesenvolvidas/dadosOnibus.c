#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dadosOnibus.h"


Onibus* insereOnibus(Onibus* lst, Onibus* novo){
	novo->ant = NULL;
	novo->prox = lst;
	if(lst!= NULL)
		lst->ant = novo;
	return novo;
}



Onibus* realizarRequisicao(char* requisicao){
               	               
	FILE *saidaComando;
	Onibus* lstOnibus=NULL;
	Onibus*	onibusAtual;
	char *stringRecebida;
	char *fracaoString;
	char *prtSalvaFracaoString;
	char *campo;
	char *campoLimpo;
	char *prtSalvaCampo;
	char buffer[TAMANHO_BUFFER];
	int  tamanhoStringRecebida=0;
	int tamanhoCampo;
	int  contadorColchetes=0;
	int i=0,j=0,contadorCampo=0;
	
	char auxChar = '0';
	
	int contadorOnibus = 0;

	/* Executa a requisicao para o servidor da prefeitura */
     
    saidaComando = popen(requisicao, "r");
   	if (saidaComando == NULL) {
    	printf("Nao foi possivel executar o comando para o servidor da prefeitura. Verifique sua conexao com a Internet.\n" );
    	return NULL;
    } 

	
	//A Rotina abaixo vai posicionar o cursor do arquivo para o inicio dos dados
	while((auxChar != EOF) && (contadorColchetes !=2)){
		auxChar = fgetc(saidaComando);
		//Verifica se ja chegou no momento no qual os dados sao recebidos
		//Para tal, tenta encontrar dois colchetes consecutivos
		if (auxChar == '[')
			contadorColchetes++;
		else
			contadorColchetes = 0;
	}
	
	//Caso os dois colchetes consecutivos nao sejam encontrados
	if (auxChar == EOF) {
	  printf("Nao foi possivel ler os dados recebidos. Provavelmente, nao existe dados GPS para essa linha/onibus. \n" );
	  return NULL;
	}
	
	//Aloca dinamicamente a estrutura que armazenara a string recebida. Primeiramente ela eh alocada com o tamanho 1, 
	//depois sera alocada de acordo com o recebimendo da string, utilizando a funcao realloc
	stringRecebida = (char *) malloc((1)*sizeof(char));
	stringRecebida[0] = 0;

	//Apos o dados, Le a string recebida em um buffer
	while (fgets(buffer, sizeof(buffer)-1, saidaComando) != NULL) {
		//Atualiza o tamanho da string recebida com o tamanho da string no buffer
		tamanhoStringRecebida += strlen(buffer);
		//Aloca
		stringRecebida = realloc(stringRecebida,(tamanhoStringRecebida+1)*sizeof(char));
		//Le do buffer e armazena na string
	  	strcat(stringRecebida,buffer);
	}
	

	
	//O delimitar de cada dado eh colchete de fim
	//Assim, fazemos um while para ir encontrando cada dados do onibus

	fracaoString = strtok_r(stringRecebida,"]",&prtSalvaFracaoString);
	do{

			onibusAtual = (Onibus*) malloc(sizeof(Onibus));
			//A variavel fracaostring tem todos os dados do onibus
			//Agora utiliza-se a virgula como limitador de cada dado
			contadorCampo = 0;
			campo = strtok_r(fracaoString,",",&prtSalvaCampo);
			do{
				//Verifica o tamanho do campo, para pode iterar e remover caracteres indesejaveis
				//Essa string limpa vai para a variavel campoLimpo
				tamanhoCampo = strlen(campo);
				if (tamanhoCampo > 1){
					campoLimpo = (char *) malloc((tamanhoCampo+1)*sizeof(char));
					i=0;
					j=0;
					auxChar = campo[i];
					do{
						//Ignora caracteres que nao fazem parte do campo
						if ((auxChar > 31) && (auxChar < 127)){
							if ((auxChar != '\"') && (auxChar != '{') && (auxChar != '}') && (auxChar != ']') && (auxChar != '[') && (auxChar != '\n') && (auxChar != '[')){
								campoLimpo[j]=campo[i];
								j++;
							}
						}
						i++;  
						auxChar = campo[i];
					}while(auxChar != '\0');
					campoLimpo[j] = '\0';
					if (contadorCampo==0){
						strcpy(onibusAtual->data,campoLimpo);
					}
					else if (contadorCampo==1){
						strcpy(onibusAtual->ordem,campoLimpo);
					}
					else if (contadorCampo==2){
						strcpy(onibusAtual->linha,campoLimpo);
					}
					else if (contadorCampo==3){
						onibusAtual->latitude = atof(campoLimpo);
					}
					else if (contadorCampo==4){
						onibusAtual->longitude = atof(campoLimpo);
					}
					else if (contadorCampo == 5){
						onibusAtual->velocidade = atof(campoLimpo);
					}
					else{
						printf("Erro na leitura\n" );
	  					return NULL;
					}					
					contadorCampo++;
				}
				else{
					free(onibusAtual);
					onibusAtual = NULL;
				}
			}while ((campo = strtok_r(NULL,",",&prtSalvaCampo)) != NULL);
			contadorOnibus++;
			//Soh insere algo se onibusAtual != NULL
			if (onibusAtual != NULL){
				lstOnibus = insereOnibus(lstOnibus,onibusAtual);
			}					
	}while ((fracaoString = strtok_r(NULL,"]",&prtSalvaFracaoString)) != NULL);
	
	
	free(stringRecebida);
	pclose(saidaComando);
	return lstOnibus;
}

Onibus* infoOnibus(char* ordemOnibus){
	//Variavel com a requisicao
	char requisicaoLinha[TAMANHO_MAX_REQUISICAO + TAMANHO_LINHA] = "curl dadosabertos.rio.rj.gov.br/apiTransporte/apresentacao/rest/index.cfm/obterPosicoesDoOnibus/";
	               	               
	/* Executa a requisicao para o servidor da prefeitura */
	strcat(requisicaoLinha,ordemOnibus);

	return realizarRequisicao(requisicaoLinha);
}

void posicaoOnibus(char* ordemOnibus, double* longitude, double* latitude){
	Onibus* veiculo;
	
	//Variavel com a requisicao
	char requisicaoLinha[TAMANHO_MAX_REQUISICAO + TAMANHO_LINHA] = "curl dadosabertos.rio.rj.gov.br/apiTransporte/apresentacao/rest/index.cfm/obterPosicoesDoOnibus/";
	               	               
	/* Executa a requisicao para o servidor da prefeitura */
	strcat(requisicaoLinha,ordemOnibus);

	veiculo = realizarRequisicao(requisicaoLinha);
	
	*longitude = veiculo->longitude;
	
	*latitude = veiculo->longitude;
}

Onibus* infoLinha(char* linhaOnibus){
	//Variavel com a requisicao
	char requisicaoLinha[TAMANHO_MAX_REQUISICAO + TAMANHO_LINHA] = "curl dadosabertos.rio.rj.gov.br/apiTransporte/apresentacao/rest/index.cfm/obterPosicoesDaLinha/";
	
	/*Se tiver uma linha especificada, pede a linha, senao pede todas*/
	if (strcmp("",linhaOnibus) == 0){
		strcpy(requisicaoLinha,"curl dadosabertos.rio.rj.gov.br/apiTransporte/apresentacao/rest/index.cfm/obterTodasPosicoes");
     }
     else{
    	strcat(requisicaoLinha,linhaOnibus);
     }
     return realizarRequisicao(requisicaoLinha);
}

void imprimeListaOnibus(Onibus* lst){
	Onibus* p;
	int contador=0;
	for (p = lst; p != NULL; p = p->prox , contador++){
		printf("\n Onibus %d\n",contador);
		printf("Data %s\n",p->data);
		printf("Ordem %s\n",p->ordem);
		printf("Linha %s\n",p->linha);
		printf("Latitude %f\n",p->latitude);
		printf("Longitude %f\n",p->longitude);
		printf("Velocidade %f\n",p->velocidade);
	}
}

void imprimeListaOnibusApenasOrdem(Onibus* lst){
	Onibus* p;
	int contador=0;
	for (p = lst; p != NULL; p = p->prox , contador++){
		printf("Ordem %s\n",p->ordem);
	}
}
