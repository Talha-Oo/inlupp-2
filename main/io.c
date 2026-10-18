#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utilities/utils.h"

char *get_name_io(void)
{
  return ask_question_string("MERCHANDISE NAME: ",
                             "INVALID NAME!!");
}

char *get_desc_io(void)
{
  return ask_question_string("DESCRIPTION: ",
                             "INVALID DESCRIPTION!!");
}

int get_price_io(void)
{
  return ask_question_int("PRICE: ",
                          "PLEASE ENTER A POSITIVE NUMBER!!");
}

char *get_shelf_io(void)
{
  return ask_question_shelf("SHELF: ",
                            "ENTER A VALID SHELF (eg. B21)!!");
}

size_t get_quantity_io(void)
{
  return (size_t) ask_question_int("QUANTITY: ",
                          "PLEASE ENTER A POSITIVE NUMBER!!");
}

int get_cart_id_io(void)
{
  return ask_question_int("CART ID: ",
                          "PLEASE ENTER A POSITIVE NUMBER!!");
}

bool is_confirmed_io(void)
{
  print("ARE YOU SURE YOU WANT TO PROCEED?\n");
  char *str = ask_question_string("(Y/N): ", "PLEASE ENTER SOMETHING!!");

  if(toupper(*str) == 'Y')
  {
    free(str);
    return true;
  }

  free(str);
  return false;
}

bool should_continue_listing_io(void)
{
  print("DO YOU WANT TO STOP LISTING?\n");
  char *str = ask_question_string("(Y/N): ", "PLEASE ENTER SOMETHING!!");

  if(toupper(*str) == 'N')
  {
    return true;
  }
  
  return false;
}

