#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "dp_common_hash_table.h"
#include "dp_hash_table.h"
#include "dp_hash_table_iterator.h"

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn, ioopm_destroy_function *fn_destroy_fn)
{
  return ioopm_hash_table_create_sized(hash_fn, key_eq_fn, fn_destroy_fn, 17, 0.75);
}

ioopm_hash_table_t *ioopm_hash_table_create_sized(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn, ioopm_destroy_function *fn_destroy_fn, size_t initial_capacity, float load_factor)
{
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));

  ht->hash_fn = hash_fn;
  ht->key_eq_fn = key_eq_fn;
  ht->fn_destroy_fn = fn_destroy_fn;
  ht->size = 0;
  ht->capacity = initial_capacity;
  ht->load_factor = load_factor;

  ht->buckets = calloc(initial_capacity, sizeof(entry_t *));

  return ht;
}

static void entry_destroy(ioopm_hash_table_t *ht, entry_t *entry)
{
  ht->fn_destroy_fn(entry);
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  entry_t *current_address;

  for (size_t i = 0; i < ht->capacity; i++)
  {
    current_address = ht->buckets[i]; // The first entry in the bucket

    while (current_address != NULL) // While we have an entry in the bucket
    {
      entry_t *temp = current_address;
      current_address = current_address->next;

      entry_destroy(ht, temp); // free the entry
    }
  }

  free(ht->buckets);
  free(ht); // Free the space for the hash table itself
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *entry = calloc(1, sizeof(entry_t));

  entry->key = key;
  entry->value = value;
  entry->next = next;

  return entry;
}

static entry_t **find_previous_entry(ioopm_hash_table_t *ht, const elem_t key)
{
  size_t bucket = ht->hash_fn(key) % ht->capacity; // Find bucket

  entry_t **previous = &ht->buckets[bucket];

  // While not at end of bucket, and key not found...
  while (*previous != NULL && !(ht->key_eq_fn((*previous)->key, key)))
  {
    previous = &(*previous)->next;
  }

  return previous;
}

static size_t get_new_capacity(ioopm_hash_table_t *ht)
{
  // 0 at the end acts as an array terminator
  size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381, 0};

  // While we have not reached the end of the primes array...
  for (int i = 0; primes[i] != 0; i++)
  {
    if (primes[i] == ht->capacity) // If we've found the current capacity of ht...
    {
      return primes[i + 1]; // ...return the next capacity.
    }
  }
  return 0; // If we did not find the current capacity of ht in the primes array, return 0.
}

static void resize_hash_table(ioopm_hash_table_t *ht)
{
  size_t new_capacity = get_new_capacity(ht);
  // Exit if there is no new capacity.
  if (new_capacity == 0)
  {
    return;
  }

  ioopm_hash_table_t *temp_ht = ioopm_hash_table_create_sized(
      ht->hash_fn, ht->key_eq_fn, ht->fn_destroy_fn,
      new_capacity, ht->load_factor);

  if (temp_ht == NULL)
  {
    return;
  }

  // Rehash from old buckets to new buckets
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    // Get key and value from old bucket
    elem_t key = ioopm_hash_table_iterator_current_key(it);
    elem_t value = ioopm_hash_table_iterator_current_value(it);

    // Find new bucket
    size_t bucket = temp_ht->hash_fn(key) % temp_ht->capacity;

    // Find end of new bucket
    entry_t **current = &temp_ht->buckets[bucket];
    while (*current != NULL)
    {
      current = &(*current)->next;
    }

    // Put entry at end of new bucket
    *current = entry_create(key, value, NULL);
    temp_ht->size++;

    // Go to next entry
    ioopm_hash_table_iterator_advance(it);
  }

  ioopm_hash_table_iterator_destroy(it);

  // Save old hash table information
  entry_t **old_buckets = ht->buckets;
  size_t old_capacity = ht->capacity;

  // Update hash table with new information
  ht->buckets = temp_ht->buckets;
  ht->capacity = temp_ht->capacity;
  ht->size = temp_ht->size;

  // Write old information to temporary hash table
  temp_ht->buckets = old_buckets;
  temp_ht->capacity = old_capacity;

  // Destroy the old entries without destroying their keys or values
  for (size_t i = 0; i < temp_ht->capacity; i++)
  {
    entry_t *current_entry = temp_ht->buckets[i];

    while (current_entry != NULL)
    {
      entry_t *previous_entry = current_entry;
      current_entry = current_entry->next;

      free(previous_entry);
      // (Do not free the entry's key or value, as they are being used by the new buckets)
    }
  }

  free(temp_ht->buckets);
  free(temp_ht);
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  entry_t **previous = find_previous_entry(ht, key);
  entry_t *to_insert = *previous;

  // If the key exists, update the value; otherwise, create a new entry
  if (to_insert != NULL)
  {
    to_insert->value = value;
  }
  else
  {
    // Check if adding one more entry would exceed the load factor
    if ((double)(ht->size + 1) / ht->capacity > ht->load_factor)
    {
      resize_hash_table(ht);

      // The bucket may have changed, so find the insertion position again
      previous = find_previous_entry(ht, key);
    }

    *previous = entry_create(key, value, NULL);
    ht->size++;
  }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, const elem_t key, elem_t *result)
{
  entry_t **previous = find_previous_entry(ht, key); // Find previous entry, or the last entry if the key does not exist
  entry_t *current = *previous;

  // if the key exists, return the value, otherwise, indicate that the lookup failed
  if (current != NULL)
  {
    *result = current->value;
    return true;
  }
  else
  {
    return false;
  }
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, const elem_t key)
{
  elem_t lookedup_value;
  return ioopm_hash_table_lookup(ht, key, &lookedup_value);
}

size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht)
{
  return ht->size;
}

bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht)
{
  return ioopm_hash_table_size(ht) == 0;
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, const elem_t key, elem_t *removed_value)
{
  entry_t **previous = find_previous_entry(ht, key); // Find previous entry, or the last entry if the key does not exist
  entry_t *current = *previous;

  // If the key we are trying to remove exists...
  if (current != NULL)
  {
    // ...remove it and relink the previous node to the new next node
    *removed_value = current->value;
    (*previous) = current->next;
    entry_destroy(ht, current);
    ht->size--;

    return true;
  }
  else
  {
    // ...return that the lookup failed
    return false;
  }
}

bool ioopm_hash_table_remove_without_destroy(ioopm_hash_table_t *ht, const elem_t key, elem_t *removed_value)
{
  entry_t **previous = find_previous_entry(ht, key); // Find previous entry, or the last entry if the key does not exist
  entry_t *current = *previous;

  // If the key we are trying to remove exists...
  if (current != NULL && ht->key_eq_fn(current->key, key))
  {
    // ...remove it and relink the previous node to the new next node
    *removed_value = current->value;
    *previous = current->next;

    free(current);
    ht->size--;

    return true;
  }
  else
  {
    // ...return that the lookup failed
    return false;
  }
}