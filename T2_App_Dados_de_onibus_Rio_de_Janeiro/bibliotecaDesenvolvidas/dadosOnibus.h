#define TAMANHO_LINHA 10
#define TAMANHO_MAX_REQUISICAO 100
#define TAMANHO_BUFFER 512

//Estrutura que representa um onibus
typedef struct onibus{
	char data[24]; //Data na qual a informacao foi coleta
	char ordem[8]; //Ordem (identificador) do onibus
	char linha[8]; //Numero da linha
	double latitude; //Latitude do onibus na coleta
	double longitude; //Longitude do onibus na coleta
	float velocidade; //Velocidade do onibus em Km/h
	struct onibus* prox; //Ponteiro para o proximo elemento
	struct onibus* ant; //Ponteiro para o elemento anterior
}Onibus;		

//Funcao que retorna uma lista encadeada com as linhas
//Recebe o numero da linha, ou uma string vazia "", para retornar todas as linhas
//Se houver algum erro, retorna NULL
Onibus* infoLinha(char* linhaOnibus);

//Retorna um ponteiro para informacoes de um onibus especifico
//Recebe o numero do onibus e retorna o ponto para um elemento do tipo Onibus
//Se houver algum erro, retorna NULL
Onibus* infoOnibus(char* ordemOnibus);

//Imprime todos os onibus na lista lst
void imprimeListaOnibus(Onibus* lst);

//Mesma que a anterior, mas imprime so a ordem dos onibus da lista
void imprimeListaOnibusApenasOrdem(Onibus* lst);

//Escrevea em longitude e latitude a posicao dos onbibus
void posicaoOnibus(char* ordemOnibus, double* longitude, double* latitude);
