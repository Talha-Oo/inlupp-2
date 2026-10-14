#pragma once

#include "../utilities/utils.h"

/// @brief Prompts the user to enter merchandise information
/// @param name pointer to the merchandise name pointer
/// @param desc pointer to the merchandise description pointer
/// @param price pointer to the merchandise price
void get_merch_info(char **name, char **desc, int *price);