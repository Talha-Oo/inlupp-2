#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"


/// @brief Creates a merchendise 
/// @param name the name of the merchendise
/// @param value the value to be appended
merch_t *create_merch(char* name, char* desc, size_t price);

