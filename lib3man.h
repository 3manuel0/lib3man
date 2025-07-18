#ifndef LIB_3MAN
#define LIB_3MAN
#include <stdio.h>
#include <stdlib.h>

//dynamic Array
#define CREATE_ARR(type, name) \
    typedef struct { \
        type *data; \
        size_t size; \
        size_t capacity; \
    } name; \
    \
    static inline name name##_new(size_t capacity) { \
        int * arr = malloc(capacity * sizeof(type));\
        if (arr == NULL) return (name) {\
            .data = NULL,\
            .size = 0,\
            .capacity = 0\
        };\
        return (name){\
            .data = arr,\
            .size = 0,\
            .capacity = capacity,\
        };\
    }\
    \
    static inline void name##_push(name *arr, type n) { \
        if(arr->size < arr->capacity){\
            arr->data[arr->size] = n;\
            arr->size++;\
        }else{\
            if(arr->capacity == 0) arr->capacity = 5;\
            arr->capacity *= 2;\
            arr->data = realloc(arr->data, arr->capacity * sizeof(type));\
            arr->data[arr->size] = n;\
            arr->size++;\
        }\
    }\
    \
    static inline void name##_free(name *arr) { \
        free(arr->data);\
        arr->capacity = 0;\
        arr->size = 0;\
    }\
//end dynamic Array
#endif