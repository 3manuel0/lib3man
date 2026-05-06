#include "../includes/lib3man.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

Matrix matrix_create_empty(size_t rows, size_t cols){
    assert(rows > 0 && cols > 0);
    f64 *temp = calloc(rows * cols, sizeof(f64));
    if(temp == NULL){
        fprintf(stderr, "ALLOCATION FAILED!\n");
        return (Matrix){.mtx = NULL, .cols = 0, .rows = 0};
    }
    return (Matrix){.mtx = temp, .cols = cols, .rows = rows};
}

Matrix matrix_create(size_t rows, size_t cols, f64 arr[rows][cols]){
    assert(arr != NULL);
    Matrix m = matrix_create_empty(rows, cols);
    for(size_t i = 0; i < rows; i++){
        for(size_t j = 0; j < cols; j++){
            printf("%lf %zu\n", arr[i][j], (i * m.cols) + j);
            m.mtx[(i * m.cols) + j] = arr[i][j];
        }
    }
    return m;
}

int matrix_fill(Matrix *matrix, f64 value){
    assert(matrix != NULL);
    assert(matrix->mtx != NULL);
    for(size_t i = 0; i < matrix->cols * matrix->rows; i++){
        matrix->mtx[i] = value;
    }
    return true;
}

void matrix_randomize(Matrix* matrix, f64 min, f64 max){
    assert(matrix != NULL);
    assert(matrix->mtx != NULL);
    for(size_t i = 0; i < matrix->cols * matrix->rows; i++){
        f64 u = (f64)u32_entropy_random() / (f64)UINT32_MAX;
        matrix->mtx[i] = min + u * (max - min);
    }
}

void matrix_add(Matrix *a, Matrix b){
    assert(a != NULL);
    assert(a->mtx != NULL);
    if(a->cols != b.cols || b.rows != a->rows){
        printf("MATRIXES ARE NOT THE SAME DEMENSIONS\n");
        return;
    }
    for(size_t i = 0; i < a->cols * a->rows; i++){
        a->mtx[i] += b.mtx[i];
    }
}

void matrix_sub(Matrix *a, Matrix b){
    assert(a != NULL);
    assert(a->mtx != NULL);
    if(a->cols != b.cols || b.rows != a->rows){
        printf("MATRIXES ARE NOT THE SAME DEMENSIONS\n");
        return;
    }
    for(size_t i = 0; i < a->cols * a->rows; i++){
        a->mtx[i] -= b.mtx[i];
    }
}

void matrix_mul(Matrix *a, Matrix b){
    assert(a != NULL);
    assert(a->mtx != NULL && b.mtx != NULL);
    if(a->cols != b.rows){
        printf("UNDIFINED BEHAVIOUR!\n");
        return;
    }
    // Matrix temp = matrix_create_empty(a->rows, b.cols);

    for(size_t i = 0; i < a->cols*b.rows; i++){
        for(size_t j = 0 + (i * a->rows); j < a->rows + i; j++){
            printf("%lf\n", a->mtx[j]); 
        }
    }
    // TODO : ADD LOGIC 
}

void matrix_scale(Matrix *matrix, f64 k){
    assert(matrix != NULL);
    assert(matrix->mtx != NULL);
    for(size_t i = 0; i < matrix->cols * matrix->rows; i++)
        matrix->mtx[i] *= k;
}

Matrix matrix_copy(Matrix src){
    assert(src.mtx != NULL);
    Matrix m = matrix_create_empty(src.rows, src.cols);
    memcpy(m.mtx, src.mtx, src.rows * src.cols * sizeof(f64));// fixed a bug that corrupts the heap
    return m;
}

void matrix_map(Matrix *matrix, f64(*func)(f64)){
    assert(matrix != NULL);
    assert(matrix->mtx != NULL);
    for(size_t i = 0; i < matrix->cols * matrix->rows; i++)
        matrix->mtx[i] = func(matrix->mtx[i]);
}

void matrix_print(Matrix matrix){
    for(size_t i = 0; i < matrix.rows; i++){
        printf("[");
        for(size_t j = 0; j < matrix.cols; j++){
            printf(" %6.2lf ", matrix.mtx[(i * matrix.cols) + j]);
        }
        printf("]");
        printf("\n");
    }
}

void matrix_free(Matrix *matrix){
    assert(matrix != NULL);
    free(matrix->mtx);
    matrix->cols = 0;
    matrix->rows = 0;
}