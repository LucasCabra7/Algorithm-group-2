#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "Personagem.h"
#include "Mapa.h"

int save_game(const Player *p, const Map *m, const char *filename);
int load_game(Player *p, Map *m, const char *filename);

#endif