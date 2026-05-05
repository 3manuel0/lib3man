#include "../includes/lib3man.h"

Matrix matrix_create(size_t rows, size_t cols){
    assert(rows > 0 && cols > 0);
    f64 *temp = malloc(rows * cols * sizeof(f64));
    if(temp == NULL){
        fprintf(stderr, "MALLOC FAILED!\n");
        return (Matrix){.mtx = NULL, .cols = 0, .rows = 0};
    }
    return (Matrix){.mtx = temp, .cols = cols, .rows = rows};
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
    if(a->cols != b.rows || (a->cols & b.cols) > 0 || (a->rows & b.rows) > 0 ){
        printf("UNDIFINED BEHAVIOUR!\n");
        return;
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
    Matrix m = matrix_create(src.rows, src.cols);
    memcpy(m.mtx, src.mtx, src.rows * src.rows * sizeof(f64));
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