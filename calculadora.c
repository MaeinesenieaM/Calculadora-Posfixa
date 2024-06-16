#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <calculadora.h>

typedef float Numero;

enum Operador {
	add, 	// +
	sub, 	// -
	mult,	// *
	div  	// /
};

enum TipoExp {
	Numero,
	Operador,
}

typedef struct Item {
	TipoExp valor;
	struct Item *proximo;
} Item;

typede struct {
	Item *topo;
	int tamanho;
} Pilha

Pilha *criarPilha() {
	Pilha *pilha = (Pilha*)malloc(sizeof(Pilha));
	if (pilha == NULL) {
		printf ("!ERROR!\nNAO FOI POSSIVEL ALOCAR MEMORIA PARA A PILHA!");
		exit(101);
	};

	pilha->topo = NULL;
	pilha->tamanho = 0;

	return pilha;
}

Item *criarItem() {
	Item *item = (Item*)malloc(sizeof(Item));
	if (item == NULL) {
		printf ("!ERROR!\nNAO FOI POSSIVEL ALOCAR MEMORIA PARA UM ITEM!");
		exit(102);
	};

	item->valor = 404;
	item->proximo = NULL;

	return item;
}

void empilha(Pilha *pilha, TipoExp valor) {
	Item *item = criarItem();
	item->valor = valor;
	item->proximo = pilha->topo;

	pilha->topo = item;
	pilha->tamanho = pilha->tamanho + 1;
}

TipoExp *desempilhaDado(Pilha *pilha) {	
	if (pilha->topo != NULL) {
		Item *item = pilha->topo;
		TipoExp valor = item->valor;
		pilha->topo = item->proximo;
		pilha->tamanho = pilha->tamanho - 1;

		free(item);
		return valor
	}
	else {
		printf("!ERROR!\nA PILHA ESTA VAZIA!");
		exit(103);
	}
}

Pilha initPilha (char *Str) {
	Pilha *pilha = criarPilha();
	Item  *item = criarItem();

	char *letra = strtok(Str, " ");
	while (letra != NULL) {
		if (pilha->topo == NULL) {

		};
		Pilha *ultimo = pilha->topo; 

		while (ultimo != NULL) {
			ultimo = ultimo->topo;
		};
		ultimo = (Pilha*)malloc(sizeof(Pilha));


	}	
}

char *getFormaInFixa(char *Str);    // Retorna a forma inFixa de Str (posFixa)
float getValor(char *Str) {

};          // Calcula o valor de Str (na forma posFixa)