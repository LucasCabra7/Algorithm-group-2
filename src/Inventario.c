#include "Inventario.h"
#include <stdio.h>
#include <string.h>

#define INV_INITIAL_CAP 4

void inventory_init(Inventory *inv){
    inv->itens = malloc(sizeof(Item) * INV_INITIAL_CAP);
    inv->size = 0;
    inv->capacity = INV_INITIAL_CAP;
};

void inventory_free(Inventory *inv){
    free(inv->itens);
    inv->itens = NULL;
    inv->size = 0;
    inv->capacity = 0;
};

void inventory_grow_if_needed(Inventory *inv) {
if (inv->size >= inv->capacity) {
inv->capacity *= 2;
inv->itens = realloc(inv->itens, sizeof(Item) * inv->capacity);
}
};

void inventory_add(Inventory *inv, Item item){
    // Se já exisitir do mesmo tipo, então soma a quantidade.
    int idx = inventory_find_type(inv, item.tipo);
    if(idx >= 0){
        inv->itens[idx].quantidade += item.quantidade;
        return;
    }
    inventory_grow_if_needed(inv);
    inv->itens[inv->size++] = item;
};

int inventory_remove_index(Inventory *inv, size_t idx){
    if(idx >= inv->size) return 0;
    for(size_t i = idx; i + 1 < inv->size; i++){
        inv->itens[i] = inv->itens[i + 1];
    }
    inv->size--;
    return 1;
};

void inventory_list(const Inventory *inv){
    printf("\n--- INVENTARIO (%zu itens) ---\n", inv->size);
    for(size_t i = 0; i < inv->size; i++){
        printf("%zu) %s x%d (poder %d)\n", i+1, inv->itens[i].nome, inv->itens[i].quantidade, inv->itens[i].poder);
    }
};

int inventory_find_type(const Inventory *inv, ItemType tipo){
    for(size_t i = 0; i < inv->size; i++){
        if(inv->itens[i].tipo == tipo){
            return (int)i;
        }
    }
    return -1;
}

