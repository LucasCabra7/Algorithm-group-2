#include "Batalha.h"
#include "Inventario.h"
#include <stdio.h>
#include <stdlib.h>

int batalha_turno(Player *jogador, Inimigo *inimigo) {

    int hp_inimigo_atual = inimigo->hp;

    printf("\n--- ENCONTRO! Um %s apareceu! ---\n", inimigo->nome);

    while (jogador->hp > 0 && hp_inimigo_atual > 0) {
        
        printf("\nSua Vida: %d HP | Vida do %s: %d HP\n", jogador->hp, inimigo->nome, hp_inimigo_atual);
        printf("Escolha uma acao: 1) Atacar 2) Usar Medkit 3) Fugir\n> ");
        
        int op = 0;
        if (scanf("%d", &op) != 1) {
            while (getchar() != '\n');
            op = 0;
        }

        if (op == 1) { // --- ATACAR ---
            int dano_causado = (rand() % jogador->ataque) + 2 - inimigo->defesa;
            if (dano_causado < 1) {
                dano_causado = 1;
            }
            hp_inimigo_atual -= dano_causado;
            printf("Voce ataca o %s e causa %d de dano!\n", inimigo->nome, dano_causado);

            if (hp_inimigo_atual <= 0) {
                printf("\n%s derrotado!\n", inimigo->nome);
                jogador->xp += inimigo->xp;
                printf("Voce ganhou %d XP!\n", inimigo->xp);
                inimigo->ativo = 0;
                return 0; // Código de Vitória
            }

        } else if (op == 2) { // --- USAR MEDKIT ---
            int idx = inventory_find_type(&jogador->inventario, ITEM_MEDKIT);
            if (idx >= 0) {
                Item *it = &jogador->inventario.itens[idx];
                int cura = it->poder;
                jogador->hp += cura;

                if (jogador->hp > jogador->hp_max) {
                    jogador->hp = jogador->hp_max;
                }
                it->quantidade -= 1;
                printf("Voce usou um medkit e recuperou %d de HP. HP atual: %d\n", cura, jogador->hp);

                if (it->quantidade <= 0) {
                    inventory_remove_index(&jogador->inventario, idx);
                }
            } else {
                printf("Nenhum medkit no inventario!\n");
                continue; 
            }

        } else if (op == 3) { // --- FUGIR ---
            int chance_de_fuga = rand() % 100;
            if (chance_de_fuga < 50) {
                printf("Voce fugiu com sucesso!\n");
                return 2; // Código de Fuga
            } else {
                printf("A fuga falhou! O inimigo ataca!\n");
            }

        } else {
            printf("Opcao invalida. Voce perdeu seu turno!\n");
        }

        if (hp_inimigo_atual > 0) {
            printf("O %s ataca!\n", inimigo->nome);
            int dano_recebido = (rand() % inimigo->ataque) + 1 - jogador->defesa;
            if (dano_recebido < 1) {
                dano_recebido = 1;
            }
            jogador->hp -= dano_recebido;
            printf("Voce recebeu %d de dano!\n", dano_recebido);

            if (jogador->hp <= 0) {
                jogador->hp = 0;
                printf("\nVoce foi derrotado... Fim de jogo.\n");
                return 1; // Código de Derrota
            }
        }
    }
    return 2;
}