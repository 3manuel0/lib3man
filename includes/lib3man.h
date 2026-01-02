#ifndef LIB_3MAN
#define LIB_3MAN
#include <stddef.h>
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

#define false 0
#define true 1

// ############ Arena allocator ##############################################
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

// for multiple ArenaList (linked list of Arenas)
typedef struct ArenaList {
  Arena arena;
  struct ArenaList *next;
} ArenaList;

Arena create_Arena(size_t arena_size);
void *arena_Alloc(Arena *arena, size_t size);
void arena_reset(Arena *arena);
void arena_free(Arena *arena);

// for multiple Arenas (linked list of Arenas)
ArenaList *create_ArenaList(size_t size);
void *arenaList_Alloc(ArenaList *arenalist, size_t size);
void arenaList_free(ArenaList *head);
// #############################################################################

// ############ Dynamic Array #################################################
// I kinda hate this macros TODO: find another way maybe void *                #
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
  } // #
// #############################################################################

// ############ Length-Based string and dynamicly allocated string #############
typedef struct {
  char *str;
  size_t len;
} string_view;

typedef struct {
  char *str;
  size_t len;
  size_t cap;
} string_buffer;

typedef string_view string_v;
typedef string_buffer string_b;

enum { str_fail = -1, str_succ, str_err };

#define string_v_from_lit(str) (string_v){str, sizeof(str) - 1}

// string-view functions
string_v string_v_fcharp(const char *str, size_t len);

int string_v_cmp(const string_v *s1, const string_v *s2); // compare 2 string_vs

void string_v_println(const string_v *s); // prints with new line(\n)

void string_v_print(const string_v *s); // prints without new line

// string_buffer functions :
string_b create_string_b_fchar(const char *s);

string_b *string_b_cat(string_b *dest, string_b *src);

string_b string_b_fstring_v(const string_v *str);

int string_b_push_string_v(string_b *sb, const string_v *sv);

int string_b_push_str(string_b *sb, const char *str);

int string_b_push_sstr(string_b *sb, const char *str, size_t size);

void string_b_println(const string_b *s); // prints with new line(\n)

void string_b_print(const string_b *s); // prints without new line

void string_b_free(string_b *dest);
// ##############################################################################

#endif