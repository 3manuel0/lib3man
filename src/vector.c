#include "../includes/lib3man.h"
#include <assert.h>


Vector vector_create_empty(size_t len){
    assert(len > 0);
    f64 *temp = calloc(len, sizeof(f64));
    if(temp == NULL){
        fprintf(stderr, "ALLOCATION FAILED!\n");
        return (Vector){.vec = NULL, .len = 0};
    }
    return (Vector){.vec = temp, .len =len};
}

Vector  vector_create(size_t len, f64 arr[len]){
    assert(arr != NULL);
    Vector v = vector_create_empty(len);
    if(v.vec == NULL){
        return v;
    }
    for(size_t i = 0; i < len; i++){
        printf("%lf %zu\n", arr[i], i);
        v.vec[i] = arr[i];
    }
    return v;
}

void vector_free(Vector *vec){
    assert(vec != NULL);
    assert(vec->vec != NULL); // to not free a NULL pointer
    free(vec->vec);
    vec->len = 0;
}