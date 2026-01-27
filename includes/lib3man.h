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
typedef int64_t i64;
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

typedef string_view sv;
typedef string_buffer sb;

enum { str_fail = -1, str_succ, str_err };

#define sv_from_lit(str) (sv){str, sizeof(str) - 1}

// string-view functions ###############################################
sv sv_from_cstr_sz(const char *str, size_t size);// creating a string view from char * + size

sv sv_from_cstr(const char *str);// creating a string view from char *

sv sv_from_sb(const sb *sb);// string view from string buffer (a view to that string buffer)

size_t sb_split_svs_char(const sb * sb, char delimiter, sv * sv_arr /* can be NULL*/, size_t sv_arr_len /* can be 0*/);// splits sb into sv_arr (use own array), returns the number of sub string (svs) in that sb

int sv_cmp(const sv *sv1, const sv *sv2); // compare 2 string-views

int sv_to_int64(const sv *sv, i64 *out);// return true if succesful, out is the pointer to which it writes the number

void sv_println(const sv *sv); // prints sdtring-view with new line(\n)

void sv_print(const sv *sv); // prints sdtring-view  without new line

// string_buffer functions ###########################################################
sb sb_from_cstr(const char *str);// creating a string-buffer from char *

sb create_sb_inside_arenaList(ArenaList *arenaList, size_t cap);

sb sb_arenaList_from_cstr_sz(ArenaList *arenaList, const char *str, size_t size); // creating a string-buffer from char * with it's size inside an areanaList

int sb_arenaList_push_cstr_sz(ArenaList *arenaList, sb *sb, const char *str, size_t size);

int sb_arenaList_push_sv(ArenaList *arenaList, sb *sb, sv sv);

sb *sb_cat(sb *dest, sb *src); // concatanate two string-buffers in the heap

sb sb_from_sv(const sv *sv); // creates a string-buffer from a string view in the heap

int sb_push_sv(sb *sb, const sv *sv); // append (push) a string-view inside a string-buffer

int sb_push_cstr(sb *sb, const char *str);// append (push) a char * inside a string-buffer

int sb_push_cstr_sz(sb *sb, const char *str, size_t size);// append (push) a char * (with its size) inside a string-buffer

int sb_push_char(sb *sb, char ch);// append (push) a charachter inside a string-buffer

char * cstr_from_sb(const sb *sb);// char * with /0 at the end from string_buffer

void sb_println(const sb *sb); // prints a string-buffer (current used bytes (chars)) with new line(\n)

void sb_print(const sb *sb); // prints a string-buffer without new line

void sb_free(sb *sb); // frees string-buffer in the heap
// ###########################################################################################

#endif