// Aula Prática sobre Heaps

/* Podemos ter vários tipos de Heaps que se caracterizam devido à prioridade dos seus elementos
   As mais conhecidas são: 
    1. minHeap -> menor valor é o 1º a sair
    2. maxHeap -> maior valor é o 1º a sair
*/

// Vamos usar as minHeap

/* Para a definição das minHeap temos de cumprir certos invariantes de tipo, são eles:
    1. Invariante de Ordem: 
      > O valor associado a cada nó deve ser inferior ou igual aos seus descendentes.
    2. Invartiante de forma:
      > A árvore binária é completa, sendo que apenas o último nível pode não estar totalmente preenchido;
      > O último nível deve ser preenchido da esquerda para a direita sem lacunas.
*/ 

/* Bubble-up
    Utilizamos o bubble-up quando a árvore não cumpre o requisito de ordem, mais especificamente, quando um, ou mais, dos nós descendentes é menor que o seu "pai".
    Consiste numa técnica de "puxar" os elementos de forma a que a árvore binária passe a cumprir com os invariantes de tipo.
    Quanto à Complexidade:
      > No pior caso, ou seja, quando temos de mudar o elemento sucessivamente das folhas até à raíz da árvore.
         T(N) = O(log N)

            30               30                10
           /  \             /  \              /  \
          60   40   ->    10    40    ->     30   40           
         /                /                 /
        10               60                60

      > No melhor caso, ou seja, quando o descendente é maior ou igual ao "pai".
         T(N) = Omega(1)
*/

/* Bubble-down
    Utilizamos o bubble-down para o processo inverso do bubble-up, ou seja, quando o elemento que temos à raíz se trata do maior elemento da heap.
    Consiste numa técniva de levar esse mesmo elemento até a uma posição onde a árvore binária passe a cumprir com os invariantes de tipo.
*/

/* Remoção de um elemento da minHeap
    Pela definição da minHeap, compreendemos que o elemento que deve ser retirado em primeiro lugar da árvore binária é o elemento da raíz.
    Como retiramos esse elemento, ficamos com o problema de qual colocar no seu lugar.
    Geralmente, a técnica utilizada consiste em colocar o elemento mais à direita das folhas da árvore binária na raíz e, após isso, executar o bubble-down até que
a árvore cumpra com os invariantes de tipo.
*/

// Cálculo da posição do "pai"
     #define PARENT(i) (i - 1) / 2

// Cálculo da posição dos "filhos"
     #define LEFT(i)  (2 * i) + 1
     #define RIGHT(i)  (2 * i) + 2


// --------------------------------

// Definições de Types para os exercícios
typedef int Elem; // elementos da Heap

typedef struct{
    int size;
    int used;
    Elem *values;
} Heap;

// Funções auxiliares para os exercícios 
void swap(int v[], int a,int b)
{
   int temp = v[a];
   v[a] = v[b];
   v[b] = temp;
}

// Função BubbleUp
void bubbleUp (Elem h[], int i){
    while (i > 0 && h[i] < h[PARENT(i)])
    {
      swap (h, i, PARENT(i));
      i = PARENT(i);
    }
}

// Função para inserir um elemento na Heap
int insertHeap (Heap *h, Elem x){
    if (h->used == h->size){
        h->values = realloc (h->values, 2 * (h->size) * sizeof(Elem));
        h->size *= 2;
    }
    h->values[h->used] = x;
    (h->used)++;
    bubbleUp(h->values, h->used-1);
    return 1;
}

// Função BubbleDown 
void bubbleDown (int h[], int N) {
  int i = 0, menor, flag = 1;
  while (2 * i + 2 < N && flag) {
      menor = (h[2 * i+1] < h[2 * i+2]) ? 2 * i + 1 : 2 * i + 2;
      if (h[i] > h[menor]) {
          swap(h,i,menor);
          i = menor;
      }
      else 
          flag = 0;
  } 
  if (2 * i + 1 < N && h[2 * i + 1] < h[i]) 
      swap(h, i, 2* i + 1);
}

// Função para extrair um elemento da Heap
int extractMin (Heap *h, Elem *x){
    if (h->used > 0){
        *x = h->values[0];
        h->values[0] = h->values[h->used - 1];
        (h->used)--;
        bubbleDown (h->values, h->used);
        return 1;
    }
    else return 0;
}
