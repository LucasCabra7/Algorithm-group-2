#include "Personagem.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Player player_creat(const char *nome, Classe classe){

    Player p;
    strncpy(p.nome, nome, NAME_MAX_LEN - 1);
    p.nome[NAME_MAX_LEN - 1] = '\0';
    p.Classe = classe;
    p.nivel = 1;
    p.xp = 0;
    p.pos_x = 0;
    p.pos_y = 1;

    // Atributos base da Classe:
    switch (classe)
    {
    case SOLDADO:
        p.hp_max = 100;
        p.hp = 100;
        p.ataque = 22; // Aumento de 4 no ataque base como bônus da classe
        p.defesa = 8;
        break;
    case ENGENHEIRO:
        p.hp_max = 85;
        p.hp = 85;
        p.ataque = 12;
        p.defesa = 10;
        break;
    case MEDICO:
        p.hp_max = 110;
        p.hp = 110;
        p.ataque = 10;
        p.defesa = 12;
        break;
    default:
        p.hp_max = 80;
        p.hp = 80;
        p.ataque = 10;
        p.defesa = 10;
    }
    inventory_init(&p.inventario);
    return p;
}

void player_print_stats(const Player *p) {
printf("\n--- STATUS ---\n");
printf("Nome: %s\n", p->nome);
printf("Classe: %s\n", (p->Classe==SOLDADO)?"Soldado":(p->Classe==ENGENHEIRO)?"Engenheiro":"Medico");
printf("Nivel: %d XP: %d\n", p->nivel, p->xp);
printf("HP: %d/%d\n", p->hp, p->hp_max);
printf("Ataque: %d Defesa: %d\n", p->ataque, p->defesa);
}