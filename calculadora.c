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

enum Operacao {
	add,	// +
	sub,	// -
	mult,	// *
	div,	// /
	pow,	// ^
	log, 	// log
	cos,	// cos
	sen,	// sen
	NONE	// usado qando nenhuma operação foi feita ainda.
}

void mostraErro(int erro) {
	//tipos de erro:
	// 1xx -> culpa do sistema.
	// 2xx -> culpa do usuario.
	switch (erro) {
		case 101:
			printf("!ERROR! [101]\nNAO FOI POSSIVEL ALOCAR MEMORIA PARA A PILHA!");
			exit(101);
		case 102:
			printf("!ERROR! [102]\nNAO FOI POSSIVEL ALOCAR MEMORIA PARA UM ITEM!");
			exit(102);
		case 103:
			printf("!ERROR! [103]\nA PILHA ESTA VAZIA!");
			exit(103);
		case 201:
			printf("!ERROR! [201]\nFORMATO DE EXPRESSAO INCORRETA!");
			exit(201);
		case 202:
			printf("!ERROR! [202]\nNAO E ACEITO LETRAS NA ESPRESSAO!");
			exit(202);
		default:
			printf("!ERROR! [???]\nERRO DESCONHECIDO!");
			exit(1);
	}
}

Pilha *criarPilha() {
	Pilha *pilha = (Pilha*)malloc(sizeof(Pilha));
	if (pilha == NULL) mostraErro(101);

	pilha->topo = NULL;
	pilha->tamanho = 0;
	return pilha;
}

float topo(Pilha *pilha) {
	if (pilha->topo == NULL) mostraErro(103);
	return pilha->topo->valor;
}

Item *criarItem() {
	Item *item = (Item*)malloc(sizeof(Item));
	if (item == NULL) mostraErro(102);

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
	else mostraErro(103);
}

void verificaFormato (char *Str) {
	char formatos[][4] = {"log", "cos", "sen"};
	for (int i = 0; i < 3; i++) {
		if (strcmp(Str, formatos[i]) == 0) return;
	}
	mostraErro(202);
}

void verificaLetra (char *Str) {
	int tamanho = strlen(Str);
	for (int i = 0; i < tamanho; i++) {
		if (isalpha(Str[i])) verificaFormato(Str);
	}
}

void operacao(Pilha *pilha, char *Str) {
	float val1, val2;

	switch (Str[0]) {
		case '(': case ')':
			mostraErro(201);
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
		case 'l':
			val1 = desempilhaDado(pilha);
			log(val1);
			empilhaDado(pilha, val1);
			break;
		case 'c':
			val1 = desempilhaDado(pilha);
			cos(val1);
			empilhaDado(pilha, val1);
			break;
		case 's':
			val1 = desempilhaDado(pilha);
			sin(val1);
			empilhaDado(pilha, val1);
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
	char *letra = strtok(Str, " ,;");
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