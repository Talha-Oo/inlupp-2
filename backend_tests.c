#include "utilities/common.h"
#include "src/db.h"
#include "src/cart.h"

#include <stdbool.h>
#include <string.h>
#include <CUnit/Basic.h>

int init_suite(void)
{
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void)
{
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

//////////////////////// TODO: Tests
void test_create_destroy_store()
{
  store_t *store = ioopm_create_store();
  ioopm_destroy_store(store);
}

void test_make_destroy_merch()
{
  char *name = strdup("T");
  char *desc = strdup("TB");

  merch_t *merch = ioopm_create_merch(name, desc, 123);

  CU_ASSERT_EQUAL(strcmp(name, ioopm_merch_name_get(merch)), 0); // Using name_get instead instead of merch->name
  CU_ASSERT_EQUAL(strcmp(desc, ioopm_merch_desc_get(merch)), 0); // Using desc_get instead instead of merch->desc
  CU_ASSERT_EQUAL(ioopm_merch_price_get(merch), 123); // Using price_get instead instead of merch->price

  ioopm_destroy_merch(merch);
}

void test_add_merchs()
{
  store_t *store = ioopm_create_store();

  elem_t looked_up_elem;

  char *name1 = strdup("T");
  char *desc1 = strdup("TB");
  merch_t *merch1 = ioopm_create_merch(name1, desc1, 123);

  // Adding a new merchendise
  bool add_res = ioopm_add_merch(store, merch1);

  bool lookup_res = ioopm_get_merch(store, name1, &looked_up_elem);

  CU_ASSERT_TRUE(add_res);
  CU_ASSERT_TRUE(lookup_res);

  // Checking if we can read the stored merchendise with lookup
  merch_t *looked_up_merch = looked_up_elem.p;
  CU_ASSERT_EQUAL(strcmp(name1, ioopm_merch_name_get(looked_up_merch)), 0);
  CU_ASSERT_EQUAL(strcmp(desc1, ioopm_merch_desc_get(looked_up_merch)), 0);
  CU_ASSERT_EQUAL(ioopm_merch_price_get(looked_up_merch), 123);

  // Adding a merchendise which already exists
  char *name2 = strdup("T");
  char *desc2 = strdup("TB");
  merch_t *merch2 = ioopm_create_merch(name2, desc2, 123);

  add_res = ioopm_add_merch(store, merch2);
  lookup_res = ioopm_get_merch(store, name1, &looked_up_elem);

  CU_ASSERT_FALSE(add_res);
  CU_ASSERT_TRUE(lookup_res);

  // Checking if information is still the same
  looked_up_merch = looked_up_elem.p;
  CU_ASSERT_EQUAL(strcmp(name1, ioopm_merch_name_get(looked_up_merch)), 0);
  CU_ASSERT_EQUAL(strcmp(desc1, ioopm_merch_desc_get(looked_up_merch)), 0);
  CU_ASSERT_EQUAL(ioopm_merch_price_get(looked_up_merch), 123);

  // Adding a new merchendise with similar name to a one which already exists
  char *old_name = strdup("T");
  char *new_desc = strdup("YO");
  merch_t *merch3 = ioopm_create_merch(old_name, new_desc, 123123);

  add_res = ioopm_add_merch(store, merch3);
  lookup_res = ioopm_get_merch(store, name1, &looked_up_elem);

  CU_ASSERT_FALSE(add_res);
  CU_ASSERT_TRUE(lookup_res);

  // Checking if information is still the same
  looked_up_merch = looked_up_elem.p;
  CU_ASSERT_EQUAL(strcmp(name1, ioopm_merch_name_get(looked_up_merch)), 0);
  CU_ASSERT_EQUAL(strcmp(desc1, ioopm_merch_desc_get(looked_up_merch)), 0);
  CU_ASSERT_EQUAL(ioopm_merch_price_get(looked_up_merch), 123);

  // Adding a new completely merchendise
  char *new_name = strdup("YO!");
  new_desc = strdup("YO");
  merch_t *merch4 = ioopm_create_merch(new_name, new_desc, 123123);

  add_res = ioopm_add_merch(store, merch4);

  lookup_res = ioopm_get_merch(store, new_name, &looked_up_elem);

  CU_ASSERT_TRUE(add_res);
  CU_ASSERT_TRUE(lookup_res);

  // Checking if information is matches
  looked_up_merch = looked_up_elem.p;
  CU_ASSERT_EQUAL(strcmp(new_name, ioopm_merch_name_get(looked_up_merch)), 0);
  CU_ASSERT_EQUAL(strcmp(new_desc, ioopm_merch_desc_get(looked_up_merch)), 0);
  CU_ASSERT_EQUAL(ioopm_merch_price_get(looked_up_merch), 123123);

  ioopm_destroy_store(store);
}

void test_replenish_merchs()
{
  store_t *store = ioopm_create_store();

  char *name1 = strdup("Kunai");
  char *desc1 = strdup("Kaching");
  merch_t *merch1 = ioopm_create_merch(name1, desc1, 123);

  char *name2 = strdup("Kaching");
  char *desc2 = strdup("Kunai");
  merch_t *merch2 = ioopm_create_merch(name2, desc2, 321);

  ioopm_add_merch(store, merch2);
  ioopm_add_merch(store, merch1);

  elem_t merch1_elem;
  elem_t merch2_elem;
  CU_ASSERT_TRUE(ioopm_get_merch(store, name1, &merch1_elem));
  CU_ASSERT_TRUE(ioopm_get_merch(store, name2, &merch2_elem));

  ioopm_list_t *merch1_list = ioopm_merch_locs_get(merch1); // Using locs_get instead och merck->locs
  ioopm_list_t *merch2_list = ioopm_merch_locs_get(merch2); // Same as merch1

  ioopm_replenish(store, strdup("Kunai"), 1, strdup("ABC"));
  CU_ASSERT_EQUAL(ioopm_list_size(merch1_list), 1); // check if shelf is added in the list of kunai
  CU_ASSERT_EQUAL(ioopm_list_size(merch2_list), 0); // list of Kaching should still be emepty

  ioopm_replenish(store, strdup("Kaching"), 2, strdup("XYZ"));
  CU_ASSERT_EQUAL(ioopm_list_size(merch2_list), 1); // check if shelf is added in the list of kaching

  // checking if the shelf has been added to the hashtable and if the info matches correctly with the
  //  shelf in the list, in the hashtable and the merch itself
  // Kunai:
  elem_t shelf_elem = ioopm_list_head(merch1_list);
  shelf_t *shelf = shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_name_get(shelf), "ABC"), 0);
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_merch_name_get(shelf), ioopm_merch_name_get(merch1)), 0);
  CU_ASSERT_EQUAL(ioopm_merch_stock_get(merch1), 1);

  elem_t looked_up_shelf_elem;
  CU_ASSERT_TRUE(ioopm_get_shelf(store, "ABC", &looked_up_shelf_elem));

  shelf_t *looked_up_shelf = looked_up_shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_name_get(looked_up_shelf), "ABC"), 0);
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_merch_name_get(looked_up_shelf), ioopm_merch_name_get(merch1)), 0);
  CU_ASSERT_EQUAL(ioopm_shelf_quantity_get(looked_up_shelf), 1);

  // checking if the shelf has been added to the hashtable and if the info matches correctly with the
  //  shelf in the list, in the hashtable and the merch itself
  // Kaching:
  shelf_elem = ioopm_list_head(merch2_list);
  shelf = shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_name_get(shelf), "XYZ"), 0);
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_merch_name_get(shelf), ioopm_merch_name_get(merch2)), 0);
  CU_ASSERT_EQUAL(ioopm_shelf_quantity_get(shelf), 2);

  CU_ASSERT_TRUE(ioopm_get_shelf(store, "XYZ", &looked_up_shelf_elem));

  looked_up_shelf = looked_up_shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_name_get(looked_up_shelf), "XYZ"), 0);
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_merch_name_get(looked_up_shelf), ioopm_merch_name_get(merch2)), 0);
  CU_ASSERT_EQUAL(ioopm_shelf_quantity_get(looked_up_shelf), 2);

  // Trying to add stock to a shelf of different type of merch
  CU_ASSERT_FALSE(ioopm_replenish(store, strdup("Kaching"), 1000000, strdup("ABC")));
  CU_ASSERT_FALSE(ioopm_replenish(store, strdup("Kunai"), 1000000, strdup("XYZ")));

  // Trying to add stock to a shelf of a nonexistent merch
  CU_ASSERT_FALSE(ioopm_replenish(store, strdup("YO!"), 128936412, strdup("LMNOP")));

  // adding stock to a shelfwith correct type
  CU_ASSERT_TRUE(ioopm_replenish(store, strdup("Kunai"), 1000000, strdup("ABC")));
  // checking if the shelf has been updated in the hashtable and if the info matches correctly with the
  //  shelf in the list, in the hashtable and the merch itself
  // Kunai:
  shelf_elem = ioopm_list_head(merch1_list);
  shelf = shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_name_get(shelf), "ABC"), 0);
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_merch_name_get(shelf), ioopm_merch_name_get(merch1)), 0);
  CU_ASSERT_EQUAL(ioopm_merch_stock_get(merch1), 1000001);

  CU_ASSERT_TRUE(ioopm_get_shelf(store, "ABC", &looked_up_shelf_elem));

  looked_up_shelf = looked_up_shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_name_get(looked_up_shelf), "ABC"), 0);
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_merch_name_get(looked_up_shelf), ioopm_merch_name_get(merch1)), 0);
  CU_ASSERT_EQUAL(ioopm_shelf_quantity_get(looked_up_shelf), 1000001);


  // Having two different shelves
  ioopm_replenish(store, strdup("Kunai"), 1, strdup("LMNOP"));
  CU_ASSERT_TRUE(ioopm_get_shelf(store, "LMNOP", &looked_up_shelf_elem));
  looked_up_shelf = looked_up_shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_name_get(looked_up_shelf), "LMNOP"), 0);
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_merch_name_get(looked_up_shelf), ioopm_merch_name_get(merch1)), 0);
  CU_ASSERT_EQUAL(ioopm_shelf_quantity_get(looked_up_shelf) , 1);

  // Checking if the new shelf is in the list
  shelf_elem = ioopm_list_head(merch1_list);
  shelf = shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_name_get(shelf), "LMNOP"), 0);
  CU_ASSERT_EQUAL(strcmp(ioopm_shelf_merch_name_get(shelf)  , ioopm_merch_name_get(merch1)), 0);
  CU_ASSERT_EQUAL(ioopm_merch_stock_get(merch1), 1000002);
  CU_ASSERT_EQUAL(ioopm_list_size(merch1_list), 2);

  // Adding stock to a existing shelf with correct merch
  ioopm_replenish(store, strdup("Kunai"), 1, strdup("LMNOP"));
  CU_ASSERT_TRUE(ioopm_get_shelf(store, "LMNOP", &looked_up_shelf_elem));
  looked_up_shelf = looked_up_shelf_elem.p;
  CU_ASSERT_EQUAL(ioopm_shelf_quantity_get(looked_up_shelf) , 2);

  ioopm_destroy_store(store);
}

void test_edit_desc_price()
{

  char *name = strdup("T");
  char *desc = strdup("TB");

  merch_t *merch = ioopm_create_merch(name, desc, 123);

  CU_ASSERT_EQUAL(strcmp(name, ioopm_merch_name_get(merch) ), 0);
  CU_ASSERT_EQUAL(strcmp(desc, ioopm_merch_desc_get(merch)), 0);
  CU_ASSERT_EQUAL(ioopm_merch_price_get(merch), 123);

  char *new_desc = strdup("CHANGE!!");
  edit_merch_desc(merch, new_desc);
  edit_merch_price(merch, 321);

  CU_ASSERT_EQUAL(strcmp(new_desc, ioopm_merch_desc_get(merch)), 0);
  CU_ASSERT_EQUAL(ioopm_merch_price_get(merch), 321);

  ioopm_destroy_merch(merch);
}

void test_edit_merch_name()
{
  store_t *store = ioopm_create_store();

  // Creating merchandise with its original name and description
  char *name = strdup("Kunai");
  char *desc = strdup("Kaching");
  merch_t *merch = ioopm_create_merch(name, desc, 123);

  // Adding the merchandise to the store
  ioopm_add_merch(store, merch);

  // Creating a new name and renaming the merchandise
  char *new_name = strdup("Shuriken");
  bool edit_res = edit_merch_name(store, merch, new_name);

  // Checking that the rename operation succeeded
  CU_ASSERT_TRUE(edit_res);

  // Checking that the merchandise has the new name
  CU_ASSERT_EQUAL(strcmp(ioopm_merch_name_get(merch), "Shuriken"), 0);

  // Checking that the new name works for lookup
  elem_t looked_up_elem;
  CU_ASSERT_TRUE(ioopm_get_merch(store, "Shuriken", &looked_up_elem));

  // Checking that the lookup returns the same merchandise
  merch_t *looked_up_merch = looked_up_elem.p;
  CU_ASSERT_PTR_EQUAL(looked_up_merch, merch);

  // Checking that the old name no longer works for lookup
  CU_ASSERT_FALSE(ioopm_get_merch(store, "Kunai", &looked_up_elem));

  // Checking that the merchandise description and price remain unchanged
  CU_ASSERT_EQUAL(strcmp(ioopm_merch_desc_get(merch), "Kaching"), 0);
  CU_ASSERT_EQUAL(ioopm_merch_price_get(merch), 123);

  // Trying to rename the merchandise to a name that already exists
  merch_t *other_merch = ioopm_create_merch(strdup("Kunai"), strdup("Other"), 456);
  ioopm_add_merch(store, other_merch);

  char *duplicate_name = strdup("Shuriken");
  bool duplicate_res = edit_merch_name(store, other_merch, duplicate_name);

  // Checking that renaming to an existing name fails
  CU_ASSERT_FALSE(duplicate_res);

  // Checking that both merchandise items keep their original names
  CU_ASSERT_EQUAL(strcmp(ioopm_merch_name_get(merch), "Shuriken"), 0);
  CU_ASSERT_EQUAL(strcmp(ioopm_merch_name_get(other_merch), "Kunai"), 0);

  // Checking that both merchandise items can still be found
  CU_ASSERT_TRUE(ioopm_get_merch(store, "Shuriken", &looked_up_elem));
  CU_ASSERT_PTR_EQUAL(looked_up_elem.p, merch);

  CU_ASSERT_TRUE(ioopm_get_merch(store, "Kunai", &looked_up_elem));
  CU_ASSERT_PTR_EQUAL(looked_up_elem.p, other_merch);

  // Renaming the first merchandise a second time
  char *last_name = strdup("Ninja Star");
  bool second_edit = edit_merch_name(store, merch, last_name);

  // Checking that the second rename succeeds
  CU_ASSERT_TRUE(second_edit);

  // Checking that only the latest name works
  CU_ASSERT_FALSE(ioopm_get_merch(store, "Shuriken", &looked_up_elem));
  CU_ASSERT_TRUE(ioopm_get_merch(store, "Ninja Star", &looked_up_elem));
  CU_ASSERT_PTR_EQUAL(looked_up_elem.p, merch);

  // Checking that the merchandise description and price remain unchanged
  CU_ASSERT_EQUAL(strcmp(ioopm_merch_desc_get(merch), "Kaching"), 0);
  CU_ASSERT_EQUAL(ioopm_merch_price_get(merch), 123);

  // Destroying the store and freeing its allocated memory
  ioopm_destroy_store(store);
}

void test_remove_merch()
{
  store_t *store = ioopm_create_store();

  char *name1 = strdup("T");
  char *desc1 = strdup("TB");
  char *name2 = strdup("YO!");
  char *desc2 = strdup("YO");

  merch_t *merch1 = ioopm_create_merch(name1, desc1, 123);
  merch_t *merch2 = ioopm_create_merch(name2, desc2, 123123);

  ioopm_add_merch(store, merch1);
  ioopm_add_merch(store, merch2);

  ioopm_replenish(store, strdup("T"), 1, strdup("ABC"));
  ioopm_replenish(store, strdup("T"), 10, strdup("LMNOP"));
  ioopm_replenish(store, strdup("T"), 100, strdup("QWERTY"));

  ioopm_replenish(store, strdup("YO!"), 1, strdup("XYZ"));
  ioopm_replenish(store, strdup("YO!"), 1, strdup("ÅÄÖ"));

  elem_t looked_up_elem;

  CU_ASSERT_TRUE(ioopm_get_merch(store, "T", &looked_up_elem));
  CU_ASSERT_TRUE(ioopm_get_merch(store, "YO!", &looked_up_elem));

  CU_ASSERT_TRUE(ioopm_get_shelf(store, "ABC", &looked_up_elem));
  CU_ASSERT_TRUE(ioopm_get_shelf(store, "LMNOP", &looked_up_elem));
  CU_ASSERT_TRUE(ioopm_get_shelf(store, "QWERTY", &looked_up_elem));
  CU_ASSERT_TRUE(ioopm_get_shelf(store, "XYZ", &looked_up_elem));
  CU_ASSERT_TRUE(ioopm_get_shelf(store, "ÅÄÖ", &looked_up_elem));

  // Trying to remove a merch which doesn't exist
  CU_ASSERT_FALSE(ioopm_remove_merch(store, "BlaBlaBla"));

  // Removing a merch which exists
  CU_ASSERT_TRUE(ioopm_remove_merch(store, "T"));

  CU_ASSERT_FALSE(ioopm_get_merch(store, "T", &looked_up_elem));
  CU_ASSERT_FALSE(ioopm_get_shelf(store, "ABC", &looked_up_elem));
  CU_ASSERT_FALSE(ioopm_get_shelf(store, "LMNOP", &looked_up_elem));
  CU_ASSERT_FALSE(ioopm_get_shelf(store, "QWERTY", &looked_up_elem));

  // Checking if the other merch exists
  CU_ASSERT_TRUE(ioopm_get_merch(store, "YO!", &looked_up_elem));
  CU_ASSERT_TRUE(ioopm_get_shelf(store, "XYZ", &looked_up_elem));
  CU_ASSERT_TRUE(ioopm_get_shelf(store, "ÅÄÖ", &looked_up_elem));

  // Trying to remove a removed merch
  CU_ASSERT_FALSE(ioopm_remove_merch(store, "T"));

  // Removing a merch which exists
  CU_ASSERT_TRUE(ioopm_remove_merch(store, "YO!"));

  CU_ASSERT_FALSE(ioopm_get_merch(store, "YO!", &looked_up_elem));
  CU_ASSERT_FALSE(ioopm_get_shelf(store, "XYZ", &looked_up_elem));
  CU_ASSERT_FALSE(ioopm_get_shelf(store, "ÅÄÖ", &looked_up_elem));

  ioopm_destroy_store(store);
}

/////////////////////////////////////////// CART /////////////////////////////////////////////////////

void test_create_destroy_cart()
{
  store_t *store = ioopm_create_store();
  cart_t *cart = ioopm_create_cart(store);

  CU_ASSERT_EQUAL(ioopm_cart_id_get(cart), 1);

  ioopm_destroy_cart(cart);
  ioopm_destroy_store(store);
}

void test_add_carts(void)
{
  store_t *store = ioopm_create_store();
  ioopm_add_cart(store);

  CU_ASSERT_EQUAL(ioopm_store_carts_num_get(store), 1);

  elem_t cart_elem;
  ioopm_hash_table_lookup(ioopm_store_all_carts_get(store), int_elem(1), &cart_elem);
  cart_t *cart = cart_elem.p;

  CU_ASSERT_EQUAL(ioopm_cart_id_get(cart), 1);

  ioopm_add_cart(store);
  CU_ASSERT_EQUAL(ioopm_store_carts_num_get(store), 2);

  ioopm_destroy_store(store);
}

void test_add_to_cart(void)
{
  store_t *store = ioopm_create_store();

  merch_t *merch = ioopm_create_merch(strdup("Kunai"), strdup("Knife"), 100);
  ioopm_add_merch(store, merch);
  ioopm_replenish(store, strdup("Kunai"), 10, strdup("ABC")); // Has stock 10

  merch_t *merch1 = ioopm_create_merch(strdup("BLA"), strdup("BLA"), 100);
  ioopm_add_merch(store, merch1);
  ioopm_replenish(store, strdup("BLA"), 3, strdup("XYZ")); // Has stock 3

  ioopm_add_cart(store);
  ioopm_add_cart(store);

  // Adding merchandise to the cart
  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 1, strdup("Kunai"), 4), 1); // IT should succedd

  elem_t cart1_elem;
  elem_t cart2_elem;
  CU_ASSERT_TRUE(ioopm_get_cart(store, 1, &cart1_elem)); // Get the first cart
  CU_ASSERT_TRUE(ioopm_get_cart(store, 2, &cart2_elem)); // Get the second cart
  cart_t *cart1 = cart1_elem.p;
  cart_t *cart2 = cart2_elem.p;

  elem_t quantity1_elem;
  elem_t quantity2_elem;
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_cart_merchandise_get(cart1), string_elem("Kunai"), &quantity1_elem));
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ioopm_cart_merchandise_get(cart2), string_elem("Kunai"), &quantity2_elem));
  CU_ASSERT_EQUAL(quantity1_elem.i, 4); // Should be 4 Kunais in the cart

  // Adding more of the same merchandise to the same cart
  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 2, strdup("Kunai"), 6), 1); // Adding 6 Kuanais in 2nd cart
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_cart_merchandise_get(cart2), string_elem("Kunai"), &quantity2_elem));
  CU_ASSERT_EQUAL(quantity2_elem.i, 6);

  elem_t reserved_elem;
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_store_reserved_merchs_get(store), string_elem("Kunai"), &reserved_elem));
  CU_ASSERT_EQUAL(reserved_elem.i, 10); // Checking if the reserved kunais are now ten

  // Adding 1 more Kuanais in 2nd cart, shouldn't work bcs the stock is out
  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 2, strdup("Kunai"), 1), -3);

  // Checking if the info is still same
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_cart_merchandise_get(cart1), string_elem("Kunai"), &quantity1_elem));
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_cart_merchandise_get(cart2), string_elem("Kunai"), &quantity2_elem));
  CU_ASSERT_EQUAL(quantity1_elem.i, 4); // Should still be 4 Kunais in the cart
  CU_ASSERT_EQUAL(quantity2_elem.i, 6); // Should still be 6 Kunais in the cart

  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 1123213, strdup("BLA"), 1), -2); // It should fail, cart doesn't exist
  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 1, strdup("BLABLABLA"), 1), -1); // It should fail, merch doesn't exist
  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 1, strdup("BLA"), 1), 1);        // It should succeed

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_cart_merchandise_get(cart1), string_elem("BLA"), &quantity1_elem));
  CU_ASSERT_EQUAL(quantity1_elem.i, 1);

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_store_reserved_merchs_get(store), string_elem("BLA"), &reserved_elem));
  CU_ASSERT_EQUAL(reserved_elem.i, 1);

  ioopm_destroy_store(store);
}

void test_remove_cart(void)
{
  store_t *store = ioopm_create_store();
  ioopm_add_cart(store);
  ioopm_add_cart(store);
  ioopm_add_cart(store);
  ioopm_add_cart(store);

  elem_t cart_elem;

  // Removing a empty cart
  CU_ASSERT_TRUE(ioopm_get_cart(store, 4, &cart_elem));
  CU_ASSERT_TRUE(ioopm_remove_cart(store, 4));
  CU_ASSERT_FALSE(ioopm_get_cart(store, 4, &cart_elem));

  // Removing a non-existent cart
  CU_ASSERT_FALSE(ioopm_get_cart(store, 999, &cart_elem));
  CU_ASSERT_FALSE(ioopm_remove_cart(store, 999));

  ioopm_add_merch(store, ioopm_create_merch(strdup("Kunai"), strdup("Sharp weapon"), 100));
  ioopm_add_merch(store, ioopm_create_merch(strdup("Kaching"), strdup("Another weapon"), 200));
  ioopm_replenish(store, strdup("Kunai"), 10, strdup("ABC"));
  ioopm_replenish(store, strdup("Kaching"), 1, strdup("XYZ"));

  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 1, strdup("Kunai"), 6), 1);
  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 1, strdup("Kaching"), 1), 1);

  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 2, strdup("Kunai"), 5), -3);   // Not enough stock
  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 3, strdup("Kaching"), 1), -3); // Not enough stock

  CU_ASSERT_TRUE(ioopm_remove_cart(store, 1));
  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 2, strdup("Kunai"), 5), 1);   // Now enough stock
  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 3, strdup("Kaching"), 1), 1); // Now enough stock

  ioopm_destroy_store(store);
}

void test_remove_from_cart(void)
{
  store_t *store = ioopm_create_store();

  ioopm_add_merch(store, ioopm_create_merch(strdup("Kunai"), strdup("Sharp weapon"), 100));
  ioopm_replenish(store, strdup("Kunai"), 10, strdup("ABC"));

  ioopm_add_cart(store);
  ioopm_add_cart(store);

  elem_t cart1_elem;
  elem_t cart2_elem;
  elem_t quantity_elem;
  elem_t reserved_elem;

  CU_ASSERT_TRUE(ioopm_get_cart(store, 1, &cart1_elem));
  CU_ASSERT_TRUE(ioopm_get_cart(store, 2, &cart2_elem));

  cart_t *cart1 = cart1_elem.p;

  // removing merch which is not in the cart
  CU_ASSERT_EQUAL(ioopm_remove_from_cart(store, 1, strdup("Kunai"), 1), -2);

  // add 6 Kunai in cart 1, (6 kunais reserved)
  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 1, strdup("Kunai"), 6), 1);

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_store_reserved_merchs_get(store), string_elem("Kunai"), &reserved_elem));
  CU_ASSERT_EQUAL(reserved_elem.i, 6); // 6 kunais reserved

  // cart 2 cannot reserve 5 because only 4 are available
  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 2, strdup("Kunai"), 5), -3);

  // remove 2 Kunai from cart 1
  CU_ASSERT_EQUAL(ioopm_remove_from_cart(store, 1, strdup("Kunai"), 2), 1);

  // cart 1 should now contain 4 Kunai.
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_cart_merchandise_get(cart1), string_elem("Kunai"), &quantity_elem));
  CU_ASSERT_EQUAL(quantity_elem.i, 4); // Should have 4 kunais in cart 1

  // now only 4 kunais are reserved
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_store_reserved_merchs_get(store), string_elem("Kunai"), &reserved_elem));
  CU_ASSERT_EQUAL(reserved_elem.i, 4);

  // now we can add 5 kunais to cart 2
  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 2, strdup("Kunai"), 5), 1);

  // there is not 5 kunais in cart 1
  CU_ASSERT_EQUAL(ioopm_remove_from_cart(store, 1, strdup("Kunai"), 5), -3);

  // cart 1 should still only have 4 kunais
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_cart_merchandise_get(cart1), string_elem("Kunai"), &quantity_elem));
  CU_ASSERT_EQUAL(quantity_elem.i, 4);

  CU_ASSERT_EQUAL(ioopm_remove_from_cart(store, 1, strdup("Kunai"), 4), 1);

  // no kuani in cart 1
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ioopm_cart_merchandise_get(cart1), string_elem("Kunai"), &quantity_elem));

  // cart 2 should still have its 5 Kunai reserved
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_store_reserved_merchs_get(store), string_elem("Kunai"), &reserved_elem));
  CU_ASSERT_EQUAL(reserved_elem.i, 5);

  // the cart is emepty so it shouldn't work
  CU_ASSERT_EQUAL(ioopm_remove_from_cart(store, 1, strdup("Kunai"), 1), -2);

  // removing 5 kunais from cart 2
  CU_ASSERT_EQUAL(ioopm_remove_from_cart(store, 2, strdup("Kunai"), 5), 1);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ioopm_store_reserved_merchs_get(store), string_elem("Kunai"), &reserved_elem));

  ioopm_destroy_store(store);
}

void test_checkout(void)
{
  store_t *store = ioopm_create_store();

  ioopm_add_merch(store, ioopm_create_merch(strdup("Kunai"), strdup("Sharp weapon"), 100));
  ioopm_add_merch(store, ioopm_create_merch(strdup("Kaching"), strdup("Magic item"), 50));

  ioopm_replenish(store, strdup("Kunai"), 10, strdup("ABC"));
  ioopm_replenish(store, strdup("Kaching"), 8, strdup("DEF"));

  ioopm_add_cart(store);
  ioopm_add_cart(store);
  ioopm_add_cart(store);
  ioopm_add_cart(store);

  elem_t merch_elem;
  elem_t cart_elem;
  elem_t reserved_elem;

  ioopm_get_merch(store, "Kunai", &merch_elem);
  merch_t *kunai = merch_elem.p;

  // Empty cart checkout
  CU_ASSERT_EQUAL(ioopm_checkout(store, 1), 0);
  CU_ASSERT_FALSE(ioopm_get_cart(store, 1, &cart_elem)); // Cart is removed after checkout

  ioopm_add_to_cart(store, 2, strdup("Kunai"), 2);
  ioopm_add_to_cart(store, 2, strdup("Kunai"), 2);
  ioopm_add_to_cart(store, 2, strdup("Kaching"), 3);

  // 9 kunais are now reserve cart 2: 4 cart 3: 5
  ioopm_add_to_cart(store, 3, strdup("Kunai"), 5);

  CU_ASSERT_EQUAL(ioopm_calculate_cost(store, 2), 550); // the total cost of cart 2

  // stock should be same
  CU_ASSERT_EQUAL(ioopm_merch_stock_get(kunai), 10);

  ioopm_get_cart(store, 2, &cart_elem);

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_store_reserved_merchs_get(store), string_elem("Kunai"), &reserved_elem));
  CU_ASSERT_EQUAL(reserved_elem.i, 9);

  // 5. Checkout cart 2: 4 Kunai + 3 Kaching = 550
  CU_ASSERT_EQUAL(ioopm_checkout(store, 2), 550);

  //stock should have updated: 10 - 4 
  CU_ASSERT_EQUAL(ioopm_merch_stock_get(kunai), 6); 

  elem_t kaching_elem;
  CU_ASSERT_TRUE(ioopm_get_merch(store, "Kaching", &kaching_elem));
  merch_t *kaching = kaching_elem.p;
  //stock should have updated: 8 - 43
  CU_ASSERT_EQUAL(ioopm_merch_stock_get(kaching), 5);  

  //cart has been removed after checkout
  CU_ASSERT_FALSE(ioopm_get_cart(store, 2, &cart_elem));

  // Cart 3s Kunai reservation must remain while cart 2s should have been removed
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ioopm_store_reserved_merchs_get(store), string_elem("Kunai"), &reserved_elem));
  CU_ASSERT_EQUAL(reserved_elem.i, 5);

  //Checkout cart 3
  CU_ASSERT_EQUAL(ioopm_checkout(store, 3), 500);
  CU_ASSERT_EQUAL(ioopm_merch_stock_get(kunai), 1); //Now one kunai left

  //Now there is no kunai reserved
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ioopm_store_reserved_merchs_get(store), string_elem("Kunai"), &reserved_elem));
  CU_ASSERT_FALSE(ioopm_get_cart(store, 3, &cart_elem));

  CU_ASSERT_EQUAL(ioopm_add_to_cart(store, 4, strdup("Kunai"), 1), 1);
  CU_ASSERT_EQUAL(ioopm_checkout(store, 4), 100);
  CU_ASSERT_EQUAL(ioopm_merch_stock_get(kunai), 0);

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ioopm_store_reserved_merchs_get(store), string_elem("Kunai"), &reserved_elem));

  ioopm_destroy_store(store);
}

int main()
{
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite db = CU_add_suite("db.c", init_suite, clean_suite);
  CU_pSuite cart = CU_add_suite("cart.c", init_suite, clean_suite);
  if (db == NULL || cart == NULL)
  {
    // If the test suite could not be added, tear down CUnit and exit
    CU_cleanup_registry();
    return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if ((
          CU_add_test(db, "Create/destroy", test_create_destroy_store) == NULL ||
          CU_add_test(db, "Create merch", test_make_destroy_merch) == NULL ||
          CU_add_test(db, "Adding merchs", test_add_merchs) == NULL ||
          CU_add_test(db, "Replenish merchs", test_replenish_merchs) == NULL ||
          CU_add_test(db, "Edit desc and price", test_edit_desc_price) == NULL ||
          CU_add_test(db, "Edit name", test_edit_merch_name) == NULL ||
          CU_add_test(db, "Remove merch", test_remove_merch) == NULL ||
          //////////////////////////// CART /////////////////////////////////
          CU_add_test(cart, "Create/destroy", test_create_destroy_cart) == NULL ||
          CU_add_test(cart, "Adding cart", test_add_carts) == NULL ||
          CU_add_test(cart, "Add to cart", test_add_to_cart) == NULL ||
          CU_add_test(cart, "Remove cart", test_remove_cart) == NULL ||
          CU_add_test(cart, "Remove from cart", test_remove_from_cart) == NULL ||
          CU_add_test(cart, "Remove from cart", test_checkout) == NULL ||
          0))
  {
    // If adding any of the tests fails, we tear down CUnit and exit
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_NORMAL);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
}