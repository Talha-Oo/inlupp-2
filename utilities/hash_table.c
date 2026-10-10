#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "common_hash_table.h"
#include "hash_table.h"

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn, ioopm_destroy_function *elem_destroy_fn)
{
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));

  ht->hash_fn = hash_fn;
  ht->key_eq_fn = key_eq_fn;
  ht->elem_destroy_fn = elem_destroy_fn;
  ht->size = 0;

  return ht;
}

static void entry_destroy(ioopm_hash_table_t *ht, entry_t *entry)
{
  ht->elem_destroy_fn(entry);
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  entry_t *current_address;

  for (size_t i = 0; i < No_Buckets; i++)
  {
    current_address = ht->buckets[i].next; // The first entry in the bucket

    while (current_address != NULL) // While we have an entry in the bucket
    {
      entry_t *temp = current_address;
      current_address = current_address->next;

      entry_destroy(ht, temp); // free the entry
    }
  }

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

static entry_t *find_previous_entry(ioopm_hash_table_t *ht, const elem_t key)
{
  size_t bucket = ht->hash_fn(key) % No_Buckets; // Find bucket

  entry_t *previous = &ht->buckets[bucket];    // Get the sentinel node
  entry_t *current = ht->buckets[bucket].next; // Get the first logical node

  // While not at end of bucket, and key not found...
  while (current != NULL && !(ht->key_eq_fn(current->key, key)))
  {
    previous = current;
    current = current->next;
  }

  return previous;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  entry_t *previous = find_previous_entry(ht, key); // Find previous entry, or the last entry if the key does not exist

  // if the key exists, update the value, otherwise create a new entry
  if (previous->next != NULL)
  {
    previous->next->value = value;
  }
  else
  {
    previous->next = entry_create(key, value, NULL);
    ht->size++;
  }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, const elem_t key, elem_t *result)
{
  entry_t *previous = find_previous_entry(ht, key); // Find previous entry, or the last entry if the key does not exist
  entry_t *current = previous->next;

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
  entry_t *previous = find_previous_entry(ht, key); // Find previous entry, or the last entry if the key does not exist
  entry_t *current = previous->next;

  // If the key we are trying to remove exists...
  if (current != NULL)
  {
    // ...remove it and relink the previous node to the new next node
    *removed_value = current->value;
    previous->next = current->next;
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