#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "../utilities/common.h"

// TODO: UPDATE THE DOC

/// @brief Creates an empty shopping cart
/// @return pointer to the new shopping cart
void add_cart(store_t *store);

/// @brief Removes a shopping cart from the container
/// @param store pointer to the container holding all shopping carts
/// @param id identifier of the shopping cart to remove
/// @return true if the cart was removed successfully, false otherwise
bool remove_cart(store_t *store, size_t chart_id);

/// @brief Adds a quantity of merchandise to a shopping cart
/// @param store pointer to the store containing the merchandise
/// @param id identifier of the shopping cart to which merchandise should be added
/// @param merch_name name of the merchandise to add
/// @param quantity quantity of merchandise to add
/// @return true if the merchandise was added successfully, false otherwise
bool add_to_cart(store_t *store, size_t chart_id, char *merch_name, size_t quantity);

/// @brief Removes a quantity of merchandise from a shopping cart
/// @param store pointer to the store containing the merchandise
/// @param id identifier of the shopping cart from which merchandise should be added
/// @param merch name of the merchandise to remove
/// @param quantity quantity of merchandise to remove
/// @return true if the merchandise was removed successfully, false otherwise
bool remove_from_cart(store_t *store, size_t chart_id, char *merch_name, size_t quantity);

/// @brief A purchase removes a shopping cart as well as all its items
/// @param store pointer to the store containing the merchandise
/// @param id identifier of the shopping cart
/// @param merch_name name of the merchandise to remove
/// @param quantity quantity of merchandise to remove
/// @return true if the merchandise was removed successfully, false otherwise
long checkout(store_t *store, size_t chart_id, char *merch_name, size_t quantity);


void cart_destroy(entry_t *entry);
cart_t *create_cart(store_t *store);