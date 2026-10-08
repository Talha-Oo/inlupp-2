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

/// @brief adds at least one item of a merchandise
/// @param store the store the merchendise is in
/// @param name the name to be edited
/// @param quantity the number of items to add
/// @pre merchandise has to be in store
/// @pre quantity is at least one 
/// @param name the name of the merchendise 
bool replenish(store_t *store, char* name, int quantity);