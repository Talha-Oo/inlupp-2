#pragma once
// TODO: DOCUMENTATION!!

/// @brief reads the name from the user from the terminal
/// @return the input as char *
char *get_name_io(void);

/// @brief reads the description from the user from the terminal
/// @return the input as char *
char *get_desc_io(void);

/// @brief reads the price from the user from the terminal
/// @return the input as an int
int get_price_io(void);

/// @brief reads the shelf id from the user from the terminal
/// @return the input as char *
char *get_shelf_io(void);

/// @brief reads the quantity from the user from the terminal
/// @return the input as an int
int get_quantity_io(void);

/// @brief reads the cart id from the user from the terminal
/// @return the input as an int
int get_cart_id_io(void);

/// @brief reads the input from the user from the terminal
/// @return if the input starts with y/Y returns true and false otherwise
bool is_confirmed_io(void);

/// @brief reads the input from the user from the terminal
/// @return if the input starts with n/N returns true and false otherwise
bool should_continue_listing_io(void);