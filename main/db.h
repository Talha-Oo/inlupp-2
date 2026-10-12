#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utilities/common.h"

/// @brief Creates and initializes a new store.
/// @return a pointer to the newly created store.
store_t *create_store(void);

/// @brief Delete a store and free its memory
/// @param store a hash table to be deleted
void destroy_store(store_t *store);

/// TODO: Documentation


/// @brief d
/// @param name sds
/// @param desc sd
/// @param price sd
/// @return  ss
merch_t *create_merch(char *name, char *desc, const size_t price);

void destroy_merch(merch_t *merch);

/// @brief Adds merchandise to the store if an item with the same name does not already exist
/// @param store pointer to the store where the merchandise should be added
/// @param merch pointer to the merchandise to add
/// @return true if the merchandise was added successfully, false otherwise
bool add_merch(store_t *store, merch_t *merch);

bool get_shelf(ioopm_hash_table_t *shelf_ht, elem_t shelf_name, elem_t *lookup_res);

bool get_merch(ioopm_hash_table_t *merch_ht, elem_t merch_name, elem_t *lookup_res);

/// @brief Adds stock of merchandise to a specified shelf
/// @param store pointer to the store containing the merchandise
/// @param name name of the merchandise to replenish
/// @param quantity quantity of merchandise to add
/// @param id identifier of the shelf where the merchandise should be stored
/// @pre the quantity must be at least one
/// @return true if the stock was replenished successfully, false otherwise
bool replenish(store_t *store, char *name, int quantity, char *shelf_id);

bool edit_merch_name(store_t *store, merch_t *merch, char *new_name);

/// @brief Changes the description of existing merchandise
/// @param merch pointer to the merchandise whose description should be changed
/// @param desc the new description for the merchandise
/// @return true if the description was changed successfully, false otherwise
void edit_merch_desc(merch_t *merch, char *desc);

/// @brief Changes the price of existing merchandise
/// @param merch pointer to the merchandise whose price should be changed
/// @param price the new price for the merchandise
/// @return true if the price was changed successfully, false otherwise
void edit_merch_price(merch_t *merch, size_t price);

/// @brief Removes merchandise from the store
/// @param store pointer to the store containing the merchandise
/// @param name name of the merchandise to remove
/// @return true if the merchandise was removed successfully, false otherwise
bool remove_merch(store_t *store, char *name);

/// @brief Changes the name of existing merchandise
/// @param store pointer to the store containing the merchandise
/// @param merch pointer to the merchandise whose name should be changed
/// @param name the new name for the merchandise
/// @return true if the name was changed successfully, false otherwise

/// @brief Prints the merchandise in the store, 20 items at a time
/// @param store pointer to the store containing the merchandise
/// @param n Starting position of the merchandise batch to print
/// @return true if there is no more merchandise to print, false if more remains
bool list_merch(store_t *store, size_t n);

/// @brief Prints all shelves on which the specified merchandise is stored
/// @param store pointer to the store containing the merchandise
/// @param name name of the merchandise whose stock should be displayed
/// @return true if the stock information was displayed successfully, false otherwise
bool show_stock(store_t *store, char *name);

//////////////////////// TODO: THE CART SYSTEM, REWRITE THE DEFINATION + ADD NEW ////////////////////////

// /// @brief Creates a container for all shopping carts
// /// @return pointer to the container of shopping carts, or NULL if creation fails
// all_shopping_carts_t *create_cart_holder(void);

// /// @brief Creates an empty shopping cart
// /// @return pointer to the new shopping cart, or NULL if creation fails
// shopping_cart_t *create_cart_(void);

// /// @brief Removes a shopping cart from the container
// /// @param all_carts pointer to the container holding all shopping carts
// /// @param id identifier of the shopping cart to remove
// /// @return true if the cart was removed successfully, false otherwise
// bool remove_cart(all_shopping_carts_t *all_carts, size_t id);

// /// @brief Adds a quantity of merchandise to a shopping cart
// /// @param store pointer to the store containing the merchandise
// /// @param all_carts pointer to the container holding all shopping carts
// /// @param id identifier of the shopping cart to which merchandise should be added
// /// @param merch name of the merchandise to add
// /// @param quantity quantity of merchandise to add
// /// @return true if the merchandise was added successfully, false otherwise
// bool add_to_cart(store_t *store, all_shopping_carts_t *all_carts, size_t id, char *merch, size_t quantity);
