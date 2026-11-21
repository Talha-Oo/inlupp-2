#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../utilities/utils.h"
#include "db.h"
#include "cart.h"
#include "io.h"

store_t *ioopm_make_store(void)
{
  return ioopm_create_store();
}

static merch_t *create_merch_user(void)
{
  char *name = get_name_io();
  char *desc = get_desc_io();
  int price = get_price_io();

  return ioopm_create_merch(name, desc, price);
}

void print_merch(merch_t *merch)
{
  puts("----------------------------------");
  puts("------------- MERCH --------------");

  printf("NAME: %s\n", ioopm_merch_name_get(merch)  /*merch->name*/ );
  printf("DESCRIPTION: %s\n", ioopm_merch_desc_get(merch) /*merch->desc*/ );
  printf("Price: %ld\n", ioopm_merch_price_get(merch) /*merch->price*/);

  puts("----------------------------------\n");
}

static int cmp_merch_name(const void *p1, const void *p2)
{
  merch_t *const *merch1_ptr = p1;
  merch_t *const *merch2_ptr = p2;

  merch_t *merch1 = *merch1_ptr;
  merch_t *merch2 = *merch2_ptr;

  const char *name1 = ioopm_merch_name_get(merch1) /*merch1->name*/;
  const char *name2 = ioopm_merch_name_get(merch2) /*merch2->name*/;

  return strcmp(name1, name2);
}

static int cmp_shelf_name(const void *p1, const void *p2)
{
  shelf_t *const *shelf1_ptr = p1;
  shelf_t *const *shelf2_ptr = p2;

  shelf_t *shelf1 = *shelf1_ptr;
  shelf_t *shelf2 = *shelf2_ptr;

  const char *name1 = ioopm_shelf_name_get(shelf1) /*shelf1->shelf_name*/;
  const char *name2 = ioopm_shelf_name_get(shelf2) /*shelf2->shelf_name*/;

  return strcmp(name1, name2);
}

static void sort_merch(merch_t *all_merch[], size_t merch_num)
{
  qsort(all_merch, merch_num, sizeof(merch_t *), cmp_merch_name);
}

static void sort_shelf(shelf_t *shelves[], size_t shelf_num)
{
  qsort(shelves, shelf_num, sizeof(shelf_t *), cmp_shelf_name);
}

static void put_merch_in_arr(store_t *store, merch_t *merchs[])
{
  ioopm_hash_table_t *ht = ioopm_store_name_to_item_get(store) /* store->name_to_item */;
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

static void put_shelf_in_arr(ioopm_list_t *list, shelf_t *shelves[])
{
  ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);

  size_t i = 0;
  while (!ioopm_list_iterator_at_end(it))
  {
    elem_t shelf_elem;
    ioopm_list_iterator_current(it, &shelf_elem);
    shelf_t *shelf = shelf_elem.p;

    shelves[i] = shelf;
    i++;

    ioopm_list_iterator_advance(it);
  }

  ioopm_list_iterator_destroy(it);
}

static void print_list_merch(merch_t *merchs[], size_t size, size_t *printed)
{
  size_t i;
  for (i = *printed; i < *printed + 20 && i < size; i++)
  {
    merch_t *merch = merchs[i];
    print_merch(merch);
  }

  *printed = i;
}

static void print_list_shelf(shelf_t *shelves[], size_t size, size_t *printed)
{
  size_t i;
  for (i = *printed; i < *printed + 20 && i < size; i++)
  {
    shelf_t *shelf = shelves[i];
    printf("%s: %ld\n", ioopm_shelf_name_get(shelf)  /*shelf->shelf_name*/, ioopm_shelf_quantity_get(shelf) /*shelf->quantity*/);
  }

  *printed = i;
}

void add_merchandise_to_store(store_t *store)
{
  puts("----------------------------------");
  puts("---------- ADDING MERCH ----------");

  merch_t *merch = create_merch_user();

  bool added = ioopm_add_merch(store, merch);
  if (!added)
  {
    puts("\nA MERCHANDISE BY THIS NAME ALREADY EXISTS!!");
  }
  else
  {
    printf("\n%s HAS BEEN ADDED TO THE STORE!!\n", ioopm_merch_name_get(merch) /*merch->name*/);
  }

  puts("----------------------------------\n");
}

void remove_merchandise_from_store(store_t *store)
{
  puts("----------------------------------");
  puts("--------- REMOVING MERCH ---------");

  char *name = get_name_io();

  bool to_remove = is_confirmed_io();
  if (!to_remove)
  {
    free(name);
    puts("----------------------------------\n");
    return;
  }

  bool removed = ioopm_remove_merch(store, name);
  if (!removed)
  {
    puts("\nA MERCHANDISE BY THIS NAME DOES NOT EXIST!!");
  }
  else
  {
    printf("\n%s HAS BEEN REMOVED FROM THE STORE!!\n", name);
  }

  free(name);
  puts("----------------------------------\n");
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
  bool exists = ioopm_get_merch(store, name, &merch_elem);
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

  char *shelf_name = get_shelf_io();

  char *merch_name = get_name_io();

  elem_t merch_elem;
  bool exists = ioopm_get_merch(store, merch_name, &merch_elem);
  if (!exists)
  {
    puts("\nA MERCHANDISE BY THIS NAME DOES NOT EXIST!!");

    free(shelf_name);
    free(merch_name);
    puts("----------------------------------\n");

    return;
  }

  size_t quantity = get_quantity_io();

  bool replenished = ioopm_replenish(store, merch_name, quantity, shelf_name);
  if (!replenished)
  {
    puts("YOU CAN ONLY STORE ONE TYPE OF MERCHANDISE ON A SHELF!!");
    puts("----------------------------------\n");
  }

  printf("THER MERCHANDISE HAS BEEN ADDED TO %s!!", shelf_name);
  puts("----------------------------------\n");
}

void list_merchandise(store_t *store)
{
  puts("----------------------------------");
  puts("------- LIST MERCHANDISE ---------");

  size_t merch_num = ioopm_store_merch_num_get(store) /* store->merch_num */;

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
    print_list_merch(merchs, merch_num, &printed);

    if (printed < merch_num)
    {
      to_continue = should_continue_listing_io();
    }
    else
    {
      break;
    }
  }

  puts("----------------------------------\n");
}

void show_stock(store_t *store)
{
  puts("----------------------------------");
  puts("---------- SHOW STOCK ------------");

  char *merch_name = get_name_io();

  elem_t merch_elem;
  bool exists = ioopm_get_merch(store, merch_name, &merch_elem);
  if (!exists)
  {
    puts("\nA MERCHANDISE BY THIS NAME DOES NOT EXIST!!");

    free(merch_name);
    puts("----------------------------------\n");
    return;
  }

  merch_t *merch = merch_elem.p;

  ioopm_list_t *shelf_list = ioopm_merch_locs_get(merch) /* merch->locs */;
  size_t shelf_num = ioopm_list_size(shelf_list);

  if (shelf_num == 0)
  {
    free(merch_name);
    puts("NOTHIING TO LIST, THE MERCHANDISE HAS NO STOCK!!");
    puts("----------------------------------\n");

    return;
  }

  shelf_t *shelves[shelf_num];

  put_shelf_in_arr(shelf_list, shelves);
  sort_shelf(shelves, shelf_num);

  size_t printed = 0;
  bool to_continue = true;

  while (to_continue && printed < shelf_num)
  {
    print_list_shelf(shelves, shelf_num, &printed);

    if (printed < shelf_num)
    {
      to_continue = should_continue_listing_io();
    }
    else
    {
      break;
    }
  }

  free(merch_name);

  puts("----------------------------------\n");
}

void add_cart_in_store(store_t *store)
{
  puts("----------------------------------");

  ioopm_add_cart(store);
  puts("A CART HAS SUCCESSFULLY BEEN ADDED!!");

  puts("----------------------------------\n");
}

void remove_cart_from_store(store_t *store)
{
  puts("----------------------------------");
  puts("--------- REMOVE CART ------------");

  size_t cart_id = get_cart_id_io();

  bool delete = is_confirmed_io();
  if (delete)
  {
    bool result = ioopm_remove_cart(store, cart_id);
    if (!result)
    {
      puts("\nINVALID CART ID!!");
    }
    else
    {
      printf("CART %ld HAS SUCCESSFULLY DELETED!!\n", cart_id);
    }
  }

  puts("----------------------------------\n");
}

void add_to_cart_in_store(store_t *store)
{
  puts("----------------------------------");
  puts("--------- ADDING TO CART ---------");

  size_t cart_id = get_cart_id_io();
  char *merch_name = get_name_io();
  size_t quantity = get_quantity_io();
  puts("----------------------------------");

  int res = ioopm_add_to_cart(store, cart_id, merch_name, quantity);
  switch (res)
  {
  case -1:
    printf("THERE IS NO SUCH MERCHANDISE IN THE STORE!!\n");
    break;

  case -2:
    printf("INVALID CART ID!!\n");
    break;

  case -3:
    printf("ENOUGH STOCK IS NOT AVAILABLE!!\n");
    break;

  default:
    printf("ADDED TO THE CART %ld!!\n", cart_id);
    break;
  }

  puts("----------------------------------\n");
}

void remove_from_cart_in_store(store_t *store)
{
  puts("----------------------------------");
  puts("------- REMOVING FROM CART -------");

  size_t cart_id = get_cart_id_io();
  char *merch_name = get_name_io();
  size_t quantity = get_quantity_io();
  puts("----------------------------------");

  int res = ioopm_remove_from_cart(store, cart_id, merch_name, quantity);
  switch (res)
  {
  case -1:
    printf("INVALID CART ID!!\n");
    break;

  case -2:
    printf("THERE IS NO SUCH MERCHANDISE IN THE STORE!!\n");
    break;

  case -3:
    printf("QUANTITY EXCEEDS THE AMOUNT IN THE CART!!\n");
    break;

  default:
    printf("MERCHANDISE HAS BEEN REMOVED FROM CART %ld!!\n", cart_id);
    break;
  }

  puts("----------------------------------\n");
}

void remove_cart_in_store(store_t *store)
{
  puts("----------------------------------");
  puts("--------- REMOVING CART ----------");

  size_t cart_id = get_cart_id_io();
  puts("----------------------------------");

  bool removed = ioopm_remove_cart(store, cart_id);
  if (removed)
  {
    printf("CART %ld HAS BEEN REMOVED!!", cart_id);
  }
  else
  {
    printf("INVALID CART ID!!");
  }

  puts("----------------------------------\n");
}

void calculate_cart_cost(store_t *store)
{
  puts("----------------------------------");
  puts("--------- CALCULATE COST ---------");

  size_t cart_id = get_cart_id_io();

  long long cost = ioopm_calculate_cost(store, cart_id);
  if (cost == -1)
  {
    printf("CART %ld DOES NOT EXIST!!\n", cart_id);
    puts("----------------------------------\n");
    return;
  }

  printf("THE TOTAL COST OF CART %ld is %lld\n", cart_id, cost);

  puts("----------------------------------\n");
}

void checkout_cart(store_t *store)
{
  puts("----------------------------------");
  puts("------------ CHECKOUT ------------");

  size_t cart_id = get_cart_id_io();

  long long cost = ioopm_checkout(store, cart_id);
  if (cost == -1)
  {
    printf("CART %ld DOES NOT EXIST!!\n", cart_id);
    puts("----------------------------------\n");
    return;
  }

  printf("CART %ld HAS BEEN CHECKED OUT!!\n", cart_id);
  printf("TOTAL: %lld!!\n", cost);

  puts("----------------------------------\n");
}

bool confirm_quit(store_t *store)
{
  puts("----------------------------------");
  puts("------------- QUIT?! -------------");

  bool quit = is_confirmed_io();
  if(quit)
  {
    ioopm_destroy_store(store);

    puts("------------- BYE!! --------------");
    puts("----------------------------------\n");
  }

  puts("----------------------------------\n");

  return quit;
}

char get_action_choice(void)
{
  puts("");

  puts("[A] Add Merchandise");
  puts("[L] List Merchandise");
  puts("[D] Remove Merchandise");
  puts("[E] Edit Merchandise");
  puts("[S] Show Stock");
  puts("[P] Replenish Stock");
  puts("");

  puts("[C] Create Cart");
  puts("[R] Remove Cart");
  puts("[+] Add To Cart");
  puts("[-] Remove From Cart");
  puts("[=] Calculate Cart Cost");
  puts("[O] Checkout Cart");
  puts("");

  puts("[Q] Quit");
  puts("");

  puts("----------------------------------");
  char *action = ask_question_string("ENTER: ", "");

  char upper_action = toupper(action[0]);
  free(action);
  puts("----------------------------------\n");
  return upper_action;
}
