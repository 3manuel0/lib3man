#include "../includes/lib3man.h"

u32 u32_random(void) {
    u32 value = 0;

    #ifdef _WIN32
        // for windows 
        BCryptGenRandom(NULL,
                        (unsigned char*)&value,
                        sizeof(value),
                        BCRYPT_USE_SYSTEM_PREFERRED_RNG);

    #elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
        // for freebsd/macos 
        arc4random_buf(&value, sizeof(value));

    #else
        // for linux
        ssize_t result = getrandom(&value, sizeof(value), 0);

        // incase getrandom failed
        if (result < 0) {
            FILE *f = fopen("/dev/urandom", "rb");
            if (f) {
                fread(&value, sizeof(value), 1, f);
                fclose(f);
            }
        }
    #endif

    return value;
}

f64 random_range(f64 min, f64 max){
    f64 u = (f64)u32_random() / (f64)UINT32_MAX;
    return min + u * (max - min);
}