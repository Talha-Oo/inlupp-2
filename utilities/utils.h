#pragma once

#include <stdbool.h>

typedef union { 
  int   int_value;
  float float_value;
  char *string_value;
} answer_t;

typedef bool check_func(char *);
typedef answer_t convert_func(char *);

extern char *strdup(const char *);

int read_string(char *buf, int buf_siz);
bool is_number(char *str);
bool not_empty(char *str);

void print(char *str);
void println(char *str);

answer_t ask_question(char *question, char *error_txt, check_func *check, convert_func *convert);
int ask_question_int(char *question, char *error_txt);
char *ask_question_string(char *question, char *error_txt);
char *ask_question_shelf(char *question, char *error_txt);