#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Personagem.h"
#include "Mapa.h"
#include "Inventario.h"
#include "Batalha.h"
#include "Arquivos.h"
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

void menu_principal()
{
    printf("\n┹┄┄┄┄┄┲⟮۝⟯┹┄┄┄┄┄┲\n");
    printf("\n ZOMBIE RAMPAGE \n");
    printf("\n┹┄┄┄┄┄┲⟮۝⟯┹┄┄┄┄┄┲\n");
    printf("1) Novo Jogo\n2) Carregar Jogo\n3) Tutorial\n4) Sair\n> ");
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
void coletar_item_do_mapa(Map *mapa, Player *jogador, Tile current_tile) {
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
        mapa->grid[jogador->pos_y][jogador->pos_x] = TILE_GRASS; // Deixa grama no lugar
        pause_console();
    }
}

int main()
{
    Map mapa;
    Player jogador;
    int executando = 1;
    srand((unsigned)time(NULL));

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

        if (op == 1)
        {
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
                        printf("Jogo salvo em %s\n", fname);
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
                                printf("Voltando ao menu principal...\n");
                                in_game = 0;
                                pause_console();
                                break;
                            }
                            if (res == 0) // Venceu!
                            {
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
                                pause_console();
                            }
                            if (res == 2) // Fugiu
                            {
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
                        coletar_item_do_mapa(&mapa, &jogador, current_tile);
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
        else if (op == 2)
        {
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
                            printf("Jogo salvo em %s\n", fname);
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
                                    printf("Voltando ao menu principal...\n");
                                    in_game = 0;
                                    pause_console();
                                    break;
                                }
                                if (res == 0)
                                {
                                    // Jogador Venceu:
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
                                    pause_console();
                                }
                                if (res == 2)
                                {
                                    // Jogador Fugiu:
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
                            coletar_item_do_mapa(&mapa, &jogador, current_tile);
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
        else if (op == 3)
        {
            mostrar_tutorial();
        }
        else if (op == 4)
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