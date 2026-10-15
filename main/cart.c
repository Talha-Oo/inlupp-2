#include "../utilities/common.h"
#include "db.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

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

void cart_destroy(entry_t *entry)
{
  // TODO: IMPLEMENT, MAYBE??

  free(entry);
}

cart_t *create_cart(store_t *store)
{
  cart_t *cart = calloc(1, sizeof(cart_t));
  cart->merchandise = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &cart_destroy);

  store->carts_num++;
  cart->id = store->carts_num;

  return cart;
}

bool get_cart(store_t *store, size_t cart_id, elem_t *looked_elem)
{
  ioopm_hash_table_t *ht = store->all_carts;
  return ioopm_hash_table_lookup(ht, int_elem(cart_id), looked_elem);
}

void add_cart(store_t *store)
{

  cart_t *cart = create_cart(store);

  ioopm_hash_table_t *all_cart = store->all_carts;
  elem_t cart_elem = ptr_elem(cart);
  elem_t key_elem = int_elem(cart->id);

  ioopm_hash_table_insert(all_cart, key_elem, cart_elem);
}

bool remove_cart(store_t *store, size_t chart_id)
{
  elem_t current_cart;
  elem_t old_quantity;
  elem_t current_key = int_elem(chart_id);

  ioopm_hash_table_t *all_carts = store->all_carts;
  ioopm_hash_table_t *reserved_merchs = store->reserved_merchs;

  bool status = ioopm_hash_table_lookup(all_carts, current_key, &current_cart);
  if (!status)
  {
    return false;
  }

  cart_t *cart = current_cart.p;

  ioopm_hash_table_t *merchandise = cart->merchandise;
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(merchandise);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    elem_t key = ioopm_hash_table_iterator_current_key(it);
    elem_t value = ioopm_hash_table_iterator_current_value(it);
    bool status = ioopm_hash_table_lookup(reserved_merchs, key, &old_quantity);
    if (!status)
    {
      return false;
    }
    old_quantity.i = old_quantity.i - value.i;
    ioopm_hash_table_insert(reserved_merchs, key, old_quantity);
    ioopm_hash_table_iterator_advance(it);
  }
  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(merchandise);
  return true;
}

bool add_to_cart(store_t *store, size_t chart_id, char *merch_name, size_t quantity)
{

  elem_t merch_name_elem = string_elem(merch_name);
  elem_t merch_elem;
  elem_t cart_elem;
  if (!get_merch(store, merch_name, &merch_elem) || !get_cart(store, chart_id, &cart_elem))
  {
    return false;
  }

  cart_t *cart = cart_elem.p;
  merch_t *merch = merch_elem.p;
  size_t merch_stock = merch->stock;

  ioopm_hash_table_t *merchs_in_cart = cart->merchandise;
  ioopm_hash_table_t *reserved_merchs = store->reserved_merchs;

  elem_t reserved_merch_elem;
  size_t reserved_merch = 0;
  if (ioopm_hash_table_lookup(reserved_merchs, merch_name_elem, &reserved_merch_elem))
  {
    reserved_merch = reserved_merch_elem.i;
  }

  elem_t old_quantity;
  if (ioopm_hash_table_lookup(merchs_in_cart, merch_name_elem, &old_quantity))
  {

    size_t merch_in_cart = old_quantity.i;

    if (merch_stock < (merch_in_cart + quantity + reserved_merch))
    {
      return false;
    }

    elem_t upd_quantity = int_elem(merch_in_cart + quantity);
    elem_t upd_reserved_merch = int_elem(reserved_merch + quantity);

    ioopm_hash_table_insert(reserved_merchs, merch_name_elem, upd_reserved_merch);
    ioopm_hash_table_insert(merchs_in_cart, merch_name_elem, upd_quantity);

    return true;
  }
  else
  {
    if (merch_stock < (quantity + reserved_merch))
    {
      return false;
    }

    elem_t upd_quantity = int_elem(quantity);
    elem_t upd_reserved_merch = int_elem(reserved_merch + quantity);

    ioopm_hash_table_insert(reserved_merchs, merch_name_elem, upd_reserved_merch);
    ioopm_hash_table_insert(merchs_in_cart, merch_name_elem, upd_quantity);

    return true;
  }
}