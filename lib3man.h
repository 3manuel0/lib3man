#ifndef LIB_3MAN
#define LIB_3MAN

#include <stddef.h>
#include <stdlib.h>

//dynamic Array
typedef struct{
    int * data;
    size_t size;
    size_t capacity;
} dynamic_arr;

dynamic_arr newArr(size_t capacity);
void ArrPush(dynamic_arr * darr, int n);

void freeArr(dynamic_arr * darr);
//end dynamic Array
#endif