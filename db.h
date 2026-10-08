#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"


/// @brief Adds the merchendise to the store
/// @param store the pointer to the store the merchendise should be added to
/// @param merch the pointer to the merchendise that should be added
bool add_merch(store_t *store, merch_t *merch);

/// @brief prints all of the merchendise in the store, 20 at a time and then asks the user to continue
/// @param store the store which contains the merchendises to be printed
bool list_merch(store_t *store);

/// @brief removes the merchendise from the store
/// @param store the store the merchendise is in
/// @param name the name of the merchendise to be removed
/// @return true if the merchendise was removed sucessfully otherwise returns false
bool remove_merch(store_t *store, char* name);

/// @brief edits the merchendise if it exists
/// @param store the store the merchendise is in
/// @param name the name of the merchendise to be edited
/// @return true if the merchendise was edited sucessfully otherwise returns false
bool edit_merch(store_t *store, char* name);

/// @brief prints all of the shelves the merchendise is stored on
/// @param store the store the merchendise is in
/// @param name the name of the merchendise 
bool show_stock(store_t *store, char* name);

