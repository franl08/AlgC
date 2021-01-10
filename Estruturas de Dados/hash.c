/* Tabelas de Hash
    -> Usadas ara reduzir o universo de chave apenas para o necessário;
    -> Geram colisões entre chaves para o mesmo valor;
    -> As probabilidades dos valores do contradomínio da função têm de ser iguais (função uniforme).
*/

/* A função principal de Hash é mapear conjuntos muito grandes em conjuntos mais pequenos de forma uniforme.
                             Chaves  --> Posições do Array
   Existem várias estratégias para a resolução de colisões:
        1. Open Adressing (dentro do array) -> Array com posições {0, ..., cap - 1} de pares (k, v);
        2. Closed Adressing (fora do array) -> Array com posições {0, ..., cap - 1} de pointers para listas ligadas de pares.
   Tendo cada uma as suas vantagens.
*/

/* 1. Open Adressing
    -> Usa-se o linear probing;
    -> Na remoção, quando retiramos um elemento, colocamos no seu lugar "removed"
        > Formação de Clusters (para evitar isto usamos o quadratic probing -> saltos cada vez mais longe)
    -> Vantagens:
        > Boa localidade;
        > Menos espaço ocupado.
*/

/* 2. Closed Adressing
    -> O comprimento médio das listas ligadas é dado por:
        alfa = (chaves_inseridas / cap);
    -> Quando alfa = alfa_max, redimensiona-se a tabela sendo, para isso, necessária a utilização de uma nova função de hash.
    -> Vantagens:
        > Complexidade, em geral, melhor (no limite, se cap = #k, a complexidade é dada por Theta(1), mas desperdiça-se espaço).
*/

// Definições auxiliares para algoritmos de Hashs em C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPTY "-"
#define DELETED "+"

struct pair {
    char key[20];
    int value;
};

typedef struct {
    int size;
    int used;
    struct pair *tbl;  
} HT;

// Exemplo de uma função de hash
int hash (char key[], int size){
    int i = 0, sum = 0;
    while (key[i] != '\0') sum += key[i++];
    return (sum % size);
}

// Função para iniciar uma tabela de hash
void initHT (HT *h, int size){
    int i;
    h -> tbl = calloc (size, sizeof(struct pair));
    h -> size = size;
    h -> used = 0;
    for (i = 0; i < h -> size; i++) strcpy(h -> tbl[i].key, EMPTY);
}

// Função para libertar um elemento de uma tabela de hash
int freeHT (HT *h, int k){
    if (strcmp((h -> tbl)[k].key, EMPTY) == 0) return 1;
    if (strcmp((h -> tbl)[k].key, DELETED) == 0) return 1;
    return 0;
}

// Função para inserir elementos a uma tabela de hash
int writeHT (HT *h, char key[], int value){
    float load = h -> used / h -> size;
    int i = hash(key, h -> size);
    while (!freeHT(h, i)){
        i = (i + 1) % (h -> size);
    }
    strcpy((h -> tbl)[i].key, key);
    (h -> tbl)[i].value = value;
    h -> used++;
    return i;
}

// Função para ler elementos de uma tabela de hash
int readHT (HT *h, char key[], int * value){
    int i, ii;
    i = ii = hash(key, h -> size);
    while(strcmp((h -> tbl)[i].key, key) != 0){
        if (strcmp ((h -> tbl)[i].key, EMPTY) == 0) return -1;
        i = (i + 1) % (h -> size);
        if (i == ii) return -1;
    }
    *value = (h -> tbl)[i].value;
    return i;
}

// Função que elimina uma tabela de hash
int deleteHT (HT *h, char key[]){
    int r, v;
    r = readHT (h, key, &v);
    if (r >= 0){
        strcpy ((h -> tbl)[r].key, DELETED);
        h -> used--;
    }
    return r;
}