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
void *arenaList_Alloc(ArenaList *arenalist, size_t size){
    if(arenalist->arena.capacity > arenalist->arena.cur_size + size){
        return arena_Alloc(&arenalist->arena, size);
    }else{
        arenalist->next = malloc(sizeof(ArenaList));
        size_t capacity = arenalist->arena.capacity;
        arenalist = arenalist->next;
        arenalist->arena = create_Arena(capacity);
        arenalist->next = NULL;
        return arena_Alloc(&arenalist->arena, size);
    }
}

// free all the arenas we created
void arenaList_free(ArenaList * head){
    while (head != NULL) {
        ArenaList * temp = head;
        head = head->next;
        free(temp);
    }
}
