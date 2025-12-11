#ifndef AVL_INVENTARIO_H
#define AVL_INVENTARIO_H

#include "Inventario.h"

// Nó da árvore AVL para itens do inventário
typedef struct NoAVL {
    Item item;                 // Item armazenado
    struct NoAVL *esquerda;    // Filho esquerdo
    struct NoAVL *direita;     // Filho direito
    int altura;                // Altura do nó
} NoAVL;

// Cria um novo nó com o item
NoAVL* avl_criar_no(Item item);

// Retorna a altura de um nó (0 se NULL)
int avl_altura(NoAVL *no);

// Calcula fator de balanceamento (altura esquerda - altura direita)
int avl_fator_balanceamento(NoAVL *no);

// Rotações
NoAVL* avl_rotacao_direita(NoAVL *y);
NoAVL* avl_rotacao_esquerda(NoAVL *x);

// Insere item pela chave "poder" (e nome como desempate)
NoAVL* avl_inserir(NoAVL *raiz, Item item);

// Busca item por tipo; retorna ponteiro ou NULL
NoAVL* avl_buscar_tipo(NoAVL *raiz, ItemType tipo);

// Impressão em-ordem (itens ordenados por poder)
void avl_imprimir_ordenado(NoAVL *raiz);

// Libera toda a árvore
void avl_liberar(NoAVL *raiz);

#endif // AVL_INVENTARIO_H
