#include "../includes/lib3man.h"

// TODO: ADD OTHER SUPPORT FOR OTHER TYPES 

// ***************************************************************************
    void csv_to_memory(u8 *mem, FILE *file, size_t size, size_t *numcolumns,
                    size_t *numrows);

    int csv_parse_head(CSV *csv, u8 *mem);

    u8 *csv_parse_row(ArenaList *arena, string_view *csv_row, csv_type *csv_data, u8 *mem);

    csv_type get_type(string_view *sv, csv_type t);

    void csv_parse_with_types(CSV *csv);

    void sv_write_j(const sv *sv, FILE *f);

    void *csv_get_cell(const CSV *csv, size_t row, size_t col);

    int csv_parse(CSV *csv, u8 *mem);
// ****************************************************************************
 
CSV *load_csv(char *file_name){
    FILE * csv_f = fopen(file_name, "r");

    if(csv_f == NULL){
        fprintf(stderr, "Error, Can't open file or wrong path!\n");
        return NULL;
    }

    CSV * csv = create_csv();

    if(csv == NULL){
        fprintf(stderr, "Error, CSV Object Creation/Allocation Failed!\n");
        csv_free(csv);
        return NULL;
    }

    fseek(csv_f, 0, SEEK_END);

    size_t temp_file_size = ftell(csv_f);
    u8 * csv_mem = calloc(temp_file_size + KiB(2), sizeof(u8));

    if(csv_mem == NULL){
        fprintf(stderr, "Error, Memory Allocation Failed");
        csv_free(csv);
        return NULL;
    }

    printf("size + 2kib=%zu size=%zu\n", ftell(csv_f) + KiB(2), ftell(csv_f));

    rewind(csv_f);
    // columns left -> right
    // rows top -> bottom
    csv_to_memory(csv_mem, csv_f, temp_file_size, &csv->numcols, &csv->numrows);
    if(csv->numcols == 0 || csv->numrows == 0){
        printf("Error, csv Parcing Failed!\n");
        csv_free(csv);
        free(csv_mem);
        return NULL;
    }
    
    if(csv_parse_head(csv, csv_mem)){
        csv_free(csv);
        free(csv_mem);
        return  NULL;
    }

    
    // csv_print_head(csv);
    // for(size_t i = 0; i < temp_file_size; i++){
        //     printf("%c", csv_mem[i]);
        // }
    // printf("%zu\n", csv->numrow);
    csv_parse(csv, csv_mem);
    // printf("string: ");
    csv_parse_with_types(csv);
    free(csv_mem);
    // sv_print((string_view *)csv->data[0]);
    return csv;
}

void print_type(csv_type t){
    // str_,
    // float_,
    // int_,
    // boolean_
    switch ((int)t) {
        case string_:
            fwrite("string\n", 1, 7, stdout);
            break;
        case float64_:
            fwrite("float\n", 1, 6, stdout);
            break;
        case int64_:
            fwrite("integer\n", 1, 8, stdout);
            break;
        case boolean_:
            fwrite("boolean\n", 1, 8, stdout);
            break;
        default:
            fwrite("Uknown type\n", 1, 12, stdout);
            break;
    }
}

void csv_to_memory(u8 *mem, FILE *file, size_t size, size_t *numcolumn, size_t *numrows){
    u8 is_next_line = 0;
    for(size_t i = 0; i < size; i++){
        u8 ch = fgetc(file);
        if(ch == '\r'){
            ch = fgetc(file);
            size--;
        }
        if(is_next_line == 0){
            if(ch == ',' || ch == '\n') (*numcolumn)++;
        }
        if(ch == '\n'){
            (*numrows)++;
            is_next_line = 1;
        }
        *mem = ch;
        mem++;
    }
    (*numrows)--;
    if(*(--mem) != '\n' && *mem != '\n')(*numrows)++; 
    // printf("%c %d\n", *(mem), *(mem));
}

CSV *create_csv(){
    CSV *csv = malloc(sizeof(CSV));
    if(csv == NULL) return NULL;
    csv->gl_arena = create_ArenaList(MiB(250));
    if(csv->gl_arena == NULL) return NULL;
    if(csv->gl_arena->arena.memory == NULL) return NULL;
    csv->gl_arena->next = NULL;
    return csv;
}

int csv_parse_head(CSV *csv, u8 *mem){
    csv->head = arenaList_Alloc(&csv->gl_arena, sizeof(sv) * csv->numcols);
    printf("numcolumn = %zu | numrow = %zu\n", csv->numcols, csv->numrows);
    assert(csv->numrows > 0);
    csv_parse_row(csv->gl_arena, csv->head, NULL,mem);
    // string_println(csv->head[i]);
    return 0;
}

void csv_print_head(const CSV *csv){
    fwrite("[ ", 1, 2, stdout);
    for(size_t i = 0; i < csv->numcols; i++){
        sv_print(csv->head[i]);
        if(i < csv->numcols - 1)
            // write(1, ", ", 2);
            fwrite(", ", 1, 2, stdout);

    }
    // fwrite(1, " ]\n", 3);
    fwrite(" ]\n", 1, 3, stdout);
}

void csv_free(CSV *csv){
    arenaList_free(csv->gl_arena);
    free(csv);
}

u8 *csv_parse_row(ArenaList *arena, string_view *csv_row, csv_type *csv_types, u8 *mem){
    size_t count = 0;
    size_t current_column = 0;
    u8 is_quotes = false; // checking if test in quotations to not split using the ,
    size_t i = 0;
    for(; mem[i] != '\n' && mem[i] != 0; i++){
        if(mem[i] == '"' && is_quotes){
            is_quotes = false;
        }else if(mem[i] == '"'){
            is_quotes = true;
        }
        if(mem[i] == ',' && !is_quotes){
            // printf("%c %zu %c\n" , mem[i], count, mem[i - count]);
            // csv->head[current_column] = arenaList_Alloc(arenas, sizeof(string));
            csv_row[current_column].str = arenaList_Alloc(&arena, count);
            csv_row[current_column].len = count;
            memcpy(csv_row[current_column].str, &mem[i - count], count);
            // checking types
            if(csv_types != NULL){
                if(csv_types[current_column] != string_)
                    csv_types[current_column] = get_type(&csv_row[current_column], csv_types[current_column]);
            }
            // string_print(csv_row[current_column]);
            // printf("len: %zu\n", csv_row[current_column].len );
            count = 0;
            current_column++;
        }else{
            count++;
        }
    }
    // add the last column that's before the \n
    csv_row[current_column].str = arenaList_Alloc(&arena, count);
    csv_row[current_column].len = count;
    memcpy(csv_row[current_column].str, &mem[i - count], count);
    // checking types
    if(csv_types != NULL){
        if(csv_types[current_column] != string_)
            csv_types[current_column] = get_type(&csv_row[current_column], csv_types[current_column]);
    }
    i++;
    return &mem[i];
}

csv_type get_type(string_view *sv, csv_type t){
    if(t == float64_){
        if(sv_to_float64(sv, NULL)){
            return float64_;
        }
        return string_; 
    }else{
        if(sv_to_int64(sv, NULL)){
            return int64_;
        }else if(sv_to_float64(sv, NULL)){
            return float64_;
        }
        return string_;  
    }
}

int csv_parse(CSV *csv, u8 *mem){
    // skip the head the first row that has only the names
    while(*(mem++) != '\n');

    // adding types for now we parse all the rows as strings
    csv->types = arenaList_Alloc(&csv->gl_arena, sizeof(csv_type) * csv->numcols);// fixed a bug allocating sizeof(csv_type *) instead of sizeof(csv_type) 
    // for(size_t i = 0; i < csv->numrows; i++){
    //     csv->types[i] = string_;
    // }

    // reading the data
    csv->data = arenaList_Alloc(&csv->gl_arena, sizeof(sv **) * csv->numrows);
    for(size_t i = 0; i < csv->numrows; i++){
        csv->data[i] = arenaList_Alloc(&csv->gl_arena, sizeof(sv) * csv->numcols);
        mem = csv_parse_row(csv->gl_arena, csv->data[i], csv->types, mem);
    }
    
    return 0;
}

void csv_print_row(const void *row, csv_type *row_types, size_t numcolumns){
    // fwrite(1, "[ ", 2);
    fwrite("[ ", 1, 2, stdout);
    for(size_t i = 0; i < numcolumns; i++){
        switch ((i64)row_types[i]) {
            case string_:
                sv_print(((string_view*)row)[i]);
                break;
            case int64_: {
                printf("%ld", ((i64*)row)[i]);
                break;
            }
            case float64_ : {
                printf("%g", ((f64*)row)[i]);
                break;
            }
            default:
                fwrite("ERROR TYPE PARSE FAIL", 1, 21, stdout);
                break;
        }
        if(i < numcolumns - 1)
            // write(1, ", ", 2);
            fwrite(", ", 1, 2, stdout);

    }
    // fwrite(1, " ]\n", 3);
    fwrite(" ]\n", 1, 3, stdout);
}

void csv_print_types(const CSV *csv){
    fwrite("[ ", 1, 2, stdout);
    for(size_t i = 0; i < csv->numcols; i++){
        switch ((int)csv->types[i]) {
            case string_:
                sv_print(csv->head[i]);
                fwrite(": string", 1, 8, stdout);
                break;
            case float64_:
                sv_print(csv->head[i]);
                fwrite(": float64", 1, 9, stdout);
                break;
            case int64_:
                sv_print(csv->head[i]);
                fwrite(": int64", 1, 7, stdout);
                break;
            case boolean_:
                sv_print(csv->head[i]);
                fwrite(": bool", 1, 6, stdout);
                break;
            default:
                sv_print(csv->head[i]);
                fwrite("NULL", 1, 4, stdout);
                break;
        }
        if(i < csv->numcols - 1)
            // write(1, ", ", 2);
            fwrite(", ", 1, 2, stdout);

    }
    // fwrite(1, " ]\n", 3);
    fwrite(" ]\n", 1, 3, stdout);
}

void csv_print_column_from_string(const CSV *csv, string_view column_name){

    ssize_t col_index = csv_get_column_index(csv, column_name);
    
    if(col_index == -1){
        fprintf(stderr, "column not found\n");
        return;
    }
    // write(1, "[ ", 2);
    fwrite("[ ", 1, 2, stdout);
    for(size_t i = 0; i < csv->numrows; i++){
        sv_print(((string_view **)csv->data)[i][col_index]);
        if(i < csv->numrows - 1)
            // write(1, ", ", 2);
            fwrite(", ", 1, 2, stdout);
    }
    // write(1, " ]\n", 3);
    fwrite(" ]\n", 1, 3, stdout);
}

ssize_t csv_get_column_index(const CSV *csv, string_view name){
    for(size_t i = 0; i < csv->numcols; i++){
        if(sv_cmp(&name, &csv->head[i])){
            return i;
        }
    }

    return -1;
}

// for now this only writes strings no type inference
void csv_write_file(const char *filename, const CSV *csv){
    assert(csv != NULL && filename != NULL);
    FILE *f = fopen(filename, "wb");
    for(size_t i = 0; i < csv->numcols; i++){
        fwrite(csv->head[i].str, 1, csv->head[i].len,f);
        if(i < csv->numcols - 1)
            fwrite(",", 1, 1,f);
    }
    fwrite("\n", 1, 1, f);
    for(size_t i = 0; i < csv->numrows; i++){

        for(size_t j = 0; j < csv->numcols; j++){
            switch ((int)csv->types[j]) {
                case string_:
                    fwrite(((sv **)csv->data)[i][j].str, 1, ((sv **)csv->data)[i][j].len,f);
                    break;
                case float64_:
                    fprintf(f, "%g", ((f64**)csv->data)[i][j]);
                    break;
                case int64_:
                    fprintf(f, "%ld", ((i64**)csv->data)[i][j]);
                    break;
                case boolean_:
                    fprintf(f, "%s", ((int**)csv->data)[i][j] ? "True" : "False");
                    break;
                default:
                    fwrite("Uknown type", 1, 12, stdout);
                    break;
            }
            if(j < csv->numcols - 1)
                fwrite(",", 1, 1,f);
        }

        fwrite("\n", 1, 1, f);
    }
}

// TODO : FINISH PARSING 
void csv_parse_with_types(CSV *csv){
    for(size_t i = 0; i < csv->numrows; i++){
        for(size_t j = 0; j < csv->numcols; j++){
            string_view sv = ((string_view **)csv->data)[i][j];
            switch ((i64)csv->types[j]) {
                case string_:
                    // printf("string : ");
                    // sv_println(&sv);
                    break;
                case int64_: {
                    sv_to_int64(&sv, &((i64**)csv->data)[i][j]);
                    // printf("integer : %ld\n", ((i64**)csv->data)[i][j]);
                    break;
                }
                case float64_ : {
                    sv_to_float64(&sv, &((f64**)csv->data)[i][j]);
                    // printf("float : %lf\n", ((f64**)csv->data)[i][j]);
                    break;
                }
                case boolean_:
                    fwrite("boolean\n", 1, 8, stdout);
                    break;
                default:
                    fwrite("Uknown type\n", 1, 12, stdout);
                    break;
            }
        }
    }
}


// TODO : FIX THE FORMATTING AND USE LESS CODE
i32 csv_write_json(const CSV *csv, const char *filename){
    assert(csv != NULL && filename != NULL);
    FILE *f = fopen(filename, "wb");
    fprintf(f, "[");
    fwrite("\n", 1, 1, f);
    for(size_t i = 0; i < csv->numrows; i++){
        fwrite("\t{\n", 1, 3,f);
        for(size_t j = 0; j < csv->numcols; j++){
            switch ((int)csv->types[j]) {
                case string_:
                    fwrite("\t\t", 1, 2, f);
                    sv_write_j(&csv->head[j], f);
                    fwrite(": ", 1, 2,f);
                    // fwrite(((sv **)csv->data)[i][j].str, 1, ((sv **)csv->data)[i][j].len,f);
                    sv_write_j(&((sv **)csv->data)[i][j], f);
                    break;
                case float64_:
                    fwrite("\t\t", 1, 2, f);
                    sv_write_j(&csv->head[j], f);
                    fprintf(f, ": %g", ((f64**)csv->data)[i][j]);
                    break;
                case int64_:
                    fwrite("\t\t", 1, 2, f);
                    sv_write_j(&csv->head[j], f);
                    fprintf(f, ": %ld", ((i64**)csv->data)[i][j]);
                    break;
                case boolean_:
                    fprintf(f, "%s", ((int**)csv->data)[i][j] ? "True" : "False");
                    break;
                default:
                    fwrite("Uknown type", 1, 12, f);
                    break;
            }
            if(j < csv->numcols - 1)
                fwrite(",\n", 1, 2, f);
        }
        fwrite("\n\t}", 1, 3,f);
        if(i < csv->numrows - 1)
            fwrite(",\n", 1, 2, f);
    }
    fwrite("\n]", 1, 2, f);
    return 1;
}

void sv_write_j(const string_view *sv, FILE *f){
    if(sv->str[0] == '"'){
        sv_fwrite(*sv, f);
        return;
    }
    fwrite("\"", 1, 1,f);
    sv_fwrite(*sv, f);
    fwrite("\"", 1, 1,f);
}

// TODO : TESTING
i64 csv_get_int_by_name(const CSV *csv, size_t row, string_view col_name){
    assert(csv != NULL);
    assert(col_name.str != NULL && col_name.len > 0);
    ssize_t col_index = csv_get_column_index(csv, col_name);
    if(col_index == -1){
        fprintf(stderr, "column not found!\n");
        return 0;
    }
    return *((i64 *)csv_get_cell(csv, row, col_index));
}

f64 csv_get_float_by_name(const CSV *csv, size_t row, string_view col_name){
    assert(csv != NULL);
    assert(col_name.str != NULL && col_name.len > 0);
    ssize_t col_index = csv_get_column_index(csv, col_name);
    if(col_index == -1){
        fprintf(stderr, "column not found!\n");
        return 0.0;
    }
    return *((f64 *)csv_get_cell(csv, row, col_index));
}

string_view csv_get_sv_by_name(const CSV *csv, size_t row, string_view col_name){
    assert(csv != NULL);
    assert(col_name.str != NULL && col_name.len > 0);
    ssize_t col_index = csv_get_column_index(csv, col_name);
    if(col_index == -1){
        fprintf(stderr, "column not found!\n");
        return (sv){NULL, 0};
    }
    return *((sv *)csv_get_cell(csv, row, col_index));
}

void *csv_get_cell(const CSV *csv, size_t row, size_t col){
    return &((void **)csv->data[row])[col];
}


// TODO: TEST THESE FUNCTIONS 
size_t csv_row_count(const CSV *csv){
    assert(csv != NULL);
    return csv->numrows;
}

size_t csv_column_count(const CSV *csv){
    assert(csv != NULL);
    return csv->numcols;
}

string_view csv_column_name(const CSV *csv, size_t column){
    assert(csv != NULL);
    if(column >= csv->numcols){
        fprintf(stderr, "column not found!\n");
        return (sv){.str = NULL, .len = 0};
    }
    return csv->head[column];
}

// TODO: TEST THESE STATISTIC FUNCTIONS :
// STATISTIC FUNCTIONS 
int64_t csv_column_sum_int(const CSV* csv, size_t col_index){
    assert(col_index < csv->numcols);
    if(csv->types[col_index] == float64_){
        i64 temp = 0;
        f64 ** col = (f64**)csv->data;
        for(size_t i = 0; i < csv->numrows; i++){
            temp+= col[i][col_index];
        }
        return temp;
    }
    else if(csv->types[col_index] == int64_){
        i64 temp = 0;
        i64 ** col = (i64**)csv->data;
        for(size_t i = 0; i < csv->numrows; i++){
            temp+= col[i][col_index];
        }
        return temp;
    }
    fprintf(stderr, "column not of type float or int\n");
    return 0;
}

f64 csv_column_sum_float(const CSV* csv, size_t col_index){
    assert(col_index < csv->numcols);
    if(csv->types[col_index] == float64_){
        f64 temp = 0.0;
        f64 ** col = (f64**)csv->data;
        for(size_t i = 0; i < csv->numrows; i++){
            temp+= col[i][col_index];
        }
        return temp;
    }
    else if(csv->types[col_index] == int64_){
        i64 temp = 0;
        i64 ** col = (i64**)csv->data;
        for(size_t i = 0; i < csv->numrows; i++){
            temp+= col[i][col_index];
        }
        return temp;
    }
    fprintf(stderr, "column not of type float or int\n");
    return 0.0;
}

f64 csv_column_mean(const CSV* csv, size_t col_index){
    f64 tmp = csv_column_sum_float(csv, col_index);
    return tmp / (f64)csv->numrows;
}

f64 csv_column_min(const CSV* csv, size_t col_index){
    if(csv->types[col_index] == float64_){
        f64 **col = ((f64**)csv->data);
        f64 min = col[0][col_index];
        for(size_t i = 1; i < csv->numrows; i++){
            if(min > col[i][col_index]){
                min = col[i][col_index];
            }
        }
        return min;
    }
    else if(csv->types[col_index] == int64_){
        i64 **col = ((i64**)csv->data);
        f64 min = (f64)col[0][col_index];
        for(size_t i = 1; i < csv->numrows; i++){
            if(min > (f64)col[i][col_index]){
                min = (f64)col[i][col_index];
            }
        }
        return min;
    }
    fprintf(stderr, "column not of type float or int\n");
    return 0.0;
}

f64 csv_column_max(const CSV* csv, size_t col_index){
    if(csv->types[col_index] == float64_){
        f64 **col = ((f64**)csv->data);
        f64 max = col[0][col_index];
        for(size_t i = 1; i < csv->numrows; i++){
            if(max < col[i][col_index]){
                max = col[i][col_index];
            }
        }
        return max;
    }
    else if(csv->types[col_index] == int64_){
        i64 **col = ((i64**)csv->data);
        f64 max = (f64)col[0][col_index];
        for(size_t i = 1; i < csv->numrows; i++){
            if(max < (f64)col[i][col_index]){
                max = (f64)col[i][col_index];
            }
        }
        return max;
    }
    fprintf(stderr, "column not of type float or int\n");
    return 0.0;
}

// f64 csv_sum_column(CSV *csv, string_view column_name){
//     ssize_t index = csv_get_column_index(csv, column_name);
//     if(index < 0){
//         fprintf(stderr, "column not found\n");
//         return 0.0;
//     }
//     if(csv->types[index] == float64_){
//         f64 temp = 0.0;
//         f64 ** col = (f64**)csv->data;
//         for(size_t i = 0; i < csv->numrows; i++){
//             temp+= col[i][index];
//         }
//         return temp;
//     }
//     else if(csv->types[index] == int64_){
//         i64 temp = 0;
//         i64 ** col = (i64**)csv->data;
//         for(size_t i = 0; i < csv->numrows; i++){
//             temp+= col[i][index];
//         }
//         return temp;
//     }
//     fprintf(stderr, "column can't be summed\n");
//     return 0.0;
// }
