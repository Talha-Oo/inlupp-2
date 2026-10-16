#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void print(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        putchar(str[i]);
        i++;
    }
}

void println(char *str)
{
    print(str);
    putchar('\n');
}

int read_string(char *buf, int buf_siz)
{
    int i = 0;
    int c;

    while (i < buf_siz - 1)
    {
        c = getchar();
        if (c == '\n' || c == EOF)
        {
            break;
        }
        buf[i] = (char)c;
        i++;
    }
    buf[i] = '\0';

    if (c != '\n' && c != EOF)
    {
        int discard;
        do
        {
            discard = getchar();
        } while (discard != '\n' && discard != EOF);
    }

    return i;
}

bool is_number(char *str)
{
    int len = strlen(str);
    if (len == 0)
        return false;

    int start = 0;
    if (str[0] == '-')
    {
        if (len == 1)
            return false;
        start = 1;
    }

    for (int i = start; i < len; i++)
    {
        if (!isdigit((unsigned char)str[i]))
        {
            return false;
        }
    }

    return true;
}

bool not_empty(char *str)
{
    return strlen(str) > 0;
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

answer_t ask_question(char *question, char *error_txt, check_func *check, convert_func *convert)
{
    int buf_siz = 255;
    char buf[buf_siz];

    size_t tries = 0;
    do
    {
        if (tries > 0)
        {
            printf("\n%s\n", error_txt);
        }

        printf("%s", question);
        read_string(buf, buf_siz);
        tries++;

    } while (!check(buf));

    return convert(buf);
}

int ask_question_int(char *question, char *error_txt)
{
    while (1)
    {
        answer_t answer = ask_question(question, error_txt, is_number, make_int);

        if (answer.int_value > 0)
        {
            return answer.int_value;
        }
        else 
        {
            printf("\n%s\n", error_txt);
        }
    }
}

char *ask_question_string(char *question, char *error_txt)
{
    return ask_question(question, error_txt, not_empty, make_string).string_value; // hämtar strängen ur 'answer_t' unionen
}

char *ask_question_shelf(char *question, char *error_txt)
{
    answer_t answer;
    char *input;

    while (1)
    {
        answer = ask_question(question, error_txt, not_empty, make_string);
        input = answer.string_value;

        if (isalpha(input[0]) && is_number(input + 1) && strlen(input) == 3)
        {
            break;
        }
        printf("%s\n", error_txt);
    }

    return input;
}