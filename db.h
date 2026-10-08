#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"


/// @brief Adds the merchandise to the store
/// @param store the pointer to the store the merchandise should be added to
/// @param merch the pointer to the merchendise that should be added
bool add_merch(store_t *store, merch_t *merch);

/// @brief prints all of the merchandise in the store, 20 at a time and then asks the user to continue
/// @param store the store which contains the merchandise that is to be printed
bool list_merch(store_t *store);

/// @brief removes the merchandise from the store
/// @param store the store the merchandise is in
/// @param name the name of the merchandise to be removed
/// @pre merchandise has to be in the store
/// @return true if the merchandise was removed sucessfully otherwise returns false
bool remove_merch(store_t *store, char* name);

/// @brief edits the merchandise if it exists
/// @param store the store the merchandise is in
/// @param name the name of the merchandise to be edited
/// @pre merchandise has to be in the store
/// @return true if the merchandise was edited sucessfully otherwise returns false
bool edit_merch(store_t *store, char* name);

/// @brief prints all of the shelves the merchandise is stored on
/// @param store the store the merchandise is in
/// @param name the name to be edited
/// @pre merchandise has to be in store
/// @param name the name of the merchandise 
bool show_stock(store_t *store, char* name);

/// @brief adds at least one item of a merchandise on one of the shelf 
/// @param store the store the merchendise is in
/// @param name the name to be edited
/// @param quantity the number of items to add
/// @pre merchandise has to be in store
/// @pre quantity is at least one 
/// @param name the name of the merchendise 
bool replenish(store_t *store, char* name, int quantity);

/// @brief creates a place for all of the shoping carts
/// @return a pointer to the linked list holding the shoping cart
all_shopping_carts_t *create_cart_holder();

/// @brief creates a shoping carts
/// @return a pointer to the shopingcart
all_shopping_carts_t *create_cart_();

/// @brief removes the shopping cart
/// @param store linked list containing all of the carts
/// @param id id of the shopping cart to be removed
/// @return true if the shopping cart was removed sucessfully otherwise returns false
bool remove_cart(all_shopping_carts_t *all_carts, size_t id);

/// @brief adds x amount of merchandise to the cart 
/// @param store pointer to the store the merchandise is in 
/// @param all_carts pointer to the linked list all carts are in
/// @param merch name of the merchandise to be added
/// @return true if the merchandise was added sucessfully to the cart otherwise returns false
bool add_to_cart(store_t *store, all_shopping_carts_t *all_carts, size_t id, char *merch);