#ifndef MAPA_H
#define MAPA_H

#include <stddef.h>
#include "Personagem.h"
#include "Inimigo.h"

#define MAP_W 25
#define MAP_H 20

typedef enum { 
    TILE_EMPTY = 0,     // Chão vazio (.)
    TILE_WALL = 1,      // Parede/obstáculo (#)
    TILE_ZOMBIE = 2,    // Zumbi (Z)
    TILE_ITEM = 3,      // Item coletável (*)
    TILE_GRASS = 4,     // Grama (,)
    TILE_TREE = 5,      // Árvore (T)
    TILE_WATER = 6,     // Água (~)
    TILE_BUILDING = 7,  // Construção (B)
    TILE_MEDKIT = 8,    // Medkit específico (+)
    TILE_WEAPON = 9,    // Arma específica (!)
    TILE_AMMO = 10      // Munição específica (^)
} Tile;

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
const char* map_get_tile_name(Tile tile); // Retorna o nome do tile
int map_check_all_enemies_defeated(const Map *m); // retorna 1 se todos os inimigos foram derrotados
void map_repopulate_enemies(Map *m); // repopula o mapa com novos inimigos
void map_sync_enemies_with_grid(Map *m); // sincroniza a grade com o array de inimigos


#endif