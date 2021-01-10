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

typedef int TreeEntry;
typedef enum balancefactor {
  LH;            // sub-árvore da esquerda tem maior altura
  RH;            // sub-árvore da direita tem maior altura
  EH             // alturas iguais
} BalanceFactor;

struct treenode{
    BalanceFactor bf;
    TreeEntry entry; 
    struct treeNode *left;
    struct treeNode *right;
};

typedef struct treenode *Tree;

// Função que faz a rotação à esquerda necessária em 3.a

// requires (t != NULL) && (t -> right != NULL)
Tree rotateLeft (Tree t){
    Tree aux = t -> right;
    t -> right = aux -> left;
    aux -> left = t;
    t = aux;
    return t;
}
// Falta rotateRight





// Função que balanceia uma árvore que deixou de respeitar o invariante por uma inserção de um elemento à direita

// requires (t != NULL) && (t -> right != NULL)
Tree balanceRight (Tree t){
  if (t -> right -> bf == RH) {      // rotação simples, ou seja, o caso 3.a
    t = rotateLeft (t);
    t -> bf = EH;
    t -> left -> bf = EH;
  }
  else {                             // rotação dupla, ou seja, o caso 3.b
    t -> right = rotateRight (t -> right);
    t = rotateLeft(t);
    switch (t -> bf){
      case EH:
          t -> left -> bf = EH;
          t -> right -> bf = EH;
          break;
      case LH:
          t -> left -> bf = EH;
          t -> right -> bf = RH;
          break;
      case RH:
          t -> left -> bf = LH;
          t -> right -> bf = EH;
    }
    t -> bf = EH;
  }
  return t;
}

// Falta balanceLeft






// Função que insere um elemento numa árvore AVL

Tree insertTree (Tree t, TreeEntry e, int *cresceu){
  if (t == NULL){
    t = (Tree)malloc(sizeof(struct treenode));
    t -> entry = e;
    t -> right = t -> left = NULL;
    t -> bf = EH;
    *cresceu = 1;
  }
  else if (e > t -> entry){
    t -> right = insertTree(t -> right, e, cresceu);
    if (*cresceu){
      switch (t -> bf){
        case LH:
            t -> bf = EH;
            *cresceu = 0;
            break;
        case EH:
            t -> bf = RH;
            *cresceu = 1;
            break;
        case RH:
            t = balanceRight(t);
            *cresceu = 0;
      }
    }
  }
  else {
      t -> left = insertTree(t -> left, e, cresceu);
      if (*cresceu){
          // falta completar a parte da inserção de um elemento à esquerda
      }
  }
  return t;
}