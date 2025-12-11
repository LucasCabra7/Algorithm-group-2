/*
 * RESUMO SPRINT 1: GRAFOS E BFS/DFS
 * ════════════════════════════════════════════════════════════════════
 * 
 * DATA: 11 de dezembro de 2025
 * DISCIPLINA: Algoritmos
 * EQUIPE: Zombie Rampage
 * 
 * ════════════════════════════════════════════════════════════════════
 * OBJETIVO
 * ════════════════════════════════════════════════════════════════════
 * 
 * Implementar estrutura de grafo e algoritmos de busca (BFS e DFS) para:
 * 1. Representar o mapa como grafo explícito
 * 2. Habilitar IA inteligente para zumbis (BFS para pathfinding)
 * 3. Preparar base para algoritmos avançados (A*, Dijkstra, DFS)
 * 
 * ════════════════════════════════════════════════════════════════════
 * ARQUIVOS CRIADOS
 * ════════════════════════════════════════════════════════════════════
 * 
 * 1. include/Grafo.h (285 linhas)
 *    - Definição de estruturas: Vertice, Grafo, EstadoBusca
 *    - Definição de interfaces públicas
 *    - Documentação educacional de cada função
 * 
 * 2. src/Grafo.c (437 linhas)
 *    - Implementações de:
 *      • grafo_criar()           - O(1)
 *      • grafo_adicionar_aresta() - O(1) amortizado
 *      • grafo_bfs()             - O(V+E)
 *      • grafo_dfs()             - O(V+E)
 *      • Estruturas auxiliares (Fila, Pilha)
 *    - Funções de debug e visualização
 * 
 * ════════════════════════════════════════════════════════════════════
 * ARQUIVOS MODIFICADOS
 * ════════════════════════════════════════════════════════════════════
 * 
 * 1. include/Mapa.h
 *    - Adicionado campo: Grafo grafo_mapa (na struct Map)
 *    - Adicionadas declarations:
 *      • mapa_construir_grafo()
 *      • mapa_atualizar_inimigos_com_bfs()
 * 
 * 2. src/Mapa.c (162 linhas adicionadas)
 *    - Função mapa_construir_grafo():
 *      Converte grid 2D em grafo com lista de adjacência
 *      Complexidade: O(V + E) onde V = tiles, E = conexões
 *    - Função mapa_atualizar_inimigos_com_bfs():
 *      Move zumbis usando BFS para encontrar caminho mais curto
 *      Complexidade: O(V + E) por turno
 *    - Chamada de mapa_construir_grafo() em map_init()
 * 
 * 3. Makefile
 *    - Corrigido syntax (remover comentários)
 *    - Targets funcionais para compilação com gcc
 * 
 * ════════════════════════════════════════════════════════════════════
 * CONCEITOS IMPLEMENTADOS
 * ════════════════════════════════════════════════════════════════════
 * 
 * ▶ ESTRUTURA DE DADOS: LISTA DE ADJACÊNCIA
 *   - Representação: Array de Vertices com array de vizinhos
 *   - Vantagens: O(V+E) espaço, ideal para grafos esparsos
 *   - Desvantagens: O(grau(v)) para verificar aresta
 *   - Aplicação: Mapa é esparso (cada tile = até 4 vizinhos)
 * 
 * ▶ ALGORITMO: BFS (Busca em Largura)
 *   - Estratégia: Processa vértices por camadas
 *   - Estrutura: Fila (FIFO)
 *   - Complexidade: O(V + E)
 *   - Garantia: Encontra caminho mais curto em grafos não-ponderados
 *   - Uso no jogo: Zumbis perseguem jogador (IMPLEMENTADO)
 * 
 * ▶ ALGORITMO: DFS (Busca em Profundidade)
 *   - Estratégia: Explora um caminho completamente antes de alternativa
 *   - Estrutura: Pilha (LIFO)
 *   - Complexidade: O(V + E)
 *   - Uso em grafos: Ciclos, componentes, topologia
 *   - Uso no jogo: Preparação para exploração e análise (NÃO USADO AINDA)
 * 
 * ════════════════════════════════════════════════════════════════════
 * COMO FUNCIONA NO JOGO
 * ════════════════════════════════════════════════════════════════════
 * 
 * 1. INICIALIZAÇÃO (map_init):
 *    a) Gera mapa 25x20 com diferentes tipos de terreno
 *    b) Chama mapa_construir_grafo()
 *    c) Grafo converte:
 *       - Tiles caminháveis em vértices (ID = y*MAP_W + x)
 *       - Adjacências (cima/baixo/esq/dir) em arestas
 * 
 * 2. CADA TURNO (durante gameplay):
 *    a) Player move usando setas
 *    b) Chama mapa_atualizar_inimigos_com_bfs(mapa, jogador)
 *    c) Para cada zumbi:
 *       - Executa BFS desde posição do jogador
 *       - Cada vizinho do zumbi tem distância até jogador
 *       - Zumbi move para vizinho mais próximo do jogador
 *    d) Resultado: Zumbis perseguem inteligentemente
 * 
 * EXEMPLO:
 *   Player está em (5, 5), Zumbi em (10, 10)
 *   → BFS calcula distância de cada tile até (5,5)
 *   → Vizinhos do zumbi: [(10,11):7, (11,10):6, (10,9):8, (9,10):7]
 *   → Zumbi move para (11,10) pois tem menor distância (6)
 *   → Próximo turno, BFS atualizado, zumbi move novamente
 *   → Eventualmente alcança jogador ou encontra obstáculo
 * 
 * ════════════════════════════════════════════════════════════════════
 * ANÁLISE DE COMPLEXIDADE
 * ════════════════════════════════════════════════════════════════════
 * 
 * MAPA_CONSTRUIR_GRAFO (chamada 1x no início):
 *   - Tempo: O(V + E) = O(tiles + conexões) ≈ O(500 + 2000) = O(2500)
 *   - Espaço: O(V + E) = armazenamento do grafo
 *   - Executado 1x durante inicialização
 * 
 * MAPA_ATUALIZAR_INIMIGOS (chamada a cada turno):
 *   - BFS: O(V + E) = O(500 + 2000) por turno
 *   - Loop zumbis: O(Z * K) onde Z=num zumbis, K=vizinhos/zumbi
 *   - Total por turno: O((V+E) + Z*4) ≈ O(2500 + 50) = O(2550)
 *   - Frame time (60 FPS): 16.67ms - SUFICIENTE
 * 
 * ════════════════════════════════════════════════════════════════════
 * PRÓXIMAS ETAPAS
 * ════════════════════════════════════════════════════════════════════
 * 
 * Sprint 2: Estruturas Balanceadas + Heaps
 * - AVL para ordenação de inventário (O(log n))
 * - Heap Min para fila de turnos em batalha
 * - QuickSort e MergeSort para ordenação
 * 
 * Sprint 3: Pathfinding e DP
 * - A* para zumbis elite
 * - Dijkstra para sistema de bússola
 * - Programação Dinâmica para combos de ataque
 * 
 * ════════════════════════════════════════════════════════════════════
 * TÓPICOS ACADÊMICOS COBERTOS
 * ════════════════════════════════════════════════════════════════════
 * 
 * ✓ Grafos (estrutura explícita)
 * ✓ Busca (BFS e DFS)
 * ✓ Análise de Algoritmos (complexidade O(V+E))
 * ✓ Programação Imperativa Estruturada (código legível)
 * ✓ Gestão de Memória (alocação em structs)
 * ✓ Arquivos (preparação para Sprint 5)
 * 
 * Pendentes:
 * ⚠ Árvores (AVL, BST) - Sprint 2
 * ⚠ Heaps - Sprint 2
 * ⚠ Algoritmos de Menor Custo (A*, Dijkstra) - Sprint 3
 * ⚠ DP - Sprint 3
 * ⚠ Backtracking - Sprint 4
 * ⚠ MST - Sprint 4
 * ⚠ Ordenação - Sprint 3
 * 
 * ════════════════════════════════════════════════════════════════════
 * COMO COMPILAR
 * ════════════════════════════════════════════════════════════════════
 * 
 * Windows (PowerShell):
 *   PS> .\compilar.ps1
 * 
 * Manual:
 *   PS> gcc -Wall -Wextra -std=c99 -g -Iinclude -c src/Grafo.c -o obj/Grafo.o
 *   PS> gcc -Wall -Wextra -std=c99 -g -Iinclude -c src/Mapa.c -o obj/Mapa.o
 *   PS> gcc -o rpg.exe obj/*.o
 * 
 * ════════════════════════════════════════════════════════════════════
 */
