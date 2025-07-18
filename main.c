#include "lib3man.h"

CREATE_ARR(int ,int_d_arr);

int main(){

    int_d_arr arr = int_d_arr_new(10);
    if(arr.data == NULL){
        perror("malloc failed\n");
        exit(-1);
    }
    int_d_arr_push(&arr, 555);
    int_d_arr_push(&arr, 16);
    int_d_arr_push(&arr, 3);
    int_d_arr_push(&arr, 4);
    printf("capacity : %ld, size: %ld\n", arr.capacity, arr.size);
    int_d_arr_push(&arr, 5);
    int_d_arr_push(&arr, 6);
    int_d_arr_push(&arr, 7);
    int_d_arr_push(&arr, 8);
    int_d_arr_push(&arr, 9);
    int_d_arr_push(&arr, 10);
    int_d_arr_push(&arr, 11);
    printf("capacity : %ld, size: %ld\n", arr.capacity, arr.size);
    for(int i = 0; i < arr.size; i++){
        printf("%d\n", arr.data[i]);
    }
    return 0;
}