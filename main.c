#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/lib3man.h"

int main(void)
{
    /*
    test 1: sb_from_cstr
    this should print "hello", but it probably prints garbage
    because the string is never copied into the buffer
    */
    sb s = sb_from_cstr("hello");
    sb_println(&s);
    sb_free(&s);

    /*
    test 2: realloc issue
    pushing a lot of data should trigger realloc
    but since realloc uses the wrong size, this will crash
    */
    /*
    sb s = sb_from_cstr("A");
    memcpy(s.str, "A", 1);
    for (int i = 0; i < 1000; i++) {
        sb_push_cstr(&s, "B");
    }
    sb_println(&s);
    sb_free(&s);
    */

    /*
    test 3: null terminator overflow
    here we force len == cap, then try to add '\0'
    this should write out of bounds
    */
    /*
    sb s = sb_from_cstr("hello");
    memcpy(s.str, "hello", 5);
    s.len = s.cap;
    char *c = cstr_from_sb(&s);
    printf("%s\n", c);
    sb_free(&s);
    */

    /*
    test 4: split function (currently active)
    this one is very likely to crash because of the loop condition
    */
    
    /*
    sb s = sb_from_cstr("a,b,c,d");
    memcpy(s.str, "a,b,c,d", 7);
    s.len = 7;

    sv arr[10];
    size_t count = sb_split_svs_char(&s, ',', arr, 10);

    printf("Count: %zu\n", count);
    sb_free(&s);
    */ 

    /*
    test 5: bad assert
    this passes NULL and relies on a broken assert (|| instead of &&)
    should crash immediately
    */
    /*
    sb *s = NULL;
    sb_arenaList_push_cstr_sz(NULL, s, "hello", 5);
    */

    /*
    test 6: arena list behavior
    tries to force allocation into a new arena
    might cause weird memory issues
    */
    /*
    ArenaList *arena = create_ArenaList(16);

    char *a = arenaList_Alloc(arena, 8);
    strcpy(a, "1234567");

    char *b = arenaList_Alloc(arena, 2147483648);
    strcpy(b, "OVERFLOW_TEST");

    printf("a: %s | b: %s\n", a, b);

    arenaList_free(arena);
    */

    /*
    test 7: arena realloc
    tries to grow memory to see if realloc logic breaks
    */
    /*
    ArenaList *arena = create_ArenaList(16);

    char *p = arenaList_Alloc(arena, 8);
    strcpy(p, "test");

    p = arenaList_Realloc(arena, p, 8, 1024);

    printf("%s\n", p);

    arenaList_free(arena);
    */
    return 0;
}
