#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoasWallaceMarques.h"
#define ARQUIVODEENTRADA "grafo.txt"

//Programa de rede social que le informacao de arquivo e cria grafo com vetor de listas encadeadas
int main(void) {
	
	FILE* fp;
	
	int numeroPessoas;
	int codigoOperacao;
	int caracteresLidos;
	char pulaTemp;
	int pessoaRelacao[2];
	int idadeComparada;
	float media;
	char nomeArquivo[81];
	
	int i;
	int j;
	
	
	//Abre arquivo para leitura e verifica se houve sucesso
	fp = fopen (ARQUIVODEENTRADA,"rt");
	if (fp == NULL){
		printf("Erro na abertura do arquivo!\n");
		printf("Pode ser que o caminho do arquivo esteja esteja incorreto ou que ele não exista ou que tenha havido outro problema.\n\n");
		printf("Encerrando o programa...\n");
		exit(1);
	}
	
	//Obtem do arquivo o numero de pessoas na rede social
	fscanf(fp, "%d\n", &numeroPessoas);
	
	//Aloca vetor de pessoas que estao na rede
	Pessoa* vetorPessoas = (Pessoa*) malloc(numeroPessoas*sizeof(Pessoa));
	
	//Le nomes e idades do arquivo e insere nas Pessoas no vetor de Pessoas
	for (i=0; i<numeroPessoas; i++) {
		caracteresLidos = fscanf(fp, "%s %d\n", vetorPessoas[i].nome, &vetorPessoas[i].idade);
		if (caracteresLidos <= 0) {
			printf("Erro na leitura do arquivo!\n");
			printf("Pode ser que a formatacao esteja incorreta ou que faltem dados ou que tenha havido outro problema.\n\n");
			printf("Encerrando o programa...\n");
			exit(1);
		}
		vetorPessoas[i].proximo = NULL;
	}
	
	//Aloca matriz de relacoes de amizade
	int** relacoesAmizade = (int**) malloc(numeroPessoas*sizeof(int*));
	for (i=0; i<numeroPessoas; i++) {
		relacoesAmizade[i] = (int*) malloc(numeroPessoas*sizeof(int));
	}
	
	//Le do arquivo e armazena na matriz as informacoes de relacoes de amizade
	for (i=0; i<numeroPessoas; i++) {
		for (j=0; j<numeroPessoas; j++) {
			
			//Armazena informacao de relacao de amizade entre pessoa i e pessoa j
			caracteresLidos = fscanf(fp, "%d", &relacoesAmizade[i][j]);
			
			//Verifica padrao de numeros
			if (caracteresLidos <= 0 || (relacoesAmizade[i][j] != 0 && relacoesAmizade[i][j] != 1)) {
				printf("Erro na leitura do arquivo!\n");
				printf("Pode ser que a formatacao ou um valor esteja incorreto ou que faltem dados ou que tenha havido outro problema.\n\n");
				printf("Encerrando o programa...\n");
				exit(1);
				}
			
		}
		
		//Verifica padrao de linhas
		caracteresLidos = fscanf(fp, "%c", &pulaTemp);
		if ((caracteresLidos <= 0 || pulaTemp != '\n' ) && (i < numeroPessoas-1 || j < numeroPessoas)) {
			printf("Erro na leitura do arquivo!\n");
			printf("Pode ser que a formatacao ou um valor esteja incorreto ou que faltem dados ou que tenha havido outro problema.\n\n");
			printf("Encerrando o programa...\n");
			exit(1);
		}
		
	}
	
	//Verifica consistencia dos dados da matriz de relacoes de amizade
	for (i=0; i<numeroPessoas; i++) {
		for (j=0; j<i; j++) {
			
			//Verifica se ha conflito entre duas informacoes de uma mesma relacao de amizade
			if (relacoesAmizade[i][j] != relacoesAmizade[j][i]) {
				printf("Ha inconsistencia nos dados de relacoes de amizades apresentados no arquivo!\n");
				printf("Existem duas informacoes conflitantes sobre uma mesma relacao de amizade.\n");
				printf("Existem informacoes de amizade entre pessoa a e b, porém não de b com a.\n\n");
				printf("O problema ocorre com as pessoas:\n");
				printf("%d - Nome: %s, Idade: %d\n", i+1, vetorPessoas[i].nome, vetorPessoas[i].idade);
				printf("%d - Nome: %s, Idade: %d\n", j+1, vetorPessoas[j].nome, vetorPessoas[j].idade);
				printf("\n");
				printf("Encerrando o programa...\n");
				exit(1);
			}
			
		}
		
		//Verifica se ha relacao de amizade de uma pessoa com ela propria
		if (relacoesAmizade[i][i]) {
			printf("Ha inconsistencia nos dados de relacoes de amizades apresentados no arquivo!\n");
			printf("Foi informada relacao de amizade de uma pessoa com ela mesma.\n\n");
			printf("O problema ocorre com a pessoa:\n");
			printf("%d - Nome: %s, Idade: %d\n", i+1, vetorPessoas[i].nome, vetorPessoas[i].idade);
			printf("\n");
			printf("Encerrando o programa...\n");
			exit(1);
		}
			
	}
	
	//Fecha o arquivo lido
	fclose(fp);
	
	//Monta estrutura em listas encadeadas
	for (i=0; i<numeroPessoas; i++) {
		for (j=0; j<numeroPessoas; j++) {
			if (relacoesAmizade[i][j]) {
				vetorPessoas[i].proximo = insereAmigo(vetorPessoas[i].proximo, vetorPessoas[j].nome, vetorPessoas[j].idade);
			}
		}
	}
	
	
	//Menu do programa
	do {
		
		//Mostra o menu na tela
		printf("Bem-vindo a essa rede social!\n");
		printf("Digite o numero da operacao desejada:\n");
		printf("1 - Adicionar uma relacao de amizade\n");
		printf("2 - Remover uma relacao de amizade\n");
		printf("3 - Buscar pessoas mais velhas que uma idade\n");
		printf("4 - Calcular numero medio de relacoes de amizade\n");
		printf("5 - Salvar arquivo da rede social\n");
		printf("6 - Sair\n");
		printf("\n");
		
		//Recebe o codigo da operacao
		printf("Opcao: ");
		fflush(stdin);
		scanf("%d", &codigoOperacao);
		printf("\n");
		
		
		switch (codigoOperacao) {
			
			//Adiciona uma relacao de amizade
			case 1:
				
				//Mostra opcoes de pessoas para fazer parte da nova amizade
				printf("Abaixo as pessoas que estao na rede social e seus codigos a esquerda:\n");
				for (i=0; i<numeroPessoas; i++) {
					printf("%d - Nome: %s, Idade: %d\n", i+1, vetorPessoas[i].nome, vetorPessoas[i].idade);
				}
				printf("\n");
				
				//Recebe os codigos das duas pessoas a formar nova amizade
				for (i=0; i<2; i++) {
					
					//Recebe um codigo
					printf("Digite o codigo da pessoa #%d a fazer parte da nova amizade: ", i+1);
					fflush(stdin);
					scanf("%d", &pessoaRelacao[i]);
					pessoaRelacao[i] = pessoaRelacao[i] - 1;
					
					//Testa se o codigo digitado eh valido
					if (!(pessoaRelacao[i]>=0 && pessoaRelacao[i]<numeroPessoas)) {
						printf("Codigo ivalido! Essa pessoa nao exite.\n");
						printf("Digite novamente.\n");
						printf("\n");
						i--;
					}
					
				}
				printf("\n");
				
				//Adiciona relacao de amizade caso nao exista e nao seja entre a mesma pessoa
				i = pessoaRelacao[0];
				j = pessoaRelacao[1];
				if (!relacoesAmizade[i][j] && i != j) {
					
					relacoesAmizade[i][j] = 1;
					relacoesAmizade[j][i] = 1;
					vetorPessoas[i].proximo = insereAmigo(vetorPessoas[i].proximo, vetorPessoas[j].nome, vetorPessoas[j].idade);
					vetorPessoas[j].proximo = insereAmigo(vetorPessoas[j].proximo, vetorPessoas[i].nome, vetorPessoas[i].idade);
					printf("Amizade adicionada com sucesso!\n");
					printf("\n");
					
				} else {
					
					if (i == j) {
						printf("Essa amizade nao eh valida pois eh de uma pessoa com ela mesma.\n");
						printf("Escolha uma operacao do menu novamente.\n");
						printf("\n");
					} else {
						printf("Essa amizade ja existe.\n");
						printf("Escolha uma operacao do menu novamente.\n");
						printf("\n");
					}
					
				}
				
			break;
			
			//Remove uma relacao de amizade
			case 2:
				
				//Mostra opcoes de pessoas para remover uma amizade
				printf("Abaixo as pessoas que estao na rede social e seus codigos a esquerda:\n");
				for (i=0; i<numeroPessoas; i++) {
					printf("%d - Nome: %s, Idade: %d\n", i+1, vetorPessoas[i].nome, vetorPessoas[i].idade);
				}
				printf("\n");
				
				//Recebe os codigos das duas pessoas a remover amizade
				for (i=0; i<2; i++) {
					
					//Recebe um codigo
					printf("Digite o codigo da pessoa #%d a remover da amizade: ", i+1);
					fflush(stdin);
					scanf("%d", &pessoaRelacao[i]);
					pessoaRelacao[i] = pessoaRelacao[i] - 1;
					
					//Testa se o codigo digitado eh valido
					if (!(pessoaRelacao[i]>=0 && pessoaRelacao[i]<numeroPessoas)) {
						printf("Codigo ivalido! Essa pessoa nao exite.\n");
						printf("Digite novamente.\n");
						printf("\n");
						i--;
					}
					
				}
				printf("\n");
				
				//Remove relacao de amizade caso exista
				i = pessoaRelacao[0];
				j = pessoaRelacao[1];
				if (relacoesAmizade[i][j]) {
					
					relacoesAmizade[i][j] = 0;
					relacoesAmizade[j][i] = 0;
					vetorPessoas[i].proximo = removeAmigo(vetorPessoas[i].proximo, vetorPessoas[j].nome, vetorPessoas[j].idade);
					vetorPessoas[j].proximo = removeAmigo(vetorPessoas[j].proximo, vetorPessoas[i].nome, vetorPessoas[i].idade);
					printf("Amizade removida com sucesso!\n");
					printf("\n");
					
				} else {
					printf("Essa amizade nao existe.\n");
					printf("Escolha uma operacao do menu novamente.\n");
					printf("\n");
				}
				
			break;
			
			//Busca e mostra pessoas da rede social mais velhas que uma idade escolhida
			case 3:
				
				//Recebe a idade a comparar
				printf("Digite a idade em anos a comparar: ");
				fflush(stdin);
				scanf("%d", &idadeComparada);
				printf("\n");
				
				//Testa se a idade digitada eh valida
				if (idadeComparada < 0) {
					printf("Idade invalida! Numero menor que 0.\n");
					printf("Escolha uma operacao do menu novamente.\n");
					printf("\n");
					break;
				}
				
				//Verifica se existem pessoas com essa idade e retorna ao menu caso contrario
				for (i=0,j=0; i<numeroPessoas; i++) {
					if (vetorPessoas[i].idade > idadeComparada) {
						j++;
						break;
					}
				}
				if (j == 0) {
					printf("Nao existem pessoas com mais de %d anos na rede social.\n", idadeComparada);
					printf("\n");
					break;
				}
				
				//Mostra pessoas com idade maior
				printf("Abaixo as pessoas de idade maior que %d anos:\n", idadeComparada);
				for (i=0; i<numeroPessoas; i++) {
					if (vetorPessoas[i].idade > idadeComparada) {
						printf("Nome: %s, Idade: %d\n", vetorPessoas[i].nome, vetorPessoas[i].idade);
					}
				}
				printf("\n");
				
			break;
			
			//Calcula e mostra o numero medio de relacoes por pessoa.
			case 4:
				
				//Calcula a media
				media = 0;
				for (i=0; i<numeroPessoas; i++) {
					for (j=0; j<numeroPessoas; j++) {
						media = media + relacoesAmizade[i][j];
					}
				}
				media = media/numeroPessoas/2;
				
				//Mostra o resultado
				printf("O numero medio se relacoes por pessoa eh: %.2f\n", media);
				printf("\n");
				
			break;
			
			//Salva arquivo da rede social
			case 5:
				
				//Obtem o nome do arquivo a criar
				printf("Digite o nome do arquivo para salvar: ");
				fflush(stdin);
				scanf(" %80[^\n]", &nomeArquivo);
				printf("\n");
				
				//Abre o arquivo para escrita
				fp = fopen (nomeArquivo,"wt");
				if (fp == NULL){
					printf("Erro na abertura do arquivo!\n");
					printf("Pode ser que o caminho nao exista ou que o nome do arquivo seja invalido ou que tenha havido outro problema.\n");
					printf("\n");
					break;
				}
				
				//Escreve o numero de pessoas na rede
				fprintf(fp, "%d\n", numeroPessoas);
				
				//Escreve nome e idade das pessoas que estão na rede
				for (i=0; i<numeroPessoas; i++) {
					fprintf(fp, "%s %d\n", vetorPessoas[i].nome, vetorPessoas[i].idade);
				}
				printf("\n");
				
				//Escreve a matriz de relacoes de amizade
				for (i=0; i<numeroPessoas; i++) {
					for (j=0; j<numeroPessoas; j++) {
						
						if (j < numeroPessoas-1) {
							
							fprintf(fp, "%d ", relacoesAmizade[i][j]);
							
						} else {
							
							if (i < numeroPessoas-1) {
								fprintf(fp, "%d\n", relacoesAmizade[i][j]);
							} else {
								fprintf(fp, "%d", relacoesAmizade[i][j]);
							}
							
						}
						
					}
				}
				
				//Fecha o salva conteudo e fecha arquivo
				int verificaArquivo;
				verificaArquivo = fclose(fp);
				if (!verificaArquivo) {
					printf("O arquivo foi salvo com sucesso!\n");
					printf("\n");
				} else {
					printf("Ocorreu um erro ao salvar o arquivo!\n");
					printf("\n");
				}
				
			break;
			
			//Encerra o programa
			case 6:
				
				//Libera a memoria ocupada pelas listas encadeadas
				for (i=0; i<numeroPessoas; i++) {
					liberaListaAmigos(vetorPessoas[i].proximo);
				}
				
				//Libera memoria ocupada pelo vetor de pessoas
				free(vetorPessoas);
				
				//Libera memoria ocupada pela matriz de relacoes de amizade
				for (i=0; i<numeroPessoas; i++) {
					free(relacoesAmizade[i]);
				}
				free(relacoesAmizade);
				
				//Encerra
				printf("Encerrando o programa...\n");
				
			break;
			
			case 7:
				
				//Easter egg
				printf("Voce desbloqueou a funcao secreta!\n");
				printf(":O\n");
				for (i=0; i<numeroPessoas; i++) {
					printf("%d - Nome: %s, Idade: %d\n", i+1, vetorPessoas[i].nome, vetorPessoas[i].idade);
					Pessoa* atual = vetorPessoas[i].proximo;
					if (atual == NULL) {
						printf("\n");
					}
					while (atual != NULL) {
						if ((* atual).proximo != NULL) {
							printf("[Nome: %s, Idade: %d] -> ", (* atual).nome, (* atual).idade);
						} else{
							printf("[Nome: %s, Idade: %d]\n", (* atual).nome, (* atual).idade);
						}
						atual = (* atual).proximo;
					}
					printf("\n");
				}
				printf("\n");
				
			break;
			
			//Opcao invalida
			default:
				printf("Opcao invalida! Escolha novamente.\n");
				printf("\n");
			break;
			
		}
		
	} while(codigoOperacao != 6);
	
	return 0;
}
