#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "hash_table.h"
#include "linked_list.h"

typedef struct shelf
{
    char *id;
    int quantity;
} shelf_t;

typedef struct merch
{
    char *name;
    char *descr;
    size_t price;
    ioopm_list_t *locs;
} merch_t;

typedef struct store
{
    ioopm_hash_table_t *name_to_item;
    ioopm_hash_table_t *shelf_to_item;
} store_t;

typedef struct cart
{
    size_t id;
    ioopm_hash_table_t *cart;
} cart_t;

typedef struct all_carts
{
    ioopm_hash_table_t *all_carts;
} all_carts_t;
