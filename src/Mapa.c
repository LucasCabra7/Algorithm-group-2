#include "Mapa.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void criar_inimigo(Inimigo *inimigo, const char* nome, int hp, int atk, int def, int xp);

void map_init(Map *mapa) {
    // Limpa o mapa e gera paredes aleatórias
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            int r = rand() % 100;
            if (r < 15) { // 15% de chance de ser uma parede
                mapa->grid[y][x] = TILE_WALL;
            } else {
                mapa->grid[y][x] = TILE_EMPTY;
            }
        }
    }

    //  Inicializa e cria os inimigos com atributos variados
    mapa->num_inimigos = 0;
    int inimigos_a_criar = 7; 

    for (int i = 0; i < inimigos_a_criar; i++) {
        // Para se não houver mais espaço no  array de inimigos
        if (mapa->num_inimigos >= MAX_INIMIGOS) {
            break;
        }

        // Escolhe uma posição aleatória que esteja vazia (TILE_EMPTY)
        int x, y;
        do {
            x = rand() % MAP_W;
            y = rand() % MAP_H;
        } while (mapa->grid[y][x] != TILE_EMPTY);

        // Pega um espaço na lista de inimigos
        Inimigo *novo_inimigo = &mapa->inimigos[mapa->num_inimigos];

        // Decide aleatoriamente qual tipo de inimigo criar
        int tipo_inimigo = rand() % 3;
        if (tipo_inimigo == 0) {
            criar_inimigo(novo_inimigo, "Zumbi Lento", 40, 8, 2, 10);
        } else if (tipo_inimigo == 1) {
            criar_inimigo(novo_inimigo, "Corredor Agil", 25, 12, 0, 15);
        } else {
            criar_inimigo(novo_inimigo, "Zumbi Robusto", 60, 6, 5, 20);
        }
        
        // Guarda a posição do inimigo na sua própria struct
        novo_inimigo->pos_x = x;
        novo_inimigo->pos_y = y;
        
        // Coloca um 'Z' no mapa para representar o inimigo visualmente
        mapa->grid[y][x] = TILE_ZOMBIE; 
        mapa->num_inimigos++; 
    }

 int itens_a_criar = 5;
    for (int i = 0; i < itens_a_criar; i++) {
        int x, y;
        do {
            x = rand() % MAP_W;
            y = rand() % MAP_H;
        } while (mapa->grid[y][x] != TILE_EMPTY);

        mapa->grid[y][x] = TILE_ITEM;
    }
}

void map_print(const Map *m, const Player *p){
    printf("\nMapa (P = jogador, Z = zumbi, # = obstaculo, * = item)\n");
    for(int y = 0; y < MAP_H; y++){
        for(int x = 0; x < MAP_W; x++){
            if(p->pos_x == x && p->pos_y == y){
                putchar('P');
                continue;
            }
            switch (m->grid[y][x])
            {
            case TILE_EMPTY:
                putchar('.');
                break;
            case TILE_WALL:
                putchar('#');
                break;
            case TILE_ZOMBIE:
                putchar('Z');
                break;
            case TILE_ITEM:
                putchar('*');
                break;
            default:
                putchar('?');
            }
        }
        putchar('\n');
    }
};

// Função auxiliar para criar um tipo de inimigo 
void criar_inimigo(Inimigo *inimigo, const char* nome, int hp, int atk, int def, int xp) {
    strncpy(inimigo->nome, nome, 49);
    inimigo->nome[49] = '\0';
    inimigo->hp = hp;
    inimigo->ataque = atk;
    inimigo->defesa = def;
    inimigo->xp = xp;
    inimigo->ativo = 1; // Nasce vivo
}

int map_move_player(Map *m, Player *p, int dx, int dy){
    int nx = p->pos_x + dx;
    int ny = p->pos_y + dy;
    if(nx < 0 || nx >= MAP_W || ny < 0 || ny >= MAP_H) return 0;
    if(m->grid[ny][nx] == TILE_WALL) return 0;

    // Movimentos:
    p->pos_x = nx;
    p->pos_y = ny;
    return 1;
};

int map_check_encounter(Map *m, const Player *p){
    if(m->grid[p->pos_y][p->pos_x] == TILE_ZOMBIE) return 1;
    return 0;
};

void map_place_player(Map *m, Player *p){
    // Encontrar uma posição vazia:
    for(int y = 0; y < MAP_H; y++){
        for(int x = 0; x < MAP_W; x++){
            if(m->grid[y][x] == TILE_EMPTY){
                p->pos_x = x;
                p->pos_y = y;
                return;
            }
        }
    }
    // Voltar a trás (fallback):
    p->pos_x = 0; p->pos_y = 0;
}