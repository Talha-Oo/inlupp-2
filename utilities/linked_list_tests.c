#include "linked_list.h"
#include "linked_list_iterator.h"
#include "common_linked_list.h"

#include <stdbool.h>
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

/// Linked list tests ///

void test_create_destroy()
{
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_destroy(list);
}

void append_one_elem()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, int_elem(123));
  elem_t last_value = ioopm_list_last(list);

  CU_ASSERT_EQUAL(last_value.i, 123);
  ioopm_list_destroy(list);
}

void prepend_one_elem()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_prepend(list, int_elem(123));
  elem_t first_value = ioopm_list_head(list);

  CU_ASSERT_EQUAL(first_value.i, 123);
  ioopm_list_destroy(list);
}

void append_prepend_mult_elem()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, int_elem(456));
  ioopm_list_prepend(list, int_elem(123));
  ioopm_list_append(list, int_elem(789));

  elem_t first_value = ioopm_list_head(list);
  elem_t last_value = ioopm_list_last(list);

  CU_ASSERT_EQUAL(first_value.i, 123);
  CU_ASSERT_EQUAL(last_value.i, 789);
  ioopm_list_destroy(list);
}

void size_mult_elem()
{
  elem_t removed_value;
  ioopm_list_t *list = ioopm_list_create();

  CU_ASSERT_EQUAL(ioopm_list_size(list), 0);
  ioopm_list_append(list, int_elem(456));

  CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
  ioopm_list_prepend(list, int_elem(123));

  CU_ASSERT_EQUAL(ioopm_list_size(list), 2);
  ioopm_list_append(list, int_elem(789));

  CU_ASSERT_EQUAL(ioopm_list_size(list), 3);

  removed_value = ioopm_list_remove(list, 0);
  CU_ASSERT_EQUAL(ioopm_list_size(list), 2);
  CU_ASSERT_EQUAL(removed_value.i, 123);

  removed_value = ioopm_list_remove(list, 1);
  CU_ASSERT_EQUAL(ioopm_list_size(list), 1);
  CU_ASSERT_EQUAL(removed_value.i, 789);

  removed_value = ioopm_list_remove(list, 0);
  CU_ASSERT_EQUAL(ioopm_list_size(list), 0);
  CU_ASSERT_EQUAL(removed_value.i, 456);

  ioopm_list_destroy(list);
}

void insert_mult_elem()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_insert(list, 0, int_elem(456));

  ioopm_list_insert(list, 0, int_elem(123));

  ioopm_list_insert(list, 2, int_elem(789));

  elem_t first_value = ioopm_list_head(list);
  CU_ASSERT_EQUAL(first_value.i, 123);

  elem_t last_value = ioopm_list_last(list);
  CU_ASSERT_EQUAL(last_value.i, 789);

  ioopm_list_destroy(list);
}

void is_empty()
{
  ioopm_list_t *list = ioopm_list_create();
  CU_ASSERT_TRUE(ioopm_list_is_empty(list));

  ioopm_list_insert(list, 0, int_elem(456));
  CU_ASSERT_FALSE(ioopm_list_is_empty(list));
  ioopm_list_remove(list, 0);
  CU_ASSERT_TRUE(ioopm_list_is_empty(list));
  ioopm_list_insert(list, 0, int_elem(123));
  CU_ASSERT_FALSE(ioopm_list_is_empty(list));
  ioopm_list_insert(list, 1, int_elem(789));
  CU_ASSERT_FALSE(ioopm_list_is_empty(list));

  ioopm_list_destroy(list);
}

void removed_values()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, int_elem(456));
  ioopm_list_prepend(list, int_elem(123));
  ioopm_list_append(list, int_elem(789));

  elem_t removed_value;

  removed_value = ioopm_list_remove(list, 2);

  CU_ASSERT_EQUAL(ioopm_list_last(list).i, 456);

  CU_ASSERT_EQUAL(removed_value.i, 789);

  removed_value = ioopm_list_remove(list, 0);
  CU_ASSERT_EQUAL(removed_value.i, 123);
  CU_ASSERT_EQUAL(ioopm_list_head(list).i, 456);

  removed_value = ioopm_list_remove(list, 0);
  CU_ASSERT_EQUAL(removed_value.i, 456);

  CU_ASSERT_EQUAL(ioopm_list_size(list), 0);

  ioopm_list_destroy(list);
}

void get_values()
{
  ioopm_list_t *list = ioopm_list_create();

  ioopm_list_append(list, int_elem(456));
  ioopm_list_prepend(list, int_elem(123));
  ioopm_list_append(list, int_elem(789));

  elem_t value;

  value = ioopm_list_get(list, 0);
  CU_ASSERT_EQUAL(value.i, 123);

  value = ioopm_list_get(list, 1);
  CU_ASSERT_EQUAL(value.i, 456);

  value = ioopm_list_get(list, 2);
  CU_ASSERT_EQUAL(value.i, 789);

  ioopm_list_destroy(list);
}

// /// Iterator tests ///

void test_create_destroy_iterator()
{
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_iterator_t *iterator = ioopm_list_iterator_create(list);

  ioopm_list_iterator_destroy(iterator);
  ioopm_list_destroy(list);
}

void test_iterator_at_end_empty_list()
{
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_iterator_t *iterator = ioopm_list_iterator_create(list);

  CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iterator));

  ioopm_list_iterator_destroy(iterator);
  ioopm_list_destroy(list);
}

void test_iterator_at_end_nonempty_list()
{
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_append(list, int_elem(1337));

  ioopm_list_iterator_t *iterator = ioopm_list_iterator_create(list);

  CU_ASSERT_FALSE(ioopm_list_iterator_at_end(iterator));

  ioopm_list_iterator_advance(iterator);

  CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iterator));

  ioopm_list_iterator_destroy(iterator);
  ioopm_list_destroy(list);
}

void test_iterator_advance_and_get_values()
{
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_append(list, int_elem(1337));
  ioopm_list_append(list, int_elem(420));
  ioopm_list_append(list, int_elem(67));

  elem_t elem;
  ioopm_list_iterator_t *iterator = ioopm_list_iterator_create(list);

  ioopm_list_iterator_current(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 1337);

  ioopm_list_iterator_advance(iterator);

  ioopm_list_iterator_current(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 420);

  ioopm_list_iterator_advance(iterator);

  ioopm_list_iterator_current(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 67);

  ioopm_list_iterator_destroy(iterator);
  ioopm_list_destroy(list);
}

void test_iterator_remove_one_element()
{
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_append(list, int_elem(1337));
  ioopm_list_append(list, int_elem(420));
  ioopm_list_append(list, int_elem(67));

  elem_t elem;
  ioopm_list_iterator_t *iterator = ioopm_list_iterator_create(list);

  ioopm_list_iterator_advance(iterator);

  ioopm_list_iterator_remove(iterator, &elem);

  CU_ASSERT_EQUAL(elem.i, 420);

  ioopm_list_iterator_current(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 67);

  ioopm_list_iterator_destroy(iterator);
  ioopm_list_destroy(list);
}

void test_iterator_remove_multiple_elements()
{
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_append(list, int_elem(1337));
  ioopm_list_append(list, int_elem(420));
  ioopm_list_append(list, int_elem(67));

  elem_t elem;
  ioopm_list_iterator_t *iterator = ioopm_list_iterator_create(list);

  ioopm_list_iterator_remove(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 1337);

  ioopm_list_iterator_current(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 420);

  ioopm_list_iterator_remove(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 420);

  ioopm_list_iterator_current(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 67);

  ioopm_list_iterator_remove(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 67);

  CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iterator));

  ioopm_list_iterator_destroy(iterator);
  ioopm_list_destroy(list);
}

void test_iterator_insert_one_element()
{
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_iterator_t *iterator = ioopm_list_iterator_create(list);
  elem_t elem;

  ioopm_list_iterator_insert(iterator, int_elem(666));

  ioopm_list_iterator_current(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 666);

  ioopm_list_iterator_destroy(iterator);
  ioopm_list_destroy(list);
}

void test_iterator_insert_multiple_elements()
{
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_iterator_t *iterator = ioopm_list_iterator_create(list);
  elem_t elem;

  ioopm_list_iterator_insert(iterator, int_elem(666));

  ioopm_list_iterator_current(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 666);

  ioopm_list_iterator_insert(iterator, int_elem(777));

  ioopm_list_iterator_current(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 777);

  ioopm_list_iterator_insert(iterator, int_elem(888));

  ioopm_list_iterator_current(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 888);

  ioopm_list_iterator_advance(iterator);

  ioopm_list_iterator_current(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 777);

  ioopm_list_iterator_advance(iterator);

  ioopm_list_iterator_current(iterator, &elem);
  CU_ASSERT_EQUAL(elem.i, 666);

  ioopm_list_iterator_advance(iterator);
  CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iterator));

  ioopm_list_iterator_destroy(iterator);
  ioopm_list_destroy(list);
}

void test_iterator_insert_remove_test()
{
  ioopm_list_t *linked_list = ioopm_list_create();

  ioopm_list_append(linked_list, int_elem(1));
  ioopm_list_append(linked_list, int_elem(3));

  ioopm_list_iterator_t *it = ioopm_list_iterator_create(linked_list);

  ioopm_list_iterator_advance(it);

  ioopm_list_iterator_insert(it, int_elem(2));

  elem_t elem;

  ioopm_list_iterator_current(it, &elem);
  CU_ASSERT_EQUAL(elem.i, 2);
  CU_ASSERT_EQUAL(ioopm_list_size(linked_list), 3);

  ioopm_list_iterator_remove(it, &elem);
  CU_ASSERT_EQUAL(elem.i, 2);

  ioopm_list_iterator_advance(it);

  CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));

  CU_ASSERT_EQUAL(ioopm_list_get(linked_list, 0).i, 1);
  CU_ASSERT_EQUAL(ioopm_list_get(linked_list, 1).i, 3);
  CU_ASSERT_EQUAL(ioopm_list_size(linked_list), 2);

  ioopm_list_iterator_destroy(it);
  ioopm_list_destroy(linked_list);
}

void test_iterator_insert_string_element()
{
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_iterator_t *iterator = ioopm_list_iterator_create(list);

  char *c = "hej";
  elem_t elem = string_elem(c);
  ioopm_list_iterator_insert(iterator, elem);

  ioopm_list_iterator_current(iterator, &elem);
  CU_ASSERT_EQUAL(strcmp(c, elem.s), 0);

  ioopm_list_iterator_destroy(iterator);
  ioopm_list_destroy(list);
}

void test_iterator_insert_void_element()
{
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_iterator_t *iterator = ioopm_list_iterator_create(list);

  char *my_string = "hej";
  void *ptr = my_string;
  elem_t elem = ptr_elem(ptr);
  ioopm_list_iterator_insert(iterator, elem);

  elem_t result;
  ioopm_list_iterator_current(iterator, &result);
  char *string_conversion = result.p;
  CU_ASSERT_EQUAL(strcmp(string_conversion, "hej"), 0);

  ioopm_list_iterator_destroy(iterator);
  ioopm_list_destroy(list);
}

static void test_iterator_advance_at_end(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);

    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));

    ioopm_list_iterator_advance(it);

    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list);
}

static void test_iterator_current_at_end(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);
    elem_t elem;

    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));
    CU_ASSERT_FALSE(ioopm_list_iterator_current(it, &elem));

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list);
}

static void test_iterator_remove_at_end(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);
    elem_t elem;

    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));
    CU_ASSERT_FALSE(ioopm_list_iterator_remove(it, &elem));

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list);
}

/// Main function ///

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
          CU_add_test(my_test_suite, "Append one elem", append_one_elem) == NULL ||
          CU_add_test(my_test_suite, "Prepend one elem", prepend_one_elem) == NULL ||
          CU_add_test(my_test_suite, "Append & Prepend mult elements", append_prepend_mult_elem) == NULL ||
          CU_add_test(my_test_suite, "Check size when adding multiple elements", size_mult_elem) == NULL ||
          CU_add_test(my_test_suite, "Insert mult elems", insert_mult_elem) == NULL ||
          CU_add_test(my_test_suite, "Insert elems and check if empty", is_empty) == NULL ||
          CU_add_test(my_test_suite, "Testing to remove values", removed_values) == NULL ||
          CU_add_test(my_test_suite, "Testing to get values", get_values) == NULL ||

          CU_add_test(my_test_suite, "Create and destroy a list iterator", test_create_destroy_iterator) == NULL ||
          CU_add_test(my_test_suite, "Check that the iterator is at the end of an empty list", test_iterator_at_end_empty_list) == NULL ||
          CU_add_test(my_test_suite, "Check that the iterator is at the end of a nonempty list", test_iterator_at_end_nonempty_list) == NULL ||
          CU_add_test(my_test_suite, "Advance the iterator through a list and get its values", test_iterator_advance_and_get_values) == NULL ||
          CU_add_test(my_test_suite, "Check that the iterator removes an entry", test_iterator_remove_one_element) == NULL ||
          CU_add_test(my_test_suite, "Check that the iterator removes multiple entries", test_iterator_remove_multiple_elements) == NULL ||
          CU_add_test(my_test_suite, "Check that the iterator inserts one element", test_iterator_insert_one_element) == NULL ||
          CU_add_test(my_test_suite, "Check that the iterator inserts multiple entries", test_iterator_insert_multiple_elements) == NULL ||
          CU_add_test(my_test_suite, "Multipule inserts and removes", test_iterator_insert_remove_test) == NULL ||
          CU_add_test(my_test_suite, "Insert string elem", test_iterator_insert_string_element) == NULL ||
          CU_add_test(my_test_suite, "Insert void elem", test_iterator_insert_void_element) == NULL ||
          CU_add_test(my_test_suite, "advance at end", test_iterator_advance_at_end) == NULL ||
          CU_add_test(my_test_suite, "current at end", test_iterator_current_at_end) == NULL ||
          CU_add_test(my_test_suite, "remove at end", test_iterator_remove_at_end) == NULL ||
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