

typedef struct pessoa {
	char nome[81];
	int idade;
	struct pessoa *proximo;
} Pessoa;

//Insere uma Pessoa em lista encadeada de Pessoa
//Recebe ponteiro para o primeiro elemento da lista, nome e idade da Pessoa a inserir
//Retorna ponteiro para o novo primeiro elemento da lista
Pessoa* insereAmigo(Pessoa* listaAmigos, char* nome, int idade);

//Remove uma Pessoa de lista encadeada de Pessoa
//Recebe ponteiro para o primeiro elemento da lista, nome e idade da Pessoa a remover
//Retorna ponteiro para o novo primeiro elemento da lista
Pessoa* removeAmigo(Pessoa* listaAmigos, char* nome, int idade);

//Libera memoria alocada para lista de pessoas
//Recebe ponteiro para o primeiro elemento da lista
void liberaListaAmigos(Pessoa* listaAmigos);
