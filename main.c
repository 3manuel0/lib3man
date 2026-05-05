#include "includes/lib3man.h"
#include <stddef.h>
#include <stdio.h>

void testing_sv_to_float64();
void testing_sb();
void testing_matrix();

int main(){
    // testing_sb();
    testing_matrix();
}

void testing_matrix(){
    Matrix m = matrix_create(4, 3);
    Matrix b = matrix_create(4, 3);
    matrix_fill(&m, 20);
    matrix_fill(&b, 60);
    matrix_print(m);
    matrix_scale(&m, 2.5);
    matrix_add(&m, b);
    printf("----------------------------\n");
    matrix_print(m);
    printf("----------------------------\n");
    matrix_sub(&m, b);
    matrix_print(m);
    printf("----------------------------\n");
    matrix_randomize(&m, -10, 105.0);
    // matrix_map(&m, sigmoid);
    matrix_print(m);
    printf("----------------------------\n");
    Matrix s = matrix_copy(m);
    matrix_sub(&s, b);
    matrix_print(s);
    // Free the memory that has the data and reset the matrix (rows = 0 & cols = 0)
    matrix_free(&m);
    matrix_free(&b);
    matrix_free(&s);
}

void testing_sb(){
    string_buffer str = create_sb_empty(50);
    sb_freadln(&str, stdin);
    sb_println(str);
    sb_free(&str);
    string_buffer str2 = create_sb_empty(100);
    FILE * f = fopen("src/string.c", "rb");
    sb_fread_all(&str2, f);
    sb_print(str2);
    string_buffer sb_fc = sb_from_cstr("this is just a test");
    sb_println(sb_fc);
    sb_free(&str2);
    sb_free(&sb_fc);
}

void testing_sv_to_float64(){
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
        sv_from_lit("1.8e+308"), // too big for (inf/infinity)
        sv_from_lit("80")
    };

    f64 t = 0.0;
    for(int i = 0; i < 13; i++){
        int s = sv_to_float64(&tests[i], &t);
        if(s)
            printf("the output = %lf\n", t);
    }
}