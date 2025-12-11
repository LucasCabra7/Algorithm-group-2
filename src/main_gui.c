#include "raylib.h"
#include "Mapa.h"
#include "Personagem.h"
#include "Inventario.h"
#include "GameState.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

// --- CONFIGURAÇÕES VISUAIS ---
#define TILE_SIZE 48        // Tamanho de cada quadrado (em pixels). 48x48 é bom para sprites.
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// --- CORES ESTILO MINIDAYZ (Paleta Desbotada) ---
#define COLOR_GRASS  CLITERAL(Color){ 50, 60, 50, 255 }  // Verde escuro desbotado
#define COLOR_WALL   CLITERAL(Color){ 30, 30, 30, 255 }  // Cinza quase preto
#define COLOR_PLAYER CLITERAL(Color){ 0, 121, 241, 255 } // Azul 
#define COLOR_ZOMBIE CLITERAL(Color){ 230, 41, 55, 255 } // Vermelho sangue
#define COLOR_ITEM   CLITERAL(Color){ 253, 249, 0, 255 } // Amarelo Ouro

// Estados do Jogo
typedef enum { 
    ESTADO_MENU_PRINCIPAL,
    ESTADO_CRIANDO_PERSONAGEM,
    ESTADO_EXPLORANDO, 
    ESTADO_BATALHA,
    ESTADO_INVENTARIO,       // NEW: Inventory screen
    ESTADO_PAUSA,            // NEW: Pause menu
    ESTADO_GAME_OVER,        // NEW: Game over screen
    ESTADO_VITORIA,          // NEW: Victory screen
    ESTADO_OPCOES,
    ESTADO_ESTATISTICAS,
    ESTADO_SOBRE,
    ESTADO_TUTORIAL
} EstadoJogo;

// Estados do menu de combate
typedef enum {
    COMBATE_MENU_PRINCIPAL,  // Escolher ação
    COMBATE_ATACANDO,        // Animação de ataque
    COMBATE_USANDO_ITEM,     // Usando item
    COMBATE_TURNO_INIMIGO,   // Inimigo atacando
    COMBATE_VITORIA,         // Jogador venceu
    COMBATE_DERROTA          // Jogador perdeu
} EstadoCombate;

int main(void) {
    // 1. Inicialização da Janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Zombie Rampage - Overworld");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL); // Disable ESC as exit key - we handle it manually

    // 2. Carregar configurações e estatísticas
    GameConfig config;
    GameStatistics stats;
    time_t inicio_sessao = 0;
    
    if (!config_load(&config, "config.dat")) {
        config_init(&config);
        config_save(&config, "config.dat");
    }
    
    if (!stats_load(&stats, "stats.dat")) {
        stats_init(&stats);
        stats_save(&stats, "stats.dat");
    }

    // 3. Variáveis do Jogo
    Map mapa;
    Player jogador;
    Inventory inventario;
    int jogoInicializado = 0;

    // 4. Estado inicial
    EstadoJogo estado = ESTADO_MENU_PRINCIPAL;
    EstadoCombate estadoCombate = COMBATE_MENU_PRINCIPAL;
    int opcaoMenu = 0;
    int opcaoOpcoes = 0;
    int opcaoCombate = 0; // Opção selecionada no menu de combate
    Inimigo *inimigoAtual = NULL; // Inimigo sendo enfrentado
    int hp_inimigo_atual = 0;
    float tempoCombate = 0.0f; // Timer para animações de combate
    char mensagemCombate[256] = ""; // Mensagem atual do combate
    
    // 5. Configuração da CÂMARA
    Camera2D camera = { 0 };
    camera.offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // 6. Carregar Texturas/Sprites
    // Battle images - new combat sprites
    Texture2D soldadoBatalha = LoadTexture("src/assets/Soldado_Batalha.png");
    Texture2D zumbiBatalha = LoadTexture("src/assets/Zumbi_Batalha.png");
    
    // Original bust images (keeping for reference, but using battle images in combat)
    Texture2D soldadBust = LoadTexture("src/assets/Soldad_Bust.png");
    Texture2D medicBust = LoadTexture("src/assets/Medic_Bust.png");
    Texture2D engenheironus = LoadTexture("src/assets/Engenheir_Bust.png");
    Texture2D zombieBust = LoadTexture("src/assets/zumbi_vest_bust.png");
    
    // Validar se as texturas de batalha carregaram
    if (soldadoBatalha.id == 0) TraceLog(LOG_WARNING, "Failed to load Soldado_Batalha.png");
    if (zumbiBatalha.id == 0) TraceLog(LOG_WARNING, "Failed to load Zumbi_Batalha.png");
    if (soldadBust.id == 0) TraceLog(LOG_WARNING, "Failed to load Soldad_Bust.png");
    if (medicBust.id == 0) TraceLog(LOG_WARNING, "Failed to load Medic_Bust.png");
    if (engenheironus.id == 0) TraceLog(LOG_WARNING, "Failed to load Engenheir_Bust.png");
    if (zombieBust.id == 0) TraceLog(LOG_WARNING, "Failed to load zumbi_vest_bust.png");
    
    // Sprite sheets para overworld
    Texture2D soldadSheet = LoadTexture("src/assets/Soldad_Sprite_Sheet.png");
    Texture2D medicSheet = LoadTexture("src/assets/Medic_Sprite_Sheet.png");
    Texture2D engenheirSheet = LoadTexture("src/assets/Engenheir_Sprite_Sheet.png");
    
    // Itens colecionáveis
    Texture2D healthArmor = LoadTexture("src/assets/recursos_coletaveis/health-armor 32px.png");
    Texture2D ammoPistol = LoadTexture("src/assets/recursos_coletaveis/ammo-pistol 32px.png");
    
    // Variáveis de animação de sprites
    int frameAtual = 0;
    float tempoFrame = 0.0f;
    float velocidadeFrame = 0.15f; // Segundos por frame

    // --- GAME LOOP ---
    while (!WindowShouldClose()) {
        
        // --- ATUALIZAÇÃO ---
        
        if (estado == ESTADO_MENU_PRINCIPAL) {
            // Navegação no menu
            if (IsKeyPressed(KEY_DOWN)) opcaoMenu = (opcaoMenu + 1) % 8;
            if (IsKeyPressed(KEY_UP)) opcaoMenu = (opcaoMenu - 1 + 8) % 8;
            
            if (IsKeyPressed(KEY_ENTER)) {
                switch (opcaoMenu) {
                    case 0: // Jogar
                        inicio_sessao = time(NULL);
                        map_init(&mapa);
                        jogador = player_creat("Sobrevivente", SOLDADO);
                        jogador.pos_x = MAP_W / 2;
                        jogador.pos_y = MAP_H / 2;
                        mapa.grid[jogador.pos_y][jogador.pos_x] = TILE_EMPTY;
                        camera.target = (Vector2){ jogador.pos_x * TILE_SIZE, jogador.pos_y * TILE_SIZE };
                        jogoInicializado = 1;
                        estado = ESTADO_EXPLORANDO;
                        break;
                    case 1: // Carregar Jogo
                        // TODO: Implementar carregamento
                        break;
                    case 2: // Opções
                        estado = ESTADO_OPCOES;
                        opcaoOpcoes = 0;
                        break;
                    case 3: // Estatísticas
                        estado = ESTADO_ESTATISTICAS;
                        break;
                    case 4: // Sobre
                        estado = ESTADO_SOBRE;
                        break;
                    case 5: // Tutorial
                        estado = ESTADO_TUTORIAL;
                        break;
                    case 6: // Reset
                        stats_reset(&stats);
                        stats_save(&stats, "stats.dat");
                        config_init(&config);
                        config_save(&config, "config.dat");
                        break;
                    case 7: // Sair
                        CloseWindow();
                        return 0;
                }
            }
        }
        else if (estado == ESTADO_OPCOES) {
            if (IsKeyPressed(KEY_DOWN)) opcaoOpcoes = (opcaoOpcoes + 1) % 3;
            if (IsKeyPressed(KEY_UP)) opcaoOpcoes = (opcaoOpcoes - 1 + 3) % 3;
            
            if (IsKeyPressed(KEY_ENTER)) {
                if (opcaoOpcoes == 0) {
                    // Ajustar volume
                    config.volume_musica = (config.volume_musica + 10) % 110;
                    config_save(&config, "config.dat");
                } else if (opcaoOpcoes == 1) {
                    // Mudar dificuldade
                    config.dificuldade = (config.dificuldade % 3) + 1;
                    config_save(&config, "config.dat");
                } else if (opcaoOpcoes == 2) {
                    // Voltar
                    estado = ESTADO_MENU_PRINCIPAL;
                }
            }
            
            if (IsKeyPressed(KEY_ESCAPE)) {
                estado = ESTADO_MENU_PRINCIPAL;
            }
        }
        else if (estado == ESTADO_ESTATISTICAS || estado == ESTADO_SOBRE || estado == ESTADO_TUTORIAL) {
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                estado = ESTADO_MENU_PRINCIPAL;
            }
        }
        else if (estado == ESTADO_EXPLORANDO) {
            // Pause menu with ESC
            if (IsKeyPressed(KEY_ESCAPE)) {
                estado = ESTADO_PAUSA;
            }
            
            // Inventory with I
            if (IsKeyPressed(KEY_I)) {
                estado = ESTADO_INVENTARIO;
            }
            
            int dx = 0, dy = 0;
            
            // Movimentação (Podes ajustar para manter pressionado ou toque único)
            if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))    dy = -1;
            if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))  dy = 1;
            if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  dx = -1;
            if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) dx = 1;

            if (dx != 0 || dy != 0) {
                // Tenta mover usando tua lógica de Mapa.c
                if (map_move_player(&mapa, &jogador, dx, dy)) {
                    
                    // Checar itens coletados e adicionar ao inventário
                    Tile current_tile = mapa.grid[jogador.pos_y][jogador.pos_x];
                    if (current_tile == TILE_MEDKIT) {
                        Item medkit = {ITEM_MEDKIT, "Medkit", 30, 1};
                        inventory_add(&jogador.inventario, medkit);
                        stats_registrar_item_coletado(&stats);
                        mapa.grid[jogador.pos_y][jogador.pos_x] = TILE_EMPTY;
                    } else if (current_tile == TILE_AMMO) {
                        Item ammo = {ITEM_MUNI, "Municao", 0, 10};
                        inventory_add(&jogador.inventario, ammo);
                        stats_registrar_item_coletado(&stats);
                        mapa.grid[jogador.pos_y][jogador.pos_x] = TILE_EMPTY;
                    } else if (current_tile == TILE_WEAPON) {
                        // Random weapon spawn - could be any weapon type
                        int weapon_type = rand() % 9; // 9 different weapons
                        Item weapon;
                        weapon.quantidade = 1;
                        
                        switch(weapon_type) {
                            case 0: // Pistola
                                weapon.tipo = ITEM_PISTOLA;
                                strcpy(weapon.nome, "Pistola");
                                weapon.poder = 15;
                                weapon.is_melee = 0;
                                break;
                            case 1: // Revolver
                                weapon.tipo = ITEM_REVOLVER;
                                strcpy(weapon.nome, "Revolver");
                                weapon.poder = 20;
                                weapon.is_melee = 0;
                                break;
                            case 2: // Rifle
                                weapon.tipo = ITEM_RIFLE;
                                strcpy(weapon.nome, "Rifle de Ferrolho");
                                weapon.poder = 30;
                                weapon.is_melee = 0;
                                break;
                            case 3: // Fuzil
                                weapon.tipo = ITEM_FUZIL;
                                strcpy(weapon.nome, "Fuzil de Assalto");
                                weapon.poder = 25;
                                weapon.is_melee = 0;
                                break;
                            case 4: // Escopeta
                                weapon.tipo = ITEM_ESCOPETA;
                                strcpy(weapon.nome, "Escopeta");
                                weapon.poder = 35;
                                weapon.is_melee = 0;
                                break;
                            case 5: // Faca
                                weapon.tipo = ITEM_FACA;
                                strcpy(weapon.nome, "Faca");
                                weapon.poder = 10;
                                weapon.is_melee = 1;
                                break;
                            case 6: // Bastão
                                weapon.tipo = ITEM_BASTAO;
                                strcpy(weapon.nome, "Bastao");
                                weapon.poder = 12;
                                weapon.is_melee = 1;
                                break;
                            case 7: // Cano
                                weapon.tipo = ITEM_CANO;
                                strcpy(weapon.nome, "Cano de Ferro");
                                weapon.poder = 15;
                                weapon.is_melee = 1;
                                break;
                            case 8: // Espada
                                weapon.tipo = ITEM_ESPADA;
                                strcpy(weapon.nome, "Espada");
                                weapon.poder = 20;
                                weapon.is_melee = 1;
                                break;
                        }
                        
                        inventory_add(&jogador.inventario, weapon);
                        // Auto-equip weapon if none equipped
                        if (jogador.equipped_weapon_idx < 0) {
                            jogador.equipped_weapon_idx = jogador.inventario.size - 1;
                        }
                        stats_registrar_item_coletado(&stats);
                        mapa.grid[jogador.pos_y][jogador.pos_x] = TILE_EMPTY;
                    } else if (current_tile == TILE_ITEM) {
                        // Generic item - could be armor
                        Item armor;
                        armor.tipo = ITEM_ARMOR;
                        strcpy(armor.nome, "Colete");
                        armor.poder = 50;
                        armor.quantidade = 1;
                        armor.is_melee = 0;
                        inventory_add(&jogador.inventario, armor);
                        // Auto-equip armor if none equipped
                        if (jogador.equipped_armor_idx < 0) {
                            jogador.equipped_armor_idx = jogador.inventario.size - 1;
                            jogador.armor_durability = armor.poder;
                        }
                        stats_registrar_item_coletado(&stats);
                        mapa.grid[jogador.pos_y][jogador.pos_x] = TILE_EMPTY;
                    }
                    
                    // Lógica de Encontro
                    if (map_check_encounter(&mapa, &jogador)) {
                        // Encontrar o inimigo na posição atual
                        for (int i = 0; i < mapa.num_inimigos; i++) {
                            if (mapa.inimigos[i].pos_x == jogador.pos_x && 
                                mapa.inimigos[i].pos_y == jogador.pos_y &&
                                mapa.inimigos[i].ativo) {
                                inimigoAtual = &mapa.inimigos[i];
                                hp_inimigo_atual = inimigoAtual->hp;
                                estadoCombate = COMBATE_MENU_PRINCIPAL;
                                opcaoCombate = 0;
                                strcpy(mensagemCombate, "");
                                estado = ESTADO_BATALHA;
                                break;
                            }
                        }
                    }
                }
            }

            // ATUALIZAR CÂMARA: Fazer a câmara seguir o jogador suavemente
            // Destino da câmara é a posição do jogador convertida para pixels + metade do tamanho do tile (para centralizar)
            Vector2 targetPos = { 
                jogador.pos_x * TILE_SIZE + (TILE_SIZE/2.0f), 
                jogador.pos_y * TILE_SIZE + (TILE_SIZE/2.0f) 
            };
            
            // Interpolação simples para suavizar o movimento da câmera (Lerp)
            camera.target.x += (targetPos.x - camera.target.x) * 0.1f;
            camera.target.y += (targetPos.y - camera.target.y) * 0.1f;
        }
        else if (estado == ESTADO_BATALHA) {
            tempoCombate += GetFrameTime();
            
            if (estadoCombate == COMBATE_MENU_PRINCIPAL) {
                // Navegação no menu de combate
                if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                    opcaoCombate = (opcaoCombate + 1) % 3;
                }
                if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                    opcaoCombate = (opcaoCombate - 1 + 3) % 3;
                }
                
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    if (opcaoCombate == 0) { // ATACAR
                        int ataque_base = jogador.ataque > 0 ? jogador.ataque : 1;
                        
                        // Add weapon damage bonus if equipped
                        int weapon_bonus = 0;
                        int is_ranged_attack = 0;
                        int is_melee_attack = 0;
                        
                        if (jogador.equipped_weapon_idx >= 0 && 
                            jogador.equipped_weapon_idx < (int)jogador.inventario.size) {
                            Item *equipped_weapon = &jogador.inventario.itens[jogador.equipped_weapon_idx];
                            
                            if (equipped_weapon->is_melee) {
                                // Melee weapon - no ammo needed
                                weapon_bonus = equipped_weapon->poder;
                                is_melee_attack = 1;
                            } else {
                                // Ranged weapon - check for ammo
                                int ammo_idx = inventory_find_type(&jogador.inventario, ITEM_MUNI);
                                if (ammo_idx >= 0 && jogador.inventario.itens[ammo_idx].quantidade > 0) {
                                    // Use ammo and add weapon damage
                                    weapon_bonus = equipped_weapon->poder;
                                    is_ranged_attack = 1;
                                    jogador.inventario.itens[ammo_idx].quantidade--;
                                    if (jogador.inventario.itens[ammo_idx].quantidade <= 0) {
                                        inventory_remove_index(&jogador.inventario, ammo_idx);
                                        // Update indices if ammo was before them
                                        if (ammo_idx < jogador.equipped_weapon_idx) {
                                            jogador.equipped_weapon_idx--;
                                        }
                                        if (ammo_idx < jogador.equipped_armor_idx) {
                                            jogador.equipped_armor_idx--;
                                        }
                                    }
                                }
                            }
                        }
                        
                        // Fixed damage calculation: base random + fixed weapon bonus
                        int dano = (rand() % ataque_base) + weapon_bonus + 2 - inimigoAtual->defesa;
                        if (dano < 1) dano = 1;
                        hp_inimigo_atual -= dano;
                        
                        // Attack message based on weapon type
                        if (is_ranged_attack) {
                            snprintf(mensagemCombate, sizeof(mensagemCombate), 
                                    "%s disparou e causou %d de dano!", jogador.nome, dano);
                        } else if (is_melee_attack) {
                            snprintf(mensagemCombate, sizeof(mensagemCombate), 
                                    "%s golpeou e causou %d de dano!", jogador.nome, dano);
                        } else {
                            snprintf(mensagemCombate, sizeof(mensagemCombate), 
                                    "%s atacou com socos e causou %d de dano!", jogador.nome, dano);
                        }
                        estadoCombate = COMBATE_ATACANDO;
                        tempoCombate = 0.0f;
                    } else if (opcaoCombate == 1) { // ITEM
                        int idx = inventory_find_type(&jogador.inventario, ITEM_MEDKIT);
                        if (idx >= 0) {
                            Item *it = &jogador.inventario.itens[idx];
                            int cura = it->poder;
                            jogador.hp += cura;
                            if (jogador.hp > jogador.hp_max) jogador.hp = jogador.hp_max;
                            it->quantidade--;
                            if (it->quantidade <= 0) {
                                inventory_remove_index(&jogador.inventario, idx);
                            }
                            snprintf(mensagemCombate, sizeof(mensagemCombate), "%s usou Medkit e recuperou %d HP!", jogador.nome, cura);
                            estadoCombate = COMBATE_USANDO_ITEM;
                            tempoCombate = 0.0f;
                        } else {
                            snprintf(mensagemCombate, sizeof(mensagemCombate), "Nenhum Medkit disponivel!");
                            tempoCombate = 0.0f;
                        }
                    } else if (opcaoCombate == 2) { // FUGIR
                        if ((rand() % 100) < 50) {
                            stats_registrar_fuga(&stats);
                            estado = ESTADO_EXPLORANDO;
                        } else {
                            snprintf(mensagemCombate, sizeof(mensagemCombate), "Nao conseguiu fugir!");
                            estadoCombate = COMBATE_TURNO_INIMIGO;
                            tempoCombate = 0.0f;
                        }
                    }
                }
            }
            else if (estadoCombate == COMBATE_ATACANDO || estadoCombate == COMBATE_USANDO_ITEM) {
                if (tempoCombate > 1.5f) { // Após 1.5s
                    if (hp_inimigo_atual <= 0) {
                        // Vitória!
                        snprintf(mensagemCombate, sizeof(mensagemCombate), "%s foi derrotado! Voce ganhou %d XP!", inimigoAtual->nome, inimigoAtual->xp);
                        jogador.xp += inimigoAtual->xp;
                        player_check_level_up(&jogador);
                        inimigoAtual->ativo = 0;
                        stats_registrar_zumbi_derrotado(&stats);
                        estadoCombate = COMBATE_VITORIA;
                        tempoCombate = 0.0f;
                    } else {
                        // Turno do inimigo
                        estadoCombate = COMBATE_TURNO_INIMIGO;
                        tempoCombate = 0.0f;
                    }
                }
            }
            else if (estadoCombate == COMBATE_TURNO_INIMIGO) {
                if (tempoCombate > 1.0f) { // Inimigo ataca após 1s
                    if (tempoCombate < 1.1f) { // Executar apenas uma vez
                        int ataque_inimigo = inimigoAtual->ataque > 0 ? inimigoAtual->ataque : 1;
                        int dano = (rand() % ataque_inimigo) + 1 - jogador.defesa;
                        if (dano < 1) dano = 1;
                        
                        // Apply armor damage reduction if active
                        int dano_real = dano;
                        if (jogador.armor_durability > 0) {
                            dano_real = dano / 2;  // 50% damage reduction
                            if (dano_real < 1) dano_real = 1;
                            jogador.armor_durability -= dano_real;
                            
                            if (jogador.armor_durability <= 0) {
                                jogador.armor_durability = 0;
                                // Remove armor from inventory
                                if (jogador.equipped_armor_idx >= 0 && 
                                    jogador.equipped_armor_idx < (int)jogador.inventario.size) {
                                    int removed_idx = jogador.equipped_armor_idx;
                                    inventory_remove_index(&jogador.inventario, removed_idx);
                                    // Update indices
                                    if (jogador.equipped_weapon_idx > removed_idx) {
                                        jogador.equipped_weapon_idx--;
                                    }
                                    jogador.equipped_armor_idx = -1;
                                }
                                snprintf(mensagemCombate, sizeof(mensagemCombate), 
                                        "%s atacou! Colete absorveu %d dano mas foi destruido!", 
                                        inimigoAtual->nome, dano - dano_real);
                            } else {
                                snprintf(mensagemCombate, sizeof(mensagemCombate), 
                                        "%s atacou! Colete absorveu parte do dano (%d)!", 
                                        inimigoAtual->nome, dano_real);
                            }
                        } else {
                            snprintf(mensagemCombate, sizeof(mensagemCombate), 
                                    "%s atacou e causou %d de dano!", inimigoAtual->nome, dano_real);
                        }
                        
                        jogador.hp -= dano_real;
                        
                        if (jogador.hp <= 0) {
                            jogador.hp = 0;
                            snprintf(mensagemCombate, sizeof(mensagemCombate), "Voce foi derrotado...");
                            stats_registrar_morte(&stats);
                            estadoCombate = COMBATE_DERROTA;
                        }
                    }
                    
                    // Aguarda 1.5s depois do ataque para voltar ao menu
                    if (tempoCombate > 2.5f && estadoCombate != COMBATE_DERROTA) {
                        estadoCombate = COMBATE_MENU_PRINCIPAL;
                        strcpy(mensagemCombate, "");
                    }
                }
            }
            else if (estadoCombate == COMBATE_VITORIA) {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    mapa.grid[jogador.pos_y][jogador.pos_x] = TILE_GRASS;
                    
                    // Verificar se todos os inimigos foram derrotados
                    if (map_check_all_enemies_defeated(&mapa)) {
                        estado = ESTADO_VITORIA;
                    } else {
                        estado = ESTADO_EXPLORANDO;
                    }
                }
            }
            else if (estadoCombate == COMBATE_DERROTA) {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    estado = ESTADO_GAME_OVER;
                }
            }
        }
        else if (estado == ESTADO_PAUSA) {
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                estado = ESTADO_EXPLORANDO;
            }
            if (IsKeyPressed(KEY_Q)) {
                // Save and quit
                if (inicio_sessao > 0) {
                    time_t fim_sessao = time(NULL);
                    int tempo_sessao = (int)difftime(fim_sessao, inicio_sessao);
                    stats_atualizar_tempo(&stats, tempo_sessao);
                    stats_save(&stats, "stats.dat");
                }
                estado = ESTADO_MENU_PRINCIPAL;
            }
        }
        else if (estado == ESTADO_INVENTARIO) {
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_I)) {
                estado = ESTADO_EXPLORANDO;
            }
        }
        else if (estado == ESTADO_GAME_OVER) {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                estado = ESTADO_MENU_PRINCIPAL;
            }
        }
        else if (estado == ESTADO_VITORIA) {
            if (IsKeyPressed(KEY_ENTER)) {
                // Continuar jogando - repopular mapa
                map_repopulate_enemies(&mapa);
                estado = ESTADO_EXPLORANDO;
            } else if (IsKeyPressed(KEY_ESCAPE)) {
                // Sair para o menu principal
                if (inicio_sessao > 0) {
                    time_t fim_sessao = time(NULL);
                    int tempo_sessao = (int)difftime(fim_sessao, inicio_sessao);
                    stats_atualizar_tempo(&stats, tempo_sessao);
                    stats_save(&stats, "stats.dat");
                }
                estado = ESTADO_MENU_PRINCIPAL;
            }
        }

        // --- DESENHO ---
        BeginDrawing();
        ClearBackground(BLACK);

        if (estado == ESTADO_MENU_PRINCIPAL) {
            // Desenhar menu principal
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){20, 20, 30, 255});
            
            DrawText("ZOMBIE RAMPAGE", SCREEN_WIDTH/2 - 180, 80, 40, RED);
            DrawText("Menu Principal", SCREEN_WIDTH/2 - 100, 140, 20, WHITE);
            
            const char* opcoes[] = {
                "1. Jogar",
                "2. Carregar Jogo",
                "3. Opcoes",
                "4. Estatisticas",
                "5. Sobre",
                "6. Tutorial",
                "7. Reset",
                "8. Sair"
            };
            
            for (int i = 0; i < 8; i++) {
                Color cor = (i == opcaoMenu) ? YELLOW : WHITE;
                DrawText(opcoes[i], SCREEN_WIDTH/2 - 80, 200 + i * 35, 20, cor);
            }
            
            DrawText("Use SETAS para navegar, ENTER para selecionar", 180, 550, 15, LIGHTGRAY);
        }
        else if (estado == ESTADO_OPCOES) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){20, 20, 30, 255});
            DrawText("OPCOES", SCREEN_WIDTH/2 - 60, 80, 40, RED);
            
            char volumeText[50];
            snprintf(volumeText, sizeof(volumeText), "Volume: %d%%", config.volume_musica);
            Color cor0 = (opcaoOpcoes == 0) ? YELLOW : WHITE;
            DrawText(volumeText, SCREEN_WIDTH/2 - 100, 200, 20, cor0);
            
            char difText[50];
            const char* dificuldades[] = {"Facil", "Normal", "Dificil"};
            snprintf(difText, sizeof(difText), "Dificuldade: %s", dificuldades[config.dificuldade - 1]);
            Color cor1 = (opcaoOpcoes == 1) ? YELLOW : WHITE;
            DrawText(difText, SCREEN_WIDTH/2 - 100, 240, 20, cor1);
            
            Color cor2 = (opcaoOpcoes == 2) ? YELLOW : WHITE;
            DrawText("Voltar", SCREEN_WIDTH/2 - 100, 280, 20, cor2);
            
            DrawText("ENTER para alterar, ESC para voltar", 230, 550, 15, LIGHTGRAY);
        }
        else if (estado == ESTADO_ESTATISTICAS) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){20, 20, 30, 255});
            DrawText("ESTATISTICAS", SCREEN_WIDTH/2 - 120, 80, 40, RED);
            
            char texto[100];
            snprintf(texto, sizeof(texto), "Zumbis Derrotados: %d", stats.zumbis_derrotados);
            DrawText(texto, 200, 180, 20, WHITE);
            
            snprintf(texto, sizeof(texto), "Itens Coletados: %d", stats.itens_coletados);
            DrawText(texto, 200, 220, 20, WHITE);
            
            snprintf(texto, sizeof(texto), "Mortes: %d", stats.mortes);
            DrawText(texto, 200, 260, 20, WHITE);
            
            snprintf(texto, sizeof(texto), "Fugas: %d", stats.fugas);
            DrawText(texto, 200, 300, 20, WHITE);
            
            int horas = stats.tempo_jogo_segundos / 3600;
            int minutos = (stats.tempo_jogo_segundos % 3600) / 60;
            snprintf(texto, sizeof(texto), "Tempo Jogado: %dh %dm", horas, minutos);
            DrawText(texto, 200, 340, 20, WHITE);
            
            DrawText("Pressione ESC ou ENTER para voltar", 220, 550, 15, LIGHTGRAY);
        }
        else if (estado == ESTADO_SOBRE) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){20, 20, 30, 255});
            DrawText("SOBRE", SCREEN_WIDTH/2 - 60, 80, 40, RED);
            
            DrawText("ZOMBIE RAMPAGE", 280, 160, 25, WHITE);
            DrawText("Jogo desenvolvido para a disciplina de Algoritmos", 150, 200, 15, LIGHTGRAY);
            DrawText("Universidade Federal de Pernambuco - CIn", 180, 230, 15, LIGHTGRAY);
            
            DrawText("Equipe:", 300, 280, 20, WHITE);
            DrawText("Bruno Gabriel, Diogo da Silva", 220, 310, 15, LIGHTGRAY);
            DrawText("Gryghor Camonni, Flavia Vitoria, Lucas Cabral", 160, 335, 15, LIGHTGRAY);
            
            DrawText("Pressione ESC ou ENTER para voltar", 220, 550, 15, LIGHTGRAY);
        }
        else if (estado == ESTADO_TUTORIAL) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){20, 20, 30, 255});
            DrawText("TUTORIAL", SCREEN_WIDTH/2 - 80, 80, 40, RED);
            
            DrawText("Controles:", 280, 160, 25, WHITE);
            DrawText("W/A/S/D ou SETAS - Mover", 240, 200, 18, LIGHTGRAY);
            DrawText("ESC - Voltar ao menu", 240, 230, 18, LIGHTGRAY);
            DrawText("ESPACO - Atacar no combate", 240, 260, 18, LIGHTGRAY);
            DrawText("F - Fugir do combate", 240, 290, 18, LIGHTGRAY);
            
            DrawText("Objetivo:", 280, 340, 25, WHITE);
            DrawText("Sobreviva derrotando zumbis e coletando itens!", 170, 380, 18, LIGHTGRAY);
            
            DrawText("Pressione ESC ou ENTER para voltar", 220, 550, 15, LIGHTGRAY);
        }
        else if (estado == ESTADO_PAUSA) {
            // Pause menu overlay
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.7f));
            DrawRectangle(200, 150, 400, 300, (Color){40, 40, 60, 230});
            DrawRectangleLines(200, 150, 400, 300, WHITE);
            
            DrawText("JOGO PAUSADO", 310, 180, 30, YELLOW);
            DrawText("ESC/ENTER - Continuar", 270, 250, 20, WHITE);
            DrawText("Q - Salvar e Sair para Menu", 230, 290, 20, WHITE);
            DrawText("Os controles:", 280, 350, 18, LIGHTGRAY);
            DrawText("I - Inventario", 300, 380, 16, LIGHTGRAY);
        }
        else if (estado == ESTADO_INVENTARIO) {
            // Inventory screen
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){20, 20, 30, 255});
            DrawText("INVENTARIO", SCREEN_WIDTH/2 - 100, 50, 35, YELLOW);
            
            int y_offset = 120;
            DrawText(TextFormat("HP: %d/%d", jogador.hp, jogador.hp_max), 50, y_offset, 20, GREEN);
            DrawText(TextFormat("Nivel: %d", jogador.nivel), 50, y_offset + 30, 20, WHITE);
            DrawText(TextFormat("Ataque: %d", jogador.ataque), 50, y_offset + 60, 20, WHITE);
            DrawText(TextFormat("Defesa: %d", jogador.defesa), 50, y_offset + 90, 20, WHITE);
            
            // Equipment status
            if (jogador.equipped_weapon_idx >= 0 && 
                jogador.equipped_weapon_idx < (int)jogador.inventario.size) {
                Item *weapon = &jogador.inventario.itens[jogador.equipped_weapon_idx];
                DrawText(TextFormat("Arma: %s (%d dano) - Equipada", weapon->nome, weapon->poder), 
                         50, y_offset + 120, 18, YELLOW);
            } else {
                DrawText("Arma: Socos (5 dano) - Desarmado", 50, y_offset + 120, 18, GRAY);
            }
            
            if (jogador.equipped_armor_idx >= 0 && 
                jogador.equipped_armor_idx < (int)jogador.inventario.size) {
                DrawText(TextFormat("Colete: %d durabilidade - Equipado", jogador.armor_durability), 
                         50, y_offset + 145, 18, SKYBLUE);
            } else {
                DrawText("Colete: Nenhum", 50, y_offset + 145, 18, GRAY);
            }
            
            DrawText("Itens:", 50, y_offset + 185, 25, YELLOW);
            if (jogador.inventario.size == 0) {
                DrawText("Inventario vazio", 70, y_offset + 225, 18, LIGHTGRAY);
            } else {
                for (size_t i = 0; i < jogador.inventario.size && i < 10; i++) {
                    Item *it = &jogador.inventario.itens[i];
                    const char* tipo_str = "";
                    const char* equip_str = "";
                    
                    // Determine item type
                    if (it->tipo == ITEM_MEDKIT) tipo_str = "[MEDKIT]";
                    else if (it->tipo == ITEM_MUNI) tipo_str = "[MUNI]";
                    else if (it->tipo == ITEM_ARMOR) {
                        tipo_str = "[COLETE]";
                        if (i == jogador.equipped_armor_idx) equip_str = " (EQUIPADO)";
                    }
                    else if (it->tipo >= ITEM_REVOLVER && it->tipo <= ITEM_ESPADA) {
                        if (it->is_melee) tipo_str = "[ARMA CC]"; // Corpo a corpo
                        else tipo_str = "[ARMA]";
                        if (i == jogador.equipped_weapon_idx) equip_str = " (EQUIPADA)";
                    }
                    
                    DrawText(TextFormat("%s %s x%d (Poder: %d)%s", tipo_str, it->nome, it->quantidade, it->poder, equip_str), 
                             70, y_offset + 225 + i * 25, 18, WHITE);
                }
            }
            
            DrawText("Pressione ESC ou I para voltar", 250, 550, 15, LIGHTGRAY);
        }
        else if (estado == ESTADO_GAME_OVER) {
            // Game over screen
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){20, 20, 30, 255});
            DrawText("GAME OVER", SCREEN_WIDTH/2 - 150, 200, 50, RED);
            DrawText(TextFormat("Voce sobreviveu ate o nivel %d", jogador.nivel), 
                     SCREEN_WIDTH/2 - 200, 300, 20, WHITE);
            DrawText("Pressione ENTER para voltar ao menu", 220, 400, 18, LIGHTGRAY);
        }
        else if (estado == ESTADO_VITORIA) {
            // Victory screen
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){20, 40, 20, 255});
            DrawText("VITORIA!", SCREEN_WIDTH/2 - 120, 150, 50, GOLD);
            DrawText("Voce eliminou todos os zumbis do mapa!", 
                     SCREEN_WIDTH/2 - 220, 230, 20, WHITE);
            DrawText(TextFormat("Nivel alcancado: %d", jogador.nivel), 
                     SCREEN_WIDTH/2 - 100, 280, 20, WHITE);
            DrawText(TextFormat("XP total: %d", jogador.xp), 
                     SCREEN_WIDTH/2 - 80, 310, 20, WHITE);
            
            DrawText("Pressione ENTER para continuar jogando", 
                     SCREEN_WIDTH/2 - 200, 380, 18, GREEN);
            DrawText("(Novos zumbis aparecerão no mapa)", 
                     SCREEN_WIDTH/2 - 160, 410, 14, LIGHTGRAY);
            DrawText("Pressione ESC para voltar ao menu", 
                     SCREEN_WIDTH/2 - 150, 450, 18, YELLOW);
        }
        else if (estado == ESTADO_EXPLORANDO || estado == ESTADO_BATALHA) {

            // INICIO DO MODO 2D (Tudo aqui dentro move-se com a câmara)
            BeginMode2D(camera);

                // 1. Desenhar o Chão (Background do Mundo)
                // Desenhamos um retângulo gigante cinza/verde para simular o chão
                DrawRectangle(0, 0, MAP_W * TILE_SIZE, MAP_H * TILE_SIZE, COLOR_GRASS);
                
                // Opcional: Desenhar linhas de grade para ajudar a ver
                for(int i = 0; i <= MAP_W; i++) DrawLine(i*TILE_SIZE, 0, i*TILE_SIZE, MAP_H*TILE_SIZE, DARKGREEN);
                for(int i = 0; i <= MAP_H; i++) DrawLine(0, i*TILE_SIZE, MAP_W*TILE_SIZE, i*TILE_SIZE, DARKGREEN);

                // 2. Desenhar Objetos do Mapa
                for (int y = 0; y < MAP_H; y++) {
                    for (int x = 0; x < MAP_W; x++) {
                        int posX = x * TILE_SIZE;
                        int posY = y * TILE_SIZE;
                        
                        // Desenhar tiles com sprites quando disponíveis
                        switch (mapa.grid[y][x]) {
                            case TILE_WALL:
                                DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, COLOR_WALL);
                                // Efeito de sombra na parede
                                DrawRectangle(posX, posY + TILE_SIZE - 5, TILE_SIZE, 5, BLACK); 
                                break;
                            case TILE_ZOMBIE:
                                // Usar sprite de batalha escalado para o overworld
                                if (zumbiBatalha.id > 0) {
                                    // Escalar sprite de batalha para caber no tile (48x48)
                                    float scale = (float)TILE_SIZE / (float)zumbiBatalha.width;
                                    if (scale > 1.0f) scale = 1.0f; // Não aumentar, só diminuir
                                    Rectangle sourceRec = {0, 0, (float)zumbiBatalha.width, (float)zumbiBatalha.height};
                                    Rectangle destRec = {
                                        posX + (TILE_SIZE - zumbiBatalha.width * scale) / 2,
                                        posY + (TILE_SIZE - zumbiBatalha.height * scale) / 2,
                                        zumbiBatalha.width * scale,
                                        zumbiBatalha.height * scale
                                    };
                                    DrawTexturePro(zumbiBatalha, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
                                } else {
                                    // Fallback se sprite não carregou
                                    DrawRectangle(posX + 10, posY + 10, TILE_SIZE - 20, TILE_SIZE - 20, COLOR_ZOMBIE);
                                    DrawText("Z", posX + 15, posY + 10, 20, WHITE);
                                }
                                break;
                            case TILE_MEDKIT:
                                // Desenhar sprite de medkit
                                DrawTextureEx(healthArmor, (Vector2){posX + 8, posY + 8}, 0.0f, 1.0f, WHITE);
                                break;
                            case TILE_AMMO:
                                // Desenhar sprite de munição
                                DrawTextureEx(ammoPistol, (Vector2){posX + 8, posY + 8}, 0.0f, 1.0f, WHITE);
                                break;
                            case TILE_ITEM:
                            case TILE_WEAPON:
                                DrawRectangle(posX + 15, posY + 15, TILE_SIZE - 30, TILE_SIZE - 30, COLOR_ITEM);
                                break;
                        }
                    }
                }

                // 3. Desenhar Jogador usando sprite de batalha (Soldado_Batalha)
                // Usar a mesma sprite da batalha para consistência visual
                
                if (soldadoBatalha.id > 0) {
                    // Calcular escala para caber no tile
                    float scaleX = (float)TILE_SIZE / (float)soldadoBatalha.width;
                    float scaleY = (float)TILE_SIZE / (float)soldadoBatalha.height;
                    float scale = (scaleX < scaleY) ? scaleX : scaleY; // Usar menor escala
                    
                    // Calcular posição centralizada
                    float spriteWidth = soldadoBatalha.width * scale;
                    float spriteHeight = soldadoBatalha.height * scale;
                    float posX = jogador.pos_x * TILE_SIZE + (TILE_SIZE - spriteWidth) / 2;
                    float posY = jogador.pos_y * TILE_SIZE + (TILE_SIZE - spriteHeight) / 2;
                    
                    DrawTextureEx(soldadoBatalha, (Vector2){posX, posY}, 0.0f, scale, WHITE);
                } else {
                    // Fallback se sprite não carregou
                    DrawRectangle(jogador.pos_x * TILE_SIZE + 8, 
                                  jogador.pos_y * TILE_SIZE + 8, 
                                  TILE_SIZE - 16, TILE_SIZE - 16, COLOR_PLAYER);
                }

            EndMode2D();
            // FIM DO MODO 2D (Coisas estáticas na tela, como UI)

            // --- UI / HUD ---
            DrawText("Zombie Rampage - Alpha", 10, 10, 20, WHITE);
            DrawText(TextFormat("HP: %d", jogador.hp), 10, 40, 20, GREEN);
            DrawText(TextFormat("Pos: %d, %d", jogador.pos_x, jogador.pos_y), 10, 70, 10, LIGHTGRAY);
            
            // Equipment status in HUD
            int hud_y = 90;
            if (jogador.equipped_weapon_idx >= 0 && 
                jogador.equipped_weapon_idx < (int)jogador.inventario.size) {
                Item *weapon = &jogador.inventario.itens[jogador.equipped_weapon_idx];
                if (weapon->is_melee) {
                    DrawText(TextFormat("Arma: %s (%d dano)", weapon->nome, weapon->poder), 10, hud_y, 14, YELLOW);
                } else {
                    int ammo_idx = inventory_find_type(&jogador.inventario, ITEM_MUNI);
                    int ammo_count = (ammo_idx >= 0) ? jogador.inventario.itens[ammo_idx].quantidade : 0;
                    DrawText(TextFormat("Arma: %s (%d balas)", weapon->nome, ammo_count), 10, hud_y, 14, YELLOW);
                }
                hud_y += 20;
            } else {
                DrawText("Arma: Socos (5 dano)", 10, hud_y, 14, GRAY);
                hud_y += 20;
            }
            if (jogador.armor_durability > 0) {
                Color armor_color = (jogador.armor_durability > 25) ? SKYBLUE : ORANGE;
                DrawText(TextFormat("Colete: %d", jogador.armor_durability), 10, hud_y, 14, armor_color);
                hud_y += 20;
            }
            
            DrawText("ESC - Menu | I - Inventario", 10, hud_y + 5, 13, LIGHTGRAY);

            if (estado == ESTADO_BATALHA) {
                // ===== INTERFACE DE COMBATE ESTILO POKEMON =====
                
                // Fundo semi-transparente
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.7f));
                
                // === ÁREA SUPERIOR: INIMIGO ===
                // Caixa de informações do inimigo (canto superior direito)
                int infoInimigoX = SCREEN_WIDTH - 280;
                int infoInimigoY = 30;
                DrawRectangle(infoInimigoX, infoInimigoY, 260, 90, (Color){40, 40, 60, 230});
                DrawRectangleLines(infoInimigoX, infoInimigoY, 260, 90, WHITE);
                
                DrawText(inimigoAtual->nome, infoInimigoX + 10, infoInimigoY + 10, 20, WHITE);
                DrawText(TextFormat("Lv. %d", 1), infoInimigoX + 200, infoInimigoY + 10, 18, LIGHTGRAY);
                
                // Barra de HP do inimigo
                // Nota: inimigoAtual->hp contém o HP máximo do inimigo
                // hp_inimigo_atual é o HP atual durante o combate
                DrawText("HP:", infoInimigoX + 10, infoInimigoY + 40, 16, WHITE);
                int hp_max_safe = inimigoAtual->hp > 0 ? inimigoAtual->hp : 1;
                float hpPercent = (float)hp_inimigo_atual / (float)hp_max_safe;
                if (hpPercent < 0.0f) hpPercent = 0.0f;
                if (hpPercent > 1.0f) hpPercent = 1.0f;
                int barWidth = 200;
                int barHeight = 20;
                DrawRectangle(infoInimigoX + 50, infoInimigoY + 40, barWidth, barHeight, DARKGRAY);
                Color hpColor = hpPercent > 0.5f ? GREEN : (hpPercent > 0.2f ? YELLOW : RED);
                DrawRectangle(infoInimigoX + 50, infoInimigoY + 40, (int)(barWidth * hpPercent), barHeight, hpColor);
                DrawRectangleLines(infoInimigoX + 50, infoInimigoY + 40, barWidth, barHeight, BLACK);
                DrawText(TextFormat("%d/%d", hp_inimigo_atual, inimigoAtual->hp), 
                         infoInimigoX + 70, infoInimigoY + 65, 14, WHITE);
                
                // Sprite do inimigo - usar nova imagem de batalha
                // Posição: centro-superior-direito
                int spriteInimigoX = SCREEN_WIDTH - 200; // Right side with margin
                int spriteInimigoY = 150; // Upper-center area
                
                if (zumbiBatalha.id > 0) {
                    // Draw new battle sprite
                    DrawTexture(zumbiBatalha, spriteInimigoX, spriteInimigoY, WHITE);
                } else {
                    // Fallback: desenhar retângulo se texture não carregou
                    DrawRectangle(spriteInimigoX, spriteInimigoY, 80, 100, COLOR_ZOMBIE);
                    DrawText("Z", spriteInimigoX + 30, spriteInimigoY + 30, 40, WHITE);
                }
                
                // === ÁREA INFERIOR ESQUERDA: JOGADOR ===
                // Caixa de informações do jogador (canto inferior esquerdo)
                int infoJogadorX = 20;
                int infoJogadorY = SCREEN_HEIGHT - 180;
                DrawRectangle(infoJogadorX, infoJogadorY, 280, 110, (Color){40, 40, 60, 230});
                DrawRectangleLines(infoJogadorX, infoJogadorY, 280, 110, WHITE);
                
                DrawText(jogador.nome, infoJogadorX + 10, infoJogadorY + 10, 20, WHITE);
                DrawText(TextFormat("Lv. %d", jogador.nivel), infoJogadorX + 220, infoJogadorY + 10, 18, YELLOW);
                
                // Barra de HP do jogador
                DrawText("HP:", infoJogadorX + 10, infoJogadorY + 40, 16, WHITE);
                float jogadorHpPercent = (float)jogador.hp / (float)jogador.hp_max;
                DrawRectangle(infoJogadorX + 50, infoJogadorY + 40, barWidth, barHeight, DARKGRAY);
                Color jogadorHpColor = jogadorHpPercent > 0.5f ? GREEN : (jogadorHpPercent > 0.2f ? YELLOW : RED);
                DrawRectangle(infoJogadorX + 50, infoJogadorY + 40, (int)(barWidth * jogadorHpPercent), barHeight, jogadorHpColor);
                DrawRectangleLines(infoJogadorX + 50, infoJogadorY + 40, barWidth, barHeight, BLACK);
                DrawText(TextFormat("%d/%d", jogador.hp, jogador.hp_max), 
                         infoJogadorX + 70, infoJogadorY + 65, 14, WHITE);
                
                // Barra de XP
                DrawText("XP:", infoJogadorX + 10, infoJogadorY + 80, 14, LIGHTGRAY);
                int xp_proximo_safe = jogador.xp_proximo_nivel > 0 ? jogador.xp_proximo_nivel : 1;
                float xpPercent = (float)jogador.xp / (float)xp_proximo_safe;
                if (xpPercent < 0.0f) xpPercent = 0.0f;
                if (xpPercent > 1.0f) xpPercent = 1.0f;
                DrawRectangle(infoJogadorX + 50, infoJogadorY + 80, barWidth, 10, DARKGRAY);
                DrawRectangle(infoJogadorX + 50, infoJogadorY + 80, (int)(barWidth * xpPercent), 10, SKYBLUE);
                DrawRectangleLines(infoJogadorX + 50, infoJogadorY + 80, barWidth, 10, BLACK);
                
                // Sprite do jogador - usar nova imagem de batalha
                // Posição: centro-inferior-esquerdo
                int spriteJogadorX = 50; // Left side with margin
                int spriteJogadorY = SCREEN_HEIGHT - 280; // Lower-center area, above info panel
                
                if (soldadoBatalha.id > 0) {
                    // Draw new battle sprite
                    DrawTexture(soldadoBatalha, spriteJogadorX, spriteJogadorY, WHITE);
                } else {
                    // Fallback: desenhar retângulo se texture não carregou
                    DrawRectangle(spriteJogadorX, spriteJogadorY, 80, 100, COLOR_PLAYER);
                    DrawText("P", spriteJogadorX + 30, spriteJogadorY + 30, 40, WHITE);
                }
                
                // === ÁREA INFERIOR DIREITA: MENU DE AÇÕES ===
                int menuX = SCREEN_WIDTH - 360;
                int menuY = SCREEN_HEIGHT - 180;
                
                if (estadoCombate == COMBATE_MENU_PRINCIPAL) {
                    // Menu principal de combate
                    DrawRectangle(menuX, menuY, 340, 160, (Color){20, 30, 50, 240});
                    DrawRectangleLines(menuX, menuY, 340, 160, WHITE);
                    DrawText("O que deseja fazer?", menuX + 20, menuY + 10, 18, WHITE);
                    
                    const char* opcoes[] = {"ATACAR", "ITEM (Medkit)", "FUGIR"};
                    const char* icones[] = {"[>]", "[+]", "[X]"};
                    
                    for (int i = 0; i < 3; i++) {
                        int optY = menuY + 50 + (i * 35);
                        Color cor = (i == opcaoCombate) ? YELLOW : WHITE;
                        Color corFundo = (i == opcaoCombate) ? (Color){60, 80, 120, 200} : (Color){30, 40, 60, 150};
                        
                        DrawRectangle(menuX + 20, optY - 5, 300, 30, corFundo);
                        if (i == opcaoCombate) {
                            DrawRectangleLines(menuX + 20, optY - 5, 300, 30, YELLOW);
                        }
                        DrawText(icones[i], menuX + 30, optY, 20, cor);
                        DrawText(opcoes[i], menuX + 70, optY, 20, cor);
                    }
                    
                    DrawText("Use SETAS/WS e ENTER para escolher", menuX + 30, menuY + 140, 12, LIGHTGRAY);
                }
                else if (estadoCombate == COMBATE_VITORIA || estadoCombate == COMBATE_DERROTA ||
                         estadoCombate == COMBATE_ATACANDO || estadoCombate == COMBATE_USANDO_ITEM ||
                         estadoCombate == COMBATE_TURNO_INIMIGO) {
                    // Caixa de mensagem
                    DrawRectangle(menuX, menuY, 340, 160, (Color){20, 30, 50, 240});
                    DrawRectangleLines(menuX, menuY, 340, 160, WHITE);
                    
                    // Desenhar mensagem com quebra de linha se necessário
                    DrawText(mensagemCombate, menuX + 20, menuY + 20, 18, WHITE);
                    
                    if (estadoCombate == COMBATE_VITORIA || estadoCombate == COMBATE_DERROTA) {
                        DrawText("Pressione ENTER para continuar", menuX + 40, menuY + 130, 14, YELLOW);
                    }
                }
            }
        }

        EndDrawing();
    }

    // Unload textures
    UnloadTexture(soldadoBatalha);
    UnloadTexture(zumbiBatalha);
    UnloadTexture(soldadBust);
    UnloadTexture(medicBust);
    UnloadTexture(engenheironus);
    UnloadTexture(zombieBust);
    UnloadTexture(soldadSheet);
    UnloadTexture(medicSheet);
    UnloadTexture(engenheirSheet);
    UnloadTexture(healthArmor);
    UnloadTexture(ammoPistol);

    CloseWindow();
    return 0;
}