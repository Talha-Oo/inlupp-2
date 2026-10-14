#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "hash_table.h"
#include "linked_list.h"
#include "linked_list_iterator.h"
#include "hash_table_iterator.h"
#include "common_elem.h"


typedef struct shelf
{
    char *shelf_name;
    char **merch_name;
    int quantity;
} shelf_t;

typedef struct merch
{
    char *name;
    char *desc;
    size_t price;
    size_t stock;
    ioopm_list_t *locs;
} merch_t;

typedef struct store
{
    ioopm_hash_table_t *name_to_item;
    ioopm_hash_table_t *shelf_to_item;
    size_t merch_num;
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
