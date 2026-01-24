#include "includes/lib3man.h"
#include <string.h>
#include <sys/types.h>

int main(){
    // TODO : test string sb_push_str and sstr
    sb s = sb_from_sv(&sv_from_lit("anger builds"));
    sb_push_sv(&s, &sv_from_lit(" anger builds"));
    sb_push_sv(&s, &sv_from_lit(" anger builds"));
    printf("cap:%zu len:%zu p:%p\n", s.cap, s.len, s.str);
    sb_push_sv(&s, &sv_from_lit(" anger builds"));
    printf("cap:%zu len:%zu p:%p\n", s.cap, s.len, s.str);
    // for(int i = 0; i < 100000; i++)
    //     sb_println(&s);
    // 310 <-> 298 write
    // 182 <-> 160 fwrite fwrite is faster 
    ArenaList *a = create_ArenaList(MiB(250));
    sb test = sb_arenaList_from_cstr_sz(a, "anger builds\n", 13);
    printf("%p\n", test.str);
    sb_arenaList_push_cstr_sz(a, &test, "anger builds\n", 13);
    printf("%p\n", test.str);
    sb_arenaList_push_cstr_sz(a, &test, "anger builds\n", 13);
    printf("%p\n", test.str);
    // TESTS
    printf("%zu\n", sb_split_svs_char(&s, ' ', NULL, 0));
    sb_println(&s);
    return 0;
}