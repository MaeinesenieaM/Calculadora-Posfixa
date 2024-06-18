#include <stdio.h>
#include <stdlib.h>

#include "calculadora.h"

int main () {
	char eita[] = "6 2 / 3 + 4 * 10 log +";
	char eita2[] = "6 2 / 3 + 4 * 10 log +";
	float resultado = getValor(eita);
	char *teste = getFormaInFixa(eita2);
	printf("\nEspressao: 6 2 / 3 + 4 * 10 log +\nResultado: %.2f\n%s", resultado, teste);

	return 0;
}