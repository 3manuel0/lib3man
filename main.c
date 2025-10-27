#include "lib3man.h"

int main(){
    str_t a = str_t_from_const("the universe breaks everyone");
    str_t b = str_t_from_const(" anger builds");
    str_t_append(&a, &b);
    str_t_print(a);
    str_t_free(&a);
    return 0;
}