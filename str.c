#include "lib3man.h"

str_t str_t_from_const(const char * s){
    size_t size = strlen(s);
    char * t = malloc(size);
    memcpy(t, s, size);
    return (str_t){t, size};
}

void str_t_append(str_t *a, str_t *b){
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
}

void str_t_free(str_t * s){
    if(s->str == NULL){
        s->len = 0;
        return;
    }
    free(s->str);
    s->str = NULL;
    s->len = 0;
}

void str_t_print(str_t s){
    write(1, (char *)s.str, s.len);
    write(1, "\n", 1);
}