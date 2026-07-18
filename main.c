#include "includes/lib3man.h"

void testing_sv_to_float64();
void testing_sb();
void testing_matrix();
void test_ArenaList();
void testing_csv();

int main(){
    // testing_sb();
    // Buffer b = buffer_read_file("main.c");
    // sv_print(*((string_view*)&b));
    // if(buffer_write_file(b, "test.txt") < 0){
    //     printf("file writing failed\n");
    // }
    void testing_sv_to_float64();
    // testing_matrix();
    // test_ArenaList();
    // testing_csv();
    return 0;
}

void testing_matrix(){
    Matrix m = matrix_create_empty(4, 3);
    Matrix b = matrix_create_empty(4, 3);
    f64 arr[3][2] = {{6, 5}, {4, 3}, {2, 1}};
    Matrix d = matrix_create(3, 2, arr);
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
    printf("----------------------------\n");
    Matrix t = matrix_create_empty(3, 4);
    matrix_fill(&t, 2);
    matrix_print(m);
    printf("----------------------------\n");
    Matrix x = matrix_dot(m, t);
    matrix_print(x);
    printf("----------------------------\n");
    Matrix mp = matrix_transpose(m);
    matrix_print(mp);
    printf("----------------------------\n");
    printf("%lx\n", u64_bswap(0xFFABBCEFFFABBCEFULL));
    // Free the memory that has the data and reset the matrix (rows = 0 & cols = 0)
    matrix_free(&m);
    matrix_free(&d);
    matrix_free(&b);
    matrix_free(&s);
    // matrix_free(&t);
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
    fclose(f);
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

void test_ArenaList(){
    ArenaList * a = create_ArenaList(KiB(13));
    printf("capacity :%zu adress :%p cur_size: %zu\n", a->arena.capacity, a->arena.address, a->arena.cur_size);
    for(int i = 0; i < 200; i++){
        int * tmp = arenaList_Alloc(&a, 55);
        printf("tmp_address = %zu\n", (u64)tmp);
    }
    arenaList_free(a);
    return;
}

void testing_csv(){
    CSV *csv = load_csv("test1.csv");
    // CSV *csv = load_csv("test1.csv");
    if(csv == NULL){
        printf("failed to read csv\n");
        return;
    }

    csv_print_head(csv);
    // csv_print_row(csv->data[5], csv->numcols);
    for(size_t i = 0; i < csv->numrows; i++){
        csv_print_row(csv->data[i], csv->types,csv->numcols);
    }
    
    printf("numcols: %zu  %zu\n", csv->numcols, csv->numrows);
    string_view to_look_for = sv_from_lit("Last Name");
    csv_print_column_from_string(csv , to_look_for);

    // changing a column name 
    sv t = sv_from_lit("anger");
    csv->head[2] = t;

    // ((f64**)csv->data)[0][2] = 22.0;
    // TODO: add typing to writing csv
    // writing the file
    csv_write_file("out.csv", csv);
    csv_write_json(csv, "test.json");
    // writing all the types
    csv_print_types(csv);
    printf("i64 : %ld\n", csv_get_int_by_name(csv, 0, sv_from_lit("x")));
    printf("f64 : %lf\n", csv_get_float_by_name(csv, 0, sv_from_lit("x")));
    csv_free(csv);
}