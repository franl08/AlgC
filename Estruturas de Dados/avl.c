/* ADT Dicionário / Array Associativo / Mapeamento
  -> Armazena pares chave -> valor, tendo a semântica de um função finita. As suas operações básicas são:
     1. Inserção de um par chave -> valor;
     2. Alteração do valor associado a uma chave;
     3. Consulta com base numa chave, podendo obter-se como resultado um valor ou a indicação de que a chave não ocorre no dicionário;
     4. Remoção de um par dada a respetiva chave

(Note-se que as operações 1. e 2. podem ser implementadas em conjunto.)

  -> Para otimizar estas estruturas de dados devemos garantir que a altura das árvores é log(N) e é aí que surgem as árvores AVL.

*/

/* Árvores AVL
  -> árvores binárias de procura em que todos os nós satisfazem o seguinte invariante estrutural:
     - As alturas da sub-árvore da esquerda e da sub-árvore da direita diferem no máximo numa unidade, ou seja:
                                          (h_e) - (h_d) <= 14
*/

/* Algoritmo de inserção de um elemento à direita numa árvore AVL
   Devido às restrições desta estrutura de dados, na inserção de um elemento vamos-nos deparar com diversos casos possíveis:

   -> Caso 0: a inserção do elemento não provoca um aumento da altura da sub-árvore da direita. 
        Neste caso, só precisamos de inserir o elemento sem processos adicionais.

   -> Caso 1: (h_e) = (h_d) + 1
        Na fase inicial, a sub-árvore da esquerda é mais pesada. Após a inserção do elemento, passamos a ter (h_e) = (h_d)

    -> Caso 2: (h_e) = (h_d)
        Na fase inicial, as sub-árvores têm o mesmo peso. Após a inserção, passamos a ter (h_d) = (h_e) + 1

    -> Caso 3: (h_d) = (h_e) + 1
        Vamos dividir este caso em 2, o caso 3.a e o caso 3.b. Consideremos a seguinte árvore:

                                                            A
                                                          /   \ 
                                                    (h) T1     C
                                                             /   \
                                                      (h)  T2     T3 (h)
 
       -> Caso 3.a: a inserção produziu um aumento da altura de T3 (estraga o invariante no nó A)
            Ou seja, gera a seguinte árvore:

                                                            A
                                                          /   \ 
                                                    (h) T1     C
                                                             /   \
                                                      (h)  T2     T3 (h + 1)

            Para resolver este caso, fazemos uma rotação à esquerda, passando C para a raíz e A para a sub-árvore esquerda. Além disso, para se manter uma árvore
binária de procura, T2 passa para a sub-árvore direita de A.
            Ficamos, assim com a seguinte árvore:
                              
                                                            C
                                                           / \
                                                          A   T3 (h + 1)
                                                         / \ 
                                                   (h) T1   T2 (h)

        -> Caso 3.b: a inserção produrziu um aumento da altura T2
            Ou seja, gera a seguinte árvore:

                                                            A
                                                          /   \ 
                                                    (h) T1     C
                                                             /   \
                                                        |_______|  T3 (h + 1)
                                                        |   B   |  
                                                  |_____|  / \  |_________|
                                                  | (h)  T2a  T2b (h - 1) |
                                                  |_______________________|
                                                            T2
                                                   
            Para resolver este caso, é necessário fazer 2 rotações:
              1. rodamos o C para a direita (sobe o B e desce o C)

                                                           A
                                                         /   \
                                                    (h) T1    B 
                                                            /   \
                                                      (h) T2a    C
                                                               /   \
                                                    (h - 1) T2b     T3 (h)
              
              2. rodamos agora como no caso 3.a
                            
                                                               B
                                                          /        \
                                                         A          C
                                                       /   \      /   \
                                                      T1   T2a  T2b    T3


(Note-se que para uma inserção à esquerda, apenas precisamos de reverter as rotações)
*/



// Definições auxiliares para algoritmos de AVL em C

#define LH 1; // a sub-árvore da esquerda é mais pesada
#define EH 0; // árvore balanceada
#define RH -1; // a sub-árvore da direita é mais pesada

typedef struct tree{
    int bf;
    int key, info;
    struct tree *left, *right
} *Tree;

// Função que faz a rotação à esquerda necessária em 3.a

// requires (t != NULL) && (t -> right != NULL)
Tree rotateLeft (Tree t){
    Tree aux = t -> right;
    t -> right = aux -> left;
    aux -> left = t;
    t = aux;
    return t;
}

// Função que faz a rotação à direita

// requires (t != NULL) && (t -> left != NULL)
Tree rotateRight (Tree t){
  Tree aux = t -> left;
  t -> left = aux -> right;
  aux -> right = t;
  t = aux;
  return t;
}


// Função que balanceia uma árvore que deixou de respeitar o invariante por uma inserção de um elemento à direita

// requires (t != NULL) && (t -> right != NULL)
Tree balanceRight (Tree t){
  Tree aux1, aux2;
  aux1 = t -> right;
  if (aux1 -> bf = RH){
    t -> bf = aux1 -> bf = EH;
    t = rotateLeft(t);
  }
  else{
    aux2 = aux1 -> left;
    switch (aux2 -> bf){
      case EH:
        t -> bf = aux1 -> bf = EH;
        break;
      case LH:
        t -> bf = EH;
        aux1 -> bf = RH;
        break;
      case RH:
        t -> bf = LH;
        aux1 -> bf = BAL;
        break;
    }
    aux2 -> bf = EH;
    t -> right = rotateRight (aux1);
    t = rotateLeft (t);
  }
  return a;
}

// Função que balanceia uma árvore que deixou de respeitar o invariante por uma inserção de um elemento à esquerda

// requires (t != NULL) && (t -> right != NULL)
Tree balanceLeft (Tree t){
  Tree aux1, aux2;
  aux1 = t -> left;
  if (aux1 -> bf == LH){
    t -> bf = aux1 -> bf = EH;
    t = rotateRight (t);
  }
  else{
    aux2 = aux1 -> right;
    switch (aux2 -> bf){
      case EH:
        t -> bf = aux1 -> bf = EH;
        break;
      case LH:
        t -> bf = RH;
        aux1 -> bf = EH;
        break;
      case RH:
        t -> bf = BAL;
        aux1 -> bf = LEFT;
        break;
    }
    aux2 -> bal = BAL;
    t -> left = rotateLeft(aux1);
    t = rotateRight (t);
  }
  return t;
}




// Função que atualiza uma AVL recursivamente

int updateAVL (Tree *a, int k, int i){
  int g, u;
  *a = updateAVLRec (*a, k, i, &g, &u);
  return u;
}

Tree updateAVLRec (Tree a, int k, int i, int *g, int *u){
  if (a == NULL){
    a = malloc (sizeof (struct tree));
    a -> key = k;
    a -> info = i;
    a -> bal = EH;
    a -> left = a -> right = NULL;
    *g = 1; 
    *u = 0;
  }
  else if (a -> key == k){
    a -> info = i;
    *g = 0;
    *u = 1;
  }
  else if (a -> key > k){
    a -> left = updateAVLRec(a -> left, k, i, g, u);
    if (*g == 1)
      switch (a -> bal){
        case LH:
          a = balanceLeft(a);
          *g = 0;
          break;
        case EH:
          a -> bal = LH;
          break;
        case RH:
          a -> bal = EH;
          *g = 0;
          break;
      }
  }
  else {
    a -> right = updateAVLRec(a -> right, k, i, g, u);
    if (*g == 1)
      switch (a -> bal){
        case RH:
          a = balanceRight(a);
          *g = 0;
          break;
        case EH:
          a -> bal = RH; 
          break;
        case LH:
          a -> bal = EH;
          *g = 0;
          break;
      }
  }
  return a;
}





















