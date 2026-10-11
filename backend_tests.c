#include "utilities/common.h"
#include "main/db.h"

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
void test_create_destroy()
{
  store_t *store = create_store();
  destroy_store(store);
}

void test_make_destroy_merch()
{
  char *name = strdup("T");
  char *desc = strdup("TB");

  merch_t *merch = create_merch(name, desc, 123);
  
  CU_ASSERT_EQUAL(strcmp(name, merch->name), 0);
  CU_ASSERT_EQUAL(strcmp(desc, merch->desc), 0);
  CU_ASSERT_EQUAL(merch->price, 123);

  destroy_merch(merch);
}

void test_add_merchs()
{
  store_t *store = create_store();
  ioopm_hash_table_t *ht = store->name_to_item;
  
  elem_t looked_up_elem;

  char *name1 = strdup("T");
  char *desc1 = strdup("TB");
  merch_t *merch1 = create_merch(name1, desc1, 123);
  
  //Adding a new merchendise
  bool add_res = add_merch(store, merch1);

  elem_t key1 = string_elem("T");
  bool lookup_res = ioopm_hash_table_lookup(ht, key1, &looked_up_elem);
  
  CU_ASSERT_TRUE(add_res);
  CU_ASSERT_TRUE(lookup_res);

  //Checking if we can read the stored merchendise with lookup 
  merch_t *looked_up_merch = looked_up_elem.p;
  CU_ASSERT_EQUAL(strcmp(name1, looked_up_merch->name), 0);
  CU_ASSERT_EQUAL(strcmp(desc1, looked_up_merch->desc), 0);
  CU_ASSERT_EQUAL(looked_up_merch->price, 123);

  //Adding a merchendise which already exists
  char *name2 = strdup("T");
  char *desc2 = strdup("TB");
  merch_t *merch2 = create_merch(name2, desc2, 123);

  add_res = add_merch(store, merch2);
  lookup_res = ioopm_hash_table_lookup(ht, key1, &looked_up_elem);

  CU_ASSERT_FALSE(add_res);
  CU_ASSERT_TRUE(lookup_res);

  //Checking if information is still the same 
  looked_up_merch = looked_up_elem.p;
  CU_ASSERT_EQUAL(strcmp(name1, looked_up_merch->name), 0);
  CU_ASSERT_EQUAL(strcmp(desc1, looked_up_merch->desc), 0);
  CU_ASSERT_EQUAL(looked_up_merch->price, 123);

  //Adding a new merchendise with similar name to a one which already exists
  char *old_name = strdup("T");
  char *new_desc = strdup("YO");
  merch_t *merch3 = create_merch(old_name, new_desc, 123123);

  add_res = add_merch(store, merch3);
  lookup_res = ioopm_hash_table_lookup(ht, key1, &looked_up_elem);

  CU_ASSERT_FALSE(add_res);
  CU_ASSERT_TRUE(lookup_res);

  //Checking if information is still the same 
  looked_up_merch = looked_up_elem.p;
  CU_ASSERT_EQUAL(strcmp(name1, looked_up_merch->name), 0);
  CU_ASSERT_EQUAL(strcmp(desc1, looked_up_merch->desc), 0);
  CU_ASSERT_EQUAL(looked_up_merch->price, 123);

  //Adding a new completely merchendise
  char *new_name = strdup("YO!");
  new_desc = strdup("YO");
  merch_t *merch4 = create_merch(new_name, new_desc, 123123);

  add_res = add_merch(store, merch4);

  elem_t key2 = string_elem("YO!");
  lookup_res = ioopm_hash_table_lookup(ht, key2, &looked_up_elem);

  CU_ASSERT_TRUE(add_res);
  CU_ASSERT_TRUE(lookup_res);

  //Checking if information is matches 
  looked_up_merch = looked_up_elem.p;
  CU_ASSERT_EQUAL(strcmp(new_name, looked_up_merch->name), 0);
  CU_ASSERT_EQUAL(strcmp(new_desc, looked_up_merch->desc), 0);
  CU_ASSERT_EQUAL(looked_up_merch->price, 123123);

  destroy_store(store);
}

void test_replenish_merchs()
{
  store_t *store = create_store();
  ioopm_hash_table_t *name_to_item = store->name_to_item;
  ioopm_hash_table_t *shelf_to_item = store->shelf_to_item;
  
  char *name1 = strdup("Kunai");
  char *desc1 = strdup("Kaching");
  merch_t *merch1 = create_merch(name1, desc1, 123);
  elem_t name1_elem = string_elem("Kunai");

  char* name2 = strdup("Kaching");
  char* desc2 = strdup("Kunai");
  elem_t name2_elem = string_elem("Kaching");
  merch_t *merch2 = create_merch(name2, desc2, 321);

  add_merch(store, merch2);
  add_merch(store, merch1);

  elem_t merch1_elem;
  elem_t merch2_elem;
  CU_ASSERT_TRUE(get_merch(name_to_item, name1_elem, &merch1_elem));
  CU_ASSERT_TRUE(get_merch(name_to_item, name2_elem, &merch2_elem));

  ioopm_list_t *merch1_list = merch1->locs;
  ioopm_list_t *merch2_list = merch2->locs;

  replenish(store, name1, 1, strdup("ABC"));
  CU_ASSERT_EQUAL(ioopm_list_size(merch1_list), 1); // check if shelf is added in the list of kunai 
  CU_ASSERT_EQUAL(ioopm_list_size(merch2_list), 0); // list of Kaching should still be emepty 
  
  replenish(store, name2, 2, strdup("XYZ"));
  CU_ASSERT_EQUAL(ioopm_list_size(merch2_list), 1); // check if shelf is added in the list of kaching 

  //checking if the shelf has been added to the hashtable and if the info matches correctly with the
  // shelf in the list, in the hashtable and the merch itself
  //Kunai:
  elem_t shelf_elem = ioopm_list_head(merch1_list);
  shelf_t *shelf = shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(shelf->shelf_name, "ABC"), 0);
  CU_ASSERT_EQUAL(strcmp(shelf->merch_name, merch1->name), 0);
  CU_ASSERT_EQUAL(merch1->stock, 1);

  elem_t looked_up_shelf_elem;
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(shelf_to_item, string_elem("ABC"), &looked_up_shelf_elem));
  
  shelf_t *looked_up_shelf = looked_up_shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(looked_up_shelf->shelf_name, "ABC"), 0);
  CU_ASSERT_EQUAL(strcmp(looked_up_shelf->merch_name, merch1->name), 0);
  CU_ASSERT_EQUAL(looked_up_shelf->quantity, 1);

  //checking if the shelf has been added to the hashtable and if the info matches correctly with the
  // shelf in the list, in the hashtable and the merch itself
  //Kaching:
  shelf_elem = ioopm_list_head(merch2_list);
  shelf = shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(shelf->shelf_name, "XYZ"), 0);
  CU_ASSERT_EQUAL(strcmp(shelf->merch_name, merch2->name), 0);
  CU_ASSERT_EQUAL(merch2->stock, 2);

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(shelf_to_item, string_elem("XYZ"), &looked_up_shelf_elem));
  
  looked_up_shelf = looked_up_shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(looked_up_shelf->shelf_name, "XYZ"), 0);
  CU_ASSERT_EQUAL(strcmp(looked_up_shelf->merch_name, merch2->name), 0);
  CU_ASSERT_EQUAL(looked_up_shelf->quantity, 2);

  //Trying to add stock to a shelf of different type of merch
  CU_ASSERT_FALSE(replenish(store, "Kaching", 1000000, strdup("ABC")));
  CU_ASSERT_FALSE(replenish(store, "Kunai", 1000000, strdup("XYZ")));
  
  //Trying to add stock to a shelf of a nonexistent merch
  CU_ASSERT_FALSE(replenish(store, "YO!", 128936412, strdup("LMNOP")));

  //adding stock to a shelfwith correct type
  CU_ASSERT_TRUE(replenish(store, "Kunai", 1000000, strdup("ABC")));
  //checking if the shelf has been updated in the hashtable and if the info matches correctly with the
  // shelf in the list, in the hashtable and the merch itself
  //Kunai:
  shelf_elem = ioopm_list_head(merch1_list);
  shelf = shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(shelf->shelf_name, "ABC"), 0);
  CU_ASSERT_EQUAL(strcmp(shelf->merch_name, merch1->name), 0);
  CU_ASSERT_EQUAL(merch1->stock, 1000001);

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(shelf_to_item, string_elem("ABC"), &looked_up_shelf_elem));
  
  looked_up_shelf = looked_up_shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(looked_up_shelf->shelf_name, "ABC"), 0);
  CU_ASSERT_EQUAL(strcmp(looked_up_shelf->merch_name, merch1->name), 0);
  CU_ASSERT_EQUAL(looked_up_shelf->quantity, 1000001);

  //Having two different shelves
  replenish(store, "Kunai", 1, strdup("LMNOP"));
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(shelf_to_item, string_elem("LMNOP"), &looked_up_shelf_elem));
  looked_up_shelf = looked_up_shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(looked_up_shelf->shelf_name, "LMNOP"), 0);
  CU_ASSERT_EQUAL(strcmp(looked_up_shelf->merch_name, merch1->name), 0);
  CU_ASSERT_EQUAL(looked_up_shelf->quantity, 1);

  //Checking if the new shelf is in the list 
  shelf_elem = ioopm_list_head(merch1_list);
  shelf = shelf_elem.p;
  CU_ASSERT_EQUAL(strcmp(shelf->shelf_name, "LMNOP"), 0);
  CU_ASSERT_EQUAL(strcmp(shelf->merch_name, merch1->name), 0);
  CU_ASSERT_EQUAL(merch1->stock, 1000002);
  CU_ASSERT_EQUAL(ioopm_list_size(merch1_list), 2);

  destroy_store(store);
}

int main()
{
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL)
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
          CU_add_test(my_test_suite, "Create/destroy", test_create_destroy) == NULL ||
          CU_add_test(my_test_suite, "Create merch", test_make_destroy_merch) == NULL ||
          CU_add_test(my_test_suite, "Adding merchs", test_add_merchs) == NULL ||
          CU_add_test(my_test_suite, "Replenish merchs", test_replenish_merchs) == NULL ||
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