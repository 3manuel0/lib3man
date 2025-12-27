#include "includes/lib3man.h"

int main(){
    ArenaList arenaList = (ArenaList){.arena = create_Arena(MiB(2)), .next = NULL};

    char * str = "testing testing 123\n";

    char * test = arenaList_Alloc(&arenaList, 500); 
    char * a = arenaList_Alloc(&arenaList, 500);
    char * b = arenaList_Alloc(&arenaList, 500);
    printf("%ld", (size_t)arenaList.arena.memory - (size_t)arenaList.arena.address);
    return 0;
}