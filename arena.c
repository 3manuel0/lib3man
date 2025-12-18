#include "lib3man.h"

void create_Arena(Arena * arena){
    arena->capacity = 5 * 1024 * 1024; // 5mb
    arena->memory = malloc(arena->capacity);
    if(arena->memory == NULL){
        perror("Error allocating memory\n");
        exit(-1);
    }
    arena->address = arena->memory;
    arena->cur_size = 0;
}


void * arena_Alloc(Arena * arena, size_t size){
    void * ptr = NULL;
    if(arena->memory == NULL)create_Arena(arena);
    if(arena->cur_size + size < arena->capacity){
        ptr = arena->address;
        arena->address =  (char *)arena->address + size;
        arena->cur_size += size;
    }else{
        perror("arena is full\n");
        // I will add more options later
    }
    return ptr;
}

void arena_reset(Arena * arena){
    // restart the arena writing 
    // it's ok we write over old data if we don't need it
    arena->address = arena->memory;
    arena->cur_size = 0;
}


void arena_free(Arena * arena){
    if(arena->memory == NULL) return;
    // free the arena before closing the programme
    free(arena->memory);
    arena->address = NULL;
    arena->capacity = 0;
    arena->cur_size = 0;
}