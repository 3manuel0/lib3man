#include "../includes/lib3man.h"
#include <assert.h>
#include <stdio.h>

Arena create_Arena(size_t arena_size){
    Arena arena = {0};
    arena.capacity = arena_size;
    arena.memory = malloc(arena.capacity);
    if(arena.memory == NULL){
        fprintf(stderr, "Error, Arena Allocation Failed\n");
        exit(-1);
    }
    arena.address = arena.memory;
    arena.cur_size = 0;
    return arena;
}

void * arena_Alloc(Arena * arena, size_t size){
    assert(arena != NULL && size > 0);

    void * ptr = NULL;
    if(arena->memory == NULL) return NULL;
    if(arena->cur_size + size < arena->capacity){
        ptr = arena->address;
        arena->address =  (char *)arena->address + size;
        arena->cur_size += size;
    }else{
        fprintf(stderr, "Error, Arena is Full\n");
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
    assert(arena->memory != NULL);
    // free the arena before closing the programme
    free(arena->memory);
    arena->memory = NULL;
    arena->address = NULL;
    arena->capacity = 0;
    arena->cur_size = 0;
}


ArenaList *create_ArenaList(size_t size){
    ArenaList * arenaList = malloc(sizeof(ArenaList));
    if(arenaList == NULL){
        fprintf(stderr, "Error, ArenaList Allocation Failed\n");
        return NULL;
    }
    arenaList->next = NULL;
    if(arenaList == NULL) return NULL;
    *arenaList = (ArenaList){.arena = create_Arena(size), .next = NULL};
    return arenaList;
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

void *arenaList_Realloc(ArenaList * arenaList, void *p, size_t oldsz , size_t newsz){
    assert(arenaList != NULL && p != NULL);
    assert(newsz > oldsz && "new size is less or equals old size");
    size_t diff = newsz - oldsz;

    if((char *)p + oldsz == arenaList->arena.address && \
        arenaList->arena.cur_size + diff <= arenaList->arena.capacity)
    {
        arenaList->arena.address = (char *)arenaList->arena.address + diff;
        arenaList->arena.cur_size += diff;
        return p;
    }else{
        void *temp = arenaList_Alloc(arenaList, newsz);
        memcpy(temp, p, oldsz);
        return temp;
    }
}

// free all the arenas we created
void arenaList_free(ArenaList * head){
    while (head != NULL) {
        ArenaList * temp = head;
        head = head->next;
        arena_free(&temp->arena);
        free(temp);
    }
}
