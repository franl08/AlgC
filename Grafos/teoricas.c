#include <stdlib.h>
#include <stdio.h>
// Aulas Teóricas de Grafos

/* Aplicações de Grafos:
 - Mapas:
   > Vértices são os cruzamentos;
   > Arestas são ruas a ligar os cruzamentos.
*/

/* Vértices são representados por {0,1,...,N-1}
   e estão relacionados por arestas, p.e. x está relacionado com y == existe um aresta x ---> y
*/

/* Os grafos podem ser:
     1. Não Orientados (a aresta x->y é igual à aresta y->x);
     2. Pesados (associado a cada aresta existe um peso).
*/

// Representação de grafos:

#define V 100 // V é o número de vértices

// 1. Grafo em forma de matrizes de adjacência:

typedef int GrafoM[V][V]; /* note-se que esta representação é demasiado pesada, pois, tipicamente, o número de arestas é muito inferior a (V^2), 
                           logo, estamos a desperdiçar recursos. 
    Seja GrafoM g, em que:
      1. g[2][3] = 3
         > Concluímos que existe um aresta com Origem 2, Destino 5 e Peso 3
      2. g[3][6] = 0
         > Concluímos que não existe nenhuma aresta de 3 para 6
    */

// 2. Grafo em forma de lista de adjacência:

typedef struct aresta
{
    int destino, peso;
    struct aresta *prox; 
} *ListaAdj;

typedef ListaAdj GrafoL[V]; /* geralmente, vamos utilizar esta representação, pois no caso médio, apresenta melhor complexidade que os grafos representados em
                              matrizes de adjacência
                           */


/* Para o cálculo da complexidade das diversas funções de grafos que iremos reproduzir, devemos considerar o tamanho do input (V,E) em que V representa o número
de vértices do grafo e E o número de arestas.
*/


// Função que conta quantas Arestas tem um grafo orientado

int contaArestasM (GrafoM g)
{
    int o, d, r = 0;
    for (o = 0; o < V; o++)
      for (d = 0; d < V; d++)
        if (g[o][d] != 0) r++;
    return r;
}
// A Complexidade desta função é dada por: T(V,E) = Theta(V^2)

int contaArestasL (GrafoL g)
{
    int o, r = 0;
    ListaAdj x;
    for (o = 0; o < V; o++)
       for (x = g[o]; x != NULL; x = x->prox) r++;
    return r;
}
// A Complexidade desta função é dada por: T(V,E) = Theta(V + E)

/* O grau de saída de um grafo é dado pelo maior número de arestas com origem em comum
   O grau de saída de um vértice é dado pelo número de arestas que têm esse vértice como origem
*/


// Função que calcula o grau de saída de um grafo

int outDegreeL (GrafoL g)
{
    int o, r = 0, t;
    ListaAdj x;
    for (o = 0; o < V; o++){
        t = 0;
        for (x = g[o]; x != NULL; x = x->prox) t++;
        if (t > r) r = t;
    }
    return r;
}
// A Complexidade desta função é dada por: T(V,E) = Theta(v + E)

int outDegreeM (GrafoM g)
{
    int o, d, r = 0, t;
    for (o = 0; o < V; o++){
        t = 0;
        for (d = 0; d < V; d++)
          if (g[o][d] != 0) t++;
        if (t > r) r = t;
    }
    return r;
}
// A Complexidade desta função é dada por: T(V,E) = Theta(V^2)


// Função para a procura de arestas adjacentes

int haArestaM (GrafoM g, int o, int d)
{
    return (g[o][d] != 0);
}
// A Complexidade desta função é dada por: T(V,E) = Theta(1)

int haArestaL (GrafoL g, int o, int d)
{
    ListaAdj x;
    for (x = g[o]; x != NULL && x->destino != d; x = x->prox);
    return (x != NULL);
}
/* A Complexidade desta função é dada por: 
  > No melhor caso (quando o 1º elemento avaliado é o vértice destino procurado) = Omega(1)
  > No pior caso (tem de percorrer toda a lista, ou seja, o vértice destino procurado é o último elemento) = O(V)
*/


// Função que procura um caminho (se d é alcançável a partir de o)

int haCaminhoAux (GrafoL g, int o, int d, int vis[])
{
    ListaAdj x;
    vis[o] = 1; // não iniciamos mais procuras a partir da origem
    if (o == d) return 1;
    for (x = g[o]; x != NULL; x = x->prox){
        // Existe uma aresta desde o até x->destino com peso x->peso
        if (vis[x->destino] == 0 // ainda não procuramos a partir deste ponto
            && 
            haCaminhoAux(g, x->destino, d, vis)) return 1;
    }
    return 0;
}
// A Complexidade desta função é dada por T(V,E) = V + E

int haCaminho (GrafoL g, int o, int d)
{
    int vis[V]; // a dimensão do array vis é o número de vértices, pois, é utilizado para testar por quais vértices já passamos
    int i;
    for (i = 0; i < V; i++) vis[i] = 0;
    return (haCaminhoAux(g,o,d,vis));
}
/* A Complexidade desta função é dada por T(V,E) = V + T_haCaminhoAux(V,E) = V + V + E = Theta(V + E)
   Podemos considerar esta função linear, pois percorremos toda a estrutura apenas uma vez.
*/
