#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <time.h>

// Estados do jogo
typedef enum {
    STATE_MENU_PRINCIPAL,
    STATE_PLAYING,
    STATE_OPTIONS,
    STATE_STATISTICS,
    STATE_ABOUT,
    STATE_EXIT
} GameState;

// Configurações do jogo
typedef struct {
    int volume_musica;      // 0-100
    int dificuldade;        // 1=Fácil, 2=Normal, 3=Difícil
} GameConfig;

// Estatísticas do jogador
typedef struct {
    int zumbis_derrotados;
    int itens_coletados;
    int mortes;
    int jogos_salvos;
    int fugas;
    int tempo_jogo_segundos;
    time_t primeira_partida;
    time_t ultima_partida;
} GameStatistics;

// Funções de gerenciamento de configurações
void config_init(GameConfig *cfg);
void config_save(const GameConfig *cfg, const char *filename);
int config_load(GameConfig *cfg, const char *filename);

// Funções de gerenciamento de estatísticas
void stats_init(GameStatistics *stats);
void stats_save(const GameStatistics *stats, const char *filename);
int stats_load(GameStatistics *stats, const char *filename);
void stats_print(const GameStatistics *stats);
void stats_reset(GameStatistics *stats);

// Registrar eventos
void stats_registrar_zumbi_derrotado(GameStatistics *stats);
void stats_registrar_item_coletado(GameStatistics *stats);
void stats_registrar_morte(GameStatistics *stats);
void stats_registrar_salvamento(GameStatistics *stats);
void stats_registrar_fuga(GameStatistics *stats);
void stats_atualizar_tempo(GameStatistics *stats, int segundos);

#endif
