#include "includes/lib3man.h"

int main(){
    // TODO : test string string_b_push_str and sstr
    string_b s = string_b_fstring_v(&string_v_from_lit("anger builds"));
    string_b_push_string_v(&s, &string_v_from_lit(" anger builds"));
    string_b_push_string_v(&s, &string_v_from_lit(" anger builds"));
    printf("cap:%zu len:%zu p:%p\n", s.cap, s.len, s.str);
    string_b_push_string_v(&s, &string_v_from_lit(" anger builds"));
    printf("cap:%zu len:%zu p:%p\n", s.cap, s.len, s.str);
    string_b_println(&s);
    return 0;
}