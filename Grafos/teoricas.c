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

int procuraAux (GrafoL g, int o, int d, int vis[])
{
    ListaAdj x;
    vis[o] = 1; // não iniciamos mais procuras a partir da origem
    if (o == d) return 1;
    for (x = g[o]; x != NULL; x = x->prox){
        // Existe uma aresta desde o até x->destino com peso x->peso
        if (vis[x->destino] == 0 // ainda não procuramos a partir deste ponto
            && 
            procuraAux(g, x->destino, d, vis)) return 1;
    }
    return 0;
}
// A Complexidade desta função é dada por T(V,E) = O(V + E)

int procura (GrafoL g, int o, int d)
{
    int vis[V]; // a dimensão do array vis é o número de vértices, pois, é utilizado para testar por quais vértices já passamos
    int i;
    for (i = 0; i < V; i++) vis[i] = 0;
    return (procuraAux(g,o,d,vis));
}
/* A Complexidade desta função é dada por T(V,E) = V + T_procuraAux(V,E) = V + V + E = O(V + E)
   Podemos considerar esta função linear, pois percorremos toda a estrutura apenas uma vez.
*/

// Dado um vértice, dizemos que a ligação entre esses vértices e os seus pontos alcançáveis chama-se travessia.

// Função que analisa a travessia em profundidade e a sua função auxiliar

int travessiaDepthFirstAux (GrafoL g, int o, int vis[])
{
    int r;
    ListaAdj x;
    vis[o] = 1;
    r = 1;
    for (x = g[o]; x != NULL; x = x->prox)
       if (vis[x->destino] == 0)
          r += travessiaDepthFirstAux (g, x->destino, vis);
    return r;
}

int travessiaDepthFirst (GrafoL g, int o)
{
    int v[V];
    int i;
    for (i = 0; i < V; i++) v[i] = 0;
    return (travessiaDepthFirstAux(g, o, v));   // retorna o número de vértices alcançáveis a partir de o
}

/* Outro modo de travessia de um grafo é a travessia em largura.
Para podermos criar esta função devemos compreender o conceito de Algoritmos Baseados em Orla.
Para a definição destes algoritmos, devemos considerar que os vértices dos grafos estão divididos em 3 zonas:
   1. Não visitados (Branco) -> não sabemos nada sobre eles;
   2. Orla (Cinzento) -> os próximos elementos a serem visitados;
   3. Visitados (Preto) -> elementos que já foram visitados e não o voltarão a ser.
Podemos considerar que dento destes algoritmos existe o seguinte Invariante:
   "Todos os nodos na orla são adjacentes a algum vértice visitado."
no entanto existe uma exceção para este invariante.

Para estes algoritmos devemos:
   1. inicializar todos os vértices não visitados (Brancos)
   2. inicializar a orla, da seguinte forma:
      while (houver candidatos){
          -> escolher vértice v da orla (Cinzento)
          -> v passa para os visitados (Preto)
          for (x in adjacentes de v)
             acrescentar x à orla
      }

*/

// Faremos, então, agora a função que analisa a travessia em largura

#define Branco 0
#define Cinzento 1
#define Preto 2

int travessiaBreadthFirst (GrafoL g, int o)
{
    int cor[v];
    int orla[v];
    int i, v, r;
    int inicio0, fim0;                // estas duas variáveis são as suficientes para definir uma Queue Orla
    ListaAdj x;
    for (i = 0; i < v; i++) cor[i] = Branco;
    inicio0 = fim0 = 0;            // queue vazia
    cor[o] = Cinzento;
    orla[fim0++] = o;            // enqueue (orla, o)
    tamOrla = 1;
    while (fim0 != inicio0){   // tamanho da orla > 0
        v = orla[inicio0++];  // dequeue (orla)
        cor[v] = Preto;
        r++;
        for (x = g[v]; x != NULL; x = x->prox){
            // existe uma aresta de v e destino x->destino
            if (cor[x->destino] == Branco){
                orla[fim0++] = x->destino;   // enqueue (orla, x->destino)
                cor[x->destino] = Cinzento;
            }
        }
    
    }
    return r;
}