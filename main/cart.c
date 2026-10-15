#include "../utilities/common.h"

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


static void cart_destroy(entry_t *entry) {
  // TODO: IMPLEMENT, MAYBE??

  free(entry);
}

static cart_t *create_cart(store_t *store) {
  cart_t *cart = calloc(1, sizeof(cart_t));
  cart->merchandise = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &cart_destroy);
  
  store->carts_num++;
  cart->id = store->carts_num;

  return cart;
}

void add_cart(store_t *store) {

  cart_t *cart = create_cart(store);

  ioopm_hash_table_t *all_cart = store->all_carts;
  elem_t cart_elem = ptr_elem(cart);
  elem_t key_elem = int_elem(cart->id);

  ioopm_hash_table_insert(all_cart, key_elem, cart_elem);
}

bool remove_cart(store_t *store, size_t id){
  elem_t current_cart;
  elem_t old_quantity;
  elem_t current_key = int_elem(id);
  
  ioopm_hash_table_t *all_carts = store->all_carts;
  ioopm_hash_table_t *reserved_merchs = store->reserved_merchs;

  bool status = ioopm_hash_table_lookup(all_carts, current_key, &current_cart);
  if(!status){return false;}

  cart_t *cart = current_cart.p;

  ioopm_hash_table_t *merchandise = cart->merchandise;
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(merchandise);
  while(!ioopm_hash_table_iterator_at_end(it)){
    elem_t key = ioopm_hash_table_iterator_current_key(it);
    elem_t value = ioopm_hash_table_iterator_current_value(it);
    bool status = ioopm_hash_table_lookup(reserved_merchs, key, &old_quantity);
    if(!status){return false;}
    old_quantity.i = old_quantity.i - value.i;
    ioopm_hash_table_insert(reserved_merchs, key, old_quantity);
    ioopm_hash_table_iterator_advance(it);
  }
  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(merchandise);
  return true;
}