#ifndef BATALHA_H
#define BATALHA_H

#include "Personagem.h"
#include "Mapa.h"

typedef struct{
    int hp;
    int ataque;
    int defesa;
} Enemy;

int batalha_turno(Player *p, Map *m);

#endif