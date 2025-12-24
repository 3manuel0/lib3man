#include "../includes/lib3man.h"
#include <stdlib.h>

Arena create_Arena(size_t arena_size){
    Arena arena = {0};
    arena.capacity = arena_size;
    arena.memory = malloc(arena.capacity);
    if(arena.memory == NULL){
        perror("Error allocating memory\n");
        exit(-1);
    }
    arena.address = arena.memory;
    arena.cur_size = 0;
    return arena;
}


void * arena_Alloc(Arena * arena, size_t size){
    void * ptr = NULL;
    if(arena->memory == NULL) return NULL;
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

// linked list of arenas in case the first arena got full
//  so we can deallocat everything in the end
void *arenas_Alloc(Arenas *arenas, size_t size){
    if(arenas->arena.capacity > arenas->arena.cur_size + size){
        return arena_Alloc(&arenas->arena, size);
    }else{
        arenas->next = malloc(sizeof(Arenas));
        size_t capacity = arenas->arena.capacity;
        arenas = arenas->next;
        arenas->arena = create_Arena(capacity);
        arenas->next = NULL;
        return arena_Alloc(&arenas->arena, size);
    }
}

// free all the arenas we created
void arenas_free(Arenas * head){
    while (head != NULL) {
        Arenas * temp = head;
        head = head->next;
        free(temp);
    }
}
