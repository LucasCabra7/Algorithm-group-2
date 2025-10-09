#include "Batalha.h"
#include "Inventario.h"
#include <stdio.h>
#include <stdlib.h>

static Enemy create_zombie_for_tile(){
    Enemy e;
    e.hp = 30;
    e.ataque = 8;
    e.defesa = 2;
    return e;
};

static int player_attack_enemy(Player *p, Enemy *e){
    int dano = p->ataque - e->defesa;
    if(dano < 1){
        dano = 1;
    };
    e->hp -= dano;
    printf("Voce ataca o zumbi e causa %d de dano! (zumbi hp: %d)\n", dano, e->hp);
    return e->hp <= 0;
};

static int enemy_attack_player(Player *p, Enemy *e){
    int dano = e->ataque - p->defesa / 2;
    if(dano < 1){
        dano = 1;
    }
    p->hp -= dano;
    printf("Zumbi ataca e causa %d de dano! (seu hp: %d)\n", dano, p->hp);
    return p->hp <= 0;
};

static void usar_medkit(Player *p){
    int idx = inventory_find_type(&p->inventario, ITEM_MEDKIT);

    if(idx < 0){ printf("Nenhum medkit no inventario!\n"); return; }

    Item *it = &p->inventario.itens[idx];
    int cura = it->poder;
    p->hp += cura;

    if(p->hp > p->hp_max){
        p->hp = p->hp_max;
    }
    it->quantidade -= 1;

    if(it->quantidade <= 0){
        inventory_remove_index(&p->inventario, idx);
    }
    printf("Voce usou um medkit e recuperou %d de HP. HP atual: %d\n", cura, p->hp);
};

int batalha_turno(Player *p, Map *m){
    Enemy e = create_zombie_for_tile();

    printf("\n--- ENCONTRO! Um zumbi apareceu! ---\n");
    while (1)
    {
        printf("\nEscolha acao: 1) Atacar 2) Usar medkit 3) Fugir\n> ");
        int op = 0;

        if(scanf("%d", &op) != 1) { while (getchar()!='\n'); op = 0; }

        if(op == 1){
            if(player_attack_enemy(p, &e)){
                printf("Zumbi derrotado!\n");
                // Remove o zumbi do mapa:
                m->grid[p->pos_y][p->pos_x] = TILE_EMPTY;
                // Recompensa de XP:
                p->xp += 10;
                printf("Ganhou 10 XP!\n");
                return 0; // Vitoria
            }
        }else if(op == 2){
            usar_medkit(p);
        }else if(op == 3){
            // Tentativa de fuga com Prob(X =< 0.5) de sucesso:
            int r = rand() % 100;
            if(r < 50){
                printf("Voce fugiu com sucesso!\n");
                // Recuar um passo (mover para posição anterior)
                // Ainda implementar o recuo simples aqui
                return 2; // Fugiu
            }else{
                printf("Fuga falhou!\n");
            }
        }else {
            printf("Opcao invalida\n");
        }
        // Vez do Inimigo:
        if(enemy_attack_player(p, &e)){
            printf("Voce foi derrotado... Fim de jogo.\n");
            return 1; // Derrotado
        }
    }
}