#include "includes/lib3man.h"

int main(){
    Arena arena = create_Arena(MiB(5));

    char * str = "testing testing 123\n";

    string d = string_from_lit("anger builds");

    printf("%zu %s", d.len, d.str);

    int str_len = strlen(str);

    char * s = arena_Alloc(&arena,str_len);

    memcpy(s, str, str_len);

    s = arena_Alloc(&arena, strlen(str));

    memcpy(s, str, str_len);

    s = arena_Alloc(&arena, strlen(str));

    memcpy(s, str, str_len);

    s = arena_Alloc(&arena, strlen(str));

    memcpy(s, str, str_len);

    int a = 500;

    int * n = arena_Alloc(&arena, sizeof(int));

    memcpy(n, &a, sizeof(int));
    
    printf("%zu %zu\n", arena.cur_size, strlen(str) * 4);

    for(size_t i = 0; i < arena.cur_size; i++){
        if(*((char *)arena.memory + i) < 32 || *((char *)arena.memory + i) > 126)
            printf(" %u ", *((unsigned char *)arena.memory + i));
        else printf("%c", *((char *)arena.memory + i));
    }

    arena_free(&arena);
    return 0;
}