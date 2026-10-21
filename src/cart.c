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

static void merchandise_destroyer(entry_t *entry)
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

bool ioopm_get_cart(store_t *store, size_t cart_id, elem_t *lookup_res)
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

static bool possible_to_reserve(size_t quantity, size_t reserved, size_t stock)
{
  return (quantity + reserved) <= stock ? true : false;
}

static bool reserve_merch(store_t *store, merch_t *merch, size_t quantity)
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

static void update_cart(cart_t *cart, merch_t *merch, size_t quantity)
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

int ioopm_add_to_cart(store_t *store, size_t cart_id, char *merch_name, size_t quantity)
{
  elem_t merch_elem;
  if (!ioopm_get_merch(store, merch_name, &merch_elem))
  {
    free(merch_name);
    return -1;
  }

  elem_t cart_elem;
  if (!ioopm_get_cart(store, cart_id, &cart_elem))
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

static void update_reserved_after_cart_rem(store_t *store, cart_t *cart)
{
  ioopm_hash_table_t *merchs_in_cart = cart->merchandise;
  ioopm_hash_table_t *reserved_merchs = store->reserved_merchs;
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(merchs_in_cart);

  elem_t reserved_num_elem;
  elem_t merch_name_elem;
  elem_t merch_in_cart_elem;
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    merch_name_elem = ioopm_hash_table_iterator_current_key(it);
    merch_in_cart_elem = ioopm_hash_table_iterator_current_value(it);

    size_t in_cart_num = merch_in_cart_elem.i;

    ioopm_hash_table_lookup(reserved_merchs, merch_name_elem, &reserved_num_elem);
    size_t reserved_num = reserved_num_elem.i;

    int upd_reserved = reserved_num - in_cart_num;

    elem_t dummy;
    if (upd_reserved <= 0)
    {
      ioopm_hash_table_remove(reserved_merchs, merch_name_elem, &dummy);
    }
    else
    {
      ioopm_hash_table_insert(reserved_merchs, merch_name_elem, int_elem(upd_reserved));
    }

    ioopm_hash_table_iterator_advance(it);
  }

  ioopm_hash_table_iterator_destroy(it);
}

static void update_reserved_after_merch_rem(store_t *store, char *merch_name, size_t to_remove)
{
  ioopm_hash_table_t *reserved_merchs = store->reserved_merchs;
  elem_t key = string_elem(merch_name);

  elem_t reserved_quantity_elem;
  ioopm_hash_table_lookup(reserved_merchs, key, &reserved_quantity_elem);
  size_t reserved_quantity = reserved_quantity_elem.i;

  int new_reserved_quantity = reserved_quantity - to_remove;

  elem_t dummy;
  if (new_reserved_quantity <= 0)
  {
    ioopm_hash_table_remove(reserved_merchs, key, &dummy);
  }
  else
  {
    ioopm_hash_table_insert(reserved_merchs, key, int_elem(new_reserved_quantity));
  }
}

bool ioopm_remove_cart(store_t *store, size_t cart_id)
{
  elem_t cart_elem;
  if (!ioopm_get_cart(store, cart_id, &cart_elem))
  {
    return false;
  }

  cart_t *cart = cart_elem.p;
  update_reserved_after_cart_rem(store, cart);

  ioopm_hash_table_t *all_carts = store->all_carts;
  elem_t key = int_elem(cart->id);

  ioopm_hash_table_remove(all_carts, key, &cart_elem);
  // ioopm_destroy_cart(cart);

  return true;
}

int ioopm_remove_from_cart(store_t *store, size_t cart_id, char *merch_name, size_t to_remove)
{
  elem_t cart_elem;
  if (!ioopm_get_cart(store, cart_id, &cart_elem))
  {
    free(merch_name);
    return -1;
  }
  cart_t *cart = cart_elem.p;

  ioopm_hash_table_t *merchs_in_carts = cart->merchandise;

  elem_t key = string_elem(merch_name);
  elem_t in_cart_elem;
  if (!ioopm_hash_table_lookup(merchs_in_carts, key, &in_cart_elem))
  {
    free(merch_name);
    return -2;
  }
  size_t in_cart = in_cart_elem.i;

  if (in_cart < to_remove)
  {
    free(merch_name);
    return -3;
  }

  update_reserved_after_merch_rem(store, merch_name, to_remove);

  int new_quantity = in_cart - to_remove;
  elem_t dummy;
  if (new_quantity <= 0)
  {
    ioopm_hash_table_remove(merchs_in_carts, key, &dummy);
  }
  else
  {
    ioopm_hash_table_insert(merchs_in_carts, key, int_elem(new_quantity));
  }

  free(merch_name);
  return 1;
}

static void decrease_shelf_stock(store_t *store, merch_t *merch, size_t to_remove)
{
  ioopm_list_t *shelves = merch->locs;
  ioopm_list_iterator_t *it = ioopm_list_iterator_create(shelves);

  elem_t shelf_elem;
  shelf_t *shelf;
  while (to_remove > 0 && !ioopm_list_iterator_at_end(it))
  {
    ioopm_list_iterator_current(it, &shelf_elem);
    shelf = shelf_elem.p;

    if (shelf->quantity > to_remove)
    {
      shelf->quantity -= to_remove;
      to_remove = 0;
      ioopm_list_iterator_advance(it);
    }
    else
    {
      to_remove -= shelf->quantity;
      char **name = shelf->merch_name;
      elem_t key = string_elem(*name);

      ioopm_hash_table_remove(store->shelf_to_item, key, &shelf_elem);
      ioopm_list_iterator_remove(it, &shelf_elem);

      free(shelf->shelf_name);
      free(shelf);
    }
  }

  ioopm_list_iterator_destroy(it);
}

long long merch_cost(store_t *store, elem_t merch_name_elem, elem_t quantity_elem, bool check_out)
{
  char *merch_name = merch_name_elem.s;
  size_t quantity = quantity_elem.i;

  elem_t merch_elem;
  ioopm_get_merch(store, merch_name, &merch_elem);
  merch_t *merch = merch_elem.p;

  if (check_out)
  {
    merch->stock -= quantity;
    decrease_shelf_stock(store, merch, quantity);
  }

  size_t price = merch->price;

  return price * quantity;
}

long long get_total(store_t *store, size_t cart_id, bool check_out)
{
  elem_t cart_elem;
  if (!ioopm_get_cart(store, cart_id, &cart_elem))
  {
    return -1;
  }

  cart_t *cart = cart_elem.p;
  ioopm_hash_table_t *merchs_in_cart = cart->merchandise;
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(merchs_in_cart);
  long long total = 0;

  elem_t merch_name_elem;
  elem_t quantity_elem;
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    merch_name_elem = ioopm_hash_table_iterator_current_key(it);
    quantity_elem = ioopm_hash_table_iterator_current_value(it);

    total += merch_cost(store, merch_name_elem, quantity_elem, check_out);
    ioopm_hash_table_iterator_advance(it);
  }

  ioopm_hash_table_iterator_destroy(it);

  return total;
}

long long ioopm_calculate_cost(store_t *store, size_t cart_id)
{
  return get_total(store, cart_id, false);
}

long long ioopm_checkout(store_t *store, size_t cart_id)
{
  long long total = get_total(store, cart_id, true);
  ioopm_remove_cart(store, cart_id);
  return total;
}