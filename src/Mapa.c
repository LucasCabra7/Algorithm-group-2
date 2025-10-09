#include "Mapa.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void map_init(Map *m){
    srand((unsigned)time(NULL));
    for(int y = 0; y < MAP_H; y++){
        for(int x = 0; x < MAP_W; x++){
            int r = rand() % 100;
            if(r < 10){
                m->grid[y][x] = TILE_WALL;
            }else if(r < 20){
                m->grid[y][x] = TILE_ZOMBIE;
            }else if(r < 25){
                m->grid[y][x] = TILE_ITEM;
            }else{
                m->grid[y][x] = TILE_EMPTY;
            }
        }
    }
};

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