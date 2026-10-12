#pragma once

#define int_elem(x) ((elem_t){.i = (x)})
#define bool_elem(x) ((elem_t){.b = (x)})
#define ptr_elem(x) ((elem_t){.p = (x)})
#define string_elem(x) ((elem_t){.s = (x)})

#include <stdbool.h>
#include <stdlib.h>
