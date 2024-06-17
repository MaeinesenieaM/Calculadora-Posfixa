#include <stdio.h>
#include <stdlib.h>

#include "calculadora.h"

int main () {
	char eita[] = "6 2 / 3 + 4 * 10 sen +";
	char eita2[] = "6 2 / 3 + 4 * 10 sen +";
	float resultado = getValor(eita);
	char *teste = getFormaInFixa(eita2);
	printf("\nEspressao: 6 2 / 3 + 4 * 10 sen +\nResultado: %.2f\n%s", resultado, teste);

	return 0;
}