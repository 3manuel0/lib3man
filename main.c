#include "includes/lib3man.h"
#include <stdlib.h>

int main(){
    ArenaList * arenaList = create_ArenaList(MiB(2));
    char * test = arenaList_Alloc(arenaList, 500); 
    char * a = arenaList_Alloc(arenaList, 500);
    char * b = arenaList_Alloc(arenaList, 500);
    printf("%ld", (size_t)arenaList->arena.memory - (size_t)arenaList->arena.address);
    arenaList_free(arenaList);
    return 0;
}