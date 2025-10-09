#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdint.h>
#include "inventario.h"

#define NAME_MAX_LEN 32

typedef enum {SOLDADO = 0, ENGENHEIRO = 1, MEDICO = 2} Classe;

typedef struct 
{
    char nome[NAME_MAX_LEN];
    Classe Classe;
    int nivel;
    int hp_max;
    int hp;
    int ataque;
    int defesa;
    int xp;
    int pos_x;
    int pos_y;
    Inventory inventario;
} Player;

Player player_creat(const char *nome, Classe classe);
void player_print_stats(const Player *p);


#endif