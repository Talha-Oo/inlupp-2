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

}

static cart_t *create_cart(store_t *store) {
  cart_t *cart = calloc(1, sizeof(cart_t));
  cart->merchendise = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &cart_destroy);
  
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

bool remove_cart(store_t *store, size_t id) {

  return 
}