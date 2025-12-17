#include "lib3man.h"

int main(){
    Arena arena;
    char * str = "testing testing 123\n";
    arena_Alloc(&arena, str, strlen(str));
    arena_Alloc(&arena, str, strlen(str));
    arena_Alloc(&arena, str, strlen(str));
    arena_Alloc(&arena, str, strlen(str));
    int a = 500;
    arena_Alloc(&arena, &a, sizeof(int));
    printf("%zu %zu\n", arena.cur_size, strlen(str) * 4);
    for(int i = 0; i < arena.cur_size; i++){
        printf("%c", *((char *)arena.memory + i));
        // printf("%u ", *((unsigned char *)arena.memory + i));
    }
    arena_free(&arena);
    return 0;
}