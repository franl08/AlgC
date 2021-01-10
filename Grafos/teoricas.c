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

int travessiaBreadthFirst (GrafoL g, int o, int alc[], int pais[])
{
    // retorna o nº de vértices alcançáveis a partir de o
    // preenche o array alc com 1 se o vértice for alcançável de o
    // preenche o array pais com a árvore produzida

    int cor[V];
    int orla[V];
    int tamOrla, r, v;
    int inicio0, fim0;                // estas duas variáveis com o array orla são suficientes para definir uma Queue Orla
    ListaAdj x;
    r = 0;
    for (int i = 0; i < V; i++){
        alc[i] = 0;
        pais[i] = -2;
    }
    for (int i = 0; i < V; i++) cor[i] = Branco;
    tamOrla = 0;
    inicio0 = fim0 = 0;            // queue vazia
    cor[o] = Cinzento;
    orla[fim0++] = o;            // enqueue (orla, o)
    tamOrla = 1;
    pais[o] = -1;
    while (fim0 != inicio0){   // tamanho da orla > 0
     // escolha do vértice v da orla (cinzento)
        v = orla[inicio0++];  // dequeue (orla)
        cor[v] = Preto;
        tamOrla--;
        r++;
        alc[v] = 1;
        for (x = g[v]; x != NULL; x = x->prox){
            // existe uma aresta de v e destino x->destino
            if (cor[x->destino] == Branco){
                orla[fim0++] = x->destino;   // enqueue (orla, x->destino)
                cor[x->destino] = Cinzento;
                tamOrla++;
                pais[x -> destino] = v;
            }
        }
    
    }
    return r;
}

/* Iniciaremos agora o estudo do caminho mais curto em grafos (Shortest Path). Para isso devemos distinguir as diversas maneiras de considerar um caminho mais curto tendo em conta os grafos
definidos anteriormente.
        1. Quanto ao número de arestas;
        2. Quando ao peso.

   Para calcularmos o caminho mais curto quanto ao número de arestas podemos utilizar o algoritmo "travessiaBreadthFirst" que construímos acima.
   Para calcularmos o caminho mais curto quanto ao peso das arestas iremos estudar o algoritmo de Dijkstra.
*/

/* Algoritmo de Dijkstra
        -> Toma decisões localmente, logo, não podem haver caminhos que retirem peso ao caminho.
        -> Através disso, concluímos que não funciona quando existem arestas com peso negativo.
        -> Podemos adaptar o algoritmo "travessiaBreadthFirst" para o obter (a grande diferença é que a orla deixa de ser uma queue)
*/

// Função que calcula o caminho mais curto com base no algoritmo de Dijkstra

int DijkstraSP (GrafoL g, int o, int alc[], int pais[], int W[]){
    int cor[V];
    // podemos definir a orla de diversas maneiras
    int tamOrla, r, v;
    ListaAdj x;

    r = 0;
    for (int i = 0; i < V; i++){
        alc[i] = 0;
        pais[i] = -2;
        cor[i] = Branco;
    }
    for (int i = 0; i < V; i++) cor[i] = Branco;
    tamOrla = 0;
    cor[o] = Cinzento;
    W[o] = 0;
    // adicionar o à orla
    tamOrla = 1;
    pais[o] = -1;
    while (tamOrla > 0){   // tamanho da orla > 0
     // escolha do vértice v da orla (cinzento)
     // depende da definição da orla
        cor[v] = Preto;
        tamOrla--;
        r++;
        alc[v] = 1;
        for (x = g[v]; x != NULL; x = x->prox){
            // existe uma aresta de v e destino x -> destino ccom peso x -> peso
            if (cor[x->destino] == Branco){
                // x -> destino é não visitado
                // adicionar x -> destino à orla
                cor[x->destino] = Cinzento;
                tamOrla++;
                pais[x -> destino] = v;
                W[x -> destino] = W[v] + x -> peso;
            }
            else if (cor [x -> destino] == Cinzento && W[v] + x -> peso < W[x -> destino]){
                W[x -> destino] = W[v] + x -> peso;
                pais [x -> destino] = v;
            }
        }
    
    }
    return r;
}

/* A complexidade desta função é dada por  T_DijsktraSP (V,E) = V + T_Adicionar + V * T_Selecionar + V * T_Adicionar + (E - V) * T_Atualizar
      Logo, podemos concluir que irá depender da forma que vamos definir a orla.
      Alternativas para definir a Orla:
    1. Orla é um array de vértices ordenados por W
       T_Adicionar = O(V);
       T_Selecionar = O(1);
       T_Atualizar = O(V);
       T_DijsktraSP = V + V + V * 1 + V * V + E * V - (V^2) = O(V + E * V) 
    2. Orla é um array desordenado
       T_Adicionar = O(1);
       T_Selecionar = O(V);
       T_Atualizar = O(1);
       T_DijsktraSP = V + 1 + V * V + V * 1 + E * 1 - V * 1 = V + (V^2) + V * E = O((V^2) + E)
    3. Orla é uma min-heap ordenada por W (melhor solução estudada)
       T_Adicionar = O(log V)
       T_Selecionar = O(log V)
       T_Atualizar = O(log V)
       T_DijsktraSP = V + (log V) + V * (log V) + V * (log V) + E * (log V)  - V * (log V) = O(E * log(V) + V * log(V))
*/

/* Definiremos agora, um algoritmo que, em alguns casos, pode vir a ser mais eficiente que o Dijkstra e, além disso, tem a mais valia
de funcionar com arestas de peso negativo. 
*/

/* Algoritmo de Floyd-Warshall:
        Dado um grafo g, devemos calcular o grafo g+ em que existe uma aresta de a para b com peso p se e só se, em g, o caminho mais curto de a para b tem peso p.
*/

/* Para isto, devemos ver o que é um caminho, ou seja, uma sequência de arestas encadeadas. Podemos ter os seguintes caminhos:
        1. Caminhos Simples (caminhos com 1 só aresta);
        2. Caminhos com Vértices Intermédios.
*/

// Definiremos a ideia base de implementação deste algoritmo:
 
     g+ = g; // Para ser possível cumprir o invariante de ciclo não podemos ter vértices intermédios inicialmente, para isso, inicializamos g+ como g
    for (i = 0; i < V; i++)
        // Invariante de Ciclo:
        /* existe uma aresta de a para b em g+ se e só se
           o caminho mais curto de a até b
           usando um número de vértices < i como intermédios
           tem custo p
        */
        /* O nosso objetivo de agora é:
            Incluir um caminho de a para b no caso de termos um caminho de a para i e de i para b.
            Caso já exista um caminho de a para b mais curto do que desejamos acrescentar devemos deixar esse mesmo caminho.
        */
        // Fazemos algo do género:
        for (a in antecessores(i))
            for (b in sucessores(i))
                acrescentar info da aresta (a,b);
    
// A complexidade deste Algoritmo é dado por  T_warshall (V, E) = (V^2) + V * (V * V) = (V^2) + (V^3) = Theta (V^3)