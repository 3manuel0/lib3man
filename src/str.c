#include "../includes/lib3man.h"

// creating a string from buffer 
string string_from_buffer(const char *s){
    size_t size = strlen(s);
    char * str = malloc(size);

    memcpy(str, s, size);

    return (string){str, size};
}

int string_append(string *a, string *b){

    if(a->len == 0 
    || a->str == NULL 
    || b->str == NULL 
    || b->len == 0) return str_err;

    size_t newlen = a->len + b->len;
    char * temp = malloc(newlen);
    size_t offst = 0;

    if(temp == NULL) return str_fail;
    
    memcpy(temp, a->str, a->len);
    memcpy(temp + a->len, b, b->len);

    free(b->str);
    b->len = 0;
    b = NULL;

    free(a->str);
    a->str = temp;
    a->len = newlen;
    return str_succ;
}

string string_append_arena(Arena *arena, string *a, string *b){
    char * str = arena_Alloc(arena, a->len + b->len);

    if(str == NULL) return (string){ .str = NULL, .len = 0};

    memcpy(str, a->str, a->len);
    memcpy(str + a->len, b, b->len);

    return (string){.str = str, .len = a->len + b->len};
}

int string_from_mem_to_arena(Arena *arena, string *s){

    if(s->len == 0 || s->str == NULL) return str_err;

    char * str = arena_Alloc(arena, s->len);

    if(str == NULL) return str_fail;

    free(s->str);
    s->str = str;

    return str_succ;
}

void string_free(string *s){
    if(s->str == NULL){
        s->len = 0;
        return;
    }
    free(s->str);
    s->str = NULL;
    s->len = 0;
}

void string_println(string s){
    write(1, (char *)s.str, s.len);
    write(1, "\n", 1);
}

void string_print(string s){
    write(1, (char *)s.str, s.len);
}