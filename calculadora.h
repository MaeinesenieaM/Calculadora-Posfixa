#ifndef EXPRESSAO_H
#define EXPRESSAO_H

typedef struct Item {
    float valor;
    struct Item *proximo;
} Item;

typedef struct {
    Item *topo;
    int tamanho;
} Pilha;

typedef enum {
    NONE,   // usado quando nenhuma operação foi feita ainda.
    NUM,    // quando um número for empilhado.
    SEN,    // sen
    COS,    // cos
    TAN,    // tan
    LOG,    // log
    SQRT,   // sqrt ou raiz
    SUB =  7,   // -
    ADD =  8,   // +
    DIV =  10,  // /
    MULT = 11,  // *
    POW = 13    // ^
} Operacao;

typedef struct {
    char posFixa[512];     // Expressão na forma pos fixa, como 3 12 4 + *
    char inFixa[512];      // Expressão na forma pos fixa, como 3 * (12 + 4)
    float Valor;           // Valor numérico da expressão  
} Expressao;

char *getFormaInFixa(char *Str);    // Retorna a forma inFixa de Str (posFixa)
float getValor(char *Str);          // Calcula o valor de Str (na forma posFixa)

#endif