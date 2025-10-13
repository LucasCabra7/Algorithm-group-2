#ifndef INIMIGO_H
#define INIMIGO_H

#define MAX_INIMIGOS 10

typedef struct {
    char nome[50];    
    int hp;         
    int ataque;       
    int defesa;      
    int xp;          
    
    int pos_x;
    int pos_y;
    int ativo; // para saber se o inimigo está vivo (1) ou morto (0)
} Inimigo;

#endif