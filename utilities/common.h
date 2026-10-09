#pragma once

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "hash_table.c"
#include "linked_list.c"

typedef struct shelf
{
    char *shelf;v
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
    ioopm_hash_table_t name_to_item;
    ioopm_hash_table_t shelf_to_item;
} store_t;

typedef struct shopping_cart
{
    size_t id;
    ioopm_hash_table_t cart;
} shopping_cart_t;

typedef struct all_shopping_carts
{
    ioopm_hash_table_t all_carts;
} all_shopping_carts_t;
