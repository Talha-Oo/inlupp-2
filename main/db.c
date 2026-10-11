#include "../utilities/common.h"

#include <string.h>
#include <stdbool.h>

// Functions required to make a hashtable which uses string as key and elem_t as object and destory funcs
void destory_shelves(ioopm_list_t *list)
{
  ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);

  elem_t curr_merch;
  shelf_t *shelf;
  while (!ioopm_list_iterator_at_end(it))
  {
    ioopm_list_iterator_current(it, &curr_merch);
    shelf = curr_merch.p;
    free(shelf->shelf_name);
    free(shelf);

    ioopm_list_iterator_advance(it);
  }

    ioopm_list_iterator_destroy(it);
}

void destroy_merch(merch_t *merch)
{
  ioopm_list_t *list = merch->locs;

  destory_shelves(list);
  ioopm_list_destroy(merch->locs);

  free(merch->name);
  free(merch->desc);

  free(merch);
}

static void name_to_item_destroy(entry_t *entry)
{
  destroy_merch(entry->value.p);
  free(entry);
}

static void shelf_to_item_destroy(entry_t *entry)
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

// Store functions
store_t *create_store(void)
{
  store_t *store = calloc(1, sizeof(store_t));

  store->name_to_item = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &name_to_item_destroy);
  store->shelf_to_item = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &shelf_to_item_destroy);
  store->merch_num = 0;

  return store;
}

void destroy_store(store_t *store)
{
  ioopm_hash_table_destroy(store->shelf_to_item);
  ioopm_hash_table_destroy(store->name_to_item);
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
    return true;
  }
  else
  {
    destroy_merch(merch);
    return false;
  }
}

static shelf_t *create_shelf(char *shelf_name, char *merch_name, size_t quantity)
{
  shelf_t *shelf = calloc(1, sizeof(shelf_t));

  shelf->merch_name = merch_name;
  shelf->shelf_name = shelf_name;
  shelf->quantity = quantity;

  return shelf;
}

bool get_shelf(ioopm_hash_table_t *shelf_ht, elem_t shelf_name, elem_t *lookup_res) {
  return ioopm_hash_table_lookup(shelf_ht, shelf_name, lookup_res);
}

bool get_merch(ioopm_hash_table_t *merch_ht, elem_t merch_name, elem_t *lookup_res) {
  return ioopm_hash_table_lookup(merch_ht, merch_name, lookup_res);
}

static bool update_shelf(elem_t shelf_elem, merch_t *merch, size_t quantity)
{
  shelf_t *shelf = shelf_elem.p;

  // if the merch stored in the shelf is the same we are addding
  if (strcmp(shelf->merch_name, merch->name) == 0)
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
  ioopm_list_t *merch_shelves_list = merch->locs;

  shelf_t *shelf = create_shelf(shelf_name.s, merch->name, quantity);

  elem_t shelf_elem = ptr_elem(shelf);

  ioopm_list_prepend(merch_shelves_list, shelf_elem);
  ioopm_hash_table_insert(ht, shelf_name, shelf_elem);
  merch->stock += quantity;
}

bool replenish(store_t *store, char *name, size_t quantity, char *shelf_id)
{
  elem_t merch_name = string_elem(name);
  elem_t shelf_name = string_elem(shelf_id);

  ioopm_hash_table_t *name_ht = store->name_to_item;
  ioopm_hash_table_t *shelf_ht = store->shelf_to_item;

  elem_t looked_up_elem_merch;
  if (get_merch(name_ht, merch_name, &looked_up_elem_merch))
  {

    elem_t looked_up_elem_shelf;
    if (get_shelf(shelf_ht, shelf_name, &looked_up_elem_shelf)) 
    {
      // Shelf exits
      merch_t *merch = looked_up_elem_merch.p;
      free(shelf_id);
      return update_shelf(looked_up_elem_shelf, merch, quantity);
    }
    else
    {
      // shelf doesn't exist
      add_new_shelf(shelf_ht, looked_up_elem_merch, shelf_name, quantity);
      return true;
      
    }
  }
  else
  {
    free(shelf_id);
    return false;
  }
}

// bool edit_merch_name(store_t *store, merch_t *merch, char *name)
// {
// }

// bool edit_merch_desc(merch_t *merch, char *desc);

// bool edit_merch_price(merch_t *merch, size_t price);