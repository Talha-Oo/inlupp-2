#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "../utilities/common.h"

store_t *ioopm_make_store(void);

merch_t *create_merch_user(void);

void add_merchandise_to_store(store_t *store);

void remove_merchandise_from_store(store_t *store);

void edit_merchandise_in_store(store_t *store);

void replenish_merchandise_in_store(store_t *store);

void list_merchandise(store_t *store);

void show_stock(store_t *store);

void add_cart_in_store(store_t *store);

void remove_cart_from_store(store_t *store);

void add_to_cart_in_store(store_t *store);

void remove_from_cart_in_store(store_t *store);

void remove_cart_in_store(store_t *store);

void calculate_cart_cost(store_t *store);

void checkout_cart(store_t *store);

bool confirm_quit(store_t *store);

char get_action_choice(void);

