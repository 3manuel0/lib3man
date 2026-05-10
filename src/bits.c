#include "../includes/lib3man.h"


u16 u16_bswap(u16 x){
    return (((x << 8) & 0xff00) | ((x >> 8) & 0x00ff));
}
u32 u32_bswap(u32 x){
    return (((x << 24) & 0xff000000) | ((x << 8) & 0x00ff0000) | ((x >> 8) & 0x0000ff00) | ((x >> 24) & 0x000000ff));
}

// TODO
u64 u64_bswap(u64 x);