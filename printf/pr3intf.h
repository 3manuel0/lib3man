#ifndef PR3INTF
#define PR3INTF

#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>

size_t putint_signed(int nb);

size_t putint_unsigned(unsigned int nb);

size_t putint_signed_long(long int nb);

size_t putint_unsigned_long(size_t nb);

size_t putfloat(float nb);

size_t man_strlen(char * str);

size_t putint_hex(unsigned int nb);

size_t putint_hex_long(size_t nb);

size_t putstr(char *a); 

size_t pr3intf(char * str, ...);


#endif