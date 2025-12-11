#include "Mapa.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void criar_inimigo(Inimigo *inimigo, const char* nome, int hp, int atk, int def, int xp);

void map_init(Map *mapa) {
    // Inicializa o mapa com grama como base
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            int r = rand() % 100;
            if (r < 50) { // 50% grama
                mapa->grid[y][x] = TILE_GRASS;
            } else if (r < 55) { // 5% árvores
                mapa->grid[y][x] = TILE_TREE;
            } else if (r < 60) { // 5% água
                mapa->grid[y][x] = TILE_WATER;
            } else if (r < 70) { // 10% paredes/rochas
                mapa->grid[y][x] = TILE_WALL;
            } else { // 30% chão vazio
                mapa->grid[y][x] = TILE_EMPTY;
            }
        }
    }

    // Cria algumas construções (prédios abandonados)
    int num_buildings = 3;
    for (int i = 0; i < num_buildings; i++) {
        int bx = rand() % (MAP_W - 4) + 2;
        int by = rand() % (MAP_H - 4) + 2;
        // Cria um prédio 3x3
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (bx + dx < MAP_W && by + dy < MAP_H) {
                    if (dx == 0 && dy == 0) {
                        mapa->grid[by + dy][bx + dx] = TILE_EMPTY; // Centro vazio
                    } else {
                        mapa->grid[by + dy][bx + dx] = TILE_BUILDING;
                    }
                }
            }
        }
    }

    // Garante uma área inicial limpa para o jogador (canto superior esquerdo)
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            mapa->grid[y][x] = TILE_EMPTY;
        }
    }

    // Inicializa e cria os inimigos com atributos variados
    mapa->num_inimigos = 0;
    // Número de inimigos proporcional ao tamanho do mapa (cerca de 2.4% dos tiles)
    int inimigos_a_criar = (MAP_W * MAP_H) / 42; // 500 tiles / 42 ≈ 12 inimigos

    for (int i = 0; i < inimigos_a_criar; i++) {
        if (mapa->num_inimigos >= MAX_INIMIGOS) {
            break;
        }

        // Escolhe uma posição aleatória que esteja vazia ou em grama
        int x, y;
        int tentativas = 0;
        do {
            x = rand() % MAP_W;
            y = rand() % MAP_H;
            tentativas++;
            if (tentativas > 100) break; // Evita loop infinito
        } while (mapa->grid[y][x] != TILE_EMPTY && mapa->grid[y][x] != TILE_GRASS);

        // Evita spawnar muito perto do jogador
        if (x < 4 && y < 4) continue;

        Inimigo *novo_inimigo = &mapa->inimigos[mapa->num_inimigos];

        // Decide aleatoriamente qual tipo de inimigo criar
        int tipo_inimigo = rand() % 3;
        if (tipo_inimigo == 0) {
            criar_inimigo(novo_inimigo, "Zumbi Lento", 40, 8, 2, 10);
        } else if (tipo_inimigo == 1) {
            criar_inimigo(novo_inimigo, "Corredor Agil", 25, 12, 0, 15);
        } else {
            criar_inimigo(novo_inimigo, "Zumbi Robusto", 60, 6, 5, 20);
        }
        
        novo_inimigo->pos_x = x;
        novo_inimigo->pos_y = y;
        
        mapa->grid[y][x] = TILE_ZOMBIE; 
        mapa->num_inimigos++; 
    }

    // Distribui diferentes tipos de itens pelo mapa
    // Medkits
    int medkits = 4;
    for (int i = 0; i < medkits; i++) {
        int x, y;
        int tentativas = 0;
        do {
            x = rand() % MAP_W;
            y = rand() % MAP_H;
            tentativas++;
            if (tentativas > 100) break;
        } while (mapa->grid[y][x] != TILE_EMPTY && mapa->grid[y][x] != TILE_GRASS);

        mapa->grid[y][x] = TILE_MEDKIT;
    }

    // Armas
    int weapons = 2;
    for (int i = 0; i < weapons; i++) {
        int x, y;
        int tentativas = 0;
        do {
            x = rand() % MAP_W;
            y = rand() % MAP_H;
            tentativas++;
            if (tentativas > 100) break;
        } while (mapa->grid[y][x] != TILE_EMPTY && mapa->grid[y][x] != TILE_GRASS);

        mapa->grid[y][x] = TILE_WEAPON;
    }

    // Munição
    int ammo = 6;
    for (int i = 0; i < ammo; i++) {
        int x, y;
        int tentativas = 0;
        do {
            x = rand() % MAP_W;
            y = rand() % MAP_H;
            tentativas++;
            if (tentativas > 100) break;
        } while (mapa->grid[y][x] != TILE_EMPTY && mapa->grid[y][x] != TILE_GRASS);

        mapa->grid[y][x] = TILE_AMMO;
    }
}

void map_print(const Map *m, const Player *p){
    // Conta zumbis vivos e itens disponíveis
    int zumbis_vivos = 0;
    int itens_disponiveis = 0;
    
    for (int i = 0; i < m->num_inimigos; i++) {
        if (m->inimigos[i].ativo) {
            zumbis_vivos++;
        }
    }
    
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            Tile t = m->grid[y][x];
            if (t == TILE_MEDKIT || t == TILE_WEAPON || t == TILE_AMMO || t == TILE_ITEM) {
                itens_disponiveis++;
            }
        }
    }
    
    printf("\n=== ZOMBIE RAMPAGE - OVERWORLD ===\n");
    printf("Posicao: (%d, %d) | HP: %d/%d | Nivel: %d | XP: %d/%d\n", 
           p->pos_x, p->pos_y, p->hp, p->hp_max, p->nivel, p->xp, p->xp_proximo_nivel);
    printf("Zumbis restantes: %d | Itens no mapa: %d\n", zumbis_vivos, itens_disponiveis);
    printf("Legenda: P=Voce Z=Zumbi #=Obstaculo T=Arvore ~=Agua B=Predio\n");
    printf("         +=Medkit !=Arma ^=Municao ,=Grama .=Chao\n");
    printf("--------------------------------------------------\n");
    
    for(int y = 0; y < MAP_H; y++){
        for(int x = 0; x < MAP_W; x++){
            if(p->pos_x == x && p->pos_y == y){
                putchar('P');
                continue;
            }
            switch (m->grid[y][x])
            {
            case TILE_EMPTY:
                putchar('.');
                break;
            case TILE_WALL:
                putchar('#');
                break;
            case TILE_ZOMBIE:
                putchar('Z');
                break;
            case TILE_ITEM:
                putchar('*');
                break;
            case TILE_GRASS:
                putchar(',');
                break;
            case TILE_TREE:
                putchar('T');
                break;
            case TILE_WATER:
                putchar('~');
                break;
            case TILE_BUILDING:
                putchar('B');
                break;
            case TILE_MEDKIT:
                putchar('+');
                break;
            case TILE_WEAPON:
                putchar('!');
                break;
            case TILE_AMMO:
                putchar('^');
                break;
            default:
                putchar('?');
            }
        }
        putchar('\n');
    }
    printf("--------------------------------------------------\n");
};

// Função auxiliar para criar um tipo de inimigo 
void criar_inimigo(Inimigo *inimigo, const char* nome, int hp, int atk, int def, int xp) {
    strncpy(inimigo->nome, nome, 49);
    inimigo->nome[49] = '\0';
    inimigo->hp = hp;
    inimigo->ataque = atk;
    inimigo->defesa = def;
    inimigo->xp = xp;
    inimigo->ativo = 1; // Nasce vivo
}

int map_move_player(Map *m, Player *p, int dx, int dy){
    int nx = p->pos_x + dx;
    int ny = p->pos_y + dy;
    if(nx < 0 || nx >= MAP_W || ny < 0 || ny >= MAP_H) return 0;
    
    // Não pode atravessar paredes, árvores, água ou prédios
    if(m->grid[ny][nx] == TILE_WALL || 
       m->grid[ny][nx] == TILE_TREE || 
       m->grid[ny][nx] == TILE_WATER ||
       m->grid[ny][nx] == TILE_BUILDING) {
        return 0;
    }

    // Movimentos:
    p->pos_x = nx;
    p->pos_y = ny;
    return 1;
};

int map_check_encounter(Map *m, const Player *p){
    if(m->grid[p->pos_y][p->pos_x] == TILE_ZOMBIE) return 1;
    return 0;
};

void map_place_player(Map *m, Player *p){
    // Encontrar uma posição vazia:
    for(int y = 0; y < MAP_H; y++){
        for(int x = 0; x < MAP_W; x++){
            if(m->grid[y][x] == TILE_EMPTY){
                p->pos_x = x;
                p->pos_y = y;
                return;
            }
        }
    }
    // Voltar a trás (fallback):
    p->pos_x = 0; p->pos_y = 0;
}

const char* map_get_tile_name(Tile tile) {
    switch(tile) {
        case TILE_WALL: return "uma parede";
        case TILE_TREE: return "uma arvore";
        case TILE_WATER: return "agua";
        case TILE_BUILDING: return "um predio";
        case TILE_ZOMBIE: return "um zumbi";
        default: return "obstaculo";
    }
}

int map_check_all_enemies_defeated(const Map *m) {
    // Verifica se todos os inimigos foram derrotados
    for (int i = 0; i < m->num_inimigos; i++) {
        if (m->inimigos[i].ativo) {
            return 0; // Ainda há inimigos vivos
        }
    }
    return 1; // Todos os inimigos foram derrotados
}

void map_repopulate_enemies(Map *m) {
    // Remove todos os inimigos antigos do mapa
    for (int i = 0; i < m->num_inimigos; i++) {
        if (m->inimigos[i].pos_x >= 0 && m->inimigos[i].pos_x < MAP_W &&
            m->inimigos[i].pos_y >= 0 && m->inimigos[i].pos_y < MAP_H) {
            if (m->grid[m->inimigos[i].pos_y][m->inimigos[i].pos_x] == TILE_ZOMBIE) {
                m->grid[m->inimigos[i].pos_y][m->inimigos[i].pos_x] = TILE_GRASS;
            }
        }
    }
    
    // Reseta o contador de inimigos
    m->num_inimigos = 0;
    
    // Cria novos inimigos (mesma lógica do map_init)
    int inimigos_a_criar = (MAP_W * MAP_H) / 42; // Cerca de 12 inimigos
    
    for (int i = 0; i < inimigos_a_criar; i++) {
        if (m->num_inimigos >= MAX_INIMIGOS) {
            break;
        }
        
        // Escolhe uma posição aleatória que esteja vazia ou em grama
        int x, y;
        int tentativas = 0;
        do {
            x = rand() % MAP_W;
            y = rand() % MAP_H;
            tentativas++;
            if (tentativas > 100) break; // Evita loop infinito
        } while (m->grid[y][x] != TILE_EMPTY && m->grid[y][x] != TILE_GRASS);
        
        // Evita spawnar muito perto do jogador (área 5x5)
        // Assume que o jogador pode estar em qualquer lugar, então verifica se não está muito próximo
        if (tentativas > 100) continue;
        
        Inimigo *novo_inimigo = &m->inimigos[m->num_inimigos];
        
        // Decide aleatoriamente qual tipo de inimigo criar
        int tipo_inimigo = rand() % 3;
        if (tipo_inimigo == 0) {
            criar_inimigo(novo_inimigo, "Zumbi Lento", 40, 8, 2, 10);
        } else if (tipo_inimigo == 1) {
            criar_inimigo(novo_inimigo, "Corredor Agil", 25, 12, 0, 15);
        } else {
            criar_inimigo(novo_inimigo, "Zumbi Robusto", 60, 6, 5, 20);
        }
        
        novo_inimigo->pos_x = x;
        novo_inimigo->pos_y = y;
        
        m->grid[y][x] = TILE_ZOMBIE;
        m->num_inimigos++;
    }
}

void map_sync_enemies_with_grid(Map *m) {
    // Limpa todos os tiles de zombie da grade
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (m->grid[y][x] == TILE_ZOMBIE) {
                m->grid[y][x] = TILE_GRASS;
            }
        }
    }
    
    // Recoloca os zombies ativos na grade
    for (int i = 0; i < m->num_inimigos; i++) {
        if (m->inimigos[i].ativo) {
            int x = m->inimigos[i].pos_x;
            int y = m->inimigos[i].pos_y;
            
            // Verifica se a posição é válida
            if (x >= 0 && x < MAP_W && y >= 0 && y < MAP_H) {
                // Só coloca o zombie se a posição estiver vazia ou com grama
                if (m->grid[y][x] == TILE_EMPTY || m->grid[y][x] == TILE_GRASS) {
                    m->grid[y][x] = TILE_ZOMBIE;
                }
            }
        }
    }
}