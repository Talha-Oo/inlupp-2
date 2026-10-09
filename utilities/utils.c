extern char *strdup(const char *);

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union
{
  int int_value;
  float float_value;
  char *string_value;
} answer_t;

typedef bool check_func(char *);
typedef answer_t convert_func(char *);

int read_string(char *buf, int buf_siz)
{

  int counter = 0;
  int c;

  while ((c = getchar()) != '\n' && counter < buf_siz - 1)
  {
    buf[counter] = c;
    counter++;
  }

  buf[counter] = '\0';

  if (c != '\n')
  {
    while (getchar() != '\n')
      ;
  }

  return counter;
}

bool is_number(char *str)
{

  int len = strlen(str);

  if (len <= 0)
    return false;

  for (int i = 0; i < len; i++)
  {
    if (i == 0 && str[0] == '-' && len > 1)
    {
      continue;
    }
    else
    {
      if (!isdigit(str[i]))
      {
        return false;
      }
    }
  }

  return true;
}

bool is_float(char *str)
{

  int len = strlen(str);

  int dot_count = 0;
  for (int i = 0; i < len; i++)
  {
    if (i == 0 && (str[i] == '-' || str[i] == '.') && len > 1)
    {
      if (str[i] == '.')
      {
        dot_count++;
      }
      continue;
    }
    else if (str[i] == '.' && len > 1)
    {
      dot_count++;
    }
    else
    {
      if (!isdigit(str[i]))
      {
        return false;
      }
    }

    if (dot_count > 1)
    {
      return false;
    }
  }

  return true;
}

answer_t make_float(char *str)
{

  answer_t f;
  f.float_value = atof(str);
  return f;
}

answer_t make_string(char *str)
{

  answer_t s;
  s.string_value = strdup(str);
  return s;
}

answer_t make_int(char *str)
{

  answer_t i;
  i.int_value = atoi(str);
  return i;
}

bool not_empty(char *str)
{
  return strlen(str) > 0;
}

answer_t ask_question(char *question, check_func *check, convert_func *convert)
{

  char buffer[20];
  int buff_size = sizeof(buffer) / sizeof(buffer[0]);

  while (1)
  {
    printf("%s", question);
    read_string(buffer, buff_size);

    if (check(buffer))
    {
      break;
    }

    printf("Invalid Input!!\n");
  }

  return convert(buffer);
}

char *ask_question_string(char *question)
{
  answer_t answer = ask_question(question, not_empty, make_string);
  return answer.string_value;
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, make_int);
  return answer.int_value; // svaret som ett heltal
}

double ask_question_float(char *question)
{
  answer_t answer = ask_question(question, is_float, make_float);
  return answer.float_value; // svaret som ett decimaltal
}

int string_length(char str[])
{

  int count = 0;

  int i = 0;
  while (str[i] != '\0')
  {
    count++;
    i++;
  }

  return count;
}

void print(char str[])
{

  int i = 0;
  while (str[i] != '\0')
  {
    putchar(str[i]);
    i++;
  }
}

void println(char str[])
{

  print(str);
  printf("\n");
}