#ifndef LIB_3MAN
#define LIB_3MAN
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
void *arena_Alloc(Arena *arena, size_t size); // reserves a size inside the arena and returns a pointer to the start of it
void arena_reset(Arena *arena); // resets the arena (!!!data inside the arena can be overwritten)
void arena_free(Arena *arena);

// for multiple Arenas (linked list of Arenas)
ArenaList *create_ArenaList(size_t size);
void *arenaList_Alloc(ArenaList *arenalist, size_t size);// reserves a size inside the arenaList and returns a pointer to the start of it
void *arenaList_Realloc(ArenaList * arenaList, void *p, size_t oldsz , size_t newsz); 
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

// string-view functions ###############################################
strview strview_fcstr(const char *str, size_t len);// creating a string view from char *

int strview_cmp(const strview *s1, const strview *s2); // compare 2 string-views

void strview_println(const strview *s); // prints sdtring-view with new line(\n)

void strview_print(const strview *s); // prints sdtring-view  without new line

// string_buffer functions ###############################################
strbuf strbuf_fcstr(const char *s);// creating a string-buffer from char *

strbuf strbuf_arenaList_cstrsz(ArenaList *arenaList, const char *s, size_t len); // creating a string-buffer from char * with it's size inside an areanaList

strbuf *strbuf_cat(strbuf *dest, strbuf *src); // concatanate two string-buffers in the heap

strbuf strbuf_fstrview(const strview *str); // creates a string-buffer from a string view in the heap

int strbuf_push_strview(strbuf *sb, const strview *sv); // append (push) a string-view inside a string-buffer

int strbuf_push_cstr(strbuf *sb, const char *str);// append (push) a char * inside a string-buffer

int strbuf_push_cstrsz(strbuf *sb, const char *str, size_t size);// append (push) a char * (with its size) inside a string-buffer

int strbuf_push_char(strbuf *sb, char ch);// append (push) a charachter inside a string-buffer

void strbuf_println(const strbuf *sb); // prints a string-buffer (current used bytes (chars)) with new line(\n)

void strbuf_print(const strbuf *sb); // prints a string-buffer without new line

void strbuf_free(strbuf *sb); // frees string-buffer in the heap
// ##############################################################################

#endif