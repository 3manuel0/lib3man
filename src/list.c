#include "../includes/lib3man.h"

void list_push(list * node, int value){
    list * head = node;
    while(head->next != NULL){
        head = head->next;
    }
    head->next = malloc(sizeof(list));
    head = head->next;
    head->data = value;
    head->next = NULL;
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