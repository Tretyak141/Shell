#ifndef _PARSER_H
#define _PARSER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define STRING_LENGTH 100

typedef struct Posl{
    char *args;
    bool need_recur;
} posl_elem;

typedef struct iff{
    char *args;
    bool need_recur;
    int success;
} if_elem;

typedef struct conv{
    char *args;
    bool need_recur;
} conv_elem;

typedef struct Posl_arr{
    posl_elem *arr;
    int len;
} Posl_array;

typedef struct If_arr{
    if_elem *arr;
    int len;
} If_array;

typedef struct Conv_arr{
    conv_elem *arr;
    int len;
} Conv_array;

posl_elem string_to_posl_elem(char *s);//Конвертация строки в элемент для последовательного выполнения
if_elem string_to_if_elem(char *s,char *operato00r);//Конвертация строки в элемент для условного выполнения
conv_elem string_to_conv_elem(char *s);//Конвертация строки в элемент для конвейерного выполнения
Posl_array string_parser(char *s);//Перевод строки в элементы последовательного выполнения
If_array iff(char *s);//Перевод строки в элементы условного выполнения
Conv_array conv_parser(char *s);//Перевод строки в элементы конвейерного выполнения
#endif