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

void menu_principal()
{
    printf("\n┹┄┄┄┄┄┲⟮۝⟯┹┄┄┄┄┄┲\n");
    printf("\n ZOMBIE RAMPAGE \n");
    printf("\n┹┄┄┄┄┄┲⟮۝⟯┹┄┄┄┄┄┲\n");
    printf("1) Novo Jogo\n2) Carregar Jogo\n3) Sair\n> ");
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
    Item med;
    med.tipo = ITEM_MEDKIT;
    strncpy(med.nome, "Medkit", 31);
    med.nome[31] = '\0';
    med.poder = 25;
    med.quantidade = 2;
    inventory_add(&p->inventario, med);

    Item pist;
    pist.tipo = ITEM_PISTOLA;
    strncpy(pist.nome, "Pistola", 31);
    pist.nome[31] = '\0';
    pist.poder = 6;
    pist.quantidade = 1;
    inventory_add(&p->inventario, pist);

    Item muni;
    muni.tipo = ITEM_MUNI;
    strncpy(muni.nome, "Municao", 31);
    muni.nome[31] = '\0';
    muni.poder = 1;
    muni.quantidade = 12;
    inventory_add(&p->inventario, muni);
};

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

            // Loop do Jogo:
            int in_game = 1;
            while (in_game)
            {
                map_print(&mapa, &jogador);
                printf("\nAcoes: w/a/s/d mover | i inventario | p status | q salvar | e sair para menu\n> ");
                char cmd = 0;
                while (getchar() != '\n')
                    ; // limpa
                cmd = getchar();
                while (getchar() != '\n')
                    ;

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
                        int res = batalha_turno(&jogador, &mapa);
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
                            // Venceu!
                            // Chance de dropar um item:
                            int r = rand() % 100;
                            if (r < 40)
                            {
                                Item it;
                                it.tipo = ITEM_MUNI;
                                strncpy(it.nome, "Municao", 31);
                                inventory_add(&jogador.inventario, it);
                                printf("O zumbi deixou municao!\n");
                            }
                            pause_console();
                        }
                        if (res == 2)
                        {
                            // Fugiu
                            printf("Você fugiu do encontro.\n");
                            pause_console();
                        }
                    }
                    else
                    {
                        // Checar item
                        if (mapa.grid[jogador.pos_y][jogador.pos_x] == TILE_ITEM)
                        {
                            Item it;
                            it.tipo = ITEM_MEDKIT;
                            strncpy(it.nome, "Medkit", 31);
                            it.nome[31] = '\0';
                            it.poder = 20;
                            it.quantidade = 1;
                            inventory_add(&jogador.inventario, it);
                            printf("Voce encontrou um item: Medkit!\n");
                            mapa.grid[jogador.pos_y][jogador.pos_x] = TILE_EMPTY;
                            pause_console();
                        }
                    }
                }
                else
                {
                    printf("Nao foi possivel mover para essa direcao.\n");
                    pause_console();
                }
            }
        }
        else if (op == 2)
        {
            // carregar jogo
            map_init(&mapa); // inicializa por segurança
            if (load_game(&jogador, &mapa, "savegame,dat"))
            {
                printf("Jogo carregado com sucesso!\n");
                // Entrar no loop do jogo:
                int in_game = 1;
                while (in_game)
                {
                    map_print(&mapa, &jogador);
                    printf("\nAcoes: w/a/s/d mover | i inventario | p status | q salvar | e sair para menu\n> ");
                    char cmd = 0;
                    while (getchar() != '\n')
                        ; // limpa
                    cmd = getchar();
                    while (getchar() != '\n')
                        ;

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
                            int res = batalha_turno(&jogador, &mapa);

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
                                    printf("O zumbi deixou municao!\n");
                                }
                                pause_console();
                            }
                            if (res == 2)
                            {
                                // Jogador Fugiu:
                                printf("Você fugiu do encontro.\n");
                                pause_console();
                            }
                        }
                        else
                        {
                            if (mapa.grid[jogador.pos_y][jogador.pos_x] == TILE_ITEM)
                            {
                                Item it;
                                it.tipo = ITEM_MEDKIT;
                                strncpy(it.nome, "Medkit", 31);
                                it.nome[31] = '\0';
                                it.poder = 20;
                                it.quantidade = 1;
                                inventory_add(&jogador.inventario, it);
                                printf("Voce encontrou um item: Medkit!\n");
                                mapa.grid[jogador.pos_y][jogador.pos_x] = TILE_EMPTY;
                                pause_console();
                            }
                        }
                    }
                    else
                    {
                        printf("Nao foi possivel mover para essa direcao.\n");
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