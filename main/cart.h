#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "../utilities/common.h"

/// @brief Creates a new shopping cart and adds it to the store.
/// @param store pointer to the store.
/// @return nothing.
void ioopm_add_cart(store_t *store);

/// @brief Adds a quantity of merchandise to a shopping cart.
/// @param store pointer to the store containing the merchandise.
/// @param cart_id identifier of the shopping cart.
/// @param merch_name name of the merchandise to add. The function takes ownership of this string.
/// @param quantity quantity of merchandise to add.
/// @return 1 on success, -1 if the merchandise does not exist, -2 if the cart does not exist, or -3 if there is insufficient stock.
int ioopm_add_to_cart(store_t *store, size_t cart_id, char *merch_name, size_t quantity);

/// @brief Removes a shopping cart and releases its merchandise reservations.
/// @param store pointer to the store containing the cart.
/// @param cart_id identifier of the shopping cart to remove.
/// @return true if the cart was removed successfully, false if it does not exist.
bool ioopm_remove_cart(store_t *store, size_t cart_id);

/// @brief Removes a quantity of merchandise from a shopping cart.
/// @param store pointer to the store containing the merchandise.
/// @param cart_id identifier of the shopping cart.
/// @param merch_name name of the merchandise to remove. The function takes ownership of this string.
/// @param quantity quantity of merchandise to remove.
/// @return 1 on success, -1 if the cart does not exist, -2 if the merchandise is not in the cart, or -3 if the quantity exceeds the amount in the cart.
int ioopm_remove_from_cart(store_t *store, size_t cart_id, char *merch_name, size_t quantity);

/// @brief Checks out a shopping cart, deducts its merchandise from stock, and removes the cart.
/// @param store pointer to the store containing the cart.
/// @param cart_id identifier of the shopping cart.
/// @return total cost of the purchase, or -1 if the cart does not exist.
long long ioopm_checkout(store_t *store, size_t cart_id);

/// @brief Calculates the total cost of a shopping cart without changing stock.
/// @param store pointer to the store containing the cart.
/// @param cart_id identifier of the shopping cart.
/// @return total cost of the cart, or -1 if the cart does not exist.
long long ioopm_calculate_cost(store_t *store, size_t cart_id);

/// @brief Looks up a shopping cart by its identifier.
/// @param store pointer to the store containing the cart.
/// @param cart_id identifier of the shopping cart.
/// @param lookup_res pointer where the cart entry will be stored.
/// @return true if the cart exists, false otherwise.
bool ioopm_get_cart(store_t *store, size_t cart_id, elem_t *lookup_res);

/// @brief Destroys a shopping cart and frees its memory.
/// @param cart pointer to the cart to destroy.
void ioopm_destroy_cart(cart_t *cart);

/// @brief Creates a new empty shopping cart.
/// @param store pointer to the store used to assign the cart identifier.
/// @return pointer to the newly created cart.
cart_t *ioopm_create_cart(store_t *store);

/// @brief Destroys a shopping cart stored in a hash table entry.
/// @param entry pointer to the hash table entry containing the cart.
void all_cart_destroyer(entry_t *entry);