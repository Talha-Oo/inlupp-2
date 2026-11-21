#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "../utilities/common.h"

/// @brief Creates a new store.
/// @return A pointer to the store
/// @note The caller must eventually call destroy_store().
store_t *ioopm_create_store(void);

/// @brief Deletes a store and frees its memory.
/// @param store Pointer to the store to delete.
/// @note The store owns its merchandise and shelf data.
void ioopm_destroy_store(store_t *store);

/// @brief Creates new merchandise.
/// @param name Name of the merchandise. Ownership is transferred.
/// @param desc Description of the merchandise. Ownership is transferred.
/// @param price Price of the merchandise.
/// @return A pointer to the new merchandise.
/// @note The strings are not copied. Use heap-allocated strings.
merch_t *ioopm_create_merch(char *name, char *desc, const size_t price);

/// @brief Deletes merchandise and frees its memory.
/// @param merch Pointer to the merchandise to delete.
/// @note Frees its name, description, shelf data, and list.
void ioopm_destroy_merch(merch_t *merch);

/// @brief Adds merchandise if its name does not already exist.
/// @param store Store where the merchandise should be added.
/// @param merch Merchandise to add.
/// @return true if added, false otherwise.
/// @note On success, the store owns merch. On failure, merch is destroyed.
bool ioopm_add_merch(store_t *store, merch_t *merch);

/// @brief Looks up a shelf by its name.
/// @param shelf_ht Hash table containing shelves.
/// @param shelf_name Name of the shelf to find.
/// @param lookup_res Where the result is stored.
/// @return true if found, false otherwise.
/// @note The returned shelf pointer is borrowed. Do not free it.
bool ioopm_get_shelf(store_t *store, char *shelf_name, elem_t *lookup_res);

/// @brief Looks up merchandise by its name.
/// @param merch_ht Hash table containing merchandise.
/// @param merch_name Name of the merchandise to find.
/// @param lookup_res Where the result is stored.
/// @return true if found, false otherwise.
/// @note The returned merchandise pointer is borrowed. Do not free it.
bool ioopm_get_merch(store_t *store, char *merch_name, elem_t *lookup_res);

/// @brief Adds stock to a shelf.
/// @param store Store containing the merchandise.
/// @param name Name of the merchandise.
/// @param quantity Quantity to add.
/// @param shelf_name Name of the shelf.
/// @return true if successful, false otherwise.
/// @note Takes ownership of shelf_name, even if it fails.
///Do not free or use shelf_name afterward.
bool ioopm_replenish(store_t *store, char *name, size_t quantity, char *shelf_name);

/// @brief Changes the name of merchandise.
/// @param store Store containing the merchandise.
/// @param merch Merchandise to rename.
/// @param new_name New name for the merchandise.
/// @return true if changed, false otherwise.
/// @note On success, the old name is freed and new_name is stored.
///       If the name already exists, new_name is freed.
bool edit_merch_name(store_t *store, merch_t *merch, char *new_name);

/// @brief Changes the description of merchandise.
/// @param merch Merchandise to update.
/// @param desc New description.
/// @note The old description is freed. The new string is stored without copying.
void edit_merch_desc(merch_t *merch, char *desc);

/// @brief Changes the price of merchandise.
/// @param merch Merchandise to update.
/// @param price New price.
void edit_merch_price(merch_t *merch, size_t price);

/// @brief Removes merchandise from the store.
/// @param store Store containing the merchandise.
/// @param name Name of the merchandise to remove.
/// @return true if removed, false otherwise.
bool ioopm_remove_merch(store_t *store, char *name);


// TILLÄGG VID GRANSKNING

/// @brief 
/// @param store 
/// @return 
ioopm_hash_table_t *ioopm_store_name_to_item_get(store_t *store); 

/// @brief 
/// @param store 
/// @return 
ioopm_hash_table_t *ioopm_store_shelf_to_item_get(store_t *store); 

/// @brief 
/// @param store 
/// @return 
ioopm_hash_table_t *ioopm_store_all_carts_get(store_t *store); 

/// @brief 
/// @param store 
/// @return 
ioopm_hash_table_t *ioopm_store_reserved_merchs_get(store_t *store); 

/// @brief 
/// @param store 
/// @return 
size_t ioopm_store_merch_num_get(store_t *store);

/// @brief 
/// @param store 
/// @return 
size_t ioopm_store_carts_num_get(store_t *store);

/// @brief 
/// @param merch 
/// @return 
char *ioopm_merch_name_get(merch_t *merch);

/// @brief 
/// @param merch 
/// @return 
char *ioopm_merch_desc_get(merch_t *merch);

/// @brief 
/// @param merch 
/// @return 
size_t ioopm_merch_price_get(merch_t *merch);

/// @brief 
/// @param merch 
/// @return 
ioopm_list_t *ioopm_merch_locs_get(merch_t *merch);

/// @brief 
/// @param merch 
/// @return 
size_t ioopm_merch_stock_get(merch_t *merch);

/// @brief 
/// @param shelf 
/// @return 
char *ioopm_shelf_name_get(shelf_t *shelf);

/// @brief 
/// @param shelf 
/// @return 
char *ioopm_shelf_merch_name_get(shelf_t *shelf);

/// @brief 
/// @param shelf 
/// @return 
size_t ioopm_shelf_quantity_get(shelf_t *shelf);