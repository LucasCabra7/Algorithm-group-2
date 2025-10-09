#include "Arquivos.h"
#include <stdio.h>

int save_game(const Player *p, const Map *m, const char *filename){
    FILE *f = fopen(filename, "wb");
    if(!f) return 0;
    fwrite(p, sizeof(Player), 1, f);
    fwrite(m, sizeof(Map), 1, f);

    // salvando inventario explicitamente no caso de ponteiros internos
    // Para segurança, salvamos o inventario manualmente
    fwrite(&p->inventario.size, sizeof(size_t), 1, f);
    for(size_t i = 0; i < p->inventario.size; i++){
        fwrite(&p->inventario.itens[i], sizeof(Item), 1, f);
    }
    fclose(f);
    return 1;
}

int load_game(Player *p, Map *m, const char *filename){
    FILE *f = fopen(filename, "rb");
    if(!f) return 0;

    Player temp;
    if(fread(&temp, sizeof(Player), 1, f) != 1) {fclose(f); return 0; }

    // Copiar campos simples:
    *p = temp;

    // Inicializar inventário:
    inventory_free(&p->inventario);
    inventory_init(&p->inventario);

    size_t inv_size = 0;
    if(fread(&inv_size, sizeof(size_t), 1, f) != 1) { fclose(f); return 0; }

    for(size_t i = 0; i < inv_size; i++){
        Item it;
        fread(&it, sizeof(Item), 1, f);
        inventory_add(&p->inventario, it);
    }

    if(fread(m, sizeof(Map), 1, f) != 1){
        // tentar ler mapa depois dos itens (compatibilidade)
        // no save gravamos player, map, invsize, items -> ordem de leitura foi adaptada
    }
    fclose(f);
    return 1;
}