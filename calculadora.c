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
		case 104:
			printf("!ERROR! [103]\nNAO FOI POSSIVEL ALOCAR MEMORIA PARA A EXPRESSAO");
			exit(103);
		case 201:
			printf("!ERROR! [201]\nFORMATO DE EXPRESSAO INCORRETA!");
			exit(201);
		case 202:
			printf("!ERROR! [202]\nNAO E ACEITO LETRAS NA EXPRESSAO!");
			exit(202);
		default:
			printf("!ERROR! [???]\nERRO DESCONHECIDO!");
			exit(1);
	}
}

Expressao *criarExpressao() {
	Expressao *expressao = (Expressao*)malloc(sizeof(Expressao));
	if (expressao == NULL) mostraErro(104);
	return expressao;
}

void guardaPosFixa(Expressao *expressao, char *Str) {
	strcpy(expressao->posFixa, Str);
}
void guardaInFixa(Expressao *expressao, char *Str) {
	strcpy(expressao->inFixa, Str);
}
void guardaValor(Expressao *expressao, float num) {
	expressao->Valor = num;
}


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

//Verifica se a string for algum das operações especiais.
void verificaTrigonometria (char *Str) {
	char formatos[][5] = {"sen", "cos", "tan", "log", "raiz"};
	for (int i = 0; i < 5; i++) {
		if (strcmp(Str, formatos[i]) == 0) return;
	}
	mostraErro(202);
}

//Verifica se a string esta no formato correto.
void verificaLetra (char *Str) {
	int tamanho = strlen(Str);
	for (int i = 0; i < tamanho; i++) {
		if (isalpha(Str[i])) verificaTrigonometria(Str);
	}
}

//Aplica as operações na pilha.
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
		case 's': //sen
			val1 = desempilhaDado(pilha);
			val1 = sin(val1 * (M_PI / 180));
			empilhaDado(pilha, val1);
			break;
		case 'c': //cos
			val1 = desempilhaDado(pilha);
			val1 = cos(val1 * (M_PI / 180));
			empilhaDado(pilha, val1);
			break;
		case 't': //tan
			val1 = desempilhaDado(pilha);
			val1 = tan(val1 * (M_PI / 180));
			empilhaDado(pilha, val1);
			break;
		case 'l': //log
			val1 = desempilhaDado(pilha);
			val1 = log10(val1);
			empilhaDado(pilha, val1);
			break;
		case 'r': //sqrt ou raiz
			val1 = desempilhaDado(pilha);
			val1 = sqrt(val1);
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
		case 's':
			return SEN;
		case 'c':
			return COS;
		case 't':
			return TAN;
		case 'l':
			return LOG;
		case 'r':
			return SQRT;
		default:
			float num = atoi(Str);
			empilhaDado(pilha, num);
			return NUM;
	}
}

// Calcula o valor de Str (na forma posFixa)
float getValor(char *Str) {
	Pilha *pilha = criarPilha();

	//Isso Garante que a String anterior possa ser utilizada multiplas vezes.
	char posFixa[512];
	strcpy(posFixa, Str);

	char *letra = strtok(posFixa, " ,;");
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

//Cobre a string dada em parenteses.
void pareteses(char *Str) {
	char novaStr[256] = "(";
	strcat(novaStr, Str);
	strcat(novaStr, ") ");
	strcpy (Str, novaStr);
}

// Retorna a forma inFixa de Str (posFixa)
char *getFormaInFixa(char *Str) {
	Pilha *pilha = criarPilha();

	//Isso Garante que a String anterior possa ser utilizada multiplas vezes.
	char posFixa[512];
	strcpy(posFixa, Str);

	char *inFixa = (char*)malloc(sizeof(char) * 256);
	sprintf(inFixa, "");
	char topNum[64];

	Operacao ultimoOpera = NONE;		//Este valor nunca vai ser [NUM].
	Operacao opera = NONE;

	char *letra = strtok(posFixa, " ,;");
	while (letra != NULL) {
		verificaLetra(letra);

		if (pilha->topo != NULL) sprintf(topNum, "%g", topo(pilha));

		opera = operacaoVerifica(pilha, letra);
		if (opera != NUM) {
			//estes dois IF são responsaveis por escrever a formula InFixada.
			if (ultimoOpera != NONE && ultimoOpera != opera && ultimoOpera - opera < -2  && (opera > 6 && ultimoOpera > 6)) pareteses(inFixa);
			if (ultimoOpera == NONE && opera > 6) sprintf(inFixa,"%g ", desempilhaDado(pilha));

			char temp[64];
			//Idenfica qual foi a opeação feita e aplica as instruçoes dadas.
			switch (opera) {
				case POW:
					sprintf(temp, "^ %s ", topNum);
					strcat(inFixa, temp);
					break;
				case MULT:
					sprintf(temp, "* %s ", topNum);
					strcat(inFixa, temp);
					break;
				case DIV:
					sprintf(temp, "/ %s ", topNum);
					strcat(inFixa, temp);
					break;
				case ADD:
					sprintf(temp, "+ %s ", topNum);
					strcat(inFixa, temp);
					break;
				case SUB:
					sprintf(temp, "- %s ", topNum);
					strcat(inFixa, temp);
					break;
				case SEN:
					if (pilha->topo == NULL) {
						pareteses(inFixa);
						sprintf(temp, "sen%s ", inFixa);
						strcpy(inFixa, temp);
					}
					else {
						sprintf(temp, "sen(%g) ", desempilhaDado(pilha));
						strcat(inFixa, temp);
					}
					break;
				case COS:
					if (pilha->topo == NULL) {
						pareteses(inFixa);
						sprintf(temp, "cos%s ", inFixa);
						strcpy(inFixa, temp);
					}
					else {
						sprintf(temp, "cos(%g) ", desempilhaDado(pilha));
						strcat(inFixa, temp);
					}
					break;
				case TAN:
					if (pilha->topo == NULL) {
						pareteses(inFixa);
						sprintf(temp, "tan%s ", inFixa);
						strcpy(inFixa, temp);
					}
					else {
						sprintf(temp, "tan(%g) ", desempilhaDado(pilha));
						strcat(inFixa, temp);
					}
					break;
				case LOG:
					if (pilha->topo == NULL) {
						pareteses(inFixa);
						sprintf(temp, "log%s ", inFixa);
						strcpy(inFixa, temp);
					}
					else {
						sprintf(temp, "log(%g) ", desempilhaDado(pilha));
						strcat(inFixa, temp);
					}
					break;
				case SQRT:
					if (pilha->topo == NULL) {
						pareteses(inFixa);
						sprintf(temp, "raiz%s ", inFixa);
						strcpy(inFixa, temp);
					}
					else {
						sprintf(temp, "raiz(%g) ", desempilhaDado(pilha));
						strcat(inFixa, temp);
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
}