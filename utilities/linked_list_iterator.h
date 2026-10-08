#pragma once
#include <stdbool.h>

#include "common_linked_list.h"
#include "linked_list.h"

typedef struct list_iterator ioopm_list_iterator_t;

/// @brief Create a new iterator
/// @param l the list to iterate over
ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *list);

/// @brief Destroy the iterator and return its resources
/// @param iterator the iterator
void ioopm_list_iterator_destroy(ioopm_list_iterator_t *iterator);

/// @brief Checks if there are more elements to iterate over
/// @param iterator the iterator
/// @return true if there is at least one more element
bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *iterator);

/// @brief Step the iterator forward one step
/// @param iterator the iterator
void ioopm_list_iterator_advance(ioopm_list_iterator_t *iterator);

/// @brief Return the current element from the underlying list
/// @param iterator the iterator
/// @return the current element
bool ioopm_list_iterator_current(ioopm_list_iterator_t *iterator, elem_t *elem);

/// @brief Remove the current element from the underlying list
/// @param iterator the iterator
/// @return the removed element
bool ioopm_list_iterator_remove(ioopm_list_iterator_t *iterator, elem_t *elem);

/// @brief Insert a new element into the underlying list making the current element it's next
/// @param iterator the iterator
/// @param element the element to be inserted
void ioopm_list_iterator_insert(ioopm_list_iterator_t *iterator, elem_t element);