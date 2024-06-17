#include <stdio.h>
#include <stdlib.h>

#include "calculadora.h"

int main () {
	char eita[] = "2 4 +    2 - 3 3 * + 10 +";
	float resultado = getValor(eita);
	printf("\nResultado: %.2f", resultado);

	return 0;
}