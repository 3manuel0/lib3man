#ifndef LIB_3MAN
#define LIB_3MAN
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
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
  struct ArenaList *prev;
  struct ArenaList *next;
} ArenaList;

Arena create_Arena(size_t arena_size);
void *arena_Alloc(Arena *arena, size_t size); // reserves a size inside the arena and returns a pointer to the start of it
void arena_reset(Arena *arena); // resets the arena (!!!data inside the arena can be overwritten)
void arena_free(Arena *arena);

// for multiple Arenas (linked list of Arenas)
ArenaList *create_ArenaList(size_t size);
void *arenaList_Alloc(ArenaList **arenalist, size_t size);// reserves a size inside the arenaList and returns a pointer to the start of it
void *arenaList_Realloc(ArenaList **arenaList, void *p, size_t oldsz , size_t newsz); // TODO REWRITE IT, IT DOESN"T WORK
void arenaList_free(ArenaList * arenaList);
// #########################################################

// ############ Binary TREE ##############################################
// TODO: IMPLIMENT BINARY TREE's FUNCTIONS
typedef struct bt_node {
  i64 value;
  struct bt_node *right;
  struct bt_node *left;
}bt_node;

// ###########################################################################

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
string_view sv_from_cstr_sz(const char *str, size_t size);// creating a string view from char * + size

string_view sv_from_cstr(const char *str);// creating a string view from char *

string_view sv_from_sb(const string_buffer *sb);// string view from string buffer (a view to that string buffer)

size_t sb_split_svs_char(const string_buffer * sb, char delimiter, string_view * sv_arr /* can be NULL*/, size_t sv_arr_len /* can be 0*/);// splits sb into sv_arr (use own array), returns the number of sub string (svs) in that sb

int sv_cmp(const string_view *sv1, const string_view *sv2); // compare 2 string-views returns 0 if not equal and 1 if they are equal

int sv_to_int64(const string_view *sv, i64 *out);// return true (1) if succesful else false (0), out is the pointer to which it writes the parsed number

int sv_to_int32(const string_view *sv, i32 *out);// return true (1) if succesful else false (0), out is the pointer to which it writes the parsed number

int sv_to_float64(const string_view *sv, f64 *out);// return true (1) if succesful else false (0), out is the pointer to which it writes the parsed number 

void sv_println(string_view sv); // prints string-view's string with new line(\n) at the end of it

void sv_print(string_view sv); // prints the sdtring-view's string

void sv_fwrite(string_view sv, FILE *file); // wirtes sdtring-view to a file stream 
// string_buffer functions ###########################################################string_buffer create_sb_empty(size_t cap);

string_buffer create_sb_empty(size_t cap);

string_buffer sb_from_cstr(const char *str);// creating a string-buffer from char *

string_buffer create_sb_inside_arenaList(ArenaList **arenaList, size_t cap);

string_buffer sb_arenaList_from_cstr_sz(ArenaList **arenaList, const char *str, size_t size); // creating a string-buffer from char * with it's size inside an areanaList

int sb_arenaList_push_cstr_sz(ArenaList **arenaList, string_buffer *sb, const char *str, size_t size);

int sb_arenaList_push_sv(ArenaList **arenaList, string_buffer *sb, string_view sv);

int sb_cat(string_buffer *dest, string_buffer *src); // concatanate two string-buffers in the heap

string_buffer sb_from_sv(const string_view *sv); // creates a string-buffer from a string view in the heap

int sb_push_sv(string_buffer *sb, const string_view *sv); // append (push) a string-view inside a string-buffer

int sb_push_cstr(string_buffer *sb, const char *str);// append (push) a char * inside a string-buffer

int sb_push_cstr_sz(string_buffer *sb, const char *str, size_t size);// append (push) a char * (with its size) inside a string-buffer

int sb_push_char(string_buffer *sb, char ch);// append (push) a charachter inside a string-buffer

char * cstr_from_sb(const string_buffer *sb);// char * with /0 at the end from string_buffer

int sb_cmp(string_buffer sb1, string_buffer sb2); // compare 2 string_buffers returns 1 (true) they are equals, else returns 0 (false)

int sb_freadln(string_buffer *sb, FILE *stream);// reads a line from a file stream

int sb_fread_all(string_buffer *sb, FILE *stream);// reads everything from a file stream (stops at EOF)

void sb_println(string_buffer sb); // prints a string-buffer's string (current used bytes (chars)) with new line(\n) at the end

void sb_print(string_buffer sb); // prints a string-buffer's string

int sb_fprint(string_buffer sb, FILE *stream);// prints sb to file stream, it returns len else -1

void sb_fwrite(string_buffer sb, FILE *stream);// wirtes sb to a file stream

void sb_free(string_buffer *sb); // frees string-buffer (for the heap one not the arena one)
// ###########################################################################################

//############ Utility #######################################################################
#ifdef _WIN32
  #include <windows.h>
  #include <bcrypt.h>
  #pragma comment(lib, "bcrypt.lib")

#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
  #include <stdlib.h> // arc4random_buf

#else
  #include <sys/random.h>
#endif

typedef struct{
  u8 * buf;
  size_t size;
  size_t offset;
} Buffer;

u32 u32_entropy_random(void);//os based unsigned 32bit integer pseudo-random entropy generator

f64 f64_random_range(f64 min, f64 max);// return float 64bit (double) based on a range [min, max]

f32 f32_random_range(f32 min, f32 max);// return float 32bit (float) based on a range [min, max]

u32 u32_random_range(u32 min, u32 max);// return unsigned integer 32bit based on a range [min, max]

u16 u16_bswap(u16 x);//swaping bits (Endianess) 16bit

u32 u32_bswap(u32 x);//swaping bits (Endianess) 32bit

u64 u64_bswap(u64 x);//swaping bits (Endianess) 64bit

Buffer buffer_read_file(const char * file);// reads an entier file into a Buffer

void * buffer_get_next_sz(Buffer * buffer, size_t size);// it uses a cursor like into that buffer and moves it by the size, it returns the ptr before that.

void buffer_reset(Buffer * buffer);// reset the cursor of the Buffer or the offset.

ssize_t buffer_write_file(Buffer buffer, const char * file_name);// write the entier Buffer into a file

void buffer_free(Buffer *buffer);// frees the Buffer's memory and rests it.
// ###########################################################################################

// ########### C3SV ##########################################################################
#ifdef _WIN32
    #include <BaseTsd.h>
    typedef SSIZE_T ssize_t;
#else
    #include <unistd.h>
#endif

typedef enum { string_ = 1, float64_, int64_, boolean_ } csv_type;

typedef struct {
  string_view *head;
  csv_type *types;
  void **data; // maybe void * is better for infering the types later
  size_t numrows;
  size_t numcols;
  ArenaList *gl_arena;
} CSV;

CSV *create_csv(); // creates an empty csv struct in memory

CSV *load_csv(char *file_name);// load with types (only int and float 64bit)

void csv_free(CSV *csv); // free csv in memory

void csv_print_head(const CSV *csv);// prints head

void csv_print_row(const void *row, csv_type * row_types, size_t numcolumns);

void csv_print_types(const CSV *csv);

void csv_print_column_from_string(const CSV *csv, string_view column_name);

void csv_write_file(const char *filename, const CSV *csv); // write a csv file 

ssize_t csv_get_column_index(const CSV *csv, string_view name); // returns -1 if it doesn't find the column name

i32 csv_write_json(const CSV *csv, const char *filename);// writes a json files from a csv in memory

i64 csv_get_int_by_name(const CSV *csv, size_t row, string_view col_name);

f64 csv_get_float_by_name(const CSV *csv,size_t row, string_view col_name);

string_view csv_get_sv_by_name(const CSV *csv, size_t row, string_view col_name);

int64_t csv_column_sum_int(const CSV* csv, size_t col_index);

f64 csv_column_sum_float(const CSV* csv, size_t col_index);

f64 csv_column_mean(const CSV* csv, size_t col_index);

f64 csv_column_min(const CSV* csv, size_t col_index);

f64 csv_column_max(const CSV* csv, size_t col_index);

size_t csv_row_count(const CSV *csv);

size_t csv_column_count(const CSV *csv);

string_view csv_column_name(const CSV* csv, size_t column);

void print_type(csv_type t);
// ##########################################################################################

//############ Matrix #######################################################################
typedef struct Matrix{
    f64    *mtx;
    size_t rows;
    size_t cols;
}Matrix;

// TODO : I NEED TO FINISH THESE MATRIX FUNCTIONS
Matrix matrix_create_empty(size_t rows, size_t cols);// creates a matrix full of zeros

Matrix matrix_create(size_t rows, size_t cols, f64 arr[rows][cols]);// creates a matrix from a 2d array

int matrix_fill(Matrix *matrix, f64 value);// fills the matrix with the value given

void matrix_randomize(Matrix* matrix, f64 min, f64 max);// fills the matrix with random numbers between min and max both includes

void matrix_add(Matrix *a, Matrix b);// addes matrix b to a

void matrix_sub(Matrix *a, Matrix b);// subtructs matrix b from a 

void matrix_mul(Matrix *a, Matrix b);// multiplaying a by b and storing the result in a

Matrix matrix_dot(Matrix a, Matrix b);// returns a matrix which is the result of matrix a multiplied by matrix b

void matrix_scale(Matrix *matrix, f64 k);// multiplies each element in the matrix with the value k

Matrix matrix_copy(Matrix src);// creates a copy of a matrix (must be freed)

void matrix_map(Matrix *matrix, f64(*func)(f64));// does the function func for every elemnt of the matrix

Matrix matrix_transpose(Matrix matrix);// matrix transpose (fliping rows with columns) M[R x C] -> M[C x R]

void matrix_print(Matrix matrix);// prints the matrix

void matrix_free(Matrix *matrix);// free memory

// ######################################################################################################################
#endif