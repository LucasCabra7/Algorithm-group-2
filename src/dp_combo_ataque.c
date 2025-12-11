/*
 * Exemplo de Programação Dinâmica: Combo de Ataque (Mochila)
 * Dado um número de ações e uma lista de armas/itens com dano e custo,
 * calcula o máximo dano possível usando DP.
 */
#include <stdio.h>
#include <string.h>

#define MAX_ITENS 10
#define MAX_ACOES 20

typedef struct {
    char nome[32];
    int dano;
    int custo; // Quantas ações consome
} ItemAtaque;

// Retorna o máximo dano possível usando até "acoes" ações
int dp_max_dano(ItemAtaque itens[], int n, int acoes) {
    int dp[MAX_ACOES+1];
    memset(dp, 0, sizeof(dp));
    for (int i = 0; i < n; i++) {
        for (int j = acoes; j >= itens[i].custo; j--) {
            if (dp[j - itens[i].custo] + itens[i].dano > dp[j]) {
                dp[j] = dp[j - itens[i].custo] + itens[i].dano;
            }
        }
    }
    return dp[acoes];
}

int main() {
    ItemAtaque itens[] = {
        {"Soco", 5, 1},
        {"Faca", 12, 2},
        {"Pistola", 20, 3},
        {"Espada", 25, 4}
    };
    int n = 4;
    int acoes = 7;
    int max_dano = dp_max_dano(itens, n, acoes);
    printf("Maximo dano com %d acoes: %d\n", acoes, max_dano);
    return 0;
}
