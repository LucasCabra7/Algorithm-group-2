#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Personagem.h"
#include "Mapa.h"
#include "Inventario.h"
#include "Batalha.h"
#include "Arquivos.h"
#include "GameState.h"
#include <time.h>

void pause_console()
{
    printf("\nPressione Enter para continuar...");
    while (getchar() != '\n')
        ;
}

void mostrar_tutorial() {
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║           BEM-VINDO AO ZOMBIE RAMPAGE!                   ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
    printf("OBJETIVO:\n");
    printf("  Sobreviva em um mundo pos-apocaliptico infestado de zumbis!\n");
    printf("  Explore o mapa, colete itens e derrote os zumbis.\n\n");
    
    printf("CONTROLES:\n");
    printf("  w/a/s/d - Mover para cima/esquerda/baixo/direita\n");
    printf("  i       - Ver inventario\n");
    printf("  p       - Ver status do personagem\n");
    printf("  q       - Salvar jogo\n");
    printf("  e       - Voltar ao menu principal\n\n");
    
    printf("MAPA:\n");
    printf("  P  = Voce (jogador)\n");
    printf("  Z  = Zumbi (combate ao pisar)\n");
    printf("  +  = Medkit (cura HP)\n");
    printf("  !  = Arma (aumenta ataque)\n");
    printf("  ^  = Municao\n");
    printf("  #  = Obstaculo (nao pode atravessar)\n");
    printf("  T  = Arvore (bloqueia passagem)\n");
    printf("  ~  = Agua (nao pode atravessar)\n");
    printf("  B  = Predio (bloqueia passagem)\n");
    printf("  ,  = Grama (pode atravessar)\n");
    printf("  .  = Chao livre (pode atravessar)\n\n");
    
    printf("DICAS:\n");
    printf("  - Colete medkits antes de enfrentar zumbis\n");
    printf("  - Encontre armas para aumentar seu poder de ataque\n");
    printf("  - Ganhe XP derrotando zumbis para subir de nivel\n");
    printf("  - Salve frequentemente seu progresso!\n\n");
    
    pause_console();
}

void menu_opcoes(GameConfig *config) {
    int opcoes_ativas = 1;
    while (opcoes_ativas) {
        printf("\n╔═══════════════════════════════════════════════════════════╗\n");
        printf("║                       OPCOES                              ║\n");
        printf("╚═══════════════════════════════════════════════════════════╝\n\n");
        printf("Volume da Musica: %d%%\n", config->volume_musica);
        printf("Dificuldade: ");
        if (config->dificuldade == 1) printf("Facil\n");
        else if (config->dificuldade == 2) printf("Normal\n");
        else if (config->dificuldade == 3) printf("Dificil\n");
        printf("\n1) Ajustar Volume\n");
        printf("2) Mudar Dificuldade\n");
        printf("3) Voltar\n");
        printf("> ");
        
        int op = 0;
        if (scanf("%d", &op) != 1) {
            while (getchar() != '\n');
            continue;
        }
        
        if (op == 1) {
            printf("Digite o volume (0-100): ");
            int vol = 0;
            if (scanf("%d", &vol) == 1) {
                if (vol >= 0 && vol <= 100) {
                    config->volume_musica = vol;
                    config_save(config, "config.dat");
                    printf("Volume ajustado para %d%%\n", vol);
                } else {
                    printf("Volume invalido! Use valores entre 0 e 100.\n");
                }
            }
            pause_console();
        }
        else if (op == 2) {
            printf("Escolha a dificuldade:\n");
            printf("1) Facil\n2) Normal\n3) Dificil\n> ");
            int dif = 0;
            if (scanf("%d", &dif) == 1) {
                if (dif >= 1 && dif <= 3) {
                    config->dificuldade = dif;
                    config_save(config, "config.dat");
                    printf("Dificuldade alterada!\n");
                } else {
                    printf("Opcao invalida!\n");
                }
            }
            pause_console();
        }
        else if (op == 3) {
            opcoes_ativas = 0;
        }
    }
}

void menu_sobre() {
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                    SOBRE O JOGO                           ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
    printf("ZOMBIE RAMPAGE\n\n");
    printf("Projeto desenvolvido para a disciplina de Algoritmos,\n");
    printf("utilizando a linguagem C e os principios fundamentais da\n");
    printf("Programacao Orientada a Objetos e Algoritmos.\n\n");
    printf("O jogo e um RPG 2D pos-apocaliptico em um mundo devastado\n");
    printf("por um virus que transforma humanos em zumbis. O jogador\n");
    printf("assume o papel de um sobrevivente que precisa explorar,\n");
    printf("lutar e administrar recursos para permanecer vivo.\n\n");
    printf("INTEGRANTES DA EQUIPE:\n");
    printf("  - Bruno Gabriel (bgprs)\n");
    printf("  - Diogo da Silva (dsr)\n");
    printf("  - Gryghor Camonni (gcfc)\n");
    printf("  - Flavia Vitoria (fves)\n");
    printf("  - Lucas Cabral (lsc)\n\n");
    printf("Universidade Federal de Pernambuco - Centro de Informatica\n");
    printf("(c) 2025 - Todos os direitos reservados\n\n");
    pause_console();
}

void menu_reset(GameStatistics *stats, GameConfig *config) {
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                    RESET DO JOGO                          ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
    printf("ATENCAO! Esta acao ira:\n");
    printf("  - Apagar todos os saves de jogo\n");
    printf("  - Resetar todas as estatisticas\n");
    printf("  - Restaurar configuracoes padrao\n\n");
    printf("Tem certeza? (s/n): ");
    
    char confirm = 0;
    scanf(" %c", &confirm);
    
    if (confirm == 's' || confirm == 'S') {
        // Reset estatísticas
        stats_reset(stats);
        stats_save(stats, "stats.dat");
        
        // Reset configurações
        config_init(config);
        config_save(config, "config.dat");
        
        // Remover save game
        remove("savegame.dat");
        
        printf("\nTodos os dados foram resetados com sucesso!\n");
    } else {
        printf("\nOperacao cancelada.\n");
    }
    pause_console();
}

void menu_principal()
{
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                    ZOMBIE RAMPAGE                         ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\n1) Jogar\n");
    printf("2) Carregar Jogo\n");
    printf("3) Opcoes\n");
    printf("4) Estatisticas\n");
    printf("5) Sobre\n");
    printf("6) Tutorial\n");
    printf("7) Reset\n");
    printf("8) Sair\n");
    printf("> ");
};

int escolher_classe()
{
    printf("Escolha a classe:\n1) Soldado 2) Engenheiro 3) Medico\n> ");
    int c = 0;
    if (scanf("%d", &c) != 1)
    {
        while (getchar() != '\n')
            ;
        return -1;
    }
    return c - 1;
};

void criar_personagem_flow(Player *p)
{
    char nome[NAME_MAX_LEN];
    printf("Digite o nome do personagem: ");

    while (getchar() != '\n')
        ;
    fgets(nome, NAME_MAX_LEN, stdin);

    // Remover newline:
    nome[strcspn(nome, "\n")] = '\0';

    int cl = -1;
    while (cl < 0 || cl > 2)
    {
        cl = escolher_classe();
    }
    *p = player_creat(nome, (Classe)cl);
};

void adicionar_item_exemplo(Player *p)
{
    // Itens iniciais balanceados para o novo mapa maior
    // Reduzido de 2 medkits para 1 para aumentar desafio
    // Munição reduzida de 12 para 5 para encorajar exploração
    Item med;
    med.tipo = ITEM_MEDKIT;
    strncpy(med.nome, "Medkit", 31);
    med.nome[31] = '\0';
    med.poder = 25;
    med.quantidade = 1;
    inventory_add(&p->inventario, med);

    Item muni;
    muni.tipo = ITEM_MUNI;
    strncpy(muni.nome, "Municao", 31);
    muni.nome[31] = '\0';
    muni.poder = 1;
    muni.quantidade = 5;
    inventory_add(&p->inventario, muni);
};

// Função auxiliar para coletar itens do mapa e reduzir duplicação de código
void coletar_item_do_mapa(Map *mapa, Player *jogador, Tile current_tile, GameStatistics *stats) {
    Item it;
    int found_item = 0;
    
    if (current_tile == TILE_MEDKIT)
    {
        it.tipo = ITEM_MEDKIT;
        strncpy(it.nome, "Medkit", 31);
        it.nome[31] = '\0';
        it.poder = 25;
        it.quantidade = 1;
        inventory_add(&jogador->inventario, it);
        printf("Voce encontrou um Medkit! (+25 HP quando usado)\n");
        found_item = 1;
    }
    else if (current_tile == TILE_WEAPON)
    {
        it.tipo = ITEM_PISTOLA;
        strncpy(it.nome, "Pistola", 31);
        it.nome[31] = '\0';
        it.poder = 8;
        it.quantidade = 1;
        inventory_add(&jogador->inventario, it);
        printf("Voce encontrou uma Pistola! (Aumenta seu ataque)\n");
        jogador->ataque += 5; // Bonus permanente
        found_item = 1;
    }
    else if (current_tile == TILE_AMMO)
    {
        it.tipo = ITEM_MUNI;
        strncpy(it.nome, "Municao", 31);
        it.nome[31] = '\0';
        it.poder = 1;
        it.quantidade = 5;
        inventory_add(&jogador->inventario, it);
        printf("Voce encontrou Municao! (x5)\n");
        found_item = 1;
    }
    else if (current_tile == TILE_ITEM)
    {
        // Item genérico antigo (mantido para compatibilidade)
        it.tipo = ITEM_MEDKIT;
        strncpy(it.nome, "Medkit", 31);
        it.nome[31] = '\0';
        it.poder = 25; // Consistente com TILE_MEDKIT
        it.quantidade = 1;
        inventory_add(&jogador->inventario, it);
        printf("Voce encontrou um item: Medkit!\n");
        found_item = 1;
    }
    
    if (found_item) {
        stats_registrar_item_coletado(stats);
        // Estatísticas serão salvas ao sair do jogo ou salvar
        mapa->grid[jogador->pos_y][jogador->pos_x] = TILE_GRASS; // Deixa grama no lugar
        pause_console();
    }
}

int main()
{
    Map mapa;
    Player jogador;
    GameConfig config;
    GameStatistics stats;
    int executando = 1;
    time_t inicio_sessao = 0;
    srand((unsigned)time(NULL));

    // Carregar ou inicializar configurações
    if (!config_load(&config, "config.dat")) {
        // Primeira execução ou arquivo corrompido - usar configurações padrão
        config_init(&config);
        config_save(&config, "config.dat");
    }

    // Carregar ou inicializar estatísticas
    if (!stats_load(&stats, "stats.dat")) {
        // Primeira execução ou arquivo corrompido - criar novas estatísticas
        stats_init(&stats);
        stats_save(&stats, "stats.dat");
    }

    while (executando)
    {
        menu_principal();
        int op = 0;
        if (scanf("%d", &op) != 1)
        {
            while (getchar() != '\n')
                ;
            continue;
        }

        if (op == 1) // Jogar (Novo Jogo)
        {
            inicio_sessao = time(NULL);
            criar_personagem_flow(&jogador);
            map_init(&mapa);
            map_place_player(&mapa, &jogador);
            adicionar_item_exemplo(&jogador);
            
            // Mensagem de boas-vindas
            printf("\n╔═══════════════════════════════════════════════════════════╗\n");
            printf("║  Voce acordou em um mundo devastado por um virus...      ║\n");
            printf("║  Zumbis vagam pelas ruas. Sua missao: SOBREVIVER!        ║\n");
            printf("╚═══════════════════════════════════════════════════════════╝\n");
            pause_console();

            // Loop do Jogo:
            int in_game = 1;
            while (in_game)
            {
                map_print(&mapa, &jogador);
                printf("\nAcoes: w/a/s/d mover | i inventario | p status | h ajuda | q salvar | e sair\n> ");

                // Gryghor: Esse código não estava funcionando corretamente no
                // tratamento de entrada do teclado, então substituí por um scanf simples e funcionou

                //  char cmd = 0;
                //   while (getchar() != '\n')
                //    ; // limpa
                //    cmd = getchar();
                // while (getchar() != '\n')
                //    ;

                char cmd = 0;
                scanf(" %c", &cmd);

                int prev_x = jogador.pos_x;
                int prev_y = jogador.pos_y;

                int moved = 0;
                if (cmd == 'w')
                    moved = map_move_player(&mapa, &jogador, 0, -1);
                else if (cmd == 's')
                    moved = map_move_player(&mapa, &jogador, 0, 1);
                else if (cmd == 'a')
                    moved = map_move_player(&mapa, &jogador, -1, 0);
                else if (cmd == 'd')
                    moved = map_move_player(&mapa, &jogador, 1, 0);
                else if (cmd == 'i')
                {
                    inventory_list(&jogador.inventario);
                    pause_console();
                    continue;
                }
                else if (cmd == 'p')
                {
                    player_print_stats(&jogador);
                    pause_console();
                    continue;
                }
                else if (cmd == 'q')
                {
                    char fname[128] = "savegame.dat";
                    if (save_game(&jogador, &mapa, fname))
                    {
                        stats_registrar_salvamento(&stats);
                        stats_save(&stats, "stats.dat");
                        printf("Jogo salvo em %s\n", fname);
                    }
                    else
                        printf("Falha ao salvar\n");
                    pause_console();
                    continue;
                }
                else if (cmd == 'h')
                {
                    mostrar_tutorial();
                    continue;
                }
                else if (cmd == 'e')
                {
                    // Atualizar tempo jogado antes de sair
                    if (inicio_sessao > 0) {
                        time_t fim_sessao = time(NULL);
                        int tempo_sessao = (int)difftime(fim_sessao, inicio_sessao);
                        stats_atualizar_tempo(&stats, tempo_sessao);
                        stats_save(&stats, "stats.dat");
                    }
                    in_game = 0;
                    continue;
                }
                else
                {
                    printf("Comando invalido\n");
                    pause_console();
                    continue;
                }

                if (moved)
                {
                    if (map_check_encounter(&mapa, &jogador))
                    {
                        // Encontra qual inimigo está na posição do jogador
                        Inimigo *inimigo_encontrado = NULL;
                        for (int i = 0; i < mapa.num_inimigos; i++)
                        {
                            if (mapa.inimigos[i].pos_x == jogador.pos_x && mapa.inimigos[i].pos_y == jogador.pos_y)
                            {
                                // Verifica se o inimigo já não foi derrotado
                                if (mapa.inimigos[i].ativo)
                                {
                                    inimigo_encontrado = &mapa.inimigos[i];
                                    break;
                                }
                            }
                        }

                        if (inimigo_encontrado)
                        {
                            // Passa o inimigo encontrado para a função de batalha
                            int res = batalha_turno(&jogador, inimigo_encontrado);

                            if (res == 1) // Jogador Morreu
                            {
                                stats_registrar_morte(&stats);
                                stats_save(&stats, "stats.dat");
                                printf("Voltando ao menu principal...\n");
                                in_game = 0;
                                pause_console();
                                break;
                            }
                            if (res == 0) // Venceu!
                            {
                                stats_registrar_zumbi_derrotado(&stats);
                                // Estatísticas serão salvas ao sair do jogo ou salvar
                                // Remove o 'Z' do mapa após a vitória
                                mapa.grid[jogador.pos_y][jogador.pos_x] = TILE_GRASS;

                                // Chance de dropar um item:
                                if ((rand() % 100) < 40)
                                {
                                    Item it;
                                    it.tipo = ITEM_MUNI;
                                    strncpy(it.nome, "Municao", 31);
                                    inventory_add(&jogador.inventario, it);
                                    printf("O inimigo deixou municao!\n");
                                }
                                
                                // Verificar se todos os inimigos foram derrotados
                                if (map_check_all_enemies_defeated(&mapa)) {
                                    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
                                    printf("║          PARABENS! VOCE VENCEU!                           ║\n");
                                    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
                                    printf("Voce eliminou todos os zumbis do mapa!\n\n");
                                    printf("Deseja continuar jogando? (s/n): ");
                                    char continuar = 0;
                                    scanf(" %c", &continuar);
                                    
                                    if (continuar == 's' || continuar == 'S') {
                                        printf("\nNovos zumbis apareceram no mapa!\n");
                                        map_repopulate_enemies(&mapa);
                                        pause_console();
                                    } else {
                                        printf("\nVoltando ao menu principal...\n");
                                        if (inicio_sessao > 0) {
                                            time_t fim_sessao = time(NULL);
                                            int tempo_sessao = (int)difftime(fim_sessao, inicio_sessao);
                                            stats_atualizar_tempo(&stats, tempo_sessao);
                                            stats_save(&stats, "stats.dat");
                                        }
                                        in_game = 0;
                                        pause_console();
                                        break;
                                    }
                                }
                                
                                pause_console();
                            }
                            if (res == 2) // Fugiu
                            {
                                stats_registrar_fuga(&stats);
                                // Estatísticas serão salvas ao sair do jogo ou salvar
                                printf("Você fugiu do encontro e voltou para sua posição anterior.\n");
                                jogador.pos_x = prev_x; // Restaura a posição X anterior
                                jogador.pos_y = prev_y; // Restaura a posição Y anterior
                                pause_console();
                            }
                        }
                    }
                    else
                    {
                        // Checar e coletar diferentes tipos de itens
                        Tile current_tile = mapa.grid[jogador.pos_y][jogador.pos_x];
                        coletar_item_do_mapa(&mapa, &jogador, current_tile, &stats);
                    }
                }
                else
                {
                    // Melhor feedback sobre por que não pode mover
                    int nx = jogador.pos_x;
                    int ny = jogador.pos_y;
                    
                    if (cmd == 'w') ny--;
                    else if (cmd == 's') ny++;
                    else if (cmd == 'a') nx--;
                    else if (cmd == 'd') nx++;
                    
                    if (nx < 0 || nx >= MAP_W || ny < 0 || ny >= MAP_H) {
                        printf("Voce nao pode sair dos limites do mapa!\n");
                    } else {
                        Tile bloqueio = mapa.grid[ny][nx];
                        printf("Voce nao pode atravessar %s!\n", map_get_tile_name(bloqueio));
                    }
                    pause_console();
                }
            }
        }
        else if (op == 2) // Carregar Jogo
        {
            inicio_sessao = time(NULL);
            // carregar jogo
            if (load_game(&jogador, &mapa, "savegame.dat"))
            {
                printf("Jogo carregado com sucesso!\n");
                pause_console();
                // Entrar no loop do jogo:
                int in_game = 1;
                while (in_game)
                {
                    map_print(&mapa, &jogador);
                    printf("\nAcoes: w/a/s/d mover | i inventario | p status | h ajuda | q salvar | e sair\n> ");

                    int prev_x = jogador.pos_x;
                    int prev_y = jogador.pos_y;

                    char cmd = 0;
                    scanf(" %c", &cmd);

                    int moved = 0;
                    if (cmd == 'w')
                        moved = map_move_player(&mapa, &jogador, 0, -1);
                    else if (cmd == 's')
                        moved = map_move_player(&mapa, &jogador, 0, 1);
                    else if (cmd == 'a')
                        moved = map_move_player(&mapa, &jogador, -1, 0);
                    else if (cmd == 'd')
                        moved = map_move_player(&mapa, &jogador, 1, 0);
                    else if (cmd == 'i')
                    {
                        inventory_list(&jogador.inventario);
                        pause_console();
                        continue;
                    }
                    else if (cmd == 'p')
                    {
                        player_print_stats(&jogador);
                        pause_console();
                        continue;
                    }
                    else if (cmd == 'q')
                    {
                        char fname[128] = "savegame.dat";
                        if (save_game(&jogador, &mapa, fname))
                        {
                            stats_registrar_salvamento(&stats);
                            stats_save(&stats, "stats.dat");
                            printf("Jogo salvo em %s\n", fname);
                        }
                        else
                            printf("Falha ao salvar\n");
                        pause_console();
                        continue;
                    }
                    else if (cmd == 'h')
                    {
                        mostrar_tutorial();
                        continue;
                    }
                    else if (cmd == 'e')
                    {
                        // Atualizar tempo jogado antes de sair
                        if (inicio_sessao > 0) {
                            time_t fim_sessao = time(NULL);
                            int tempo_sessao = (int)difftime(fim_sessao, inicio_sessao);
                            stats_atualizar_tempo(&stats, tempo_sessao);
                            stats_save(&stats, "stats.dat");
                        }
                        in_game = 0;
                        continue;
                    }
                    else
                    {
                        printf("Comando invalido\n");
                        pause_console();
                        continue;
                    }

                    if (moved)
                    {
                        if (map_check_encounter(&mapa, &jogador))
                        {
                            Inimigo *inimigo_encontrado = NULL;
                            for (int i = 0; i < mapa.num_inimigos; i++)
                            {
                                if (mapa.inimigos[i].pos_x == jogador.pos_x && mapa.inimigos[i].pos_y == jogador.pos_y)
                                {
                                    if (mapa.inimigos[i].ativo)
                                    {
                                        inimigo_encontrado = &mapa.inimigos[i];
                                        break;
                                    }
                                }
                            }

                            if (inimigo_encontrado)
                            {
                                int res = batalha_turno(&jogador, inimigo_encontrado);

                                if (res == 1)
                                {
                                    // Jogador Morreu:
                                    stats_registrar_morte(&stats);
                                    stats_save(&stats, "stats.dat");
                                    printf("Voltando ao menu principal...\n");
                                    in_game = 0;
                                    pause_console();
                                    break;
                                }
                                if (res == 0)
                                {
                                    // Jogador Venceu:
                                    stats_registrar_zumbi_derrotado(&stats);
                                    // Estatísticas serão salvas ao sair do jogo ou salvar
                                    mapa.grid[jogador.pos_y][jogador.pos_x] = TILE_GRASS;
                                    int r = rand() % 100;
                                    if (r < 40)
                                    {
                                        Item it;
                                        it.tipo = ITEM_MUNI;
                                        strncpy(it.nome, "Municao", 31);
                                        it.nome[31] = '\0';
                                        it.poder = 1;
                                        it.quantidade = 3;
                                        inventory_add(&jogador.inventario, it);
                                        printf("O inimigo deixou municao!\n");
                                    }
                                    
                                    // Verificar se todos os inimigos foram derrotados
                                    if (map_check_all_enemies_defeated(&mapa)) {
                                        printf("\n╔═══════════════════════════════════════════════════════════╗\n");
                                        printf("║          PARABENS! VOCE VENCEU!                           ║\n");
                                        printf("╚═══════════════════════════════════════════════════════════╝\n\n");
                                        printf("Voce eliminou todos os zumbis do mapa!\n\n");
                                        printf("Deseja continuar jogando? (s/n): ");
                                        char continuar = 0;
                                        scanf(" %c", &continuar);
                                        
                                        if (continuar == 's' || continuar == 'S') {
                                            printf("\nNovos zumbis apareceram no mapa!\n");
                                            map_repopulate_enemies(&mapa);
                                            pause_console();
                                        } else {
                                            printf("\nVoltando ao menu principal...\n");
                                            if (inicio_sessao > 0) {
                                                time_t fim_sessao = time(NULL);
                                                int tempo_sessao = (int)difftime(fim_sessao, inicio_sessao);
                                                stats_atualizar_tempo(&stats, tempo_sessao);
                                                stats_save(&stats, "stats.dat");
                                            }
                                            in_game = 0;
                                            pause_console();
                                            break;
                                        }
                                    }
                                    
                                    pause_console();
                                }
                                if (res == 2)
                                {
                                    // Jogador Fugiu:
                                    stats_registrar_fuga(&stats);
                                    // Estatísticas serão salvas ao sair do jogo ou salvar
                                    printf("Você fugiu do encontro e voltou para sua posição anterior.\n");
                                    jogador.pos_x = prev_x;
                                    jogador.pos_y = prev_y;
                                    pause_console();
                                }
                            }
                        }
                        else
                        {
                            // Checar e coletar diferentes tipos de itens
                            Tile current_tile = mapa.grid[jogador.pos_y][jogador.pos_x];
                            coletar_item_do_mapa(&mapa, &jogador, current_tile, &stats);
                        }
                    }
                    else
                    {
                        // Melhor feedback sobre por que não pode mover
                        int nx = jogador.pos_x;
                        int ny = jogador.pos_y;
                        
                        if (cmd == 'w') ny--;
                        else if (cmd == 's') ny++;
                        else if (cmd == 'a') nx--;
                        else if (cmd == 'd') nx++;
                        
                        if (nx < 0 || nx >= MAP_W || ny < 0 || ny >= MAP_H) {
                            printf("Voce nao pode sair dos limites do mapa!\n");
                        } else {
                            Tile bloqueio = mapa.grid[ny][nx];
                            printf("Voce nao pode atravessar %s!\n", map_get_tile_name(bloqueio));
                        }
                        pause_console();
                    }
                }
            }
            else
            {
                printf("Falha ao carregar savegame.dat\n");
                pause_console();
            }
        }
        else if (op == 3) // Opções
        {
            menu_opcoes(&config);
        }
        else if (op == 4) // Estatísticas
        {
            stats_print(&stats);
            pause_console();
        }
        else if (op == 5) // Sobre
        {
            menu_sobre();
        }
        else if (op == 6) // Tutorial
        {
            mostrar_tutorial();
        }
        else if (op == 7) // Reset
        {
            menu_reset(&stats, &config);
        }
        else if (op == 8) // Sair
        {
            executando = 0;
        }
        else
        {
            printf("Opcao invalida\n");
        }
    }

    printf("Saindo...\n");
    return 0;
}