#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoasWallaceMarques.h"

Pessoa* insereAmigo(Pessoa* listaAmigos, char* nome, int idade) {
	Pessoa* amigo = (Pessoa*) malloc(sizeof(Pessoa));
	strcpy((* amigo).nome, nome);
	(* amigo).idade = idade;
	(* amigo).proximo = listaAmigos;
	
	return amigo;
}

Pessoa* removeAmigo(Pessoa* listaAmigos, char* nome, int idade) {
	Pessoa* anterior = NULL;
	Pessoa* atual = listaAmigos;
	
	while (atual != NULL && !(strcmp((* atual).nome, nome) == 0 && (* atual).idade == idade)) {
		anterior = atual;
		atual = (* atual).proximo;
	}
	if (atual == NULL) {
		return listaAmigos;
	}
	
	if (anterior == NULL) {
		listaAmigos = (* atual).proximo;
	} else {
		(* anterior).proximo = (* atual).proximo;
	}
	
	free (atual);
	
	return listaAmigos;
}

void liberaListaAmigos(Pessoa* listaAmigos) {
	
	Pessoa* atual = listaAmigos;
	
	while (atual != NULL) {
		Pessoa* proximo = (* atual).proximo;
		free(atual);
		atual = proximo;
	}
}
