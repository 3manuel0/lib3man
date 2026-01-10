#include "../includes/lib3man.h"
#include <assert.h>
#include <string.h>

// Importent replaced wirte with fwrite :
// 1- for buffering meaning printing is faster
// 2- compatibility with windows 
// TODO: sv inside areanaList

// string View functions ##################################################################
// creating a sv from const char *
sv sv_from_cstr_sz(const char *str, size_t size){
    if(str == NULL) return (sv){.str = NULL, .len = 0};
    return (sv){.str = (char*) str, .len = size};
}

int sv_cmp(const sv *sv1, const sv *sv2){
    if(sv1->len != sv2->len) return false;
    for(size_t i = 0; i < sv1->len; i++){
        if(sv1->str[i] != sv2->str[i]) return false;
    }
    return true;
}

void sv_println(const sv *sv){
    if(sv->str == NULL){
        fwrite("empty\n", 1, 6, stdout);
        return;
    }
    fwrite(sv->str, 1, sv->len,stdout);
    fwrite("\n", 1, 1,stdout);
    // write(1, (char *)s->str, s->len); // write is too slow no buffering
    // write(1, "\n", 1);
}

void sv_print(const sv *sv){
    if(sv->str == NULL){
        fwrite("empty", 1, 5, stdout);
        return;
    }
    fwrite(sv->str, 1, sv->len,stdout);
    // write(1, (char *)s->str, s->len); // write is too slow no buffering
}


// string buffer functions ##################################################################

// TODO: sb inside areana and arenaList

// creating a sb from char *
sb sb_from_cstr(const char *str){
    assert(str != NULL);
    size_t len = strlen(str);
    size_t cap = len * 4;
    char *temp = malloc(cap);

    if(temp == NULL){
        fprintf(stderr, "Error, Allocation Failed");
        return (sb){.str = NULL, .len = 0, .cap = 0};
    }

    return (sb){.str = temp, .len = len, .cap = cap};
}

sb sb_arenaList_from_cstr_sz(ArenaList *arenaList, const char *str, size_t size){
    size_t cap = size * 4;
    char *temp = arenaList_Alloc(arenaList, cap);
    if(temp == NULL){
        fprintf(stderr, "Error, Allocation Failed");
        return (sb){.str = NULL, .len = 0, .cap = 0};
    }
    memcpy(temp, str, size);
    return (sb){.str = temp, .len = size, .cap = cap};
}

int sb_arenaList_push_cstr_sz(ArenaList *arenaList, sb *sb, const char *str, size_t size){
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
        char * temp  = arenaList_Realloc(arenaList, sb->str, sb->cap, sb->cap * 2);
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

sb *sb_cat(sb *dest, sb  *src){

    assert(dest->len > 0 && src->str != NULL && dest->str != NULL && src->len != 0);

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

sb sb_from_sv(const sv *sv){
    assert(sv->len > 0 && sv->str != NULL);

    char * temp = malloc(sv->len * 4);

    if(temp == NULL){
        fprintf(stderr, "Error, Allocation Failed");
        return (sb){.str = NULL, .len = 0, .cap = 0};
    }

    memcpy(temp, sv->str, sv->len);
    
    return (sb){.str = temp, .len = sv->len, .cap = sv->len*4};
}

int sb_push_sv(sb *sb, const sv *sv){
    assert(sb != NULL && sv != NULL);

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

int sb_push_cstr(sb *sb, const char *str){
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

int sb_push_cstr_sz(sb *sb, const char *str, size_t size){
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

int sb_push_char(sb *sb, char ch){
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

char * cstr_from_sb(const sb *sb){
    assert(sb != NULL && sb->len > 0);
    sb->str[sb->len] = 0;
    return sb->str;
}

void sb_println(const sb *sb){
    if(sb->str == NULL){
        // write(1, "empty\n", 6);
        fwrite("empty\n", 1, 6, stdout);
        return;
    }
    fwrite(sb->str, 1, sb->len,stdout);
    fwrite("\n", 1, 1,stdout);
}

void sb_print(const sb *sb){
    if(sb->str == NULL){
        // write(1, "empty", 5);
        fwrite("empty", 1, 5, stdout);
        return;
    }
    // write(1, s->str, s->len);
    fwrite(sb->str, 1, sb->len,stdout);
}

void sb_free(sb *sb){
    free(sb->str);
    sb->str = NULL;
    sb->cap = 0;
    sb->len = 0;
}