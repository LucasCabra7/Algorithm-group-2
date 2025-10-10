#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <stdlib.h>

typedef enum {ITEM_MEDKIT = 0, ITEM_PISTOLA = 1, ITEM_MUNI = 2 } ItemType;

typedef struct 
{
    ItemType tipo;
    char nome[32];
    int poder; // Cura ou dano base
    int quantidade;
} Item;

typedef struct{
    Item *itens;
    size_t size;
    size_t capacity;
} Inventory;

void inventory_init(Inventory *inv);
void inventory_free(Inventory *inv);
void inventory_add(Inventory *inv, Item item);
int inventory_remove_index(Inventory *inv, size_t idx);
void inventory_list(const Inventory *inv);
int inventory_find_type(const Inventory *inv, ItemType tipo);

#endif