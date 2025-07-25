#include "pr3intf.h"

// the main is temporary
int main(){
    // tsting
    pr3intf("pr3intf testing testing %x anger builds %s\n", 999, "The universe breaks everyone");
    printf("printf testing testing %x anger builds %s\n", 999, "The universe breaks everyone");
    return 0;
}

int pr3intf(char * str, ...){
    va_list args;
    va_start(args, str);
    while(*str){
        if(*str == '%'){
            switch ((char)*(++str)) {
                case 'u':
                    putint_unsigned(va_arg(args, int));
                    break;
                case 'd':
                case 'i':
                case 'D':
                    putint_signed(va_arg(args, int));
                    break;
                case 'x':
                    putint_hex(va_arg(args, int));
                    break;
                case 's':
                case 'S':
                    putstr(va_arg(args, char *));
                    break;
                default:
                    write(1, str, 1);
            }
        }else
            write(1, str, 1);
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

void putint_hex(unsigned int nb){
    int i = 0;
    char nums[8];
    if(nb == 0){
        write(1, "0", 1);
    }

    while(nb){
        nums[i] = "0123456789abcdef"[nb % 16];
        nb /= 16;
        i++;
    }

    while(i--){
        write(1, &nums[i], 1);
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