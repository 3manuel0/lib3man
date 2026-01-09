#include "includes/lib3man.h"
#include <stdio.h>

int main(){
    // TODO : test string strbuf_push_str and sstr
    strbuf s = strbuf_fstrview(&strview_from_lit("anger builds"));
    strbuf_push_strview(&s, &strview_from_lit(" anger builds"));
    strbuf_push_strview(&s, &strview_from_lit(" anger builds"));
    printf("cap:%zu len:%zu p:%p\n", s.cap, s.len, s.str);
    strbuf_push_strview(&s, &strview_from_lit(" anger builds"));
    printf("cap:%zu len:%zu p:%p\n", s.cap, s.len, s.str);
    // for(int i = 0; i < 100000; i++)
    //     strbuf_println(&s);
    // 310 <-> 298 write
    // 182 <-> 160 fwrite fwrite is faster 
    ArenaList *a = create_ArenaList(MiB(250));
    strbuf test = strbuf_arenaList_cstrsz(a, "anger builds\n", 13);
    strbuf_print(&test);
    return 0;
}