#ifndef LIB_3MAN
#define LIB_3MAN
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef float f32;
typedef double f64;

// Arena allocator #########################################################

#define KiB(x) ((uint64_t)(x) << 10)
#define MiB(x) ((uint64_t)(x) << 20)
#define GiB(x) ((uint64_t)(x) << 30)

// single arena
typedef struct {
  void *memory;
  void *address;
  size_t capacity;
  size_t cur_size;
} Arena;

// for multiple ArenaList (linked list of ArenaList)
typedef struct ArenaList {
  Arena arena;
  struct ArenaList *next;
} ArenaList;

Arena create_Arena(size_t arena_size);
void *arena_Alloc(Arena *arena, size_t size);
void arena_reset(Arena *arena);
void arena_free(Arena *arena);

// for multiple Arenas (linked list of ArenaList)
ArenaList *create_ArenaList(size_t size);
void *arenaList_Alloc(ArenaList *arenalist, size_t size);
void arenaList_free(ArenaList *head);

// end of arena allocator #####################################################

// dynamic Array ##############################################################

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
  }

// end of dynamic Array ####################################################

// linked list#################################################################

/* just testing should make it dynamicly typed maybe I will
use (void *) or macros like the dynamic array */
typedef struct list {
  int data;
  struct list *next;
} list;

void list_push(list *node, int value);

void print_list(list *head);

void free_list(list *head);

// end of linked list ##################################################

// length-base string ##################################################

typedef struct {
  char *str;
  size_t len;
} string;

enum { str_fail = -1, str_succ, str_err };

#define string_from_lit(str) (string){str, sizeof(str) - 1}

string string_from_buffer(const char *s);

int string_append(string *a, string *b);

string string_append_arena(Arena *arena, string *a, string *b);

string arena_string_from_mem(Arena *arena, char *str);

void string_println(string s); // prints the string with \n new line at the end

void string_print(string s); // prints the string without new line

void string_free(string *s);

// end of length base string ###############################################

#endif