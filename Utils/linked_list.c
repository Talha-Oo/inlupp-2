#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "common_linked_list.h"
#include "linked_list.h"

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

ioopm_list_t *ioopm_list_create(void)
{
  ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
  list->sentinel = calloc(1, sizeof(entry_t));

  list->last = list->sentinel;
  return list;
}

static entry_t *ioopm_entry_create(elem_t value)
{
  entry_t *entry = calloc(1, sizeof(entry_t));
  entry->value = value;
  return entry;
}

void ioopm_entry_destroy(entry_t *entry)
{
  free(entry);
}

void ioopm_list_destroy(ioopm_list_t *list)
{

  entry_t *current = list->sentinel;

  entry_t *temp;
  while (current != NULL)
  {
    temp = current;
    current = current->next;

    ioopm_entry_destroy(temp);
  }

  free(list);
}

void ioopm_list_append(ioopm_list_t *list, elem_t value)
{
  entry_t *last = list->last;
  entry_t *entry = ioopm_entry_create(value);

  //Make the current last point at the new entry and make it the new last
  last->next = entry;
  list->last = entry;

  list->size++;
}

void ioopm_list_prepend(ioopm_list_t *list, elem_t value)
{
  entry_t *sentinel = list->sentinel;
  entry_t *entry = ioopm_entry_create(value);

  //Make the entry point to the current first node (the node after sentinal node)
  //Make the sentinal node point at the entry making it the new first logical node
  entry->next = sentinel->next;
  sentinel->next = entry;
  list->size++;
}

elem_t ioopm_list_last(ioopm_list_t *list)
{
  assert(list->last != NULL && "THE LIST IS EMPTY!!");
  return list->last->value;
}

elem_t ioopm_list_head(ioopm_list_t *list)
{
  assert(list->sentinel->next != NULL && "THE LIST IS EMPTY!!");
  entry_t *first = list->sentinel->next;
  return first->value;
}

static entry_t *find_previous_entry(ioopm_list_t *list, size_t index)
{
  entry_t *current = list->sentinel;

  //Go to the entry just before the entry at the given index
  for (size_t i = 0; i < index; i++)
  {
    current = current->next;
  }

  return current;
}

void ioopm_list_insert(ioopm_list_t *list, size_t index, elem_t value)
{
  assert(index <= list->size && "INVALID INDEX");

  entry_t *entry = ioopm_entry_create(value);

  entry_t *prev = find_previous_entry(list, index);

  entry->next = prev->next;
  //If there is no element after prev
  if (prev->next == NULL)
  {
    //Make last point at the enrty to be added
    list->last = entry;
  }

  //make prev point to the entry to be added
  prev->next = entry;

  list->size++;
}

elem_t ioopm_list_remove(ioopm_list_t *list, size_t index)
{
  assert(index < list->size && "INVALID INDEX");
  entry_t *prev = find_previous_entry(list, index);

  entry_t *current = prev->next; //the element to be removed

  elem_t removed_value = current->value;

  //Jump over the current elem but linking the prev element to currents next element
  prev->next = current->next;

  //If there is no other element after the elemnt we are trying to remove
  if (current->next == NULL)
  {
    list->last = prev; //Make the previous element last 
  }

  ioopm_entry_destroy(current);

  list->size--;

  return removed_value;
}

elem_t ioopm_list_get(ioopm_list_t *list, size_t index)
{
  entry_t *prev = find_previous_entry(list, index);
  entry_t *current = prev->next;

  return current->value;
}

size_t ioopm_list_size(ioopm_list_t *list)
{
  return list->size;
}

bool ioopm_list_is_empty(ioopm_list_t *list)
{
  return list->size == 0;
}