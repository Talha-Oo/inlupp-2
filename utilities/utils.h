#pragma once

extern char *strdup(const char *);

#include <stdbool.h>

typedef union
{
    int int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef bool check_func(char *);
typedef answer_t convert_func(char *);

int read_string(char *buf, int buf_siz);

bool is_number(char *str);
bool not_empty(char *str);
bool is_float(char *str);

answer_t make_float(char *str);
answer_t make_string(char *str);
answer_t make_int(char *str);

answer_t ask_question(char *question, check_func *check, convert_func *convert);
char *ask_question_string(char *question);
int ask_question_int(char *question);
double ask_question_float(char *question);
char *ask_question_shelf(char *question);

int string_length(char str[]);
void print(char str[]);
void println(char str[]);