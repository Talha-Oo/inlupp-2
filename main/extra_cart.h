#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "../utilities/common.h"

// TODO: UPDATE THE DOC

/// @brief Creates an empty shopping cart
/// @return pointer to the new shopping cart
void ioopm_add_cart(store_t *store);

/// @brief Adds a quantity of merchandise to a shopping cart
/// @param store pointer to the store containing the merchandise
/// @param id identifier of the shopping cart to which merchandise should be added
/// @param merch_name name of the merchandise to add
/// @param quantity quantity of merchandise to add
/// @return true if the merchandise was added successfully, false otherwise
int add_to_cart(store_t *store, size_t cart_id, char *merch_name, size_t quantity);




bool get_cart(store_t *store, size_t cart_id, elem_t *lookup_res);
void ioopm_destroy_cart(cart_t *cart);
cart_t *ioopm_create_cart(store_t *store);
void all_cart_destroyer(entry_t *entry);
