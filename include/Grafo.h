/*
 * GRAFO.H - Estrutura e funções para trabalhar com grafos
 * 
 * Disciplina: Algoritmos
 * Objetivo educacional: Implementar grafos usando lista de adjacência
 * 
 * Um grafo é uma estrutura matemática composta por:
 *   - Vértices (nós): pontos no grafo
 *   - Arestas (conexões): linhas conectando vértices
 * 
 * Representação escolhida: LISTA DE ADJACÊNCIA
 *   Razão: mapa é esparso (cada tile tem no máximo 4 vizinhos)
 *   Complexidade: O(V+E) espaço, ideal quando E << V²
 */

#ifndef GRAFO_H
#define GRAFO_H

#include <stdbool.h>

#define MAX_VERTICES 500
#define MAX_VIZINHOS 4

/*
 * Estrutura que representa um vértice do grafo
 * Cada vértice mantém lista de seus vizinhos diretos
 */
typedef struct {
    int vizinhos[MAX_VIZINHOS];      // IDs dos vértices vizinhos
    int numero_vizinhos;              // Quantos vizinhos tem
    int x;                            // Coordenada X (para mapa)
    int y;                            // Coordenada Y (para mapa)
} Vertice;

/*
 * Estrutura que representa o grafo completo
 */
typedef struct {
    Vertice vertices[MAX_VERTICES];   // Array de todos os vértices
    int numero_vertices;              // Total de vértices
} Grafo;

/*
 * Estrutura para rastrear visitados durante busca
 */
typedef struct {
    bool visitado[MAX_VERTICES];      // Marca se cada vértice foi visitado
    int distancia[MAX_VERTICES];      // Distância do vértice de origem
} EstadoBusca;

/* ========== FUNÇÕES PRINCIPAIS ========== */

/*
 * Cria um grafo vazio
 * Retorna: Grafo inicializado
 */
Grafo grafo_criar(void);

/*
 * Adiciona uma aresta bidirecional entre dois vértices
 * Complexidade: O(1) amortizado (append na lista)
 */
void grafo_adicionar_aresta(Grafo *grafo, int vertice_origem, int vertice_destino);

/*
 * BFS (Busca em Largura) - Breadth First Search
 * Visita vértices em ORDEM DE CHEGADA (usa fila)
 * Complexidade: O(V + E)
 * Retorna: EstadoBusca com distâncias calculadas
 */
EstadoBusca grafo_bfs(Grafo *grafo, int vertice_inicio);

/*
 * DFS (Busca em Profundidade) - Depth First Search
 * Visita vértices AFUNDO antes de explorar alternativas (usa pilha/recursão)
 * Complexidade: O(V + E)
 * Retorna: EstadoBusca com distâncias calculadas
 */
EstadoBusca grafo_dfs(Grafo *grafo, int vertice_inicio);

/*
 * Encontra vizinho não visitado de um vértice
 * Usada internamente para DFS iterativo
 * Retorna: ID do vizinho ou -1 se nenhum encontrado
 */
int grafo_primeiro_vizinho_nao_visitado(Grafo *grafo, int vertice, EstadoBusca *estado);

/*
 * Imprime informações do grafo para debug
 */
void grafo_imprimir(Grafo *grafo);

/*
 * Imprime resultado da busca (visitados e distâncias)
 */
void grafo_imprimir_busca(EstadoBusca *resultado, int num_vertices);

#endif // GRAFO_H
