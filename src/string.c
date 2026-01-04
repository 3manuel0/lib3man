#include "../includes/lib3man.h"

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
    if(s->str == NULL){
        fwrite("empty\n", 1, 6, stdout);
        return;
    }
    fwrite(s->str, 1, s->len,stdout);
    fwrite("\n", 1, 1,stdout);
    // write(1, (char *)s->str, s->len); // write is too slow no buffering
    // write(1, "\n", 1);
}

void string_v_print(const string_v *s){
    if(s->str == NULL){
        fwrite("empty", 1, 5, stdout);
        return;
    }
    fwrite(s->str, 1, s->len,stdout);
    // write(1, (char *)s->str, s->len); // write is too slow no buffering
}


// string buffer functions ##################################################################

// TODO: string_b inside areana and arenaList

// creating a string_b from char *
string_b string_b_fchar(const char *s){
    if(s == NULL) return (string_b){.str = NULL, .len = 0, .cap = 0};

    size_t len = strlen(s);
    size_t cap = len * 4;
    char *temp = malloc(cap);

    if(temp == NULL){
        fprintf(stderr, "Error, Allocation Failed");
        return (string_b){.str = NULL, .len = 0, .cap = 0};
    }

    return (string_b){.str = temp, .len = len, .cap = cap};
}

string_b string_b_fstring_v(const string_v *s){
    if(s->len == 0 || s->str == NULL) 
        return (string_b){.str = NULL, .len = 0, .cap = 0};

    char * temp = malloc(s->len * 4);

    if(temp == NULL){
        fprintf(stderr, "Error, Allocation Failed");
        return (string_b){.str = NULL, .len = 0, .cap = 0};
    }

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

int string_b_push_string_v(string_b *sb, const string_v *sv){
    if(sb == NULL || sv == NULL){
        fprintf(stderr, "Erorr, NULL Pointer\n");
        return str_err;
    }

    if(sb->str == NULL || sv->str == NULL || sb->cap == 0){
        fprintf(stderr, "Erorr, Invalid strings\n");
        return str_err;
    }

    if(sb->cap > sb->len + sv->len){
        memcpy(sb->str+sb->len, sv->str, sv->len);
        sb->len += sv->len;
    }else{
        size_t temp_cap = sb->cap * 2;
        char * s  = realloc(sb->str, sb->cap);
        if(s == NULL){
            fprintf(stderr, "Erorr, Realocation Failed\n");
            return str_fail;
        }
        sb->cap = temp_cap;
        sb->str = s;
        memcpy(sb->str+sb->len, sv->str, sv->len);
        sb->len += sv->len;
    }

    return str_succ;
}

int string_b_push_str(string_b *sb, const char *str){
    if(sb == NULL || str == NULL){
        fprintf(stderr, "Erorr, NULL Pointer\n");
        return str_err;
    }

    if(sb->str == NULL || sb->cap == 0){
        fprintf(stderr, "Erorr, Invalid String Buffer\n");
        return str_err;
    }

    size_t str_len = strlen(str);
    if(sb->cap > sb->len + str_len){
        memcpy(sb->str+sb->len, str, str_len);
        sb->len += str_len;
    }else{
        size_t temp_cap = sb->cap * 2;
        char * s  = realloc(sb->str, sb->cap);
        if(s == NULL){
            fprintf(stderr, "Erorr, Realocation Failed\n");
            return str_fail;
        }
        sb->cap = temp_cap;
        sb->str = s;
        memcpy(sb->str+sb->len, str, str_len);
        sb->len += str_len;
    }

    return str_succ;
}

int string_b_push_sstr(string_b *sb, const char *str, size_t size){
    if(sb == NULL || str == NULL){
        fprintf(stderr, "Erorr, NULL Pointer\n");
        return str_err;
    }

    if(sb->str == NULL || sb->cap == 0){
        fprintf(stderr, "Erorr, Invalid String Buffer\n");
        return str_err;
    }

    if(sb->cap > sb->len + size){
        memcpy(sb->str+sb->len, str, size);
        sb->len += size;
    }else{
        size_t temp_cap = sb->cap * 2;
        char * temp  = realloc(sb->str, sb->cap);
        if(temp == NULL){
            fprintf(stderr, "Erorr, Realocation Failed\n");
            return str_fail;
        }
        sb->cap = temp_cap;
        sb->str = temp;
        memcpy(sb->str+sb->len, str, size);
        sb->len += size;
    }

    return str_succ;
}

void string_b_println(const string_b *s){
    if(s->str == NULL){
        // write(1, "empty\n", 6);
        fwrite("empty\n", 1, 6, stdout);
        return;
    }
    fwrite(s->str, 1, s->len,stdout);
    fwrite("\n", 1, 1,stdout);
}

void string_b_print(const string_b *s){
    if(s->str == NULL){
        // write(1, "empty", 5);
        fwrite("empty", 1, 5, stdout);
        return;
    }
    // write(1, s->str, s->len);
    fwrite(s->str, 1, s->len,stdout);
}

void string_b_free(string_b *dest){
    free(dest->str);
    dest->cap = 0;
    dest->len = 0;
}