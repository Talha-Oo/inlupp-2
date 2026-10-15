#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void print(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        putchar(str[i]);
        i++;
    }
}

void println(char *str) {
    print(str);
    putchar('\n');
}

int read_string(char *buf, int buf_siz) {
    int i = 0;
    int c;

    while (i < buf_siz - 1) {
        c = getchar();
        if (c == '\n' || c == EOF) {
            break;
        }
        buf[i] = (char)c;
        i++;
    }
    buf[i] = '\0';

    if (c != '\n' && c != EOF) {
        int discard;
        do {
            discard = getchar();
        } while (discard != '\n' && discard != EOF);
    }

    return i;
}

bool is_number(char *str) {
    int len = strlen(str);
    if (len == 0) return false;

    int start = 0;
    if (str[0] == '-') {
        if (len == 1) return false;
        start = 1;
    }

    for (int i = start; i < len; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return false;
        }
    }

    return true;
}

bool not_empty(char *str) {
    return strlen(str) > 0;
}

answer_t ask_question(char *question, check_func *check, convert_func *convert) {
    int buf_siz = 255;
    char buf[buf_siz];
    do {
        println(question);
        read_string(buf, buf_siz);
    } while (!check(buf));

    return convert(buf);
}

int ask_question_int(char *question) {
    answer_t answer = ask_question(question, is_number, (convert_func *) atoi);
    return answer.int_value;
}

char *ask_question_string(char *question) {
    return ask_question(question, not_empty, (convert_func *) strdup).string_value; //hämtar strängen ur 'answer_t' unionen
}