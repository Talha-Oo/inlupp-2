#include <stdio.h>

#include "../utilities/utils.h"


static char *get_merch_name(void)
{
  return ask_question_string("ENTER THE NAME OF THE MERCHENDISE: ");
}

static char *get_merch_desc(void)
{
  return ask_question_string("ENTER THE DESCRIPTION OF THE MERCHENDISE: ");
}

static int get_merch_price(void)
{
  return ask_question_int("ENTER THE PRICE OF THE MERCHENDISE: ");
}

void get_merch_info(char **name, char **desc, int *price) {

  printf("ENTER THE INFORMATION\n");
  printf("--------------------------\n");
  *name = get_merch_name();
  *desc = get_merch_desc();
  *price = get_merch_price();
  printf("--------------------------\n\n");

}

int main(void) {

  char *name;
  char *desc;
  int price;

  get_merch_info(&name, &desc, &price);
  printf("name: %s\n", name);
  printf("desc: %s\n", desc);
  printf("price: %d\n", price);

  return 0;
}
