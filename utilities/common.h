#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "hash_table.c"
#include "linked_list.c"

typedef struct shelf_t {
    char * shelf;
    int quantity;
};

typedef struct merch_t {
    char * name;
    char * descr;
    size_t price;
    ioopm_list_t * locs;
};

typedef struct store_t {
    ioopm_hash_table_t name_to_item;
    ioopm_hash_table_t shelf_to_item;
};

typedef struct shopping_cart_t {
    ioopm_hash_table_t cart;
};

typedef struct all_shopping_carts_t {
    ioopm_list_t all_carts;
};