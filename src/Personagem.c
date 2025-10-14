#include "Personagem.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Calcula o XP necessário para o próximo nível (fórmula progressiva)
int player_calcular_xp_necessario(int nivel) {
    // Fórmula: XP = 50 + (nivel * 25)
    // Nível 1→2: 75 XP
    // Nível 2→3: 100 XP
    // Nível 3→4: 125 XP, etc.
    return 50 + (nivel * 25);
}

Player player_creat(const char *nome, Classe classe){
    Player p;
    strncpy(p.nome, nome, NAME_MAX_LEN - 1);
    p.nome[NAME_MAX_LEN - 1] = '\0';
    p.Classe = classe;
    p.nivel = 1;
    p.xp = 0;
    p.xp_proximo_nivel = player_calcular_xp_necessario(1);  // NOVO: Inicializa XP necessário
    p.pos_x = 0;
    p.pos_y = 1;

    // Atributos base da Classe:
    switch (classe)
    {
    case SOLDADO:
        p.hp_max = 100;
        p.hp = 100;
        p.ataque = 18;
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
    printf("Nivel: %d (XP: %d/%d)\n", p->nivel, p->xp, p->xp_proximo_nivel);  // MODIFICADO: Mostra barra de XP
    printf("HP: %d/%d\n", p->hp, p->hp_max);
    printf("Ataque: %d Defesa: %d\n", p->ataque, p->defesa);
}

// NOVA FUNÇÃO: Verifica se o jogador tem XP suficiente e sobe de nível
void player_check_level_up(Player *p) {
    // Permite múltiplos level ups de uma vez (caso ganhe muito XP)
    while (p->xp >= p->xp_proximo_nivel) {
        // Subtrai o XP usado
        p->xp -= p->xp_proximo_nivel;
        
        // Aumenta o nível
        p->nivel++;
        
        // Calcula o XP necessário para o próximo nível
        p->xp_proximo_nivel = player_calcular_xp_necessario(p->nivel);
        
        // Bônus de atributos por nível (variáveis por classe)
        int bonus_hp = 0;
        int bonus_ataque = 0;
        int bonus_defesa = 0;
        
        switch (p->Classe) {
            case SOLDADO:
                bonus_hp = 15;
                bonus_ataque = 3;
                bonus_defesa = 2;
                break;
            case ENGENHEIRO:
                bonus_hp = 12;
                bonus_ataque = 2;
                bonus_defesa = 3;
                break;
            case MEDICO:
                bonus_hp = 18;
                bonus_ataque = 1;
                bonus_defesa = 3;
                break;
        }
        
        // Aplica os bônus
        p->hp_max += bonus_hp;
        p->ataque += bonus_ataque;
        p->defesa += bonus_defesa;
        
        // Restaura HP ao subir de nível (bônus extra!)
        p->hp = p->hp_max;
        
        // Mensagem de level up
        printf("\n========================================\n");
        printf("    LEVEL UP! Voce subiu para o nivel %d!\n", p->nivel);
        printf("========================================\n");
        printf("  HP Max: +%d (Total: %d)\n", bonus_hp, p->hp_max);
        printf("  Ataque: +%d (Total: %d)\n", bonus_ataque, p->ataque);
        printf("  Defesa: +%d (Total: %d)\n", bonus_defesa, p->defesa);
        printf("  HP restaurado para o maximo!\n");
        printf("  Proximo nivel em: %d XP\n", p->xp_proximo_nivel);
        printf("========================================\n\n");
    }
}
