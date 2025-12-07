#include "raylib.h"
#include "Mapa.h"
#include "Personagem.h"
#include "Inventario.h"
#include <stdio.h>

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
    ESTADO_EXPLORANDO, 
    ESTADO_BATALHA 
} GameState;

int main(void) {
    // 1. Inicialização da Janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Zombie Rampage - Overworld");
    SetTargetFPS(60);

    // 2. Carregar Dados do Jogo (Tuas Structs)
    Map mapa;
    Player jogador;
    Inventory inventario; // Adicionei para evitar erros, mesmo se não usarmos agora

    // Inicializa lógica antiga
    map_init(&mapa);
    jogador = player_creat("Sobrevivente", SOLDADO);
    
    // Força uma posição segura inicial
    jogador.pos_x = MAP_W / 2;
    jogador.pos_y = MAP_H / 2;
    mapa.grid[jogador.pos_y][jogador.pos_x] = TILE_EMPTY; 

    // 3. Configuração da CÂMARA (O Segredo do Top-Down)
    Camera2D camera = { 0 };
    camera.target = (Vector2){ jogador.pos_x * TILE_SIZE, jogador.pos_y * TILE_SIZE };
    camera.offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    GameState estado = ESTADO_EXPLORANDO;

    // --- GAME LOOP ---
    while (!WindowShouldClose()) {
        
        // --- ATUALIZAÇÃO ---
        
        if (estado == ESTADO_EXPLORANDO) {
            int dx = 0, dy = 0;
            
            // Movimentação (Podes ajustar para manter pressionado ou toque único)
            if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))    dy = -1;
            if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))  dy = 1;
            if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  dx = -1;
            if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) dx = 1;

            if (dx != 0 || dy != 0) {
                // Tenta mover usando tua lógica de Mapa.c
                // Nota: map_move_player retorna 1 se moveu, 0 se bateu na parede
                if (map_move_player(&mapa, &jogador, dx, dy)) {
                    
                    // Lógica de Encontro
                    if (map_check_encounter(&mapa, &jogador)) {
                        estado = ESTADO_BATALHA;
                        printf("Log: Entrou em combate!\n");
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

        // --- DESENHO ---
        BeginDrawing();
        ClearBackground(BLACK); // Fundo preto para o que estiver fora do mapa

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
                        
                        // Aqui no futuro entra a lógica: "Se tiver sprite, desenha sprite. Senão, desenha quadrado"
                        switch (mapa.grid[y][x]) {
                            case TILE_WALL:
                                DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, COLOR_WALL);
                                // Efeito de sombra na parede
                                DrawRectangle(posX, posY + TILE_SIZE - 5, TILE_SIZE, 5, BLACK); 
                                break;
                            case TILE_ZOMBIE:
                                DrawRectangle(posX + 10, posY + 10, TILE_SIZE - 20, TILE_SIZE - 20, COLOR_ZOMBIE);
                                break;
                            case TILE_ITEM:
                                DrawRectangle(posX + 15, posY + 15, TILE_SIZE - 30, TILE_SIZE - 30, COLOR_ITEM);
                                break;
                        }
                    }
                }

                // 3. Desenhar Jogador
                // Desenhamos um pouco menor que o tile para parecer que ele está "dentro" do mundo
                DrawRectangle(jogador.pos_x * TILE_SIZE + 8, 
                              jogador.pos_y * TILE_SIZE + 8, 
                              TILE_SIZE - 16, TILE_SIZE - 16, COLOR_PLAYER);

            EndMode2D();
            // FIM DO MODO 2D (Coisas estáticas na tela, como UI)

            // --- UI / HUD ---
            DrawText("MiniDayZ Clone - Alpha", 10, 10, 20, WHITE);
            DrawText(TextFormat("HP: %d", jogador.hp), 10, 40, 20, GREEN);
            DrawText(TextFormat("Pos: %d, %d", jogador.pos_x, jogador.pos_y), 10, 70, 10, LIGHTGRAY);

            if (estado == ESTADO_BATALHA) {
                DrawRectangle(100, 200, 600, 200, Fade(BLACK, 0.8f));
                DrawRectangleLines(100, 200, 600, 200, WHITE);
                DrawText("COMBATE!", 350, 220, 30, RED);
                DrawText("Zumbi te atacou! (Pressione ESPACO para fugir)", 180, 300, 20, WHITE);
                
                if (IsKeyPressed(KEY_SPACE)) {
                    // Lógica temporária para sair do combate
                    mapa.grid[jogador.pos_y][jogador.pos_x] = TILE_EMPTY; // Mata o zumbi
                    estado = ESTADO_EXPLORANDO;
                }
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}