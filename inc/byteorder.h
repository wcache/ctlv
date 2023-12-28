#ifndef __BYTEORDER__H__
#define __BYTEORDER__H__

#include "tlv_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BIG_ENDIAN_PLATFORM     1
#define LITTLE_ENDIAN_PLATFORM  0

#define TAG_LENGTH_ENDIAN BIG_ENDIAN_PLATFORM

#define BSWAP_32(x) \
    (tlv_uint32_t)((((tlv_uint32_t)(x) & 0xff000000) >> 24) | \
                    (((tlv_uint32_t)(x) & 0x00ff0000) >> 8) | \
                    (((tlv_uint32_t)(x) & 0x0000ff00) << 8) | \
                    (((tlv_uint32_t)(x) & 0x000000ff) << 24) \
             )  

#define BSWAP_16(x) (tlv_uint16_t)((((tlv_uint16_t)(x) & 0xff00) >> 8) | (((tlv_uint16_t)(x) & 0x00ff) << 8))  


#ifdef __cplusplus
}
#endif

#endif
