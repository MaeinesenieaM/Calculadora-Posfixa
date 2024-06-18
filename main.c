#include <stdio.h>
#include <stdlib.h>

#include "calculadora.h"

int main () {

	Expressao *expExemplo = criarExpressao();

	guardaPosFixa(expExemplo, "5 2 * 6 + 8 4 - /");

	float valorExemplo01 = getValor(expExemplo->posFixa);

	guardaValor(expExemplo, valorExemplo01);

	//Pode ser feito de forma mais direta com:
	//	guardaValor(expExemplo, getValor(exemplo->posFixa));

	//Pode ser feita com strings normais.
	float valorExemplo02 = getValor("10 2 * 3 +");

	char *inFixaExemplo01 = getFormaInFixa(expExemplo->posFixa);

	guardaInFixa(expExemplo, inFixaExemplo01);

	//Pode ser feito de forma mais direta com:
	//	guardaInFixa(expExemplo, getFormaInFixa(exemplo->posFixa));

	//Também pode ser feita com strings normais.
	char *inFixaExemplo02 = getFormaInFixa("10 2 * 3 +");

	//As formas também aceitam outras operações como cos, sen, tan, log e raiz.
	char *inFixaExemplo03 = getFormaInFixa("50 raiz 5 +");

	printf("Expressao [\nPosFixa: %s\nInFixa: %s\nValor: %g\n]\n", expExemplo->posFixa, expExemplo->inFixa, expExemplo->Valor);
	printf("valorExemplo02: %g\ninFixaExemplo02: %s\ninFixaExemplo03: %s\n", valorExemplo02, inFixaExemplo02, inFixaExemplo03);
	return 0;
}