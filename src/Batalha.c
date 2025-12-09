#include "Batalha.h"
#include "Inventario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar para desenhar barra de HP estilo Pokemon
void desenhar_barra_hp(int hp_atual, int hp_max, const char* nome, int nivel) {
    int largura_barra = 20;
    int porcentagem = (hp_atual * 100) / hp_max;
    int blocos_cheios = (hp_atual * largura_barra) / hp_max;
    
    printf("╔════════════════════════════════╗\n");
    printf("║ %-15s Lv.%-2d       ║\n", nome, nivel);
    printf("║ HP: [");
    for (int i = 0; i < largura_barra; i++) {
        if (i < blocos_cheios) {
            if (porcentagem > 50) printf("█");
            else if (porcentagem > 20) printf("▓");
            else printf("▒");
        } else {
            printf("░");
        }
    }
    printf("] %d/%d  ║\n", hp_atual, hp_max);
    printf("╚════════════════════════════════╝\n");
}

// Função auxiliar para mostrar menu de ações estilo Pokemon
void mostrar_menu_combate() {
    printf("\n╔════════════════════════════════╗\n");
    printf("║  O que você deseja fazer?      ║\n");
    printf("╠════════════════════════════════╣\n");
    printf("║  1) ⚔ ATACAR                   ║\n");
    printf("║  2) 🎒 ITEM (Usar Medkit)      ║\n");
    printf("║  3) 🏃 FUGIR                   ║\n");
    printf("╚════════════════════════════════╝\n");
    printf("> ");
}

// Função auxiliar para animação de texto (simulada com delay)
void texto_combate(const char* msg) {
    printf("\n┌─────────────────────────────────────────┐\n");
    printf("│ %-40s│\n", msg);
    printf("└─────────────────────────────────────────┘\n");
}

int batalha_turno(Player *jogador, Inimigo *inimigo) {

    int hp_inimigo_atual = inimigo->hp;

    // Apresentação do combate estilo Pokemon
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("║          ⚠  ENCONTRO COM INIMIGO!  ⚠                 ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");
    texto_combate("Um %s selvagem apareceu!");
    printf("  Um %s selvagem apareceu!\n\n", inimigo->nome);

    while (jogador->hp > 0 && hp_inimigo_atual > 0) {
        
        // Mostrar barras de HP estilo Pokemon
        printf("\n═══════════════════════════════════════════════════════\n");
        printf("                    INIMIGO\n");
        desenhar_barra_hp(hp_inimigo_atual, inimigo->hp, inimigo->nome, 1);
        
        printf("\n                    JOGADOR\n");
        desenhar_barra_hp(jogador->hp, jogador->hp_max, jogador->nome, jogador->nivel);
        printf("═══════════════════════════════════════════════════════\n");
        
        // Mostrar menu de ações
        mostrar_menu_combate();
        
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
            
            char msg[100];
            sprintf(msg, "%s usou ATAQUE!", jogador->nome);
            texto_combate(msg);
            sprintf(msg, "Causou %d de dano em %s!", dano_causado, inimigo->nome);
            texto_combate(msg);

            if (hp_inimigo_atual <= 0) {
                printf("\n");
                texto_combate("O inimigo foi derrotado!");
                printf("\n╔═══════════════════════════════════════════════════════╗\n");
                printf("║                    VITÓRIA!                           ║\n");
                printf("╚═══════════════════════════════════════════════════════╝\n");
                jogador->xp += inimigo->xp;
                printf("  %s ganhou %d pontos de XP!\n", jogador->nome, inimigo->xp);
                player_check_level_up(jogador);
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
                
                char msg[100];
                sprintf(msg, "%s usou MEDKIT!", jogador->nome);
                texto_combate(msg);
                sprintf(msg, "Recuperou %d pontos de HP!", cura);
                texto_combate(msg);

                if (it->quantidade <= 0) {
                    inventory_remove_index(&jogador->inventario, idx);
                }
            } else {
                texto_combate("Nenhum medkit disponivel no inventario!");
                continue; 
            }

        } else if (op == 3) { // --- FUGIR ---
            int chance_de_fuga = rand() % 100;
            if (chance_de_fuga < 50) {
                texto_combate("Voce fugiu com sucesso!");
                printf("\n");
                return 2; // Código de Fuga
            } else {
                texto_combate("A fuga falhou!");
            }

        } else {
            texto_combate("Opcao invalida! Voce perdeu seu turno!");
        }

        // Turno do inimigo
        if (hp_inimigo_atual > 0) {
            char msg[100];
            sprintf(msg, "%s preparou um ataque!", inimigo->nome);
            texto_combate(msg);
            
            int dano_recebido = (rand() % inimigo->ataque) + 1 - jogador->defesa;
            if (dano_recebido < 1) {
                dano_recebido = 1;
            }
            jogador->hp -= dano_recebido;
            
            sprintf(msg, "%s causou %d de dano!", inimigo->nome, dano_recebido);
            texto_combate(msg);

            if (jogador->hp <= 0) {
                jogador->hp = 0;
                printf("\n╔═══════════════════════════════════════════════════════╗\n");
                printf("║                    DERROTA...                         ║\n");
                printf("╚═══════════════════════════════════════════════════════╝\n");
                texto_combate("Voce foi derrotado...");
                return 1; // Código de Derrota
            }
        }
    }
    return 2;
}
