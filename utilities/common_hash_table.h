#pragma once

#define int_elem(x) ((elem_t){.i = (x)})
#define bool_elem(x) ((elem_t){.b = (x)})
#define ptr_elem(x) ((elem_t){.p = (x)})
#define string_elem(x) ((elem_t){.s = (x)})

#include <stdbool.h>
#include <stdlib.h>
#include "common_elem.h"

#define No_Buckets 17

typedef struct entry entry_t;

struct entry
{
  elem_t key;   // holds the key
  elem_t value;  // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

typedef bool ioopm_eq_function(elem_t a, elem_t b);
typedef size_t ioopm_hash_function(elem_t key);
typedef void ioopm_destroy_function(entry_t *elem);

struct hash_table
{
  entry_t buckets[No_Buckets];
  ioopm_hash_function *hash_fn;
  ioopm_eq_function *key_eq_fn;
  ioopm_destroy_function *elem_destroy_fn;
  size_t size;
};