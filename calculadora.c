#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <math.h>
#include <ctype.h>

#include "calculadora.h"

typedef struct Item {
	float valor;
	struct Item *proximo;
} Item;

typedef struct {
	Item *topo;
	int tamanho;
} Pilha;

Pilha *criarPilha() {
	Pilha *pilha = (Pilha*)malloc(sizeof(Pilha));
	if (pilha == NULL) {
		printf("!ERROR!\nNAO FOI POSSIVEL ALOCAR MEMORIA PARA A PILHA!");
		exit(101);
	};

	pilha->topo = NULL;
	pilha->tamanho = 0;
	return pilha;
}

float topo(Pilha *pilha) {
	if (pilha->topo == NULL) {
		printf("!ERROR!\nA PILHA ESTA VAZIA!");
		exit(103);
	}
	return pilha->topo->valor;
}

Item *criarItem() {
	Item *item = (Item*)malloc(sizeof(Item));
	if (item == NULL) {
		printf("!ERROR!\nNAO FOI POSSIVEL ALOCAR MEMORIA PARA UM ITEM!");
		exit(102);
	};

	item->valor = 404.0;
	item->proximo = NULL;

	return item;
}

void empilhaDado(Pilha *pilha, float valor) {
	Item *item = criarItem();
	item->valor = valor;
	item->proximo = pilha->topo;

	pilha->topo = item;
	pilha->tamanho = pilha->tamanho + 1;
}

float desempilhaDado(Pilha *pilha) {	
	if (pilha->topo != NULL) {
		Item *item = pilha->topo;
		float valor = item->valor;

		pilha->topo = item->proximo;
		pilha->tamanho = pilha->tamanho - 1;

		free(item);
		item = NULL;
		return valor;
	}
	else {
		printf("!ERROR!\nA PILHA ESTA VAZIA!");
		exit(103);
	}
}

void verificaLetra (char *Str) {
	int tamanho = strlen(Str);
	for (int i = 0; i < tamanho; i++) {
		if (isalpha(Str[i])) {
			printf("!ERROR!\nNAO E ACEITO LETRAS NA ESPRESSAO!");
			exit(202);
		}
	}
}

void operacao(Pilha *pilha, char *Str) {
	float val1, val2;

	switch (Str[0]) {
		case '(': case ')':
			printf("!ERROR!\nFORMATO DE EXPRESSAO INCORRETA!");
			exit(201);
		case '+':
			val1 = desempilhaDado(pilha);
			val2 = desempilhaDado(pilha);
			empilhaDado(pilha, val2 + val1);
			break;
		case '-':
			val1 = desempilhaDado(pilha);
			val2 = desempilhaDado(pilha);
			empilhaDado(pilha, val2 - val1);
			break;
		case '*':
			val1 = desempilhaDado(pilha);
			val2 = desempilhaDado(pilha);
			empilhaDado(pilha, val2 * val1);
			break;
		case '/':
			val1 = desempilhaDado(pilha);
			val2 = desempilhaDado(pilha);
			if (val1 == 0 && val2 == 0) empilhaDado(pilha, 0);
			else empilhaDado(pilha, val2 / val1);
			break;
		case '^':
			val1 = desempilhaDado(pilha);
			val2 = desempilhaDado(pilha);
			pow(val2, val1);
			empilhaDado(pilha, val2);
			break;
		default:
			float num = atoi(Str);
			empilhaDado(pilha, num);
			break;
	}
}
/*
Pilha *initPilha (char *Str) {
	Pilha *pilha = criarPilha();
	char *letra = strtok(Str, " ");
	while (letra != NULL) {
		TipoExp dado = classificaDado(letra);
		empilhaDado(pilha, dado);
	}
	return pilha;
}
*/
// Calcula o valor de Str (na forma posFixa)
float getValor(char *Str) {
	Pilha *pilha = criarPilha();
	char *letra = strtok(Str, " ");
	while (letra != NULL) {
		printf("%s ", letra);
		verificaLetra(letra);
		operacao(pilha, letra);
		letra = strtok(NULL, " ");
	}

	float valor = desempilhaDado(pilha);
	free (pilha);
	pilha = NULL;
	return (valor);
};

char *getFormaInFixa(char *Str) {

};    // Retorna a forma inFixa de Str (posFixa)