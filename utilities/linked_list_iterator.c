#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "common_linked_list.h"
#include "linked_list.h"
#include "linked_list_iterator.h"

// Create data structures and define their types

typedef struct entry
{
    elem_t value;
    struct entry *next;

} entry_t;

typedef struct list
{
    entry_t *sentinel;
    entry_t *last;
    size_t size;

} ioopm_list_t;

typedef struct list_iterator
{
    ioopm_list_t *list;
    entry_t *current_entry;
    entry_t *previous_entry;
} ioopm_list_iterator_t;

// Iterator functions

ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *list)
{
    // Create iterator
    ioopm_list_iterator_t *iterator = calloc(1, sizeof(ioopm_list_iterator_t));

    // Link iterator to given list
    iterator->list = list;

    // Make iterator point to first list element
    iterator->current_entry = list->sentinel->next;

    // Let iterator know that the previous entry is the sentinel node
    iterator->previous_entry = list->sentinel;

    return iterator;
}

void ioopm_list_iterator_destroy(ioopm_list_iterator_t *iterator)
{
    // Make available the heap space that the iterator is taking up
    free(iterator);
}

bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *iterator)
{
    if (iterator->current_entry == NULL)
    {
        // The iterator is pointing at the end of the list
        return true;
    }
    else
    {
        // The iterator is pointing at an entry
        return false;
    }
}

void ioopm_list_iterator_advance(ioopm_list_iterator_t *iterator)
{
    // Failsafe if iterator is at end of list
    if (iterator->current_entry == NULL)
    {
        puts("ERROR: The iterator is at the end of the list.");
        return;
    }

    // Set the new previous entry, to the old current entry
    iterator->previous_entry = iterator->current_entry;

    // Set the new current entry, to the entry after the old current entry
    iterator->current_entry = iterator->current_entry->next;
}

bool ioopm_list_iterator_current(ioopm_list_iterator_t *iterator, elem_t *elem)
{
    // Failsafe if iterator, is at end of list
    if (iterator->current_entry == NULL)
    {
        puts("ERROR: The iterator is at the end of the list.");
        return false;
    }

    // Return the value of the entry that the iterator is currently pointing at
    *elem = iterator->current_entry->value;
    return true;
}

bool ioopm_list_iterator_remove(ioopm_list_iterator_t *iterator, elem_t *elem)
{
    // Failsafe if iterator is at end of list
    if (iterator->current_entry == NULL)
    {
        puts("ERROR: The iterator is at the end of the list.");
        return false;
    }

    // Label the iterator's current entry, as the entry to remove
    entry_t *removed_entry = iterator->current_entry;

    // Make the previous entry point, to the entry after the removed entry
    iterator->previous_entry->next = removed_entry->next;

    // Set the iterator's new current entry, to the entry after the removed entry
    iterator->current_entry = removed_entry->next;

    // Save the value of the removed entry
    elem_t removed_value = removed_entry->value;

    // Make the removed entry's heap space available
    free(removed_entry);
    iterator->list->size--;

    *elem = removed_value;
    return true;
}

// Helper function for the insertion function
static entry_t *ioopm_entry_create(elem_t value)
{
    entry_t *entry = calloc(1, sizeof(entry_t));
    entry->value = value;
    return entry;
}

void ioopm_list_iterator_insert(ioopm_list_iterator_t *iterator, elem_t element)
{
    // Create a new entry with the specified value
    entry_t *inserted_entry = ioopm_entry_create(element);

    // Set the new entry to precede the iterator's current entry
    inserted_entry->next = iterator->current_entry;

    // Set the iterator's previous entry to precede the new entry
    iterator->previous_entry->next = inserted_entry;

    // Set the iterator's current entry to the new entry
    iterator->current_entry = inserted_entry;
    iterator->list->size++;
}