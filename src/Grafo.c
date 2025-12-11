/*
 * GRAFO.C - Implementação de grafos com BFS e DFS
 * 
 * Este arquivo contém implementações educacionais de:
 * - Grafo com lista de adjacência
 * - BFS (Busca em Largura)
 * - DFS (Busca em Profundidade)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Grafo.h"

/*
 * Fila simples para BFS
 * FIFO: Primeiro que entra é o primeiro que sai
 */
typedef struct {
    int dados[MAX_VERTICES];
    int inicio;
    int fim;
} FilaBFS;

/*
 * Pilha simples para DFS iterativo
 * LIFO: Último que entra é o primeiro que sai
 */
typedef struct {
    int dados[MAX_VERTICES];
    int topo;
} PilhaDFS;

/*
 * ===== FUNÇÕES AUXILIARES DE FILA E PILHA =====
 */

/*
 * Inicializa fila vazia
 */
FilaBFS fila_criar(void) {
    FilaBFS fila;
    fila.inicio = 0;
    fila.fim = 0;
    return fila;
}

/*
 * Adiciona elemento no final da fila (enqueue)
 * Complexidade: O(1)
 */
void fila_adicionar(FilaBFS *fila, int valor) {
    fila->dados[fila->fim] = valor;
    fila->fim++;
}

/*
 * Remove e retorna primeiro elemento da fila (dequeue)
 * Complexidade: O(1)
 */
int fila_remover(FilaBFS *fila) {
    int valor = fila->dados[fila->inicio];
    fila->inicio++;
    return valor;
}

/*
 * Verifica se fila está vazia
 */
bool fila_vazia(FilaBFS *fila) {
    return fila->inicio >= fila->fim;
}

/*
 * Inicializa pilha vazia
 */
PilhaDFS pilha_criar(void) {
    PilhaDFS pilha;
    pilha.topo = 0;
    return pilha;
}

/*
 * Adiciona elemento no topo da pilha (push)
 * Complexidade: O(1)
 */
void pilha_adicionar(PilhaDFS *pilha, int valor) {
    pilha->dados[pilha->topo] = valor;
    pilha->topo++;
}

/*
 * Remove e retorna elemento do topo (pop)
 * Complexidade: O(1)
 */
int pilha_remover(PilhaDFS *pilha) {
    pilha->topo--;
    return pilha->dados[pilha->topo];
}

/*
 * Verifica se pilha está vazia
 */
bool pilha_vazia(PilhaDFS *pilha) {
    return pilha->topo == 0;
}

/*
 * ===== FUNÇÕES PRINCIPAIS DO GRAFO =====
 */

Grafo grafo_criar(void) {
    Grafo grafo;
    grafo.numero_vertices = 0;
    
    // Inicializa todos os vértices
    for (int i = 0; i < MAX_VERTICES; i++) {
        grafo.vertices[i].numero_vizinhos = 0;
        grafo.vertices[i].x = 0;
        grafo.vertices[i].y = 0;
    }
    
    return grafo;
}

void grafo_adicionar_aresta(Grafo *grafo, int vertice_origem, int vertice_destino) {
    // Validação básica
    if (vertice_origem < 0 || vertice_origem >= MAX_VERTICES) return;
    if (vertice_destino < 0 || vertice_destino >= MAX_VERTICES) return;
    
    // Atualiza numero_vertices se necessário
    if (vertice_origem >= grafo->numero_vertices) {
        grafo->numero_vertices = vertice_origem + 1;
    }
    if (vertice_destino >= grafo->numero_vertices) {
        grafo->numero_vertices = vertice_destino + 1;
    }
    
    // Adiciona aresta de origem para destino (grafo não-direcionado)
    if (grafo->vertices[vertice_origem].numero_vizinhos < MAX_VIZINHOS) {
        grafo->vertices[vertice_origem].vizinhos[grafo->vertices[vertice_origem].numero_vizinhos] = vertice_destino;
        grafo->vertices[vertice_origem].numero_vizinhos++;
    }
    
    // Para grafo não-direcionado, também adiciona volta
    if (grafo->vertices[vertice_destino].numero_vizinhos < MAX_VIZINHOS) {
        grafo->vertices[vertice_destino].vizinhos[grafo->vertices[vertice_destino].numero_vizinhos] = vertice_origem;
        grafo->vertices[vertice_destino].numero_vizinhos++;
    }
}

/*
 * ===== BUSCA EM LARGURA (BFS) =====
 * 
 * Algoritmo:
 *   1. Marca origem como visitada e coloca em fila
 *   2. Enquanto fila não vazia:
 *        a. Remove primeiro da fila (FIFO)
 *        b. Para cada vizinho não visitado:
 *           - Marca como visitado
 *           - Calcula distância (distância_vizinho = distância_atual + 1)
 *           - Adiciona na fila
 * 
 * Complexidade: O(V + E)
 *   - V: número de vértices (cada visitado uma vez)
 *   - E: número de arestas (cada percorrida uma vez)
 * 
 * Quando usar BFS:
 *   - Encontrar caminho mais curto em grafo não-ponderado
 *   - Exploração por camadas (nível por nível)
 *   - IA em jogo: zumbi perseguindo jogador
 */
EstadoBusca grafo_bfs(Grafo *grafo, int vertice_inicio) {
    EstadoBusca estado;
    
    // Inicializa: ninguém visitado, todos com distância infinita
    for (int i = 0; i < grafo->numero_vertices; i++) {
        estado.visitado[i] = false;
        estado.distancia[i] = 9999; // "infinito"
    }
    
    // Cria fila para BFS
    FilaBFS fila = fila_criar();
    
    // Marca origem como visitada
    estado.visitado[vertice_inicio] = true;
    estado.distancia[vertice_inicio] = 0;
    fila_adicionar(&fila, vertice_inicio);
    
    // Processa elementos da fila
    while (!fila_vazia(&fila)) {
        // Remove primeiro da fila
        int vertice_atual = fila_remover(&fila);
        
        // Para cada vizinho do vértice atual
        for (int i = 0; i < grafo->vertices[vertice_atual].numero_vizinhos; i++) {
            int vizinho = grafo->vertices[vertice_atual].vizinhos[i];
            
            // Se vizinho não foi visitado
            if (!estado.visitado[vizinho]) {
                // Marca como visitado
                estado.visitado[vizinho] = true;
                
                // Calcula distância: distância do atual + 1
                estado.distancia[vizinho] = estado.distancia[vertice_atual] + 1;
                
                // Adiciona vizinho na fila para processar depois
                fila_adicionar(&fila, vizinho);
            }
        }
    }
    
    return estado;
}

/*
 * ===== BUSCA EM PROFUNDIDADE (DFS) =====
 * 
 * Algoritmo (versão iterativa com pilha):
 *   1. Marca origem como visitada e coloca em pilha
 *   2. Enquanto pilha não vazia:
 *        a. Remove do topo da pilha (LIFO)
 *        b. Para cada vizinho não visitado:
 *           - Marca como visitado
 *           - Calcula distância
 *           - Adiciona na pilha
 * 
 * Diferença de BFS:
 *   - BFS usa FILA (FIFO) - processa camadas
 *   - DFS usa PILHA (LIFO) - vai fundo antes de alternar
 * 
 * Complexidade: O(V + E)
 * 
 * Quando usar DFS:
 *   - Detectar ciclos
 *   - Componentes conectadas
 *   - Busca em profundidade (labirintos)
 *   - Exploração completa de região
 */
EstadoBusca grafo_dfs(Grafo *grafo, int vertice_inicio) {
    EstadoBusca estado;
    
    // Inicializa: ninguém visitado, todos com distância infinita
    for (int i = 0; i < grafo->numero_vertices; i++) {
        estado.visitado[i] = false;
        estado.distancia[i] = 9999; // "infinito"
    }
    
    // Cria pilha para DFS
    PilhaDFS pilha = pilha_criar();
    
    // Marca origem como visitada
    estado.visitado[vertice_inicio] = true;
    estado.distancia[vertice_inicio] = 0;
    pilha_adicionar(&pilha, vertice_inicio);
    
    // Processa elementos da pilha
    while (!pilha_vazia(&pilha)) {
        // Remove do topo da pilha
        int vertice_atual = pilha_remover(&pilha);
        
        // Para cada vizinho do vértice atual
        for (int i = 0; i < grafo->vertices[vertice_atual].numero_vizinhos; i++) {
            int vizinho = grafo->vertices[vertice_atual].vizinhos[i];
            
            // Se vizinho não foi visitado
            if (!estado.visitado[vizinho]) {
                // Marca como visitado
                estado.visitado[vizinho] = true;
                
                // Calcula distância: distância do atual + 1
                estado.distancia[vizinho] = estado.distancia[vertice_atual] + 1;
                
                // Adiciona vizinho na pilha para processar depois
                pilha_adicionar(&pilha, vizinho);
            }
        }
    }
    
    return estado;
}

int grafo_primeiro_vizinho_nao_visitado(Grafo *grafo, int vertice, EstadoBusca *estado) {
    for (int i = 0; i < grafo->vertices[vertice].numero_vizinhos; i++) {
        int vizinho = grafo->vertices[vertice].vizinhos[i];
        if (!estado->visitado[vizinho]) {
            return vizinho;
        }
    }
    return -1;
}

/*
 * ===== FUNÇÕES DE DEBUG =====
 */

void grafo_imprimir(Grafo *grafo) {
    printf("\n===== ESTRUTURA DO GRAFO =====\n");
    printf("Total de vértices: %d\n\n", grafo->numero_vertices);
    
    for (int i = 0; i < grafo->numero_vertices; i++) {
        printf("Vértice %d conectado a: [", i);
        for (int j = 0; j < grafo->vertices[i].numero_vizinhos; j++) {
            printf("%d", grafo->vertices[i].vizinhos[j]);
            if (j < grafo->vertices[i].numero_vizinhos - 1) printf(", ");
        }
        printf("]\n");
    }
}

void grafo_imprimir_busca(EstadoBusca *resultado, int num_vertices) {
    printf("\nVértices visitados: ");
    for (int i = 0; i < num_vertices; i++) {
        if (resultado->visitado[i]) printf("%d ", i);
    }
    printf("\n");
    
    printf("Distâncias da origem:\n");
    for (int i = 0; i < num_vertices; i++) {
        if (resultado->visitado[i]) {
            printf("  Vértice %d: distância %d\n", i, resultado->distancia[i]);
        }
    }
}
