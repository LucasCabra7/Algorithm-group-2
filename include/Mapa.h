#ifndef MAPA_H
#define MAPA_H

#include <stddef.h>
#include "Personagem.h"
#include "Inimigo.h"

#define MAP_W 10
#define MAP_H 10

typedef enum { TILE_EMPTY = 0, TILE_WALL = 1, TILE_ZOMBIE = 2, TILE_ITEM = 3} Tile;

typedef struct {
    int grid[MAP_H][MAP_W];
    Inimigo inimigos[MAX_INIMIGOS];
    int num_inimigos;              
} Map;

void map_init(Map *m);
void map_print(const Map *m, const Player *p);
int map_move_player(Map *m, Player *p, int dx, int dy);
int map_check_encounter(Map *m, const Player *p); // retorna 1 se encontro zumbi
void map_place_player(Map *m, Player *p);


#endif