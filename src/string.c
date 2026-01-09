#include "../includes/lib3man.h"

// Importent replaced wirte with fwrite :
// 1- for buffering meaning printing is faster
// 2- compatibility with windows 
// TODO: strview inside areanaList

// string View functions ##################################################################
// creating a strview from const char *
strview strview_fcstr(const char *str, size_t len){
    if(str == NULL) return (strview){.str = NULL, .len = 0};
    return (strview){.str = (char*) str, .len = len};
}

int strview_cmp(const strview *s1, const strview *s2){
    if(s1->len != s2->len) return false;
    for(size_t i = 0; i < s1->len; i++){
        if(s1->str[i] != s2->str[i]) return false;
    }
    return true;
}

void strview_println(const strview *s){
    if(s->str == NULL){
        fwrite("empty\n", 1, 6, stdout);
        return;
    }
    fwrite(s->str, 1, s->len,stdout);
    fwrite("\n", 1, 1,stdout);
    // write(1, (char *)s->str, s->len); // write is too slow no buffering
    // write(1, "\n", 1);
}

void strview_print(const strview *s){
    if(s->str == NULL){
        fwrite("empty", 1, 5, stdout);
        return;
    }
    fwrite(s->str, 1, s->len,stdout);
    // write(1, (char *)s->str, s->len); // write is too slow no buffering
}


// string buffer functions ##################################################################

// TODO: strbuf inside areana and arenaList

// creating a strbuf from char *
strbuf strbuf_fcstr(const char *s){
    if(s == NULL) return (strbuf){.str = NULL, .len = 0, .cap = 0};

    size_t len = strlen(s);
    size_t cap = len * 4;
    char *temp = malloc(cap);

    if(temp == NULL){
        fprintf(stderr, "Error, Allocation Failed");
        return (strbuf){.str = NULL, .len = 0, .cap = 0};
    }

    return (strbuf){.str = temp, .len = len, .cap = cap};
}

strbuf strbuf_arenaList_cstrsz(ArenaList *arenaList, const char *s, size_t len){
    size_t cap = len * 4;
    char *temp = arenaList_Alloc(arenaList, cap);
    if(temp == NULL){
        fprintf(stderr, "Error, Allocation Failed");
        return (strbuf){.str = NULL, .len = 0, .cap = 0};
    }
    memcpy(temp, s, len);
    return (strbuf){.str = temp, .len = len, .cap = cap};
}

strbuf *strbuf_cat(strbuf *dest, strbuf  *src){
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

strbuf strbuf_fstrview(const strview *s){
    if(s->len == 0 || s->str == NULL) 
        return (strbuf){.str = NULL, .len = 0, .cap = 0};

    char * temp = malloc(s->len * 4);

    if(temp == NULL){
        fprintf(stderr, "Error, Allocation Failed");
        return (strbuf){.str = NULL, .len = 0, .cap = 0};
    }

    memcpy(temp, s->str, s->len);
    
    return (strbuf){.str = temp, .len = s->len, .cap = s->len*4};
}

int strbuf_push_strview(strbuf *sb, const strview *sv){
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

int strbuf_push_cstr(strbuf *sb, const char *str){
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

int strbuf_push_cstrsz(strbuf *sb, const char *str, size_t size){
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

int strbuf_push_char(strbuf *sb, char ch){
    if(sb->cap > sb->len + 1){
        sb->str[sb->len] = ch;
        sb->len++;
    }else{
        size_t temp_cap = sb->cap * 2;
        char * temp  = realloc(sb->str, sb->cap);
        if(temp == NULL){
            fprintf(stderr, "Erorr, Realocation Failed\n");
            return str_fail;
        }
        sb->str = temp;
        sb->str[sb->len] = ch;
        sb->len++;
        sb->cap = temp_cap;
    }
    return str_succ;
}

void strbuf_println(const strbuf *sb){
    if(sb->str == NULL){
        // write(1, "empty\n", 6);
        fwrite("empty\n", 1, 6, stdout);
        return;
    }
    fwrite(sb->str, 1, sb->len,stdout);
    fwrite("\n", 1, 1,stdout);
}

void strbuf_print(const strbuf *sb){
    if(sb->str == NULL){
        // write(1, "empty", 5);
        fwrite("empty", 1, 5, stdout);
        return;
    }
    // write(1, s->str, s->len);
    fwrite(sb->str, 1, sb->len,stdout);
}

void strbuf_free(strbuf *sb){
    free(sb->str);
    sb->cap = 0;
    sb->len = 0;
}