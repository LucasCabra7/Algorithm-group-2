#include "GameState.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void config_init(GameConfig *cfg) {
    cfg->volume_musica = 50;    // Volume padrão: 50%
    cfg->dificuldade = 2;       // Dificuldade padrão: Normal
}

void config_save(const GameConfig *cfg, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    fwrite(cfg, sizeof(GameConfig), 1, f);
    fclose(f);
}

int config_load(GameConfig *cfg, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    int result = fread(cfg, sizeof(GameConfig), 1, f);
    fclose(f);
    return result == 1;
}

void stats_init(GameStatistics *stats) {
    stats->zumbis_derrotados = 0;
    stats->itens_coletados = 0;
    stats->mortes = 0;
    stats->jogos_salvos = 0;
    stats->fugas = 0;
    stats->tempo_jogo_segundos = 0;
    // Inicializa primeira_partida e ultima_partida com a hora atual.
    // Isso é usado quando não há stats.dat (primeira execução) ou após um reset.
    // Se stats.dat existir, stats_load() sobrescreverá esses valores com os salvos.
    stats->primeira_partida = time(NULL);
    stats->ultima_partida = time(NULL);
}

void stats_save(const GameStatistics *stats, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    fwrite(stats, sizeof(GameStatistics), 1, f);
    fclose(f);
}

int stats_load(GameStatistics *stats, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    int result = fread(stats, sizeof(GameStatistics), 1, f);
    fclose(f);
    return result == 1;
}

void stats_print(const GameStatistics *stats) {
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║              ESTATÍSTICAS DO JOGADOR                      ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
    
    printf("Zumbis Derrotados: %d\n", stats->zumbis_derrotados);
    printf("Itens Coletados: %d\n", stats->itens_coletados);
    printf("Mortes: %d\n", stats->mortes);
    printf("Jogos Salvos: %d\n", stats->jogos_salvos);
    printf("Fugas: %d\n", stats->fugas);
    
    int horas = stats->tempo_jogo_segundos / 3600;
    int minutos = (stats->tempo_jogo_segundos % 3600) / 60;
    int segundos = stats->tempo_jogo_segundos % 60;
    printf("Tempo Total de Jogo: %dh %dm %ds\n", horas, minutos, segundos);
    
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&stats->primeira_partida));
    printf("Primeira Partida: %s\n", buffer);
    
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&stats->ultima_partida));
    printf("Última Partida: %s\n", buffer);
    
    printf("\n");
}

void stats_reset(GameStatistics *stats) {
    stats_init(stats);
}

void stats_registrar_zumbi_derrotado(GameStatistics *stats) {
    stats->zumbis_derrotados++;
    stats->ultima_partida = time(NULL);
}

void stats_registrar_item_coletado(GameStatistics *stats) {
    stats->itens_coletados++;
    stats->ultima_partida = time(NULL);
}

void stats_registrar_morte(GameStatistics *stats) {
    stats->mortes++;
    stats->ultima_partida = time(NULL);
}

void stats_registrar_salvamento(GameStatistics *stats) {
    stats->jogos_salvos++;
    stats->ultima_partida = time(NULL);
}

void stats_registrar_fuga(GameStatistics *stats) {
    stats->fugas++;
    stats->ultima_partida = time(NULL);
}

void stats_atualizar_tempo(GameStatistics *stats, int segundos) {
    stats->tempo_jogo_segundos += segundos;
    stats->ultima_partida = time(NULL);
}
