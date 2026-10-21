#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/ui.h"

int main(void)
{
  store_t *store = ioopm_make_store();
  bool quit = false;

  puts("----------------------------------");
  puts("------- STORE MANAGEMENT!! -------");
  puts("----------------------------------\n");

  char action;
  while (!quit)
  {
    action = get_action_choice();

    if (action == 'A')
    {
      add_merchandise_to_store(store);
    }
    else if (action == 'L')
    {
      list_merchandise(store);
    }
    else if (action == 'D')
    {
      remove_merchandise_from_store(store);
    }
    else if (action == 'E')
    {
      edit_merchandise_in_store(store);
    }
    else if (action == 'S')
    {
      show_stock(store);
    }
    else if (action == 'P')
    {
      replenish_merchandise_in_store(store);
    }
    else if (action == 'C')
    {
      add_cart_in_store(store);
    }
    else if (action == 'R')
    {
      remove_cart_from_store(store);
    }
    else if (action == '+')
    {
      add_to_cart_in_store(store);
    }
    else if (action == '-')
    {
      remove_from_cart_in_store(store);
    }
    else if (action == '=')
    {
      calculate_cart_cost(store);
    }
    else if (action == 'O')
    {
      checkout_cart(store);
    }
    else if (action == 'Q')
    {
      quit = confirm_quit(store);
    }
    else
    {
      puts("CHOOSE A VALID OPTION!!");
    }
  }

  return 0;
}