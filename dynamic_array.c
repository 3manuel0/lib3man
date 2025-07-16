#include "lib3man.h"
#include <stddef.h>

dynamic_arr newArr(size_t capacity){
    int * arr = malloc(capacity * sizeof(int));
    if (arr == NULL) return (dynamic_arr) {
        .data = NULL,
        .size = 0,
        .capacity = 0
    };
    return (dynamic_arr){
        .data = arr,
        .size = 0,
        .capacity = capacity,
    };
}

void ArrPush(dynamic_arr * arr, int n){
    if(arr->size < arr->capacity){
        arr->data[arr->size] = n;
        arr->size++;
    }else{
        if(arr->capacity == 0) arr->capacity = 5;
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(int));
        arr->data[arr->size] = n;
        arr->size++;
    }
}

void freeArr(dynamic_arr * arr){
    free(arr->data);
    arr->capacity = 0;
    arr->size = 0;
}
// int main(){
//     dynamic_arr arr = newArr(1);
//     if(arr.data == NULL){
//         perror("malloc failed\n");
//         exit(-1);
//     }
//     ArrPush(&arr, 555);
//     ArrPush(&arr, 16);
//     ArrPush(&arr, 3);
//     ArrPush(&arr, 4);
//     printf("capacity : %ld, size: %ld\n", arr.capacity, arr.size);
//     ArrPush(&arr, 5);
//     ArrPush(&arr, 6);
//     ArrPush(&arr, 7);
//     ArrPush(&arr, 8);
//     ArrPush(&arr, 9);
//     ArrPush(&arr, 10);
//     ArrPush(&arr, 11);
//     printf("capacity : %ld, size: %ld\n", arr.capacity, arr.size);
//     for(int i = 0; i < arr.size; i++){
//         printf("%d\n", arr.data[i]);
//     }
//     system("pwd");
//     return 0;
// }