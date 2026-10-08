#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "hash_table.h"
#include "linked_list.h"

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