/*
 * Exemplo de Árvore Geradora Mínima (MST) - Algoritmo de Kruskal
 * Conecta prédios do mapa com o menor custo de caminhos
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 20
#define MAX_EDGES 100

typedef struct {
    int u, v, peso;
} Aresta;

int parent[MAX_VERTICES];

int find(int x) {
    if (parent[x] != x) parent[x] = find(parent[x]);
    return parent[x];
}

void unite(int x, int y) {
    parent[find(x)] = find(y);
}

int kruskal(int n, Aresta arestas[], int m) {
    int custo = 0;
    for (int i = 0; i < n; i++) parent[i] = i;
    // Ordena arestas por peso (Insertion Sort para didático)
    for (int i = 1; i < m; i++) {
        Aresta temp = arestas[i];
        int j = i - 1;
        while (j >= 0 && arestas[j].peso > temp.peso) {
            arestas[j+1] = arestas[j];
            j--;
        }
        arestas[j+1] = temp;
    }
    int usados = 0;
    for (int i = 0; i < m && usados < n-1; i++) {
        if (find(arestas[i].u) != find(arestas[i].v)) {
            unite(arestas[i].u, arestas[i].v);
            custo += arestas[i].peso;
            usados++;
            printf("Aresta (%d-%d) peso %d\n", arestas[i].u, arestas[i].v, arestas[i].peso);
        }
    }
    return custo;
}

int main() {
    // Exemplo: 4 prédios conectados
    int n = 4;
    Aresta arestas[] = {
        {0, 1, 10}, {0, 2, 6}, {0, 3, 15}, {1, 2, 5}, {2, 3, 4}
    };
    int m = 5;
    int custo = kruskal(n, arestas, m);
    printf("Custo total da MST: %d\n", custo);
    return 0;
}
