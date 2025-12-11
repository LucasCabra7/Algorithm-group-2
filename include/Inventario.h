#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <stdlib.h>

typedef enum {
    ITEM_MEDKIT = 0, 
    ITEM_MUNI = 1,
    ITEM_ARMOR = 2,
    // Ranged weapons (require ammo)
    ITEM_REVOLVER = 3,
    ITEM_PISTOLA = 4,
    ITEM_RIFLE = 5,
    ITEM_FUZIL = 6,
    ITEM_ESCOPETA = 7,
    ITEM_ESCOPETA_SERRADA = 8,
    // Melee weapons (no ammo)
    ITEM_FACA = 9,
    ITEM_BASTAO = 10,
    ITEM_CANO = 11,
    ITEM_ESPADA = 12
} ItemType;

typedef struct 
{
    ItemType tipo;
    char nome[32];
    int poder; // Cura ou dano base
    int quantidade;
    int is_melee; // 1 for melee weapons, 0 for ranged
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