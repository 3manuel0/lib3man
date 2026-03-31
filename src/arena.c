#include "../includes/lib3man.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

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

void *arena_Alloc(Arena * arena, size_t size){
    assert(arena != NULL && size > 0);
    assert(arena->capacity >= size);
    void * ptr = NULL;
    if(arena->memory == NULL) return NULL;
    if(arena->cur_size + size <= arena->capacity){
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
    assert(arena != NULL);
    assert(arena->memory != NULL);
    // free the arena before closing the programme
    free(arena->memory);
    arena->memory = NULL;
    arena->address = NULL;
    arena->capacity = 0;
    arena->cur_size = 0;
}

ArenaList *create_ArenaList(size_t size){
    if(size == 0) return NULL;

    ArenaList * arenaList = malloc(sizeof(ArenaList));

    if(arenaList == NULL){
        fprintf(stderr, "Error, ArenaList Creation (AreanaList struct Allocation) Failed\n");
        return NULL;
    }
    // adding prev to the creation
    *arenaList = (ArenaList){.arena = create_Arena(size), .prev = NULL, .next = NULL};
    return arenaList;
}

// linked list of arenas in case the first arena got full
//  so we can deallocat everything in the end
void *arenaList_Alloc(ArenaList **arenalist, size_t size){
    assert(arenalist != NULL);
    assert(*arenalist != NULL);
    if((*arenalist)->arena.capacity >= (*arenalist)->arena.cur_size + size){
        return arena_Alloc(&(*arenalist)->arena, size);
    }

    ArenaList * prev = (*arenalist)->prev;
    
    while(prev != NULL){
        if(prev->arena.capacity >= prev->arena.cur_size + size){
            return arena_Alloc(&prev->arena, size);
        }
        prev = prev->prev;
    }

    (*arenalist)->next = malloc(sizeof(ArenaList));

    if((*arenalist)->next == NULL){
        fprintf(stderr, "Error, ArenaList Allocation Failed\n");
        exit(-1);
    }
    
    size_t capacity = (*arenalist)->arena.capacity;

    while(size > capacity){
        capacity *= 2;
    }

    prev = (*arenalist);
    *arenalist = (*arenalist)->next;
    (*arenalist)->prev = prev;
    (*arenalist)->arena = create_Arena(capacity);
    (*arenalist)->next = NULL;
    return arena_Alloc(&(*arenalist)->arena, size);
}

// TODO MAKE THIS BETTER
void *arenaList_Realloc(ArenaList **arenaList, void *p, size_t oldsz , size_t newsz){
    assert(arenaList != NULL && p != NULL && oldsz > 0 && newsz > 0 && newsz > oldsz);
    
    size_t diff = newsz - oldsz;

    if(diff == 0){
        fprintf(stderr, "No size change no Reallocation\n");
        return NULL;
    } 

    if((char *)p + oldsz == (*arenaList)->arena.address && \
        (*arenaList)->arena.cur_size + diff <= (*arenaList)->arena.capacity)
    {
        (*arenaList)->arena.address = (char *)(*arenaList)->arena.address + diff;
        (*arenaList)->arena.cur_size += diff;
        return p;
    }

    void *temp = arenaList_Alloc(arenaList, newsz);
    memcpy(temp, p, oldsz);
    return temp;

}

// free all the arenas we created
void arenaList_free(ArenaList * arenaList){
    while (arenaList != NULL) {
        printf("+++++ arenaList Address  = %p +++++++++++\n", arenaList);
        ArenaList * temp = arenaList;
        arenaList = arenaList->prev;
        arena_free(&temp->arena);
        free(temp);
    }
}

