#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "dp_common_hash_table.h"
#include "dp_hash_table.h"
#include "dp_hash_table_iterator.h"

struct hash_table_iterator
{
  ioopm_hash_table_t *ht;
  size_t current_bucket;
  entry_t *current_entry;
};

static void advance_iterator_state(ioopm_hash_table_iterator_t *it)
{
  // Go to the next entry
  it->current_entry = it->current_entry->next;

  // If at end of bucket...
  if (it->current_entry == NULL)
  {
    // ... Go to the next bucket.
    it->current_bucket += 1;

    // If not at the last bucket...
    if (it->current_bucket < it->ht->capacity)
    {
      it->current_entry = it->ht->buckets[it->current_bucket]; // ... Get the first element in the new bucket.
    }
  }
}

static void skip_empty_buckets(ioopm_hash_table_iterator_t *it)
{
  while (it->current_bucket < it->ht->capacity &&
         it->current_entry == NULL)
  {
    it->current_bucket++;

    if (it->current_bucket < it->ht->capacity)
    {
      it->current_entry = it->ht->buckets[it->current_bucket];
    }
  }
}

ioopm_hash_table_iterator_t *ioopm_hash_table_iterator_create(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_iterator_t *it = calloc(1, sizeof(ioopm_hash_table_iterator_t));

  it->ht = ht;
  it->current_bucket = 0;
  it->current_entry = ht->buckets[0];

  // Go to the first logical entry
  skip_empty_buckets(it);

  return it;
}

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it)
{
  free(it);
}

bool ioopm_hash_table_iterator_at_end(ioopm_hash_table_iterator_t *it)
{
  return it->current_bucket == it->ht->capacity;
}

void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it)
{
  assert(!ioopm_hash_table_iterator_at_end(it) && "iterator at end when advancing");

  advance_iterator_state(it);
  skip_empty_buckets(it);
}

elem_t ioopm_hash_table_iterator_current_key(ioopm_hash_table_iterator_t *it)
{
  return it->current_entry->key;
}

elem_t ioopm_hash_table_iterator_current_value(ioopm_hash_table_iterator_t *it)
{
  return it->current_entry->value;
}