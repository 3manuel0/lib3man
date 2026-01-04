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

typedef string_view strview;
typedef string_buffer strbuf;

enum { str_fail = -1, str_succ, str_err };

#define strview_from_lit(str) (strview){str, sizeof(str) - 1}

// string-view functions
strview strview_fcharp(const char *str, size_t len);

int strview_cmp(const strview *s1, const strview *s2); // compare 2 strviews

void strview_println(const strview *s); // prints with new line(\n)

void strview_print(const strview *s); // prints without new line

// string_buffer functions :
strbuf strbuf_fcstr(const char *s);// string buffer from C_string (char *, Null terminated)

strbuf *strbuf_cat(strbuf *dest, strbuf *src);

strbuf strbuf_fstrview(const strview *str);

int strbuf_push_strview(strbuf *sb, const strview *sv);

int strbuf_push_str(strbuf *sb, const char *str);

int strbuf_push_szstr(strbuf *sb, const char *str, size_t size);

int strbuf_push_char(strbuf *sb, char ch);

void strbuf_println(const strbuf *s); // prints with new line(\n)

void strbuf_print(const strbuf *s); // prints without new line

void strbuf_free(strbuf *dest);
// ##############################################################################

#endif