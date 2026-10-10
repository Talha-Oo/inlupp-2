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

void make_merch()
{
  store_t *store = create_store();

  char *name = strdup("T");
  char *desc = strdup("TB");

  entry_t *entry = calloc(1, sizeof(entry_t));
  merch_t *merch = create_merch(name, desc, 123);
  
  CU_ASSERT_EQUAL(strcmp(name, merch->name), 0);
  CU_ASSERT_EQUAL(strcmp(desc, merch->desc), 0);
  CU_ASSERT_EQUAL(merch->price, 123);

  entry->value.p = merch;
  destroy_merch(entry);
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
          CU_add_test(my_test_suite, "Create merch", make_merch) == NULL ||
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