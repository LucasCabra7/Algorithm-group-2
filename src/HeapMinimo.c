#include "HeapMinimo.h"

static void trocar(int *a, int *b) {
    int tmp = *a; *a = *b; *b = tmp;
}

void heap_inicializar(HeapMinimo *h) {
    h->tamanho = 0;
}

static void subir(HeapMinimo *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (h->dados[pai] <= h->dados[i]) break;
        trocar(&h->dados[pai], &h->dados[i]);
        i = pai;
    }
}

void heap_inserir(HeapMinimo *h, int valor) {
    if (h->tamanho >= HEAP_MAX) return; // cheio
    h->dados[h->tamanho] = valor;
    subir(h, h->tamanho);
    h->tamanho++;
}

static void descer(HeapMinimo *h, int i) {
    while (1) {
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;
        int menor = i;
        if (esq < h->tamanho && h->dados[esq] < h->dados[menor]) menor = esq;
        if (dir < h->tamanho && h->dados[dir] < h->dados[menor]) menor = dir;
        if (menor == i) break;
        trocar(&h->dados[i], &h->dados[menor]);
        i = menor;
    }
}

int heap_remover_min(HeapMinimo *h, int *ok) {
    if (h->tamanho == 0) { if(ok) *ok = 0; return 0; }
    int min = h->dados[0];
    h->tamanho--;
    h->dados[0] = h->dados[h->tamanho];
    descer(h, 0);
    if(ok) *ok = 1;
    return min;
}

int heap_vazio(HeapMinimo *h) {
    return h->tamanho == 0;
}
