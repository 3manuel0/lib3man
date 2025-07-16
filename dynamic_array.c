#include "lib3man.h"

dynamic_arr newArr(int cap){
    int * arr = malloc(cap * sizeof(int));
    if (arr == NULL) return (dynamic_arr) {
        .data = NULL,
        .size = 0,
        .capacity = 0
    };
    return (dynamic_arr){
        .data = arr,
        .size = 0,
        .capacity = cap,
    };
}

void ArrPush(dynamic_arr * darr, int n){
    if(darr->size < darr->capacity){
        darr->data[darr->size] = n;
        darr->size++;
    }else{
        darr->capacity *= 2;
        darr->data = realloc(darr->data, darr->capacity * sizeof(int));
        darr->data[darr->size] = n;
        darr->size++;
    }
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