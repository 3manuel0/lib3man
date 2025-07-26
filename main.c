#include "lib3man.h"
#include <stdio.h>
#include <stdlib.h>

void list_push(list * head, int value){
    list * current = head;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = malloc(sizeof(list));
    current = current->next;
    current->data = value;
    current->next = NULL;
}

void print_list(list * head){
    list * current = head;
    while(current != NULL){
        printf("%d\n", current->data);
        current = current->next;
    }
}

void free_list(list * head){
    while (head != NULL) {
        list * temp = head;
        head = head->next;
        free(temp);
    }
}

int main(){
    list * head = malloc(sizeof(list));
    head->data = 66;
    for(int i = 0; i < 99; i++){
        list_push(head, 10 * i);
    }
    print_list(head);
    free_list(head);
    printf("%d\n", head->data);
    return 0;
}