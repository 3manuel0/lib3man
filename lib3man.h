#ifndef LIB_3MAN
#define LIB_3MAN

#include <stdlib.h>

//dynamic Array
typedef struct{
    int * data;
    size_t size;
    size_t capacity;
} dynamic_arr;

dynamic_arr newArr(int cap);
void ArrPush(dynamic_arr * darr, int n);
//end dynamic Array
#endif