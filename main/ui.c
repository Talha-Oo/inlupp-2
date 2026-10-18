#include <stdio.h>
#include <string.h>

#include "../utilities/utils.h"
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

void print_merch(merch_t *merch)
{
  puts("----------------------------------");
  puts("------------- MERCH --------------");

  printf("NAME: %s\n", merch->name);
  printf("DESCRIPTION: %s\n", merch->desc);
  printf("Price: %ld\n", merch->price);

  puts("----------------------------------");
}

void add_merchandise_to_store(store_t *store)
{
  puts("----------------------------------");
  puts("---------- ADDING MERCH ----------");

  merch_t *merch = creat_merch_user();

  bool added = add_merch(store, merch);
  if (!added)
  {
    puts("\nA MERCHANDISE BY THIS NAME ALREADY EXISTS!!");
  }
  else
  {
    printf("\n%s HAS BEEN ADDED TO THE STORE!!\n", merch->name);
  }

  puts("----------------------------------\n");
}

void remove_merchandise_from_store(store_t *store)
{
  puts("----------------------------------");
  puts("--------- REMOVING MERCH ---------");

  char *name = get_name_io();
  bool removed = remove_merch(store, name);
  if (!removed)
  {
    puts("\nA MERCHANDISE BY THIS NAME DOES NOT EXIST!!");
  }
  else
  {
    printf("\n%s HAS BEEN REMOVED FROM THE STORE!!\n", name);
  }

  puts("----------------------------------\n");
  free(name);
}

static void update_merch_info(store_t *store, merch_t *merch)
{
  puts("\n----------------------------------");
  printf("NEW ");
  char *new_name = get_name_io();

  printf("NEW ");
  char *new_desc = get_desc_io();

  printf("NEW ");
  int new_price = get_price_io();

  puts("----------------------------------");

  bool confirm = is_confirmed_io();

  if (confirm)
  {
    edit_merch_name(store, merch, new_name);
    edit_merch_desc(merch, new_desc);
    edit_merch_price(merch, new_price);
  }
  else
  {
    free(new_name);
    free(new_desc);
  }
}

void edit_merchandise_in_store(store_t *store)
{
  puts("----------------------------------");
  puts("--------- EDITING MERCH ----------");
  puts("\n WHICH MERCH DO YOU WANT TO EDIT? ");

  char *name = get_name_io();

  elem_t merch_elem;
  bool exists = get_merch(store, name, &merch_elem);
  if (!exists)
  {
    puts("\nA MERCHANDISE BY THIS NAME DOES NOT EXIST!!");
  }
  else
  {
    merch_t *merch = merch_elem.p;
    update_merch_info(store, merch);

    print_merch(merch);
  }

  free(name);
  puts("----------------------------------\n");
}

void replenish_merchandise_in_store(store_t *store)
{
  puts("----------------------------------");
  puts("---------- REPLENSISH ------------");

  char *merch_name = get_name_io();

  elem_t merch_elem;
  bool exists = get_merch(store, merch_name, &merch_elem);
  if (!exists)
  {
    puts("\nA MERCHANDISE BY THIS NAME DOES NOT EXIST!!");

    free(merch_name);
    return;
  }

  char *shelf_name = get_shelf_io();
  size_t quantity = get_quantity_io();

  bool replenished = replenish(store, merch_name, quantity, shelf_name);
  if (!replenished)
  {
    puts("YOU CAN ONLY STORE ON TYPE OF MERCHANDISE ON A SHELF!!");
  }

  free(merch_name);
  puts("----------------------------------");
}

static int cmp_merch_name(const void *p1, const void *p2)
{
  merch_t *const *merch1_ptr = p1;
  merch_t *const *merch2_ptr = p2;

  merch_t *merch1 = *merch1_ptr;
  merch_t *merch2 = *merch2_ptr;

  const char *name1 = merch1->name;
  const char *name2 = merch2->name;

  return strcmp(name1, name2);
}

void sort_merch(merch_t *all_merch[], size_t merch_num)
{
  qsort(all_merch, merch_num, sizeof(merch_t *), cmp_merch_name);
}

void put_merch_in_arr(store_t *store, merch_t *merchs[])
{
  ioopm_hash_table_t *ht = store->name_to_item;
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  size_t i = 0;
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    elem_t merch_elem = ioopm_hash_table_iterator_current_value(it);
    merch_t *merch = merch_elem.p;

    merchs[i] = merch;
    i++;

    ioopm_hash_table_iterator_advance(it);
  }

  ioopm_hash_table_iterator_destroy(it);
}

void print_merch_list(merch_t *merchs[], size_t size, size_t *printed)
{
  size_t i;
  for (i = *printed; i < *printed + 20 && i < size; i++)
  {
    merch_t *merch = merchs[i];
    print_merch(merch);
  }

  *printed = i;
}

void list_merchandise(store_t *store)
{
  size_t merch_num = store->merch_num;

  if (merch_num == 0)
  {
    puts("NOTHIING TO LIST, PLEASE ADD MERCHANDISE TO THE STORE FIRST!!");
    return;
  }

  merch_t *merchs[merch_num];

  put_merch_in_arr(store, merchs);
  sort_merch(merchs, merch_num);

  size_t printed = 0;
  bool to_continue = true;

  while (to_continue && printed < merch_num)
  {
    print_merch_list(merchs, merch_num, &printed);

    if (printed < merch_num)
    {
      to_continue = should_continue_listing_io();
    }
    else
    {
      break;
    }
  }
}

int main(void)
{
  store_t *store = create_store();

  add_merchandise_to_store(store);
  add_merchandise_to_store(store);

  destroy_store(store);
  return 0;
}