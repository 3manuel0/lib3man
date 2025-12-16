#ifndef LIB_3MAN
#define LIB_3MAN
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Arena allocator
typedef struct {
  void *memory;
  void *address;
  size_t capacity;
  size_t cur_size;
} Arena;

void create_Arena(Arena *arena);
void arena_Alloc(Arena *arena, void *data, size_t size);
void arena_reset(Arena *arena);
void arena_free(Arena *arena);
// end of arena allocator

// dynamic Array
#define CREATE_ARR(type, name)                                                 \
  typedef struct {                                                             \
    type *data;                                                                \
    size_t size;                                                               \
    size_t capacity;                                                           \
  } name;                                                                      \
                                                                               \
  static inline name name##_new(size_t capacity) {                             \
    int *arr = malloc(capacity * sizeof(type));                                \
    if (arr == NULL)                                                           \
      return (name){.data = NULL, .size = 0, .capacity = 0};                   \
    return (name){                                                             \
        .data = arr,                                                           \
        .size = 0,                                                             \
        .capacity = capacity,                                                  \
    };                                                                         \
  }                                                                            \
                                                                               \
  static inline void name##_push(name *arr, type n) {                          \
    if (arr->size < arr->capacity) {                                           \
      arr->data[arr->size] = n;                                                \
      arr->size++;                                                             \
    } else {                                                                   \
      if (arr->capacity == 0)                                                  \
        arr->capacity = 5;                                                     \
      arr->capacity *= 2;                                                      \
      arr->data = realloc(arr->data, arr->capacity * sizeof(type));            \
      arr->data[arr->size] = n;                                                \
      arr->size++;                                                             \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline void name##_free(name *arr) {                                  \
    free(arr->data);                                                           \
    arr->capacity = 0;                                                         \
    arr->size = 0;                                                             \
  }                                                                            \
  // end of dynamic Array

// linked list

typedef struct list {
  int data;
  struct list *next;
} list;

void list_push(list *node, int value);

void print_list(list *head);

void free_list(list *head);

// end of linked list

// first attempt at a length base string

typedef struct {
  char *str;
  size_t len;
} str_t;

str_t str_t_from_const(const char *s);

str_t str_t_append(str_t *a, str_t *b);

void str_t_free(str_t *s);

void str_t_print(str_t s);

// end of length base string

#endif