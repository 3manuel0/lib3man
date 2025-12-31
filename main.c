#include "includes/lib3man.h"

int main(){
    string_b s = string_b_from_string_v(&string_v_from_lit("anger builds"));
    printf("cap:%zu len:%zu\n", s.cap, s.len);
    string_v_println((string_v*)&s);
    return 0;
}