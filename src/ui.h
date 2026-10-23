#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "../utilities/common.h"

/// @brief Makes a store 
/// @return returns a pointer to a store
store_t *ioopm_make_store(void);

/// @brief asks the user for relevant merchandise information and adds a merchandise with that information
/// in the store, prints error message if the merchandise with the name already exists
/// @param store pointer to the store to add the merchandise to
void add_merchandise_to_store(store_t *store);

/// @brief asks the user for the name of the merchandise to be removed, waits for confirmation before 
/// removing the merchandise from the store
/// @param store pointer to the store from the which the merchandise is supposed to be removed from
void remove_merchandise_from_store(store_t *store);

/// @brief asks the user for the merchandise name, prints a error message if the merch doesn't exist
/// otherwise asks user for name, description and price and updates the information of the merchandise 
/// with the provided name at the start
/// @param store pointer to the store which contains the merchandise
void edit_merchandise_in_store(store_t *store);

/// @brief ask user for a shelf name and a merchandise name and adds stock in that shelf,
/// prints error message if the merchandise doesn't exists in the store, or if the shelf 
/// already store a different merchandise
/// @param store pointer to the store which contains ther merchandise
void replenish_merchandise_in_store(store_t *store);

/// @brief prints all of the merchandises in the store
/// @param store pointer to the store which contains all of the merchandises
void list_merchandise(store_t *store);

/// @brief asks the user for a merchandises name and prints the shelves which contain it and the ammount 
/// they contain, prints an error message if the merchandise doesn't exist
/// @param storepointer to the store which contains the merchandise
void show_stock(store_t *store);

/// @brief adds a cart in the store
/// @param store pointer to the store which the cart should be added to
void add_cart_in_store(store_t *store);

/// @brief asks the user for cart id and removes the cart with that id, prints an error message if
/// a cart with that id doesn't exist
/// @param store pointer to the store which the cart should be removed from
void remove_cart_from_store(store_t *store);

/// @brief asks the user for cart id, merchandise name and quantity and adds that many merchandises to the 
/// cart which has that id. Prints error messages, if a cart with that cart id doesn't exits, if a
/// merchandise with that name doesn't exist and if sufficent stock doesn't exist for the mechandise
/// a cart with that id doesn't exist. On succesfull add, reserveres that many merchandises
/// @param store pointer to the store which conatins the cart and the merchandise
void add_to_cart_in_store(store_t *store);

/// @brief asks the user for cart id, merchandise name and quantity and removes that many merchandises to 
/// the cart which has that id. Prints error messages, if a cart with that cart id doesn't exits, if a
/// merchandise with that name doesn't exist and if sufficent stock doesn't exist for the mechandise
/// a cart with that id doesn't exist
/// @param store pointer to the store which conatins the cart and the merchandise
void remove_from_cart_in_store(store_t *store);

/// @brief asks the user for cart id and removes the cart and updates all of the reserves for all of the 
/// merchandises in the cart if a cart with that id exists, prints an error message if such cart does not 
/// exist
/// @param store pointer to the store which conatins the cart to be removed
void remove_cart_in_store(store_t *store);

/// @brief asks the user for cart id and prints the total cart of that cart, if the cart doesn't exist,
/// prints an error message
/// @param store pointer to the store which conatins the cart
void calculate_cart_cost(store_t *store);

/// @brief asks the user for cart id and prints the total cart of that cart. It also updates the 
/// reserved quantiy for all of the merchandises in the cart and also their stocks in the store
/// @param store pointer to the store which conatins the cart
void checkout_cart(store_t *store);

/// @brief asks user for confirmation, if the input starts with y/Y, deletes the store, and frees all of 
/// the memory
/// @param store pointer to the store trying to remove
bool confirm_quit(store_t *store);

/// @brief prints the menu and ask the user for input
/// @return captilized first character of the input
char get_action_choice(void);

