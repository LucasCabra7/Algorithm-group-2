#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVLInventario.h"

static int max_int(int a, int b) { return (a > b) ? a : b; }

NoAVL* avl_criar_no(Item item) {
    NoAVL *novo = (NoAVL*) malloc(sizeof(NoAVL));
    if (!novo) return NULL;
    novo->item = item;
    novo->esquerda = novo->direita = NULL;
    novo->altura = 1; // novo nó começa com altura 1
    return novo;
}

int avl_altura(NoAVL *no) {
    return no ? no->altura : 0;
}

int avl_fator_balanceamento(NoAVL *no) {
    if (!no) return 0;
    return avl_altura(no->esquerda) - avl_altura(no->direita);
}

NoAVL* avl_rotacao_direita(NoAVL *y) {
    NoAVL *x = y->esquerda;
    NoAVL *T2 = x->direita;

    // Rotação
    x->direita = y;
    y->esquerda = T2;

    // Atualiza alturas
    y->altura = 1 + max_int(avl_altura(y->esquerda), avl_altura(y->direita));
    x->altura = 1 + max_int(avl_altura(x->esquerda), avl_altura(x->direita));

    return x;
}

NoAVL* avl_rotacao_esquerda(NoAVL *x) {
    NoAVL *y = x->direita;
    NoAVL *T2 = y->esquerda;

    // Rotação
    y->esquerda = x;
    x->direita = T2;

    // Atualiza alturas
    x->altura = 1 + max_int(avl_altura(x->esquerda), avl_altura(x->direita));
    y->altura = 1 + max_int(avl_altura(y->esquerda), avl_altura(y->direita));

    return y;
}

// Comparação: primeiro por poder, depois por nome para desempate
static int comparar_item(const Item *a, const Item *b) {
    if (a->poder < b->poder) return -1;
    if (a->poder > b->poder) return 1;
    return strcmp(a->nome, b->nome);
}

NoAVL* avl_inserir(NoAVL *raiz, Item item) {
    if (!raiz) return avl_criar_no(item);

    if (comparar_item(&item, &raiz->item) < 0) {
        raiz->esquerda = avl_inserir(raiz->esquerda, item);
    } else if (comparar_item(&item, &raiz->item) > 0) {
        raiz->direita = avl_inserir(raiz->direita, item);
    } else {
        // item duplicado: inserir à direita para manter estrutura
        raiz->direita = avl_inserir(raiz->direita, item);
    }

    // Atualiza altura
    raiz->altura = 1 + max_int(avl_altura(raiz->esquerda), avl_altura(raiz->direita));

    // Verifica balanceamento
    int fb = avl_fator_balanceamento(raiz);

    // Caso Esquerda-Esquerda
    if (fb > 1 && comparar_item(&item, &raiz->esquerda->item) < 0)
        return avl_rotacao_direita(raiz);

    // Caso Direita-Direita
    if (fb < -1 && comparar_item(&item, &raiz->direita->item) > 0)
        return avl_rotacao_esquerda(raiz);

    // Caso Esquerda-Direita
    if (fb > 1 && comparar_item(&item, &raiz->esquerda->item) > 0) {
        raiz->esquerda = avl_rotacao_esquerda(raiz->esquerda);
        return avl_rotacao_direita(raiz);
    }

    // Caso Direita-Esquerda
    if (fb < -1 && comparar_item(&item, &raiz->direita->item) < 0) {
        raiz->direita = avl_rotacao_direita(raiz->direita);
        return avl_rotacao_esquerda(raiz);
    }

    return raiz; // já balanceado
}

NoAVL* avl_buscar_tipo(NoAVL *raiz, ItemType tipo) {
    if (!raiz) return NULL;
    if (raiz->item.tipo == tipo) return raiz;
    NoAVL *esq = avl_buscar_tipo(raiz->esquerda, tipo);
    if (esq) return esq;
    return avl_buscar_tipo(raiz->direita, tipo);
}

void avl_imprimir_ordenado(NoAVL *raiz) {
    if (!raiz) return;
    avl_imprimir_ordenado(raiz->esquerda);
    printf("- %s (poder %d, qtd %d)\n", raiz->item.nome, raiz->item.poder, raiz->item.quantidade);
    avl_imprimir_ordenado(raiz->direita);
}

void avl_liberar(NoAVL *raiz) {
    if (!raiz) return;
    avl_liberar(raiz->esquerda);
    avl_liberar(raiz->direita);
    free(raiz);
}
