#include "pr3intf.h"
#include <stddef.h>
#include <stdio.h>

// the main is temporary
int main(){
    // tsting
    // putint_signed_long(-9223372036854775807LL);
    printf("\n%zu\n", putint_signed(185261));
    //pr3intf("\npr3intf testing testing %x %lu %ld anger builds %s\n", 999, -1, -9223372036854775807LL,"The universe breaks everyone");
    //printf("printf testing testing %x %lu %ld anger builds %s\n", 999, (long)-1,-9223372036854775807LL,"The universe breaks everyone");
    // printf("pr3intf %zu printf %zu\n", pr3intf("\npr3intf testing testing %x  %d anger builds %s\n", 999, -1,"The universe breaks everyone"), printf("\npr3intf testing testing %x  %d anger builds %s\n", 999, -1, "The universe breaks everyone"));
    return 0;
}

size_t pr3intf(char * str, ...){
    va_list args;
    va_start(args, str);
    size_t length;
    while(*str){
        if(*str == '%'){
            switch ((char)*(++str)) {
                case 'u':
                    length += putint_unsigned(va_arg(args, int));
                    break;
                case 'd':
                case 'i':
                case 'D':
                    length += putint_signed(va_arg(args, int));
                    break;
                case 'x':
                    length += putint_hex(va_arg(args, int));
                    break;
                case 's':
                case 'S':
                    length += putstr(va_arg(args, char *));
                    break;
                case 'l':
                    switch ((char)*(++str)) {
                        case 'u':
                            length += putint_unsigned_long(va_arg(args, unsigned long int));
                            break;
                        case 'd':
                        case 'i':
                        case 'D':
                            length += putint_signed_long(va_arg(args, long int));
                            break;
                        case 'x':
                            // length += putint_hex_long(va_arg(args, unsigned long int));
                            break;
                    }
                    break;
                default:
                    write(1, str, 1);
                    length++;
            }
        }else{
            write(1, str, 1);
            length++;
        }
        str++;
    }
    va_end(args);
    // return length;
    // fix the problem with the length
    return 0;
}


size_t man_strlen(char * str){
    int len = 0;
    while(str[len]){
        len ++;
    }
    return len;
}

size_t putint_signed(int nb){
    char nums[11];
    int i = 0;
    int length;
    if(nb == -2147483648){
        write(1, "-2147483648", 11);
        return 11;
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
    length = i;
    while(i--){
        write(1, &nums[i], 1);
    }
    return length;
}

size_t putint_signed_long(signed long nb){
    char nums[20];
    int i = 0;
    int length;
    signed long min_long = (-9223372036854775807LL - 1);
    if(nb == min_long){
        write(1, "-9223372036854775808", 20);
        return 20;
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
    length = i;
    while(i--){
        write(1, &nums[i], 1);
    }
    return length;
}

size_t putint_unsigned_long(unsigned long int nb){
    char nums[20];
    int i = 0;
    int length;
    if(nb == 0){
        write(1, "0", 1);
    }

    while(nb){
        nums[i] = '0' + (nb % 10);
        nb /= 10;
        i++;
    }
    length = i;
    while(i--){
        write(1, &nums[i], 1);
    }
    return length;
}

size_t putstr(char *a){
    int i = 0;
    while(a[i]){
        write(1, &a[i], 1);
        i++;
    }
    return i;
}

size_t putint_hex(unsigned int nb){
    int i = 0;
    int length;
    char nums[8];
    if(nb == 0){
        write(1, "0", 1);
    }

    while(nb){
        nums[i] = "0123456789abcdef"[nb % 16];
        nb /= 16;
        i++;
    }
    length = i;
    while(i--){
        write(1, &nums[i], 1);
    }
    return length;
}


size_t putint_unsigned(unsigned int nb){
    char nums[11];
    int i = 0;
    int length;
    if(nb == 0){
        write(1, "0", 1);
    }

    while(nb){
        nums[i] = '0' + (nb % 10);
        nb /= 10;
        i++;
    }
    length = i;
    while(i--){
        write(1, &nums[i], 1);
    }
    return length;
}