#include "../utilities/common.h"

#include <string.h>
#include <stdbool.h>

// Functions required to make a hashtable which uses string as key and elem_t as object
void destroy_merch(merch_t *merch)
{
  free(merch->name);
  free(merch->desc);
  ioopm_list_destroy(merch->locs);

  free(merch);
}

static void name_to_item_destroy(entry_t *entry)
{
  destroy_merch(entry->value.p);
  free(entry);
}

static size_t string_knr_hash(const elem_t elem)
{
  size_t result = 2;

  char *str = elem.s;
  while (*str != '\0')
  {
    result = result * 31 + ((unsigned char)*str);
    str++;
  }

  return result;
}

static bool str_eq_func(const elem_t elem1, const elem_t elem2)
{
  return strcmp(elem1.s, elem2.s) == 0;
}

// Store functions
store_t *create_store(void)
{
  store_t *store = calloc(1, sizeof(store_t));

  store->name_to_item = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &name_to_item_destroy);
  store->shelf_to_item = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &name_to_item_destroy);

  return store;
}

void destroy_store(store_t *store)
{
  /// TODO: Free the list of every merchandise
  ioopm_hash_table_destroy(store->name_to_item);
  ioopm_hash_table_destroy(store->shelf_to_item);

  free(store);
}

merch_t *create_merch(char *name, char *desc, const size_t price)
{
  merch_t *merch = calloc(1, sizeof(merch_t));

  merch->name = name;
  merch->desc = desc;
  merch->price = price;
  merch->locs = ioopm_list_create();

  return merch;
}

bool add_merch(store_t *store, merch_t *merch)
{
  ioopm_hash_table_t *ht = store->name_to_item;
  elem_t key = string_elem(merch->name);
  elem_t value = ptr_elem(merch);

  elem_t lookedup_value;
  bool result = ioopm_hash_table_lookup(ht, key, &lookedup_value);

  if (!result)
  {
    ioopm_hash_table_insert(ht, key, value);
    return true;
  }
  else
  {
    destroy_merch(merch);
    return false;
  }
}