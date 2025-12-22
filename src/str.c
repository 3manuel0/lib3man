#include "../includes/lib3man.h"

string string_from_const(const char * s){
    size_t size = strlen(s);
    char * t = malloc(size);
    memcpy(t, s, size);
    return (string){t, size};
}

string string_append(string *a, string *b){
    size_t newlen = a->len + b->len;
    char * temp = malloc(newlen);
    size_t offst = 0;
    for(; offst < a->len; offst++){
        temp[offst] = a->str[offst];
    }
    
    for(size_t i = 0; i < b->len; i++, offst++){
        temp[offst] = b->str[i];
    }

    free(b->str);
    b->len = 0;
    b = NULL;

    free(a->str);
    a->str = temp;
    a->len = newlen;
    return *a;
}

void string_free(string * s){
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