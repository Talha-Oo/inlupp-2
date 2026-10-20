#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "common_hash_table.h"

/**
 * @file hash_table.h
 * @author Marcus & Talha
 * @date write the date you started working on this
 * @brief Simple hash table that maps keys to values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 */

typedef struct hash_table ioopm_hash_table_t;

/// @brief Create a new hash table
/// @param hash_fn pointer to the hash function
/// @param key_eq_fn pointer to the function which compares keys
/// @param elem_destroy_fn pointer to the function which destorys the elements of the hashtable
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn, ioopm_destroy_function *elem_destroy_fn);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @param result pointer to where the value should be written
/// @return the value mapped to by key
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, const elem_t key, elem_t *result);

/// @brief check if the key in hash table ht exists
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return true if the keys exists and false if it doesn't
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, const elem_t key);

/// @brief get the size of the hash table ht
/// @param ht hash table operated upon
/// @return size of the hash table
size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht);

/// @brief check if the hash table ht is empty
/// @param ht hash table operated upon
/// @return true if it's empty and false if it isn't
bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return the value mapped to by key
bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, const elem_t key, elem_t *removed_value);

bool ioopm_hash_table_remove_without_destroy(ioopm_hash_table_t *ht, const elem_t key, elem_t *removed_value);