#include "../utilities/common.h"
#include "extra_cart.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

// Functions required to make a hashtable which uses string as key and elem_t as object and destroy funcs
void destroy_merch_shelves(ioopm_list_t *list)
{
  ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);

  elem_t shelf_elem;
  shelf_t *shelf;
  while (!ioopm_list_iterator_at_end(it))
  {
    ioopm_list_iterator_current(it, &shelf_elem);
    shelf = shelf_elem.p;

    free(shelf->shelf_name);
    free(shelf);

    ioopm_list_iterator_advance(it);
  }

  ioopm_list_iterator_destroy(it);
  ioopm_list_destroy(list);
}

void remove_merch_shelf_ht(store_t *store, ioopm_list_t *list)
{
  ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);
  ioopm_hash_table_t *ht = store->shelf_to_item;

  elem_t shelf_elem;
  shelf_t *shelf;

  char *name;
  elem_t dummy;
  while (!ioopm_list_iterator_at_end(it))
  {
    ioopm_list_iterator_current(it, &shelf_elem);
    shelf = shelf_elem.p;
    name = shelf->shelf_name;

    ioopm_hash_table_remove(ht, string_elem(name), &dummy);
    ioopm_list_iterator_advance(it);
  }

  ioopm_list_iterator_destroy(it);
}

void destroy_merch(merch_t *merch)
{
  ioopm_list_t *list = merch->locs;

  destroy_merch_shelves(list);

  free(merch->name);
  free(merch->desc);

  free(merch);
}

static void name_to_item_destroy(entry_t *entry)
{
  destroy_merch(entry->value.p);
  free(entry);
}

static void simple_entry_destroy(entry_t *entry)
{
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

static size_t int_hash(const elem_t elem)
{
  return elem.i;
}

static bool int_eq_func(const elem_t elem1, const elem_t elem2)
{
  return elem1.i == elem2.i;
}


// Store functions
store_t *create_store(void)
{
  store_t *store = calloc(1, sizeof(store_t));

  store->name_to_item = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &name_to_item_destroy);
  store->shelf_to_item = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &simple_entry_destroy);

  // TODO: Make the destroy func for all_carts hashtable
  store->all_carts = ioopm_hash_table_create(&int_hash, &int_eq_func, &all_cart_destroyer);
  store->reserved_merchs = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &simple_entry_destroy);
  store->merch_num = 0;

  return store;
}

void destroy_store(store_t *store)
{
  ioopm_hash_table_destroy(store->shelf_to_item);
  ioopm_hash_table_destroy(store->all_carts);
  ioopm_hash_table_destroy(store->name_to_item);
  ioopm_hash_table_destroy(store->reserved_merchs);
  free(store);
}

merch_t *create_merch(char *name, char *desc, const size_t price)
{
  merch_t *merch = calloc(1, sizeof(merch_t));

  merch->name = name;
  merch->desc = desc;
  merch->price = price;
  merch->stock = 0;
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
    store->merch_num++;
    return true;
  }
  else
  {
    destroy_merch(merch);
    return false;
  }
}

static shelf_t *create_shelf(char *shelf_name, char **merch_name, size_t quantity)
{
  shelf_t *shelf = calloc(1, sizeof(shelf_t));

  shelf->merch_name = merch_name;
  shelf->shelf_name = shelf_name;
  shelf->quantity = quantity;

  return shelf;
}

bool get_shelf(store_t *store, char *shelf_name, elem_t *lookup_res)
{
  elem_t elem_name = string_elem(shelf_name);
  return ioopm_hash_table_lookup(store->shelf_to_item, elem_name, lookup_res);
}

bool get_merch(store_t *store, char *merch_name, elem_t *lookup_res)
{
  elem_t elem_name = string_elem(merch_name);
  bool res = ioopm_hash_table_lookup(store->name_to_item, elem_name, lookup_res);

  return res;
}

static bool update_shelf(elem_t shelf_elem, merch_t *merch, size_t quantity)
{
  shelf_t *shelf = shelf_elem.p;

  // if the merch stored in the shelf is the same we are addding
  if (strcmp(*(shelf->merch_name), merch->name) == 0)
  {
    shelf->quantity += quantity;
    merch->stock += quantity;
    return true;
  }
  else
  {
    // trying to store a new type of merch on a shelf which already stores a merch
    return false;
  }
}

static void add_new_shelf(ioopm_hash_table_t *ht, elem_t merch_elem, elem_t shelf_name, size_t quantity)
{
  merch_t *merch = merch_elem.p;
  ioopm_list_t *shelves_list = merch->locs;

  shelf_t *shelf = create_shelf(shelf_name.s, &(merch->name), quantity);
  elem_t shelf_elem = ptr_elem(shelf);

  ioopm_list_prepend(shelves_list, shelf_elem);
  ioopm_hash_table_insert(ht, shelf_name, shelf_elem);

  merch->stock += quantity;
}

bool replenish(store_t *store, char *name, size_t quantity, char *shelf_name)
{
  elem_t shelf_name_elem = string_elem(shelf_name);

  ioopm_hash_table_t *shelf_ht = store->shelf_to_item;

  elem_t looked_up_elem_merch;
  if (get_merch(store, name, &looked_up_elem_merch))
  {
    free(name);

    elem_t looked_up_elem_shelf;
    if (get_shelf(store, shelf_name, &looked_up_elem_shelf))
    {
      // Shelf exits
      merch_t *merch = looked_up_elem_merch.p;

      free(shelf_name);
      return update_shelf(looked_up_elem_shelf, merch, quantity);
    }
    else
    {
      // shelf doesn't exist
      add_new_shelf(shelf_ht, looked_up_elem_merch, shelf_name_elem, quantity);
      return true;
    }
  }
  else
  {
    free(name);
    free(shelf_name);
    return false;
  }
}

/// TODO: Maybe support char *merch_name instead of merch_t *merch for edit functions
void edit_merch_desc(merch_t *merch, char *desc)
{
  free(merch->desc);
  merch->desc = desc;
}

void edit_merch_price(merch_t *merch, size_t price)
{
  merch->price = price;
}

bool edit_merch_name(store_t *store, merch_t *merch, char *new_name)
{
  ioopm_hash_table_t *name_to_item = store->name_to_item;

  elem_t looked_up_merch_elem;
  elem_t merch_elem;

  // if a merch with this new_name doesn't exist
  if (!get_merch(store, new_name, &looked_up_merch_elem))
  {
    elem_t old_name = string_elem(merch->name);

    // remove item without freeing its memory
    ioopm_hash_table_remove_without_destroy(name_to_item, old_name, &merch_elem);

    free(merch->name); // free old name
    merch->name = new_name;

    ioopm_hash_table_insert(name_to_item, string_elem(new_name), merch_elem);
    return true;
  }
  else // if a merch with the new_name exists
  {
    free(new_name);
    return false;
  }
}

bool remove_merch(store_t *store, char *name)
{
  elem_t merch_elem;
  bool res = get_merch(store, name, &merch_elem);
  merch_t *merch = merch_elem.p;

  if (!res)
  {
    //free(name);
    return res;
  }

  remove_merch_shelf_ht(store, merch->locs);
  res = ioopm_hash_table_remove(store->name_to_item, string_elem(name), &merch_elem);

  if(res)
  {
    store->merch_num--;
  }

  //free(name);
  return res;
}
