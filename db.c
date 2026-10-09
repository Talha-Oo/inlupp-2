#include "utilities/common.h"

#include <string.h>

static size_t string_knr_hash(const elem_t elem)
{
  size_t result = 2;

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