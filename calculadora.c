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

typedef enum {
	NONE,	// usado quando nenhuma operação foi feita ainda.
	NUM,	// quando um número for empilhado.
	SEN,	// sen
	COS,	// cos
	LOG, 	// log
	SUB =  5,	// -
	ADD =  6,	// +
	DIV =  8,	// /
	MULT = 9,	// *
	POW = 11	// ^
} Operacao;

void mostraErro(int erro);

Pilha *criarPilha() {
	Pilha *pilha = (Pilha*)malloc(sizeof(Pilha));
	if (pilha == NULL) mostraErro(101);

	pilha->topo = NULL;
	pilha->tamanho = 0;
	return pilha;
}

Item *criarItem() {
	Item *item = (Item*)malloc(sizeof(Item));
	if (item == NULL) mostraErro(102);

	item->valor = 404.0;
	item->proximo = NULL;

	return item;
}

int estaVazio(Pilha *pilha) {
	if (pilha->topo == NULL) return 1;
	else return 0;
}

float topo(Pilha *pilha) {
	if (pilha->topo == NULL) mostraErro(103);
	return pilha->topo->valor;
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

void operacaoPilha(Pilha *pilha, char *Str) {
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
			val1 = log(val1 * (M_PI / 180));
			empilhaDado(pilha, val1);
			break;
		case 'c':
			val1 = desempilhaDado(pilha);
			val1 = cos(val1 * (M_PI / 180));
			empilhaDado(pilha, val1);
			break;
		case 's':
			val1 = desempilhaDado(pilha);
			val1 = sin(val1 * (M_PI / 180));
			empilhaDado(pilha, val1);
			break;
		default:
			float num = atoi(Str);
			empilhaDado(pilha, num);
			break;
	}
}

//Diferente de operacaoPilha, esta função verifica as operações mas não aplica elas. (exceto empilhar)
//Ao detectar elas retorna a operação e descarta o topo.
Operacao operacaoVerifica(Pilha *pilha, char *Str) {

	switch (Str[0]) {
		case '(': case ')':
			mostraErro(201);
		case '+':
			desempilhaDado(pilha);
			return ADD;
		case '-':
			desempilhaDado(pilha);
			return SUB;
		case '*':
			desempilhaDado(pilha);
			return MULT;
		case '/':
			desempilhaDado(pilha);
			return DIV;
		case '^':
			desempilhaDado(pilha);
			return POW;
		case 'l':
			return LOG;
		case 'c':
			return COS;
		case 's':
			return SEN;
		default:
			float num = atoi(Str);
			empilhaDado(pilha, num);
			return NUM;
	}
}
// Calcula o valor de Str (na forma posFixa)
float getValor(char *Str) {
	Pilha *pilha = criarPilha();
	char *letra = strtok(Str, " ,;");
	while (letra != NULL) {
		verificaLetra(letra);
		operacaoPilha(pilha, letra);
		letra = strtok(NULL, " ,;");
	}

	float valor = desempilhaDado(pilha);
	free (pilha);
	pilha = NULL;
	return (valor);
}

void pareteses(char *Str) {
	char novaStr[256] = "(";
	strcat(novaStr, Str);
	strcat(novaStr, ")");
	strcpy (Str, novaStr);
}

char *getFormaInFixa(char *Str) {
	Pilha *pilha = criarPilha();

	char *inFixa = (char*)malloc(sizeof(char) * 256);
	sprintf(inFixa, "");
	char topNum[64];
	Operacao ultimoOpera = NONE;		//Este valor nunca vai ser [NUM].
	Operacao opera = NONE;

	char *letra = strtok(Str, " ,;");
	while (letra != NULL) {
		if (pilha->topo != NULL) sprintf(topNum, "%.0f", topo(pilha));

		verificaLetra(letra);

		if (estaVazio(pilha) == 1 && strchr(topNum, '(') != NULL) {
			char temp[64];
			sprintf (temp, " %s %s", letra, topNum);
			strcat(inFixa, temp);
			letra = strtok(NULL, " ,;");
			continue;
		}
		opera = operacaoVerifica(pilha, letra);
		if (opera != NUM) {
			if (ultimoOpera != NONE && (opera - ultimoOpera > 1 && opera > 4)) pareteses(inFixa);
			if (ultimoOpera == NONE) sprintf(inFixa,"%.0f", desempilhaDado(pilha));

			char temp[64];
			switch (opera) {
				case POW:
					sprintf(temp, " ^ %s", topNum);
					strcat(inFixa, temp);
					break;
				case MULT:
					sprintf(temp, " * %s", topNum);
					strcat(inFixa, temp);
					break;
				case DIV:
					sprintf(temp, " / %s", topNum);
					strcat(inFixa, temp);
					break;
				case ADD:
					sprintf(temp, " + %s", topNum);
					strcat(inFixa, temp);
					break;
				case SUB:
					sprintf(temp, " - %s", topNum);
					strcat(inFixa, temp);
					break;
				case LOG:
					if (estaVazio(pilha) != 0) {
						pareteses(inFixa);
						sprintf(temp, "log%s", inFixa);
						strcpy(inFixa, temp);
					}
					else {
						sprintf(temp, "log(%.0f)", desempilhaDado(pilha));
						strcpy(topNum, temp);
					}
					break;
				case SEN:
					if (estaVazio(pilha) != 0) {
						pareteses(inFixa);
						sprintf(temp, "sen%s", inFixa);
						strcpy(inFixa, temp);
					}
					else {
						sprintf(temp, "sen(%.0f)", desempilhaDado(pilha));
						strcpy(topNum, temp);
					}
					break;
				case COS:
					if (estaVazio(pilha) != 0) {
						pareteses(inFixa);
						sprintf(temp, "cos%s", inFixa);
						strcpy(inFixa, temp);
					}
					else {
						sprintf(temp, "cos(%.0f)", desempilhaDado(pilha));
						strcpy(topNum, temp);
					}
					break;
			}

			ultimoOpera = opera;
		}
		letra = strtok(NULL, " ,;");
	}
	free (pilha);
	pilha = NULL;
	return (inFixa);

}    // Retorna a forma inFixa de Str (posFixa)

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