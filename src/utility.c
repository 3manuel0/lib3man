#include "../includes/lib3man.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>


u32 u32_entropy_random(void) {
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

f64 f64_random_range(f64 min, f64 max){
    f64 u = (f64)u32_entropy_random() / (f64)UINT32_MAX;
    return min + u * (max - min);
}

f32 f32_random_range(f32 min, f32 max){
    f32 u = (f32)u32_entropy_random() / (f32)UINT32_MAX;
    return min + u * (max - min);
}

u32 u32_random_range(u32 min, u32 max){
    return min + (u32_entropy_random() % (max - min + 1));
}


u16 u16_bswap(u16 x){

    #if defined(__GNUC__) || defined(__clang__)// clang or gcc
        return __builtin_bswap16(x);

    #elif defined(_MSC_VER)//msvc

        return _byteswap_ushort(x);

    #else
        return (((x << 8) & 0xff00) | ((x >> 8) & 0x00ff));
    #endif
}

u32 u32_bswap(u32 x){

    #if defined(__GNUC__) || defined(__clang__)// clang or gcc
    
        return __builtin_bswap32(x);

    #elif defined(_MSC_VER)//msvc

        return _byteswap_ulong(x);

    #else
        return (((x << 24) & 0xff000000) | 
        ((x << 8) & 0x00ff0000) | 
        ((x >> 8) & 0x0000ff00) | 
        ((x >> 24) & 0x000000ff));
    #endif
}

u64 u64_bswap(u64 x){

    #if defined(__GNUC__) || defined(__clang__)// clang or gcc

        return __builtin_bswap64(x);

    #elif defined(_MSC_VER)//msvc

        return _byteswap_uint64(x);

    #else
        return  ((x << 56) & 0xff00000000000000ULL) |
                ((x << 40) & 0x00ff000000000000ULL) |
                ((x << 24) & 0x0000ff0000000000ULL) |
                ((x << 8)  & 0x000000ff00000000ULL) |
                ((x >> 8)  & 0x00000000ff000000ULL) |
                ((x >> 24) & 0x0000000000ff0000ULL) |
                ((x >> 40) & 0x000000000000ff00ULL) |
                ((x >> 56) & 0x00000000000000ffULL);
    #endif
}

// TODO: TESTS FOR THIS FUNCTION
Buffer buffer_read_file(const char *file){
    assert(file != NULL);
    Buffer buff = {0};
    FILE * f = fopen(file, "rb");
    
    if(f == NULL){
        printf("FAILED TO OPEN THE FILE : %s\n", file);
        return buff;
    }

    fseek(f, 0, SEEK_END);
    buff.size = ftell(f);

    if(buff.size == 0){
        printf("FILE IS EMPTY : %s\n", file);
        return buff;
    } 

    fseek(f, 0, SEEK_SET);
    buff.buf = malloc(buff.size);

    if(buff.buf == NULL){
        printf("BUFFER ALLOCATION FAILED WHEN READING : %s\n", file);
        return buff;
    } 

    size_t len = fread(buff.buf, sizeof(u8), buff.size, f);
    if(len != buff.size){
        printf("FAILED TO READ THE FILE : %s\n", file);
        free(buff.buf);
        return (Buffer){NULL, 0, 0};
    }
    buff.offset = 0;
    printf("%zu\n", buff.size);
    fclose(f);
    return buff;
}

void * buffer_get_next_sz(Buffer buffer, size_t size){
    assert(buffer.buf != NULL && buffer.size > 0);

    assert(buffer.offset + size < buffer.size 
        && "Out of Buffer's range.\n");
    
    void * ptr = buffer.buf + buffer.offset;
    buffer.offset += size;

    return ptr;
}

void buffer_reset(Buffer * buffer){
    buffer->offset = 0;
}

ssize_t buffer_write_file(Buffer buffer, const char * file_name){
    assert(buffer.buf != NULL);
    FILE* f = fopen(file_name, "wb");

    if(f == NULL){
        printf("FAILED TO OPEN THE FILE : %s\n", file_name);
        return -1;
    }

    size_t len = fwrite(buffer.buf, sizeof(u8), buffer.size, f);

    if(len != buffer.size){
        printf("FAILED TO WRITE THE FILE : %s\n", file_name);
        return -1;
    }

    return len;
}

void buffer_free(Buffer *buffer){
    assert(buffer != NULL);
    assert(buffer->buf != NULL);
    free(buffer->buf);
    buffer->buf = NULL;
    buffer->size = 0;
}