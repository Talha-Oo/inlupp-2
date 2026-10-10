#include "../utilities/common.h"

#include <string.h>

// Functions required to make a hashtable which uses string as key and elem_t as object
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

void destroy_merch(entry_t *entry)
{
  merch_t *merch = entry->value.p;
  
  free(merch->name);
  free(merch->desc);
  ioopm_list_destroy(merch->locs);
  free(merch);

  free(entry);
}

// Store functions
store_t *create_store(void)
{
  store_t *store = calloc(1, sizeof(store_t));

  store->name_to_item = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy_merch);
  store->shelf_to_item = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy_merch);

  return store;
}

void destroy_store(store_t *store)
{
  /// TODO: Free the list of every merchandise
  ioopm_hash_table_destroy(store->name_to_item);
  ioopm_hash_table_destroy(store->shelf_to_item);

  free(store);
}

merch_t *create_merch(char *name, char *desc, size_t price)
{
  merch_t *merch = calloc(1, sizeof(merch_t));

  merch->name = name;
  merch->desc = desc;
  merch->price = price;
  merch->locs = ioopm_list_create();

  return merch;
}
