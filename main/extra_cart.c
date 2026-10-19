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

void ioopm_destroy_cart(cart_t *cart)
{
  ioopm_hash_table_t *ht = cart->merchandise;
  ioopm_hash_table_destroy(ht);

  free(cart);
}

void merchandise_destroyer(entry_t *entry)
{
  free(entry);
}

void all_cart_destroyer(entry_t *entry)
{
  // TODO: IMPLEMENT, MAYBE??
  ioopm_destroy_cart(entry->value.p);
  free(entry);
}

cart_t *ioopm_create_cart(store_t *store)
{
  cart_t *cart = calloc(1, sizeof(cart_t));
  cart->merchandise = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &merchandise_destroyer);

  store->carts_num++;
  cart->id = store->carts_num;

  return cart;
}

bool get_cart(store_t *store, size_t cart_id, elem_t *lookup_res)
{
  elem_t key = int_elem(cart_id);
  bool res = ioopm_hash_table_lookup(store->all_carts, key, lookup_res);

  return res;
}

void ioopm_add_cart(store_t *store)
{
  cart_t *cart = ioopm_create_cart(store);

  ioopm_hash_table_t *ht = store->all_carts;
  elem_t cart_elem = ptr_elem(cart);
  elem_t key = int_elem(cart->id);

  ioopm_hash_table_insert(ht, key, cart_elem);
}

bool remove_cart(store_t *store, size_t cart_id)
{
  ioopm_hash_table_t *ht = store->all_carts;

  elem_t key = int_elem(cart_id);
  elem_t cart_elem;
  if (!ioopm_hash_table_lookup(ht, key, &cart_elem))
  {
    return false;
  }

  return ioopm_hash_table_remove(ht, key, &cart_elem);
}

bool possible_to_reserve(size_t quantity, size_t reserved, size_t stock)
{
  return (quantity + reserved) <= stock ? true : false;
}

bool reserve_merch(store_t *store, merch_t *merch, size_t quantity)
{
  ioopm_hash_table_t *reserved_merch = store->reserved_merchs;
  elem_t merch_name = string_elem(merch->name);

  elem_t reserved_num_elem;
  if (!ioopm_hash_table_lookup(reserved_merch, merch_name, &reserved_num_elem))
  {
    if (possible_to_reserve(quantity, 0, merch->stock))
    {
      ioopm_hash_table_insert(reserved_merch, merch_name, int_elem(quantity));
      return true;
    }

    return false;
  }
  else
  {
    size_t reserved_num = reserved_num_elem.i;
    if (possible_to_reserve(quantity, reserved_num, merch->stock))
    {
      ioopm_hash_table_insert(reserved_merch, merch_name, int_elem(quantity + reserved_num));
      return true;
    }

    return false;
  }
}

void update_cart(cart_t *cart, merch_t *merch, size_t quantity)
{
  ioopm_hash_table_t *merchs_in_cart = cart->merchandise;
  elem_t key = string_elem(merch->name);

  elem_t in_cart_elem;
  if (ioopm_hash_table_lookup(merchs_in_cart, key, &in_cart_elem))
  {
    size_t in_cart = in_cart_elem.i;
    ioopm_hash_table_insert(merchs_in_cart, key, int_elem(quantity + in_cart));
  }
  else
  {
    ioopm_hash_table_insert(merchs_in_cart, key, int_elem(quantity));
  }
}

int add_to_cart(store_t *store, size_t cart_id, char *merch_name, size_t quantity)
{
  elem_t merch_elem;
  if (!get_merch(store, merch_name, &merch_elem))
  {
    free(merch_name);
    return -1;
  }

  elem_t cart_elem;
  if (!get_cart(store, cart_id, &cart_elem))
  {
    free(merch_name);
    return -2;
  }

  merch_t *merch = merch_elem.p;

  bool possible = reserve_merch(store, merch, quantity);
  if (possible)
  {
    cart_t *cart = cart_elem.p;
    update_cart(cart, merch, quantity);

    free(merch_name);
    return 1;
  }

  free(merch_name);
  return -3;
}

