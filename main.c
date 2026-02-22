#include "includes/lib3man.h"
#include <stdio.h>


int main(){
    sv tests[] = {
        sv_from_lit("123.456"),
        sv_from_lit("-987"),
        sv_from_lit("000.000100"),
        sv_from_lit(".505"),
        sv_from_lit("1.23e-4"),
        sv_from_lit("5.5E+2"),
        sv_from_lit("0.12345678901234567"),
        sv_from_lit("3.14159junk"),
        sv_from_lit("12.34.56"),
        sv_from_lit("4.9e-324"),// too small rounds to 0 
        sv_from_lit("1.7976931348623157891575E+308"),// max_float64
        sv_from_lit("1.8e+308") // too big for (inf/infinity)
    };

    f64 t = 0.0;
    for(int i = 0; i < 12; i++){
        int s = sv_to_float64(&tests[i], &t);
        if(s)
            printf("the output = %lf\n", t);
    }

    return 0;
}

void last_test(){
    string_view strv = sv_from_lit("2147483647");
    i32 n_ = 0;
    int s = sv_to_int32(&strv, &n_);
    printf("successful:%s number32:%d\n", s ? "true" : "false", n_);
    i64 n = 0;
    s = sv_to_int64(&strv, &n);
    printf("successful:%s number64:%ld\n", s ? "true" : "false", n);
    // double experiment = 1.7976931348623157891575E+308;
    double experiment = 645644564.5555654464456456664;
    long e = 0x7FF0000000000000;
    printf("%ld %.19lf %lb\n", *(long*)&experiment, *(double*)&e, e);
}

void old_test(){
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
}