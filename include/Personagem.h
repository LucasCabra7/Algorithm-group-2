#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdint.h>
#include "inventario.h"

#define NAME_MAX_LEN 32

//Oque tem sendo feito aqui, XP necessário para o próximo nível,uma Função para verificar e executar level up e Calcula XP necessário para próximo nível

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
    int xp_proximo_nivel;
    int pos_x;
    int pos_y;
    Inventory inventario;
} Player;

Player player_creat(const char *nome, Classe classe);
void player_print_stats(const Player *p);
void player_check_level_up(Player *p);
int player_calcular_xp_necessario(int nivel);

#endif
