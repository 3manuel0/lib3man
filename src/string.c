#include "../includes/lib3man.h"
#include <stddef.h>

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

sv sv_from_cstr(const char *str){
    assert(str != NULL);
    size_t size = strlen(str);
    return (sv){.str = (char*) str, .len = size};
}

sv sv_from_sb(const sb *sb){
    assert(sb != NULL);
    assert(sb->str != NULL && sb->len > 0);
    return (sv){.str = sb->str, .len = sb->len};
}

// returns the number of sub strings (svs)
size_t sb_split_svs_char(const sb * sb, char delimiter, sv * sv_arr /* can be NULL*/, size_t sv_arr_len /* can be 0*/){
    size_t count = 0;
    if(delimiter == 0) return 0;
    if(sv_arr == NULL || sv_arr_len == 0){
        for(size_t i = 0; i < sb->cap; i++){
            if(sb->str[i] == delimiter) count++;
        }
        count++;
    }else{
        size_t len = 0;
        char * tmp = sb->str;
        size_t total = 0;
        for(size_t i = 0; i < sv_arr_len; i++){
            while(tmp[len] != delimiter || total < sb->len){
                len++;
                total++;
            }
            sv_arr[i].str = tmp;
            sv_arr[i].len = len;
            tmp += len;
            len = 0;
        }
        if(total < sb->len){
            for(size_t i = 0; i < sb->cap; i++){
                if(sb->str[i] == delimiter) count++;
            }
            count++;
        }
    }

    return count;
}

int sv_cmp(const sv *sv1, const sv *sv2){
    if(sv1->len != sv2->len) return false;
    for(size_t i = 0; i < sv1->len; i++){
        if(sv1->str[i] != sv2->str[i]) return false;
    }
    return true;
}

int sv_to_int64(const sv *sv, i64 *out){
    assert(sv != NULL && out != NULL);

    if(sv->len > 20) return false;
    if(sv->len == 20 && (sv->str[0] != '-' || sv->str[0] != '+'))
        return false;
    i64 temp = 0;
    u32 i = 0;
    u8 is_negative = 0;

    if(sv->str[0] == '-'){
        is_negative = 1;
        i++;
    }else if(sv->str[0] == '+'){
        i++;
    }else if(sv->str[0] < '0' || sv->str[0] > '9') return false;

    for( ; i < sv->len; i++){
        if(sv->str[i] < '0' || sv->str[i] > '9') return false;
        temp *=  10;
        temp += sv->str[i] - '0';
    }
    
    if(!is_negative && temp < 0) return false;
    if(is_negative) temp = -temp;
    if(is_negative && temp > 0) return false;
    *out = temp;

    return true;
}

int sv_to_int32(const sv *sv, i32 *out){
    assert(sv != NULL && out != NULL);

    if(sv->len > 11) return false;
    if(sv->len == 11 && (sv->str[0] != '-' || sv->str[0] != '+'))
        return false;
    
    i32 temp = 0;
    u32 i = 0;
    u8 is_negative = 0;

    if(sv->str[0] == '-'){
        is_negative = 1;
        i++;
    }else if(sv->str[0] == '+'){
        i++;
    }
    else if(sv->str[0] < '0' || sv->str[0] > '9') return false;

    for( ; i < sv->len; i++){
        if(sv->str[i] < '0' || sv->str[i] > '9') return false;
        temp *=  10;
        temp += sv->str[i] - '0';
    }
    
    if(!is_negative && temp < 0) return false;
    if(is_negative) temp = -temp;
    if(is_negative && temp > 0) return false;
    *out = temp;

    return true;
}

int sv_to_float64(const sv *sv, f64 *out){
    assert(sv != NULL && out != NULL);

    u32 i = 0;
    u8 is_negative = 0;
    f64 temp = 0.0;
    f64 exponent = 1.0;
    // printf("-------------------------------------------\n");

    if(sv->str[0] == '-' || sv->str[0] == '+'){
        is_negative = 1;
        i++;
    }

    for(;i < sv->len; i++){
        if(sv->str[i] < '0' || sv->str[i] > '9')
            break;
        temp *= 10;
        temp += sv->str[i] - '0';
    }

    // sv_println(sv);
    // printf("break was in %c | f64 = %.8lf\n", sv->str[i], temp);
    
    if(sv->str[i] == '.') i++;
    else return false;

    for(f64 t = 10.0;i < sv->len; i++){
        if(sv->str[i] < '0' || sv->str[i] > '9')
            break;
        temp += (sv->str[i] - '0') / t;
        t*=10.0;
    }
    // printf("break was in %c | f64 = %.8lf\n", sv->str[i], temp);
    
    // TODO: DO MORE TESTS AND FIXES
    if(i < sv->len){

        if(sv->str[i] != 'e' && sv->str[i] != 'E') return false;
        i++;
        char neg = 0;
        int exp = 0;

        // printf("sign = %c\n", sv->str[i]);

        if(sv->str[i] == '+'){
            neg = 0;
        }else if(sv->str[i] == '-'){
            neg = 1;
        }else return false;
        i++;

        for( ; i < sv->len; i++){
            if(sv->str[i] < '0' || sv->str[i] > '9') return false;
            exp *=  10;
            exp += sv->str[i] - '0';
        }

        // exponent = neg ? pow(10, -exp) : pow(10, exp);
        // no need for pow also no need to link with math (-lm)
        if(neg){
            while (exp > 0) { exponent /= 10;
                exp--;
            }
        }else{
            while (exp > 0) { exponent *= 10;
                exp--;
            }
        }
        // printf(" exp = %d exponent = %lf f64 = %lf\n", exp, exponent, temp * exponent);
    }

    if(exponent != 0.0)
        *out = temp * exponent;
    else
        *out = temp;

    if(is_negative) *out = -*out;

    return true;
}

void sv_println(const sv *sv){
    if(sv == NULL){
        fwrite("NULL\n", 1, 5, stdout);
        return;
    }
    if(sv->str == NULL || sv->len == 0){
        fwrite("EMPTY_STR\n", 1, 10, stdout);
        return;
    }
    fwrite(sv->str, 1, sv->len,stdout);
    fwrite("\n", 1, 1,stdout);
    // write(1, (char *)s->str, s->len); // write is too slow no buffering
    // write(1, "\n", 1);
}

void sv_print(const sv *sv){
    if(sv == NULL){
        fwrite("NULL", 1, 4, stdout);
        return;
    }
    if(sv->str == NULL || sv->len == 0){
        fwrite("EMPTY_STR", 1, 9, stdout);
        return;
    }
    fwrite(sv->str, 1, sv->len,stdout);
    // write(1, (char *)s->str, s->len); // write is too slow no buffering
}


// string buffer functions ##################################################################

// TODO: sb inside areana and arenaList ## Almost done

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

sb create_sb_inside_arenaList(ArenaList *arenaList, size_t cap){
    assert(arenaList != NULL && cap > 0);
    char *temp = arenaList_Alloc(arenaList, cap);
    if(temp == NULL){
        fprintf(stderr, "Error, Allocation Failed");
        return (sb){.str = NULL, .len = 0, .cap = 0};
    }
    return (sb){.str = temp, .len = 0, .cap = cap};
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
    assert(sb != NULL || str != NULL);

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

int sb_arenaList_push_sv(ArenaList *arenaList, sb *sb, sv sv){
    assert(sb != NULL || sv.str != NULL);

    if(sb->str == NULL || sb->cap == 0){
        fprintf(stderr, "Erorr, Invalid String Buffer\n");
        return str_err;
    }

    if(sb->cap > sb->len + sv.len){
        memcpy(sb->str+sb->len, sv.str, sv.len);
        sb->len += sv.len;
    }else{
        size_t temp_cap = sb->cap * 2;
        char * temp  = arenaList_Realloc(arenaList, sb->str, sb->cap, sb->cap * 2);
        if(temp == NULL){
            fprintf(stderr, "Erorr, Realocation Failed\n");
            return str_fail;
        }
        sb->cap = temp_cap;
        sb->str = temp;
        memcpy(sb->str+sb->len, sv.str, sv.len);
        sb->len += sv.len;
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
        sb_free(src);
        return dest;
    }
    memcpy(&dest->str[dest->len],src->str , src->len);
    dest->len += src->len;
    sb_free(src);
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