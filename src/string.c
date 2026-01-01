#include "../includes/lib3man.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ucontext.h>
#include <unistd.h>

// TODO: string_v inside areanaList

// string View functions ##################################################################
// creating a string_v from char *
string_v string_v_fcharp(const char *str, size_t len){
    if(str == NULL) return (string_v){.str = NULL, .len = 0};
    return (string_v){.str = (char*) str, .len = len};
}

int string_v_cmp(const string_v *s1, const string_v *s2){
    if(s1->len != s2->len) return false;
    for(size_t i = 0; i < s1->len; i++){
        if(s1->str[i] != s2->str[i]) return false;
    }
    return true;
}

void string_v_println(const string_v *s){
    write(1, (char *)s->str, s->len);
    write(1, "\n", 1);
}

void string_v_print(const string_v *s){
    write(1, (char *)s->str, s->len);
}


// string buffer functions ##################################################################

string_b create_string_b_fchar(const char *s){
    if(s == NULL) return (string_b){.str = NULL, .len = 0, .cap = 0};

    size_t len = strlen(s);
    size_t cap = len * 4;
    char *temp = malloc(cap);

    return (string_b){.str = temp, .len = len, .cap = cap};
}

string_b string_b_fstring_v(const string_v *s){
    if(s->len == 0 || s->str == NULL) 
        return (string_b){.str = NULL, .len = 0, .cap = 0};

    char * temp = malloc(s->len * 4);
    memcpy(temp, s->str, s->len);
    
    return (string_b){.str = temp, .len = s->len, .cap = s->len*4};
}

string_b *string_b_cat(string_b *dest, string_b  *src){
    if(dest->len == 0 
    || src->str == NULL 
    || dest->str == NULL 
    || src->len == 0
    ) return NULL;
    if(dest->cap - 1 <  dest->len + src->len){
        size_t temp_len = (dest->len + src->len);
        size_t temp_cap = temp_len * 2;
        char * temp_str = realloc(dest->str, temp_cap);
        if(temp_str == NULL){
            fprintf(stderr, "Error, allocation failed");
            return NULL;
        }
        dest->str = temp_str;
        memcpy(&dest->str[dest->len],src->str , src->len);
        dest->len = temp_len;
        dest->cap = temp_cap;
        return dest;
    }
    memcpy(&dest->str[dest->len],src->str , src->len);
    dest->len += src->len;
    return dest;
}

void string_b_println(const string_b *s){
    if(s->str == NULL){
        write(1, "empty\n", 6);
        return;
    }
    write(1, s->str, s->len);
    write(1, "\n", 1);
}

void string_b_print(const string_b *s){
    if(s->str == NULL){
        write(1, "empty", 5);
        return;
    }
    write(1, s->str, s->len);
}

void string_b_free(string_b *dest){
    free(dest->str);
    dest->cap = 0;
    dest->len = 0;
}