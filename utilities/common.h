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

typedef struct cart
{
    size_t id;
    ioopm_hash_table_t *merchendise;
} cart_t;

typedef struct store
{
    ioopm_hash_table_t *name_to_item;
    ioopm_hash_table_t *shelf_to_item;
    ioopm_hash_table_t *all_carts;
    ioopm_hash_table_t *reserved_merchs; 
    // TODO: TESTS for merch_num!!
    size_t merch_num; 
    size_t carts_num; 
} store_t;