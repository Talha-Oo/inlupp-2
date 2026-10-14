#include "common_hash_table.h"
#include "hash_table.h"
#include "hash_table_iterator.h"
#include <stdbool.h>
#include <CUnit/Basic.h>

static size_t string_knr_hash(const elem_t elem)
{
  size_t result = 0;

  char *str = elem.s;
  while (*str != '\0')
  {
    result = result * 31 + ((unsigned char)*str);
    str++;
  }

  return result;
}

static void destroy(entry_t *entry)
{
  free(entry);
}

static bool str_eq_func(const elem_t elem1, const elem_t elem2)
{
  return strcmp(elem1.s, elem2.s) == 0;
}

static size_t int_hash(const elem_t elem)
{
  return elem.i;
}

static bool int_eq_func(const elem_t elem1, const elem_t elem2)
{
  return elem1.i == elem2.i;
}

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

// These are example test functions. You should replace them with
// functions of your own.
void test_create_destroy()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);
  CU_ASSERT_PTR_NOT_NULL(ht);
  ioopm_hash_table_destroy(ht);
}

void test_insert_once()
{
  // create new hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);

  elem_t key1 = string_elem("abc");
  elem_t value1 = int_elem(123);

  // check that key is not in ht
  elem_t result = int_elem(-1);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, -1);

  // insert key-value pair and check that the mapping exists
  ioopm_hash_table_insert(ht, key1, value1);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, value1.i);

  elem_t key2 = string_elem("abc");
  elem_t value2 = int_elem(456);

  ioopm_hash_table_insert(ht, key2, value2);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result.i, value2.i);

  // destroy hash table
  ioopm_hash_table_destroy(ht);
}

void test_two_keys()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);

  elem_t key1 = string_elem("a");
  elem_t value1 = int_elem(123);

  elem_t key2 = string_elem("b");
  elem_t value2 = int_elem(456);

  ioopm_hash_table_insert(ht, key1, value1);
  ioopm_hash_table_insert(ht, key2, value2);

  elem_t result1;
  elem_t result2;
  ioopm_hash_table_lookup(ht, key1, &result1);
  ioopm_hash_table_lookup(ht, key2, &result2);

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result1));
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result2));

  CU_ASSERT_EQUAL(result1.i, value1.i);
  CU_ASSERT_EQUAL(result2.i, value2.i);

  ioopm_hash_table_destroy(ht);
}

void nonexistent_key_lookup()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);

  elem_t result;
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem("xyz"), &result));

  ioopm_hash_table_destroy(ht);
}

void remove_key()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);

  elem_t key1 = string_elem("a");
  elem_t value1 = int_elem(123);

  elem_t key2 = string_elem("b");
  elem_t value2 = int_elem(456);

  ioopm_hash_table_insert(ht, key1, value1);
  ioopm_hash_table_insert(ht, key2, value2);

  elem_t removed_value;
  bool result = ioopm_hash_table_remove(ht, key1, &removed_value);

  CU_ASSERT_TRUE(result);
  CU_ASSERT_EQUAL(removed_value.i, value1.i);

  elem_t saved_value;
  ioopm_hash_table_lookup(ht, key2, &saved_value);
  CU_ASSERT_EQUAL(saved_value.i, value2.i);

  result = ioopm_hash_table_remove(ht, key1, &removed_value);
  CU_ASSERT_FALSE(result);

  ioopm_hash_table_destroy(ht);
}

void has_key()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);

  elem_t key1 = string_elem("abc");
  elem_t key2 = string_elem("def");
  elem_t key3 = string_elem("ghi");
  elem_t key4 = string_elem("jkl");

  elem_t value1 = int_elem(1);
  elem_t value2 = int_elem(2);
  elem_t value3 = int_elem(3);

  // Test case: 1
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, key1)); // checking a nonexistent key
  ioopm_hash_table_destroy(ht);

  // Test case: 2
  ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);

  ioopm_hash_table_insert(ht, key1, value1);

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key1));  // checking if the key we added exists, it should
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, key2)); // checking a nonexistent key
  ioopm_hash_table_destroy(ht);

  // Test case: 3
  ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);

  ioopm_hash_table_insert(ht, key1, value1);
  ioopm_hash_table_insert(ht, key2, value2);
  ioopm_hash_table_insert(ht, key3, value3);

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key1));  // checking if the key we added exists, it should
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key2));  // checking if the key we added exists, it should
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key3));  // checking if the key we added exists, it should
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, key4)); // checking a nonexistent key

  ioopm_hash_table_destroy(ht);

  // Test case: 4
  ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);

  elem_t removed_value;
  ioopm_hash_table_insert(ht, key1, value1);
  ioopm_hash_table_remove(ht, key1, &removed_value);

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, key1)); // checning if a removed key exists, it shouldn't

  ioopm_hash_table_destroy(ht);

  // Test case: 5
  ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);

  ioopm_hash_table_insert(ht, key1, value1);
  ioopm_hash_table_insert(ht, key2, value2);
  ioopm_hash_table_insert(ht, key3, value3);

  ioopm_hash_table_remove(ht, key2, &removed_value);

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key1));  // checking if the key we added exists, it should
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key3));  // checking if the key we added exists, it should
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, key2)); // checning if a removed key exists, it shouldn't

  ioopm_hash_table_destroy(ht);
}

void size()
{
  // Test 1: Size for empty ht
  ioopm_hash_table_t *ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  ioopm_hash_table_destroy(ht);

  // Test 2: One key
  ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(123));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
  ioopm_hash_table_destroy(ht);

  // Test 3: Add and remove one key
  elem_t removed_value;

  ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(123));
  ioopm_hash_table_remove(ht, string_elem("abc"), &removed_value);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  ioopm_hash_table_destroy(ht);

  // Test 4: Add three keys, remove one
  ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(123));
  ioopm_hash_table_insert(ht, string_elem("def"), int_elem(123));
  ioopm_hash_table_insert(ht, string_elem("ghi"), int_elem(123));
  ioopm_hash_table_remove(ht, string_elem("def"), &removed_value);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 2);
  ioopm_hash_table_destroy(ht);
}

void is_empty()
{
  // Test 1: Size for empty ht
  ioopm_hash_table_t *ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_destroy(ht);

  // Test 2: One key
  ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(123));
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_destroy(ht);

  // Test 3: Add and remove one key
  elem_t removed_value;

  ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(123));
  ioopm_hash_table_remove(ht, string_elem("abc"), &removed_value);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_destroy(ht);

  // Test 4: Add three keys, remove one
  ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(123));
  ioopm_hash_table_insert(ht, string_elem("def"), int_elem(123));
  ioopm_hash_table_insert(ht, string_elem("ghi"), int_elem(123));
  ioopm_hash_table_remove(ht, string_elem("def"), &removed_value);
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_destroy(ht);
}

void insert_int_key_str_value()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(&int_hash, &int_eq_func, &destroy);

  elem_t key = int_elem(123);
  elem_t value = string_elem("abc");
  ioopm_hash_table_insert(ht, key, value);

  elem_t lookup_value;
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &lookup_value));
  CU_ASSERT_EQUAL(lookup_value.s, value.s);

  ioopm_hash_table_destroy(ht);
}

// ITERATOR TESTS

void iterating_over_empty_hash_table()
{

  // Test 1: Empty hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

  ioopm_hash_table_destroy(ht);
  ioopm_hash_table_iterator_destroy(it);
}

void iterating_over_hash_table_one_elm()
{

  ioopm_hash_table_t *ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  while (!ioopm_hash_table_iterator_at_end(it))
  {
    elem_t value = ioopm_hash_table_iterator_current_value(it);
    elem_t key = ioopm_hash_table_iterator_current_key(it);

    CU_ASSERT_EQUAL(value.i, 1);

    CU_ASSERT_EQUAL(key.s, "abc");

    ioopm_hash_table_iterator_advance(it);
  }

  ioopm_hash_table_destroy(ht);
  ioopm_hash_table_iterator_destroy(it);
}

void iterating_over_hash_table_mult_elm()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(&string_knr_hash, &str_eq_func, &destroy);
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));
  ioopm_hash_table_insert(ht, string_elem("def"), int_elem(20));
  ioopm_hash_table_insert(ht, string_elem("ghi"), int_elem(300));

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  int sum = 0;
  int iteration_count = 0;
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    elem_t value = ioopm_hash_table_iterator_current_value(it);

    sum += value.i;
    iteration_count++;

    ioopm_hash_table_iterator_advance(it);
  }

  CU_ASSERT_EQUAL(sum, 321);
  CU_ASSERT_EQUAL(iteration_count, 3);

  ioopm_hash_table_destroy(ht);
  ioopm_hash_table_iterator_destroy(it);
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
  if (
      (CU_add_test(my_test_suite, "Create/destroy", test_create_destroy) == NULL ||
       CU_add_test(my_test_suite, "Insert", test_insert_once) == NULL ||
       CU_add_test(my_test_suite, "Different keys insert & lookup", test_two_keys) == NULL ||
       CU_add_test(my_test_suite, "Nonexistent key lookup", nonexistent_key_lookup) == NULL) ||
      CU_add_test(my_test_suite, "Remove key", remove_key) == NULL ||
      CU_add_test(my_test_suite, "has_key", has_key) == NULL ||
      CU_add_test(my_test_suite, "size", size) == NULL ||
      CU_add_test(my_test_suite, "check if the hash table is empty", is_empty) == NULL ||
      CU_add_test(my_test_suite, "insert int key with string value", insert_int_key_str_value) == NULL ||

      CU_add_test(my_test_suite, "testing iterator", iterating_over_empty_hash_table) == NULL ||
      CU_add_test(my_test_suite, "testing iterator, one elm", iterating_over_hash_table_one_elm) == NULL ||
      CU_add_test(my_test_suite, "testing iterator, mult elm", iterating_over_hash_table_mult_elm) == NULL ||
      0)
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