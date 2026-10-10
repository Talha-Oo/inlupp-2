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