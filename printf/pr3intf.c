#include "pr3intf.h"
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>




int main(){
    // pr3intf("Hello world! %d\n", 55);
    putint_signed(-2147483648);
    write(1, "\n", 1);
    putint_signed(55);
    write(1, "\n", 1);
    putint_signed(2147483647);
    write(1, "\n", 1);
    putint_unsigned(2147483999);
    write(1, "\n", 1);

    // printf("%d", 0.5f == 0);
    pr3intf("testing testing %d anger builds %s\n", 9.555, "The universe breaks everyone");
    return 0;
}

int pr3intf(char * str, ...){
    char * buff;
    va_list args;
    va_start(args, str);
    while(*str){
        char a = *str;
        if(*str == '%'){
            switch ((char)*(++str)) {
                case 'd':
                case 'D':
                    // printf("int %d\n", va_arg(args, int));
                    putint_signed((int)va_arg(args, double));
                    break;
                case 's':
                case 'S':
                    putstr(va_arg(args, char *));
                    break;
                default:
                    write(1, str, 1);
            }
        }else
            write(1, &a, 1);
        str++;
    }
    va_end(args);
    return 0;
}


int man_strlen(char * str){
    int len = 0;
    while(str[len]){
        len ++;
    }
    return len;
}

void putint_signed(int nb){
    char nums[11];
    int i = 0;
    if(nb == -2147483648){
        write(1, "-2147483648", 11);
        return;
    }
    if(nb == 0){
        write(1, "0", 1);
    }
    if(nb < 0){
        nb = -nb;
        write(1, "-", 1);
    }
    while(nb){
        nums[i] = '0' + (nb % 10);
        nb /= 10;
        i++;
    }
    while(i--){
        write(1, &nums[i], 1);
    }
}

void putstr(char *a){
    int i = 0;
    while(a[i]){
        write(1, &a[i], 1);
        i++;
    }
}

void putint_unsigned(unsigned int nb){
    char nums[11];
    int i = 0;

    if(nb == 0){
        write(1, "0", 1);
    }

    while(nb){
        nums[i] = '0' + (nb % 10);
        nb /= 10;
        i++;
    }

    while(i--){
        write(1, &nums[i], 1);
    }
}