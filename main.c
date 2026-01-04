#include "includes/lib3man.h"

int main(){
    // TODO : test string string_b_push_str and sstr
    string_b s = string_b_fstring_v(&string_v_from_lit("anger builds"));
    string_b_push_string_v(&s, &string_v_from_lit(" anger builds"));
    string_b_push_string_v(&s, &string_v_from_lit(" anger builds"));
    printf("cap:%zu len:%zu p:%p\n", s.cap, s.len, s.str);
    string_b_push_string_v(&s, &string_v_from_lit(" anger builds"));
    printf("cap:%zu len:%zu p:%p\n", s.cap, s.len, s.str);
    for(int i = 0; i < 100000; i++)
        string_b_println(&s);
    // 310 <-> 298 write
    // 182 <-> 160 fwrite fwrite is faster 
    return 0;
}