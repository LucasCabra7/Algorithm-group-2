#ifndef HEAP_MINIMO_H
#define HEAP_MINIMO_H

// Heap mínimo simples de inteiros
#define HEAP_MAX 128

typedef struct {
    int dados[HEAP_MAX];
    int tamanho;
} HeapMinimo;

void heap_inicializar(HeapMinimo *h);
void heap_inserir(HeapMinimo *h, int valor);
int heap_remover_min(HeapMinimo *h, int *ok);
int heap_vazio(HeapMinimo *h);

#endif // HEAP_MINIMO_H
