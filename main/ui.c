#include <stdio.h>

#include "db.h"
#include "cart.h"
#include "io.h"

merch_t *creat_merch_user(void)
{
  char *name = get_name_io();
  char *desc = get_desc_io();
  int price = get_price_io();

  return create_merch(name, desc, price);
}
void add_merchandise_to_store(store_t *store)
{
  merch_t *merch = creat_merch_user();

  bool added = add_merch(store, merch);

  if (!added)
  {
    puts("A MERCHANDISE BY THIS NAME ALREADY EXISTS!!\n");
  }
  else
  {
    printf("%s HAS BEEN ADDED TO THE STORE!!\n", merch->name);
  }
}

int main(void)
{
  store_t *store = create_store();

  add_merchandise_to_store(store);

  elem_t merch_elem;
  ioopm_hash_table_lookup(store->name_to_item, string_elem("T"), &merch_elem);
  merch_t *merch = merch_elem.p;

  printf("%s\n", merch->name);
  printf("%s\n", merch->desc);
  printf("%ld\n", merch->price);

  return 0;
}