#ifndef PR3INTF
#define PR3INTF

#include <unistd.h>
#include <stdarg.h>

void putint_signed(int nb);

void putint_unsigned(unsigned int nb);

void putfloat(float nb);


int man_strlen(char * str);

void putstr(char *a); 

int pr3intf(char * str, ...);


#endif